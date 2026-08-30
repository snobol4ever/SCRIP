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
/* row raku-silent-wrong-answers: bare scalar names seen as the LHS of "name = [...]" (array-composer
   RHS, tagged __rk_arr_lit below) so TT_SAY (lower_raku.c) can gist them bracketed like a real Array --
   marked here at PARSE time (not in lower_raku.c) because lower_rblock walks statements BACKWARD
   (continuation-passing box construction), so a lower-time mark on stmt 1 is not yet visible when
   stmt 3's TT_SAY is lowered first; parsing is naturally forward, so parse-time marking has no such
   ordering hazard. Deliberately NOT flow-sensitive (whole-compilation-unit fact, matching rk_array_names'
   own precedent above) -- a later reassignment to a List does not un-mark; out of scope for this pass. */
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

#line 489 "raku.tab.c"

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
  YYSYMBOL_DOLLAR_LBRACKET = 18,           /* DOLLAR_LBRACKET  */
  YYSYMBOL_VAR_CAPTURE = 19,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 20,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 21,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 22,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 23,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 24,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 25,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 26,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 27,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 28,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 29,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 30,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 31,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 32,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 33,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 34,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 35,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 36,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 37,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 38,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 39,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 40,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 41,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 42,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 43,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 44,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 45,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 46,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 47,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 48,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 49,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 50,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 51,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 52,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 53,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 54,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 55,                   /* KW_SORT  */
  YYSYMBOL_KW_REVERSE = 56,                /* KW_REVERSE  */
  YYSYMBOL_KW_TRY = 57,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 58,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 59,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 60,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 61,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 62,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 63,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 64,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 65,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 66,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 67,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 68,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 69,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 70,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 71,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 72,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 73,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 74,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 75,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 76,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 77,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 78,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 79,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 80,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 81,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 82,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 83,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 84,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 85,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 86,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 87,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 88,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 89,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 90,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 91,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 92,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 93,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 94,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 95,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 96,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 97,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 98,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 99,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 100,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 101,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 102,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 103,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 104,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 105,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 106,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 107,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 108,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 109,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 110,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 111,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 112,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 113,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 114,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 115,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 116,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 117,                   /* OP_POW  */
  YYSYMBOL_118_ = 118,                     /* '='  */
  YYSYMBOL_119_ = 119,                     /* '!'  */
  YYSYMBOL_120_ = 120,                     /* '<'  */
  YYSYMBOL_121_ = 121,                     /* '>'  */
  YYSYMBOL_122_ = 122,                     /* '|'  */
  YYSYMBOL_123_ = 123,                     /* '&'  */
  YYSYMBOL_124_ = 124,                     /* '~'  */
  YYSYMBOL_125_ = 125,                     /* '+'  */
  YYSYMBOL_126_ = 126,                     /* '-'  */
  YYSYMBOL_127_ = 127,                     /* '*'  */
  YYSYMBOL_128_ = 128,                     /* '/'  */
  YYSYMBOL_129_ = 129,                     /* '%'  */
  YYSYMBOL_UMINUS = 130,                   /* UMINUS  */
  YYSYMBOL_131_ = 131,                     /* '.'  */
  YYSYMBOL_132_ = 132,                     /* ';'  */
  YYSYMBOL_133_ = 133,                     /* '('  */
  YYSYMBOL_134_ = 134,                     /* ')'  */
  YYSYMBOL_135_ = 135,                     /* ','  */
  YYSYMBOL_136_ = 136,                     /* '['  */
  YYSYMBOL_137_ = 137,                     /* ']'  */
  YYSYMBOL_138_ = 138,                     /* '{'  */
  YYSYMBOL_139_ = 139,                     /* '}'  */
  YYSYMBOL_140_ = 140,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 141,                 /* $accept  */
  YYSYMBOL_program = 142,                  /* program  */
  YYSYMBOL_stmt_list = 143,                /* stmt_list  */
  YYSYMBOL_stmt = 144,                     /* stmt  */
  YYSYMBOL_if_stmt = 145,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 146,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 147,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 148,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 149,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 150,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 151,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 152,                /* loop_incr  */
  YYSYMBOL_for_stmt = 153,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 154,               /* given_stmt  */
  YYSYMBOL_when_list = 155,                /* when_list  */
  YYSYMBOL_sub_decl = 156,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 157,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 158,                 /* sub_body  */
  YYSYMBOL_method_body = 159,              /* method_body  */
  YYSYMBOL_class_decl = 160,               /* class_decl  */
  YYSYMBOL_role_decl = 161,                /* role_decl  */
  YYSYMBOL_is_clauses = 162,               /* is_clauses  */
  YYSYMBOL_class_body_list = 163,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 164,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 165,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 166,           /* named_arg_list  */
  YYSYMBOL_pair_list = 167,                /* pair_list  */
  YYSYMBOL_param_list = 168,               /* param_list  */
  YYSYMBOL_block = 169,                    /* block  */
  YYSYMBOL_closure = 170,                  /* closure  */
  YYSYMBOL_expr = 171,                     /* expr  */
  YYSYMBOL_tern_expr = 172,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 173,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 174,               /* divis_expr  */
  YYSYMBOL_jct_expr = 175,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 176,                 /* dor_expr  */
  YYSYMBOL_range_expr = 177,               /* range_expr  */
  YYSYMBOL_add_expr = 178,                 /* add_expr  */
  YYSYMBOL_repl_expr = 179,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 180,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 181,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 182,               /* unary_expr  */
  YYSYMBOL_pow_expr = 183,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 184,              /* scalar_list  */
  YYSYMBOL_meth_name = 185,                /* meth_name  */
  YYSYMBOL_postfix_expr = 186,             /* postfix_expr  */
  YYSYMBOL_call_expr = 187,                /* call_expr  */
  YYSYMBOL_arg_list = 188,                 /* arg_list  */
  YYSYMBOL_atom = 189                      /* atom  */
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
#define YYLAST   5068

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  141
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  473
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1189

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   373


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
       2,     2,     2,   119,     2,     2,     2,   129,   123,     2,
     133,   134,   127,   125,   135,   126,   131,   128,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   140,   132,
     120,   118,   121,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   136,     2,   137,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   138,   122,   139,   124,     2,     2,     2,
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
     115,   116,   117,   130
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   507,   507,   518,   519,   522,   524,   526,   528,   532,
     534,   536,   538,   540,   544,   548,   550,   554,   558,   560,
     562,   564,   566,   568,   572,   576,   578,   580,   582,   584,
     586,   588,   590,   592,   594,   602,   604,   606,   608,   610,
     612,   614,   617,   620,   622,   625,   628,   630,   634,   636,
     638,   641,   644,   646,   649,   652,   655,   657,   662,   666,
     670,   675,   679,   680,   682,   684,   686,   688,   691,   693,
     695,   697,   700,   703,   706,   709,   712,   714,   716,   718,
     720,   723,   725,   727,   729,   732,   735,   739,   742,   744,
     746,   748,   751,   754,   757,   760,   763,   766,   769,   770,
     771,   772,   773,   774,   775,   777,   779,   781,   783,   784,
     785,   786,   787,   788,   789,   791,   793,   795,   797,   798,
     799,   800,   803,   805,   807,   809,   811,   813,   815,   817,
     821,   823,   825,   827,   829,   831,   835,   837,   841,   843,
     845,   847,   851,   853,   857,   859,   861,   865,   867,   869,
     873,   876,   882,   888,   893,   899,   904,   909,   917,   928,
     929,   935,   943,   949,   955,   963,   969,   975,   984,   991,
    1000,  1009,  1018,  1025,  1032,  1033,  1036,  1039,  1041,  1043,
    1045,  1047,  1049,  1051,  1053,  1056,  1058,  1060,  1062,  1065,
    1068,  1074,  1075,  1076,  1079,  1082,  1084,  1086,  1088,  1090,
    1092,  1094,  1096,  1099,  1101,  1103,  1105,  1108,  1111,  1117,
    1133,  1148,  1149,  1161,  1175,  1176,  1179,  1182,  1185,  1188,
    1191,  1194,  1197,  1200,  1203,  1206,  1209,  1212,  1219,  1226,
    1233,  1240,  1244,  1248,  1252,  1256,  1263,  1270,  1277,  1284,
    1294,  1302,  1310,  1319,  1326,  1333,  1343,  1351,  1361,  1376,
    1377,  1382,  1387,  1394,  1398,  1402,  1406,  1412,  1414,  1416,
    1418,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,
    1441,  1442,  1443,  1446,  1447,  1448,  1450,  1452,  1454,  1456,
    1458,  1461,  1463,  1465,  1467,  1469,  1472,  1475,  1479,  1482,
    1485,  1488,  1490,  1494,  1497,  1500,  1503,  1506,  1509,  1511,
    1513,  1515,  1517,  1519,  1521,  1523,  1525,  1529,  1532,  1533,
    1534,  1535,  1536,  1537,  1538,  1539,  1544,  1549,  1551,  1554,
    1556,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,
    1568,  1569,  1570,  1571,  1572,  1573,  1579,  1585,  1591,  1594,
    1595,  1598,  1599,  1600,  1603,  1605,  1608,  1609,  1610,  1613,
    1614,  1617,  1618,  1619,  1622,  1623,  1624,  1627,  1628,  1629,
    1630,  1631,  1633,  1635,  1638,  1639,  1640,  1641,  1642,  1647,
    1650,  1651,  1654,  1655,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1665,  1666,  1667,  1668,  1669,  1670,  1672,  1674,  1680,
    1685,  1686,  1688,  1690,  1695,  1697,  1703,  1709,  1713,  1717,
    1722,  1729,  1734,  1740,  1746,  1753,  1760,  1765,  1770,  1777,
    1782,  1787,  1789,  1791,  1793,  1795,  1797,  1799,  1801,  1804,
    1805,  1806,  1809,  1810,  1811,  1812,  1821,  1822,  1823,  1824,
    1825,  1826,  1827,  1828,  1829,  1833,  1837,  1840,  1842,  1844,
    1846,  1848,  1850,  1852,  1854,  1856,  1858,  1859,  1863,  1867,
    1871,  1872,  1874,  1876,  1879,  1880,  1882,  1884,  1887,  1888,
    1889,  1891,  1894,  1895
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
  "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET",
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

