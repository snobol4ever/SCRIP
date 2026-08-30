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
#define YYLAST   4943

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  467
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1178

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
    1514,  1515,  1516,  1517,  1518,  1519,  1524,  1529,  1531,  1534,
    1536,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,  1547,
    1548,  1549,  1550,  1551,  1552,  1553,  1559,  1565,  1571,  1574,
    1575,  1578,  1579,  1580,  1583,  1585,  1588,  1589,  1590,  1593,
    1594,  1597,  1598,  1599,  1602,  1603,  1604,  1607,  1608,  1609,
    1610,  1611,  1613,  1615,  1618,  1619,  1620,  1621,  1622,  1627,
    1630,  1631,  1634,  1635,  1638,  1639,  1640,  1641,  1642,  1643,
    1644,  1645,  1646,  1647,  1648,  1649,  1651,  1653,  1659,  1664,
    1665,  1667,  1669,  1674,  1676,  1682,  1688,  1692,  1696,  1701,
    1708,  1713,  1719,  1725,  1732,  1739,  1744,  1749,  1756,  1761,
    1766,  1768,  1770,  1772,  1774,  1776,  1778,  1781,  1782,  1783,
    1786,  1787,  1788,  1789,  1798,  1799,  1800,  1801,  1802,  1803,
    1804,  1805,  1806,  1810,  1814,  1817,  1819,  1821,  1823,  1825,
    1827,  1829,  1831,  1833,  1835,  1836,  1840,  1844,  1848,  1849,
    1851,  1853,  1856,  1857,  1858,  1860,  1863,  1864
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

