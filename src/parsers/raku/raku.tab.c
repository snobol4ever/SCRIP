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
  YYSYMBOL_class_decl = 161,               /* class_decl  */
  YYSYMBOL_role_decl = 162,                /* role_decl  */
  YYSYMBOL_is_clauses = 163,               /* is_clauses  */
  YYSYMBOL_class_body_list = 164,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 165,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 166,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 167,           /* named_arg_list  */
  YYSYMBOL_pair_list = 168,                /* pair_list  */
  YYSYMBOL_param_list = 169,               /* param_list  */
  YYSYMBOL_block = 170,                    /* block  */
  YYSYMBOL_closure = 171,                  /* closure  */
  YYSYMBOL_expr = 172,                     /* expr  */
  YYSYMBOL_tern_expr = 173,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 174,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 175,               /* divis_expr  */
  YYSYMBOL_jct_expr = 176,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 177,                 /* dor_expr  */
  YYSYMBOL_range_expr = 178,               /* range_expr  */
  YYSYMBOL_add_expr = 179,                 /* add_expr  */
  YYSYMBOL_repl_expr = 180,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 181,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 182,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 183,               /* unary_expr  */
  YYSYMBOL_pow_expr = 184,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 185,              /* scalar_list  */
  YYSYMBOL_meth_name = 186,                /* meth_name  */
  YYSYMBOL_postfix_expr = 187,             /* postfix_expr  */
  YYSYMBOL_call_expr = 188,                /* call_expr  */
  YYSYMBOL_arg_list = 189,                 /* arg_list  */
  YYSYMBOL_atom = 190                      /* atom  */
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
#define YYLAST   5233

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  142
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  476
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1195

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
       0,   508,   508,   519,   520,   523,   525,   527,   529,   533,
     535,   537,   539,   541,   545,   549,   551,   555,   559,   561,
     563,   565,   567,   569,   573,   577,   579,   581,   583,   585,
     587,   589,   591,   593,   595,   597,   599,   601,   609,   611,
     613,   615,   617,   619,   621,   624,   627,   629,   632,   635,
     637,   641,   643,   645,   648,   651,   653,   656,   659,   662,
     664,   669,   673,   677,   682,   686,   687,   689,   691,   693,
     695,   698,   700,   702,   704,   707,   710,   713,   716,   719,
     721,   723,   725,   727,   730,   732,   734,   736,   739,   742,
     746,   749,   751,   753,   755,   758,   761,   764,   767,   770,
     773,   776,   777,   778,   779,   780,   781,   782,   784,   786,
     788,   790,   791,   792,   793,   794,   795,   796,   798,   800,
     802,   804,   805,   806,   807,   810,   812,   814,   816,   818,
     820,   822,   824,   828,   830,   832,   834,   836,   838,   842,
     844,   848,   850,   852,   854,   858,   860,   864,   866,   868,
     872,   874,   876,   880,   883,   889,   895,   900,   906,   911,
     916,   924,   935,   936,   942,   950,   956,   962,   970,   976,
     982,   991,   998,  1007,  1016,  1025,  1032,  1039,  1040,  1043,
    1046,  1048,  1050,  1052,  1054,  1056,  1058,  1060,  1063,  1065,
    1067,  1069,  1072,  1075,  1081,  1082,  1083,  1086,  1089,  1091,
    1093,  1095,  1097,  1099,  1101,  1103,  1106,  1108,  1110,  1112,
    1115,  1118,  1124,  1140,  1155,  1156,  1168,  1182,  1183,  1186,
    1189,  1192,  1195,  1198,  1201,  1204,  1207,  1210,  1213,  1216,
    1219,  1226,  1233,  1240,  1247,  1251,  1255,  1259,  1263,  1270,
    1277,  1284,  1291,  1301,  1309,  1317,  1326,  1333,  1340,  1350,
    1358,  1368,  1383,  1384,  1389,  1394,  1401,  1405,  1409,  1413,
    1419,  1421,  1423,  1425,  1429,  1430,  1431,  1432,  1433,  1434,
    1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,
    1445,  1446,  1447,  1448,  1449,  1450,  1453,  1454,  1455,  1457,
    1459,  1461,  1463,  1465,  1468,  1470,  1472,  1474,  1476,  1479,
    1482,  1486,  1489,  1492,  1495,  1497,  1501,  1504,  1507,  1510,
    1513,  1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,  1532,
    1536,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,  1551,
    1556,  1558,  1561,  1563,  1566,  1567,  1568,  1569,  1570,  1571,
    1572,  1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1586,
    1592,  1598,  1601,  1602,  1605,  1606,  1607,  1610,  1612,  1615,
    1616,  1617,  1620,  1621,  1624,  1625,  1626,  1629,  1630,  1631,
    1634,  1635,  1636,  1637,  1638,  1640,  1642,  1645,  1646,  1647,
    1648,  1649,  1654,  1657,  1658,  1661,  1662,  1665,  1666,  1667,
    1668,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1679,  1681,  1687,  1692,  1693,  1695,  1697,  1702,  1704,  1710,
    1716,  1720,  1724,  1729,  1736,  1741,  1747,  1753,  1760,  1767,
    1772,  1777,  1784,  1789,  1794,  1796,  1798,  1800,  1802,  1804,
    1806,  1808,  1811,  1812,  1813,  1816,  1817,  1818,  1819,  1828,
    1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,  1840,  1844,
    1847,  1849,  1851,  1853,  1855,  1857,  1859,  1861,  1863,  1865,
    1866,  1870,  1874,  1878,  1879,  1881,  1883,  1886,  1887,  1889,
    1891,  1894,  1895,  1896,  1898,  1901,  1902
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

