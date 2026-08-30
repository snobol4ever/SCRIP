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
    /* unary '+' (numeric context): an array numifies to its element count -- same
       array-name-table check rk_range_ex already uses for ^@grid. Non-array operands are
       already numeric in every corpus usage seen so far, so '+' on them is a pass-through
       identity rather than a general (unbuilt) numify -- minimal version this needs, not a
       general one, per this row's own standing discipline. */
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
/* ⭐ construct (f), raku-frontend-real-world-syntax-gaps: a bounded hand-rolled subscript-expression
   scanner for "@arr[expr]" interpolation, per the task's own catalog note that the alternative (a
   reentrant flex sub-parse) needed its reentrancy risk "not yet assessed" -- this avoids that risk
   entirely by never re-entering the lexer/parser. Deliberately narrow: a $name/@name/integer primary,
   optionally one +-*-/-% operator and a second primary -- measured against every "@name[...]" inside a
   "..." string across all 17 corpus/benchmarks/raku kernels (spinner's "$_ % @spinner" is the ONLY one),
   not guessed at a wider generality nothing in the corpus needs. */
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
  YYSYMBOL_VAR_ARRAY_TWIGIL = 15,          /* VAR_ARRAY_TWIGIL  */
  YYSYMBOL_VAR_HASH_TWIGIL = 16,           /* VAR_HASH_TWIGIL  */
  YYSYMBOL_CARET = 17,                     /* CARET  */
  YYSYMBOL_VAR_CAPTURE = 18,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 19,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 20,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 21,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 22,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 23,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 24,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 25,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 26,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 27,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 28,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 29,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 30,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 31,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 32,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 33,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 34,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 35,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 36,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 37,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 38,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 39,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 40,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 41,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 42,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 43,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 44,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 45,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 46,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 47,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 48,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 49,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 50,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 51,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 52,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 53,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 54,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 55,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 56,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 57,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 58,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 59,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 60,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 61,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 62,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 63,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 64,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 65,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 66,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 67,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 68,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 69,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 70,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 71,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 72,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 73,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 74,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 75,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 76,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 77,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 78,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 79,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 80,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 81,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 82,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 83,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 84,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 85,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 86,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 87,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 88,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 89,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 90,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 91,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 92,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 93,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 94,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 95,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 96,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 97,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 98,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 99,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 100,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 101,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 102,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 103,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 104,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 105,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 106,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 107,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 108,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 109,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 110,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 111,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 112,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 113,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 114,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 115,                   /* OP_POW  */
  YYSYMBOL_116_ = 116,                     /* '='  */
  YYSYMBOL_117_ = 117,                     /* '!'  */
  YYSYMBOL_118_ = 118,                     /* '<'  */
  YYSYMBOL_119_ = 119,                     /* '>'  */
  YYSYMBOL_120_ = 120,                     /* '|'  */
  YYSYMBOL_121_ = 121,                     /* '&'  */
  YYSYMBOL_122_ = 122,                     /* '~'  */
  YYSYMBOL_123_ = 123,                     /* '+'  */
  YYSYMBOL_124_ = 124,                     /* '-'  */
  YYSYMBOL_125_ = 125,                     /* '*'  */
  YYSYMBOL_126_ = 126,                     /* '/'  */
  YYSYMBOL_127_ = 127,                     /* '%'  */
  YYSYMBOL_UMINUS = 128,                   /* UMINUS  */
  YYSYMBOL_129_ = 129,                     /* '.'  */
  YYSYMBOL_130_ = 130,                     /* ';'  */
  YYSYMBOL_131_ = 131,                     /* '('  */
  YYSYMBOL_132_ = 132,                     /* ')'  */
  YYSYMBOL_133_ = 133,                     /* ','  */
  YYSYMBOL_134_ = 134,                     /* '['  */
  YYSYMBOL_135_ = 135,                     /* ']'  */
  YYSYMBOL_136_ = 136,                     /* '{'  */
  YYSYMBOL_137_ = 137,                     /* '}'  */
  YYSYMBOL_138_ = 138,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 139,                 /* $accept  */
  YYSYMBOL_program = 140,                  /* program  */
  YYSYMBOL_stmt_list = 141,                /* stmt_list  */
  YYSYMBOL_stmt = 142,                     /* stmt  */
  YYSYMBOL_if_stmt = 143,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 144,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 145,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 146,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 147,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 148,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 149,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 150,                /* loop_incr  */
  YYSYMBOL_for_stmt = 151,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 152,               /* given_stmt  */
  YYSYMBOL_when_list = 153,                /* when_list  */
  YYSYMBOL_sub_decl = 154,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 155,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 156,                 /* sub_body  */
  YYSYMBOL_method_body = 157,              /* method_body  */
  YYSYMBOL_class_decl = 158,               /* class_decl  */
  YYSYMBOL_role_decl = 159,                /* role_decl  */
  YYSYMBOL_is_clauses = 160,               /* is_clauses  */
  YYSYMBOL_class_body_list = 161,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 162,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 163,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 164,           /* named_arg_list  */
  YYSYMBOL_pair_list = 165,                /* pair_list  */
  YYSYMBOL_param_list = 166,               /* param_list  */
  YYSYMBOL_block = 167,                    /* block  */
  YYSYMBOL_closure = 168,                  /* closure  */
  YYSYMBOL_expr = 169,                     /* expr  */
  YYSYMBOL_tern_expr = 170,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 171,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 172,               /* divis_expr  */
  YYSYMBOL_jct_expr = 173,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 174,                 /* dor_expr  */
  YYSYMBOL_range_expr = 175,               /* range_expr  */
  YYSYMBOL_add_expr = 176,                 /* add_expr  */
  YYSYMBOL_repl_expr = 177,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 178,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 179,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 180,               /* unary_expr  */
  YYSYMBOL_pow_expr = 181,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 182,              /* scalar_list  */
  YYSYMBOL_meth_name = 183,                /* meth_name  */
  YYSYMBOL_postfix_expr = 184,             /* postfix_expr  */
  YYSYMBOL_call_expr = 185,                /* call_expr  */
  YYSYMBOL_arg_list = 186,                 /* arg_list  */
  YYSYMBOL_atom = 187                      /* atom  */
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
#define YYLAST   4600

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  465
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1174

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   371


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
       2,     2,     2,   117,     2,     2,     2,   127,   121,     2,
     131,   132,   125,   123,   133,   124,   129,   126,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   138,   130,
     118,   116,   119,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   134,     2,   135,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   136,   120,   137,   122,     2,     2,     2,
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
     115,   128
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   487,   487,   498,   499,   502,   504,   506,   508,   512,
     514,   516,   518,   520,   524,   528,   530,   534,   538,   540,
     542,   544,   546,   548,   552,   556,   558,   560,   562,   564,
     566,   568,   570,   572,   574,   582,   584,   586,   588,   590,
     592,   594,   597,   600,   602,   605,   608,   610,   614,   616,
     618,   621,   624,   626,   629,   632,   635,   637,   642,   646,
     650,   655,   659,   660,   662,   664,   666,   668,   671,   673,
     675,   677,   680,   683,   686,   689,   692,   694,   696,   698,
     700,   703,   705,   707,   709,   712,   715,   719,   722,   724,
     726,   728,   731,   734,   737,   740,   743,   746,   749,   750,
     751,   752,   753,   754,   755,   757,   759,   761,   763,   764,
     765,   766,   767,   768,   769,   771,   773,   775,   777,   778,
     779,   780,   783,   785,   787,   789,   791,   793,   795,   797,
     801,   803,   805,   807,   809,   811,   815,   817,   821,   823,
     825,   827,   831,   833,   837,   839,   841,   845,   847,   849,
     853,   856,   862,   868,   873,   879,   884,   889,   897,   908,
     909,   915,   923,   929,   935,   943,   949,   955,   964,   971,
     980,   989,   998,  1005,  1012,  1013,  1016,  1019,  1021,  1023,
    1025,  1027,  1029,  1031,  1033,  1036,  1038,  1040,  1042,  1045,
    1048,  1054,  1055,  1056,  1059,  1062,  1064,  1066,  1068,  1070,
    1072,  1074,  1076,  1079,  1081,  1083,  1085,  1088,  1091,  1097,
    1113,  1128,  1129,  1141,  1155,  1156,  1159,  1162,  1165,  1168,
    1171,  1174,  1177,  1180,  1183,  1186,  1189,  1192,  1199,  1206,
    1213,  1220,  1224,  1228,  1232,  1236,  1243,  1250,  1257,  1264,
    1274,  1282,  1290,  1299,  1306,  1313,  1323,  1331,  1341,  1356,
    1357,  1362,  1367,  1374,  1378,  1382,  1386,  1392,  1394,  1396,
    1398,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1426,  1427,  1428,  1430,  1432,  1434,  1436,
    1438,  1441,  1443,  1445,  1447,  1449,  1452,  1455,  1459,  1462,
    1465,  1468,  1470,  1474,  1477,  1480,  1483,  1486,  1489,  1491,
    1493,  1495,  1497,  1499,  1501,  1503,  1505,  1509,  1512,  1513,
    1514,  1515,  1516,  1517,  1518,  1519,  1524,  1529,  1532,  1534,
    1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,
    1547,  1548,  1549,  1550,  1551,  1557,  1563,  1569,  1572,  1573,
    1576,  1577,  1578,  1581,  1583,  1586,  1587,  1588,  1591,  1592,
    1595,  1596,  1597,  1600,  1601,  1602,  1605,  1606,  1607,  1608,
    1609,  1611,  1613,  1616,  1617,  1618,  1619,  1620,  1625,  1628,
    1629,  1632,  1633,  1636,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1649,  1651,  1657,  1662,  1663,
    1665,  1667,  1672,  1674,  1680,  1686,  1690,  1694,  1699,  1706,
    1711,  1717,  1723,  1730,  1737,  1742,  1747,  1754,  1759,  1764,
    1766,  1768,  1770,  1772,  1774,  1777,  1778,  1779,  1782,  1783,
    1784,  1785,  1794,  1795,  1796,  1797,  1798,  1799,  1800,  1801,
    1802,  1806,  1810,  1813,  1815,  1817,  1819,  1821,  1823,  1825,
    1827,  1829,  1831,  1832,  1836,  1840,  1844,  1845,  1847,  1849,
    1852,  1853,  1854,  1856,  1859,  1860
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
  "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET", "VAR_CAPTURE",
  "VAR_FH", "VAR_NAMED_CAPTURE", "KW_USE", "TESTOP", "KW_MY", "KW_SAY",
  "KW_PRINT", "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR",
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_EXIT", "KW_CONSTANT",
  "KW_ENUM", "KW_JOIN", "KW_GIVEN", "KW_WHEN", "KW_DEFAULT", "KW_WITH",
  "KW_WITHOUT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL",
  "KW_REPEAT", "KW_LOOP", "KW_LAST", "KW_NEXT", "KW_MAP", "KW_GREP",
  "KW_SORT", "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD",
  "KW_HAS", "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME",
  "OP_REDUCE", "ARR_ALL_SLICE", "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED",
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
  "scalar_methcall", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "divis_expr", "jct_expr", "dor_expr",
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