#define YYPACT_NINF (-861)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-467)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -861,    42,  2271,  -861,  -861,  -861,  -861,  -861,   939,   -58,
     121,   -49,    24,  -861,  -861,   900,  -861,  -861,  -861,    66,
    2346,   268,  3857,  3932,  4007,  4082,  4157,     0,     9,  4157,
    2421,  2496,   258,    72,  4157,  4157,   139,   163,  4232,  4307,
      62,   202,    70,   285,    95,    95,  4382,    62,    62,  4157,
     248,   284,    17,   900,  -861,  -861,   360,   374,   391,   900,
     900,   900,  -861,  2571,  2647,   315,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,   543,  -861,  -861,  -861,
    4814,   637,   336,   546,   656,   477,   321,  -861,   -25,   369,
     488,   580,  -861,  -861,   341,   340,   382,   451,  -861,  -861,
    4157,  4157,  4157,  4157,  4157,  4157,   528,  2722,  4457,  2798,
     561,  4157,  4157,   402,    26,  1157,   240,   426,   152,  -861,
     178,    62,  2873,  -861,  -861,   490,   791,   181,  -861,  2949,
    -861,   141,   -47,   110,   270,   725,   660,   396,   643,  2873,
     524,  2873,    69,  2873,    62,  2873,    62,    21,    47,   420,
    -861,  -861,  -861,   362,  -861,   308,  -861,   309,   594,   603,
     628,   624,   515,   241,   242,  2873,    62,  2873,    62,   332,
    3025,  -861,  4157,  4157,  -861,  4157,  4157,  -861,  4157,  3100,
    4157,  3175,  4157,  -861,   729,  -861,  -861,  -861,   684,   645,
      27,  -861,   695,  -861,  -861,  -861,  -861,  -861,   659,  -861,
     680,   708,  -861,   372,   714,  1641,  4157,  4157,  4157,  4157,
    4157,  4157,  4157,  4157,  -861,  4157,  4157,  4157,  4157,  4157,
    4157,  4157,  4157,  -861,  4157,   900,   900,   900,   900,   900,
     900,   900,   900,   900,   900,   900,   900,   900,   900,   900,
     796,   900,   900,   900,   900,   900,   900,   900,   900,   900,
     900,   900,   900,   900,   900,   900,   900,   900,   900,   810,
     785,   760,   728,   735,   744,   800,   826,   828,   -23,  -861,
     776,  2873,   446,   805,   563,   816,   806,   833,  -861,   834,
     957,   844,    92,  -861,   968,   801,   807,  2798,   972,  4157,
    -861,  4157,  4157,  4157,  4157,  4157,  4157,  4157,   892,   680,
     809,  -861,  4157,  4157,  -861,  4532,  -861,  4607,  -861,   282,
     287,   307,   465,   884,   914,  -861,   813,   815,  4157,  4157,
    4157,  4157,  4157,  4157,  4157,  -861,  4157,   817,  4157,  4157,
    -861,  4157,   836,   946,   845,  -861,  1025,  4157,  -861,   900,
     900,     2,  -861,  -861,  -861,  4157,  4157,  4157,  -861,  4157,
    4157,  -861,  4157,  4157,   907,  4157,  -861,  1033,  4157,  1057,
    4157,   848,  1012,   852,  -861,  4157,  4157,  1059,   943,   954,
     962,   975,   982,   979,  4157,  -861,  -861,  -861,    62,    16,
    -861,    49,   989,   990,  -861,  -861,  3250,   987,  1114,  3326,
    -861,  -861,   973,   251,   255,  1009,  3857,  3932,  4157,  1986,
     100,   342,  -861,   323,   996,  1006,  1007,  1008,  1010,  1011,
    1013,  1014,  1015,  1020,  1021,  1022,  1023,  1024,  1026,  1027,
    -861,  1030,  1030,  1030,  1030,  1030,  1030,  1038,  1030,  1030,
    1030,  1030,  1030,  1030,  1030,  1030,  -861,  -861,  -861,   477,
    -861,  -861,  -861,  1036,  1036,   369,   488,   488,   580,   580,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  1028,
    1141,  1034,  -861,  3401,  -861,  -861,  -861,  -861,  -861,  -861,
    4157,  3477,  -861,   856,  -861,  4157,  4157,  4157,  4157,  1048,
    1050,  1062,  -861,  3553,  -861,  1293,  4157,  1049,  -861,    -8,
    -861,  1505,   620,  1060,  1044,  -861,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  1052,  -861,  1053,  2873,   542,  1104,   179,
    3629,   639,  1061,  4157,  -861,  4682,  -861,  4157,  -861,    54,
    -861,  4157,  4157,  1071,  3250,  1063,  1064,  1066,  1067,  1068,
    1070,  1072,   640,  3250,  1074,  1075,   641,    62,    -1,  4757,
    -861,    62,    62,    57,   -38,   -34,    -4,  -861,   151,  -861,
    -861,  -861,  1054,   858,  1776,   663,  1076,  1077,  1078,  1082,
    1083,  1086,  -861,  1084,    10,  1080,  1081,  1100,  1085,    62,
      62,    62,  1090,  1091,  1087,  4157,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  1178,  1211,  -861,    19,  1054,   861,    63,
     535,   188,  -861,   864,  4157,  -861,  -861,   662,  1212,  2798,
    4157,  4157,   379,    29,    99,  -861,   361,  4157,  4157,  -861,
    4157,  4157,  -861,  4157,  4157,  4157,  4157,  4157,  4157,  4157,
    4157,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,   900,  3705,
    -861,  1429,  1097,   866,  1098,  -861,   871,  3250,   681,  1095,
    1096,   726,  4157,  4157,  4157,  -861,   873,  -861,   875,   879,
    -861,  4157,  1152,  1219,   881,  -861,  -861,  -861,  -861,  -861,
     883,  -861,  4157,  4157,  4157,   886,  -861,   153,  -861,  1105,
    2873,   703,  1106,  1054,   888,  1107,  1108,  4157,   893,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,   895,  -861,  -861,
    -861,  1005,  -861,  -861,  2873,    62,  -861,  -861,  1224,  -861,
    1229,  1230,  1227,  4157,  1126,  1233,  1234,  -861,  1054,   577,
    3857,  3932,  2061,  -861,   404,  -861,  -861,  -861,  -861,  -861,
    -861,  -861,  4157,    62,  -861,  -861,  -861,  1115,  1116,  1220,
    -861,  -861,  -861,  -861,  4157,  1118,  -861,  -861,   277,   689,
     812,  1190,  -861,  -861,  1054,  1054,   917,   919,  1236,  1237,
    1238,  -861,  -861,  1124,  -861,   144,   790,  1119,   -16,  4157,
    4157,  4157,  4157,  4157,  4157,  -861,  -861,  -861,  -861,    64,
     209,   325,   329,   331,   350,   351,   366,   371,   376,   380,
     389,  -861,  -861,   921,  -861,   924,   926,  -861,  1125,  -861,
    -861,   928,  -861,  -861,  -861,  -861,  1130,  1131,  1132,  -861,
    -861,  -861,  1133,  4157,  1135,  -861,  3250,   712,  -861,  -861,
    1134,  1183,  1187,  -861,   931,  -861,  4157,  -861,  -861,  1054,
    -861,  -861,   716,  1138,  1139,    -1,  -861,   933,  1055,    62,
      62,    62,  -861,  -861,  4157,  -861,  -861,  -861,    -3,  -861,
     154,  -861,  -861,  -861,   406,    59,  -861,   385,  4157,  4157,
    4157,  4157,  4157,  4157,  4157,  4157,  -861,    62,  1136,  -861,
    -861,    62,  1140,  4157,  -861,   507,   584,    -5,    67,   911,
    1142,  1145,   810,  -861,  -861,  1054,  1054,  1264,  1269,  1270,
    -861,  4157,  3781,  1162,  1163,  -861,   390,   394,   403,   407,
     418,   422,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861,  1153,  -861,
    -861,  -861,  -861,  -861,  4157,   958,  -861,  -861,  4157,  4157,
    3250,   720,  -861,  -861,  4157,  -861,  -861,  -861,  -861,    62,
      62,  -861,  -861,  -861,  -861,  -861,  1271,  4157,  1168,  1277,
    1280,  4157,  4157,  4157,  -861,  -861,  -861,   427,   431,   441,
     444,   462,   463,   476,   480,  -861,  -861,  -861,  4157,  1160,
    -861,    80,  1225,  -861,   174,  -861,  1287,  1288,  1164,  4157,
    -861,  1300,  1301,  1186,  4157,  -861,   259,   310,  1188,  1189,
    -861,  -861,   623,  -861,  -861,  -861,  -861,  -861,   499,  1180,
     964,  4157,  4157,  -861,  -861,  -861,  -861,  -861,  -861,  -861,
    1191,  1192,  -861,  -861,   966,  -861,   727,  1073,  -861,  -861,
    -861,  4157,  -861,  -861,   518,   519,   522,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  1196,    62,  1185,   974,  1193,
    1911,  1185,   976,  1197,  1198,  -861,  1199,  1202,  1203,  -861,
    1204,  1321,  1322,  1208,  4157,  -861,  1325,  1326,  1213,  4157,
    -861,  -861,  -861,   244,  -861,  -861,  -861,  1205,   540,   541,
    -861,  -861,  1214,  -861,    62,  -861,  -861,  -861,  -861,  -861,
    4157,  -861,  -861,  1185,  -861,  3857,  3932,  2136,  -861,   428,
    -861,  1185,  -861,  -861,  -861,  -861,  -861,  -861,  1218,  1221,
    -861,  1222,  1223,  1226,  -861,  1228,  1185,   978,  -861,  -861,
    -861,  -861,  -861,  1209,  -861,   447,   367,  -861,   398,  4157,
    4157,  4157,  4157,  4157,  4157,  4157,  4157,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  1185,    62,  4157,  4157,
    4157,  -861,  -861,  -861,   555,   556,   557,   567,   571,   579,
     587,   588,  -861,  -861,   601,   602,   621,  -861,  -861,  -861,
    -861,  -861,  -861,  -861,  -861,  -861,  -861,  -861
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   430,   431,   432,   434,   435,   440,
     441,   455,   454,   456,   457,     0,   442,   443,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   449,   433,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     3,     4,   100,   101,   108,
     109,   110,   111,   102,   103,   118,     0,   119,   120,   121,
     107,     0,   328,   330,   348,   350,   353,   355,   358,   360,
     363,   366,   373,   379,   381,   396,   426,     0,   438,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   440,   441,   455,
     454,     0,     0,   466,   377,     0,   435,   440,    35,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   358,     0,
     467,   326,   325,     0,    49,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
       0,   147,     0,     0,   112,     0,     0,   113,     0,     0,
       0,     3,     0,   424,   104,   106,   420,   211,     0,     0,
       0,   378,     0,   436,   437,   376,   375,   374,   435,   462,
       0,     0,   458,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   320,   321,   322,   323,   324,   318,     0,   403,
       0,     0,   319,     0,     0,     0,     0,     0,    39,   408,
       0,   407,   454,   399,     0,     0,     0,     0,     0,     0,
      29,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,    38,   429,     0,     6,     0,    10,     0,    11,     0,
       0,     0,     0,     0,     0,   382,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,     0,     0,     0,     0,
      43,     0,     0,   125,     0,   137,     0,     0,   156,     0,
       0,     0,     3,   163,    46,     0,     0,     0,    48,     0,
       0,    52,     0,     0,     0,     0,   159,     0,     0,     0,
       0,     0,   140,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   421,   423,   425,     0,     0,
     214,     0,     0,     0,   249,   463,     0,     0,     0,     0,
     459,   284,   435,   440,   441,   455,     0,     0,     0,     0,
       0,     0,   283,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     327,   333,   334,   337,   338,   331,   332,     0,   335,   336,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   349,
     351,   352,   354,   356,   357,   359,   361,   362,   364,   365,
     370,   371,   372,   367,   368,   369,   380,   384,   395,   388,
     389,   390,   391,   394,   392,   393,   386,   387,   385,   419,
       0,   416,    59,     0,    93,    94,    95,    96,    97,    56,
       0,     0,   402,     0,    15,     0,     0,     0,     0,   445,
     450,   451,    61,     0,   411,     0,     0,     0,   400,     0,
     398,   429,     0,     0,     0,   320,   321,   322,   323,   324,
     318,   319,    37,     0,   428,     0,     0,     0,   432,   454,
       0,     0,     0,     0,    26,     0,    27,     0,    28,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
     128,   463,     0,     0,   356,   357,   261,   264,     0,   277,
     279,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,   397,     0,     0,     0,     0,     0,   463,
       0,   463,     0,     0,     0,     0,   114,   115,   116,   117,
     317,   422,   105,     0,     0,   214,     0,     0,     0,     0,
       0,     0,   464,     0,     0,   383,   460,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,     0,   311,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,   285,    63,    65,    67,    70,    68,    69,    64,    66,
      76,    78,    80,    83,    81,    82,    77,    79,     0,     0,
     412,     0,     0,     0,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   410,     0,   406,     0,     0,
     253,     0,     0,     0,     0,   445,   450,   451,    36,     5,
       0,    12,     0,     0,     0,     0,    19,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
      87,    86,    90,    88,    89,    85,    41,     0,    91,    92,
      44,   122,   127,   126,     0,     0,   136,   153,     0,   155,
       0,     0,     0,     0,   266,     0,     0,   162,     0,     0,
       0,     0,     0,   174,     0,    47,    50,    51,    54,    55,
      31,    30,     0,     0,   157,   452,   453,     0,     0,   138,
     141,   142,   145,   146,     0,     0,   212,   213,     0,     0,
       0,     0,   210,   171,     0,     0,     0,     0,     0,     0,
       0,   248,   465,     0,   461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,   301,   308,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,   418,     0,   415,     0,     0,    58,     0,    60,
     172,     0,    16,   448,   447,   446,     0,     0,     0,   409,
     404,   405,     0,     0,     0,   401,     0,     0,   258,   257,
       0,     0,     0,    21,     0,    22,     0,    25,   165,     0,
      33,    32,     0,   465,   465,     0,   129,     0,   130,     0,
       0,     0,   262,   273,     0,   267,   268,   161,   269,   265,
       0,   278,   280,   282,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,     0,     0,    74,
      75,     0,     0,     0,   209,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   168,     0,     0,     0,     0,     0,
       9,     0,     0,   445,   451,   305,     0,     0,     0,     0,
       0,     0,   313,   314,   315,   316,   286,   288,   290,   293,
     291,   292,   287,   289,   417,   414,   413,    57,   465,    71,
      72,    73,   254,   255,     0,     0,    13,    20,     0,     0,
       0,     0,   164,     7,     0,    42,    45,   124,   123,   463,
       0,   134,   154,   151,   152,   275,     0,     0,   270,     0,
       0,     0,     0,     0,   178,   179,   176,     0,     0,     0,
       0,     0,     0,     0,     0,   160,   158,   139,     0,     0,
     150,     0,     3,   244,     0,   241,     0,     0,     0,     0,
     220,     0,     0,     0,     0,   215,     0,     0,     0,     0,
     216,   217,     0,   167,   169,   250,   251,   252,     0,   173,
       0,     0,     0,   295,   297,   300,   298,   299,   296,    17,
       0,   465,   260,   259,     0,    23,     0,   131,   132,   263,
     274,     0,   271,   272,     0,     0,     0,   180,   182,   184,
     187,   185,   186,   181,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   222,     0,     0,     0,     0,
     221,   218,   219,     0,   247,   304,   303,   172,     0,     0,
     256,    14,   465,     8,     0,   135,   276,   188,   190,   189,
       0,   149,   243,     0,   192,     0,     0,     0,   191,     0,
     240,     0,   229,   230,   224,   227,   228,   223,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,   302,   306,
     307,    24,   133,     0,   242,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   193,   239,   237,
     238,   226,   235,   236,   225,   246,     0,     0,     0,     0,
       0,   196,   197,   194,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   148,     0,     0,     0,   198,   200,   202,
     205,   203,   204,   199,   201,   206,   208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -861,  -861,     1,  -861,  -543,  -704,  -861,  -861,  -861,  -861,
    -861,   264,  1327,  -861,  -861,  -861,  -861,  -301,  -860,  -861,
    -861,  -861,   754,  -861,  -861,  -491,   837,  -347,   236,  1069,
      -2,  -229,  -861,   650,  1120,  -861,   658,   -11,  1117,   870,
     872,    -7,  -861,  1231,  -257,  -861,  -861,   313,  -861
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   205,    66,    67,   550,    68,    69,    70,    71,
      72,   979,    73,    74,   574,    75,    76,   343,   983,    77,
      78,   379,   596,    79,   601,   285,   521,   563,   123,   179,
     130,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,   201,   469,    94,    95,   131,    96
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      81,     2,   427,   471,   668,   712,   672,   846,   124,   986,
     674,   530,   556,   557,   149,   148,   558,   987,   722,   956,
     140,   142,   144,   146,   147,    24,   985,   153,   155,   157,
     593,   189,   161,   162,   598,   113,   166,   168,   594,    26,
     279,   382,     3,   280,   183,   720,   191,   186,   190,   721,
     742,   743,   195,   196,   197,   328,   245,   246,   108,   556,
     557,   200,   203,   558,   556,   557,   988,   112,   558,   303,
     559,   560,   561,   556,   557,   329,   109,   558,   759,   760,
     125,   991,   761,   304,   247,   328,   160,   281,   247,   992,
     556,   557,   383,   480,   558,   328,   172,   247,   262,   263,
     264,   265,   266,   267,   336,   329,   272,   274,   481,   276,
     277,   989,   723,   957,   492,   329,   173,   559,   560,   561,
     200,   905,   559,   560,   561,   990,   617,   299,   339,   340,
     673,   559,   560,   561,   562,    65,    65,   317,   993,   327,
     718,   332,  1074,   334,   951,    65,   618,   744,   559,   560,
     561,   163,   595,   114,   337,   115,   762,    65,   831,   330,
     805,   724,   331,   361,   958,   363,   786,   832,   368,   247,
     369,   370,   496,   371,   372,   164,   373,   375,   376,   373,
     377,   597,   694,   994,   556,   557,   693,  1092,   558,   330,
     302,  1100,   331,    65,   586,   765,   965,   995,    65,   330,
     174,   912,   331,   403,   404,   405,   406,   407,   408,   409,
     410,   411,  1047,   412,   413,   414,   415,   416,   417,   418,
     419,   114,   420,   115,   725,   726,   305,   959,   960,   344,
     174,   178,   345,  1124,   443,   444,   787,   619,    80,   110,
     306,  1138,   559,   560,   561,   450,   451,   452,   453,   454,
     455,   456,   766,   767,   556,   557,  1145,   111,   558,   684,
     901,   727,   187,   150,   152,   768,   769,   770,   158,   483,
     288,   301,   159,  1061,   302,   902,   169,   171,   132,   133,
     134,  1062,   135,   184,   185,   502,  1162,   504,   289,   505,
     506,   507,   508,   509,   510,   511,   763,   297,   188,   136,
     514,   515,   947,   517,   137,   522,  1051,   114,   114,   115,
     115,   175,   559,   560,   561,   287,   535,   536,   537,   538,
     539,   540,   541,  1085,  1066,   771,   544,   545,   554,   555,
    1063,   176,  1067,   170,   346,   349,   759,   760,    65,   587,
     761,    98,    99,   564,   566,   567,   913,   568,   569,   623,
     570,   571,   624,   625,   347,   350,   576,   150,   578,   357,
     359,   365,   626,   582,   583,   627,   628,   108,   620,   629,
     630,   107,   591,   110,   192,  1064,  1116,   358,   360,   366,
     333,  1068,   335,   338,   193,   609,   307,   346,   621,  1065,
     204,   610,   838,   328,   612,   613,   614,   616,   523,   138,
     308,   194,   362,   525,   364,   779,   313,   347,   319,   780,
     314,   346,   524,   329,   884,   177,   224,   526,   781,   801,
     270,   782,   783,   527,   346,   784,  1069,   857,   286,   244,
     868,   347,   961,   869,   870,   319,   962,   528,   348,   351,
    1070,    80,   300,   871,   347,   321,   872,   873,   322,   323,
     874,   875,   963,   223,  1129,   588,   258,  1130,  1131,   589,
     631,   640,   914,   893,   894,   261,   915,  1132,   916,   259,
    1133,  1134,   177,  1148,  1135,  1136,   319,  1149,   654,   622,
     641,   642,   248,   249,   659,   660,   321,   917,   918,   322,
     323,   348,   344,  1150,   670,   345,   643,   330,   788,   514,
     331,   644,   886,   919,  1152,   389,   645,   390,   920,   325,
     646,   260,   326,   921,   680,   348,   785,   922,   200,   647,
     699,   689,   966,   691,   701,   692,   923,  1013,   348,   695,
     696,  1014,   278,   702,   223,  1153,   325,   703,   942,   326,
    1015,   876,   268,   964,  1016,   556,   557,   715,   704,   558,
     318,   341,   705,   319,   320,  1017,   342,   640,   223,  1018,
     287,   641,   734,   321,  1037,  1137,   322,   323,  1038,   206,
     324,   642,   207,   208,   643,   275,   484,   325,  1039,   485,
     326,  1040,   209,   755,  1151,   210,   211,   858,   859,   212,
     213,   860,   644,   645,  1003,  1004,   529,   242,   243,  1041,
    1042,   342,   773,   559,   560,   561,   646,   776,   777,   778,
     647,   250,   251,  1043,   592,   789,   790,  1044,   791,   792,
     290,   793,   794,   795,   796,   797,   798,   799,   800,   809,
     225,   226,   227,   228,  1048,  1002,  1075,  1052,   981,   542,
     229,   230,   231,   982,   546,   861,   862,   863,   699,   701,
     553,   356,   705,   315,   325,  1087,  1088,   326,   565,  1089,
     816,   817,   818,   215,   232,   233,   216,   217,   573,   822,
     929,   931,   681,   214,   312,   682,   218,  1119,  1120,   219,
     220,   828,   829,   221,   222,   640,   641,   642,   834,   252,
     253,   254,  1167,  1168,  1169,   842,   488,   643,   489,   603,
     354,   644,   607,   457,  1170,   255,   256,   257,  1171,   645,
     352,   458,   847,   459,   460,   984,  1172,   646,   647,   353,
     982,   853,   461,   462,  1173,  1174,  1117,   463,   864,   865,
     867,   699,   701,   464,   465,   309,   310,   311,  1175,  1176,
     877,   466,   467,   468,   234,   235,   236,   237,   238,   239,
     885,   705,   882,   488,  1073,   675,   240,   355,  1177,   982,
      98,    99,   291,   292,   293,   294,   295,   223,   241,   686,
     706,   710,   687,   302,   302,   296,   381,   906,   907,   908,
     909,   910,   911,   711,   713,   378,   653,   716,   717,   719,
     107,  -382,  -382,   735,   656,   302,   302,   774,   658,   457,
      80,   661,   470,   436,   437,   438,   666,   458,   669,   459,
     460,   812,   385,   386,   302,   749,   750,   751,   461,   462,
     380,   933,   887,   463,   457,   888,   889,   890,   891,   464,
     465,   384,   458,   835,   459,   460,   836,   466,   467,   468,
     387,   388,   936,   461,   462,   302,   943,   698,   463,   944,
    1025,   391,   955,   302,   464,   465,   707,  1083,   474,   302,
     302,   815,   466,   467,   468,   475,   967,   968,   969,   970,
     971,   972,   973,   974,   476,   421,   422,   423,   424,   425,
     426,   980,   428,   429,   430,   431,   432,   433,   434,   435,
     472,   473,    98,    99,   291,   292,   293,   294,   295,  1008,
     440,   441,   442,     4,     5,     6,     7,   296,   482,   302,
     116,   117,   118,   119,   120,    13,    14,    15,    16,    17,
      18,   996,   107,   488,   997,   903,   998,   999,   486,   487,
     477,   121,  1020,   498,   499,   490,  1022,  1023,    34,   500,
     501,   513,   302,   491,    36,   533,   388,   385,   534,   385,
     543,   848,    44,    45,    46,  1030,   478,    49,   479,  1034,
    1035,  1036,   803,   492,   806,   493,    53,    54,   547,   386,
     811,   494,    55,   548,   549,   495,  1045,   551,   386,   878,
     579,   386,   497,  1050,   581,   386,   503,  1056,   385,   657,
     728,   729,  1060,   764,   729,   827,   772,   302,   808,   302,
     531,    57,    58,   810,   302,   819,   302,   820,   499,  1078,
    1079,   821,   302,   825,   499,   385,   826,    59,   830,   687,
     839,   729,   512,    60,    61,   843,   302,   844,   302,  1086,
     532,   122,   845,   549,    64,   552,    65,   572,    97,   580,
      98,    99,   100,   101,   102,   103,   104,   575,  1099,   895,
     729,   896,   729,   924,   302,   105,   925,   499,   926,   302,
     928,   302,  1111,   385,   940,   949,   386,  1115,   106,   584,
     107,   577,    97,   585,    98,    99,   100,   101,   102,   103,
     104,   948,   950,   549,   586,   952,   953,   954,   980,   105,
    1021,   302,   587,  1125,  1126,  1128,  1077,   302,  1082,   302,
    1084,   549,   608,   604,   107,   588,  1093,   729,  1101,   729,
    1146,   729,   589,   975,   180,   182,   590,   977,   446,   447,
     599,   600,   448,   449,   605,   611,   632,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,   648,   633,   634,   635,   935,
     636,   637,   241,   638,   639,   640,  1164,  1165,  1166,   941,
     641,   642,   643,   644,   645,   650,   646,   647,   247,   649,
       4,     5,     6,     7,   662,   651,   663,   126,   127,   118,
     119,   282,    13,    14,    15,    16,    17,    18,   664,   676,
     671,   677,   678,   679,   683,  1027,  1028,   697,   121,    28,
     342,   688,   756,   699,   700,    34,   701,   702,   703,   745,
     704,    36,   705,   754,   708,   709,   736,   737,   738,    44,
      45,    46,   739,   740,    49,  1010,   741,   302,   746,   747,
     752,   753,   748,    53,    54,   757,   775,   807,   809,    55,
     813,   814,   823,   824,   849,   833,   837,   840,   841,   850,
     851,   852,   854,   855,   856,   879,   880,   881,   883,   892,
     897,   898,   899,  1024,   900,   927,   904,  1026,    57,    58,
     929,   930,   931,   938,   937,   932,   934,   939,   945,   946,
     978,  1005,  1000,   976,    59,  1001,  1006,  1007,  1011,  1012,
      60,    61,  1091,  1019,  1031,  1029,    80,  1032,   122,   283,
    1033,    64,  1046,    65,  1055,   284,     4,     5,     6,     7,
    1049,  1053,  1054,   126,   127,   118,   119,   282,    13,    14,
      15,    16,    17,    18,  1057,  1058,  1059,  1076,  1071,  1072,
    1122,   982,  1081,  1080,   121,    28,  1090,  1102,  1103,  1104,
    1094,    34,  1105,  1106,  1107,  1108,  1109,    36,  1110,  1112,
    1113,  1147,  1118,  1114,  1121,    44,    45,    46,  1139,   758,
      49,  1140,  1141,  1142,  1123,   151,  1143,   685,  1144,    53,
      54,   439,     0,     0,   445,    55,     0,     0,     0,   316,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1163,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,     0,     0,   122,   667,     0,    64,     0,    65,
       0,   284,     4,     5,     6,     7,     0,     0,     0,   126,
     127,   118,   119,   282,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   282,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,    60,    61,     0,     0,     0,    44,    45,    46,
     122,   804,    49,    64,     0,    65,     0,   284,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,     0,     0,   122,     0,     0,    64,
       0,    65,     0,   284,     4,     5,     6,     7,     0,     0,
       0,   392,   393,   394,   395,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   396,   397,    24,     0,     0,
      25,    26,    27,    28,   398,   399,    31,    32,    33,    34,
      35,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,   400,   401,    44,    45,    46,    47,    48,    49,    50,
       0,     0,     0,    51,    52,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
       0,    62,    63,     0,     0,    64,     0,    65,   402,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     730,   731,    24,     0,     0,    25,    26,    27,    28,    29,
     732,    31,    32,    33,    34,    35,     0,     0,     0,     0,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,     0,    51,    52,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,    62,    63,     0,     0,
      64,     0,    65,   733,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,  1095,  1096,    24,     0,     0,
      25,    26,    27,    28,    29,  1097,    31,    32,    33,    34,
      35,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,     0,     0,    51,    52,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,    56,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,     0,    59,     0,
      36,     0,     0,     0,    60,    61,     0,     0,    44,    45,
      46,    62,    63,    49,     0,    64,     0,    65,  1098,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   126,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,    60,
      61,     0,     0,    44,    45,    46,   154,   122,    49,     0,
      64,     0,    65,   615,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,     0,    59,     0,
      36,     0,     0,     0,    60,    61,     0,     0,    44,    45,
      46,   154,   122,    49,     0,    64,     0,    65,   866,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,     0,   154,   122,     0,     0,
      64,     0,    65,  1127,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     0,     0,     0,     0,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,     0,     0,    51,    52,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,    56,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,     0,    59,     0,
      36,     0,     0,     0,    60,    61,     0,     0,    44,    45,
      46,    62,    63,    49,     0,    64,     0,    65,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   126,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,    60,
      61,     0,     0,    44,    45,    46,   128,   129,    49,     0,
      64,     0,    65,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,     0,    59,     0,
      36,     0,     0,     0,    60,    61,     0,     0,    44,    45,
      46,   154,   122,    49,     0,    64,     0,    65,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   198,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,    60,
      61,     0,     0,    44,    45,    46,   156,   122,    49,     0,
      64,     0,    65,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,    60,    61,     0,     0,     0,    44,
      45,    46,   122,   199,    49,    64,     0,    65,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   122,    49,
       0,    64,   202,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   126,   127,
     118,   119,   120,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,   121,
      28,     0,     0,     0,     0,     0,    34,     0,     0,    59,
       0,     0,    36,     0,     0,    60,    61,     0,     0,     0,
      44,    45,    46,   122,   269,    49,    64,     0,    65,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   126,   127,   118,   119,   120,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   121,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,    60,    61,   273,     0,    44,    45,    46,     0,   122,
      49,     0,    64,     0,    65,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   126,
     127,   118,   119,   120,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
      59,     0,     0,    36,     0,     0,    60,    61,     0,     0,
       0,    44,    45,    46,   122,   199,    49,    64,     0,    65,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,   367,     0,
      57,    58,     0,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,    60,    61,     0,     0,     0,    44,    45,    46,
     122,   298,    49,    64,     0,    65,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   122,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    59,     0,    36,
       0,     0,     0,    60,    61,     0,     0,    44,    45,    46,
       0,   122,    49,   374,    64,     0,    65,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
     204,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     126,   127,   118,   119,   120,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   121,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,    60,    61,
       0,     0,    44,    45,    46,     0,   122,    49,     0,    64,
       0,    65,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   126,   127,   118,   119,
     120,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,   121,    28,     0,
       0,     0,     0,     0,    34,     0,     0,    59,     0,     0,
      36,     0,     0,    60,    61,     0,     0,     0,    44,    45,
      46,   122,   602,    49,    64,     0,    65,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   126,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,    60,
      61,     0,     0,    44,    45,    46,     0,   122,    49,     0,
      64,   606,    65,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,    60,    61,     0,     0,     0,    44,
      45,    46,   122,   652,    49,    64,     0,    65,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   126,   127,   118,   119,   120,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,   121,    28,     0,     0,     0,     0,
       0,    34,     0,     0,    59,     0,     0,    36,     0,     0,
      60,    61,     0,     0,     0,    44,    45,    46,   122,   655,
      49,    64,     0,    65,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     4,     5,   518,     7,     0,     0,     0,   126,
     127,   118,   119,   519,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
     121,    28,     0,     0,     0,     0,     0,    34,     0,     0,
      59,     0,     0,    36,     0,     0,    60,    61,     0,     0,
       0,    44,    45,    46,   122,   665,    49,    64,     0,    65,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   126,   127,   118,   119,   120,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   121,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,    60,    61,     0,     0,     0,    44,    45,    46,
     122,   199,    49,    64,     0,    65,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   126,   127,   118,   119,   120,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,   121,    28,     0,     0,     0,     0,     0,    34,
       0,     0,    59,     0,     0,    36,     0,     0,    60,    61,
       0,     0,     0,    44,    45,    46,   122,   802,    49,    64,
       0,    65,     0,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,    60,    61,     0,     0,     0,    44,
      45,    46,   122,  1009,    49,    64,     0,    65,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   139,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   141,    49,     0,    64,     0,    65,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   143,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   145,    49,     0,    64,     0,    65,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   122,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   165,    49,     0,    64,     0,    65,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   167,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   122,    49,     0,    64,     0,   181,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   271,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,   518,     7,     0,     0,     0,   126,   127,   118,
     119,   519,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   516,    49,     0,    64,     0,    65,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   126,   127,   118,   119,   120,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   121,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
      60,    61,     0,     0,    44,    45,    46,     0,   520,    49,
       0,    64,     0,    65,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   126,   127,   118,
     119,   120,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   121,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,    60,    61,     0,     0,    44,
      45,    46,     0,   690,    49,     0,    64,     0,    65,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -466,  -466,    57,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,     0,     0,   714,     0,
       0,    64,     0,    65,  -466,  -466,  -466,     0,  -466,  -466,
    -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
    -466,     0,     0,     0,  -466,     0,     0,     0,     0,     0,
       0,     0,  -466,  -466,  -466,  -466,  -466,  -466,  -466,  -466,
       0,     0,  -466,  -466,  -466,  -466,  -466,     0,     0,  -466,
    -466,  -466,     0,  -466
};