#define YYPACT_NINF (-877)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-476)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -877,    85,  2211,  -877,  -877,  -877,  -877,  -877,   549,   112,
     162,    17,    -1,  -877,  -877,  5094,  2307,  -877,  -877,  -877,
     529,  2382,    29,  4119,  4194,  4269,  4344,  4419,     6,   -14,
    4419,  2457,  2532,   769,   153,  4419,  4419,   174,   206,  4494,
    4569,    88,   -55,    28,    58,   109,   109,  4644,  4419,    88,
      88,  4419,   288,   296,    74,  5094,  -877,  -877,   305,   269,
     311,  5094,  5094,  5094,  -877,  2607,  2683,   312,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,   487,  -877,
    -877,  -877,   736,   536,   264,   505,   697,   523,   241,  -877,
     115,   545,   447,   797,  -877,  -877,   278,   315,   342,   368,
    -877,  -877,  4419,  4419,  4419,  4419,  4419,  4419,   441,  2758,
    4719,  2834,   478,  4419,  4419,   387,   259,  1121,   128,   364,
     250,  -877,   464,    88,  2909,  -877,  -877,   584,   265,  -877,
     793,  2985,  3060,  -877,  3135,  -877,   227,   224,   326,   344,
     958,   595,   831,   612,  2909,   450,  2909,    70,  2909,    88,
    2909,    88,    11,   120,   -19,  -877,  -877,  -877,   284,  -877,
      62,  -877,   159,   509,   517,   627,   516,   558,   271,   277,
    2909,    88,  2909,    88,   372,  3211,  -877,  4419,  4419,  -877,
    4419,  4419,  -877,  4419,  3286,  4419,  3361,  4419,  -877,  -877,
     664,  -877,  -877,  -877,   589,   619,    45,  -877,   643,  -877,
    -877,  -877,  -877,  -877,   667,  -877,   544,   575,  -877,   825,
     678,  1551,  4419,  4419,  4419,  4419,  4419,  4419,  4419,  4419,
    -877,  4419,  4419,  4419,  4419,  4419,  4419,  4419,  4419,  -877,
    4419,  5094,  5094,  5094,  5094,  5094,  5094,  5094,  5094,  5094,
    5094,  5094,  5094,  5094,  5094,  5094,   966,  5094,  5094,  5094,
    5094,  5094,  5094,  5094,  5094,  5094,  5094,  5094,  5094,  5094,
    5094,  5094,  5094,  5094,  5094,   867,   695,   606,   707,   709,
     713,   743,   764,   775,   257,  -877,   598,  2909,   497,   679,
     828,   790,   806,   838,  -877,   857,   989,   882,   -15,  -877,
    1011,   784,   815,  2834,  1054,  4419,  4419,  4419,  4419,  4419,
    4419,  4419,  4419,  3437,  -877,  -877,   960,  -877,   971,   976,
     544,   859,  -877,  4419,  4419,  -877,  4794,  -877,  4869,  -877,
     366,   394,   405,   179,   977,   994,  -877,   861,   863,  4419,
    4419,  4419,  4419,  4419,  4419,  4419,  -877,  4419,   866,  4419,
    4419,  -877,  4419,   870,   978,   893,  -877,  1068,  4419,  -877,
    5094,  5094,     2,  -877,  -877,  -877,  4419,  4419,  4419,  -877,
    4419,  4419,  -877,  4419,  4419,   984,  4419,  -877,  1114,  4419,
    1115,  4419,   895,  1101,   897,  -877,  4419,  4419,  1126,  1019,
    1020,  1023,  1024,  1025,  1022,  4419,  -877,  -877,  -877,    88,
      12,  -877,    21,  1026,  1032,  -877,  -877,  3512,  1040,  1158,
    3588,  -877,  -877,   833,   299,   323,  1050,  4119,  4194,  4419,
    1923,    32,   106,  -877,   383,  1038,  1039,  1041,  1046,  1047,
    1051,  1052,  1053,  1055,  1056,  1059,  1061,  1062,  1064,  1065,
    1067,  -877,  1058,  1058,  1058,  1058,  1058,  1058,  1070,  1058,
    1058,  1058,  1058,  1058,  1058,  1058,  1058,  -877,  -877,  -877,
     523,  -877,  -877,  -877,  1076,  1076,   545,   447,   447,   797,
     797,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  1049,  1173,  1073,  -877,  3663,  -877,  -877,  -877,  -877,
    -877,  -877,  4419,  3739,  -877,   899,  -877,  4419,  4419,  4419,
    4419,  1074,  1083,  1087,  -877,  3815,  -877,  1260,  4419,  1075,
    -877,    -6,  -877,  1412,   846,  1086,  1077,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,   847,  -877,  -877,  -877,  1078,
    -877,  1079,  2909,   534,  1127,   183,  3891,   631,  1080,  4419,
    -877,  4944,  -877,  4419,  -877,    55,  -877,  4419,  4419,  1095,
    3512,  1082,  1085,  1088,  1089,  1090,  1094,  1096,   651,  3512,
    1097,  1099,   759,    88,    14,  5019,  -877,    88,    88,    38,
     -42,   -38,    -5,  -877,   161,  -877,  -877,  -877,  1098,   901,
    1689,   763,  1100,  1103,  1105,  1106,  1107,  1109,  -877,  1092,
      53,  1112,  1104,  1123,  1110,    88,    88,    88,  1113,  1116,
    1124,  4419,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  1202,
    1205,  -877,   208,  1098,   903,    67,   432,   353,  -877,   905,
    4419,  -877,  -877,   850,  1237,  2834,  4419,  4419,   237,    22,
     389,  -877,   260,  4419,  4419,  -877,  4419,  4419,  -877,  4419,
    4419,  4419,  4419,  4419,  4419,  4419,  4419,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  5094,  3967,  -877,  1336,  1119,   907,
    1120,  -877,   909,  3512,   782,  1129,  1130,   851,  4419,  4419,
    4419,  -877,   912,  -877,   914,   916,  -877,  4419,  1171,  1243,
     918,  -877,  -877,  -877,  -877,  -877,  -877,   920,  -877,  4419,
    4419,  4419,   922,  -877,    77,  -877,  1128,  2909,   811,  1136,
    1098,   924,  1142,  1150,  4419,   926,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,   928,  -877,  -877,  -877,  1037,  -877,
    -877,  2909,    88,  -877,  -877,  1249,  -877,  1274,  1275,  1272,
    4419,  1168,  1278,  1279,  -877,  1098,   577,  4119,  4194,  1998,
    -877,   409,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  4419,
      88,  -877,  -877,  -877,  1157,  1159,  1262,  -877,  -877,  -877,
    -877,  4419,  1162,  -877,  -877,   279,   660,   541,  1234,  -877,
    -877,  1098,  1098,   935,   937,  1283,  1284,  1285,  -877,  -877,
    1169,  -877,   304,   854,  1161,   -82,  4419,  4419,  4419,  4419,
    4419,  4419,  -877,  -877,  -877,  -877,   -17,    56,   333,   339,
     367,   369,   393,   407,   419,   429,   449,   457,  -877,  -877,
     939,  -877,   941,   945,  -877,  1172,  -877,  -877,   947,  -877,
    -877,  -877,  -877,  1174,  1175,  1176,  -877,  -877,  -877,  1177,
    4419,  1179,  -877,  3512,   812,  -877,  -877,  1178,  1227,  1235,
    -877,   950,  -877,  4419,  -877,  -877,  1098,  -877,  -877,   822,
    1186,  1188,    14,  -877,   952,  1060,    88,    88,    88,  -877,
    -877,  4419,  -877,  -877,  -877,    33,  -877,   274,  -877,  -877,
    -877,   381,    24,  -877,   261,  4419,  4419,  4419,  4419,  4419,
    4419,  4419,  4419,  -877,    88,  1182,  -877,  -877,    88,  1190,
    4419,  -877,   422,   431,    47,    75,   600,  1191,  1192,   867,
    -877,  -877,  1098,  1098,  1319,  1320,  1321,  -877,  4419,  4043,
    1212,  1213,  -877,   468,   481,   531,   532,   533,   542,  -877,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877,  1200,  -877,  -877,  -877,  -877,
    -877,  4419,   956,  -877,  -877,  4419,  4419,  3512,   823,  -877,
    -877,  4419,  -877,  -877,  -877,  -877,    88,    88,  -877,  -877,
    -877,  -877,  -877,  1322,  4419,  1215,  1327,  1328,  4419,  4419,
    4419,  -877,  -877,  -877,   568,   587,   592,   603,   604,   621,
     622,   625,  -877,  -877,  -877,  4419,  1208,  -877,   187,  1266,
    -877,   233,  -877,  1331,  1337,  1226,  4419,  -877,  1346,  1347,
    1229,  4419,  -877,   262,   266,  1230,  1233,  -877,  -877,   437,
    -877,  -877,  -877,  -877,  -877,   626,  1228,   959,  4419,  4419,
    -877,  -877,  -877,  -877,  -877,  -877,  -877,  1232,  1238,  -877,
    -877,   963,  -877,   824,  1071,  -877,  -877,  -877,  4419,  -877,
    -877,   645,   671,   674,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  1239,    88,  1236,   967,  1241,  1827,  1236,   972,
    1240,  1244,  -877,  1245,  1246,  1250,  -877,  1251,  1360,  1371,
    1255,  4419,  -877,  1375,  1384,  1269,  4419,  -877,  -877,  -877,
     253,  -877,  -877,  -877,  1263,   675,   676,  -877,  -877,  1276,
    -877,    88,  -877,  -877,  -877,  -877,  -877,  4419,  -877,  -877,
    1236,  -877,  4119,  4194,  2073,  -877,   420,  -877,  1236,  -877,
    -877,  -877,  -877,  -877,  -877,  1277,  1280,  -877,  1281,  1286,
    1287,  -877,  1288,  1236,   979,  -877,  -877,  -877,  -877,  -877,
    1273,  -877,   439,   213,  -877,   316,  4419,  4419,  4419,  4419,
    4419,  4419,  4419,  4419,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  1236,    88,  4419,  4419,  4419,  -877,  -877,
    -877,   677,   703,   704,   722,   723,   734,   737,   738,  -877,
    -877,   739,   740,   742,  -877,  -877,  -877,  -877,  -877,  -877,
    -877,  -877,  -877,  -877,  -877
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   435,   436,   437,   439,   440,   445,
     446,   460,   459,   461,   462,     0,     0,   447,   448,   449,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   454,   438,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     3,     4,   103,
     104,   111,   112,   113,   114,   105,   106,   121,     0,   122,
     123,   124,   110,     0,   331,   333,   351,   353,   356,   358,
     361,   363,   366,   369,   376,   382,   384,   400,   431,     0,
     443,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,   445,
     446,   460,   459,     0,     0,   475,   380,   440,   445,   467,
       0,     0,     0,    38,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   361,     0,   476,   329,   328,     0,    52,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,   150,     0,     0,   115,
       0,     0,   116,     0,     0,     0,     3,     0,   428,   430,
     107,   109,   424,   214,     0,     0,     0,   381,     0,   441,
     442,   379,   378,   377,   440,   471,     0,     0,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     325,   326,   327,   321,     0,   407,     0,     0,   322,     0,
       0,     0,     0,     0,    42,   412,     0,   411,   459,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   468,    29,     0,    31,     0,   471,
       0,     0,    41,   434,     0,     6,     0,    10,     0,    11,
       0,     0,     0,     0,     0,     0,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,    46,     0,     0,   128,     0,   140,     0,     0,   159,
       0,     0,     0,     3,   166,    49,     0,     0,     0,    51,
       0,     0,    55,     0,     0,     0,     0,   162,     0,     0,
       0,     0,     0,   143,     0,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   425,   427,   429,     0,
       0,   217,     0,     0,     0,   252,   472,     0,     0,     0,
       0,   464,   287,   440,   445,   446,   460,     0,     0,     0,
       0,     0,     0,   286,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,   336,   337,   340,   341,   334,   335,     0,   338,
     339,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     352,   354,   355,   357,   359,   360,   362,   364,   365,   367,
     368,   373,   374,   375,   370,   371,   372,   383,   387,   399,
     392,   393,   394,   395,   398,   396,   397,   390,   391,   388,
     389,   423,     0,   420,    62,     0,    96,    97,    98,    99,
     100,    59,     0,     0,   406,     0,    15,     0,     0,     0,
       0,   450,   455,   456,    64,     0,   415,     0,     0,     0,
     404,     0,   402,   434,     0,     0,     0,   323,   324,   325,
     326,   327,   321,   322,   469,     0,    30,    32,    40,     0,
     433,     0,     0,     0,   437,   459,     0,     0,     0,     0,
      26,     0,    27,     0,    28,     0,   169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   472,     0,     0,   131,   472,     0,     0,
     359,   360,   264,   267,     0,   280,   282,   284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,   401,
       0,     0,     0,     0,     0,   472,     0,   472,     0,     0,
       0,     0,   117,   118,   119,   120,   320,   426,   108,     0,
       0,   217,     0,     0,     0,     0,     0,     0,   473,     0,
       0,   386,   465,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,   314,     0,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,   288,    66,    68,
      70,    73,    71,    72,    67,    69,    79,    81,    83,    86,
      84,    85,    80,    82,     0,     0,   416,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,   410,     0,     0,   256,     0,     0,     0,
       0,   450,   455,   456,   470,    39,     5,     0,    12,     0,
       0,     0,     0,    19,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    90,    89,    93,
      91,    92,    88,    44,     0,    94,    95,    47,   125,   130,
     129,     0,     0,   139,   156,     0,   158,     0,     0,     0,
       0,   269,     0,     0,   165,     0,     0,     0,     0,     0,
     177,     0,    50,    53,    54,    57,    58,    34,    33,     0,
       0,   160,   457,   458,     0,     0,   141,   144,   145,   148,
     149,     0,     0,   215,   216,     0,     0,     0,     0,   213,
     174,     0,     0,     0,     0,     0,     0,     0,   251,   474,
       0,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,   304,   311,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   332,   422,
       0,   419,     0,     0,    61,     0,    63,   175,     0,    16,
     453,   452,   451,     0,     0,     0,   413,   408,   409,     0,
       0,     0,   405,     0,     0,   261,   260,     0,     0,     0,
      21,     0,    22,     0,    25,   168,     0,    36,    35,     0,
     474,   474,     0,   132,     0,   133,     0,     0,     0,   265,
     276,     0,   270,   271,   164,   272,   268,     0,   281,   283,
     285,     0,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,    77,    78,     0,     0,
       0,   212,     0,     0,     0,     0,     0,     0,     0,     0,
     173,   171,     0,     0,     0,     0,     0,     9,     0,     0,
     450,   456,   308,     0,     0,     0,     0,     0,     0,   316,
     317,   318,   319,   289,   291,   293,   296,   294,   295,   290,
     292,   421,   418,   417,    60,   474,    74,    75,    76,   257,
     258,     0,     0,    13,    20,     0,     0,     0,     0,   167,
       7,     0,    45,    48,   127,   126,   472,     0,   137,   157,
     154,   155,   278,     0,     0,   273,     0,     0,     0,     0,
       0,   181,   182,   179,     0,     0,     0,     0,     0,     0,
       0,     0,   163,   161,   142,     0,     0,   153,     0,     3,
     247,     0,   244,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   218,     0,     0,     0,     0,   219,   220,     0,
     170,   172,   253,   254,   255,     0,   176,     0,     0,     0,
     298,   300,   303,   301,   302,   299,    17,     0,   474,   263,
     262,     0,    23,     0,   134,   135,   266,   277,     0,   274,
     275,     0,     0,     0,   183,   185,   187,   190,   188,   189,
     184,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   225,     0,     0,     0,     0,   224,   221,   222,
       0,   250,   307,   306,   175,     0,     0,   259,    14,   474,
       8,     0,   138,   279,   191,   193,   192,     0,   152,   246,
       0,   195,     0,     0,     0,   194,     0,   243,     0,   232,
     233,   227,   230,   231,   226,     0,     0,   236,     0,     0,
       0,   237,     0,     0,     0,   305,   309,   310,    24,   136,
       0,   245,     0,     0,   198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   196,   242,   240,   241,   229,   238,
     239,   228,   249,     0,     0,     0,     0,     0,   199,   200,
     197,     0,     0,     0,     0,     0,     0,     0,     0,   248,
     151,     0,     0,     0,   201,   203,   205,   208,   206,   207,
     202,   204,   209,   211,   210
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -877,  -877,     1,  -877,  -559,  -724,  -877,  -877,  -877,  -877,
    -877,   300,  1383,  -877,  -877,  -877,  -877,  -306,  -876,  -877,
    -877,  -877,   816,  -877,  -877,  -498,   900,  -386,   337,  1072,
      -2,  -234,  -877,   779,  1195,  -877,   731,   -16,  1184,   868,
     864,    -8,  -877,  1292,  -264,  -877,  -877,   263,  -877
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   211,    68,    69,   566,    70,    71,    72,    73,
      74,   996,    75,    76,   590,    77,    78,   354,  1000,    79,
      80,   390,   612,    81,   617,   291,   537,   579,   125,   184,
     135,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   207,   481,    96,    97,   136,    98
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      83,     2,   483,   438,   863,   729,   614,   126,   688,   684,
     115,   153,   572,   573,   130,   690,   574,   546,    27,   739,
     154,   145,   147,   149,   151,   152,   609,  1002,   158,   160,
     162,   572,   573,   166,   167,   574,   610,   171,   173,   137,
     138,   139,    25,   140,   737,   188,   189,   197,   738,   192,
     339,   504,   339,   201,   202,   203,   177,   973,   922,   393,
     633,  1003,   141,   206,   209,   572,   573,   142,   508,   574,
     340,  1004,   340,   575,   576,   577,   178,   572,   573,   175,
     634,   574,   848,   253,    67,     3,   180,   253,   195,  1008,
     357,   849,   575,   576,   577,   759,   760,   347,   339,  1009,
     268,   269,   270,   271,   272,   273,   181,   196,   278,   280,
     358,   282,   283,   394,   740,   352,   602,   116,   340,   117,
     353,  1005,   206,   929,   735,    67,   575,   576,   577,   306,
     308,   116,   310,   117,   636,   689,   114,   578,   575,   576,
     577,   968,   328,  1091,   338,    67,   343,   348,   345,  1010,
      67,   611,   974,    67,   637,   341,   613,   341,   342,   711,
     342,   179,   803,   143,   982,   179,  1006,   165,   372,   822,
     374,   741,   635,   379,   313,   380,   381,    67,   382,   383,
    1007,   384,   386,   387,   384,   388,   168,   360,  1109,   603,
     710,   182,  1117,   761,  1011,   359,   930,   572,   573,   251,
     252,   574,   782,   341,   350,   351,   342,   361,  1012,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   169,   423,
     424,   425,   426,   427,   428,   429,   430,    67,   431,   783,
     784,   110,   100,   101,  1141,   454,   455,   742,   743,   182,
     253,   339,  1155,   572,   573,   253,   638,   574,   183,   111,
     461,   462,   463,   464,   465,   466,   467,  1162,   575,   576,
     577,   340,   109,   572,   573,   796,   701,   574,   330,   797,
     776,   777,   744,   285,   778,   495,  1078,   286,   798,   199,
    1083,   799,   800,   112,   975,   801,  1079,  1179,   357,   357,
    1084,   514,   362,   516,   517,   518,   519,   520,   521,   522,
     523,   113,   193,   964,   575,   576,   577,   780,   358,   358,
     194,   530,   531,   545,   533,   116,   538,   117,   353,   198,
    1102,   200,  1064,   287,   575,   576,   577,   551,   552,   553,
     554,   555,   556,   557,   570,   571,  1080,   560,   561,    82,
    1085,   776,   777,   314,   357,   778,   341,   230,   779,   342,
     976,   977,   250,  1169,   580,   582,   583,   315,   584,   585,
     312,   586,   587,   313,   358,   155,   157,   592,  1068,   594,
     336,   294,   276,   337,   598,   599,   492,   802,   174,   176,
     292,  1081,   267,   607,   302,  1086,   190,   191,  1133,   295,
     210,   493,   368,   359,   359,  1082,   264,   311,   370,  1087,
     805,   983,   293,   376,   855,   628,   629,   630,   632,   978,
     369,   639,   330,   979,   640,   641,   371,   355,   110,   901,
     356,   377,   332,   918,   642,   333,   334,   643,   644,   980,
     818,   645,   646,   785,   786,   787,   625,   885,   919,   874,
     886,   887,   572,   573,   112,   316,   574,   265,  1146,   359,
     888,  1147,  1148,   889,   890,   274,  1170,   891,   892,   317,
     155,  1149,   626,   318,  1150,  1151,   604,  1165,  1152,  1153,
     330,  1166,   605,   931,   266,   910,   911,   319,   329,   932,
     332,   330,   331,   333,   334,   539,   344,  1167,   346,   349,
     670,   332,   281,   788,   333,   334,   675,   676,   335,   540,
     656,   293,   657,   575,   576,   577,   686,   933,   373,   934,
     375,   530,   903,   541,   336,   212,   229,   337,   213,   214,
     284,   981,   355,   647,   543,   356,   658,   542,   215,   804,
     697,   216,   217,   935,   206,   218,   219,   706,   544,   708,
     659,   709,   229,   131,   132,   712,   713,   936,    82,   893,
     959,   904,   660,   229,   905,   906,   998,   907,   908,   937,
    1154,   999,   661,   732,   221,  1001,   525,   222,   223,   938,
     999,  1090,   336,   256,   257,   337,   999,   224,   751,  1168,
     225,   226,   662,   336,   227,   228,   337,   875,   876,   939,
     663,   877,   231,   232,   233,   234,   116,   940,   117,   772,
     558,   716,   235,   236,   237,   562,  1020,  1021,  1030,   323,
    1013,   569,  1065,  1014,   718,  1069,  1015,  1016,   790,   581,
     220,  1031,   326,   793,   794,   795,   238,   239,   363,   589,
     496,   806,   807,   497,   808,   809,   364,   810,   811,   812,
     813,   814,   815,   816,   817,  1019,   248,   249,   878,   879,
     880,    99,   366,   100,   101,   102,   103,   104,   105,   106,
     619,   254,   255,   623,   719,   720,   721,   698,   107,   229,
     699,  1032,  1033,  1034,   468,   722,   833,   834,   835,   396,
     397,   108,  1035,   109,   469,   839,   470,   471,   100,   101,
     296,   297,   298,   299,   300,   472,   473,   367,   845,   846,
     474,   656,   365,   301,  1134,   851,   475,   476,  1054,   468,
     398,   399,   859,   482,   477,   478,   479,   480,   109,   469,
     657,   470,   471,   389,   902,   658,   608,  1055,   391,   864,
     472,   473,  1056,   494,   313,   474,   659,   660,   870,   484,
     485,   475,   476,  1057,  1058,   881,   882,   884,   669,   477,
     478,   479,   480,   392,   661,   662,   672,   894,   663,   826,
     674,  1059,  1060,   677,   703,  1061,  1092,   704,   682,   899,
     685,   100,   101,   296,   297,   298,   299,   300,   716,   163,
    -475,  -475,   395,   164,   723,  1104,   301,   313,   240,   241,
     242,   243,   244,   245,   923,   924,   925,   926,   927,   928,
     246,   109,  -385,  -385,   718,   498,   499,   722,   946,   948,
     656,  1105,   247,   715,  1106,  1136,  1137,  1184,   402,  -475,
    -475,  -475,   724,  -475,  -475,  -475,  -475,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,  -475,   657,   658,   950,  -475,
     486,   324,   487,  1185,  1186,   325,   488,  -475,  -475,  -475,
    -475,  -475,  -475,  -475,  -475,   659,   660,  -475,  -475,  -475,
    -475,  -475,  1187,  1188,  -475,  -475,  -475,   661,  -475,   972,
     662,   663,   716,   718,  1189,   722,   489,  1190,  1191,  1192,
    1193,   468,  1194,   984,   985,   986,   987,   988,   989,   990,
     991,   469,   727,   470,   471,   313,   752,   490,   997,   313,
     728,   730,   472,   473,   733,   734,   736,   474,   491,   258,
     259,   260,   502,   475,   476,   829,  1025,    82,   313,   510,
     511,   477,   478,   479,   480,   261,   262,   263,   820,   303,
     823,   304,   766,   767,   768,    99,   828,   100,   101,   102,
     103,   104,   105,   106,   852,   953,   503,   853,   313,  1037,
     512,   513,   107,  1039,  1040,   960,  1042,  1100,   961,   313,
     313,   400,   844,   401,   500,   624,   501,   109,   320,   321,
     322,   504,  1047,   447,   448,   449,  1051,  1052,  1053,   451,
     452,   453,   500,   313,   691,   694,   313,   313,   791,   832,
     500,   505,   920,  1062,   529,   313,   549,   399,   396,   550,
    1067,   396,   559,   506,  1073,   563,   397,   564,   565,  1077,
     432,   433,   434,   435,   436,   437,   507,   439,   440,   441,
     442,   443,   444,   445,   446,   509,  1095,  1096,   567,   397,
     595,   397,   597,   397,   396,   673,   745,   746,   781,   746,
     789,   313,   825,   313,   827,   313,  1103,   836,   313,   837,
     511,   838,   313,   842,   511,   396,   843,   847,   704,   856,
     746,   860,   313,   861,   313,  1116,   862,   565,   515,   865,
     912,   746,   913,   746,   941,   313,   942,   511,   568,  1128,
     943,   313,   945,   313,  1132,   396,   957,   966,   397,   967,
     565,  1038,   313,   526,  1094,   313,   547,   895,  1099,   313,
    1101,   565,  1110,   746,   527,   997,   952,  1118,   746,   528,
    1142,  1143,  1145,   548,  1163,   746,   958,   588,   185,   187,
     459,   460,   457,   458,     4,     5,     6,     7,   591,   593,
     596,   127,   128,   120,   121,   288,   600,    13,    14,    15,
      16,    17,    18,    19,  1171,  1172,  1173,  1174,  1175,  1176,
    1177,  1178,   601,   602,   123,    29,   603,   604,   605,   620,
     615,    35,   606,  1181,  1182,  1183,   616,    37,   621,   627,
     664,   648,   649,   247,   650,    45,    46,    47,    48,   651,
     652,    51,  1027,   665,   653,   654,   655,   666,   656,   657,
      55,    56,   658,   678,   659,   660,    57,   661,   662,   965,
     663,   253,   679,   969,   970,   971,   680,   667,   692,   687,
     700,   695,   696,   705,   714,   716,   773,   693,   717,   774,
    1041,   718,   719,   720,  1043,    59,    60,   721,   313,   722,
     725,   992,   726,   753,   762,   994,   754,   353,   755,   756,
     757,    61,   758,   771,   763,   764,   769,    62,    63,   770,
     765,   792,   824,   826,   840,   124,   289,   841,    66,   866,
      67,   850,   290,     4,     5,     6,     7,   830,   831,   854,
     127,   128,   120,   121,   288,   857,    13,    14,    15,    16,
      17,    18,    19,   858,   867,   868,   869,   871,   872,   873,
     896,   898,   897,   123,    29,   900,   909,   914,   915,   916,
      35,   921,   917,  1044,  1045,   944,    37,   946,   947,   948,
     955,   954,   949,   951,    45,    46,    47,    48,   956,   962,
      51,   963,   993,   995,  1017,  1018,  1022,  1023,  1024,    55,
      56,  1028,  1029,  1036,  1048,    57,  1046,  1049,  1050,     4,
       5,     6,     7,  1063,  1066,  1070,   127,   128,   120,   121,
     288,  1071,    13,    14,    15,    16,    17,    18,    19,  1072,
    1074,  1075,  1076,  1088,    59,    60,  1089,  1097,  1093,   123,
      29,  1098,  1107,  1119,  1125,   999,    35,  1120,  1121,  1122,
      61,  1111,    37,  1123,  1124,  1126,    62,    63,  1127,  1129,
      45,    46,    47,    48,   124,   683,    51,    66,  1130,    67,
    1108,   290,  1131,  1135,    82,    55,    56,  1140,  1164,  1138,
    1156,    57,   156,  1157,  1158,     4,     5,     6,     7,  1159,
    1160,  1161,   127,   128,   120,   121,   288,   775,    13,    14,
      15,    16,    17,    18,    19,   327,   702,   456,  1139,     0,
      59,    60,   450,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    61,     0,    37,     0,
       0,     0,    62,    63,     0,     0,    45,    46,    47,    48,
     124,   821,    51,    66,     0,    67,     0,   290,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,    62,    63,
       0,     0,     0,     0,     0,     0,   124,     0,     0,    66,
       0,    67,     0,   290,     4,     5,     6,     7,     0,     0,
       0,   403,   404,   405,   406,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   407,   408,    25,
       0,     0,    26,    27,    28,    29,   409,   410,    32,    33,
      34,    35,    36,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,   411,   412,    45,    46,    47,    48,    49,
      50,    51,    52,     0,     0,     0,    53,    54,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,    62,    63,     0,
       0,     0,     0,     0,    64,    65,     0,     0,    66,     0,
      67,   413,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,     0,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   747,   748,    25,     0,     0,
      26,    27,    28,    29,    30,   749,    32,    33,    34,    35,
      36,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,     0,     0,    53,    54,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,    62,    63,     0,     0,     0,
       0,     0,    64,    65,     0,     0,    66,     0,    67,   750,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,  1112,  1113,    25,     0,     0,    26,    27,
      28,    29,    30,  1114,    32,    33,    34,    35,    36,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,     0,
       0,     0,    53,    54,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,    59,    60,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,    61,     0,     0,
       0,     0,     0,    62,    63,     0,   123,    29,     0,     0,
      64,    65,     0,    35,    66,     0,    67,  1115,     0,    37,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,   159,   124,    51,     0,
      66,     0,    67,   631,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,   159,   124,    51,     0,    66,     0,    67,   883,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,    62,
      63,     0,     0,     0,     0,     0,   159,   124,     0,     0,
      66,     0,    67,  1144,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,    28,    29,    30,    31,    32,    33,
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
      67,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    48,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   124,    51,     0,    66,   129,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,   133,   134,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
     159,   124,    51,     0,    66,     0,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   204,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    59,    60,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    61,    37,     0,     0,     0,     0,    62,    63,
       0,    45,    46,    47,    48,   161,   124,    51,     0,    66,
       0,    67,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    61,     0,    37,
       0,     0,     0,    62,    63,     0,     0,    45,    46,    47,
      48,   124,   205,    51,    66,     0,    67,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,     0,    59,    60,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    61,    37,     0,     0,     0,     0,    62,
      63,     0,    45,    46,    47,    48,     0,   124,    51,     0,
      66,   208,    67,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    59,    60,     0,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,    61,     0,
      37,     0,     0,     0,    62,    63,     0,     0,    45,    46,
      47,    48,   124,   275,    51,    66,     0,    67,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,   279,    45,    46,    47,    48,     0,   124,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    61,
       0,    37,     0,     0,     0,    62,    63,     0,     0,    45,
      46,    47,    48,   124,   205,    51,    66,     0,    67,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     127,   128,   120,   121,   122,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,     0,    59,
      60,     0,     0,   123,    29,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,    61,    37,     0,     0,     0,
       0,    62,    63,     0,    45,    46,    47,    48,   305,   124,
      51,     0,    66,     0,    67,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,    57,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      61,    37,     0,     0,     0,     0,    62,    63,     0,    45,
      46,    47,    48,   307,   124,    51,     0,    66,     0,    67,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,   378,     0,     0,    59,
      60,     0,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   124,
     309,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   127,   128,   120,   121,
     122,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,     0,    59,    60,     0,     0,   123,
      29,     0,     0,     0,     0,     0,    35,     0,     0,     0,
       0,    61,    37,     0,     0,     0,     0,    62,    63,     0,
      45,    46,    47,    48,     0,   124,    51,     0,    66,     0,
      67,     0,     0,     0,     0,    55,    56,     0,     0,     0,
       0,    57,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   127,   128,   120,   121,   122,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,     0,
      59,    60,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,     0,    61,    37,     0,     0,
       0,     0,    62,    63,     0,    45,    46,    47,    48,     0,
     124,    51,   385,    66,     0,    67,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,   210,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   124,     0,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   127,   128,   120,   121,   122,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
       0,    59,    60,     0,     0,   123,    29,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,    61,    37,     0,
       0,     0,     0,    62,    63,     0,    45,    46,    47,    48,
       0,   124,    51,     0,    66,   524,    67,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,    57,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   127,   128,
     120,   121,   122,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    59,    60,     0,     0,
       0,   123,    29,     0,     0,     0,     0,     0,    35,     0,
       0,     0,    61,     0,    37,     0,     0,     0,    62,    63,
       0,     0,    45,    46,    47,    48,   124,   618,    51,    66,
       0,    67,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,    57,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,     0,    59,    60,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    61,    37,
       0,     0,     0,     0,    62,    63,     0,    45,    46,    47,
      48,     0,   124,    51,     0,    66,   622,    67,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    61,     0,    37,     0,     0,     0,    62,
      63,     0,     0,    45,    46,    47,    48,   124,   668,    51,
      66,     0,    67,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   127,   128,   120,   121,   122,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     0,   123,    29,
       0,     0,     0,     0,     0,    35,     0,     0,     0,    61,
       0,    37,     0,     0,     0,    62,    63,     0,     0,    45,
      46,    47,    48,   124,   671,    51,    66,     0,    67,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
      57,     0,     0,     0,     4,     5,   534,     7,     0,     0,
       0,   127,   128,   120,   121,   535,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    59,
      60,     0,     0,     0,   123,    29,     0,     0,     0,     0,
       0,    35,     0,     0,     0,    61,     0,    37,     0,     0,
       0,    62,    63,     0,     0,    45,    46,    47,    48,   124,
     681,    51,    66,     0,    67,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,    57,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   127,   128,   120,
     121,   122,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    59,    60,     0,     0,     0,
     123,    29,     0,     0,     0,     0,     0,    35,     0,     0,
       0,    61,     0,    37,     0,     0,     0,    62,    63,     0,
       0,    45,    46,    47,    48,   124,   205,    51,    66,     0,
      67,     0,     0,     0,     0,     0,    55,    56,     0,     0,
       0,     0,    57,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   127,   128,   120,   121,   122,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    59,    60,     0,     0,     0,   123,    29,     0,     0,
       0,     0,     0,    35,     0,     0,     0,    61,     0,    37,
       0,     0,     0,    62,    63,     0,     0,    45,    46,    47,
      48,   124,   819,    51,    66,     0,    67,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,    57,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    59,    60,     0,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,    61,     0,    37,     0,     0,     0,    62,
      63,     0,     0,    45,    46,    47,    48,   124,  1026,    51,
      66,     0,    67,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   144,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   146,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   148,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   150,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   124,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   170,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   172,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   124,    51,
       0,    66,     0,   186,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   277,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,   534,     7,     0,     0,     0,   127,
     128,   120,   121,   535,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   532,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   127,   128,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,    29,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   536,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   127,
     128,   120,   121,   122,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,   123,    29,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,    61,    37,     0,     0,     0,     0,
      62,    63,     0,    45,    46,    47,    48,     0,   707,    51,
       0,    66,     0,    67,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,    57,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   120,   121,   122,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,   123,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,    61,
      37,     0,     0,     0,     0,    62,    63,     0,    45,    46,
      47,    48,     0,   731,    51,     0,    66,     0,    67,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
      62,    63,     0,     0,     0,     0,     0,     0,   124,     0,
       0,    66,     0,    67
};