#define YYPACT_NINF (-768)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-465)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -768,     9,  1929,  -768,  -768,  -768,  -768,  -768,   842,   -37,
     -24,   -91,    27,  -768,  -768,  4415,  -768,  -768,  -768,    31,
    2004,   267,  3440,  3515,  3590,  3665,  3740,     2,    -4,  3740,
    2079,  2154,   216,   112,  3740,  3740,   130,   170,  3815,  3890,
      65,   -90,    23,    67,   139,   139,  3965,    65,    65,  3740,
     295,   317,   353,  4415,  -768,  -768,   325,   367,   369,  4415,
    4415,  4415,  -768,  2229,  2305,   315,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,   725,  -768,  -768,  -768,
    4471,   845,  -768,   818,   621,   -47,   303,  -768,   188,   320,
     355,   422,  -768,  -768,   308,   341,   345,   488,  -768,  -768,
    3740,  3740,  3740,  3740,  3740,  3740,   546,  2380,  4040,  2456,
     572,  3740,  3740,   446,    26,   433,   234,   456,    76,  -768,
      19,    65,  2531,  -768,  -768,   486,   569,    56,  -768,  2607,
    -768,   302,   107,   129,   222,   616,   631,   447,   652,  2531,
     676,  2531,    24,  2531,    65,  2531,    65,   -41,   215,   136,
    -768,  -768,  -768,   359,  -768,    70,  -768,   176,   577,   587,
     635,   583,   608,   117,   223,  2531,    65,  2531,    65,   357,
    2683,  -768,  3740,  3740,  -768,  3740,  3740,  -768,  3740,  3740,
    3740,  2758,  3740,  -768,   735,  -768,  -768,  -768,   672,   679,
      21,  -768,   691,  -768,  -768,  -768,  -768,  -768,  1003,  -768,
     407,   627,  -768,   386,   696,  1299,  3740,  3740,  3740,  3740,
    3740,  3740,  3740,  3740,  -768,  3740,  3740,  3740,  3740,  3740,
    3740,  3740,  3740,  -768,  4415,  4415,  4415,  4415,  4415,  4415,
    4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,   902,
    4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,
    4415,  4415,  4415,  4415,  4415,  4415,  4415,  4415,  1214,  1188,
     651,   717,   722,   759,   766,   769,   787,   -50,  -768,   749,
    2531,   466,   803,   467,   800,   805,   791,  -768,   809,   936,
     823,   249,  -768,   945,   797,   802,  2456,   947,  3740,  -768,
    3740,  3740,  3740,  3740,  3740,  3740,  3740,   839,   407,   806,
    -768,  3740,  3740,  -768,  4115,  -768,  4190,  -768,   271,   311,
     324,   427,   858,   866,  -768,   820,   830,  3740,  3740,  3740,
    3740,  3740,  3740,  3740,  -768,  3740,   835,  3740,  3740,  -768,
    3740,   844,   957,   854,  -768,   985,  3740,  -768,  4415,  4415,
      20,  -768,  -768,  -768,  3740,  3740,  3740,  -768,  3740,  3740,
    -768,  3740,  3740,   861,  3740,  -768,   979,  3740,   990,  3740,
     865,   972,   870,  -768,  3740,  3740,   997,   888,   927,   992,
    1011,  1019,   906,  -768,  -768,  -768,    65,    -3,  -768,    49,
     922,   928,  -768,  -768,  2833,   958,  1143,  2909,  -768,  -768,
    1275,   272,   278,  1046,  3440,  3515,  3740,  1644,    32,   236,
    -768,   373,  1040,  1041,  1042,  1054,  1055,  1056,  1057,  1058,
    1060,  1061,  1062,  1066,  1067,  1068,  1070,  1073,  1092,  1092,
    1092,  1092,  1092,  1092,  1109,  1092,  1092,  1092,  1092,  1092,
    1092,  1092,  1092,  -768,  -768,  -768,   -47,  -768,  -768,  -768,
    1086,  1086,   320,   355,   355,   422,   422,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  1078,  1197,  1083,  -768,
    2984,  -768,  -768,  -768,  -768,  -768,  -768,  3740,  3060,  -768,
     879,  -768,  3740,  3740,  3740,  3740,  1102,  1103,  1107,  -768,
    3136,  -768,   784,  3740,  1093,  -768,   -10,  -768,  1163,   595,
    1106,  1090,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    1101,  -768,  1113,  2531,   480,  1154,   252,  3212,   511,  1114,
    3740,  -768,  4265,  -768,  3740,  -768,    54,  -768,  3740,  3740,
    1121,  2833,  1115,  1116,  1119,  1120,  1123,  1124,  1125,   571,
    2833,  1126,  1127,   599,    65,    13,  4340,  -768,    65,    65,
     138,   161,   250,    -8,  -768,   154,  -768,  -768,  -768,  1133,
     881,  1434,   628,  1130,  1131,  1140,  1141,  1142,  1144,  -768,
    1118,   -23,  1156,  1136,  1157,  1145,    65,    65,    65,  1147,
    1148,  1165,  3740,  -768,  -768,  -768,  -768,  -768,  -768,  1249,
    1265,  -768,   197,  1133,   883,    61,   360,   -26,  -768,   891,
    3740,  -768,  -768,   688,  1269,  2456,  3740,  3740,   379,    58,
     221,  -768,   343,  3740,  3740,  -768,  3740,  3740,  -768,  3740,
    3740,  3740,  3740,  3740,  3740,  3740,  3740,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  4415,  3288,  -768,  1016,  1155,   905,
    1158,  -768,   907,  2833,   650,  1149,  1160,   726,  3740,  3740,
    3740,  -768,   909,  -768,   913,   917,  -768,  3740,  1209,  1276,
     919,  -768,  -768,  -768,  -768,  -768,   923,  -768,  3740,  3740,
    3740,   929,  -768,   259,  -768,  1162,  2531,   716,  1166,  1133,
     934,  1168,  1170,  3740,   939,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,   944,  -768,  -768,  -768,  1051,  -768,  -768,
    2531,    65,  -768,  -768,  1281,  -768,  1283,  1296,  1293,  3740,
    1192,  1316,  1317,  -768,  1133,   483,  3440,  3515,  1719,  -768,
     499,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  3740,    65,
    -768,  -768,  -768,  1210,  1211,  1312,  -768,  -768,  -768,  -768,
    3740,  1212,  -768,  -768,   248,   956,   829,  1300,  -768,  -768,
    1133,  1133,   948,   954,  1344,  1346,  1349,  -768,  -768,  1234,
    -768,   -49,   737,  1230,   270,  3740,  3740,  3740,  3740,  3740,
    3740,  -768,  -768,  -768,  -768,   294,   329,   330,   346,   371,
     376,   377,   387,   393,   406,   424,   425,  -768,  -768,   960,
    -768,   962,   964,  -768,  1238,  -768,  -768,   966,  -768,  -768,
    -768,  -768,  1239,  1240,  1242,  -768,  -768,  -768,  1241,  3740,
    1252,  -768,  2833,   724,  -768,  -768,  1254,  1322,  1323,  -768,
     968,  -768,  3740,  -768,  -768,  1133,  -768,  -768,   750,  1277,
    1278,    13,  -768,   970,  1084,    65,    65,    65,  -768,  -768,
    3740,  -768,  -768,  -768,    35,  -768,   165,  -768,  -768,  -768,
     604,    59,  -768,   442,  3740,  3740,  3740,  3740,  3740,  3740,
    3740,  3740,  -768,    65,  1268,  -768,  -768,    65,  1279,  3740,
    -768,   451,   698,    69,   326,   863,  1280,  1282,  1214,  -768,
    -768,  1133,  1133,  1404,  1406,  1407,  -768,  3740,  3364,  1301,
    1302,  -768,   436,   438,   440,   444,   454,   455,  -768,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768,  1285,  -768,  -768,  -768,  -768,  -768,
    3740,   981,  -768,  -768,  3740,  3740,  2833,   760,  -768,  -768,
    3740,  -768,  -768,  -768,  -768,    65,    65,  -768,  -768,  -768,
    -768,  -768,  1405,  3740,  1304,  1411,  1414,  3740,  3740,  3740,
    -768,  -768,  -768,   471,   495,   512,   523,   524,   529,   549,
     551,  -768,  -768,  -768,  3740,  1294,  -768,    66,  1350,  -768,
     173,  -768,  1413,  1417,  1298,  3740,  -768,  1418,  1420,  1311,
    3740,  -768,   339,   342,  1313,  1331,  -768,  -768,   704,  -768,
    -768,  -768,  -768,  -768,   552,  1305,   983,  3740,  3740,  -768,
    -768,  -768,  -768,  -768,  -768,  -768,  1330,  1345,  -768,  -768,
     988,  -768,   764,  1104,  -768,  -768,  -768,  3740,  -768,  -768,
     553,   557,   562,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  1347,    65,  1338,  1005,  1339,  1569,  1338,  1013,  1363,
    1364,  -768,  1365,  1368,  1369,  -768,  1372,  1489,  1490,  1375,
    3740,  -768,  1493,  1494,  1379,  3740,  -768,  -768,  -768,   244,
    -768,  -768,  -768,  1374,   590,   610,  -768,  -768,  1382,  -768,
      65,  -768,  -768,  -768,  -768,  -768,  3740,  -768,  -768,  1338,
    -768,  3440,  3515,  1794,  -768,   723,  -768,  1338,  -768,  -768,
    -768,  -768,  -768,  -768,  1383,  1384,  -768,  1385,  1386,  1387,
    -768,  1388,  1338,  1023,  -768,  -768,  -768,  -768,  -768,  1389,
    -768,   609,    99,  -768,   458,  3740,  3740,  3740,  3740,  3740,
    3740,  3740,  3740,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  1338,    65,  3740,  3740,  3740,  -768,  -768,  -768,
     620,   626,   649,   655,   675,   677,   681,   683,  -768,  -768,
     689,   694,   695,  -768,  -768,  -768,  -768,  -768,  -768,  -768,
    -768,  -768,  -768,  -768
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   428,   429,   430,   432,   433,   438,
     439,   453,   452,   454,   455,     0,   440,   441,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   431,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     3,     4,   100,   101,   108,
     109,   110,   111,   102,   103,   118,     0,   119,   120,   121,
     107,     0,   327,   329,   347,   349,   352,   354,   357,   359,
     362,   365,   372,   378,   380,   395,   424,     0,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   438,   439,   453,
     452,     0,     0,   464,   376,     0,   433,   438,    35,     0,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   357,     0,
     465,   326,   325,     0,    49,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,   147,     0,     0,   112,     0,     0,   113,     0,     0,
       0,     3,     0,   422,   104,   106,   419,   211,     0,     0,
       0,   377,     0,   434,   435,   375,   374,   373,   433,   460,
       0,     0,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,   321,   322,   323,   324,   318,     0,   402,     0,
       0,   319,     0,     0,     0,     0,     0,    39,   407,     0,
     406,   452,   398,     0,     0,     0,     0,     0,     0,    29,
       0,     0,     0,     0,     0,     0,     0,   460,     0,     0,
      38,   427,     0,     6,     0,    10,     0,    11,     0,     0,
       0,     0,     0,     0,   381,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,    43,
       0,     0,   125,     0,   137,     0,     0,   156,     0,     0,
       0,     3,   163,    46,     0,     0,     0,    48,     0,     0,
      52,     0,     0,     0,     0,   159,     0,     0,     0,     0,
       0,   140,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   420,   421,   423,     0,     0,   214,     0,
       0,     0,   249,   461,     0,     0,     0,     0,   457,   284,
     433,   438,   439,   453,     0,     0,     0,     0,     0,     0,
     283,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   333,
     336,   337,   330,   331,     0,   334,   335,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   348,   350,   351,   353,
     355,   356,   358,   360,   361,   363,   364,   369,   370,   371,
     366,   367,   368,   379,   383,   394,   387,   388,   389,   390,
     393,   391,   392,   385,   386,   384,   418,     0,   415,    59,
       0,    93,    94,    95,    96,    97,    56,     0,     0,   401,
       0,    15,     0,     0,     0,     0,   443,   448,   449,    61,
       0,   410,     0,     0,     0,   399,     0,   397,   427,     0,
       0,     0,   320,   321,   322,   323,   324,   318,   319,    37,
       0,   426,     0,     0,     0,   430,   452,     0,     0,     0,
       0,    26,     0,    27,     0,    28,     0,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,     0,     0,   128,   461,     0,
       0,   355,   356,   261,   264,     0,   277,   279,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
     396,     0,     0,     0,     0,     0,   461,     0,   461,     0,
       0,     0,     0,   114,   115,   116,   117,   317,   105,     0,
       0,   214,     0,     0,     0,     0,     0,     0,   462,     0,
       0,   382,   458,     0,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,   311,     0,     0,   312,     0,
       0,     0,     0,     0,     0,     0,     0,   285,    63,    65,
      67,    70,    68,    69,    64,    66,    76,    78,    80,    83,
      81,    82,    77,    79,     0,     0,   411,     0,     0,     0,
       0,   173,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   409,     0,   405,     0,     0,   253,     0,     0,     0,
       0,   443,   448,   449,    36,     5,     0,    12,     0,     0,
       0,     0,    19,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,    87,    86,    90,    88,
      89,    85,    41,     0,    91,    92,    44,   122,   127,   126,
       0,     0,   136,   153,     0,   155,     0,     0,     0,     0,
     266,     0,     0,   162,     0,     0,     0,     0,     0,   174,
       0,    47,    50,    51,    54,    55,    31,    30,     0,     0,
     157,   450,   451,     0,     0,   138,   141,   142,   145,   146,
       0,     0,   212,   213,     0,     0,     0,     0,   210,   171,
       0,     0,     0,     0,     0,     0,     0,   248,   463,     0,
     459,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   294,   301,   308,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   417,     0,
     414,     0,     0,    58,     0,    60,   172,     0,    16,   446,
     445,   444,     0,     0,     0,   408,   403,   404,     0,     0,
       0,   400,     0,     0,   258,   257,     0,     0,     0,    21,
       0,    22,     0,    25,   165,     0,    33,    32,     0,   463,
     463,     0,   129,     0,   130,     0,     0,     0,   262,   273,
       0,   267,   268,   161,   269,   265,     0,   278,   280,   282,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,     0,    74,    75,     0,     0,     0,
     209,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     168,     0,     0,     0,     0,     0,     9,     0,     0,   443,
     449,   305,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   286,   288,   290,   293,   291,   292,   287,   289,
     416,   413,   412,    57,   463,    71,    72,    73,   254,   255,
       0,     0,    13,    20,     0,     0,     0,     0,   164,     7,
       0,    42,    45,   124,   123,   461,     0,   134,   154,   151,
     152,   275,     0,     0,   270,     0,     0,     0,     0,     0,
     178,   179,   176,     0,     0,     0,     0,     0,     0,     0,
       0,   160,   158,   139,     0,     0,   150,     0,     3,   244,
       0,   241,     0,     0,     0,     0,   220,     0,     0,     0,
       0,   215,     0,     0,     0,     0,   216,   217,     0,   167,
     169,   250,   251,   252,     0,   173,     0,     0,     0,   295,
     297,   300,   298,   299,   296,    17,     0,   463,   260,   259,
       0,    23,     0,   131,   132,   263,   274,     0,   271,   272,
       0,     0,     0,   180,   182,   184,   187,   185,   186,   181,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   222,     0,     0,     0,     0,   221,   218,   219,     0,
     247,   304,   303,   172,     0,     0,   256,    14,   463,     8,
       0,   135,   276,   188,   190,   189,     0,   149,   243,     0,
     192,     0,     0,     0,   191,     0,   240,     0,   229,   230,
     224,   227,   228,   223,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,   302,   306,   307,    24,   133,     0,
     242,     0,     0,   195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,   239,   237,   238,   226,   235,   236,
     225,   246,     0,     0,     0,     0,     0,   196,   197,   194,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   148,
       0,     0,     0,   198,   200,   202,   205,   203,   204,   199,
     201,   206,   208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -768,  -768,     1,  -768,  -540,  -700,  -768,  -768,  -768,  -768,
    -768,   434,  1491,  -768,  -768,  -768,  -768,  -298,  -767,  -768,
    -768,  -768,   931,  -768,  -768,  -486,  1006,  -371,   119,  1112,
      -2,  -227,  -768,  1161,  1284,  -768,   682,   -11,  1286,   912,
     914,    -5,  -768,  1390,  -257,  -768,  -768,   253,  -768
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   205,    66,    67,   547,    68,    69,    70,    71,
      72,   975,    73,    74,   571,    75,    76,   342,   979,    77,
      78,   377,   592,    79,   597,   284,   518,   560,   123,   179,
     130,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,   201,   466,    94,    95,   131,    96
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      81,     2,   468,   424,   668,   708,   664,   842,   594,     3,
     124,   589,   670,   527,   718,   148,   149,   738,   739,   590,
     140,   142,   144,   146,   147,   112,    26,   153,   155,   157,
     553,   554,   161,   162,   555,   380,   166,   168,   113,    24,
     278,   170,   335,   279,   183,   125,    65,   186,   191,   172,
     327,   764,   765,   766,   195,   196,   197,   952,   613,   553,
     554,   200,   203,   555,   553,   554,   477,   897,   555,   173,
     328,   553,   554,   241,   242,   555,   553,   554,   614,   108,
     555,   478,   898,   982,   327,   327,   381,   280,   556,   557,
     558,   983,   336,   175,   110,    65,   345,   109,   261,   262,
     263,   264,   265,   266,   328,   328,   271,   273,   719,   275,
     276,   767,   111,   176,   740,   981,   346,   556,   557,   558,
     200,    80,   556,   557,   558,   327,   160,   298,   669,   556,
     557,   558,    65,   591,   556,   557,   558,   316,    65,   326,
     984,   331,   163,   333,   947,   328,   150,   152,   114,    65,
     115,   953,   559,   174,   329,   690,   114,   330,   115,   169,
     171,   801,   174,   360,   720,   362,   184,   185,   367,   615,
     368,   369,   296,   370,   371,   954,   372,   373,   374,   372,
     375,   593,   164,   553,   554,   985,   689,   555,   329,   329,
     286,   330,   330,   761,   287,   782,   961,   177,  1043,   986,
     347,    65,   348,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   288,   410,   411,   412,   413,   414,   415,   416,
     417,   714,   349,   302,   762,   763,   158,   721,   722,   329,
     159,  1070,   330,   440,   441,   356,  1148,   303,   955,   956,
     150,   556,   557,   558,   716,   304,   447,   448,   449,   450,
     451,   452,   453,   357,   553,   554,   755,   756,   555,   305,
     757,   723,   616,   332,   827,   334,   337,   340,   480,   244,
     245,   301,   341,   828,    65,   178,  1088,   132,   133,   134,
    1096,   135,   617,   246,   499,   361,   501,   363,   502,   503,
     504,   505,   506,   507,   508,   759,   338,   339,   136,   511,
     512,   943,   514,   137,   519,  1047,   350,   755,   756,   187,
     246,   757,   556,   557,   558,   532,   533,   534,   535,   536,
     537,   538,  1120,  1081,    80,   541,   542,   551,   552,   493,
    1134,   188,   680,   717,   758,    98,    99,   246,   306,   192,
     987,   358,   561,   563,   564,  1141,   565,   566,   988,   567,
     568,   343,   307,  1057,   344,   573,  1062,   575,   783,   359,
     269,  1058,   579,   580,  1063,   107,   177,   189,   285,   345,
     553,   554,   246,   618,   555,  1158,  1112,   193,   114,   194,
     115,   114,   299,   115,   190,   880,   364,   520,   108,   346,
     204,   834,   608,   609,   610,   612,   110,   989,   138,   619,
     489,   521,   620,   621,   365,   775,   605,   901,   318,   776,
    1059,   243,   622,  1064,   606,   623,   624,   797,   777,   625,
     626,   778,   779,   257,   583,   780,   853,   522,   556,   557,
     558,   908,   300,   247,   248,   301,     4,     5,     6,     7,
     524,   523,   990,   126,   127,   118,   119,   281,    13,    14,
      15,    16,    17,    18,   525,  1060,   991,   312,  1065,   584,
     585,   313,   889,   890,   121,    28,   909,   910,   345,  1061,
     258,    34,  1066,   347,   259,   650,   586,    36,   249,   250,
     784,   655,   656,   911,   345,    44,    45,    46,   346,   343,
      49,   666,   344,   854,   855,   588,   511,   856,   882,    53,
      54,   636,   260,   223,   346,    55,   637,   638,   912,   324,
     627,   676,   325,   913,   914,   200,   781,   639,   685,   387,
     687,   388,   688,   640,   915,   864,   691,   692,   865,   866,
     916,   251,   252,   253,    57,    58,   641,   938,   867,   383,
     384,   868,   869,   917,   711,   870,   871,   254,   255,   256,
      59,   857,   858,   859,   642,   643,    60,    61,   526,   730,
     267,   918,   919,   341,   122,   282,   695,    64,   697,    65,
     698,   283,   347,  1009,   699,  1010,   277,  1011,   539,   962,
     751,  1012,   977,   543,   700,   701,   274,   978,   347,   550,
     286,  1013,  1014,   999,  1000,  1149,   481,   562,   769,   482,
     485,   636,   486,   772,   773,   774,  1044,   570,  1033,  1048,
     677,   785,   786,   678,   787,   788,   289,   789,   790,   791,
     792,   793,   794,   795,   796,   637,   308,   309,   310,   223,
     957,   998,  1034,   318,   958,  1144,   872,   599,   318,  1145,
     603,   682,   638,   320,   683,   311,   321,   322,   320,  1035,
     959,   321,   322,   639,   640,  1146,   812,   813,   814,   641,
    1036,  1037,   314,   707,   709,   818,  1038,   712,   713,   715,
      98,    99,   290,   291,   292,   293,   294,   824,   825,   642,
      80,   643,   805,   695,   830,   295,  1039,   697,  1040,  1071,
    1083,   838,   701,   351,  1084,   745,   746,   747,  1113,  1085,
     107,   702,   317,   352,   301,   318,   319,   353,   843,   233,
     234,   235,   236,   237,   238,   320,   354,   849,   321,   322,
     925,   239,   323,   649,   860,   861,   863,  1115,   485,   706,
     671,   652,   301,   240,   324,   654,   873,   325,   657,   324,
     927,   960,   325,   662,   355,   665,  1147,  1116,   878,  1125,
     636,   206,  1126,  1127,   207,   208,   637,  1163,   731,   385,
     386,   301,  1128,  1164,   209,  1129,  1130,   210,   211,  1131,
    1132,   212,   213,   902,   903,   904,   905,   906,   907,   638,
     808,   469,   470,   301,   694,   639,  1165,     4,     5,     6,
       7,   376,  1166,   703,   126,   127,   118,   119,   281,    13,
      14,    15,    16,    17,    18,   640,   324,   641,   378,   325,
     379,   642,  1167,   643,  1168,   121,    28,   929,  1169,   695,
    1170,   301,    34,   770,   697,   701,  1171,   382,    36,   980,
     844,  1172,  1173,   389,   978,  1069,    44,    45,    46,   883,
     978,    49,   884,   885,   886,   887,   831,   471,   951,   832,
      53,    54,   472,   223,   932,   214,    55,   301,   874,   301,
    1133,   811,   963,   964,   965,   966,   967,   968,   969,   970,
     485,   215,   899,   992,   216,   217,   993,   976,   994,   995,
     939,   479,   301,   940,   218,    57,    58,   219,   220,   473,
    1021,   221,   222,   301,  1079,  1004,   474,   301,   799,   475,
     802,    59,   224,   225,   226,   227,   807,    60,    61,   433,
     434,   435,   228,   229,   230,   122,   663,   476,    64,   487,
      65,   489,   283,   437,   438,   439,   483,   484,  1016,   495,
     496,   823,  1018,  1019,   497,   498,   231,   232,   510,   301,
     490,    97,   488,    98,    99,   100,   101,   102,   103,   104,
     491,  1026,   530,   386,   492,  1030,  1031,  1032,   105,   494,
     944,   500,   383,   531,   948,   949,   950,   383,   540,   509,
     454,   106,  1041,   107,   528,   223,   544,   384,   455,  1046,
     456,   457,   529,  1052,   545,   546,   548,   384,  1056,   458,
     459,   569,   971,   572,   460,   549,   973,   576,   384,   577,
     461,   462,   578,   384,   574,  1074,  1075,   581,   463,   464,
     465,   383,   653,   724,   725,   760,   725,   881,   582,     4,
       5,     6,     7,   768,   301,  1082,   126,   127,   118,   119,
     281,    13,    14,    15,    16,    17,    18,   804,   301,   806,
     301,   815,   301,   587,  1095,   816,   496,   121,    28,   817,
     301,   821,   496,   595,    34,   383,   822,   583,  1107,   596,
      36,   826,   683,  1111,  1023,  1024,   835,   725,    44,    45,
      46,   839,   301,    49,   600,   931,   840,   301,   841,   546,
     891,   725,    53,    54,   976,   937,   892,   725,    55,  1121,
    1122,  1124,   920,   301,   921,   496,   922,   301,   924,   301,
     383,   936,   945,   384,    98,    99,   290,   291,   292,   293,
     294,   946,   546,  1017,   301,  1073,   301,    57,    58,   295,
    1078,   301,   584,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1080,   546,    59,   107,  -381,  -381,  1089,   725,    60,
      61,   585,  1160,  1161,  1162,  1097,   725,   122,   800,   586,
      64,  1006,    65,   601,   283,  1142,   725,   180,   182,   443,
     444,  1087,   607,   445,   446,    80,     4,     5,     6,     7,
     628,   629,   630,   126,   127,   118,   119,   281,    13,    14,
      15,    16,    17,    18,   631,   632,   633,   634,   635,  1020,
     636,   637,   638,  1022,   121,    28,   639,   640,   641,  1118,
     642,    34,   454,   643,   240,   467,   644,    36,   246,   645,
     455,   646,   456,   457,   647,    44,    45,    46,   658,   659,
      49,   458,   459,   660,   667,   672,   460,   673,   454,    53,
      54,   674,   461,   462,   679,    55,   455,   693,   456,   457,
     463,   464,   465,   675,   684,   695,   696,   458,   459,   697,
     698,   301,   460,   699,   700,   701,   704,   705,   461,   462,
     732,   733,  1159,   752,    57,    58,   463,   464,   465,   341,
     734,   735,   736,   742,   737,   741,   743,   748,   749,   753,
      59,   750,   744,   771,   809,   803,    60,    61,   805,   819,
     820,   845,   829,   846,   122,   810,   833,    64,   836,    65,
     837,   283,     4,     5,     6,     7,   847,   848,   850,   390,
     391,   392,   393,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   394,   395,    24,   851,   852,    25,    26,
      27,    28,   396,   397,    31,    32,    33,    34,    35,   877,
     875,   876,   879,    36,    37,    38,    39,    40,    41,   398,
     399,    44,    45,    46,    47,    48,    49,    50,   893,   888,
     894,    51,    52,   895,   896,    53,    54,   900,   923,   925,
     926,    55,   927,   928,    97,    56,    98,    99,   100,   101,
     102,   103,   104,   930,   933,   418,   419,   420,   421,   422,
     423,   105,   425,   426,   427,   428,   429,   430,   431,   432,
      57,    58,   934,   935,   604,   972,   107,   941,   942,   974,
     996,  1001,   997,  1002,  1003,  1015,    59,  1007,  1008,  1025,
    1027,  1028,    60,    61,  1029,  1045,  1042,  1049,  1051,    62,
      63,  1050,  1053,    64,  1054,    65,   400,     4,     5,     6,
       7,  1055,  1072,  1067,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   726,   727,
      24,  1068,  1076,    25,    26,    27,    28,    29,   728,    31,
      32,    33,    34,    35,   978,  1077,  1090,  1086,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,  1098,  1099,  1100,    51,    52,  1101,  1102,
      53,    54,  1103,  1104,  1105,  1106,    55,  1108,  1109,  1110,
      56,  1114,  1117,  1135,  1136,  1137,  1138,  1139,  1140,   151,
    1119,  1143,   754,   681,   436,     0,     0,     0,   315,     0,
       0,     0,   442,     0,     0,    57,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,    62,    63,     0,     0,    64,     0,
      65,   729,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,  1091,  1092,    24,     0,     0,    25,    26,
      27,    28,    29,  1093,    31,    32,    33,    34,    35,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
       0,    51,    52,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,    56,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,    60,    61,     0,     0,    44,    45,    46,    62,
      63,    49,     0,    64,     0,    65,  1094,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   126,
     127,   118,   119,   120,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,    60,    61,     0,
       0,    44,    45,    46,   154,   122,    49,     0,    64,     0,
      65,   611,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,    60,    61,     0,     0,    44,    45,    46,   154,
     122,    49,     0,    64,     0,    65,   862,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,     0,   154,   122,     0,     0,    64,     0,
      65,  1123,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,     0,
       0,     0,     0,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
       0,    51,    52,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,    56,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,    60,    61,     0,     0,    44,    45,    46,    62,
      63,    49,     0,    64,     0,    65,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   126,
     127,   118,   119,   120,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,    60,    61,     0,
       0,    44,    45,    46,   128,   129,    49,     0,    64,     0,
      65,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,    60,    61,     0,     0,    44,    45,    46,   154,
     122,    49,     0,    64,     0,    65,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   198,
     127,   118,   119,   120,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,    60,    61,     0,
       0,    44,    45,    46,   156,   122,    49,     0,    64,     0,
      65,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,    60,    61,     0,     0,     0,    44,    45,    46,
     122,   199,    49,    64,     0,    65,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   122,    49,     0,    64,
     202,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,    59,     0,     0,
      36,     0,     0,    60,    61,     0,     0,     0,    44,    45,
      46,   122,   268,    49,    64,     0,    65,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   126,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,    60,
      61,   272,     0,    44,    45,    46,     0,   122,    49,     0,
      64,     0,    65,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,    60,    61,     0,     0,     0,    44,
      45,    46,   122,   199,    49,    64,     0,    65,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   126,   127,   118,   119,   120,    13,    14,
      15,    16,    17,    18,     0,     0,   366,     0,    57,    58,
       0,     0,     0,     0,   121,    28,     0,     0,     0,     0,
       0,    34,     0,     0,    59,     0,     0,    36,     0,     0,
      60,    61,     0,     0,     0,    44,    45,    46,   122,   297,
      49,    64,     0,    65,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   126,   127,
     118,   119,   120,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   121,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,    60,    61,     0,     0,
      44,    45,    46,     0,   122,    49,     0,    64,     0,    65,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,   204,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   126,   127,   118,   119,   120,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   121,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,    60,    61,     0,     0,    44,    45,    46,     0,   122,
      49,     0,    64,     0,    65,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   126,
     127,   118,   119,   120,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
      59,     0,     0,    36,     0,     0,    60,    61,     0,     0,
       0,    44,    45,    46,   122,   598,    49,    64,     0,    65,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,    60,    61,     0,     0,    44,    45,    46,     0,
     122,    49,     0,    64,   602,    65,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,    59,     0,     0,    36,     0,     0,    60,    61,     0,
       0,     0,    44,    45,    46,   122,   648,    49,    64,     0,
      65,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,    59,     0,     0,
      36,     0,     0,    60,    61,     0,     0,     0,    44,    45,
      46,   122,   651,    49,    64,     0,    65,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     4,     5,   515,     7,     0,
       0,     0,   126,   127,   118,   119,   516,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,    59,     0,     0,    36,     0,     0,    60,
      61,     0,     0,     0,    44,    45,    46,   122,   661,    49,
      64,     0,    65,     0,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   126,   127,
     118,   119,   120,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,   121,
      28,     0,     0,     0,     0,     0,    34,     0,     0,    59,
       0,     0,    36,     0,     0,    60,    61,     0,     0,     0,
      44,    45,    46,   122,   199,    49,    64,     0,    65,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   126,   127,   118,   119,   120,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,   121,    28,     0,     0,     0,
       0,     0,    34,     0,     0,    59,     0,     0,    36,     0,
       0,    60,    61,     0,     0,     0,    44,    45,    46,   122,
     798,    49,    64,     0,    65,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,    59,     0,     0,    36,     0,     0,    60,    61,     0,
       0,     0,    44,    45,    46,   122,  1005,    49,    64,     0,
      65,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   139,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   141,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   143,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   145,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   122,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   165,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   167,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   122,    49,     0,    64,
       0,   181,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   270,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,   515,     7,     0,     0,     0,
     126,   127,   118,   119,   516,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   513,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   517,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   686,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   116,   117,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,     0,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   710,    49,     0,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -464,  -464,     0,    57,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,     0,   122,     0,     0,    64,
       0,    65,  -464,  -464,     0,  -464,  -464,  -464,  -464,  -464,
    -464,  -464,  -464,  -464,  -464,  -464,  -464,  -464,     0,     0,
       0,  -464,     0,     0,     0,     0,     0,     0,     0,  -464,
    -464,  -464,  -464,  -464,  -464,  -464,  -464,     0,     0,  -464,
    -464,  -464,  -464,  -464,     0,     0,  -464,  -464,  -464,     0,
    -464
};