#define YYPACT_NINF (-857)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-473)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -857,    10,  2199,  -857,  -857,  -857,  -857,  -857,   604,   -74,
     -62,   -65,    -3,  -857,  -857,  4930,  2294,  -857,  -857,  -857,
      56,  2369,   362,  3955,  4030,  4105,  4180,  4255,     2,   -14,
    4255,  2444,  2519,    96,    64,  4255,  4255,   150,   183,  4330,
    4405,    30,   -91,    72,   123,    65,    65,  4480,  4255,    30,
      30,  4255,   212,   231,    53,  4930,  -857,  -857,   299,   328,
     350,  4930,  4930,  4930,  -857,  2594,  2670,   226,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,   618,  -857,
    -857,  -857,   713,   654,   284,   632,   774,   396,   273,  -857,
     -44,   802,   448,   306,  -857,  -857,   270,   265,   285,   451,
    -857,  -857,  4255,  4255,  4255,  4255,  4255,  4255,   461,  2745,
    4555,  2821,   480,  4255,  4255,   353,   122,  1115,   127,   269,
     129,  -857,   135,    30,  2896,  -857,  -857,   908,   136,  -857,
     174,   374,  -857,  2972,  -857,   205,   246,   275,   277,   921,
     522,   218,   532,  2896,   612,  2896,    41,  2896,    30,  2896,
      30,    11,   116,   252,  -857,  -857,  -857,   314,  -857,   216,
    -857,   217,   446,   474,   397,   453,   468,   137,   184,  2896,
      30,  2896,    30,   305,  3048,  -857,  4255,  4255,  -857,  4255,
    4255,  -857,  4255,  3123,  4255,  3198,  4255,  -857,  -857,   572,
    -857,  -857,  -857,   516,   530,    17,  -857,   540,  -857,  -857,
    -857,  -857,  -857,   752,  -857,   807,   828,  -857,   301,   552,
    1543,  4255,  4255,  4255,  4255,  4255,  4255,  4255,  4255,  -857,
    4255,  4255,  4255,  4255,  4255,  4255,  4255,  4255,  -857,  4255,
    4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,
    4930,  4930,  4930,  4930,  4930,   931,  4930,  4930,  4930,  4930,
    4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,  4930,
    4930,  4930,  4930,  4930,   930,   881,   841,   568,   583,   616,
     626,   640,   650,   143,  -857,   846,  2896,   470,   863,   415,
     615,   565,   652,  -857,   631,   780,   665,   -10,  -857,   808,
     856,   858,  2821,   831,  4255,  4255,  4255,  4255,  4255,  4255,
    4255,  4255,  3273,  -857,  -857,   721,   807,   861,  -857,  4255,
    4255,  -857,  4630,  -857,  4705,  -857,   293,   297,   319,   307,
     744,   755,  -857,   865,   888,  4255,  4255,  4255,  4255,  4255,
    4255,  4255,  -857,  4255,   890,  4255,  4255,  -857,  4255,   893,
    1001,   902,  -857,   879,  4255,  -857,  4930,  4930,    45,  -857,
    -857,  -857,  4255,  4255,  4255,  -857,  4255,  4255,  -857,  4255,
    4255,   767,  4255,  -857,   889,  4255,   897,  4255,   904,   886,
     923,  -857,  4255,  4255,   911,   813,   818,   822,   826,   836,
     839,  4255,  -857,  -857,  -857,    30,    13,  -857,    55,   849,
     864,  -857,  -857,  3348,   883,  1008,  3424,  -857,  -857,   901,
     225,   239,   913,  3955,  4030,  4255,  1912,     5,   259,  -857,
     383,   903,   916,   938,   940,   985,   992,  1005,  1014,  1021,
    1035,  1053,  1056,  1060,  1061,  1062,  1064,  -857,   950,   950,
     950,   950,   950,   950,   999,   950,   950,   950,   950,   950,
     950,   950,   950,  -857,  -857,  -857,   396,  -857,  -857,  -857,
    1073,  1073,   802,   448,   448,   306,   306,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,   941,  1184,  1069,
    -857,  3499,  -857,  -857,  -857,  -857,  -857,  -857,  4255,  3575,
    -857,   926,  -857,  4255,  4255,  4255,  4255,  1085,  1086,  1087,
    -857,  3651,  -857,  1253,  4255,  1075,  -857,    -8,  -857,  1405,
     419,  1088,  1067,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,   485,  -857,  1078,  -857,  1079,  2896,   542,  1125,   156,
    3727,   582,  1080,  4255,  -857,  4780,  -857,  4255,  -857,    63,
    -857,  4255,  4255,  1095,  3348,  1082,  1083,  1084,  1089,  1090,
    1091,  1092,   685,  3348,  1093,  1094,   716,    30,   -18,  4855,
    -857,    30,    30,   101,   -70,    -6,    -5,  -857,    16,  -857,
    -857,  -857,  1097,   928,  1680,   717,  1096,  1098,  1100,  1101,
    1104,  1105,  -857,  1103,    19,  1099,  1106,  1108,  1107,    30,
      30,    30,  1110,  1111,  1121,  4255,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  1203,  1230,  -857,    25,  1097,   932,    83,
     489,   236,  -857,   934,  4255,  -857,  -857,   590,  1233,  2821,
    4255,  4255,   412,    24,   349,  -857,   359,  4255,  4255,  -857,
    4255,  4255,  -857,  4255,  4255,  4255,  4255,  4255,  4255,  4255,
    4255,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  4930,  3803,
    -857,  1329,  1118,   942,  1120,  -857,   945,  3348,   739,  1117,
    1123,   681,  4255,  4255,  4255,  -857,   947,  -857,   949,   951,
    -857,  4255,  1179,  1248,   953,  -857,  -857,  -857,  -857,  -857,
    -857,   955,  -857,  4255,  4255,  4255,   957,  -857,   187,  -857,
    1143,  2896,   740,  1145,  1097,   960,  1146,  1147,  4255,   962,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,   966,  -857,
    -857,  -857,  1074,  -857,  -857,  2896,    30,  -857,  -857,  1266,
    -857,  1270,  1271,  1268,  4255,  1165,  1274,  1277,  -857,  1097,
     515,  3955,  4030,  1987,  -857,   430,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  4255,    30,  -857,  -857,  -857,  1156,  1157,
    1262,  -857,  -857,  -857,  -857,  4255,  1159,  -857,  -857,   208,
     720,   909,  1232,  -857,  -857,  1097,  1097,   973,   975,  1280,
    1281,  1282,  -857,  -857,  1169,  -857,   251,   842,  1158,   -89,
    4255,  4255,  4255,  4255,  4255,  4255,  -857,  -857,  -857,  -857,
      66,   200,   289,   378,   382,   388,   391,   402,   406,   414,
     416,   426,  -857,  -857,   977,  -857,   979,   981,  -857,  1170,
    -857,  -857,   988,  -857,  -857,  -857,  -857,  1171,  1172,  1173,
    -857,  -857,  -857,  1176,  4255,  1178,  -857,  3348,   758,  -857,
    -857,  1181,  1234,  1235,  -857,  1015,  -857,  4255,  -857,  -857,
    1097,  -857,  -857,   759,  1182,  1183,   -18,  -857,  1017,  1127,
      30,    30,    30,  -857,  -857,  4255,  -857,  -857,  -857,    -4,
    -857,    85,  -857,  -857,  -857,   436,   309,  -857,   365,  4255,
    4255,  4255,  4255,  4255,  4255,  4255,  4255,  -857,    30,  1180,
    -857,  -857,    30,  1186,  4255,  -857,   363,   399,    34,    75,
     936,  1188,  1191,   930,  -857,  -857,  1097,  1097,  1317,  1318,
    1319,  -857,  4255,  3879,  1210,  1211,  -857,   438,   457,   458,
     471,   472,   475,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  1198,
    -857,  -857,  -857,  -857,  -857,  4255,  1027,  -857,  -857,  4255,
    4255,  3348,   773,  -857,  -857,  4255,  -857,  -857,  -857,  -857,
      30,    30,  -857,  -857,  -857,  -857,  -857,  1322,  4255,  1213,
    1327,  1328,  4255,  4255,  4255,  -857,  -857,  -857,   476,   508,
     512,   514,   518,   528,   537,   548,  -857,  -857,  -857,  4255,
    1217,  -857,   180,  1275,  -857,   248,  -857,  1339,  1340,  1223,
    4255,  -857,  1344,  1345,  1228,  4255,  -857,   115,   173,  1231,
    1237,  -857,  -857,   647,  -857,  -857,  -857,  -857,  -857,   550,
    1225,  1029,  4255,  4255,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  1236,  1239,  -857,  -857,  1031,  -857,   777,  1144,  -857,
    -857,  -857,  4255,  -857,  -857,   556,   564,   594,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  1241,    30,  1227,  1041,
    1238,  1817,  1227,  1043,  1243,  1244,  -857,  1249,  1258,  1260,
    -857,  1263,  1352,  1353,  1267,  4255,  -857,  1366,  1380,  1269,
    4255,  -857,  -857,  -857,   274,  -857,  -857,  -857,  1265,   599,
     610,  -857,  -857,  1273,  -857,    30,  -857,  -857,  -857,  -857,
    -857,  4255,  -857,  -857,  1227,  -857,  3955,  4030,  2062,  -857,
     536,  -857,  1227,  -857,  -857,  -857,  -857,  -857,  -857,  1295,
    1296,  -857,  1297,  1298,  1299,  -857,  1303,  1227,  1045,  -857,
    -857,  -857,  -857,  -857,  1272,  -857,   551,   315,  -857,   373,
    4255,  4255,  4255,  4255,  4255,  4255,  4255,  4255,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  1227,    30,  4255,
    4255,  4255,  -857,  -857,  -857,   628,   629,   630,   638,   639,
     680,   682,   699,  -857,  -857,   707,   711,   715,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   432,   433,   434,   436,   437,   442,
     443,   457,   456,   458,   459,     0,     0,   444,   445,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   435,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     3,     4,   100,
     101,   108,   109,   110,   111,   102,   103,   118,     0,   119,
     120,   121,   107,     0,   328,   330,   348,   350,   353,   355,
     358,   360,   363,   366,   373,   379,   381,   397,   428,     0,
     440,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   442,
     443,   457,   456,     0,     0,   472,   377,   437,   442,   464,
       0,     0,    35,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,     0,   473,   326,   325,     0,    49,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   144,     0,   147,     0,     0,   112,     0,
       0,   113,     0,     0,     0,     3,     0,   425,   427,   104,
     106,   421,   211,     0,     0,     0,   378,     0,   438,   439,
     376,   375,   374,   437,   468,     0,     0,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   320,   321,   322,
     323,   324,   318,     0,   404,     0,     0,   319,     0,     0,
       0,     0,     0,    39,   409,     0,   408,   456,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,    29,   468,     0,     0,    38,   431,
       0,     6,     0,    10,     0,    11,     0,     0,     0,     0,
       0,     0,   382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,    43,     0,     0,
     125,     0,   137,     0,     0,   156,     0,     0,     0,     3,
     163,    46,     0,     0,     0,    48,     0,     0,    52,     0,
       0,     0,     0,   159,     0,     0,     0,     0,     0,   140,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   422,   424,   426,     0,     0,   214,     0,     0,
       0,   249,   469,     0,     0,     0,     0,   461,   284,   437,
     442,   443,   457,     0,     0,     0,     0,     0,     0,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   333,   334,
     337,   338,   331,   332,     0,   335,   336,   339,   340,   341,
     342,   343,   344,   345,   346,   347,   349,   351,   352,   354,
     356,   357,   359,   361,   362,   364,   365,   370,   371,   372,
     367,   368,   369,   380,   384,   396,   389,   390,   391,   392,
     395,   393,   394,   387,   388,   385,   386,   420,     0,   417,
      59,     0,    93,    94,    95,    96,    97,    56,     0,     0,
     403,     0,    15,     0,     0,     0,     0,   447,   452,   453,
      61,     0,   412,     0,     0,     0,   401,     0,   399,   431,
       0,     0,     0,   320,   321,   322,   323,   324,   318,   319,
     466,     0,    37,     0,   430,     0,     0,     0,   434,   456,
       0,     0,     0,     0,    26,     0,    27,     0,    28,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   469,     0,     0,
     128,   469,     0,     0,   356,   357,   261,   264,     0,   277,
     279,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,   398,     0,     0,     0,     0,     0,   469,
       0,   469,     0,     0,     0,     0,   114,   115,   116,   117,
     317,   423,   105,     0,     0,   214,     0,     0,     0,     0,
       0,     0,   470,     0,     0,   383,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,     0,     0,   311,
       0,     0,   312,     0,     0,     0,     0,     0,     0,     0,
       0,   285,    63,    65,    67,    70,    68,    69,    64,    66,
      76,    78,    80,    83,    81,    82,    77,    79,     0,     0,
     413,     0,     0,     0,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   411,     0,   407,     0,     0,
     253,     0,     0,     0,     0,   447,   452,   453,   467,    36,
       5,     0,    12,     0,     0,     0,     0,    19,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    87,    86,    90,    88,    89,    85,    41,     0,    91,
      92,    44,   122,   127,   126,     0,     0,   136,   153,     0,
     155,     0,     0,     0,     0,   266,     0,     0,   162,     0,
       0,     0,     0,     0,   174,     0,    47,    50,    51,    54,
      55,    31,    30,     0,     0,   157,   454,   455,     0,     0,
     138,   141,   142,   145,   146,     0,     0,   212,   213,     0,
       0,     0,     0,   210,   171,     0,     0,     0,     0,     0,
       0,     0,   248,   471,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   301,   308,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,   419,     0,   416,     0,     0,    58,     0,
      60,   172,     0,    16,   450,   449,   448,     0,     0,     0,
     410,   405,   406,     0,     0,     0,   402,     0,     0,   258,
     257,     0,     0,     0,    21,     0,    22,     0,    25,   165,
       0,    33,    32,     0,   471,   471,     0,   129,     0,   130,
       0,     0,     0,   262,   273,     0,   267,   268,   161,   269,
     265,     0,   278,   280,   282,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
      74,    75,     0,     0,     0,   209,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   168,     0,     0,     0,     0,
       0,     9,     0,     0,   447,   453,   305,     0,     0,     0,
       0,     0,     0,   313,   314,   315,   316,   286,   288,   290,
     293,   291,   292,   287,   289,   418,   415,   414,    57,   471,
      71,    72,    73,   254,   255,     0,     0,    13,    20,     0,
       0,     0,     0,   164,     7,     0,    42,    45,   124,   123,
     469,     0,   134,   154,   151,   152,   275,     0,     0,   270,
       0,     0,     0,     0,     0,   178,   179,   176,     0,     0,
       0,     0,     0,     0,     0,     0,   160,   158,   139,     0,
       0,   150,     0,     3,   244,     0,   241,     0,     0,     0,
       0,   220,     0,     0,     0,     0,   215,     0,     0,     0,
       0,   216,   217,     0,   167,   169,   250,   251,   252,     0,
     173,     0,     0,     0,   295,   297,   300,   298,   299,   296,
      17,     0,   471,   260,   259,     0,    23,     0,   131,   132,
     263,   274,     0,   271,   272,     0,     0,     0,   180,   182,
     184,   187,   185,   186,   181,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   222,     0,     0,     0,
       0,   221,   218,   219,     0,   247,   304,   303,   172,     0,
       0,   256,    14,   471,     8,     0,   135,   276,   188,   190,
     189,     0,   149,   243,     0,   192,     0,     0,     0,   191,
       0,   240,     0,   229,   230,   224,   227,   228,   223,     0,
       0,   233,     0,     0,     0,   234,     0,     0,     0,   302,
     306,   307,    24,   133,     0,   242,     0,     0,   195,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   193,   239,
     237,   238,   226,   235,   236,   225,   246,     0,     0,     0,
       0,     0,   196,   197,   194,     0,     0,     0,     0,     0,
       0,     0,     0,   245,   148,     0,     0,     0,   198,   200,
     202,   205,   203,   204,   199,   201,   206,   208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -857,  -857,     1,  -857,  -554,  -717,  -857,  -857,  -857,  -857,
    -857,   311,  1373,  -857,  -857,  -857,  -857,  -307,  -856,  -857,
    -857,  -857,   809,  -857,  -857,  -496,   877,  -368,   339,  1135,
      -2,  -234,  -857,   812,  1167,  -857,   712,   -16,  1187,   933,
     935,    20,  -857,  1294,  -262,  -857,  -857,   120,  -857
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   210,    68,    69,   560,    70,    71,    72,    73,
      74,   990,    75,    76,   584,    77,    78,   350,   994,    79,
      80,   386,   606,    81,   611,   290,   531,   573,   125,   183,
     134,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   206,   477,    96,    97,   135,    98
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      83,     2,   434,   479,   723,   857,   682,   678,   115,    25,
       3,   152,   540,   684,   130,   731,   153,    27,   733,   967,
     608,   144,   146,   148,   150,   151,   735,   603,   157,   159,
     161,   389,   627,   165,   166,   126,   604,   170,   172,   250,
     251,   996,   174,   500,   110,   187,   188,    67,   997,   191,
     916,   335,   628,   114,   252,   566,   567,   998,   112,   568,
     753,   754,   111,   205,   208,   566,   567,   194,   335,   568,
     131,   336,   504,   566,   567,   196,   113,   568,   164,   732,
     252,   200,   201,   202,   390,   195,   770,   771,   336,  1002,
     772,   736,   737,   566,   567,   969,   343,   568,  1003,   176,
     267,   268,   269,   270,   271,   272,   162,   999,   277,   279,
     163,   281,   282,   734,   968,   569,   570,   571,   252,   177,
      67,   116,   205,   117,    67,   569,   570,   571,   116,  1072,
     117,   306,   683,   569,   570,   571,   284,   178,  1073,   285,
      67,   324,   962,   334,   629,   339,   344,   341,  1004,    67,
     179,   605,  1000,   569,   570,   571,   337,  1085,   755,   338,
     970,   971,   167,   797,   773,   816,  1001,   368,    67,   370,
     180,   705,   375,   337,   376,   377,   338,   378,   379,   572,
     380,   382,   383,   380,   384,   286,   729,  1077,  1074,   607,
     566,   567,   842,  1005,   568,   168,  1078,   704,   596,   346,
     347,   843,  1103,   182,   178,   923,  1111,  1006,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   776,   419,   420,
     421,   422,   423,   424,   425,   426,   192,   427,   320,   275,
     100,   101,   321,  1075,   450,   451,   309,   291,   695,    67,
     252,   777,   778,   353,   356,   193,  1079,  1076,  1135,   293,
     569,   570,   571,   307,   301,   181,  1149,   364,   566,   567,
     109,   488,   568,   354,   357,   738,   116,   294,   117,   770,
     771,  1156,   292,   772,   491,   365,   489,   457,   458,   459,
     460,   461,   462,   463,   566,   567,   630,   116,   568,   117,
     510,  1080,   512,   513,   514,   515,   516,   517,   518,   519,
     774,  1173,   958,   209,   366,  1081,   631,   524,   525,   302,
     527,   303,   532,   197,  1058,   779,   780,   781,   569,   570,
     571,  1096,   367,   545,   546,   547,   548,   549,   550,   551,
     564,   565,   597,   554,   555,   372,   335,   308,   198,   924,
     309,    82,   335,   110,   569,   570,   571,   895,   355,   358,
     574,   576,   577,   373,   578,   579,   336,   580,   581,   112,
     199,   619,   336,   586,   310,   588,   229,   154,   156,   912,
     592,   593,   136,   137,   138,   782,   139,   620,   311,   601,
     173,   175,  1062,   249,   913,   348,   353,   263,   189,   190,
     349,   181,   353,   312,   140,   314,   264,   849,   632,   141,
     353,   622,   623,   624,   626,   292,   354,   313,  1127,   315,
     633,   533,   354,   634,   635,   535,   265,   257,   258,   259,
     354,   598,   521,   636,   812,   534,   637,   638,   925,   536,
     639,   640,   868,   260,   261,   262,   396,   537,   397,   790,
     539,   337,   326,   791,   338,   349,   351,   337,   976,   352,
     338,   538,   792,   552,  1163,   793,   794,   879,   556,   795,
     880,   881,   154,   972,   563,   266,   326,   973,   904,   905,
     882,   361,   575,   883,   884,   273,   328,   885,   886,   329,
     330,   351,   583,   974,   352,   283,   664,   340,   798,   342,
     345,   355,   669,   670,   280,   142,   992,   355,   799,   566,
     567,   993,   680,   568,   977,   355,   304,   524,   897,   369,
     599,   371,  1164,   613,   650,   228,   617,   926,   247,   248,
     651,   927,   641,   652,   691,   869,   870,   928,   205,   871,
     929,   700,   995,   702,   653,   703,   319,   993,   654,   706,
     707,   930,   322,   953,   332,   931,   655,   333,   656,    82,
     496,   796,   497,   932,   496,   933,   685,   726,   657,   569,
     570,   571,   228,  1140,   359,   934,  1141,  1142,   332,   887,
     710,   333,   745,   255,   256,   975,  1143,  1024,  1159,  1144,
    1145,   326,  1160,  1146,  1147,   872,   873,   874,   362,   712,
     713,   328,   360,   766,   329,   330,  1025,  1026,  1161,  1014,
    1015,   663,   492,   714,   715,   493,   363,   716,   650,   666,
    1027,  1028,   784,   668,  1029,  1048,   671,   787,   788,   789,
     309,   676,   688,   679,  1059,   800,   801,  1063,   802,   803,
     385,   804,   805,   806,   807,   808,   809,   810,   811,   325,
     651,  1013,   326,   327,   652,   211,   653,  1049,   212,   213,
     654,  1050,   328,  1051,   387,   329,   330,  1052,   214,   331,
     655,   215,   216,   388,   709,   217,   218,  1053,   228,   656,
     827,   828,   829,   718,   692,  1148,  1054,   693,   391,   833,
     657,   220,   820,   332,   221,   222,   333,  1055,   710,  1086,
    1162,   398,   839,   840,   223,  1098,   712,   224,   225,   845,
     482,   226,   227,  1099,   499,    99,   853,   100,   101,   102,
     103,   104,   105,   106,   697,   483,  1128,   698,   230,   231,
     232,   233,   107,   858,   602,   309,   716,   785,   234,   235,
     236,   940,   864,  1100,   464,   108,   498,   109,  1130,   875,
     876,   878,   942,   465,   332,   466,   467,   333,   484,  1131,
     219,   888,   237,   238,   468,   469,  -472,  -472,   485,   470,
     650,   651,   652,   893,   501,   471,   472,  1178,  1179,  1180,
     653,   654,   486,   473,   474,   475,   476,  1181,  1182,   814,
    1084,   817,   487,   896,   500,   993,   228,   822,   917,   918,
     919,   920,   921,   922,   502,  -472,  -472,  -472,   503,  -472,
    -472,  -472,  -472,  -472,  -472,  -472,  -472,  -472,  -472,  -472,
    -472,  -472,   655,   838,   656,  -472,   309,   717,   826,  1183,
     309,  1184,   505,  -472,  -472,  -472,  -472,  -472,  -472,  -472,
    -472,   657,   944,  -472,  -472,  -472,  -472,  -472,  1185,   710,
    -472,  -472,  -472,   712,  -472,   511,  1186,   716,   721,   746,
    1187,   309,   309,   522,  1188,   100,   101,   295,   296,   297,
     298,   299,   541,   966,   239,   240,   241,   242,   243,   244,
     300,   823,   846,   542,   309,   847,   245,   978,   979,   980,
     981,   982,   983,   984,   985,   109,  -382,  -382,   246,   562,
     947,   954,   991,   309,   955,   464,   722,   724,   478,   582,
     727,   728,   730,   585,   465,  1036,   466,   467,   309,  1094,
    1019,   587,   309,    82,   590,   468,   469,   253,   254,   898,
     470,   594,   899,   900,   901,   902,   471,   472,   760,   761,
     762,   316,   317,   318,   473,   474,   475,   476,   443,   444,
     445,   392,   393,  1031,   464,   595,  1007,  1033,  1034,  1008,
     596,  1009,  1010,   465,   597,   466,   467,   946,   598,   447,
     448,   449,   394,   395,   468,   469,  1041,   952,   599,   470,
    1045,  1046,  1047,   480,   481,   471,   472,   496,   600,   914,
     490,   309,   609,   473,   474,   475,   476,  1056,   494,   495,
     506,   507,   508,   509,  1061,   523,   309,   610,  1067,   543,
     395,   614,    99,  1071,   100,   101,   102,   103,   104,   105,
     106,   100,   101,   295,   296,   297,   298,   299,   615,   107,
    1089,  1090,   392,   544,   392,   553,   300,   557,   393,   558,
     559,   621,   618,  1021,   109,   642,   561,   393,   589,   393,
    1097,   109,   428,   429,   430,   431,   432,   433,   643,   435,
     436,   437,   438,   439,   440,   441,   442,   591,   393,  1110,
     392,   667,   739,   740,   246,   859,   775,   740,   783,   309,
     644,  1035,   645,  1122,   659,  1037,   819,   309,  1126,   821,
     309,   830,   309,   831,   507,   832,   309,   836,   507,   392,
     837,   841,   698,   889,   850,   740,   854,   309,   658,   991,
     855,   309,   856,   559,  1136,  1137,  1139,   906,   740,   907,
     740,   935,   309,   936,   507,   937,   309,   646,     4,     5,
       6,     7,   939,   309,   647,   127,   128,   120,   121,   287,
      13,    14,    15,    16,    17,    18,    19,   648,  1165,  1166,
    1167,  1168,  1169,  1170,  1171,  1172,   649,   123,    29,   392,
     951,   960,   393,   650,    35,   961,   559,  1175,  1176,  1177,
      37,  1032,   309,  1088,   309,  1093,   309,   651,    45,    46,
      47,    48,  1095,   559,    51,  1104,   740,  1112,   740,  1157,
     740,   184,   186,    55,    56,   652,   453,   454,   653,    57,
     455,   456,   654,   655,   656,   959,   657,   252,   660,   963,
     964,   965,   661,   672,   673,   674,   687,   694,   681,   686,
     689,   690,   699,   708,   710,   711,   712,   767,    59,    60,
     756,   713,   714,   715,   716,   719,   720,   986,   747,   758,
     748,   988,   749,   750,    61,   349,   751,   752,   309,   765,
      62,    63,   763,   764,   768,   757,   759,   786,   124,   288,
     818,    66,   820,    67,   824,   289,     4,     5,     6,     7,
     825,   834,   835,   127,   128,   120,   121,   287,    13,    14,
      15,    16,    17,    18,    19,   844,   860,   848,   851,   852,
     861,   862,   863,   865,   866,   123,    29,   867,   890,   891,
     892,   894,    35,   903,   908,   909,   910,   915,    37,  1038,
    1039,   911,   938,   940,   941,   942,    45,    46,    47,    48,
     943,   945,    51,   948,   956,   957,   949,   950,   989,   987,
    1011,    55,    56,  1012,  1016,  1017,  1018,    57,  1022,  1023,
    1030,  1042,     4,     5,     6,     7,  1040,  1043,  1044,   127,
     128,   120,   121,   287,    13,    14,    15,    16,    17,    18,
      19,  1057,  1060,  1064,  1065,  1066,    59,    60,  1068,  1069,
    1070,   123,    29,  1082,  1087,   993,  1119,  1120,    35,  1083,
    1091,  1092,    61,  1101,    37,  1113,  1114,  1105,    62,    63,
    1123,  1115,    45,    46,    47,    48,   124,   677,    51,    66,
    1116,    67,  1117,   289,  1124,  1118,  1102,    55,    56,  1121,
      82,  1125,   155,    57,  1129,  1132,  1158,   696,     4,     5,
       6,     7,  1134,   446,   769,   127,   128,   120,   121,   287,
      13,    14,    15,    16,    17,    18,    19,  1150,  1151,  1152,
    1153,  1154,    59,    60,  1133,  1155,   323,   123,    29,   452,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   124,   815,    51,    66,     0,    67,     0,   289,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1174,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
      62,    63,     0,     0,     0,     0,     0,     0,   124,     0,
       0,    66,     0,    67,     0,   289,     4,     5,     6,     7,
       0,     0,     0,   399,   400,   401,   402,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   403,   404,
      25,     0,     0,    26,    27,    28,    29,   405,   406,    32,
      33,    34,    35,    36,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,   407,   408,    45,    46,    47,    48,
      49,    50,    51,    52,     0,     0,     0,    53,    54,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,    64,    65,     0,     0,    66,
       0,    67,   409,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   741,   742,    25,     0,     0,
      26,    27,    28,    29,    30,   743,    32,    33,    34,    35,
      36,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,     0,     0,    53,    54,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,    64,    65,     0,     0,    66,     0,    67,   744,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,  1106,  1107,    25,     0,     0,    26,    27,    28,
      29,    30,  1108,    32,    33,    34,    35,    36,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     0,     0,
       0,    53,    54,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
      59,    60,   127,   128,   120,   121,   122,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    61,     0,     0,     0,
       0,     0,    62,    63,   123,    29,     0,     0,     0,    64,
      65,    35,     0,    66,     0,    67,  1109,    37,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,    48,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,   158,   124,    51,     0,    66,     0,
      67,   625,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,   158,
     124,    51,     0,    66,     0,    67,   877,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,   158,   124,     0,     0,    66,     0,
      67,  1138,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,     0,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       0,     0,     0,    53,    54,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,    59,    60,   127,   128,   120,   121,   122,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    61,     0,
       0,     0,     0,     0,    62,    63,   123,    29,     0,     0,
       0,    64,    65,    35,     0,    66,     0,    67,     0,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   124,    51,     0,
      66,   129,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,   132,   133,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,   158,   124,    51,     0,
      66,     0,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   203,   128,   120,   121,   122,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,   160,   124,    51,     0,    66,     0,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    61,     0,    37,     0,     0,     0,    62,
      63,     0,     0,    45,    46,    47,    48,   124,   204,    51,
      66,     0,    67,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   124,    51,     0,    66,   207,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,   123,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    61,     0,    37,     0,     0,     0,
      62,    63,     0,     0,    45,    46,    47,    48,   124,   274,
      51,    66,     0,    67,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   127,   128,   120,   121,
     122,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,   278,    45,
      46,    47,    48,     0,   124,    51,     0,    66,     0,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   124,
     204,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,    13,    14,    15,    16,    17,    18,    19,
       0,     0,   374,     0,     0,    59,    60,     0,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   124,   305,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   124,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   124,    51,   381,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,   209,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   124,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,     0,   124,    51,     0,    66,
     520,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    61,     0,    37,
       0,     0,     0,    62,    63,     0,     0,    45,    46,    47,
      48,   124,   612,    51,    66,     0,    67,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   124,    51,     0,
      66,   616,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    59,    60,     0,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   124,   662,    51,    66,     0,    67,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,   123,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,    61,     0,    37,     0,     0,     0,
      62,    63,     0,     0,    45,    46,    47,    48,   124,   665,
      51,    66,     0,    67,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       4,     5,   528,     7,     0,     0,     0,   127,   128,   120,
     121,   529,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    59,    60,     0,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
      61,     0,    37,     0,     0,     0,    62,    63,     0,     0,
      45,    46,    47,    48,   124,   675,    51,    66,     0,    67,
       0,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      59,    60,     0,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    61,     0,    37,     0,
       0,     0,    62,    63,     0,     0,    45,    46,    47,    48,
     124,   204,    51,    66,     0,    67,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    61,     0,    37,     0,     0,     0,    62,    63,
       0,     0,    45,    46,    47,    48,   124,   813,    51,    66,
       0,    67,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    59,    60,     0,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   124,  1020,    51,    66,     0,    67,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   143,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   145,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   147,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   149,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   124,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   169,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   171,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   124,    51,     0,    66,     0,   185,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   276,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
     528,     7,     0,     0,     0,   127,   128,   120,   121,   529,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   526,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   530,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   701,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   120,   121,   122,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   725,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,     0,   124,     0,     0,    66,     0,    67
};