static const yytype_int16 yycheck[] =
{
       2,     0,   266,   237,   728,   564,   392,    15,    14,   507,
      11,    27,    10,    11,    16,   513,    14,   323,    32,    24,
      14,    23,    24,    25,    26,    27,    14,   903,    30,    31,
      32,    10,    11,    35,    36,    14,    24,    39,    40,    10,
      11,    12,    28,    14,    86,    47,    48,    55,    86,    51,
      28,   133,    28,    61,    62,    63,    28,    24,   140,    14,
      28,    14,    33,    65,    66,    10,    11,    38,    83,    14,
      48,    24,    48,    71,    72,    73,    48,    10,    11,   134,
      48,    14,     5,   125,   139,     0,    28,   125,    14,    14,
      28,    14,    71,    72,    73,    42,    43,    86,    28,    24,
     102,   103,   104,   105,   106,   107,    48,    33,   110,   111,
      48,   113,   114,    68,   119,   134,   133,   132,    48,   134,
     139,    74,   124,   140,    86,   139,    71,    72,    73,   131,
     132,   132,   134,   134,    28,   141,   119,   135,    71,    72,
      73,   865,   144,  1019,   146,   139,   148,   136,   150,    74,
     139,   139,   119,   139,    48,   133,   135,   133,   136,   545,
     136,   133,   140,   134,   140,   133,   119,    14,   170,   667,
     172,    10,   140,   175,   136,   177,   178,   139,   180,   181,
     133,   183,   184,   185,   186,   187,    12,    28,  1064,   133,
     135,   133,  1068,   140,   119,   133,   140,    10,    11,    84,
      85,    14,   135,   133,    84,    85,   136,    48,   133,   211,
     212,   213,   214,   215,   216,   217,   218,   219,    12,   221,
     222,   223,   224,   225,   226,   227,   228,   139,   230,   615,
     616,   119,   104,   105,  1110,   251,   252,    76,    77,   133,
     125,    28,  1118,    10,    11,   125,   140,    14,   139,   137,
     258,   259,   260,   261,   262,   263,   264,  1133,    71,    72,
      73,    48,   134,    10,    11,    28,    83,    14,    31,    32,
      62,    63,   578,    14,    66,   277,    14,    18,    41,    10,
      14,    44,    45,   121,    10,    48,    24,  1163,    28,    28,
      24,   293,   133,   295,   296,   297,   298,   299,   300,   301,
     302,   139,    14,   862,    71,    72,    73,   613,    48,    48,
      14,   313,   314,   134,   316,   132,   318,   134,   139,    14,
    1044,    10,   135,    64,    71,    72,    73,   329,   330,   331,
     332,   333,   334,   335,   350,   351,    74,   339,   340,     2,
      74,    62,    63,   119,    28,    66,   133,    83,   140,   136,
      76,    77,   111,   140,   353,   357,   358,   133,   360,   361,
     133,   363,   364,   136,    48,    28,    29,   369,   135,   371,
     133,   121,   109,   136,   376,   377,   119,   140,    41,    42,
     117,   119,    14,   385,   119,   119,    49,    50,   135,   139,
      78,   134,   121,   133,   133,   133,   118,   134,   121,   133,
     140,   140,   137,    31,   710,   407,   408,   409,   410,    28,
     139,    28,    31,    32,    31,    32,   139,   133,   119,   140,
     136,    49,    41,   119,    41,    44,    45,    44,    45,    48,
     664,    48,    49,    80,    81,    82,   137,    28,   134,   745,
      31,    32,    10,    11,   121,   119,    14,   132,    28,   133,
      41,    31,    32,    44,    45,    14,   140,    48,    49,   133,
     123,    41,   139,   119,    44,    45,   133,    28,    48,    49,
      31,    32,   133,   140,   132,   781,   782,   133,    28,   140,
      41,    31,    32,    44,    45,   119,   149,    48,   151,   152,
     492,    41,    14,   140,    44,    45,   498,   499,    48,   133,
     133,   137,   133,    71,    72,    73,   508,   140,   171,   140,
     173,   513,   776,   119,   133,    28,   133,   136,    31,    32,
     133,   140,   133,   140,   119,   136,   133,   133,    41,   140,
     532,    44,    45,   140,   536,    48,    49,   539,   133,   541,
     133,   543,   133,    14,    15,   547,   548,   140,   211,   140,
     856,    10,   133,   133,    13,    14,   134,    16,    17,   140,
     140,   139,   133,   565,    28,   134,   303,    31,    32,   140,
     139,   134,   133,   126,   127,   136,   139,    41,   580,   140,
      44,    45,   133,   133,    48,    49,   136,    10,    11,   140,
     133,    14,    87,    88,    89,    90,   132,   140,   134,   601,
     337,   133,    97,    98,    99,   342,   912,   913,   140,    14,
      10,   348,   998,    13,   133,  1001,    16,    17,   620,   356,
     133,   140,    10,   625,   626,   627,   121,   122,   119,   366,
     133,   633,   634,   136,   636,   637,   119,   639,   640,   641,
     642,   643,   644,   645,   646,   909,   123,   124,    71,    72,
      73,   102,   136,   104,   105,   106,   107,   108,   109,   110,
     397,   116,   117,   400,   133,   133,   133,   133,   119,   133,
     136,   140,   140,   140,    14,   133,   678,   679,   680,   135,
     136,   132,   140,   134,    24,   687,    26,    27,   104,   105,
     106,   107,   108,   109,   110,    35,    36,   139,   700,   701,
      40,   133,    75,   119,  1090,   707,    46,    47,   140,    14,
     135,   136,   714,    18,    54,    55,    56,    57,   134,    24,
     133,    26,    27,    59,    64,   133,   389,   140,   139,   731,
      35,    36,   140,   135,   136,    40,   133,   133,   740,   133,
     134,    46,    47,   140,   140,   747,   748,   749,   485,    54,
      55,    56,    57,   134,   133,   133,   493,   759,   133,   133,
     497,   140,   140,   500,   133,   140,   140,   136,   505,   771,
     507,   104,   105,   106,   107,   108,   109,   110,   133,    10,
      44,    45,   139,    14,   133,   140,   119,   136,    91,    92,
      93,    94,    95,    96,   796,   797,   798,   799,   800,   801,
     103,   134,   135,   136,   133,   126,   127,   133,   133,   133,
     133,   140,   115,   550,   140,   140,   140,   140,   140,    83,
      84,    85,   559,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   133,   133,   840,   103,
     133,    10,   133,   140,   140,    14,   133,   111,   112,   113,
     114,   115,   116,   117,   118,   133,   133,   121,   122,   123,
     124,   125,   140,   140,   128,   129,   130,   133,   132,   871,
     133,   133,   133,   133,   140,   133,   133,   140,   140,   140,
     140,    14,   140,   885,   886,   887,   888,   889,   890,   891,
     892,    24,   133,    26,    27,   136,   133,   133,   900,   136,
     563,   564,    35,    36,   567,   568,   569,    40,   133,   112,
     113,   114,   122,    46,    47,   133,   918,   580,   136,   135,
     136,    54,    55,    56,    57,   128,   129,   130,   665,   136,
     667,   138,   595,   596,   597,   102,   673,   104,   105,   106,
     107,   108,   109,   110,   133,   133,   140,   136,   136,   951,
     135,   136,   119,   955,   956,   133,   133,   133,   136,   136,
     136,   136,   699,   138,   136,   132,   138,   134,    10,    11,
      12,   133,   974,     7,     8,     9,   978,   979,   980,   248,
     249,   250,   136,   136,   138,   138,   136,   136,   138,   138,
     136,   134,   138,   995,   135,   136,   135,   136,   135,   136,
     999,   135,   136,    14,  1006,   135,   136,    29,    30,  1011,
     231,   232,   233,   234,   235,   236,   134,   238,   239,   240,
     241,   242,   243,   244,   245,    14,  1028,  1029,   135,   136,
     135,   136,   135,   136,   135,   136,   135,   136,   135,   136,
     135,   136,   135,   136,   135,   136,  1048,   135,   136,   135,
     136,   135,   136,   135,   136,   135,   136,   135,   136,   135,
     136,   135,   136,   135,   136,  1067,    29,    30,    14,   732,
     135,   136,   135,   136,   135,   136,   135,   136,    10,  1081,
     135,   136,   135,   136,  1086,   135,   136,   135,   136,    29,
      30,   135,   136,   133,   135,   136,   119,   760,   135,   136,
      29,    30,   135,   136,   133,  1107,   843,   135,   136,   133,
    1112,  1113,  1114,   119,   135,   136,   853,   133,    46,    47,
     256,   257,   254,   255,     3,     4,     5,     6,    14,    14,
      29,    10,    11,    12,    13,    14,    10,    16,    17,    18,
      19,    20,    21,    22,  1146,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,   133,   133,    33,    34,   133,   133,   133,   119,
     134,    40,   140,  1165,  1166,  1167,   134,    46,    10,   119,
     100,   133,   133,   115,   133,    54,    55,    56,    57,   133,
     133,    60,   919,   134,   133,   133,   133,    14,   133,   133,
      69,    70,   133,   119,   133,   133,    75,   133,   133,   862,
     133,   125,   119,   866,   867,   868,   119,   134,   122,   134,
      83,   133,   133,   133,   119,   133,    14,   140,   133,    14,
     957,   133,   133,   133,   961,   104,   105,   133,   136,   133,
     133,   894,   133,   133,   122,   898,   133,   139,   133,   133,
     133,   120,   133,   119,   140,   122,   133,   126,   127,   133,
     140,    14,   133,   133,    83,   134,   135,    14,   137,    10,
     139,   133,   141,     3,     4,     5,     6,   138,   138,   133,
      10,    11,    12,    13,    14,   133,    16,    17,    18,    19,
      20,    21,    22,   133,    10,    10,    14,   119,    10,    10,
     133,    29,   133,    33,    34,   133,    62,    14,    14,    14,
      40,   140,   133,   966,   967,   133,    46,   133,   133,   133,
      83,   133,   135,   134,    54,    55,    56,    57,    83,   133,
      60,   133,   140,   133,   133,   133,     7,     7,     7,    69,
      70,   119,   119,   133,   119,    75,    14,    10,    10,     3,
       4,     5,     6,   135,    78,    14,    10,    11,    12,    13,
      14,    14,    16,    17,    18,    19,    20,    21,    22,   133,
      14,    14,   133,   133,   104,   105,   133,   135,   140,    33,
      34,   133,   133,   133,    14,   139,    40,   133,   133,   133,
     120,   140,    46,   133,   133,    14,   126,   127,   133,    14,
      54,    55,    56,    57,   134,   135,    60,   137,    14,   139,
    1063,   141,   133,   140,  1067,    69,    70,  1107,   135,   133,
     133,    75,    29,   133,   133,     3,     4,     5,     6,   133,
     133,   133,    10,    11,    12,    13,    14,   611,    16,    17,
      18,    19,    20,    21,    22,   143,   536,   253,  1101,    -1,
     104,   105,   247,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,   120,    -1,    46,    -1,
      -1,    -1,   126,   127,    -1,    -1,    54,    55,    56,    57,
     134,   135,    60,   137,    -1,   139,    -1,   141,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,   137,
      -1,   139,    -1,   141,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,    -1,   137,    -1,
     139,   140,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,   126,   127,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,    -1,   137,    -1,   139,   140,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,   104,   105,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,   120,    -1,    -1,
      -1,    -1,    -1,   126,   127,    -1,    33,    34,    -1,    -1,
     133,   134,    -1,    40,   137,    -1,   139,   140,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,
     127,    -1,    54,    55,    56,    57,   133,   134,    60,    -1,
     137,    -1,   139,   140,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,   133,   134,    60,    -1,   137,    -1,   139,   140,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     139,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
      -1,   134,    60,    -1,   137,   138,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,   133,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,
      -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,    57,
     133,   134,    60,    -1,   137,    -1,   139,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,   120,    46,    -1,    -1,    -1,    -1,   126,   127,
      -1,    54,    55,    56,    57,   133,   134,    60,    -1,   137,
      -1,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
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
     137,   138,   139,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
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
     126,   127,   128,    54,    55,    56,    57,    -1,   134,    60,
      -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,   104,   105,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   120,
      -1,    46,    -1,    -1,    -1,   126,   127,    -1,    -1,    54,
      55,    56,    57,   134,   135,    60,   137,    -1,   139,    -1,
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
      75,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    -1,    -1,   104,
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
      54,    55,    56,    57,    -1,   134,    60,    -1,   137,    -1,
     139,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,   120,    46,    -1,    -1,
      -1,    -1,   126,   127,    -1,    54,    55,    56,    57,    -1,
     134,    60,   136,   137,    -1,   139,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    78,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   120,    -1,    46,    -1,    -1,    -1,   126,   127,    -1,
      -1,    54,    55,    56,    57,   134,    -1,    60,   137,    -1,
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
      -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,    56,
      57,    -1,   134,    60,    -1,   137,   138,   139,    -1,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
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
      -1,    -1,    -1,   104,   105,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,   120,
      46,    -1,    -1,    -1,    -1,   126,   127,    -1,    54,    55,
      56,    57,    -1,   134,    60,    -1,   137,    -1,   139,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,   137,    -1,   139
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
     148,   149,   150,   151,   152,   154,   155,   157,   158,   161,
     162,   165,   170,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   187,   188,   190,   102,
     104,   105,   106,   107,   108,   109,   110,   119,   132,   134,
     119,   137,   121,   139,   119,    11,   132,   134,    10,    11,
      12,    13,    14,    33,   134,   170,   183,    10,    11,   138,
     172,    14,    15,   133,   134,   172,   189,    10,    11,    12,
      14,    33,    38,   134,   134,   172,   134,   172,   134,   172,
     134,   172,   172,   179,    14,   170,   154,   170,   172,   133,
     172,   133,   172,    10,    14,    14,   172,   172,    12,    12,
     134,   172,   134,   172,   170,   134,   170,    28,    48,   133,
      28,    48,   133,   139,   171,   171,   139,   171,   172,   172,
     170,   170,   172,    14,    14,    14,    33,   183,    14,    10,
      10,   183,   183,   183,    10,   135,   172,   185,   138,   172,
      78,   144,    28,    31,    32,    41,    44,    45,    48,    49,
     133,    28,    31,    32,    41,    44,    45,    48,    49,   133,
      83,    87,    88,    89,    90,    97,    98,    99,   121,   122,
      91,    92,    93,    94,    95,    96,   103,   115,   123,   124,
     111,    84,    85,   125,   116,   117,   126,   127,   112,   113,
     114,   128,   129,   130,   118,   132,   132,    14,   172,   172,
     172,   172,   172,   172,    14,   135,   189,   134,   172,   128,
     172,    14,   172,   172,   133,    14,    18,    64,    14,   135,
     141,   167,   189,   137,   121,   139,   106,   107,   108,   109,
     110,   119,   119,   136,   138,   133,   172,   133,   172,   135,
     172,   189,   133,   136,   119,   133,   119,   133,   119,   133,
      10,    11,    12,    14,    10,    14,    10,   185,   172,    28,
      31,    32,    41,    44,    45,    48,   133,   136,   172,    28,
      48,   133,   136,   172,   170,   172,   170,    86,   136,   170,
      84,    85,   134,   139,   159,   133,   136,    28,    48,   133,
      28,    48,   133,   119,   119,    75,   136,   139,   121,   139,
     121,   139,   172,   170,   172,   170,    31,    49,    25,   172,
     172,   172,   172,   172,   172,   136,   172,   172,   172,    59,
     163,   139,   134,    14,    68,   139,   135,   136,   135,   136,
     136,   138,   140,    10,    11,    12,    13,    26,    27,    35,
      36,    52,    53,   140,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   175,   175,   175,   175,   175,   175,   173,   175,
     175,   175,   175,   175,   175,   175,   175,     7,     8,     9,
     176,   178,   178,   178,   179,   179,   180,   181,   181,   182,
     182,   183,   183,   183,   183,   183,   183,   183,    14,    24,
      26,    27,    35,    36,    40,    46,    47,    54,    55,    56,
      57,   186,    18,   186,   133,   134,   133,   133,   133,   133,
     133,   133,   119,   134,   135,   172,   133,   136,   126,   127,
     136,   138,   122,   140,   133,   134,    14,   134,    83,    14,
     135,   136,   135,   136,   172,    14,   172,   172,   172,   172,
     172,   172,   172,   172,   138,   189,   133,   133,   133,   135,
     172,   172,   134,   172,     5,    14,   134,   168,   172,   119,
     133,   119,   133,   119,   133,   134,   159,   119,   119,   135,
     136,   172,   172,   172,   172,   172,   172,   172,   189,   136,
     172,   172,   189,   135,    29,    30,   147,   135,    10,   189,
     179,   179,    10,    11,    14,    71,    72,    73,   135,   169,
     144,   189,   172,   172,   172,   172,   172,   172,   133,   189,
     156,    14,   172,    14,   172,   135,    29,   135,   172,   172,
      10,   133,   133,   133,   133,   133,   140,   172,   170,    14,
      24,   139,   164,   135,   169,   134,   134,   166,   135,   189,
     119,    10,   138,   189,   132,   137,   139,   119,   172,   172,
     172,   140,   172,    28,    48,   140,    28,    48,   140,    28,
      31,    32,    41,    44,    45,    48,    49,   140,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   100,   134,    14,   134,   135,   189,
     172,   135,   189,   136,   189,   172,   172,   189,   119,   119,
     119,   135,   189,   135,   167,   189,   172,   134,    14,   141,
     167,   138,   122,   140,   138,   133,   133,   172,   133,   136,
      83,    83,   168,   133,   136,   133,   172,   134,   172,   172,
     135,   169,   172,   172,   119,   189,   133,   133,   133,   133,
     133,   133,   133,   133,   189,   133,   133,   133,   170,   146,
     170,   134,   172,   170,   170,    86,   170,    86,    86,    24,
     119,    10,    76,    77,   159,   135,   136,    26,    27,    36,
     140,   172,   133,   133,   133,   133,   133,   133,   133,    42,
      43,   140,   122,   140,   122,   140,   170,   170,   170,   133,
     133,   119,   172,    14,    14,   164,    62,    63,    66,   140,
     159,   135,   135,   169,   169,    80,    81,    82,   140,   135,
     172,   138,    14,   172,   172,   172,    28,    32,    41,    44,
      45,    48,   140,   140,   140,   140,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   173,   135,
     189,   135,   167,   189,   133,   135,   133,   135,   189,   133,
     138,   138,   138,   172,   172,   172,   135,   135,   135,   172,
      83,    14,   135,   136,   189,   172,   172,   135,     5,    14,
     133,   172,   133,   136,   133,   159,   135,   133,   133,   172,
     135,   135,    29,   147,   172,   170,    10,    10,    10,    14,
     172,   119,    10,    10,   159,    10,    11,    14,    71,    72,
      73,   172,   172,   140,   172,    28,    31,    32,    41,    44,
      45,    48,    49,   140,   172,   170,   133,   133,    29,   172,
     133,   140,    64,   186,    10,    13,    14,    16,    17,    62,
     159,   159,   135,   135,    14,    14,    14,   133,   119,   134,
     138,   140,   140,   172,   172,   172,   172,   172,   172,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   135,   135,   135,   133,   135,   133,   133,   133,   135,
     172,   134,   189,   133,   133,    83,    83,   136,   189,   159,
     133,   136,   133,   133,   146,   170,   135,    29,   147,   170,
     170,   170,   172,    24,   119,    10,    76,    77,    28,    32,
      48,   140,   140,   140,   172,   172,   172,   172,   172,   172,
     172,   172,   170,   140,   170,   133,   153,   172,   134,   139,
     160,   134,   160,    14,    24,    74,   119,   133,    14,    24,
      74,   119,   133,    10,    13,    16,    17,   133,   133,   186,
     159,   159,     7,     7,     7,   172,   135,   189,   119,   119,
     140,   140,   140,   140,   140,   140,   133,   172,   135,   172,
     172,   189,   133,   189,   170,   170,    14,   172,   119,    10,
      10,   172,   172,   172,   140,   140,   140,   140,   140,   140,
     140,   140,   172,   135,   135,   169,    78,   144,   135,   169,
      14,    14,   133,   172,    14,    14,   133,   172,    14,    24,
      74,   119,   133,    14,    24,    74,   119,   133,   133,   133,
     134,   160,   140,   140,   135,   172,   172,   135,   133,   135,
     133,    29,   147,   172,   140,   140,   140,   133,   170,   160,
     135,   140,    26,    27,    36,   140,   172,   160,   135,   133,
     133,   133,   133,   133,   133,    14,    14,   133,   172,    14,
      14,   133,   172,   135,   169,   140,   140,   140,   133,   170,
     153,   160,   172,   172,   140,   172,    28,    31,    32,    41,
      44,    45,    48,    49,   140,   160,   133,   133,   133,   133,
     133,   133,   160,   135,   135,    28,    32,    48,   140,   140,
     140,   172,   172,   172,   172,   172,   172,   172,   172,   160,
     170,   172,   172,   172,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140
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
     160,   160,   161,   162,   163,   163,   163,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   165,   166,   166,   166,   166,   167,   167,   167,   167,
     168,   168,   168,   168,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     171,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   173,   173,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   175,   175,   176,   176,   176,   177,   177,   178,
     178,   178,   179,   179,   180,   180,   180,   181,   181,   181,
     182,   182,   182,   182,   182,   182,   182,   183,   183,   183,
     183,   183,   183,   184,   184,   185,   185,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     187,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   189,   189,   189,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190
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
       7,     7,     6,     5,     0,     3,     3,     0,     4,     4,
       4,     5,     5,     4,     5,     5,     6,     6,     7,     7,
       6,     6,     6,     6,     5,     5,     6,     6,     7,     7,
       7,     7,     7,     6,     4,     7,     6,     4,     8,     7,
       5,     5,     0,     4,     4,     4,     3,     5,     5,     7,
       3,     3,     5,     5,     1,     3,     5,     1,     3,     2,
       3,     3,     3,     4,     5,     5,     3,     5,     4,     6,
       1,     3,     1,     3,     1,     3,     3,     3,     4,     6,
       6,     6,     6,     6,     6,     6,     6,     5,     7,     7,
       7,     7,     7,     7,     5,     9,     8,     8,     6,     9,
       9,     5,     5,     4,     4,     4,     6,     6,     6,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     1,     5,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     2,
       2,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     3,     4,     6,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     2,     3,     4,     3,     2,     3,
       2,     1,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       4,     6,     6,     6,     1,     4,     4,     5,     5,     1,
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
#line 509 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3255 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 519 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3261 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 520 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3267 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 524 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3273 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 526 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3279 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 528 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3285 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 530 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3293 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 534 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3299 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 536 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3305 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 538 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3311 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 540 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3317 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 542 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3325 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 546 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3333 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 550 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3339 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 552 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3347 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 556 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3355 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 560 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3361 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 562 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3367 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 564 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3373 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 566 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3379 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 568 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3385 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 570 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3393 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 574 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3401 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3407 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 580 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3413 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 582 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3419 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 584 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3425 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 586 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3431 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 588 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3437 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE QIDENT ';'  */
#line 590 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3443 "raku.tab.c"
    break;

  case 32: /* stmt: KW_USE QIDENT expr ';'  */
#line 592 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); free((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 3449 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 594 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3455 "raku.tab.c"
    break;

  case 34: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 596 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3461 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 598 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3467 "raku.tab.c"
    break;

  case 36: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 600 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3473 "raku.tab.c"
    break;

  case 37: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 602 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3485 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP ';'  */
#line 610 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3491 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 612 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3497 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP '(' ')' ';'  */
#line 614 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3503 "raku.tab.c"
    break;

  case 41: /* stmt: TESTOP arg_list ';'  */
#line 616 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3509 "raku.tab.c"
    break;

  case 42: /* stmt: IDENT VAR_ARRAY ';'  */
#line 618 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3515 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ';'  */
#line 620 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3521 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 622 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3528 "raku.tab.c"
    break;

  case 45: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 625 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3535 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ';'  */
#line 628 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3541 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 630 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3548 "raku.tab.c"
    break;

  case 48: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 633 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3555 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TAKE expr ';'  */
#line 636 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3561 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 638 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3569 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr ';'  */
#line 642 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3575 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN ';'  */
#line 644 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3581 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 646 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3588 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 649 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3595 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 652 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3601 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 654 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3608 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 657 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3615 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 660 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3622 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 663 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3628 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 665 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3637 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 670 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3645 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 674 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3653 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 678 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3662 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 683 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3670 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall ';'  */
#line 686 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3676 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3682 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3688 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 692 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3694 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 694 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3700 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 696 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3707 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 699 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3713 "raku.tab.c"
    break;

  case 72: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 701 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3719 "raku.tab.c"
    break;

  case 73: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 703 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3725 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 705 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3732 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 708 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3739 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 711 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3746 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 714 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3753 "raku.tab.c"
    break;

  case 78: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 717 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3760 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_IF expr ';'  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3766 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNLESS expr ';'  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3772 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WHILE expr ';'  */
#line 724 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3778 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNTIL expr ';'  */
#line 726 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3784 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_FOR expr ';'  */
#line 728 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3791 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITH expr ';'  */
#line 731 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3797 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_WITHOUT expr ';'  */
#line 733 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3803 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_GIVEN expr ';'  */
#line 735 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3809 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 737 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3816 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 740 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3823 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 743 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3831 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 747 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3838 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 750 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3844 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 752 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3850 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 754 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3856 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 756 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3863 "raku.tab.c"
    break;

  case 95: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 759 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3870 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 762 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3877 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 765 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3884 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 768 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3891 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 771 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3898 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 774 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3905 "raku.tab.c"
    break;

  case 101: /* stmt: expr ';'  */
#line 776 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3911 "raku.tab.c"
    break;

  case 102: /* stmt: ';'  */
#line 777 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3917 "raku.tab.c"
    break;

  case 103: /* stmt: if_stmt  */
#line 778 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3923 "raku.tab.c"
    break;

  case 104: /* stmt: while_stmt  */
#line 779 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3929 "raku.tab.c"
    break;

  case 105: /* stmt: for_stmt  */
#line 780 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3935 "raku.tab.c"
    break;

  case 106: /* stmt: given_stmt  */
#line 781 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3941 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block  */
#line 783 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3947 "raku.tab.c"
    break;

  case 108: /* stmt: KW_TRY block KW_CATCH block  */
#line 785 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3953 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH block  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3959 "raku.tab.c"
    break;

  case 110: /* stmt: block  */
#line 789 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3965 "raku.tab.c"
    break;

  case 111: /* stmt: unless_stmt  */
#line 790 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3971 "raku.tab.c"
    break;

  case 112: /* stmt: until_stmt  */
#line 791 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3977 "raku.tab.c"
    break;

  case 113: /* stmt: repeat_stmt  */
#line 792 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3983 "raku.tab.c"
    break;

  case 114: /* stmt: loop_stmt  */
#line 793 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3989 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST ';'  */
#line 794 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3995 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT ';'  */
#line 795 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4001 "raku.tab.c"
    break;

  case 117: /* stmt: KW_LAST KW_IF expr ';'  */
#line 797 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4007 "raku.tab.c"
    break;

  case 118: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 799 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4013 "raku.tab.c"
    break;

  case 119: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 801 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4019 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 803 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4025 "raku.tab.c"
    break;

  case 121: /* stmt: sub_decl  */
#line 804 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4031 "raku.tab.c"
    break;

  case 122: /* stmt: class_decl  */
#line 805 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4037 "raku.tab.c"
    break;

  case 123: /* stmt: role_decl  */
#line 806 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4043 "raku.tab.c"
    break;

  case 124: /* stmt: grammar_decl  */
#line 807 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4049 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block  */
#line 811 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4055 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 813 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4061 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 815 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4067 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block  */
#line 817 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4073 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 819 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4079 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 821 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4085 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF expr block elsif_tail  */
#line 823 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4091 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 825 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4097 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block  */
#line 829 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4103 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 831 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4109 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 833 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4115 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 835 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4121 "raku.tab.c"
    break;

  case 137: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 837 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4127 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4133 "raku.tab.c"
    break;

  case 139: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 843 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4139 "raku.tab.c"
    break;

  case 140: /* while_stmt: KW_WHILE expr block  */
#line 845 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 4145 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 849 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4151 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4157 "raku.tab.c"
    break;

  case 143: /* unless_stmt: KW_UNLESS expr block  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4163 "raku.tab.c"
    break;

  case 144: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4169 "raku.tab.c"
    break;

  case 145: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 859 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4175 "raku.tab.c"
    break;

  case 146: /* until_stmt: KW_UNTIL expr block  */
#line 861 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 147: /* repeat_stmt: KW_REPEAT block  */
#line 865 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4187 "raku.tab.c"
    break;

  case 148: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 867 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4193 "raku.tab.c"
    break;

  case 149: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4199 "raku.tab.c"
    break;

  case 150: /* loop_stmt: KW_LOOP block  */
#line 873 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4205 "raku.tab.c"
    break;

  case 151: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 875 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4211 "raku.tab.c"
    break;

  case 152: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 877 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4217 "raku.tab.c"
    break;

  case 153: /* loop_incr: expr  */
#line 880 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4223 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 884 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4233 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 890 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4243 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 896 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4252 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 901 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4262 "raku.tab.c"
    break;

  case 158: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 907 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4271 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR expr block  */
#line 912 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4278 "raku.tab.c"
    break;

  case 160: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 917 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4290 "raku.tab.c"
    break;

  case 161: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 925 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4303 "raku.tab.c"
    break;

  case 162: /* when_list: %empty  */
#line 935 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4309 "raku.tab.c"
    break;

  case 163: /* when_list: when_list KW_WHEN expr block  */
#line 937 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4317 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 943 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4329 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 951 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4339 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT sub_body  */
#line 957 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4349 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 963 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4361 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 971 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4371 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 977 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4381 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 983 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4394 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 992 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4405 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 999 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4418 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1008 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4431 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1017 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4442 "raku.tab.c"
    break;

  case 175: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 1026 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4453 "raku.tab.c"
    break;

  case 176: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 1033 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4462 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list '}'  */
#line 1039 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4468 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr '}'  */
#line 1041 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1044 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4482 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1047 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4488 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1049 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4494 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1051 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4500 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1053 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4506 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1055 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4512 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1057 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4518 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1059 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4524 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1061 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4531 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1064 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4537 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1066 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4543 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1068 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4549 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1070 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4556 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1073 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4563 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1076 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list '}'  */
#line 1081 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4577 "raku.tab.c"
    break;

  case 195: /* method_body: '{' YADA '}'  */
#line 1082 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr '}'  */
#line 1084 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1087 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4597 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1090 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4603 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1092 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4609 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1094 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4615 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1096 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4621 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1098 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4627 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1100 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4633 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1102 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1104 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4646 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1107 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4652 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1109 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4658 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1111 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4664 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1113 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4671 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1116 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4678 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 212: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1125 "raku.y"
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
#line 4704 "raku.tab.c"
    break;

  case 213: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1141 "raku.y"
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
#line 4721 "raku.tab.c"
    break;

  case 214: /* is_clauses: %empty  */
#line 1155 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4727 "raku.tab.c"
    break;

  case 215: /* is_clauses: is_clauses IDENT IDENT  */
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
#line 4743 "raku.tab.c"
    break;

  case 216: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1169 "raku.y"
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
#line 4759 "raku.tab.c"
    break;

  case 217: /* class_body_list: %empty  */
#line 1182 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4765 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1184 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4772 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1187 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4779 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1190 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4786 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1193 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4793 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1196 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4800 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1199 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4807 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1202 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4814 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1205 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4821 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1208 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4828 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1211 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4835 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1214 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4842 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1217 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4849 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1220 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4860 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1227 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4871 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1234 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4882 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1241 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4893 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1248 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4901 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1252 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4909 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1256 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4917 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1260 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4925 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1264 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4936 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1271 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4947 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1278 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4958 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1285 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4969 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1292 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4983 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1302 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4995 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1310 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5007 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1318 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5020 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1327 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5031 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1334 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5042 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1341 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5056 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1351 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5068 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1359 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5080 "raku.tab.c"
    break;

  case 251: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1369 "raku.y"
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
#line 5097 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: %empty  */
#line 1383 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5103 "raku.tab.c"
    break;

  case 253: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1385 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5112 "raku.tab.c"
    break;

  case 254: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1390 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5121 "raku.tab.c"
    break;

  case 255: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1395 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5130 "raku.tab.c"
    break;

  case 256: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1402 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5138 "raku.tab.c"
    break;

  case 257: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1406 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5146 "raku.tab.c"
    break;

  case 258: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1410 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5154 "raku.tab.c"
    break;

  case 259: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1414 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5162 "raku.tab.c"
    break;

  case 260: /* pair_list: IDENT OP_FATARROW expr  */
#line 1420 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5168 "raku.tab.c"
    break;

  case 261: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1422 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5174 "raku.tab.c"
    break;

  case 262: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1424 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5180 "raku.tab.c"
    break;

  case 263: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1426 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5186 "raku.tab.c"
    break;

  case 264: /* param_list: VAR_SCALAR  */
#line 1429 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5192 "raku.tab.c"
    break;

  case 265: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1430 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5198 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1431 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5204 "raku.tab.c"
    break;

  case 267: /* param_list: VAR_ARRAY  */
#line 1432 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5210 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_ARRAY  */
#line 1433 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5216 "raku.tab.c"
    break;

  case 269: /* param_list: IDENT VAR_SCALAR  */
#line 1434 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5222 "raku.tab.c"
    break;

  case 270: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1435 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5228 "raku.tab.c"
    break;

  case 271: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1436 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5234 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' VAR_SCALAR  */
#line 1437 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5240 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1438 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5246 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1439 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5252 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1440 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5258 "raku.tab.c"
    break;

  case 276: /* param_list: VAR_SCALAR '=' expr  */
#line 1441 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5264 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1442 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5270 "raku.tab.c"
    break;

  case 278: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1443 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5276 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1444 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5282 "raku.tab.c"
    break;

  case 280: /* param_list: SLURPY_POS  */
#line 1445 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5288 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' SLURPY_POS  */
#line 1446 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5294 "raku.tab.c"
    break;

  case 282: /* param_list: SLURPY_LOL  */
#line 1447 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5300 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' SLURPY_LOL  */
#line 1448 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5306 "raku.tab.c"
    break;

  case 284: /* param_list: SLURPY_NAMED  */
#line 1449 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5312 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1450 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5318 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list '}'  */
#line 1453 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5324 "raku.tab.c"
    break;

  case 287: /* block: '{' YADA '}'  */
#line 1454 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5330 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr '}'  */
#line 1456 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5336 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1458 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1460 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5348 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1462 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5354 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1464 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5360 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1466 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5367 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1469 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5373 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1471 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5379 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1473 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5385 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1475 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5391 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1477 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5398 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1480 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5405 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1483 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5413 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1487 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5420 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1490 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5427 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1493 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5434 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1496 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5440 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1498 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5448 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1502 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5455 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1505 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5462 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1508 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5469 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1511 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5476 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1514 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5483 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1517 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5489 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1519 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5495 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1521 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5501 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST '}'  */
#line 1523 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5507 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1525 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5513 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1527 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5519 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1529 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5525 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1531 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5531 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1533 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5537 "raku.tab.c"
    break;

  case 320: /* closure: '{' expr '}'  */
#line 1536 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5543 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR '=' expr  */
#line 1539 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5549 "raku.tab.c"
    break;

  case 322: /* expr: VAR_ARRAY '=' expr  */
#line 1540 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5555 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1541 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5561 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1542 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5567 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1543 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5573 "raku.tab.c"
    break;

  case 326: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1544 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5579 "raku.tab.c"
    break;

  case 327: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1545 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5585 "raku.tab.c"
    break;

  case 328: /* expr: KW_GATHER block  */
#line 1546 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5595 "raku.tab.c"
    break;

  case 329: /* expr: KW_GATHER for_stmt  */
#line 1551 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5605 "raku.tab.c"
    break;

  case 330: /* expr: tern_expr OP_FATARROW expr  */
#line 1557 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5611 "raku.tab.c"
    break;

  case 331: /* expr: tern_expr  */
#line 1558 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5617 "raku.tab.c"
    break;

  case 332: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5623 "raku.tab.c"
    break;

  case 333: /* tern_expr: cmp_expr  */
#line 1563 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5629 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1566 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5635 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1567 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5641 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1568 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5647 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1569 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5653 "raku.tab.c"
    break;

  case 338: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1570 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5659 "raku.tab.c"
    break;

  case 339: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1571 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5665 "raku.tab.c"
    break;

  case 340: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1572 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5671 "raku.tab.c"
    break;

  case 341: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1573 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5677 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1574 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5683 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1575 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5689 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1576 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5695 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1577 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5701 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1578 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5707 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1579 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5713 "raku.tab.c"
    break;

  case 348: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1581 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5723 "raku.tab.c"
    break;

  case 349: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1587 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5733 "raku.tab.c"
    break;

  case 350: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1593 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5743 "raku.tab.c"
    break;

  case 351: /* cmp_expr: divis_expr  */
#line 1598 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5749 "raku.tab.c"
    break;

  case 352: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1601 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5755 "raku.tab.c"
    break;

  case 353: /* divis_expr: jct_expr  */
#line 1602 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5761 "raku.tab.c"
    break;

  case 354: /* jct_expr: jct_expr '|' range_expr  */
#line 1605 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5767 "raku.tab.c"
    break;

  case 355: /* jct_expr: jct_expr '&' range_expr  */
#line 1606 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5773 "raku.tab.c"
    break;

  case 356: /* jct_expr: dor_expr  */
#line 1607 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5779 "raku.tab.c"
    break;

  case 357: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1611 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5785 "raku.tab.c"
    break;

  case 358: /* dor_expr: range_expr  */
#line 1612 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5791 "raku.tab.c"
    break;

  case 359: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1615 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5797 "raku.tab.c"
    break;

  case 360: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1616 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5803 "raku.tab.c"
    break;

  case 361: /* range_expr: add_expr  */
#line 1617 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5809 "raku.tab.c"
    break;

  case 362: /* add_expr: add_expr '~' repl_expr  */
#line 1620 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5815 "raku.tab.c"
    break;

  case 363: /* add_expr: repl_expr  */
#line 1621 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5821 "raku.tab.c"
    break;

  case 364: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1624 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5827 "raku.tab.c"
    break;

  case 365: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1625 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5833 "raku.tab.c"
    break;

  case 366: /* repl_expr: addsub_expr  */
#line 1626 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5839 "raku.tab.c"
    break;

  case 367: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1629 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5845 "raku.tab.c"
    break;

  case 368: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1630 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5851 "raku.tab.c"
    break;

  case 369: /* addsub_expr: mul_expr  */
#line 1631 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5857 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr '*' unary_expr  */
#line 1634 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5863 "raku.tab.c"
    break;

  case 371: /* mul_expr: mul_expr '/' unary_expr  */
#line 1635 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5869 "raku.tab.c"
    break;

  case 372: /* mul_expr: mul_expr '%' unary_expr  */
#line 1636 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5875 "raku.tab.c"
    break;

  case 373: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1637 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5881 "raku.tab.c"
    break;

  case 374: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1639 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5887 "raku.tab.c"
    break;

  case 375: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1641 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5893 "raku.tab.c"
    break;

  case 376: /* mul_expr: unary_expr  */
#line 1642 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5899 "raku.tab.c"
    break;

  case 377: /* unary_expr: '-' unary_expr  */
#line 1645 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5905 "raku.tab.c"
    break;

  case 378: /* unary_expr: '+' unary_expr  */
#line 1646 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 5911 "raku.tab.c"
    break;

  case 379: /* unary_expr: '!' unary_expr  */
#line 1647 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5917 "raku.tab.c"
    break;

  case 380: /* unary_expr: CARET unary_expr  */
#line 1648 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5923 "raku.tab.c"
    break;

  case 381: /* unary_expr: OP_REDUCE unary_expr  */
#line 1650 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5932 "raku.tab.c"
    break;

  case 382: /* unary_expr: pow_expr  */
#line 1654 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5938 "raku.tab.c"
    break;

  case 383: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1657 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5944 "raku.tab.c"
    break;

  case 384: /* pow_expr: postfix_expr  */
#line 1658 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5950 "raku.tab.c"
    break;

  case 385: /* scalar_list: VAR_SCALAR  */
#line 1661 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5956 "raku.tab.c"
    break;

  case 386: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1662 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5962 "raku.tab.c"
    break;

  case 387: /* meth_name: IDENT  */
#line 1665 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5968 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_SORT  */
#line 1666 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5974 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_REVERSE  */
#line 1667 "raku.y"
                 { (yyval.sval)=strdup("reverse"); }
#line 5980 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_MAP  */
#line 1668 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5986 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_GREP  */
#line 1669 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5992 "raku.tab.c"
    break;

  case 392: /* meth_name: KW_SAY  */
#line 1670 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5998 "raku.tab.c"
    break;

  case 393: /* meth_name: KW_PRINT  */
#line 1671 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 6004 "raku.tab.c"
    break;

  case 394: /* meth_name: KW_TAKE  */
#line 1672 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 6010 "raku.tab.c"
    break;

  case 395: /* meth_name: KW_RETURN  */
#line 1673 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 6016 "raku.tab.c"
    break;

  case 396: /* meth_name: KW_EXISTS  */
#line 1674 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 6022 "raku.tab.c"
    break;

  case 397: /* meth_name: KW_DELETE  */
#line 1675 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 6028 "raku.tab.c"
    break;

  case 398: /* meth_name: KW_JOIN  */
#line 1676 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 6034 "raku.tab.c"
    break;

  case 399: /* meth_name: TESTOP  */
#line 1677 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6040 "raku.tab.c"
    break;

  case 400: /* postfix_expr: call_expr  */
#line 1679 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 6046 "raku.tab.c"
    break;

  case 401: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1682 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6056 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '(' arg_list ')'  */
#line 1688 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6065 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '(' ')'  */
#line 1692 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 6071 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1694 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 6077 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1696 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 6083 "raku.tab.c"
    break;

  case 406: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1698 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 6092 "raku.tab.c"
    break;

  case 407: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1703 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 6098 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1705 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6108 "raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1711 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6118 "raku.tab.c"
    break;

  case 410: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1717 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 6126 "raku.tab.c"
    break;

  case 411: /* call_expr: IDENT '.' KW_NEW  */
#line 1721 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6134 "raku.tab.c"
    break;

  case 412: /* call_expr: IDENT '.' IDENT  */
#line 1725 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6143 "raku.tab.c"
    break;

  case 413: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1730 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6154 "raku.tab.c"
    break;

  case 414: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1737 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6163 "raku.tab.c"
    break;

  case 415: /* call_expr: IDENT '.' CARET IDENT  */
#line 1742 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6173 "raku.tab.c"
    break;

  case 416: /* call_expr: atom '.' CARET IDENT  */
#line 1748 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6183 "raku.tab.c"
    break;

  case 417: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1754 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6194 "raku.tab.c"
    break;

  case 418: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1761 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6205 "raku.tab.c"
    break;

  case 419: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1768 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6214 "raku.tab.c"
    break;

  case 420: /* call_expr: atom '.' meth_name  */
#line 1773 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6223 "raku.tab.c"
    break;

  case 421: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1778 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6234 "raku.tab.c"
    break;

  case 422: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1785 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6243 "raku.tab.c"
    break;

  case 423: /* call_expr: call_expr '.' meth_name  */
#line 1790 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6252 "raku.tab.c"
    break;

  case 424: /* call_expr: KW_DIE expr  */
#line 1795 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6258 "raku.tab.c"
    break;

  case 425: /* call_expr: KW_MAP closure expr  */
#line 1797 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6264 "raku.tab.c"
    break;

  case 426: /* call_expr: KW_MAP closure ',' expr  */
#line 1799 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6270 "raku.tab.c"
    break;

  case 427: /* call_expr: KW_GREP closure expr  */
#line 1801 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6276 "raku.tab.c"
    break;

  case 428: /* call_expr: KW_SORT expr  */
#line 1803 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6282 "raku.tab.c"
    break;

  case 429: /* call_expr: KW_SORT closure expr  */
#line 1805 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6288 "raku.tab.c"
    break;

  case 430: /* call_expr: KW_REVERSE expr  */
#line 1807 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6294 "raku.tab.c"
    break;

  case 431: /* call_expr: atom  */
#line 1808 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6300 "raku.tab.c"
    break;

  case 432: /* arg_list: expr  */
#line 1811 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6306 "raku.tab.c"
    break;

  case 433: /* arg_list: arg_list ',' expr  */
#line 1812 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6312 "raku.tab.c"
    break;

  case 434: /* arg_list: arg_list ','  */
#line 1813 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6318 "raku.tab.c"
    break;

  case 435: /* atom: LIT_INT  */
#line 1816 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6324 "raku.tab.c"
    break;

  case 436: /* atom: LIT_FLOAT  */
#line 1817 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6330 "raku.tab.c"
    break;

  case 437: /* atom: LIT_STR  */
#line 1818 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6336 "raku.tab.c"
    break;

  case 438: /* atom: WORDLIST  */
#line 1820 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6349 "raku.tab.c"
    break;

  case 439: /* atom: LIT_INTERP_STR  */
#line 1828 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6355 "raku.tab.c"
    break;

  case 440: /* atom: VAR_SCALAR  */
#line 1829 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6361 "raku.tab.c"
    break;

  case 441: /* atom: OP_INC VAR_SCALAR  */
#line 1830 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6367 "raku.tab.c"
    break;

  case 442: /* atom: OP_DEC VAR_SCALAR  */
#line 1831 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6373 "raku.tab.c"
    break;

  case 443: /* atom: VAR_SCALAR OP_INC  */
#line 1832 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6379 "raku.tab.c"
    break;

  case 444: /* atom: VAR_SCALAR OP_DEC  */
#line 1833 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6385 "raku.tab.c"
    break;

  case 445: /* atom: VAR_ARRAY  */
#line 1834 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6391 "raku.tab.c"
    break;

  case 446: /* atom: VAR_HASH  */
#line 1835 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6397 "raku.tab.c"
    break;

  case 447: /* atom: VAR_CAPTURE  */
#line 1837 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6405 "raku.tab.c"
    break;

  case 448: /* atom: VAR_FH  */
#line 1841 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6413 "raku.tab.c"
    break;

  case 449: /* atom: VAR_NAMED_CAPTURE  */
#line 1845 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6420 "raku.tab.c"
    break;

  case 450: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1848 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6426 "raku.tab.c"
    break;

  case 451: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1850 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6432 "raku.tab.c"
    break;

  case 452: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1852 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6438 "raku.tab.c"
    break;

  case 453: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1854 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6444 "raku.tab.c"
    break;

  case 454: /* atom: ARR_ALL_SLICE  */
#line 1856 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6450 "raku.tab.c"
    break;

  case 455: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1858 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6456 "raku.tab.c"
    break;

  case 456: /* atom: VAR_HASH '{' expr '}'  */
#line 1860 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6462 "raku.tab.c"
    break;

  case 457: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1862 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6468 "raku.tab.c"
    break;

  case 458: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1864 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6474 "raku.tab.c"
    break;

  case 459: /* atom: IDENT  */
#line 1865 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6480 "raku.tab.c"
    break;

  case 460: /* atom: VAR_TWIGIL  */
#line 1867 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6488 "raku.tab.c"
    break;

  case 461: /* atom: VAR_ARRAY_TWIGIL  */
#line 1871 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6496 "raku.tab.c"
    break;

  case 462: /* atom: VAR_HASH_TWIGIL  */
#line 1875 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6504 "raku.tab.c"
    break;

  case 463: /* atom: '[' ']'  */
#line 1878 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6510 "raku.tab.c"
    break;

  case 464: /* atom: '[' expr ']'  */
#line 1880 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6516 "raku.tab.c"
    break;

  case 465: /* atom: '[' expr ',' ']'  */
#line 1882 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6522 "raku.tab.c"
    break;

  case 466: /* atom: '[' expr ',' arg_list ']'  */
#line 1884 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6529 "raku.tab.c"
    break;

  case 467: /* atom: DOLLAR_LBRACKET ']'  */
#line 1886 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit"); }
#line 6535 "raku.tab.c"
    break;

  case 468: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 1888 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6541 "raku.tab.c"
    break;

  case 469: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 1890 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6547 "raku.tab.c"
    break;

  case 470: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 1892 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6554 "raku.tab.c"
    break;

  case 471: /* atom: '(' ')'  */
#line 1894 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6560 "raku.tab.c"
    break;

  case 472: /* atom: '(' expr ')'  */
#line 1895 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6566 "raku.tab.c"
    break;

  case 473: /* atom: '(' expr ',' ')'  */
#line 1897 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6572 "raku.tab.c"
    break;

  case 474: /* atom: '(' expr ',' arg_list ')'  */
#line 1899 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6579 "raku.tab.c"
    break;

  case 475: /* atom: block  */
#line 1901 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6585 "raku.tab.c"
    break;

  case 476: /* atom: KW_SUB block  */
#line 1902 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6591 "raku.tab.c"
    break;


#line 6595 "raku.tab.c"

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

#line 1904 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