static const yytype_int16 yycheck[] =
{
       2,     0,   259,   230,    14,   545,   492,   707,   379,     0,
      15,    14,   498,   311,    22,    26,    14,    40,    41,    22,
      22,    23,    24,    25,    26,   116,    30,    29,    30,    31,
      10,    11,    34,    35,    14,    14,    38,    39,    11,    26,
      14,   131,    83,    17,    46,    14,   136,    49,    53,    26,
      26,    77,    78,    79,    59,    60,    61,    22,    26,    10,
      11,    63,    64,    14,    10,    11,   116,   116,    14,    46,
      46,    10,    11,   120,   121,    14,    10,    11,    46,   116,
      14,   131,   131,    14,    26,    26,    65,    61,    68,    69,
      70,    22,   133,    26,   118,   136,    26,   134,   100,   101,
     102,   103,   104,   105,    46,    46,   108,   109,   116,   111,
     112,   137,   136,    46,   137,   882,    46,    68,    69,    70,
     122,     2,    68,    69,    70,    26,    14,   129,   138,    68,
      69,    70,   136,   136,    68,    69,    70,   139,   136,   141,
      71,   143,    12,   145,   844,    46,    27,    28,   129,   136,
     131,   116,   132,   130,   130,   526,   129,   133,   131,    40,
      41,   647,   130,   165,    10,   167,    47,    48,   170,   137,
     172,   173,   116,   175,   176,    10,   178,   179,   180,   181,
     182,   132,    12,    10,    11,   116,   132,    14,   130,   130,
     134,   133,   133,   132,   118,   137,   137,   130,   132,   130,
     130,   136,    26,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   136,   215,   216,   217,   218,   219,   220,   221,
     222,    83,    46,   116,   595,   596,    10,    73,    74,   130,
      14,   998,   133,   244,   245,   118,   137,   130,    73,    74,
     121,    68,    69,    70,    83,   116,   251,   252,   253,   254,
     255,   256,   257,   136,    10,    11,    59,    60,    14,   130,
      63,   559,    26,   144,     5,   146,   147,   131,   270,    81,
      82,   133,   136,    14,   136,   136,  1043,    10,    11,    12,
    1047,    14,    46,   122,   286,   166,   288,   168,   290,   291,
     292,   293,   294,   295,   296,   593,    81,    82,    31,   301,
     302,   841,   304,    36,   306,   132,   130,    59,    60,    14,
     122,    63,    68,    69,    70,   317,   318,   319,   320,   321,
     322,   323,  1089,  1023,   205,   327,   328,   338,   339,    80,
    1097,    14,    80,    83,   137,   101,   102,   122,   116,    14,
      14,   118,   341,   345,   346,  1112,   348,   349,    22,   351,
     352,   130,   130,    14,   133,   357,    14,   359,   137,   136,
     107,    22,   364,   365,    22,   131,   130,    14,   115,    26,
      10,    11,   122,   137,    14,  1142,   132,    10,   129,    10,
     131,   129,   129,   131,    31,   137,    29,   116,   116,    46,
      75,   689,   394,   395,   396,   397,   118,    71,   131,    26,
     130,   130,    29,    30,    47,    26,   134,   137,    29,    30,
      71,   108,    39,    71,   136,    42,    43,   644,    39,    46,
      47,    42,    43,   115,   130,    46,   724,   116,    68,    69,
      70,   137,   130,   113,   114,   133,     3,     4,     5,     6,
     116,   130,   116,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   130,   116,   130,    10,   116,   130,
     130,    14,   760,   761,    31,    32,   137,   137,    26,   130,
     129,    38,   130,   130,   129,   477,   130,    44,   123,   124,
     137,   483,   484,   137,    26,    52,    53,    54,    46,   130,
      57,   493,   133,    10,    11,   376,   498,    14,   755,    66,
      67,   130,    14,   130,    46,    72,   130,   130,   137,   130,
     137,   513,   133,   137,   137,   517,   137,   130,   520,   133,
     522,   135,   524,   130,   137,    26,   528,   529,    29,    30,
     137,   109,   110,   111,   101,   102,   130,   835,    39,   132,
     133,    42,    43,   137,   546,    46,    47,   125,   126,   127,
     117,    68,    69,    70,   130,   130,   123,   124,   131,   561,
      14,   137,   137,   136,   131,   132,   130,   134,   130,   136,
     130,   138,   130,   137,   130,   137,   130,   137,   325,   137,
     582,   137,   131,   330,   130,   130,    14,   136,   130,   336,
     134,   137,   137,   891,   892,   137,   130,   344,   600,   133,
     133,   130,   135,   605,   606,   607,   977,   354,   137,   980,
     130,   613,   614,   133,   616,   617,   130,   619,   620,   621,
     622,   623,   624,   625,   626,   130,    10,    11,    12,   130,
      26,   888,   137,    29,    30,    26,   137,   384,    29,    30,
     387,   130,   130,    39,   133,    14,    42,    43,    39,   137,
      46,    42,    43,   130,   130,    46,   658,   659,   660,   130,
     137,   137,    10,   544,   545,   667,   137,   548,   549,   550,
     101,   102,   103,   104,   105,   106,   107,   679,   680,   130,
     561,   130,   130,   130,   686,   116,   137,   130,   137,   137,
     137,   693,   130,   116,   137,   576,   577,   578,  1069,   137,
     131,   130,    26,   116,   133,    29,    30,    72,   710,    88,
      89,    90,    91,    92,    93,    39,   133,   719,    42,    43,
     130,   100,    46,   470,   726,   727,   728,   137,   133,   130,
     135,   478,   133,   112,   130,   482,   738,   133,   485,   130,
     130,   137,   133,   490,   136,   492,   137,   137,   750,    26,
     130,    26,    29,    30,    29,    30,   130,   137,   130,   132,
     133,   133,    39,   137,    39,    42,    43,    42,    43,    46,
      47,    46,    47,   775,   776,   777,   778,   779,   780,   130,
     130,   130,   131,   133,   531,   130,   137,     3,     4,     5,
       6,    56,   137,   540,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   130,   130,   130,   136,   133,
     131,   130,   137,   130,   137,    31,    32,   819,   137,   130,
     137,   133,    38,   135,   130,   130,   137,   136,    44,   131,
     711,   137,   137,   137,   136,   131,    52,    53,    54,    10,
     136,    57,    13,    14,    15,    16,   130,   130,   850,   133,
      66,    67,   130,   130,   130,   130,    72,   133,   739,   133,
     137,   135,   864,   865,   866,   867,   868,   869,   870,   871,
     133,    26,   135,    10,    29,    30,    13,   879,    15,    16,
     130,   132,   133,   133,    39,   101,   102,    42,    43,   130,
     130,    46,    47,   133,   130,   897,   130,   133,   645,   130,
     647,   117,    84,    85,    86,    87,   653,   123,   124,     7,
       8,     9,    94,    95,    96,   131,   132,   130,   134,   119,
     136,   130,   138,   241,   242,   243,   123,   124,   930,   132,
     133,   678,   934,   935,   132,   133,   118,   119,   132,   133,
     131,    99,   137,   101,   102,   103,   104,   105,   106,   107,
      14,   953,   132,   133,   131,   957,   958,   959,   116,    14,
     841,    14,   132,   133,   845,   846,   847,   132,   133,   130,
      14,   129,   974,   131,   116,   130,   132,   133,    22,   978,
      24,    25,   116,   985,    27,    28,   132,   133,   990,    33,
      34,   130,   873,    14,    38,    10,   877,   132,   133,    27,
      44,    45,   132,   133,    14,  1007,  1008,    10,    52,    53,
      54,   132,   133,   132,   133,   132,   133,    61,   130,     3,
       4,     5,     6,   132,   133,  1027,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,   132,   133,   132,
     133,   132,   133,   137,  1046,   132,   133,    31,    32,   132,
     133,   132,   133,   131,    38,   132,   133,   130,  1060,   131,
      44,   132,   133,  1065,   945,   946,   132,   133,    52,    53,
      54,   132,   133,    57,   116,   822,   132,   133,    27,    28,
     132,   133,    66,    67,  1086,   832,   132,   133,    72,  1091,
    1092,  1093,   132,   133,   132,   133,   132,   133,   132,   133,
     132,   133,   132,   133,   101,   102,   103,   104,   105,   106,
     107,    27,    28,   132,   133,   132,   133,   101,   102,   116,
     132,   133,   130,  1125,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,    27,    28,   117,   131,   132,   133,   132,   133,   123,
     124,   130,  1144,  1145,  1146,   132,   133,   131,   132,   130,
     134,   898,   136,    10,   138,   132,   133,    45,    46,   247,
     248,  1042,   116,   249,   250,  1046,     3,     4,     5,     6,
     130,   130,   130,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   130,   130,   130,   130,   130,   936,
     130,   130,   130,   940,    31,    32,   130,   130,   130,  1080,
     130,    38,    14,   130,   112,    17,    97,    44,   122,   131,
      22,    14,    24,    25,   131,    52,    53,    54,   116,   116,
      57,    33,    34,   116,   131,   119,    38,   137,    14,    66,
      67,   130,    44,    45,    80,    72,    22,   116,    24,    25,
      52,    53,    54,   130,   130,   130,   130,    33,    34,   130,
     130,   133,    38,   130,   130,   130,   130,   130,    44,    45,
     130,   130,  1143,    14,   101,   102,    52,    53,    54,   136,
     130,   130,   130,   137,   130,   119,   119,   130,   130,    14,
     117,   116,   137,    14,   135,   130,   123,   124,   130,    80,
      14,    10,   130,    10,   131,   135,   130,   134,   130,   136,
     130,   138,     3,     4,     5,     6,    10,    14,   116,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    10,    10,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    27,
     130,   130,   130,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    14,    59,
      14,    62,    63,    14,   130,    66,    67,   137,   130,   130,
     130,    72,   130,   132,    99,    76,   101,   102,   103,   104,
     105,   106,   107,   131,   130,   224,   225,   226,   227,   228,
     229,   116,   231,   232,   233,   234,   235,   236,   237,   238,
     101,   102,    80,    80,   129,   137,   131,   130,   130,   130,
     130,     7,   130,     7,     7,   130,   117,   116,   116,    14,
     116,    10,   123,   124,    10,    75,   132,    14,   130,   130,
     131,    14,    14,   134,    14,   136,   137,     3,     4,     5,
       6,   130,   137,   130,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   130,   132,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   136,   130,   137,   130,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,   130,   130,   130,    62,    63,   130,   130,
      66,    67,   130,    14,    14,   130,    72,    14,    14,   130,
      76,   137,   130,   130,   130,   130,   130,   130,   130,    28,
    1086,   132,   591,   517,   240,    -1,    -1,    -1,   138,    -1,
      -1,    -1,   246,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,   134,    -1,
     136,   137,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    -1,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,   130,
     131,    57,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,    -1,
      -1,    52,    53,    54,   130,   131,    57,    -1,   134,    -1,
     136,   137,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,   130,
     131,    57,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,   134,    -1,
     136,   137,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    -1,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,   130,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,    -1,
      -1,    52,    53,    54,   130,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,   130,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,    -1,
      -1,    52,    53,    54,   130,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,
      -1,    -1,   123,   124,    -1,    -1,    -1,    52,    53,    54,
     131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
     135,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,
      44,    -1,    -1,   123,   124,    -1,    -1,    -1,    52,    53,
      54,   131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,
     124,   125,    -1,    52,    53,    54,    -1,   131,    57,    -1,
     134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,
      -1,    44,    -1,    -1,   123,   124,    -1,    -1,    -1,    52,
      53,    54,   131,   132,    57,   134,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    23,    -1,   101,   102,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,
     123,   124,    -1,    -1,    -1,    52,    53,    54,   131,   132,
      57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,   123,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     117,    -1,    -1,    44,    -1,    -1,   123,   124,    -1,    -1,
      -1,    52,    53,    54,   131,   132,    57,   134,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,   135,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,   117,    -1,    -1,    44,    -1,    -1,   123,   124,    -1,
      -1,    -1,    52,    53,    54,   131,   132,    57,   134,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,
      44,    -1,    -1,   123,   124,    -1,    -1,    -1,    52,    53,
      54,   131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,   123,
     124,    -1,    -1,    -1,    52,    53,    54,   131,   132,    57,
     134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,
      -1,    -1,    44,    -1,    -1,   123,   124,    -1,    -1,    -1,
      52,    53,    54,   131,   132,    57,   134,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,
      -1,   123,   124,    -1,    -1,    -1,    52,    53,    54,   131,
     132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,   117,    -1,    -1,    44,    -1,    -1,   123,   124,    -1,
      -1,    -1,    52,    53,    54,   131,   132,    57,   134,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,   134,
      -1,   136,    81,    82,    -1,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,    -1,
     129
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   140,   141,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    62,    63,    66,    67,    72,    76,   101,   102,   117,
     123,   124,   130,   131,   134,   136,   142,   143,   145,   146,
     147,   148,   149,   151,   152,   154,   155,   158,   159,   162,
     167,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   184,   185,   187,    99,   101,   102,
     103,   104,   105,   106,   107,   116,   129,   131,   116,   134,
     118,   136,   116,    11,   129,   131,    10,    11,    12,    13,
      14,    31,   131,   167,   180,    14,    10,    11,   130,   131,
     169,   186,    10,    11,    12,    14,    31,    36,   131,   131,
     169,   131,   169,   131,   169,   131,   169,   169,   176,    14,
     167,   151,   167,   169,   130,   169,   130,   169,    10,    14,
      14,   169,   169,    12,    12,   131,   169,   131,   169,   167,
     131,   167,    26,    46,   130,    26,    46,   130,   136,   168,
     168,   136,   168,   169,   167,   167,   169,    14,    14,    14,
      31,   180,    14,    10,    10,   180,   180,   180,    10,   132,
     169,   182,   135,   169,    75,   141,    26,    29,    30,    39,
      42,    43,    46,    47,   130,    26,    29,    30,    39,    42,
      43,    46,    47,   130,    84,    85,    86,    87,    94,    95,
      96,   118,   119,    88,    89,    90,    91,    92,    93,   100,
     112,   120,   121,   108,    81,    82,   122,   113,   114,   123,
     124,   109,   110,   111,   125,   126,   127,   115,   129,   129,
      14,   169,   169,   169,   169,   169,   169,    14,   132,   186,
     131,   169,   125,   169,    14,   169,   169,   130,    14,    17,
      61,    14,   132,   138,   164,   186,   134,   118,   136,   130,
     103,   104,   105,   106,   107,   116,   116,   132,   169,   186,
     130,   133,   116,   130,   116,   130,   116,   130,    10,    11,
      12,    14,    10,    14,    10,   182,   169,    26,    29,    30,
      39,    42,    43,    46,   130,   133,   169,    26,    46,   130,
     133,   169,   167,   169,   167,    83,   133,   167,    81,    82,
     131,   136,   156,   130,   133,    26,    46,   130,    26,    46,
     130,   116,   116,    72,   133,   136,   118,   136,   118,   136,
     169,   167,   169,   167,    29,    47,    23,   169,   169,   169,
     169,   169,   169,   169,   169,   169,    56,   160,   136,   131,
      14,    65,   136,   132,   133,   132,   133,   133,   135,   137,
      10,    11,    12,    13,    24,    25,    33,    34,    50,    51,
     137,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   172,   172,
     172,   172,   172,   172,   170,   172,   172,   172,   172,   172,
     172,   172,   172,     7,     8,     9,   173,   175,   175,   175,
     176,   176,   177,   178,   178,   179,   179,   180,   180,   180,
     180,   180,   180,   180,    14,    22,    24,    25,    33,    34,
      38,    44,    45,    52,    53,    54,   183,    17,   183,   130,
     131,   130,   130,   130,   130,   130,   130,   116,   131,   132,
     169,   130,   133,   123,   124,   133,   135,   119,   137,   130,
     131,    14,   131,    80,    14,   132,   133,   132,   133,   169,
      14,   169,   169,   169,   169,   169,   169,   169,   169,   130,
     132,   169,   169,   131,   169,     5,    14,   131,   165,   169,
     116,   130,   116,   130,   116,   130,   131,   156,   116,   116,
     132,   133,   169,   169,   169,   169,   169,   169,   169,   186,
     133,   169,   169,   186,   132,    27,    28,   144,   132,    10,
     186,   176,   176,    10,    11,    14,    68,    69,    70,   132,
     166,   141,   186,   169,   169,   169,   169,   169,   169,   130,
     186,   153,    14,   169,    14,   169,   132,    27,   132,   169,
     169,    10,   130,   130,   130,   130,   130,   137,   167,    14,
      22,   136,   161,   132,   166,   131,   131,   163,   132,   186,
     116,    10,   135,   186,   129,   134,   136,   116,   169,   169,
     169,   137,   169,    26,    46,   137,    26,    46,   137,    26,
      29,    30,    39,    42,    43,    46,    47,   137,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,    97,   131,    14,   131,   132,   186,
     169,   132,   186,   133,   186,   169,   169,   186,   116,   116,
     116,   132,   186,   132,   164,   186,   169,   131,    14,   138,
     164,   135,   119,   137,   130,   130,   169,   130,   133,    80,
      80,   165,   130,   133,   130,   169,   131,   169,   169,   132,
     166,   169,   169,   116,   186,   130,   130,   130,   130,   130,
     130,   130,   130,   186,   130,   130,   130,   167,   143,   167,
     131,   169,   167,   167,    83,   167,    83,    83,    22,   116,
      10,    73,    74,   156,   132,   133,    24,    25,    34,   137,
     169,   130,   130,   130,   130,   130,   130,   130,    40,    41,
     137,   119,   137,   119,   137,   167,   167,   167,   130,   130,
     116,   169,    14,    14,   161,    59,    60,    63,   137,   156,
     132,   132,   166,   166,    77,    78,    79,   137,   132,   169,
     135,    14,   169,   169,   169,    26,    30,    39,    42,    43,
      46,   137,   137,   137,   137,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   132,   186,
     132,   164,   186,   130,   132,   130,   132,   186,   130,   135,
     135,   135,   169,   169,   169,   132,   132,   132,   169,    80,
      14,   132,   133,   186,   169,   169,   132,     5,    14,   130,
     169,   130,   133,   130,   156,   132,   130,   130,   169,   132,
     132,    27,   144,   169,   167,    10,    10,    10,    14,   169,
     116,    10,    10,   156,    10,    11,    14,    68,    69,    70,
     169,   169,   137,   169,    26,    29,    30,    39,    42,    43,
      46,    47,   137,   169,   167,   130,   130,    27,   169,   130,
     137,    61,   183,    10,    13,    14,    15,    16,    59,   156,
     156,   132,   132,    14,    14,    14,   130,   116,   131,   135,
     137,   137,   169,   169,   169,   169,   169,   169,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     132,   132,   132,   130,   132,   130,   130,   130,   132,   169,
     131,   186,   130,   130,    80,    80,   133,   186,   156,   130,
     133,   130,   130,   143,   167,   132,    27,   144,   167,   167,
     167,   169,    22,   116,    10,    73,    74,    26,    30,    46,
     137,   137,   137,   169,   169,   169,   169,   169,   169,   169,
     169,   167,   137,   167,   130,   150,   169,   131,   136,   157,
     131,   157,    14,    22,    71,   116,   130,    14,    22,    71,
     116,   130,    10,    13,    15,    16,   130,   130,   183,   156,
     156,     7,     7,     7,   169,   132,   186,   116,   116,   137,
     137,   137,   137,   137,   137,   130,   169,   132,   169,   169,
     186,   130,   186,   167,   167,    14,   169,   116,    10,    10,
     169,   169,   169,   137,   137,   137,   137,   137,   137,   137,
     137,   169,   132,   132,   166,    75,   141,   132,   166,    14,
      14,   130,   169,    14,    14,   130,   169,    14,    22,    71,
     116,   130,    14,    22,    71,   116,   130,   130,   130,   131,
     157,   137,   137,   132,   169,   169,   132,   130,   132,   130,
      27,   144,   169,   137,   137,   137,   130,   167,   157,   132,
     137,    24,    25,    34,   137,   169,   157,   132,   130,   130,
     130,   130,   130,   130,    14,    14,   130,   169,    14,    14,
     130,   169,   132,   166,   137,   137,   137,   130,   167,   150,
     157,   169,   169,   137,   169,    26,    29,    30,    39,    42,
      43,    46,    47,   137,   157,   130,   130,   130,   130,   130,
     130,   157,   132,   132,    26,    30,    46,   137,   137,   137,
     169,   169,   169,   169,   169,   169,   169,   169,   157,   167,
     169,   169,   169,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   139,   140,   141,   141,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   143,   143,   143,   143,   143,   143,   143,   143,
     144,   144,   144,   144,   144,   144,   145,   145,   146,   146,
     146,   146,   147,   147,   148,   148,   148,   149,   149,   149,
     150,   151,   151,   151,   151,   151,   151,   152,   152,   153,
     153,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   155,   155,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     159,   160,   160,   160,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   162,   163,
     163,   163,   163,   164,   164,   164,   164,   165,   165,   165,
     165,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   168,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   170,   170,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   172,   172,
     173,   173,   173,   174,   174,   175,   175,   175,   176,   176,
     177,   177,   177,   178,   178,   178,   179,   179,   179,   179,
     179,   179,   179,   180,   180,   180,   180,   180,   180,   181,
     181,   182,   182,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   184,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       5,     5,     6,     6,     4,     2,     5,     4,     3,     3,
       3,     5,     7,     3,     5,     7,     3,     5,     3,     2,
       5,     5,     3,     2,     5,     5,     4,     7,     6,     4,
       6,     4,     2,     4,     4,     4,     4,     4,     4,     4,
       4,     7,     7,     7,     6,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     4,     4,     4,     4,     4,     2,     1,
       1,     1,     1,     1,     2,     4,     2,     1,     1,     1,
       1,     1,     2,     2,     4,     4,     4,     4,     1,     1,
       1,     1,     5,     7,     7,     3,     5,     5,     4,     6,
       3,     5,     5,     7,     4,     6,     5,     3,     5,     7,
       3,     5,     5,     3,     2,     5,     5,     2,    12,     9,
       1,     7,     7,     5,     7,     5,     3,     5,     7,     0,
       4,     6,     5,     3,     7,     6,     4,     7,     6,     7,
       6,     5,     6,     5,     3,     4,     5,     4,     5,     5,
       6,     6,     6,     6,     6,     6,     6,     6,     7,     7,
       7,     3,     3,     4,     5,     4,     5,     5,     6,     6,
       6,     6,     6,     6,     6,     6,     7,     7,     7,     6,
       5,     0,     3,     3,     0,     4,     4,     4,     5,     5,
       4,     5,     5,     6,     6,     7,     7,     6,     6,     6,
       6,     5,     5,     6,     6,     7,     7,     7,     7,     7,
       6,     4,     7,     6,     4,     8,     7,     5,     5,     0,
       4,     4,     4,     3,     5,     5,     7,     3,     3,     5,
       5,     1,     3,     5,     1,     3,     2,     3,     3,     3,
       4,     5,     5,     3,     5,     4,     6,     1,     3,     1,
       3,     1,     3,     3,     3,     4,     6,     6,     6,     6,
       6,     6,     6,     6,     5,     7,     7,     7,     7,     7,
       7,     5,     9,     8,     8,     6,     9,     9,     5,     5,
       4,     4,     4,     6,     6,     6,     6,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     1,     5,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     3,     4,
       6,     4,     3,     6,     6,     5,     3,     3,     6,     5,
       4,     4,     6,     6,     5,     3,     6,     5,     3,     2,
       3,     3,     2,     3,     1,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     4,     6,     6,     6,     1,     4,     4,
       5,     5,     1,     1,     1,     1,     2,     3,     4,     5,
       2,     3,     4,     5,     1,     2
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
#line 488 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3097 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 498 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3103 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 499 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3109 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 503 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3115 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 505 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3121 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 507 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3127 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 509 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3135 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 513 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3141 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 515 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3147 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 517 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3153 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 519 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3159 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 521 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3167 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 525 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3175 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 529 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3181 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 531 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3189 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 535 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3197 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3203 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3209 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 543 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3215 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 545 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3221 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3227 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 549 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3235 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 553 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3243 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 557 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3249 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 559 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3255 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 561 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3261 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3267 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 565 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3273 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 567 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3279 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 569 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3285 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 571 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3291 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 573 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3297 "raku.tab.c"
    break;

  case 34: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 575 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3309 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 583 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3315 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 585 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3321 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 587 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3327 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 589 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3333 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 591 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3339 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 593 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3345 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 595 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3352 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 598 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3359 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 601 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3365 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 603 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3372 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 606 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3379 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 609 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3385 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 611 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3393 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 615 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3399 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 617 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3405 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 619 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3412 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 622 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3419 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 625 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3425 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 627 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3432 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 630 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3439 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 633 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3446 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 636 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3452 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 638 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3461 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 643 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3469 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 647 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3477 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 651 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3486 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 656 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3494 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall ';'  */
#line 659 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3500 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3506 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3512 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 665 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3518 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3524 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 669 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3531 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 672 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3537 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 674 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3543 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 676 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3549 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 678 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3556 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 681 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3563 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 684 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3570 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 687 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3577 "raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 690 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3584 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_IF expr ';'  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3590 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNLESS expr ';'  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3596 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WHILE expr ';'  */
#line 697 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3602 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNTIL expr ';'  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3608 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_FOR expr ';'  */
#line 701 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3615 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITH expr ';'  */
#line 704 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3621 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITHOUT expr ';'  */
#line 706 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3627 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_GIVEN expr ';'  */
#line 708 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3633 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 710 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3640 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 713 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3647 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 716 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3655 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 720 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3662 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 723 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3668 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 725 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3674 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 727 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3680 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 729 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3687 "raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 732 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3694 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 735 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3701 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 738 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3708 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 741 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3715 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 744 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3722 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 747 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3729 "raku.tab.c"
    break;

  case 98: /* stmt: expr ';'  */
#line 749 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3735 "raku.tab.c"
    break;

  case 99: /* stmt: ';'  */
#line 750 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3741 "raku.tab.c"
    break;

  case 100: /* stmt: if_stmt  */
#line 751 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3747 "raku.tab.c"
    break;

  case 101: /* stmt: while_stmt  */
#line 752 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3753 "raku.tab.c"
    break;

  case 102: /* stmt: for_stmt  */
#line 753 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3759 "raku.tab.c"
    break;

  case 103: /* stmt: given_stmt  */
#line 754 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3765 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3771 "raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block KW_CATCH block  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3777 "raku.tab.c"
    break;

  case 106: /* stmt: KW_CATCH block  */
#line 760 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3783 "raku.tab.c"
    break;

  case 107: /* stmt: block  */
#line 762 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3789 "raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 763 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3795 "raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 764 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3801 "raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 765 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3807 "raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 766 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3813 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 767 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3819 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 768 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3825 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 770 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3831 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 772 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3837 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3843 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3849 "raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 777 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3855 "raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 778 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3861 "raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 779 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3867 "raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3873 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block  */
#line 784 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3879 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 786 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3885 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 788 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3891 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block  */
#line 790 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3897 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3903 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 794 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3909 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block elsif_tail  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3915 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3921 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3927 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3933 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3939 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3945 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3951 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3957 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 816 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3963 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE expr block  */
#line 818 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3969 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3975 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3981 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3987 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3993 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 832 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3999 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL expr block  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4005 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4011 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4017 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4023 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP block  */
#line 846 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4029 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 848 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4035 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 850 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4041 "raku.tab.c"
    break;

  case 150: /* loop_incr: expr  */
#line 853 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4047 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 857 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4057 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 863 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4067 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 869 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4076 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 874 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4086 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 880 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4095 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr block  */
#line 885 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4102 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 890 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4114 "raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 898 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4127 "raku.tab.c"
    break;

  case 159: /* when_list: %empty  */
#line 908 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4133 "raku.tab.c"
    break;

  case 160: /* when_list: when_list KW_WHEN expr block  */
#line 910 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4141 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 916 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4153 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 924 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4163 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT sub_body  */
#line 930 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4173 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 936 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4185 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 944 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4195 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 950 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 956 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4218 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 965 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 972 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4242 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 981 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4255 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 990 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4266 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 999 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4277 "raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1006 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4286 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list '}'  */
#line 1012 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4292 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr '}'  */
#line 1014 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4299 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1017 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4306 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1020 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4312 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1022 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4318 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1024 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4324 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1026 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4330 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1028 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4336 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1030 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4342 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1032 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4348 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1034 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4355 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1037 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4361 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4367 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1041 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4373 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1043 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4380 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1046 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4387 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1049 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4395 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list '}'  */
#line 1054 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4401 "raku.tab.c"
    break;

  case 192: /* method_body: '{' YADA '}'  */
#line 1055 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4407 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1057 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4414 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1060 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4421 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1063 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4427 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1065 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4433 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1067 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4439 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1069 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4445 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1071 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4451 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1073 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4457 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1075 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4463 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1077 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4470 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1080 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4476 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1082 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4482 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1084 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4488 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1086 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4495 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1089 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4502 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1092 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4510 "raku.tab.c"
    break;

  case 209: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1098 "raku.y"
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
#line 4528 "raku.tab.c"
    break;

  case 210: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1114 "raku.y"
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
#line 4545 "raku.tab.c"
    break;

  case 211: /* is_clauses: %empty  */
#line 1128 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4551 "raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses IDENT IDENT  */
#line 1130 "raku.y"
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
#line 4567 "raku.tab.c"
    break;

  case 213: /* is_clauses: is_clauses TESTOP IDENT  */
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
#line 4583 "raku.tab.c"
    break;

  case 214: /* class_body_list: %empty  */
#line 1155 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4589 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1157 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4596 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1160 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4603 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1163 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4610 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1166 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4617 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1169 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4624 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1172 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4631 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1175 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4638 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1178 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4645 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1181 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4652 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1184 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4659 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1187 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4666 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1190 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4673 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1193 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4684 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1200 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4695 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1207 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4706 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1214 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4717 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1221 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4725 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1225 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4733 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1229 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4741 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1233 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4749 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1237 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4760 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1244 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4771 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1251 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4782 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1258 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4793 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1265 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4807 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1275 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4819 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1283 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4831 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1291 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4844 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1300 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4855 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1307 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4866 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1314 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4880 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1324 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4892 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1332 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4904 "raku.tab.c"
    break;

  case 248: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1342 "raku.y"
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
#line 4921 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: %empty  */
#line 1356 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4927 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1358 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4936 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1363 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4945 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1368 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4954 "raku.tab.c"
    break;

  case 253: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1375 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4962 "raku.tab.c"
    break;

  case 254: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1379 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4970 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1383 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4978 "raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1387 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4986 "raku.tab.c"
    break;

  case 257: /* pair_list: IDENT OP_FATARROW expr  */
#line 1393 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4992 "raku.tab.c"
    break;

  case 258: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1395 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4998 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1397 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5004 "raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1399 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5010 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR  */
#line 1402 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5016 "raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1403 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5022 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1404 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5028 "raku.tab.c"
    break;

  case 264: /* param_list: VAR_ARRAY  */
#line 1405 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5034 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_ARRAY  */
#line 1406 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5040 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT VAR_SCALAR  */
#line 1407 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5046 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1408 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5052 "raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1409 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5058 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_SCALAR  */
#line 1410 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5064 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1411 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5070 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1412 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5076 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1413 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5082 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR '=' expr  */
#line 1414 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5088 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1415 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5094 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1416 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5100 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1417 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5106 "raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_POS  */
#line 1418 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5112 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_POS  */
#line 1419 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5118 "raku.tab.c"
    break;

  case 279: /* param_list: SLURPY_LOL  */
#line 1420 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5124 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' SLURPY_LOL  */
#line 1421 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5130 "raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_NAMED  */
#line 1422 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5136 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1423 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5142 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list '}'  */
#line 1426 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5148 "raku.tab.c"
    break;

  case 284: /* block: '{' YADA '}'  */
#line 1427 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5154 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr '}'  */
#line 1429 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5160 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1431 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5166 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1433 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5172 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1435 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5178 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1437 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5184 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1439 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5191 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1442 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5197 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1444 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5203 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1446 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5209 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1448 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5215 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1450 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5222 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1453 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5229 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1456 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5237 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1460 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5244 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1463 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5251 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1466 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5258 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1469 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5264 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1471 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5272 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1475 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5279 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1478 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5286 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1481 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5293 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1484 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5300 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1487 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5307 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1490 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5313 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1492 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5319 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1494 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5325 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_LAST '}'  */
#line 1496 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5331 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1498 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5337 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1500 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5343 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1502 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5349 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5355 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1506 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5361 "raku.tab.c"
    break;

  case 317: /* closure: '{' expr '}'  */
#line 1509 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5367 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR '=' expr  */
#line 1512 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5373 "raku.tab.c"
    break;

  case 319: /* expr: VAR_ARRAY '=' expr  */
#line 1513 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5379 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1514 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5385 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1515 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5391 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1516 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5397 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1517 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5403 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1518 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5409 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER block  */
#line 1519 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5419 "raku.tab.c"
    break;

  case 326: /* expr: KW_GATHER for_stmt  */
#line 1524 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5429 "raku.tab.c"
    break;

  case 327: /* expr: tern_expr  */
#line 1529 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5435 "raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1533 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5441 "raku.tab.c"
    break;

  case 329: /* tern_expr: cmp_expr  */
#line 1534 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5447 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1537 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5453 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1538 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5459 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1539 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5465 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1540 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5471 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1541 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5477 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1542 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5483 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1543 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5489 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1544 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5495 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1545 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5501 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1546 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5507 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1547 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5513 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1548 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5519 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1549 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5525 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1550 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5531 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1552 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5541 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1558 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5551 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1564 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5561 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr  */
#line 1569 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5567 "raku.tab.c"
    break;

  case 348: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1572 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5573 "raku.tab.c"
    break;

  case 349: /* divis_expr: jct_expr  */
#line 1573 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5579 "raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '|' range_expr  */
#line 1576 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5585 "raku.tab.c"
    break;

  case 351: /* jct_expr: jct_expr '&' range_expr  */
#line 1577 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5591 "raku.tab.c"
    break;

  case 352: /* jct_expr: dor_expr  */
#line 1578 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5597 "raku.tab.c"
    break;

  case 353: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1582 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5603 "raku.tab.c"
    break;

  case 354: /* dor_expr: range_expr  */
#line 1583 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5609 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1586 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5615 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1587 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5621 "raku.tab.c"
    break;

  case 357: /* range_expr: add_expr  */
#line 1588 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5627 "raku.tab.c"
    break;

  case 358: /* add_expr: add_expr '~' repl_expr  */
#line 1591 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5633 "raku.tab.c"
    break;

  case 359: /* add_expr: repl_expr  */
#line 1592 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5639 "raku.tab.c"
    break;

  case 360: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1595 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5645 "raku.tab.c"
    break;

  case 361: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1596 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5651 "raku.tab.c"
    break;

  case 362: /* repl_expr: addsub_expr  */
#line 1597 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5657 "raku.tab.c"
    break;

  case 363: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1600 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5663 "raku.tab.c"
    break;

  case 364: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1601 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5669 "raku.tab.c"
    break;

  case 365: /* addsub_expr: mul_expr  */
#line 1602 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5675 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '*' unary_expr  */
#line 1605 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5681 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '/' unary_expr  */
#line 1606 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5687 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr '%' unary_expr  */
#line 1607 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5693 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1608 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5699 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1610 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5705 "raku.tab.c"
    break;

  case 371: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1612 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5711 "raku.tab.c"
    break;

  case 372: /* mul_expr: unary_expr  */
#line 1613 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5717 "raku.tab.c"
    break;

  case 373: /* unary_expr: '-' unary_expr  */
#line 1616 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5723 "raku.tab.c"
    break;

  case 374: /* unary_expr: '+' unary_expr  */
#line 1617 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5729 "raku.tab.c"
    break;

  case 375: /* unary_expr: '!' unary_expr  */
#line 1618 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5735 "raku.tab.c"
    break;

  case 376: /* unary_expr: CARET unary_expr  */
#line 1619 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5741 "raku.tab.c"
    break;

  case 377: /* unary_expr: OP_REDUCE unary_expr  */
#line 1621 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5750 "raku.tab.c"
    break;

  case 378: /* unary_expr: pow_expr  */
#line 1625 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5756 "raku.tab.c"
    break;

  case 379: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1628 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5762 "raku.tab.c"
    break;

  case 380: /* pow_expr: postfix_expr  */
#line 1629 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5768 "raku.tab.c"
    break;

  case 381: /* scalar_list: VAR_SCALAR  */
#line 1632 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5774 "raku.tab.c"
    break;

  case 382: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1633 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5780 "raku.tab.c"
    break;

  case 383: /* meth_name: IDENT  */
#line 1636 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5786 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_SORT  */
#line 1637 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5792 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_MAP  */
#line 1638 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5798 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_GREP  */
#line 1639 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5804 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_SAY  */
#line 1640 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5810 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_PRINT  */
#line 1641 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5816 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_TAKE  */
#line 1642 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5822 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_RETURN  */
#line 1643 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5828 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_EXISTS  */
#line 1644 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5834 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_DELETE  */
#line 1645 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5840 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_JOIN  */
#line 1646 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 5846 "raku.tab.c"
    break;

  case 394: /* meth_name: TESTOP  */
#line 1647 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5852 "raku.tab.c"
    break;

  case 395: /* postfix_expr: call_expr  */
#line 1649 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5858 "raku.tab.c"
    break;

  case 396: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1652 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5868 "raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '(' arg_list ')'  */
#line 1658 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5877 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '(' ')'  */
#line 1662 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5883 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1664 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5889 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1666 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5895 "raku.tab.c"
    break;

  case 401: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1668 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5904 "raku.tab.c"
    break;

  case 402: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1673 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5910 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1675 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5920 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1681 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5930 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1687 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5938 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' KW_NEW  */
#line 1691 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5946 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '.' IDENT  */
#line 1695 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5955 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1700 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5966 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1707 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5975 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '.' CARET IDENT  */
#line 1712 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5985 "raku.tab.c"
    break;

  case 411: /* call_expr: atom '.' CARET IDENT  */
#line 1718 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5995 "raku.tab.c"
    break;

  case 412: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1724 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6006 "raku.tab.c"
    break;

  case 413: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1731 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6017 "raku.tab.c"
    break;

  case 414: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1738 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6026 "raku.tab.c"
    break;

  case 415: /* call_expr: atom '.' meth_name  */
#line 1743 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6035 "raku.tab.c"
    break;

  case 416: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1748 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6046 "raku.tab.c"
    break;

  case 417: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1755 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6055 "raku.tab.c"
    break;

  case 418: /* call_expr: call_expr '.' meth_name  */
#line 1760 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6064 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_DIE expr  */
#line 1765 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6070 "raku.tab.c"
    break;

  case 420: /* call_expr: KW_MAP closure expr  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6076 "raku.tab.c"
    break;

  case 421: /* call_expr: KW_GREP closure expr  */
#line 1769 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6082 "raku.tab.c"
    break;

  case 422: /* call_expr: KW_SORT expr  */
#line 1771 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6088 "raku.tab.c"
    break;

  case 423: /* call_expr: KW_SORT closure expr  */
#line 1773 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6094 "raku.tab.c"
    break;

  case 424: /* call_expr: atom  */
#line 1774 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6100 "raku.tab.c"
    break;

  case 425: /* arg_list: expr  */
#line 1777 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6106 "raku.tab.c"
    break;

  case 426: /* arg_list: arg_list ',' expr  */
#line 1778 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6112 "raku.tab.c"
    break;

  case 427: /* arg_list: arg_list ','  */
#line 1779 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6118 "raku.tab.c"
    break;

  case 428: /* atom: LIT_INT  */
#line 1782 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6124 "raku.tab.c"
    break;

  case 429: /* atom: LIT_FLOAT  */
#line 1783 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6130 "raku.tab.c"
    break;

  case 430: /* atom: LIT_STR  */
#line 1784 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6136 "raku.tab.c"
    break;

  case 431: /* atom: WORDLIST  */
#line 1786 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6149 "raku.tab.c"
    break;

  case 432: /* atom: LIT_INTERP_STR  */
#line 1794 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6155 "raku.tab.c"
    break;

  case 433: /* atom: VAR_SCALAR  */
#line 1795 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6161 "raku.tab.c"
    break;

  case 434: /* atom: OP_INC VAR_SCALAR  */
#line 1796 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6167 "raku.tab.c"
    break;

  case 435: /* atom: OP_DEC VAR_SCALAR  */
#line 1797 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6173 "raku.tab.c"
    break;

  case 436: /* atom: VAR_SCALAR OP_INC  */
#line 1798 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6179 "raku.tab.c"
    break;

  case 437: /* atom: VAR_SCALAR OP_DEC  */
#line 1799 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6185 "raku.tab.c"
    break;

  case 438: /* atom: VAR_ARRAY  */
#line 1800 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6191 "raku.tab.c"
    break;

  case 439: /* atom: VAR_HASH  */
#line 1801 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6197 "raku.tab.c"
    break;

  case 440: /* atom: VAR_CAPTURE  */
#line 1803 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6205 "raku.tab.c"
    break;

  case 441: /* atom: VAR_FH  */
#line 1807 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6213 "raku.tab.c"
    break;

  case 442: /* atom: VAR_NAMED_CAPTURE  */
#line 1811 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6220 "raku.tab.c"
    break;

  case 443: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1814 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6226 "raku.tab.c"
    break;

  case 444: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1816 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6232 "raku.tab.c"
    break;

  case 445: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1818 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6238 "raku.tab.c"
    break;

  case 446: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1820 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6244 "raku.tab.c"
    break;

  case 447: /* atom: ARR_ALL_SLICE  */
#line 1822 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6250 "raku.tab.c"
    break;

  case 448: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1824 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6256 "raku.tab.c"
    break;

  case 449: /* atom: VAR_HASH '{' expr '}'  */
#line 1826 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6262 "raku.tab.c"
    break;

  case 450: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1828 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6268 "raku.tab.c"
    break;

  case 451: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1830 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6274 "raku.tab.c"
    break;

  case 452: /* atom: IDENT  */
#line 1831 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6280 "raku.tab.c"
    break;

  case 453: /* atom: VAR_TWIGIL  */
#line 1833 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6288 "raku.tab.c"
    break;

  case 454: /* atom: VAR_ARRAY_TWIGIL  */
#line 1837 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6296 "raku.tab.c"
    break;

  case 455: /* atom: VAR_HASH_TWIGIL  */
#line 1841 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6304 "raku.tab.c"
    break;

  case 456: /* atom: '[' ']'  */
#line 1844 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6310 "raku.tab.c"
    break;

  case 457: /* atom: '[' expr ']'  */
#line 1846 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6316 "raku.tab.c"
    break;

  case 458: /* atom: '[' expr ',' ']'  */
#line 1848 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6322 "raku.tab.c"
    break;

  case 459: /* atom: '[' expr ',' arg_list ']'  */
#line 1850 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6329 "raku.tab.c"
    break;

  case 460: /* atom: '(' ')'  */
#line 1852 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6335 "raku.tab.c"
    break;

  case 461: /* atom: '(' expr ')'  */
#line 1853 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6341 "raku.tab.c"
    break;

  case 462: /* atom: '(' expr ',' ')'  */
#line 1855 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6347 "raku.tab.c"
    break;

  case 463: /* atom: '(' expr ',' arg_list ')'  */
#line 1857 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6354 "raku.tab.c"
    break;

  case 464: /* atom: block  */
#line 1859 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6360 "raku.tab.c"
    break;

  case 465: /* atom: KW_SUB block  */
#line 1860 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6366 "raku.tab.c"
    break;


#line 6370 "raku.tab.c"

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

#line 1862 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