static const yytype_int16 yycheck[] =
{
       2,     0,   236,   265,   558,   722,    14,   503,    11,    27,
       0,    27,   319,   509,    16,    85,    14,    31,    23,    23,
     388,    23,    24,    25,    26,    27,    10,    14,    30,    31,
      32,    14,    27,    35,    36,    15,    23,    39,    40,    83,
      84,   897,   133,   132,   118,    47,    48,   138,    14,    51,
     139,    27,    47,   118,   124,    10,    11,    23,   120,    14,
      41,    42,   136,    65,    66,    10,    11,    14,    27,    14,
      14,    47,    82,    10,    11,    55,   138,    14,    14,    85,
     124,    61,    62,    63,    67,    32,    61,    62,    47,    14,
      65,    75,    76,    10,    11,    10,    85,    14,    23,    27,
     102,   103,   104,   105,   106,   107,    10,    73,   110,   111,
      14,   113,   114,   118,   118,    70,    71,    72,   124,    47,
     138,   131,   124,   133,   138,    70,    71,    72,   131,    14,
     133,   133,   140,    70,    71,    72,    14,   132,    23,    17,
     138,   143,   859,   145,   139,   147,   135,   149,    73,   138,
      27,   138,   118,    70,    71,    72,   132,  1013,   139,   135,
      75,    76,    12,   139,   139,   661,   132,   169,   138,   171,
      47,   539,   174,   132,   176,   177,   135,   179,   180,   134,
     182,   183,   184,   185,   186,    63,    85,    14,    73,   134,
      10,    11,     5,   118,    14,    12,    23,   134,   132,    83,
      84,    14,  1058,   138,   132,   139,  1062,   132,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   134,   220,   221,
     222,   223,   224,   225,   226,   227,    14,   229,    10,   109,
     103,   104,    14,   118,   250,   251,   135,   117,    82,   138,
     124,   609,   610,    27,    27,    14,    73,   132,  1104,   120,
      70,    71,    72,   133,   118,   132,  1112,   120,    10,    11,
     133,   118,    14,    47,    47,   572,   131,   138,   133,    61,
      62,  1127,   136,    65,   276,   138,   133,   257,   258,   259,
     260,   261,   262,   263,    10,    11,    27,   131,    14,   133,
     292,   118,   294,   295,   296,   297,   298,   299,   300,   301,
     607,  1157,   856,    77,   120,   132,    47,   309,   310,   135,
     312,   137,   314,    14,   134,    79,    80,    81,    70,    71,
      72,  1038,   138,   325,   326,   327,   328,   329,   330,   331,
     346,   347,   132,   335,   336,    30,    27,   132,    10,   139,
     135,     2,    27,   118,    70,    71,    72,   139,   132,   132,
     349,   353,   354,    48,   356,   357,    47,   359,   360,   120,
      10,   136,    47,   365,   118,   367,    82,    28,    29,   118,
     372,   373,    10,    11,    12,   139,    14,   138,   132,   381,
      41,    42,   134,   110,   133,   133,    27,   117,    49,    50,
     138,   132,    27,   118,    32,   118,   131,   704,   139,    37,
      27,   403,   404,   405,   406,   136,    47,   132,   134,   132,
      27,   118,    47,    30,    31,   118,   131,   111,   112,   113,
      47,   132,   302,    40,   658,   132,    43,    44,   139,   132,
      47,    48,   739,   127,   128,   129,   135,   118,   137,    27,
     133,   132,    30,    31,   135,   138,   132,   132,   139,   135,
     135,   132,    40,   333,   139,    43,    44,    27,   338,    47,
      30,    31,   123,    27,   344,    14,    30,    31,   775,   776,
      40,    74,   352,    43,    44,    14,    40,    47,    48,    43,
      44,   132,   362,    47,   135,   132,   488,   148,   139,   150,
     151,   132,   494,   495,    14,   133,   133,   132,   139,    10,
      11,   138,   504,    14,   139,   132,   132,   509,   770,   170,
     132,   172,   139,   393,   132,   132,   396,   139,   122,   123,
     132,   139,   139,   132,   526,    10,    11,   139,   530,    14,
     139,   533,   133,   535,   132,   537,    14,   138,   132,   541,
     542,   139,    10,   850,   132,   139,   132,   135,   132,   210,
     135,   139,   137,   139,   135,   139,   137,   559,   132,    70,
      71,    72,   132,    27,   118,   139,    30,    31,   132,   139,
     132,   135,   574,   125,   126,   139,    40,   139,    27,    43,
      44,    30,    31,    47,    48,    70,    71,    72,   135,   132,
     132,    40,   118,   595,    43,    44,   139,   139,    47,   906,
     907,   481,   132,   132,   132,   135,   138,   132,   132,   489,
     139,   139,   614,   493,   139,   139,   496,   619,   620,   621,
     135,   501,   137,   503,   992,   627,   628,   995,   630,   631,
      58,   633,   634,   635,   636,   637,   638,   639,   640,    27,
     132,   903,    30,    31,   132,    27,   132,   139,    30,    31,
     132,   139,    40,   139,   138,    43,    44,   139,    40,    47,
     132,    43,    44,   133,   544,    47,    48,   139,   132,   132,
     672,   673,   674,   553,   132,   139,   139,   135,   138,   681,
     132,    27,   132,   132,    30,    31,   135,   139,   132,   139,
     139,   139,   694,   695,    40,   139,   132,    43,    44,   701,
     132,    47,    48,   139,   139,   101,   708,   103,   104,   105,
     106,   107,   108,   109,   132,   132,  1084,   135,    86,    87,
      88,    89,   118,   725,   385,   135,   132,   137,    96,    97,
      98,   132,   734,   139,    14,   131,   121,   133,   139,   741,
     742,   743,   132,    23,   132,    25,    26,   135,   132,   139,
     132,   753,   120,   121,    34,    35,    43,    44,   132,    39,
     132,   132,   132,   765,   133,    45,    46,   139,   139,   139,
     132,   132,   132,    53,    54,    55,    56,   139,   139,   659,
     133,   661,   132,    63,   132,   138,   132,   667,   790,   791,
     792,   793,   794,   795,    14,    82,    83,    84,   133,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,   132,   693,   132,   102,   135,   132,   137,   139,
     135,   139,    14,   110,   111,   112,   113,   114,   115,   116,
     117,   132,   834,   120,   121,   122,   123,   124,   139,   132,
     127,   128,   129,   132,   131,    14,   139,   132,   132,   132,
     139,   135,   135,   132,   139,   103,   104,   105,   106,   107,
     108,   109,   118,   865,    90,    91,    92,    93,    94,    95,
     118,   132,   132,   118,   135,   135,   102,   879,   880,   881,
     882,   883,   884,   885,   886,   133,   134,   135,   114,    10,
     132,   132,   894,   135,   135,    14,   557,   558,    17,   132,
     561,   562,   563,    14,    23,   132,    25,    26,   135,   132,
     912,    14,   135,   574,    28,    34,    35,   115,   116,    10,
      39,    10,    13,    14,    15,    16,    45,    46,   589,   590,
     591,    10,    11,    12,    53,    54,    55,    56,     7,     8,
       9,   134,   135,   945,    14,   132,    10,   949,   950,    13,
     132,    15,    16,    23,   132,    25,    26,   837,   132,   247,
     248,   249,   134,   135,    34,    35,   968,   847,   132,    39,
     972,   973,   974,   132,   133,    45,    46,   135,   139,   137,
     134,   135,   133,    53,    54,    55,    56,   989,   125,   126,
     134,   135,   134,   135,   993,   134,   135,   133,  1000,   134,
     135,   118,   101,  1005,   103,   104,   105,   106,   107,   108,
     109,   103,   104,   105,   106,   107,   108,   109,    10,   118,
    1022,  1023,   134,   135,   134,   135,   118,   134,   135,    28,
      29,   118,   131,   913,   133,   132,   134,   135,   134,   135,
    1042,   133,   230,   231,   232,   233,   234,   235,   132,   237,
     238,   239,   240,   241,   242,   243,   244,   134,   135,  1061,
     134,   135,   134,   135,   114,   726,   134,   135,   134,   135,
     132,   951,   132,  1075,   133,   955,   134,   135,  1080,   134,
     135,   134,   135,   134,   135,   134,   135,   134,   135,   134,
     135,   134,   135,   754,   134,   135,   134,   135,    99,  1101,
     134,   135,    28,    29,  1106,  1107,  1108,   134,   135,   134,
     135,   134,   135,   134,   135,   134,   135,   132,     3,     4,
       5,     6,   134,   135,   132,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   132,  1140,  1141,
    1142,  1143,  1144,  1145,  1146,  1147,   132,    32,    33,   134,
     135,   134,   135,   132,    39,    28,    29,  1159,  1160,  1161,
      45,   134,   135,   134,   135,   134,   135,   132,    53,    54,
      55,    56,    28,    29,    59,   134,   135,   134,   135,   134,
     135,    46,    47,    68,    69,   132,   253,   254,   132,    74,
     255,   256,   132,   132,   132,   856,   132,   124,    14,   860,
     861,   862,   133,   118,   118,   118,   139,    82,   133,   121,
     132,   132,   132,   118,   132,   132,   132,    14,   103,   104,
     121,   132,   132,   132,   132,   132,   132,   888,   132,   121,
     132,   892,   132,   132,   119,   138,   132,   132,   135,   118,
     125,   126,   132,   132,    14,   139,   139,    14,   133,   134,
     132,   136,   132,   138,   137,   140,     3,     4,     5,     6,
     137,    82,    14,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   132,    10,   132,   132,   132,
      10,    10,    14,   118,    10,    32,    33,    10,   132,   132,
      28,   132,    39,    61,    14,    14,    14,   139,    45,   960,
     961,   132,   132,   132,   132,   132,    53,    54,    55,    56,
     134,   133,    59,   132,   132,   132,    82,    82,   132,   139,
     132,    68,    69,   132,     7,     7,     7,    74,   118,   118,
     132,   118,     3,     4,     5,     6,    14,    10,    10,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   134,    77,    14,    14,   132,   103,   104,    14,    14,
     132,    32,    33,   132,   139,   138,    14,    14,    39,   132,
     134,   132,   119,   132,    45,   132,   132,   139,   125,   126,
      14,   132,    53,    54,    55,    56,   133,   134,    59,   136,
     132,   138,   132,   140,    14,   132,  1057,    68,    69,   132,
    1061,   132,    29,    74,   139,   132,   134,   530,     3,     4,
       5,     6,  1101,   246,   605,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   132,   132,   132,
     132,   132,   103,   104,  1095,   132,   142,    32,    33,   252,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,   119,    -1,
      45,    -1,    -1,    -1,   125,   126,    -1,    -1,    53,    54,
      55,    56,   133,   134,    59,   136,    -1,   138,    -1,   140,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,   136,    -1,   138,    -1,   140,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    -1,    64,    65,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,   125,   126,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,   136,
      -1,   138,   139,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    -1,    -1,    64,    65,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,   136,    -1,   138,   139,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
     103,   104,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,   125,   126,    32,    33,    -1,    -1,    -1,   132,
     133,    39,    -1,   136,    -1,   138,   139,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,   119,    45,    -1,    -1,    -1,    -1,   125,   126,    -1,
      53,    54,    55,    56,   132,   133,    59,    -1,   136,    -1,
     138,   139,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,   119,    45,    -1,    -1,
      -1,    -1,   125,   126,    -1,    53,    54,    55,    56,   132,
     133,    59,    -1,   136,    -1,   138,   139,    -1,    -1,    -1,
      68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,   136,    -1,
     138,   139,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,   103,   104,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,   125,   126,    32,    33,    -1,    -1,
      -1,   132,   133,    39,    -1,   136,    -1,   138,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,   125,
     126,    -1,    53,    54,    55,    56,    -1,   133,    59,    -1,
     136,   137,   138,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,    45,
      -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,    55,
      56,   132,   133,    59,    -1,   136,    -1,   138,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,   125,
     126,    -1,    53,    54,    55,    56,   132,   133,    59,    -1,
     136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,    45,
      -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,    55,
      56,   132,   133,    59,    -1,   136,    -1,   138,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,   119,    -1,    45,    -1,    -1,    -1,   125,
     126,    -1,    -1,    53,    54,    55,    56,   133,   134,    59,
     136,    -1,   138,    -1,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,   137,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   119,    -1,    45,    -1,    -1,    -1,
     125,   126,    -1,    -1,    53,    54,    55,    56,   133,   134,
      59,   136,    -1,   138,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
     119,    45,    -1,    -1,    -1,    -1,   125,   126,   127,    53,
      54,    55,    56,    -1,   133,    59,    -1,   136,    -1,   138,
      -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   119,    -1,    45,    -1,    -1,
      -1,   125,   126,    -1,    -1,    53,    54,    55,    56,   133,
     134,    59,   136,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    24,    -1,    -1,   103,   104,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   119,    -1,    45,    -1,    -1,    -1,   125,   126,    -1,
      -1,    53,    54,    55,    56,   133,   134,    59,   136,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,   119,    45,    -1,
      -1,    -1,    -1,   125,   126,    -1,    53,    54,    55,    56,
      -1,   133,    59,    -1,   136,    -1,   138,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,   119,    45,    -1,    -1,    -1,    -1,   125,   126,
      -1,    53,    54,    55,    56,    -1,   133,    59,   135,   136,
      -1,   138,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    77,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,   119,    45,    -1,
      -1,    -1,    -1,   125,   126,    -1,    53,    54,    55,    56,
      -1,   133,    59,    -1,   136,    -1,   138,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,   119,    45,    -1,    -1,    -1,    -1,   125,   126,
      -1,    53,    54,    55,    56,    -1,   133,    59,    -1,   136,
     137,   138,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,   119,    -1,    45,
      -1,    -1,    -1,   125,   126,    -1,    -1,    53,    54,    55,
      56,   133,   134,    59,   136,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,   125,
     126,    -1,    53,    54,    55,    56,    -1,   133,    59,    -1,
     136,   137,   138,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,   119,    -1,
      45,    -1,    -1,    -1,   125,   126,    -1,    -1,    53,    54,
      55,    56,   133,   134,    59,   136,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   119,    -1,    45,    -1,    -1,    -1,
     125,   126,    -1,    -1,    53,    54,    55,    56,   133,   134,
      59,   136,    -1,   138,    -1,    -1,    -1,    -1,    -1,    68,
      69,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
     119,    -1,    45,    -1,    -1,    -1,   125,   126,    -1,    -1,
      53,    54,    55,    56,   133,   134,    59,   136,    -1,   138,
      -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,   119,    -1,    45,    -1,
      -1,    -1,   125,   126,    -1,    -1,    53,    54,    55,    56,
     133,   134,    59,   136,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,   119,    -1,    45,    -1,    -1,    -1,   125,   126,
      -1,    -1,    53,    54,    55,    56,   133,   134,    59,   136,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,   119,    -1,
      45,    -1,    -1,    -1,   125,   126,    -1,    -1,    53,    54,
      55,    56,   133,   134,    59,   136,    -1,   138,    -1,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   119,
      45,    -1,    -1,    -1,    -1,   125,   126,    -1,    53,    54,
      55,    56,    -1,   133,    59,    -1,   136,    -1,   138,    -1,
      -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   119,    45,    -1,    -1,    -1,    -1,
     125,   126,    -1,    53,    54,    55,    56,    -1,   133,    59,
      -1,   136,    -1,   138,    -1,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,   136,    -1,   138
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   142,   143,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    64,    65,    68,    69,    74,    78,   103,
     104,   119,   125,   126,   132,   133,   136,   138,   144,   145,
     147,   148,   149,   150,   151,   153,   154,   156,   157,   160,
     161,   164,   169,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   186,   187,   189,   101,
     103,   104,   105,   106,   107,   108,   109,   118,   131,   133,
     118,   136,   120,   138,   118,    11,   131,   133,    10,    11,
      12,    13,    14,    32,   133,   169,   182,    10,    11,   137,
     171,    14,   132,   133,   171,   188,    10,    11,    12,    14,
      32,    37,   133,   133,   171,   133,   171,   133,   171,   133,
     171,   171,   178,    14,   169,   153,   169,   171,   132,   171,
     132,   171,    10,    14,    14,   171,   171,    12,    12,   133,
     171,   133,   171,   169,   133,   169,    27,    47,   132,    27,
      47,   132,   138,   170,   170,   138,   170,   171,   171,   169,
     169,   171,    14,    14,    14,    32,   182,    14,    10,    10,
     182,   182,   182,    10,   134,   171,   184,   137,   171,    77,
     143,    27,    30,    31,    40,    43,    44,    47,    48,   132,
      27,    30,    31,    40,    43,    44,    47,    48,   132,    82,
      86,    87,    88,    89,    96,    97,    98,   120,   121,    90,
      91,    92,    93,    94,    95,   102,   114,   122,   123,   110,
      83,    84,   124,   115,   116,   125,   126,   111,   112,   113,
     127,   128,   129,   117,   131,   131,    14,   171,   171,   171,
     171,   171,   171,    14,   134,   188,   133,   171,   127,   171,
      14,   171,   171,   132,    14,    17,    63,    14,   134,   140,
     166,   188,   136,   120,   138,   105,   106,   107,   108,   109,
     118,   118,   135,   137,   132,   134,   171,   188,   132,   135,
     118,   132,   118,   132,   118,   132,    10,    11,    12,    14,
      10,    14,    10,   184,   171,    27,    30,    31,    40,    43,
      44,    47,   132,   135,   171,    27,    47,   132,   135,   171,
     169,   171,   169,    85,   135,   169,    83,    84,   133,   138,
     158,   132,   135,    27,    47,   132,    27,    47,   132,   118,
     118,    74,   135,   138,   120,   138,   120,   138,   171,   169,
     171,   169,    30,    48,    24,   171,   171,   171,   171,   171,
     171,   135,   171,   171,   171,    58,   162,   138,   133,    14,
      67,   138,   134,   135,   134,   135,   135,   137,   139,    10,
      11,    12,    13,    25,    26,    34,    35,    51,    52,   139,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   174,   174,
     174,   174,   174,   174,   172,   174,   174,   174,   174,   174,
     174,   174,   174,     7,     8,     9,   175,   177,   177,   177,
     178,   178,   179,   180,   180,   181,   181,   182,   182,   182,
     182,   182,   182,   182,    14,    23,    25,    26,    34,    35,
      39,    45,    46,    53,    54,    55,    56,   185,    17,   185,
     132,   133,   132,   132,   132,   132,   132,   132,   118,   133,
     134,   171,   132,   135,   125,   126,   135,   137,   121,   139,
     132,   133,    14,   133,    82,    14,   134,   135,   134,   135,
     171,    14,   171,   171,   171,   171,   171,   171,   171,   171,
     137,   188,   132,   134,   171,   171,   133,   171,     5,    14,
     133,   167,   171,   118,   132,   118,   132,   118,   132,   133,
     158,   118,   118,   134,   135,   171,   171,   171,   171,   171,
     171,   171,   188,   135,   171,   171,   188,   134,    28,    29,
     146,   134,    10,   188,   178,   178,    10,    11,    14,    70,
      71,    72,   134,   168,   143,   188,   171,   171,   171,   171,
     171,   171,   132,   188,   155,    14,   171,    14,   171,   134,
      28,   134,   171,   171,    10,   132,   132,   132,   132,   132,
     139,   171,   169,    14,    23,   138,   163,   134,   168,   133,
     133,   165,   134,   188,   118,    10,   137,   188,   131,   136,
     138,   118,   171,   171,   171,   139,   171,    27,    47,   139,
      27,    47,   139,    27,    30,    31,    40,    43,    44,    47,
      48,   139,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,    99,   133,
      14,   133,   134,   188,   171,   134,   188,   135,   188,   171,
     171,   188,   118,   118,   118,   134,   188,   134,   166,   188,
     171,   133,    14,   140,   166,   137,   121,   139,   137,   132,
     132,   171,   132,   135,    82,    82,   167,   132,   135,   132,
     171,   133,   171,   171,   134,   168,   171,   171,   118,   188,
     132,   132,   132,   132,   132,   132,   132,   132,   188,   132,
     132,   132,   169,   145,   169,   133,   171,   169,   169,    85,
     169,    85,    85,    23,   118,    10,    75,    76,   158,   134,
     135,    25,    26,    35,   139,   171,   132,   132,   132,   132,
     132,   132,   132,    41,    42,   139,   121,   139,   121,   139,
     169,   169,   169,   132,   132,   118,   171,    14,    14,   163,
      61,    62,    65,   139,   158,   134,   134,   168,   168,    79,
      80,    81,   139,   134,   171,   137,    14,   171,   171,   171,
      27,    31,    40,    43,    44,    47,   139,   139,   139,   139,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   172,   134,   188,   134,   166,   188,   132,   134,
     132,   134,   188,   132,   137,   137,   137,   171,   171,   171,
     134,   134,   134,   171,    82,    14,   134,   135,   188,   171,
     171,   134,     5,    14,   132,   171,   132,   135,   132,   158,
     134,   132,   132,   171,   134,   134,    28,   146,   171,   169,
      10,    10,    10,    14,   171,   118,    10,    10,   158,    10,
      11,    14,    70,    71,    72,   171,   171,   139,   171,    27,
      30,    31,    40,    43,    44,    47,    48,   139,   171,   169,
     132,   132,    28,   171,   132,   139,    63,   185,    10,    13,
      14,    15,    16,    61,   158,   158,   134,   134,    14,    14,
      14,   132,   118,   133,   137,   139,   139,   171,   171,   171,
     171,   171,   171,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   134,   134,   134,   132,   134,
     132,   132,   132,   134,   171,   133,   188,   132,   132,    82,
      82,   135,   188,   158,   132,   135,   132,   132,   145,   169,
     134,    28,   146,   169,   169,   169,   171,    23,   118,    10,
      75,    76,    27,    31,    47,   139,   139,   139,   171,   171,
     171,   171,   171,   171,   171,   171,   169,   139,   169,   132,
     152,   171,   133,   138,   159,   133,   159,    14,    23,    73,
     118,   132,    14,    23,    73,   118,   132,    10,    13,    15,
      16,   132,   132,   185,   158,   158,     7,     7,     7,   171,
     134,   188,   118,   118,   139,   139,   139,   139,   139,   139,
     132,   171,   134,   171,   171,   188,   132,   188,   169,   169,
      14,   171,   118,    10,    10,   171,   171,   171,   139,   139,
     139,   139,   139,   139,   139,   139,   171,   134,   134,   168,
      77,   143,   134,   168,    14,    14,   132,   171,    14,    14,
     132,   171,    14,    23,    73,   118,   132,    14,    23,    73,
     118,   132,   132,   132,   133,   159,   139,   139,   134,   171,
     171,   134,   132,   134,   132,    28,   146,   171,   139,   139,
     139,   132,   169,   159,   134,   139,    25,    26,    35,   139,
     171,   159,   134,   132,   132,   132,   132,   132,   132,    14,
      14,   132,   171,    14,    14,   132,   171,   134,   168,   139,
     139,   139,   132,   169,   152,   159,   171,   171,   139,   171,
      27,    30,    31,    40,    43,    44,    47,    48,   139,   159,
     132,   132,   132,   132,   132,   132,   159,   134,   134,    27,
      31,    47,   139,   139,   139,   171,   171,   171,   171,   171,
     171,   171,   171,   159,   169,   171,   171,   171,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   141,   142,   143,   143,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   146,   146,   146,   146,   147,   147,   148,   148,
     148,   148,   149,   149,   150,   150,   150,   151,   151,   151,
     152,   153,   153,   153,   153,   153,   153,   154,   154,   155,
     155,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   157,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   160,
     161,   162,   162,   162,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   164,   165,
     165,   165,   165,   166,   166,   166,   166,   167,   167,   167,
     167,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   172,
     172,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   174,
     174,   175,   175,   175,   176,   176,   177,   177,   177,   178,
     178,   179,   179,   179,   180,   180,   180,   181,   181,   181,
     181,   181,   181,   181,   182,   182,   182,   182,   182,   182,
     183,   183,   184,   184,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189
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
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       3,     4,     6,     4,     3,     6,     6,     5,     3,     3,
       6,     5,     4,     4,     6,     6,     5,     3,     6,     5,
       3,     2,     3,     4,     3,     2,     3,     2,     1,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     4,     6,     6,
       6,     1,     4,     4,     5,     5,     1,     1,     1,     1,
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
#line 508 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3217 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 518 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3223 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 519 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3229 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 523 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3235 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 525 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3241 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 527 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3247 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 529 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3255 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 533 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3261 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 535 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3267 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 537 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3273 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3279 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 541 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3287 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 545 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3295 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 549 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3301 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 551 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3309 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 555 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3317 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 559 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3323 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 561 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3329 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 563 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3335 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3341 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3347 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 569 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3355 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 573 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3363 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3369 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3375 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3381 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3387 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 585 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3393 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 587 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3399 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 589 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3405 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 591 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3411 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 593 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3417 "raku.tab.c"
    break;

  case 34: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 595 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3429 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 603 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3435 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 605 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3441 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 607 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3447 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 609 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3453 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 611 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3459 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 613 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3465 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 615 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3472 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 618 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3479 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 621 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3485 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 623 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3492 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 626 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3499 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 629 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3505 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 631 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3513 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 635 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3519 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 637 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3525 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 639 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3532 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 642 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3539 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 645 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3545 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 647 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3552 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 650 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3559 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 653 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3566 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 656 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3572 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 658 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3581 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 663 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3589 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 667 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3597 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 671 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3606 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 676 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3614 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall ';'  */
#line 679 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3620 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3626 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3632 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 685 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3638 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 687 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3644 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 689 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3651 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 692 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3657 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 694 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3663 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 696 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3669 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 698 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3676 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 701 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3683 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 704 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3690 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 707 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3697 "raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 710 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3704 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_IF expr ';'  */
#line 713 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3710 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNLESS expr ';'  */
#line 715 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3716 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WHILE expr ';'  */
#line 717 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3722 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNTIL expr ';'  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3728 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_FOR expr ';'  */
#line 721 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3735 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITH expr ';'  */
#line 724 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3741 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITHOUT expr ';'  */
#line 726 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3747 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_GIVEN expr ';'  */
#line 728 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3753 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 730 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3760 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 733 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3767 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 736 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3775 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 740 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3782 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 743 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3788 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 745 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3794 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 747 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3800 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 749 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3807 "raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 752 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 755 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3821 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 758 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3828 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 761 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3835 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 764 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3842 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 767 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3849 "raku.tab.c"
    break;

  case 98: /* stmt: expr ';'  */
#line 769 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3855 "raku.tab.c"
    break;

  case 99: /* stmt: ';'  */
#line 770 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3861 "raku.tab.c"
    break;

  case 100: /* stmt: if_stmt  */
#line 771 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3867 "raku.tab.c"
    break;

  case 101: /* stmt: while_stmt  */
#line 772 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3873 "raku.tab.c"
    break;

  case 102: /* stmt: for_stmt  */
#line 773 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3879 "raku.tab.c"
    break;

  case 103: /* stmt: given_stmt  */
#line 774 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3885 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3891 "raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block KW_CATCH block  */
#line 778 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3897 "raku.tab.c"
    break;

  case 106: /* stmt: KW_CATCH block  */
#line 780 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3903 "raku.tab.c"
    break;

  case 107: /* stmt: block  */
#line 782 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3909 "raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 783 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3915 "raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 784 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3921 "raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 785 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3927 "raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 786 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3933 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 787 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3939 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 788 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3945 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 790 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3951 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3957 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 794 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3963 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3969 "raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 797 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3975 "raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 798 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3981 "raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 799 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3987 "raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 800 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3993 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3999 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4005 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4011 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4017 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4023 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4029 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block elsif_tail  */
#line 816 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4035 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 818 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4041 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4047 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4053 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4059 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4065 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 830 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4071 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 832 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4077 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 836 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4083 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE expr block  */
#line 838 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4089 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4095 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 844 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4101 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block  */
#line 846 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4107 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4113 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4119 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL expr block  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4125 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block  */
#line 858 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4131 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 860 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4137 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 862 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4143 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP block  */
#line 866 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4149 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 868 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4155 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 870 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4161 "raku.tab.c"
    break;

  case 150: /* loop_incr: expr  */
#line 873 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4167 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 877 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4177 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 883 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4187 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 889 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4196 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 894 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4206 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 900 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4215 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr block  */
#line 905 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4222 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 910 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4234 "raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 918 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4247 "raku.tab.c"
    break;

  case 159: /* when_list: %empty  */
#line 928 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4253 "raku.tab.c"
    break;

  case 160: /* when_list: when_list KW_WHEN expr block  */
#line 930 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4261 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 936 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4273 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 944 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4283 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT sub_body  */
#line 950 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4293 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 956 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4305 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 964 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4315 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 970 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4325 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 976 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4338 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 985 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4349 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 992 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4362 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1001 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4375 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1010 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4386 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1019 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4397 "raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1026 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4406 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list '}'  */
#line 1032 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4412 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr '}'  */
#line 1034 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4419 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1037 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4426 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1040 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4432 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1042 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4438 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1044 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4444 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1046 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4450 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1048 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4456 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1050 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4462 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1052 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4468 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1054 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1057 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4481 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1059 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4487 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1061 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4493 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1063 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4500 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1066 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4507 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1069 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4515 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list '}'  */
#line 1074 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4521 "raku.tab.c"
    break;

  case 192: /* method_body: '{' YADA '}'  */
#line 1075 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4527 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1077 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4534 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1080 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4541 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1083 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4547 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1085 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4553 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1087 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4559 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1089 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4565 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1091 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1093 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4577 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1095 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1097 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1100 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4596 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1102 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4602 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1104 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1106 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4615 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1109 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4622 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1112 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4630 "raku.tab.c"
    break;

  case 209: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
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
#line 4648 "raku.tab.c"
    break;

  case 210: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
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
#line 4665 "raku.tab.c"
    break;

  case 211: /* is_clauses: %empty  */
#line 1148 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4671 "raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses IDENT IDENT  */
#line 1150 "raku.y"
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
#line 4687 "raku.tab.c"
    break;

  case 213: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1162 "raku.y"
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
#line 4703 "raku.tab.c"
    break;

  case 214: /* class_body_list: %empty  */
#line 1175 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4709 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1177 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4716 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1180 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4723 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1183 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4730 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1186 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4737 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1189 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4744 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1192 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4751 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1195 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4758 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1198 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4765 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1201 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4772 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1204 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4779 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1207 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4786 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1210 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4793 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1213 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4804 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1220 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4815 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1227 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4826 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1234 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4837 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1241 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4845 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1245 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4853 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1249 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4861 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1253 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4869 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1257 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4880 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1264 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4891 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1271 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4902 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1278 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4913 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1285 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4927 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1295 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4939 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1303 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4951 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1311 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4964 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1320 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4975 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1327 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4986 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1334 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5000 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1344 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5012 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1352 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5024 "raku.tab.c"
    break;

  case 248: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1362 "raku.y"
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
#line 5041 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: %empty  */
#line 1376 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5047 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1378 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5056 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1383 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5065 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1388 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5074 "raku.tab.c"
    break;

  case 253: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1395 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5082 "raku.tab.c"
    break;

  case 254: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1399 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5090 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1403 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5098 "raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1407 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5106 "raku.tab.c"
    break;

  case 257: /* pair_list: IDENT OP_FATARROW expr  */
#line 1413 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5112 "raku.tab.c"
    break;

  case 258: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1415 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5118 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1417 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5124 "raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1419 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5130 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR  */
#line 1422 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5136 "raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1423 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5142 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1424 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5148 "raku.tab.c"
    break;

  case 264: /* param_list: VAR_ARRAY  */
#line 1425 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5154 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_ARRAY  */
#line 1426 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5160 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT VAR_SCALAR  */
#line 1427 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5166 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1428 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5172 "raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1429 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5178 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_SCALAR  */
#line 1430 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5184 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1431 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5190 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1432 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5196 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1433 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5202 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR '=' expr  */
#line 1434 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5208 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1435 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5214 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1436 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5220 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1437 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5226 "raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_POS  */
#line 1438 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5232 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_POS  */
#line 1439 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5238 "raku.tab.c"
    break;

  case 279: /* param_list: SLURPY_LOL  */
#line 1440 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5244 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' SLURPY_LOL  */
#line 1441 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5250 "raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_NAMED  */
#line 1442 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5256 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1443 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5262 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list '}'  */
#line 1446 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5268 "raku.tab.c"
    break;

  case 284: /* block: '{' YADA '}'  */
#line 1447 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5274 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr '}'  */
#line 1449 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5280 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1451 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5286 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1453 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5292 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1455 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5298 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1457 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5304 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1459 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5311 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1462 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5317 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1464 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5323 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1466 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5329 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1468 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5335 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1470 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1473 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5349 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1476 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5357 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1480 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5364 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1483 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5371 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1486 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5378 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1489 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5384 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1491 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5392 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1495 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5399 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1498 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5406 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1501 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5413 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1504 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5420 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1507 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5427 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1510 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5433 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1512 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5439 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1514 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5445 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_LAST '}'  */
#line 1516 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5451 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1518 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5457 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1520 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5463 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1522 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5469 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1524 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5475 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1526 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5481 "raku.tab.c"
    break;

  case 317: /* closure: '{' expr '}'  */
#line 1529 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5487 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR '=' expr  */
#line 1532 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5493 "raku.tab.c"
    break;

  case 319: /* expr: VAR_ARRAY '=' expr  */
#line 1533 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5499 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1534 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5505 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1535 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5511 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1536 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5517 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1537 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5523 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1538 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5529 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER block  */
#line 1539 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5539 "raku.tab.c"
    break;

  case 326: /* expr: KW_GATHER for_stmt  */
#line 1544 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5549 "raku.tab.c"
    break;

  case 327: /* expr: tern_expr OP_FATARROW expr  */
#line 1550 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5555 "raku.tab.c"
    break;

  case 328: /* expr: tern_expr  */
#line 1551 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5561 "raku.tab.c"
    break;

  case 329: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1555 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5567 "raku.tab.c"
    break;

  case 330: /* tern_expr: cmp_expr  */
#line 1556 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5573 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1559 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5579 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1560 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5585 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1561 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5591 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1562 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5597 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1563 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5603 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1564 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5609 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1565 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5615 "raku.tab.c"
    break;

  case 338: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1566 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5621 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1567 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5627 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1568 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5633 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1569 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5639 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1570 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5645 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1571 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5651 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1572 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5657 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1574 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5667 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1580 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5677 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1586 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5687 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr  */
#line 1591 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5693 "raku.tab.c"
    break;

  case 349: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1594 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5699 "raku.tab.c"
    break;

  case 350: /* divis_expr: jct_expr  */
#line 1595 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5705 "raku.tab.c"
    break;

  case 351: /* jct_expr: jct_expr '|' range_expr  */
#line 1598 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5711 "raku.tab.c"
    break;

  case 352: /* jct_expr: jct_expr '&' range_expr  */
#line 1599 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5717 "raku.tab.c"
    break;

  case 353: /* jct_expr: dor_expr  */
#line 1600 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5723 "raku.tab.c"
    break;

  case 354: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1604 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5729 "raku.tab.c"
    break;

  case 355: /* dor_expr: range_expr  */
#line 1605 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5735 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1608 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5741 "raku.tab.c"
    break;

  case 357: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1609 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5747 "raku.tab.c"
    break;

  case 358: /* range_expr: add_expr  */
#line 1610 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5753 "raku.tab.c"
    break;

  case 359: /* add_expr: add_expr '~' repl_expr  */
#line 1613 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5759 "raku.tab.c"
    break;

  case 360: /* add_expr: repl_expr  */
#line 1614 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5765 "raku.tab.c"
    break;

  case 361: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1617 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5771 "raku.tab.c"
    break;

  case 362: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1618 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5777 "raku.tab.c"
    break;

  case 363: /* repl_expr: addsub_expr  */
#line 1619 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5783 "raku.tab.c"
    break;

  case 364: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1622 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5789 "raku.tab.c"
    break;

  case 365: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1623 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5795 "raku.tab.c"
    break;

  case 366: /* addsub_expr: mul_expr  */
#line 1624 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5801 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '*' unary_expr  */
#line 1627 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5807 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr '/' unary_expr  */
#line 1628 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5813 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr '%' unary_expr  */
#line 1629 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5819 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1630 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5825 "raku.tab.c"
    break;

  case 371: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1632 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5831 "raku.tab.c"
    break;

  case 372: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1634 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5837 "raku.tab.c"
    break;

  case 373: /* mul_expr: unary_expr  */
#line 1635 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5843 "raku.tab.c"
    break;

  case 374: /* unary_expr: '-' unary_expr  */
#line 1638 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5849 "raku.tab.c"
    break;

  case 375: /* unary_expr: '+' unary_expr  */
#line 1639 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5855 "raku.tab.c"
    break;

  case 376: /* unary_expr: '!' unary_expr  */
#line 1640 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5861 "raku.tab.c"
    break;

  case 377: /* unary_expr: CARET unary_expr  */
#line 1641 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5867 "raku.tab.c"
    break;

  case 378: /* unary_expr: OP_REDUCE unary_expr  */
#line 1643 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5876 "raku.tab.c"
    break;

  case 379: /* unary_expr: pow_expr  */
#line 1647 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5882 "raku.tab.c"
    break;

  case 380: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1650 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5888 "raku.tab.c"
    break;

  case 381: /* pow_expr: postfix_expr  */
#line 1651 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5894 "raku.tab.c"
    break;

  case 382: /* scalar_list: VAR_SCALAR  */
#line 1654 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5900 "raku.tab.c"
    break;

  case 383: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1655 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5906 "raku.tab.c"
    break;

  case 384: /* meth_name: IDENT  */
#line 1658 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5912 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_SORT  */
#line 1659 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5918 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_REVERSE  */
#line 1660 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 5924 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_MAP  */
#line 1661 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5930 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_GREP  */
#line 1662 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5936 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_SAY  */
#line 1663 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5942 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_PRINT  */
#line 1664 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5948 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_TAKE  */
#line 1665 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5954 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_RETURN  */
#line 1666 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5960 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_EXISTS  */
#line 1667 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5966 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_DELETE  */
#line 1668 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5972 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_JOIN  */
#line 1669 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 5978 "raku.tab.c"
    break;

  case 396: /* meth_name: TESTOP  */
#line 1670 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5984 "raku.tab.c"
    break;

  case 397: /* postfix_expr: call_expr  */
#line 1672 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5990 "raku.tab.c"
    break;

  case 398: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1675 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6000 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '(' arg_list ')'  */
#line 1681 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6009 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '(' ')'  */
#line 1685 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6015 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1687 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6021 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1689 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6027 "raku.tab.c"
    break;

  case 403: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1691 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6036 "raku.tab.c"
    break;

  case 404: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1696 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6042 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1698 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6052 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1704 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6062 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1710 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6070 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' KW_NEW  */
#line 1714 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6078 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '.' IDENT  */
#line 1718 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6087 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1723 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6098 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1730 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6107 "raku.tab.c"
    break;

  case 412: /* call_expr: IDENT '.' CARET IDENT  */
#line 1735 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6117 "raku.tab.c"
    break;

  case 413: /* call_expr: atom '.' CARET IDENT  */
#line 1741 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6127 "raku.tab.c"
    break;

  case 414: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1747 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6138 "raku.tab.c"
    break;

  case 415: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1754 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6149 "raku.tab.c"
    break;

  case 416: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1761 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6158 "raku.tab.c"
    break;

  case 417: /* call_expr: atom '.' meth_name  */
#line 1766 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6167 "raku.tab.c"
    break;

  case 418: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1771 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6178 "raku.tab.c"
    break;

  case 419: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1778 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6187 "raku.tab.c"
    break;

  case 420: /* call_expr: call_expr '.' meth_name  */
#line 1783 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6196 "raku.tab.c"
    break;

  case 421: /* call_expr: KW_DIE expr  */
#line 1788 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6202 "raku.tab.c"
    break;

  case 422: /* call_expr: KW_MAP closure expr  */
#line 1790 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6208 "raku.tab.c"
    break;

  case 423: /* call_expr: KW_MAP closure ',' expr  */
#line 1792 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6214 "raku.tab.c"
    break;

  case 424: /* call_expr: KW_GREP closure expr  */
#line 1794 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6220 "raku.tab.c"
    break;

  case 425: /* call_expr: KW_SORT expr  */
#line 1796 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6226 "raku.tab.c"
    break;

  case 426: /* call_expr: KW_SORT closure expr  */
#line 1798 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6232 "raku.tab.c"
    break;

  case 427: /* call_expr: KW_REVERSE expr  */
#line 1800 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6238 "raku.tab.c"
    break;

  case 428: /* call_expr: atom  */
#line 1801 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6244 "raku.tab.c"
    break;

  case 429: /* arg_list: expr  */
#line 1804 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6250 "raku.tab.c"
    break;

  case 430: /* arg_list: arg_list ',' expr  */
#line 1805 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6256 "raku.tab.c"
    break;

  case 431: /* arg_list: arg_list ','  */
#line 1806 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6262 "raku.tab.c"
    break;

  case 432: /* atom: LIT_INT  */
#line 1809 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6268 "raku.tab.c"
    break;

  case 433: /* atom: LIT_FLOAT  */
#line 1810 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6274 "raku.tab.c"
    break;

  case 434: /* atom: LIT_STR  */
#line 1811 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6280 "raku.tab.c"
    break;

  case 435: /* atom: WORDLIST  */
#line 1813 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6293 "raku.tab.c"
    break;

  case 436: /* atom: LIT_INTERP_STR  */
#line 1821 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6299 "raku.tab.c"
    break;

  case 437: /* atom: VAR_SCALAR  */
#line 1822 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6305 "raku.tab.c"
    break;

  case 438: /* atom: OP_INC VAR_SCALAR  */
#line 1823 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6311 "raku.tab.c"
    break;

  case 439: /* atom: OP_DEC VAR_SCALAR  */
#line 1824 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6317 "raku.tab.c"
    break;

  case 440: /* atom: VAR_SCALAR OP_INC  */
#line 1825 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6323 "raku.tab.c"
    break;

  case 441: /* atom: VAR_SCALAR OP_DEC  */
#line 1826 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6329 "raku.tab.c"
    break;

  case 442: /* atom: VAR_ARRAY  */
#line 1827 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6335 "raku.tab.c"
    break;

  case 443: /* atom: VAR_HASH  */
#line 1828 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6341 "raku.tab.c"
    break;

  case 444: /* atom: VAR_CAPTURE  */
#line 1830 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6349 "raku.tab.c"
    break;

  case 445: /* atom: VAR_FH  */
#line 1834 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6357 "raku.tab.c"
    break;

  case 446: /* atom: VAR_NAMED_CAPTURE  */
#line 1838 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6364 "raku.tab.c"
    break;

  case 447: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1841 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6370 "raku.tab.c"
    break;

  case 448: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1843 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6376 "raku.tab.c"
    break;

  case 449: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1845 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6382 "raku.tab.c"
    break;

  case 450: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1847 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6388 "raku.tab.c"
    break;

  case 451: /* atom: ARR_ALL_SLICE  */
#line 1849 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6394 "raku.tab.c"
    break;

  case 452: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1851 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6400 "raku.tab.c"
    break;

  case 453: /* atom: VAR_HASH '{' expr '}'  */
#line 1853 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6406 "raku.tab.c"
    break;

  case 454: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1855 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6412 "raku.tab.c"
    break;

  case 455: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1857 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6418 "raku.tab.c"
    break;

  case 456: /* atom: IDENT  */
#line 1858 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6424 "raku.tab.c"
    break;

  case 457: /* atom: VAR_TWIGIL  */
#line 1860 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6432 "raku.tab.c"
    break;

  case 458: /* atom: VAR_ARRAY_TWIGIL  */
#line 1864 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6440 "raku.tab.c"
    break;

  case 459: /* atom: VAR_HASH_TWIGIL  */
#line 1868 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6448 "raku.tab.c"
    break;

  case 460: /* atom: '[' ']'  */
#line 1871 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6454 "raku.tab.c"
    break;

  case 461: /* atom: '[' expr ']'  */
#line 1873 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6460 "raku.tab.c"
    break;

  case 462: /* atom: '[' expr ',' ']'  */
#line 1875 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6466 "raku.tab.c"
    break;

  case 463: /* atom: '[' expr ',' arg_list ']'  */
#line 1877 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6473 "raku.tab.c"
    break;

  case 464: /* atom: DOLLAR_LBRACKET ']'  */
#line 1879 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6479 "raku.tab.c"
    break;

  case 465: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1881 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6485 "raku.tab.c"
    break;

  case 466: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1883 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6491 "raku.tab.c"
    break;

  case 467: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1885 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6498 "raku.tab.c"
    break;

  case 468: /* atom: '(' ')'  */
#line 1887 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6504 "raku.tab.c"
    break;

  case 469: /* atom: '(' expr ')'  */
#line 1888 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6510 "raku.tab.c"
    break;

  case 470: /* atom: '(' expr ',' ')'  */
#line 1890 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6516 "raku.tab.c"
    break;

  case 471: /* atom: '(' expr ',' arg_list ')'  */
#line 1892 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6523 "raku.tab.c"
    break;

  case 472: /* atom: block  */
#line 1894 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6529 "raku.tab.c"
    break;

  case 473: /* atom: KW_SUB block  */
#line 1895 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6535 "raku.tab.c"
    break;


#line 6539 "raku.tab.c"

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

#line 1897 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