static const yytype_int16 yycheck[] =
{
       2,     0,   231,   260,   495,   548,    14,   711,    15,    14,
     501,   312,    10,    11,    14,    26,    14,    22,    22,    22,
      22,    23,    24,    25,    26,    26,   886,    29,    30,    31,
      14,    14,    34,    35,   381,    11,    38,    39,    22,    30,
      14,    14,     0,    17,    46,    83,    53,    49,    31,    83,
      40,    41,    59,    60,    61,    26,    81,    82,   116,    10,
      11,    63,    64,    14,    10,    11,    71,   116,    14,   116,
      68,    69,    70,    10,    11,    46,   134,    14,    59,    60,
      14,    14,    63,   130,   122,    26,    14,    61,   122,    22,
      10,    11,    65,   116,    14,    26,    26,   122,   100,   101,
     102,   103,   104,   105,    83,    46,   108,   109,   131,   111,
     112,   116,   116,   116,   130,    46,    46,    68,    69,    70,
     122,   137,    68,    69,    70,   130,    26,   129,    81,    82,
     138,    68,    69,    70,   132,   136,   136,   139,    71,   141,
      83,   143,  1002,   145,   848,   136,    46,   137,    68,    69,
      70,    12,   136,   129,   133,   131,   137,   136,     5,   130,
     651,    10,   133,   165,    10,   167,   137,    14,   170,   122,
     172,   173,    80,   175,   176,    12,   178,   179,   180,   181,
     182,   132,   529,   116,    10,    11,   132,  1047,    14,   130,
     133,  1051,   133,   136,   130,   132,   137,   130,   136,   130,
     130,   137,   133,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   132,   215,   216,   217,   218,   219,   220,   221,
     222,   129,   224,   131,    73,    74,   116,    73,    74,   130,
     130,   136,   133,  1093,   245,   246,   137,   137,     2,   118,
     130,  1101,    68,    69,    70,   252,   253,   254,   255,   256,
     257,   258,   599,   600,    10,    11,  1116,   136,    14,    80,
     116,   562,    14,    27,    28,    77,    78,    79,    10,   271,
     118,   130,    14,    14,   133,   131,    40,    41,    10,    11,
      12,    22,    14,    47,    48,   287,  1146,   289,   136,   291,
     292,   293,   294,   295,   296,   297,   597,   116,    14,    31,
     302,   303,   845,   305,    36,   307,   132,   129,   129,   131,
     131,    26,    68,    69,    70,   134,   318,   319,   320,   321,
     322,   323,   324,  1027,    14,   137,   328,   329,   339,   340,
      71,    46,    22,   131,    26,    26,    59,    60,   136,   130,
      63,   101,   102,   342,   346,   347,   137,   349,   350,    26,
     352,   353,    29,    30,    46,    46,   358,   121,   360,   118,
     118,    29,    39,   365,   366,    42,    43,   116,    26,    46,
      47,   131,   374,   118,    14,   116,   132,   136,   136,    47,
     144,    71,   146,   147,    10,   134,   116,    26,    46,   130,
      75,   136,   693,    26,   396,   397,   398,   399,   116,   131,
     130,    10,   166,   116,   168,    26,    10,    46,    29,    30,
      14,    26,   130,    46,   137,   130,    80,   130,    39,   648,
     107,    42,    43,   116,    26,    46,   116,   728,   115,   108,
      26,    46,    26,    29,    30,    29,    30,   130,   130,   130,
     130,   205,   129,    39,    46,    39,    42,    43,    42,    43,
      46,    47,    46,   130,    26,   130,   115,    29,    30,   130,
     137,   130,   137,   764,   765,    14,   137,    39,   137,   129,
      42,    43,   130,    26,    46,    47,    29,    30,   480,   137,
     130,   130,   113,   114,   486,   487,    39,   137,   137,    42,
      43,   130,   130,    46,   496,   133,   130,   130,   137,   501,
     133,   130,   759,   137,   137,   133,   130,   135,   137,   130,
     130,   129,   133,   137,   516,   130,   137,   137,   520,   130,
     130,   523,   137,   525,   130,   527,   137,   137,   130,   531,
     532,   137,   130,   130,   130,   137,   130,   130,   839,   133,
     137,   137,    14,   137,   137,    10,    11,   549,   130,    14,
      26,   131,   130,    29,    30,   137,   136,   130,   130,   137,
     134,   130,   564,    39,   137,   137,    42,    43,   137,    26,
      46,   130,    29,    30,   130,    14,   130,   130,   137,   133,
     133,   137,    39,   585,   137,    42,    43,    10,    11,    46,
      47,    14,   130,   130,   895,   896,   131,   120,   121,   137,
     137,   136,   604,    68,    69,    70,   130,   609,   610,   611,
     130,   123,   124,   137,   378,   617,   618,   137,   620,   621,
     130,   623,   624,   625,   626,   627,   628,   629,   630,   130,
      84,    85,    86,    87,   981,   892,   137,   984,   131,   326,
      94,    95,    96,   136,   331,    68,    69,    70,   130,   130,
     337,   136,   130,    10,   130,   137,   137,   133,   345,   137,
     662,   663,   664,    26,   118,   119,    29,    30,   355,   671,
     130,   130,   130,   130,    14,   133,    39,   137,   137,    42,
      43,   683,   684,    46,    47,   130,   130,   130,   690,   109,
     110,   111,   137,   137,   137,   697,   133,   130,   135,   386,
      72,   130,   389,    14,   137,   125,   126,   127,   137,   130,
     116,    22,   714,    24,    25,   131,   137,   130,   130,   116,
     136,   723,    33,    34,   137,   137,  1073,    38,   730,   731,
     732,   130,   130,    44,    45,    10,    11,    12,   137,   137,
     742,    52,    53,    54,    88,    89,    90,    91,    92,    93,
      61,   130,   754,   133,   131,   135,   100,   133,   137,   136,
     101,   102,   103,   104,   105,   106,   107,   130,   112,   130,
     130,   130,   133,   133,   133,   116,   131,   779,   780,   781,
     782,   783,   784,   547,   548,    56,   473,   551,   552,   553,
     131,   132,   133,   130,   481,   133,   133,   135,   485,    14,
     564,   488,    17,     7,     8,     9,   493,    22,   495,    24,
      25,   130,   132,   133,   133,   579,   580,   581,    33,    34,
     136,   823,    10,    38,    14,    13,    14,    15,    16,    44,
      45,   136,    22,   130,    24,    25,   133,    52,    53,    54,
     132,   133,   130,    33,    34,   133,   130,   534,    38,   133,
     130,   137,   854,   133,    44,    45,   543,   130,   130,   133,
     133,   135,    52,    53,    54,   130,   868,   869,   870,   871,
     872,   873,   874,   875,   130,   225,   226,   227,   228,   229,
     230,   883,   232,   233,   234,   235,   236,   237,   238,   239,
     130,   131,   101,   102,   103,   104,   105,   106,   107,   901,
     242,   243,   244,     3,     4,     5,     6,   116,   132,   133,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    10,   131,   133,    13,   135,    15,    16,   123,   124,
     130,    31,   934,   132,   133,   119,   938,   939,    38,   132,
     133,   132,   133,   137,    44,   132,   133,   132,   133,   132,
     133,   715,    52,    53,    54,   957,   130,    57,   130,   961,
     962,   963,   649,   130,   651,   131,    66,    67,   132,   133,
     657,    14,    72,    27,    28,   131,   978,   132,   133,   743,
     132,   133,    14,   982,   132,   133,    14,   989,   132,   133,
     132,   133,   994,   132,   133,   682,   132,   133,   132,   133,
     116,   101,   102,   132,   133,   132,   133,   132,   133,  1011,
    1012,   132,   133,   132,   133,   132,   133,   117,   132,   133,
     132,   133,   130,   123,   124,   132,   133,   132,   133,  1031,
     116,   131,    27,    28,   134,    10,   136,   130,    99,    27,
     101,   102,   103,   104,   105,   106,   107,    14,  1050,   132,
     133,   132,   133,   132,   133,   116,   132,   133,   132,   133,
     132,   133,  1064,   132,   133,   132,   133,  1069,   129,    10,
     131,    14,    99,   130,   101,   102,   103,   104,   105,   106,
     107,   845,    27,    28,   130,   849,   850,   851,  1090,   116,
     132,   133,   130,  1095,  1096,  1097,   132,   133,   132,   133,
      27,    28,   129,   116,   131,   130,   132,   133,   132,   133,
     132,   133,   130,   877,    45,    46,   137,   881,   248,   249,
     131,   131,   250,   251,    10,   116,   130,  1129,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,    97,   130,   130,   130,   826,
     130,   130,   112,   130,   130,   130,  1148,  1149,  1150,   836,
     130,   130,   130,   130,   130,    14,   130,   130,   122,   131,
       3,     4,     5,     6,   116,   131,   116,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,   116,   119,
     131,   137,   130,   130,    80,   949,   950,   116,    31,    32,
     136,   130,    14,   130,   130,    38,   130,   130,   130,   119,
     130,    44,   130,   116,   130,   130,   130,   130,   130,    52,
      53,    54,   130,   130,    57,   902,   130,   133,   137,   119,
     130,   130,   137,    66,    67,    14,    14,   130,   130,    72,
     135,   135,    80,    14,    10,   130,   130,   130,   130,    10,
      10,    14,   116,    10,    10,   130,   130,    27,   130,    59,
      14,    14,    14,   940,   130,   130,   137,   944,   101,   102,
     130,   130,   130,    80,   130,   132,   131,    80,   130,   130,
     130,     7,   130,   137,   117,   130,     7,     7,   116,   116,
     123,   124,  1046,   130,   116,    14,  1050,    10,   131,   132,
      10,   134,   132,   136,   130,   138,     3,     4,     5,     6,
      75,    14,    14,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    14,    14,   130,   137,   130,   130,
    1084,   136,   130,   132,    31,    32,   130,   130,   130,   130,
     137,    38,   130,   130,   130,    14,    14,    44,   130,    14,
      14,   132,   137,   130,   130,    52,    53,    54,   130,   595,
      57,   130,   130,   130,  1090,    28,   130,   520,   130,    66,
      67,   241,    -1,    -1,   247,    72,    -1,    -1,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    53,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,
      -1,    -1,   123,   124,    -1,    -1,    -1,    52,    53,    54,
     131,   132,    57,   134,    -1,   136,    -1,   138,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    62,    63,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,   134,    -1,   136,   137,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    -1,    -1,    -1,    62,    63,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
     134,    -1,   136,   137,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    62,    63,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,
      44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,
      54,   130,   131,    57,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,
     124,    -1,    -1,    52,    53,    54,   130,   131,    57,    -1,
     134,    -1,   136,   137,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,
      44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,
      54,   130,   131,    57,    -1,   134,    -1,   136,   137,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
     134,    -1,   136,   137,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    62,    63,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,
      44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,
      54,   130,   131,    57,    -1,   134,    -1,   136,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,
     124,    -1,    -1,    52,    53,    54,   130,   131,    57,    -1,
     134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,
      44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,
      54,   130,   131,    57,    -1,   134,    -1,   136,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,
     124,    -1,    -1,    52,    53,    54,   130,   131,    57,    -1,
     134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,
      -1,    44,    -1,    -1,   123,   124,    -1,    -1,    -1,    52,
      53,    54,   131,   132,    57,   134,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,   135,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,
      -1,    -1,    44,    -1,    -1,   123,   124,    -1,    -1,    -1,
      52,    53,    54,   131,   132,    57,   134,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,   123,   124,   125,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     117,    -1,    -1,    44,    -1,    -1,   123,   124,    -1,    -1,
      -1,    52,    53,    54,   131,   132,    57,   134,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    23,    -1,
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
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,
      -1,    -1,    -1,   123,   124,    -1,    -1,    52,    53,    54,
      -1,   131,    57,   133,   134,    -1,   136,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,   123,   124,
      -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
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
     124,    -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,
     134,   135,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,
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
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,
     123,   124,    -1,    -1,    -1,    52,    53,    54,   131,   132,
      57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     117,    -1,    -1,    44,    -1,    -1,   123,   124,    -1,    -1,
      -1,    52,    53,    54,   131,   132,    57,   134,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,
      -1,    -1,   123,   124,    -1,    -1,    -1,    52,    53,    54,
     131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   117,    -1,    -1,    44,    -1,    -1,   123,   124,
      -1,    -1,    -1,    52,    53,    54,   131,   132,    57,   134,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,
      -1,    44,    -1,    -1,   123,   124,    -1,    -1,    -1,    52,
      53,    54,   131,   132,    57,   134,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
     123,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,   123,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,
      -1,   134,    -1,   136,    80,    81,    82,    -1,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,   119,   120,   121,   122,    -1,    -1,   125,
     126,   127,    -1,   129
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
      43,    46,    47,   130,    80,    84,    85,    86,    87,    94,
      95,    96,   118,   119,    88,    89,    90,    91,    92,    93,
     100,   112,   120,   121,   108,    81,    82,   122,   113,   114,
     123,   124,   109,   110,   111,   125,   126,   127,   115,   129,
     129,    14,   169,   169,   169,   169,   169,   169,    14,   132,
     186,   131,   169,   125,   169,    14,   169,   169,   130,    14,
      17,    61,    14,   132,   138,   164,   186,   134,   118,   136,
     130,   103,   104,   105,   106,   107,   116,   116,   132,   169,
     186,   130,   133,   116,   130,   116,   130,   116,   130,    10,
      11,    12,    14,    10,    14,    10,   182,   169,    26,    29,
      30,    39,    42,    43,    46,   130,   133,   169,    26,    46,
     130,   133,   169,   167,   169,   167,    83,   133,   167,    81,
      82,   131,   136,   156,   130,   133,    26,    46,   130,    26,
      46,   130,   116,   116,    72,   133,   136,   118,   136,   118,
     136,   169,   167,   169,   167,    29,    47,    23,   169,   169,
     169,   169,   169,   169,   133,   169,   169,   169,    56,   160,
     136,   131,    14,    65,   136,   132,   133,   132,   133,   133,
     135,   137,    10,    11,    12,    13,    24,    25,    33,    34,
      50,    51,   137,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   172,   172,   172,   172,   172,   172,   170,   172,   172,
     172,   172,   172,   172,   172,   172,     7,     8,     9,   173,
     175,   175,   175,   176,   176,   177,   178,   178,   179,   179,
     180,   180,   180,   180,   180,   180,   180,    14,    22,    24,
      25,    33,    34,    38,    44,    45,    52,    53,    54,   183,
      17,   183,   130,   131,   130,   130,   130,   130,   130,   130,
     116,   131,   132,   169,   130,   133,   123,   124,   133,   135,
     119,   137,   130,   131,    14,   131,    80,    14,   132,   133,
     132,   133,   169,    14,   169,   169,   169,   169,   169,   169,
     169,   169,   130,   132,   169,   169,   131,   169,     5,    14,
     131,   165,   169,   116,   130,   116,   130,   116,   130,   131,
     156,   116,   116,   132,   133,   169,   169,   169,   169,   169,
     169,   169,   186,   133,   169,   169,   186,   132,    27,    28,
     144,   132,    10,   186,   176,   176,    10,    11,    14,    68,
      69,    70,   132,   166,   141,   186,   169,   169,   169,   169,
     169,   169,   130,   186,   153,    14,   169,    14,   169,   132,
      27,   132,   169,   169,    10,   130,   130,   130,   130,   130,
     137,   169,   167,    14,    22,   136,   161,   132,   166,   131,
     131,   163,   132,   186,   116,    10,   135,   186,   129,   134,
     136,   116,   169,   169,   169,   137,   169,    26,    46,   137,
      26,    46,   137,    26,    29,    30,    39,    42,    43,    46,
      47,   137,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,    97,   131,
      14,   131,   132,   186,   169,   132,   186,   133,   186,   169,
     169,   186,   116,   116,   116,   132,   186,   132,   164,   186,
     169,   131,    14,   138,   164,   135,   119,   137,   130,   130,
     169,   130,   133,    80,    80,   165,   130,   133,   130,   169,
     131,   169,   169,   132,   166,   169,   169,   116,   186,   130,
     130,   130,   130,   130,   130,   130,   130,   186,   130,   130,
     130,   167,   143,   167,   131,   169,   167,   167,    83,   167,
      83,    83,    22,   116,    10,    73,    74,   156,   132,   133,
      24,    25,    34,   137,   169,   130,   130,   130,   130,   130,
     130,   130,    40,    41,   137,   119,   137,   119,   137,   167,
     167,   167,   130,   130,   116,   169,    14,    14,   161,    59,
      60,    63,   137,   156,   132,   132,   166,   166,    77,    78,
      79,   137,   132,   169,   135,    14,   169,   169,   169,    26,
      30,    39,    42,    43,    46,   137,   137,   137,   137,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   170,   132,   186,   132,   164,   186,   130,   132,   130,
     132,   186,   130,   135,   135,   135,   169,   169,   169,   132,
     132,   132,   169,    80,    14,   132,   133,   186,   169,   169,
     132,     5,    14,   130,   169,   130,   133,   130,   156,   132,
     130,   130,   169,   132,   132,    27,   144,   169,   167,    10,
      10,    10,    14,   169,   116,    10,    10,   156,    10,    11,
      14,    68,    69,    70,   169,   169,   137,   169,    26,    29,
      30,    39,    42,    43,    46,    47,   137,   169,   167,   130,
     130,    27,   169,   130,   137,    61,   183,    10,    13,    14,
      15,    16,    59,   156,   156,   132,   132,    14,    14,    14,
     130,   116,   131,   135,   137,   137,   169,   169,   169,   169,
     169,   169,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   132,   132,   132,   130,   132,   130,
     130,   130,   132,   169,   131,   186,   130,   130,    80,    80,
     133,   186,   156,   130,   133,   130,   130,   143,   167,   132,
      27,   144,   167,   167,   167,   169,    22,   116,    10,    73,
      74,    26,    30,    46,   137,   137,   137,   169,   169,   169,
     169,   169,   169,   169,   169,   167,   137,   167,   130,   150,
     169,   131,   136,   157,   131,   157,    14,    22,    71,   116,
     130,    14,    22,    71,   116,   130,    10,    13,    15,    16,
     130,   130,   183,   156,   156,     7,     7,     7,   169,   132,
     186,   116,   116,   137,   137,   137,   137,   137,   137,   130,
     169,   132,   169,   169,   186,   130,   186,   167,   167,    14,
     169,   116,    10,    10,   169,   169,   169,   137,   137,   137,
     137,   137,   137,   137,   137,   169,   132,   132,   166,    75,
     141,   132,   166,    14,    14,   130,   169,    14,    14,   130,
     169,    14,    22,    71,   116,   130,    14,    22,    71,   116,
     130,   130,   130,   131,   157,   137,   137,   132,   169,   169,
     132,   130,   132,   130,    27,   144,   169,   137,   137,   137,
     130,   167,   157,   132,   137,    24,    25,    34,   137,   169,
     157,   132,   130,   130,   130,   130,   130,   130,    14,    14,
     130,   169,    14,    14,   130,   169,   132,   166,   137,   137,
     137,   130,   167,   150,   157,   169,   169,   137,   169,    26,
      29,    30,    39,    42,    43,    46,    47,   137,   157,   130,
     130,   130,   130,   130,   130,   157,   132,   132,    26,    30,
      46,   137,   137,   137,   169,   169,   169,   169,   169,   169,
     169,   169,   157,   167,   169,   169,   169,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137
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
     169,   169,   169,   169,   169,   169,   169,   169,   169,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   172,
     172,   173,   173,   173,   174,   174,   175,   175,   175,   176,
     176,   177,   177,   177,   178,   178,   178,   179,   179,   179,
     179,   179,   179,   179,   180,   180,   180,   180,   180,   180,
     181,   181,   182,   182,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   184,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   186,   186,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187
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
       3,     3,     3,     3,     3,     2,     2,     3,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     1,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     3,
       4,     6,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       2,     3,     4,     3,     2,     3,     1,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     4,     6,     6,     6,     1,
       4,     4,     5,     5,     1,     1,     1,     1,     2,     3,
       4,     5,     2,     3,     4,     5,     1,     2
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
#line 3165 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 498 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3171 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 499 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3177 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 503 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3183 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 505 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3189 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 507 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3195 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 509 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3203 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 513 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3209 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 515 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3215 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 517 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3221 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 519 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3227 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 521 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3235 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 525 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3243 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 529 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3249 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 531 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3257 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 535 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3265 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3271 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3277 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 543 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3283 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 545 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3289 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3295 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 549 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3303 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 553 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3311 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 557 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3317 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 559 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3323 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 561 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3329 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3335 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 565 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3341 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 567 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3347 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 569 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3353 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 571 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3359 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 573 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3365 "raku.tab.c"
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
#line 3377 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 583 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3383 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 585 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3389 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 587 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3395 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 589 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3401 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 591 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3407 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 593 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3413 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 595 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3420 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 598 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3427 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 601 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3433 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 603 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3440 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 606 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3447 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 609 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3453 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 611 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3461 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 615 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3467 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 617 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3473 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 619 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3480 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 622 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3487 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 625 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3493 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 627 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3500 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 630 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3507 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 633 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3514 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 636 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3520 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 638 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3529 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 643 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3537 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 647 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3545 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 651 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3554 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 656 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3562 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall ';'  */
#line 659 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3568 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3574 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3580 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 665 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3586 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3592 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 669 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3599 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 672 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3605 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 674 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3611 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 676 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3617 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 678 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3624 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 681 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3631 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 684 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3638 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 687 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3645 "raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 690 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3652 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_IF expr ';'  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3658 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNLESS expr ';'  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3664 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WHILE expr ';'  */
#line 697 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3670 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNTIL expr ';'  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3676 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_FOR expr ';'  */
#line 701 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3683 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITH expr ';'  */
#line 704 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3689 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITHOUT expr ';'  */
#line 706 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3695 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_GIVEN expr ';'  */
#line 708 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3701 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 710 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3708 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 713 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3715 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 716 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3723 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 720 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3730 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 723 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3736 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 725 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3742 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 727 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3748 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 729 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3755 "raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 732 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3762 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 735 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3769 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 738 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3776 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 741 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3783 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 744 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3790 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 747 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3797 "raku.tab.c"
    break;

  case 98: /* stmt: expr ';'  */
#line 749 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3803 "raku.tab.c"
    break;

  case 99: /* stmt: ';'  */
#line 750 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3809 "raku.tab.c"
    break;

  case 100: /* stmt: if_stmt  */
#line 751 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3815 "raku.tab.c"
    break;

  case 101: /* stmt: while_stmt  */
#line 752 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3821 "raku.tab.c"
    break;

  case 102: /* stmt: for_stmt  */
#line 753 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3827 "raku.tab.c"
    break;

  case 103: /* stmt: given_stmt  */
#line 754 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3833 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3839 "raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block KW_CATCH block  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3845 "raku.tab.c"
    break;

  case 106: /* stmt: KW_CATCH block  */
#line 760 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3851 "raku.tab.c"
    break;

  case 107: /* stmt: block  */
#line 762 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3857 "raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 763 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3863 "raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 764 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3869 "raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 765 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3875 "raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 766 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3881 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 767 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3887 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 768 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3893 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 770 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3899 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 772 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3905 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3911 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3917 "raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 777 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3923 "raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 778 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3929 "raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 779 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3935 "raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3941 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block  */
#line 784 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3947 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 786 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3953 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 788 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3959 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block  */
#line 790 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3965 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3971 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 794 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3977 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block elsif_tail  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3983 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3989 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3995 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4001 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4007 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4013 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4019 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4025 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 816 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4031 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE expr block  */
#line 818 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4037 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4043 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4049 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4055 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4061 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 832 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4067 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL expr block  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4073 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4079 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4085 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4091 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP block  */
#line 846 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4097 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 848 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4103 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 850 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4109 "raku.tab.c"
    break;

  case 150: /* loop_incr: expr  */
#line 853 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4115 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 857 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4125 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 863 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4135 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 869 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4144 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 874 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4154 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 880 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4163 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr block  */
#line 885 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4170 "raku.tab.c"
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
#line 4182 "raku.tab.c"
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
#line 4195 "raku.tab.c"
    break;

  case 159: /* when_list: %empty  */
#line 908 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4201 "raku.tab.c"
    break;

  case 160: /* when_list: when_list KW_WHEN expr block  */
#line 910 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4209 "raku.tab.c"
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
#line 4221 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 924 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4231 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT sub_body  */
#line 930 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4241 "raku.tab.c"
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
#line 4253 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 944 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4263 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 950 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4273 "raku.tab.c"
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
#line 4286 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 965 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4297 "raku.tab.c"
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
#line 4310 "raku.tab.c"
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
#line 4323 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 990 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4334 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 999 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4345 "raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1006 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4354 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list '}'  */
#line 1012 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4360 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr '}'  */
#line 1014 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4367 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1017 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4374 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1020 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4380 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1022 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4386 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1024 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4392 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1026 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4398 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1028 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4404 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1030 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4410 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1032 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4416 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1034 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4423 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1037 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4429 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4435 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1041 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4441 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1043 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4448 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1046 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4455 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1049 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4463 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list '}'  */
#line 1054 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4469 "raku.tab.c"
    break;

  case 192: /* method_body: '{' YADA '}'  */
#line 1055 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1057 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4482 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1060 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4489 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1063 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4495 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1065 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4501 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1067 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4507 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1069 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4513 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1071 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4519 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1073 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4525 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1075 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4531 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1077 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4538 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1080 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4544 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1082 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4550 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1084 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4556 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1086 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4563 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1089 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4570 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1092 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4578 "raku.tab.c"
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
#line 4596 "raku.tab.c"
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
#line 4613 "raku.tab.c"
    break;

  case 211: /* is_clauses: %empty  */
#line 1128 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4619 "raku.tab.c"
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
#line 4635 "raku.tab.c"
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
#line 4651 "raku.tab.c"
    break;

  case 214: /* class_body_list: %empty  */
#line 1155 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4657 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1157 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4664 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1160 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4671 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1163 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4678 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1166 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4685 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1169 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4692 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1172 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4699 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1175 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4706 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1178 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4713 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1181 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4720 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1184 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4727 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1187 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4734 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1190 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4741 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1193 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4752 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1200 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4763 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1207 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4774 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1214 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4785 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1221 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4793 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1225 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4801 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1229 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4809 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1233 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4817 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1237 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4828 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1244 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4839 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1251 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4850 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1258 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4861 "raku.tab.c"
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
#line 4875 "raku.tab.c"
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
#line 4887 "raku.tab.c"
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
#line 4899 "raku.tab.c"
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
#line 4912 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1300 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4923 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1307 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4934 "raku.tab.c"
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
#line 4948 "raku.tab.c"
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
#line 4960 "raku.tab.c"
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
#line 4972 "raku.tab.c"
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
#line 4989 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: %empty  */
#line 1356 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4995 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1358 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5004 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1363 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5013 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1368 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5022 "raku.tab.c"
    break;

  case 253: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1375 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5030 "raku.tab.c"
    break;

  case 254: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1379 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5038 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1383 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5046 "raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1387 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5054 "raku.tab.c"
    break;

  case 257: /* pair_list: IDENT OP_FATARROW expr  */
#line 1393 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5060 "raku.tab.c"
    break;

  case 258: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1395 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5066 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1397 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5072 "raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1399 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5078 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR  */
#line 1402 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5084 "raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1403 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5090 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1404 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5096 "raku.tab.c"
    break;

  case 264: /* param_list: VAR_ARRAY  */
#line 1405 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5102 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_ARRAY  */
#line 1406 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5108 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT VAR_SCALAR  */
#line 1407 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5114 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1408 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5120 "raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1409 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5126 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_SCALAR  */
#line 1410 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5132 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1411 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5138 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1412 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5144 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1413 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5150 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR '=' expr  */
#line 1414 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5156 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1415 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5162 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1416 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5168 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1417 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5174 "raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_POS  */
#line 1418 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5180 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_POS  */
#line 1419 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5186 "raku.tab.c"
    break;

  case 279: /* param_list: SLURPY_LOL  */
#line 1420 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5192 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' SLURPY_LOL  */
#line 1421 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5198 "raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_NAMED  */
#line 1422 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5204 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1423 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5210 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list '}'  */
#line 1426 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5216 "raku.tab.c"
    break;

  case 284: /* block: '{' YADA '}'  */
#line 1427 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5222 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr '}'  */
#line 1429 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5228 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1431 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5234 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1433 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5240 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1435 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5246 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1437 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5252 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1439 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5259 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1442 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5265 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1444 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5271 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1446 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5277 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1448 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5283 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1450 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5290 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1453 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5297 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1456 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5305 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1460 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5312 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1463 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5319 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1466 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5326 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1469 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5332 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1471 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5340 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1475 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5347 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1478 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5354 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1481 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5361 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1484 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5368 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1487 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5375 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1490 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5381 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1492 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5387 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1494 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5393 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_LAST '}'  */
#line 1496 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5399 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1498 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5405 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1500 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5411 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1502 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5417 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5423 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1506 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5429 "raku.tab.c"
    break;

  case 317: /* closure: '{' expr '}'  */
#line 1509 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5435 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR '=' expr  */
#line 1512 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5441 "raku.tab.c"
    break;

  case 319: /* expr: VAR_ARRAY '=' expr  */
#line 1513 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5447 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1514 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5453 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1515 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5459 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1516 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5465 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1517 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5471 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1518 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5477 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER block  */
#line 1519 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5487 "raku.tab.c"
    break;

  case 326: /* expr: KW_GATHER for_stmt  */
#line 1524 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5497 "raku.tab.c"
    break;

  case 327: /* expr: tern_expr OP_FATARROW expr  */
#line 1530 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5503 "raku.tab.c"
    break;

  case 328: /* expr: tern_expr  */
#line 1531 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5509 "raku.tab.c"
    break;

  case 329: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1535 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5515 "raku.tab.c"
    break;

  case 330: /* tern_expr: cmp_expr  */
#line 1536 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5521 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1539 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5527 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1540 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5533 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1541 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5539 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1542 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5545 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1543 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5551 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1544 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5557 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1545 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5563 "raku.tab.c"
    break;

  case 338: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1546 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5569 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1547 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5575 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1548 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5581 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1549 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5587 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1550 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5593 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1551 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5599 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1552 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5605 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1554 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5615 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1560 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5625 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5635 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr  */
#line 1571 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5641 "raku.tab.c"
    break;

  case 349: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1574 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5647 "raku.tab.c"
    break;

  case 350: /* divis_expr: jct_expr  */
#line 1575 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5653 "raku.tab.c"
    break;

  case 351: /* jct_expr: jct_expr '|' range_expr  */
#line 1578 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5659 "raku.tab.c"
    break;

  case 352: /* jct_expr: jct_expr '&' range_expr  */
#line 1579 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5665 "raku.tab.c"
    break;

  case 353: /* jct_expr: dor_expr  */
#line 1580 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5671 "raku.tab.c"
    break;

  case 354: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1584 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5677 "raku.tab.c"
    break;

  case 355: /* dor_expr: range_expr  */
#line 1585 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5683 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1588 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5689 "raku.tab.c"
    break;

  case 357: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1589 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5695 "raku.tab.c"
    break;

  case 358: /* range_expr: add_expr  */
#line 1590 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5701 "raku.tab.c"
    break;

  case 359: /* add_expr: add_expr '~' repl_expr  */
#line 1593 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5707 "raku.tab.c"
    break;

  case 360: /* add_expr: repl_expr  */
#line 1594 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5713 "raku.tab.c"
    break;

  case 361: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1597 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5719 "raku.tab.c"
    break;

  case 362: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1598 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5725 "raku.tab.c"
    break;

  case 363: /* repl_expr: addsub_expr  */
#line 1599 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5731 "raku.tab.c"
    break;

  case 364: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1602 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5737 "raku.tab.c"
    break;

  case 365: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1603 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5743 "raku.tab.c"
    break;

  case 366: /* addsub_expr: mul_expr  */
#line 1604 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5749 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '*' unary_expr  */
#line 1607 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5755 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr '/' unary_expr  */
#line 1608 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5761 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr '%' unary_expr  */
#line 1609 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5767 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1610 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5773 "raku.tab.c"
    break;

  case 371: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1612 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5779 "raku.tab.c"
    break;

  case 372: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1614 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5785 "raku.tab.c"
    break;

  case 373: /* mul_expr: unary_expr  */
#line 1615 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5791 "raku.tab.c"
    break;

  case 374: /* unary_expr: '-' unary_expr  */
#line 1618 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5797 "raku.tab.c"
    break;

  case 375: /* unary_expr: '+' unary_expr  */
#line 1619 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5803 "raku.tab.c"
    break;

  case 376: /* unary_expr: '!' unary_expr  */
#line 1620 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5809 "raku.tab.c"
    break;

  case 377: /* unary_expr: CARET unary_expr  */
#line 1621 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5815 "raku.tab.c"
    break;

  case 378: /* unary_expr: OP_REDUCE unary_expr  */
#line 1623 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5824 "raku.tab.c"
    break;

  case 379: /* unary_expr: pow_expr  */
#line 1627 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5830 "raku.tab.c"
    break;

  case 380: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1630 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5836 "raku.tab.c"
    break;

  case 381: /* pow_expr: postfix_expr  */
#line 1631 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5842 "raku.tab.c"
    break;

  case 382: /* scalar_list: VAR_SCALAR  */
#line 1634 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5848 "raku.tab.c"
    break;

  case 383: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1635 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5854 "raku.tab.c"
    break;

  case 384: /* meth_name: IDENT  */
#line 1638 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5860 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_SORT  */
#line 1639 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5866 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_MAP  */
#line 1640 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5872 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_GREP  */
#line 1641 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5878 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_SAY  */
#line 1642 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5884 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_PRINT  */
#line 1643 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5890 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_TAKE  */
#line 1644 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5896 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_RETURN  */
#line 1645 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5902 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_EXISTS  */
#line 1646 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5908 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_DELETE  */
#line 1647 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5914 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_JOIN  */
#line 1648 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 5920 "raku.tab.c"
    break;

  case 395: /* meth_name: TESTOP  */
#line 1649 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5926 "raku.tab.c"
    break;

  case 396: /* postfix_expr: call_expr  */
#line 1651 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5932 "raku.tab.c"
    break;

  case 397: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1654 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5942 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '(' arg_list ')'  */
#line 1660 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5951 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '(' ')'  */
#line 1664 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5957 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1666 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5963 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1668 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5969 "raku.tab.c"
    break;

  case 402: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1670 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5978 "raku.tab.c"
    break;

  case 403: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1675 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5984 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1677 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5994 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1683 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6004 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1689 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6012 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '.' KW_NEW  */
#line 1693 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6020 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' IDENT  */
#line 1697 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6029 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1702 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6040 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1709 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6049 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '.' CARET IDENT  */
#line 1714 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6059 "raku.tab.c"
    break;

  case 412: /* call_expr: atom '.' CARET IDENT  */
#line 1720 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6069 "raku.tab.c"
    break;

  case 413: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1726 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6080 "raku.tab.c"
    break;

  case 414: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1733 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6091 "raku.tab.c"
    break;

  case 415: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1740 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6100 "raku.tab.c"
    break;

  case 416: /* call_expr: atom '.' meth_name  */
#line 1745 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6109 "raku.tab.c"
    break;

  case 417: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1750 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6120 "raku.tab.c"
    break;

  case 418: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1757 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6129 "raku.tab.c"
    break;

  case 419: /* call_expr: call_expr '.' meth_name  */
#line 1762 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6138 "raku.tab.c"
    break;

  case 420: /* call_expr: KW_DIE expr  */
#line 1767 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6144 "raku.tab.c"
    break;

  case 421: /* call_expr: KW_MAP closure expr  */
#line 1769 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6150 "raku.tab.c"
    break;

  case 422: /* call_expr: KW_MAP closure ',' expr  */
#line 1771 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6156 "raku.tab.c"
    break;

  case 423: /* call_expr: KW_GREP closure expr  */
#line 1773 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6162 "raku.tab.c"
    break;

  case 424: /* call_expr: KW_SORT expr  */
#line 1775 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6168 "raku.tab.c"
    break;

  case 425: /* call_expr: KW_SORT closure expr  */
#line 1777 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6174 "raku.tab.c"
    break;

  case 426: /* call_expr: atom  */
#line 1778 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6180 "raku.tab.c"
    break;

  case 427: /* arg_list: expr  */
#line 1781 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6186 "raku.tab.c"
    break;

  case 428: /* arg_list: arg_list ',' expr  */
#line 1782 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6192 "raku.tab.c"
    break;

  case 429: /* arg_list: arg_list ','  */
#line 1783 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6198 "raku.tab.c"
    break;

  case 430: /* atom: LIT_INT  */
#line 1786 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6204 "raku.tab.c"
    break;

  case 431: /* atom: LIT_FLOAT  */
#line 1787 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6210 "raku.tab.c"
    break;

  case 432: /* atom: LIT_STR  */
#line 1788 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6216 "raku.tab.c"
    break;

  case 433: /* atom: WORDLIST  */
#line 1790 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6229 "raku.tab.c"
    break;

  case 434: /* atom: LIT_INTERP_STR  */
#line 1798 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6235 "raku.tab.c"
    break;

  case 435: /* atom: VAR_SCALAR  */
#line 1799 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6241 "raku.tab.c"
    break;

  case 436: /* atom: OP_INC VAR_SCALAR  */
#line 1800 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6247 "raku.tab.c"
    break;

  case 437: /* atom: OP_DEC VAR_SCALAR  */
#line 1801 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6253 "raku.tab.c"
    break;

  case 438: /* atom: VAR_SCALAR OP_INC  */
#line 1802 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6259 "raku.tab.c"
    break;

  case 439: /* atom: VAR_SCALAR OP_DEC  */
#line 1803 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6265 "raku.tab.c"
    break;

  case 440: /* atom: VAR_ARRAY  */
#line 1804 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6271 "raku.tab.c"
    break;

  case 441: /* atom: VAR_HASH  */
#line 1805 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6277 "raku.tab.c"
    break;

  case 442: /* atom: VAR_CAPTURE  */
#line 1807 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6285 "raku.tab.c"
    break;

  case 443: /* atom: VAR_FH  */
#line 1811 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6293 "raku.tab.c"
    break;

  case 444: /* atom: VAR_NAMED_CAPTURE  */
#line 1815 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6300 "raku.tab.c"
    break;

  case 445: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1818 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6306 "raku.tab.c"
    break;

  case 446: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1820 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6312 "raku.tab.c"
    break;

  case 447: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1822 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6318 "raku.tab.c"
    break;

  case 448: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1824 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6324 "raku.tab.c"
    break;

  case 449: /* atom: ARR_ALL_SLICE  */
#line 1826 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6330 "raku.tab.c"
    break;

  case 450: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1828 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6336 "raku.tab.c"
    break;

  case 451: /* atom: VAR_HASH '{' expr '}'  */
#line 1830 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6342 "raku.tab.c"
    break;

  case 452: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1832 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6348 "raku.tab.c"
    break;

  case 453: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1834 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6354 "raku.tab.c"
    break;

  case 454: /* atom: IDENT  */
#line 1835 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6360 "raku.tab.c"
    break;

  case 455: /* atom: VAR_TWIGIL  */
#line 1837 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6368 "raku.tab.c"
    break;

  case 456: /* atom: VAR_ARRAY_TWIGIL  */
#line 1841 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6376 "raku.tab.c"
    break;

  case 457: /* atom: VAR_HASH_TWIGIL  */
#line 1845 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6384 "raku.tab.c"
    break;

  case 458: /* atom: '[' ']'  */
#line 1848 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6390 "raku.tab.c"
    break;

  case 459: /* atom: '[' expr ']'  */
#line 1850 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6396 "raku.tab.c"
    break;

  case 460: /* atom: '[' expr ',' ']'  */
#line 1852 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6402 "raku.tab.c"
    break;

  case 461: /* atom: '[' expr ',' arg_list ']'  */
#line 1854 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6409 "raku.tab.c"
    break;

  case 462: /* atom: '(' ')'  */
#line 1856 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6415 "raku.tab.c"
    break;

  case 463: /* atom: '(' expr ')'  */
#line 1857 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6421 "raku.tab.c"
    break;

  case 464: /* atom: '(' expr ',' ')'  */
#line 1859 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6427 "raku.tab.c"
    break;

  case 465: /* atom: '(' expr ',' arg_list ')'  */
#line 1861 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6434 "raku.tab.c"
    break;

  case 466: /* atom: block  */
#line 1863 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6440 "raku.tab.c"
    break;

  case 467: /* atom: KW_SUB block  */
#line 1864 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6446 "raku.tab.c"
    break;


#line 6450 "raku.tab.c"

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

#line 1866 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
