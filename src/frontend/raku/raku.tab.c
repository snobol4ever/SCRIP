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
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
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
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
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

#line 439 "raku.tab.c"

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
  YYSYMBOL_KW_GIVEN = 38,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 39,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 40,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 41,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 42,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 43,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 44,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 45,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 46,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 47,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 48,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 49,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 50,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 51,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 52,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 53,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 54,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 55,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 56,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 57,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 58,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 59,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 60,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 61,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 62,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 63,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 64,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 65,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 66,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 67,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 68,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 69,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 70,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 71,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 72,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 73,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 74,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 75,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 76,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 77,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 78,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 79,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 80,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 81,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 82,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 83,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 84,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 85,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 86,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 87,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 88,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 89,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 90,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 91,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 92,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 93,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 94,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 95,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 96,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 97,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 98,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 99,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 100,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 101,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 102,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 103,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 104,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 105,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 106,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 107,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 108,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 109,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 110,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 111,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 112,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 113,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 114,                   /* OP_POW  */
  YYSYMBOL_115_ = 115,                     /* '='  */
  YYSYMBOL_116_ = 116,                     /* '!'  */
  YYSYMBOL_117_ = 117,                     /* '<'  */
  YYSYMBOL_118_ = 118,                     /* '>'  */
  YYSYMBOL_119_ = 119,                     /* '|'  */
  YYSYMBOL_120_ = 120,                     /* '&'  */
  YYSYMBOL_121_ = 121,                     /* '~'  */
  YYSYMBOL_122_ = 122,                     /* '+'  */
  YYSYMBOL_123_ = 123,                     /* '-'  */
  YYSYMBOL_124_ = 124,                     /* '*'  */
  YYSYMBOL_125_ = 125,                     /* '/'  */
  YYSYMBOL_126_ = 126,                     /* '%'  */
  YYSYMBOL_UMINUS = 127,                   /* UMINUS  */
  YYSYMBOL_128_ = 128,                     /* '.'  */
  YYSYMBOL_129_ = 129,                     /* ';'  */
  YYSYMBOL_130_ = 130,                     /* '('  */
  YYSYMBOL_131_ = 131,                     /* ')'  */
  YYSYMBOL_132_ = 132,                     /* ','  */
  YYSYMBOL_133_ = 133,                     /* '['  */
  YYSYMBOL_134_ = 134,                     /* ']'  */
  YYSYMBOL_135_ = 135,                     /* '{'  */
  YYSYMBOL_136_ = 136,                     /* '}'  */
  YYSYMBOL_137_ = 137,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 138,                 /* $accept  */
  YYSYMBOL_program = 139,                  /* program  */
  YYSYMBOL_stmt_list = 140,                /* stmt_list  */
  YYSYMBOL_stmt = 141,                     /* stmt  */
  YYSYMBOL_if_stmt = 142,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 143,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 144,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 145,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 146,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 147,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 148,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 149,                /* loop_incr  */
  YYSYMBOL_for_stmt = 150,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 151,               /* given_stmt  */
  YYSYMBOL_when_list = 152,                /* when_list  */
  YYSYMBOL_sub_decl = 153,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 154,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 155,                 /* sub_body  */
  YYSYMBOL_method_body = 156,              /* method_body  */
  YYSYMBOL_class_decl = 157,               /* class_decl  */
  YYSYMBOL_role_decl = 158,                /* role_decl  */
  YYSYMBOL_is_clauses = 159,               /* is_clauses  */
  YYSYMBOL_class_body_list = 160,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 161,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 162,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 163,           /* named_arg_list  */
  YYSYMBOL_pair_list = 164,                /* pair_list  */
  YYSYMBOL_param_list = 165,               /* param_list  */
  YYSYMBOL_block = 166,                    /* block  */
  YYSYMBOL_closure = 167,                  /* closure  */
  YYSYMBOL_expr = 168,                     /* expr  */
  YYSYMBOL_tern_expr = 169,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 170,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 171,               /* divis_expr  */
  YYSYMBOL_jct_expr = 172,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 173,                 /* dor_expr  */
  YYSYMBOL_range_expr = 174,               /* range_expr  */
  YYSYMBOL_add_expr = 175,                 /* add_expr  */
  YYSYMBOL_repl_expr = 176,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 177,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 178,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 179,               /* unary_expr  */
  YYSYMBOL_pow_expr = 180,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 181,              /* scalar_list  */
  YYSYMBOL_meth_name = 182,                /* meth_name  */
  YYSYMBOL_postfix_expr = 183,             /* postfix_expr  */
  YYSYMBOL_call_expr = 184,                /* call_expr  */
  YYSYMBOL_arg_list = 185,                 /* arg_list  */
  YYSYMBOL_atom = 186                      /* atom  */
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
#define YYLAST   4687

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  461
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1167

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   370


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
       2,     2,     2,   116,     2,     2,     2,   126,   120,     2,
     130,   131,   124,   122,   132,   123,   128,   125,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   137,   129,
     117,   115,   118,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   133,     2,   134,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   135,   119,   136,   121,     2,     2,     2,
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
     127
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   455,   455,   466,   467,   470,   472,   474,   476,   480,
     482,   484,   486,   488,   492,   495,   499,   501,   505,   508,
     512,   514,   516,   518,   520,   522,   526,   530,   532,   534,
     536,   538,   540,   542,   544,   546,   548,   556,   558,   560,
     562,   564,   566,   568,   571,   574,   576,   579,   582,   584,
     586,   588,   591,   594,   596,   599,   602,   605,   607,   612,
     616,   620,   625,   629,   630,   632,   634,   636,   638,   641,
     643,   645,   647,   650,   653,   656,   659,   662,   664,   666,
     668,   670,   673,   675,   677,   679,   682,   685,   689,   692,
     694,   696,   698,   701,   704,   707,   710,   713,   716,   719,
     720,   721,   722,   723,   724,   725,   727,   729,   731,   733,
     734,   735,   736,   737,   738,   739,   741,   743,   745,   747,
     748,   749,   750,   753,   755,   757,   759,   761,   763,   765,
     767,   771,   773,   775,   777,   779,   781,   785,   787,   791,
     793,   795,   797,   801,   803,   807,   809,   811,   815,   817,
     819,   823,   826,   832,   838,   843,   849,   854,   859,   867,
     878,   879,   885,   893,   899,   905,   913,   919,   925,   934,
     941,   950,   959,   968,   975,   982,   983,   986,   989,   991,
     993,   995,   997,   999,  1001,  1003,  1006,  1008,  1010,  1012,
    1015,  1018,  1024,  1025,  1026,  1029,  1032,  1034,  1036,  1038,
    1040,  1042,  1044,  1046,  1049,  1051,  1053,  1055,  1058,  1061,
    1067,  1083,  1098,  1099,  1111,  1125,  1126,  1129,  1132,  1135,
    1138,  1141,  1144,  1147,  1150,  1153,  1156,  1159,  1162,  1169,
    1176,  1183,  1190,  1194,  1198,  1202,  1206,  1213,  1220,  1227,
    1234,  1244,  1252,  1260,  1269,  1276,  1283,  1293,  1301,  1311,
    1326,  1327,  1332,  1337,  1344,  1348,  1352,  1356,  1362,  1364,
    1366,  1368,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,
    1390,  1391,  1392,  1393,  1396,  1397,  1398,  1400,  1402,  1404,
    1406,  1408,  1411,  1413,  1415,  1417,  1419,  1422,  1425,  1429,
    1432,  1435,  1438,  1440,  1444,  1447,  1450,  1453,  1456,  1459,
    1461,  1463,  1465,  1467,  1469,  1471,  1473,  1475,  1479,  1482,
    1483,  1484,  1485,  1486,  1487,  1488,  1489,  1494,  1497,  1499,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1516,  1522,  1528,  1534,  1537,  1538,
    1541,  1542,  1543,  1546,  1548,  1551,  1552,  1553,  1556,  1557,
    1560,  1561,  1564,  1565,  1566,  1569,  1570,  1571,  1572,  1573,
    1575,  1577,  1580,  1581,  1582,  1583,  1588,  1591,  1592,  1595,
    1596,  1599,  1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,
    1608,  1609,  1611,  1613,  1618,  1619,  1621,  1623,  1628,  1630,
    1636,  1642,  1646,  1650,  1655,  1662,  1667,  1673,  1679,  1686,
    1693,  1698,  1703,  1710,  1715,  1720,  1722,  1724,  1726,  1728,
    1730,  1733,  1734,  1735,  1738,  1739,  1740,  1741,  1750,  1751,
    1752,  1753,  1754,  1755,  1756,  1757,  1758,  1762,  1766,  1769,
    1771,  1773,  1775,  1777,  1779,  1781,  1783,  1785,  1787,  1788,
    1792,  1796,  1800,  1801,  1803,  1805,  1808,  1809,  1810,  1812,
    1815,  1816
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
  "KW_ENUM", "KW_GIVEN", "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT",
  "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT",
  "KW_LOOP", "KW_LAST", "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT",
  "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS",
  "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE",
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

#define YYPACT_NINF (-845)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-461)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -845,    14,  2066,  -845,  -845,  -845,  -845,  -845,   716,   -50,
      72,   -59,    27,  -845,  -845,  4552,  -845,  -845,  -845,    52,
    2141,    41,  3577,  3652,  3727,  3802,  3877,     5,   -56,  3877,
    2216,  2291,   675,    71,  3877,   101,   150,  3952,  4027,   -56,
     -85,    -1,   232,    48,    48,  4102,   -56,   -56,  3877,   110,
     240,   287,  4552,  -845,  -845,   251,   282,   319,  4552,  4552,
    -845,  2366,  2442,   291,  -845,  -845,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,   370,  -845,  -845,  -845,   922,   650,
    -845,   617,   661,   294,   234,  -845,   -39,   334,   433,   724,
    -845,  -845,   280,   274,   329,   452,  -845,  -845,  3877,  3877,
    3877,  3877,  3877,  3877,   515,  2517,  4177,  2593,   570,  3877,
    3877,   381,    67,   772,    -7,   317,   108,  -845,   301,   -56,
    2668,  -845,  -845,   461,   696,   107,  -845,  2744,  -845,    56,
     192,   277,   290,   759,   670,   713,   699,  2668,   532,  2668,
     137,  2668,   -56,  2668,   -56,    13,   139,    91,  -845,  -845,
     589,  -845,   338,  -845,   346,   616,   631,   709,   694,   114,
     115,  2668,   -56,  2668,   -56,   372,  2820,  -845,  3877,  3877,
    -845,  3877,  3877,  -845,  3877,  3877,  3877,  2895,  3877,  -845,
     750,  -845,  -845,  -845,   710,   717,    25,  -845,   735,  -845,
    -845,  -845,  -845,   762,  -845,   388,   513,  -845,   525,   768,
    1439,  3877,  3877,  3877,  3877,  3877,  3877,  3877,  3877,  -845,
    3877,  3877,  3877,  3877,  3877,  3877,  3877,  3877,  -845,  4552,
    4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,
    4552,  4552,  4552,  4552,   786,  4552,  4552,  4552,  4552,  4552,
    4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,  4552,
    4552,  4552,  1081,  1035,   543,   790,   792,   800,   802,   811,
     818,   103,  -845,   704,  2668,   327,   719,   625,   825,   837,
     833,  -845,   845,   974,   867,   202,  -845,   990,   744,   758,
    2593,  1021,  3877,  -845,  3877,  3877,  3877,  3877,  3877,  3877,
    3877,   909,   388,   769,  -845,  3877,  3877,  -845,  4252,  -845,
    4327,  -845,   292,   355,   379,   268,   910,   938,  -845,   779,
     781,  3877,  3877,  3877,  3877,  3877,  3877,  3877,  -845,  3877,
     783,  3877,  3877,  -845,  3877,   785,   896,   796,  -845,  1045,
    3877,  -845,  4552,  4552,     1,  -845,  -845,  -845,  3877,  3877,
    -845,  3877,  3877,  -845,  3877,  3877,   916,  -845,  1053,  3877,
    1062,  3877,   803,  1058,   805,  -845,  3877,  3877,  1092,   975,
     984,   997,  1002,  1015,  1026,  -845,  -845,  -845,   -56,    -5,
    -845,    23,  1033,  1034,  -845,  -845,  2970,  1050,  1158,  3046,
    -845,  -845,  1079,   157,   253,  1054,  3577,  3652,  3877,  1782,
      61,   231,  -845,   397,  1041,  1042,  1043,  1044,  1046,  1047,
    1060,  1061,  1063,  1064,  1066,  1067,  1068,  1069,  1070,  1073,
    1093,  1093,  1093,  1093,  1093,  1093,  1078,  1093,  1093,  1093,
    1093,  1093,  1093,  1093,  1093,  -845,  -845,  -845,   294,  -845,
    -845,  -845,  1082,  1082,   334,   433,   724,   724,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  -845,  1075,  1194,  1080,  -845,
    3121,  -845,  -845,  -845,  -845,  -845,  -845,  3877,  3197,  -845,
     807,  3877,  -845,  3877,  3877,  3877,  3877,  1096,  1097,  1098,
    -845,  3273,  -845,  1135,  3877,  1085,  -845,    -8,  -845,  1304,
     747,  1099,  1083,  -845,  -845,  -845,  -845,  -845,  -845,  -845,
    -845,  1087,  -845,  1089,  2668,   363,  1141,   267,  3349,   135,
    1094,  3877,  -845,  4402,  -845,  3877,  -845,    47,  -845,  3877,
    3877,  1106,  2970,  1100,  1101,  1108,  1120,  1121,  1123,  1124,
     349,  2970,  1125,  1126,   470,   -56,   -18,  4477,  -845,   -56,
     -56,   342,   -11,    44,    -6,  -845,    30,  -845,  -845,  -845,
    1090,   814,  1573,  1127,  1128,  1132,  1134,  1138,  1140,  -845,
       8,  1104,  1091,  1146,  1137,   -56,   -56,   -56,  1145,  1147,
    1111,  3877,  -845,  -845,  -845,  -845,  -845,  -845,  1200,  1261,
    -845,    94,  1090,   822,    53,   511,    -3,  -845,   824,  3877,
    -845,  -845,   764,  1263,  2593,  3877,  3877,   407,   105,    22,
    -845,   318,  3877,  3877,  -845,  3877,  3877,  -845,  3877,  3877,
    3877,  3877,  3877,  3877,  3877,  3877,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  4552,  3425,  -845,  1228,  1149,   826,  1153,
    -845,   835,  2970,  1154,   604,  1151,  1152,   765,  3877,  3877,
    3877,  -845,   838,  -845,   840,   846,  -845,  3877,  1208,  1274,
     848,  -845,  -845,  -845,  -845,  -845,   869,  3877,  -845,  3877,
    3877,  3877,   888,  -845,   321,  -845,  1160,  2668,   613,  1161,
    1090,   892,  1162,  1163,  3877,   930,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,   932,  -845,  -845,  -845,  1038,  -845,
    -845,  2668,   -56,  -845,  -845,  1285,  -845,  1286,  1287,  1284,
    3877,  1185,  1291,  1292,  -845,  1090,   652,  3577,  3652,  1857,
    -845,   459,  -845,  -845,  -845,  -845,  -845,  -845,  3877,   -56,
    -845,  -845,  -845,  1174,  1175,  1278,  -845,  -845,  -845,  -845,
    3877,  1177,  -845,  -845,   212,   908,   870,  1253,  -845,  -845,
    1090,  1090,   939,   941,  1298,  1299,  1312,  -845,  -845,  1198,
    -845,   112,   774,  1195,   305,  3877,  3877,  3877,  3877,  3877,
    3877,  -845,  -845,  -845,  -845,   322,   332,   350,   354,   362,
     367,   378,   394,   405,   408,   409,   413,  -845,  -845,   943,
    -845,   949,   958,  -845,  1201,  -845,  -845,   960,  -845,  -845,
    -845,  -845,  -845,  1203,  1204,  1205,  -845,  -845,  -845,  1206,
    3877,  1209,  -845,  2970,  1211,   629,  -845,  -845,  1212,  1259,
    1264,  -845,   962,  -845,  3877,  -845,  -845,  1090,  -845,  -845,
     677,  1213,  1216,   -18,  -845,   965,  1071,   -56,   -56,   -56,
    -845,  -845,  3877,  -845,  -845,  -845,     4,  -845,    39,  -845,
    -845,  -845,   522,   123,  -845,   324,  3877,  3877,  3877,  3877,
    3877,  3877,  3877,  3877,  -845,   -56,  1210,  -845,  -845,   -56,
    1219,  3877,  -845,   441,   451,    64,    66,   797,  1220,  1221,
    1081,  -845,  -845,  1090,  1090,  1345,  1346,  1347,  -845,  3877,
    3501,  1247,  1249,  -845,   417,   418,   430,   436,   439,   453,
    -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  -845,  1237,  -845,  -845,  -845,
    -845,  -845,  3877,   969,  -845,  -845,  -845,  3877,  3877,  2970,
     698,  -845,  -845,  3877,  -845,  -845,  -845,  -845,   -56,   -56,
    -845,  -845,  -845,  -845,  -845,  1353,  3877,  1256,  1358,  1362,
    3877,  3877,  3877,  -845,  -845,  -845,   469,   485,   487,   488,
     490,   491,   496,   500,  -845,  -845,  -845,  3877,  1242,  -845,
     237,  1300,  -845,   289,  -845,  1363,  1364,  1250,  3877,  -845,
    1366,  1367,  1254,  3877,  -845,   124,   275,  1255,  1257,  -845,
    -845,   504,  -845,  -845,  -845,  -845,  -845,   502,  1240,   976,
    3877,  3877,  -845,  -845,  -845,  -845,  -845,  -845,  -845,  1251,
    1258,  -845,  -845,   978,  -845,   742,  1084,  -845,  -845,  -845,
    3877,  -845,  -845,   506,   514,   531,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  1260,   -56,  1262,   998,  1252,  1707,
    1262,  1011,  1265,  1266,  -845,  1267,  1269,  1270,  -845,  1271,
    1371,  1376,  1272,  3877,  -845,  1377,  1378,  1273,  3877,  -845,
    -845,  -845,   313,  -845,  -845,  -845,  1275,   541,   542,  -845,
    -845,  1277,  -845,   -56,  -845,  -845,  -845,  -845,  -845,  3877,
    -845,  -845,  1262,  -845,  3577,  3652,  1932,  -845,   486,  -845,
    1262,  -845,  -845,  -845,  -845,  -845,  -845,  1279,  1280,  -845,
    1281,  1283,  1288,  -845,  1289,  1262,  1025,  -845,  -845,  -845,
    -845,  -845,  1276,  -845,   611,   230,  -845,   326,  3877,  3877,
    3877,  3877,  3877,  3877,  3877,  3877,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  1262,   -56,  3877,  3877,  3877,
    -845,  -845,  -845,   545,   554,   557,   558,   568,   569,   577,
     578,  -845,  -845,   603,   608,   645,  -845,  -845,  -845,  -845,
    -845,  -845,  -845,  -845,  -845,  -845,  -845
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   424,   425,   426,   428,   429,   434,
     435,   449,   448,   450,   451,     0,   436,   437,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   443,   427,     0,     0,     0,     0,     0,
     100,     0,     0,     3,     4,   101,   102,   109,   110,   111,
     112,   103,   104,   119,     0,   120,   121,   122,   108,     0,
     327,   329,   347,   349,   352,   354,   357,   359,   361,   364,
     371,   376,   378,   392,   420,     0,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   434,   435,   449,   448,     0,
       0,   460,   374,     0,   429,   434,    37,     0,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   357,     0,   461,   326,
       0,    50,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,   148,     0,     0,
     113,     0,     0,   114,     0,     0,     0,     3,     0,   418,
     105,   107,   415,   212,     0,     0,     0,   375,     0,   430,
     431,   373,   372,   429,   456,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   322,   323,   324,   325,
     319,     0,   398,     0,     0,   320,     0,     0,     0,     0,
       0,    41,   403,     0,   402,   448,   394,     0,     0,     0,
       0,     0,     0,    31,     0,     0,     0,     0,     0,     0,
       0,   456,     0,     0,    40,   423,     0,     6,     0,    10,
       0,    11,     0,     0,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,    45,     0,     0,   126,     0,   138,     0,
       0,   157,     0,     0,     0,     3,   164,    48,     0,     0,
      49,     0,     0,    53,     0,     0,     0,   160,     0,     0,
       0,     0,     0,   141,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   416,   417,   419,     0,     0,
     215,     0,     0,     0,   250,   457,     0,     0,     0,     0,
     453,   285,   429,   434,   435,   449,     0,     0,     0,     0,
       0,     0,   284,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     332,   333,   336,   337,   330,   331,     0,   334,   335,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   348,   350,
     351,   353,   355,   356,   358,   360,   362,   363,   368,   369,
     370,   365,   366,   367,   377,   381,   391,   385,   386,   387,
     388,   389,   390,   383,   384,   382,   414,     0,   411,    60,
       0,    94,    95,    96,    97,    98,    57,     0,     0,   397,
       0,     0,    16,     0,     0,     0,     0,   439,   444,   445,
      62,     0,   406,     0,     0,     0,   395,     0,   393,   423,
       0,     0,     0,   321,   322,   323,   324,   325,   319,   320,
      39,     0,   422,     0,     0,     0,   426,   448,     0,     0,
       0,     0,    28,     0,    29,     0,    30,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,     0,   129,   457,
       0,     0,   355,   356,   262,   265,     0,   278,   280,   282,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,   457,     0,   457,     0,     0,
       0,     0,   115,   116,   117,   118,   318,   106,     0,     0,
     215,     0,     0,     0,     0,     0,     0,   458,     0,     0,
     380,   454,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,   312,     0,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,   286,    64,    66,    68,
      71,    69,    70,    65,    67,    77,    79,    81,    84,    82,
      83,    78,    80,     0,     0,   407,     0,     0,     0,     0,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,   401,     0,     0,   254,     0,     0,     0,
       0,   439,   444,   445,    38,     5,     0,     0,    12,     0,
       0,     0,     0,    21,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    88,    87,    91,
      89,    90,    86,    43,     0,    92,    93,    46,   123,   128,
     127,     0,     0,   137,   154,     0,   156,     0,     0,     0,
       0,   267,     0,     0,   163,     0,     0,     0,     0,     0,
     175,     0,    51,    52,    55,    56,    33,    32,     0,     0,
     158,   446,   447,     0,     0,   139,   142,   143,   146,   147,
       0,     0,   213,   214,     0,     0,     0,     0,   211,   172,
       0,     0,     0,     0,     0,     0,     0,   249,   459,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   295,   302,   309,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,   413,     0,
     410,     0,     0,    59,     0,    61,   173,     0,    18,    17,
     442,   441,   440,     0,     0,     0,   404,   399,   400,     0,
       0,     0,   396,     0,     0,     0,   259,   258,     0,     0,
       0,    23,     0,    24,     0,    27,   166,     0,    35,    34,
       0,   459,   459,     0,   130,     0,   131,     0,     0,     0,
     263,   274,     0,   268,   269,   162,   270,   266,     0,   279,
     281,   283,     0,     0,   178,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,    75,    76,     0,
       0,     0,   210,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   169,     0,     0,     0,     0,     0,     9,     0,
       0,   439,   445,   306,     0,     0,     0,     0,     0,     0,
     314,   315,   316,   317,   287,   289,   291,   294,   292,   293,
     288,   290,   412,   409,   408,    58,   459,    72,    73,    74,
     255,   256,     0,     0,    14,    13,    22,     0,     0,     0,
       0,   165,     7,     0,    44,    47,   125,   124,   457,     0,
     135,   155,   152,   153,   276,     0,     0,   271,     0,     0,
       0,     0,     0,   179,   180,   177,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   159,   140,     0,     0,   151,
       0,     3,   245,     0,   242,     0,     0,     0,     0,   221,
       0,     0,     0,     0,   216,     0,     0,     0,     0,   217,
     218,     0,   168,   170,   251,   252,   253,     0,   174,     0,
       0,     0,   296,   298,   301,   299,   300,   297,    19,     0,
     459,   261,   260,     0,    25,     0,   132,   133,   264,   275,
       0,   272,   273,     0,     0,     0,   181,   183,   185,   188,
     186,   187,   182,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   222,
     219,   220,     0,   248,   305,   304,   173,     0,     0,   257,
      15,   459,     8,     0,   136,   277,   189,   191,   190,     0,
     150,   244,     0,   193,     0,     0,     0,   192,     0,   241,
       0,   230,   231,   225,   228,   229,   224,     0,     0,   234,
       0,     0,     0,   235,     0,     0,     0,   303,   307,   308,
      26,   134,     0,   243,     0,     0,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   194,   240,   238,   239,
     227,   236,   237,   226,   247,     0,     0,     0,     0,     0,
     197,   198,   195,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   149,     0,     0,     0,   199,   201,   203,   206,
     204,   205,   200,   202,   207,   209,   208
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -845,  -845,     3,  -845,  -531,  -694,  -845,  -845,  -845,  -845,
    -845,   314,  -845,  -845,  -845,  -845,  -845,  -287,  -844,  -845,
    -845,  -845,   823,  -845,  -845,  -476,   905,  -340,   189,  1114,
      -2,  -223,  -845,   763,  1179,  -845,   615,   -16,  1178,  1173,
     917,   128,  -845,  1290,  -252,  -845,  -845,   -51,  -845
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   200,    64,    65,   538,    66,    67,    68,    69,
      70,   968,    71,    72,   560,    73,    74,   336,   972,    75,
      76,   369,   581,    77,   586,   278,   509,   551,   121,   175,
     128,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   196,   456,    92,    93,   129,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,   458,   416,     2,   834,   699,   658,   654,    24,   578,
     146,   544,   545,   660,     3,   546,   709,   579,   518,   147,
     138,   140,   142,   144,   145,   168,   945,   150,   152,   154,
     974,   583,   158,   544,   545,   162,   164,   546,   111,   372,
     711,   239,   240,   179,   169,   166,   182,   728,   729,   947,
      63,   130,   131,   132,   263,   133,   110,   544,   545,   195,
     198,   546,   279,   544,   545,   106,   123,   546,   547,   548,
     549,   707,   134,   754,   755,   756,   293,   135,   975,    63,
     980,   272,   241,   107,   273,   157,   976,   602,   981,   373,
     547,   548,   549,    96,    97,   329,   255,   256,   257,   258,
     259,   260,   712,   713,   265,   267,   603,   269,   270,   710,
     241,   948,   949,   159,   547,   548,   549,    63,   195,   946,
     547,   548,   549,   105,   183,   292,   708,   274,   170,   659,
     580,   321,   550,   757,   977,   310,   982,   320,  1050,   325,
      63,   327,   940,   122,   730,   330,  1051,  1063,    63,   321,
     322,   337,   745,   746,   582,   112,   747,   113,   773,   352,
     791,   354,   160,   321,   359,   241,   360,   361,   322,   362,
     363,   136,   364,   365,   366,   364,   367,   681,   680,   978,
     187,   983,   322,   174,   751,   294,   191,   192,   295,   108,
     170,    78,  1081,   979,  1052,   984,  1089,   604,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   109,   402,   403,
     404,   405,   406,   407,   408,   409,   148,   149,   467,   332,
     333,   334,   290,   432,   433,   281,   335,   889,   165,   167,
     748,   348,   350,   468,   323,   180,   181,   324,  1113,  1053,
     280,   772,   890,   282,   752,   753,  1127,   544,   545,   349,
     351,   546,   323,  1054,   184,   324,   321,   605,   171,   954,
     241,  1134,   470,   714,   673,   188,   323,   674,   530,   324,
     745,   746,   106,   534,   747,   322,   606,   172,   490,   541,
     492,   484,   493,   494,   495,   496,   497,   498,   499,  1055,
     594,  1151,   189,   502,   503,   749,   505,  1056,   510,   544,
     545,   185,   936,   546,   547,   548,   549,   296,   148,   523,
     524,   525,   526,   527,   528,   529,   542,   543,   186,   532,
     533,   297,  1074,   544,   545,   588,   819,   546,   592,   190,
     112,   326,   113,   328,   331,   820,   553,   554,   552,   555,
     556,   238,   557,   558,   338,  1057,   671,   562,   872,   564,
     338,   353,   338,   355,   568,   569,   547,   548,   549,   323,
     173,   173,   324,   339,   338,   199,  1141,   607,  1036,   339,
     108,   339,   341,   438,   439,   440,   441,   442,   443,   444,
     547,   548,   549,   339,   597,   598,   599,   601,   595,    78,
    1058,   342,   298,   826,   251,   112,   201,   113,   517,   202,
     203,   356,   252,   335,  1059,   300,   299,   511,   204,   638,
     787,   205,   206,   236,   237,   207,   208,   641,   357,   301,
    1040,   512,   644,   608,   705,   647,   609,   610,   845,   112,
     652,   113,   655,   765,   480,   611,   312,   766,   612,   613,
     471,   893,   614,   615,  1105,   767,   242,   340,   768,   769,
     280,   572,   770,   340,   774,   340,   472,   253,   900,   473,
     955,   573,  1142,   881,   882,   639,   254,   340,   901,   643,
     513,   685,   645,   646,   295,   343,   667,    63,   693,   574,
     694,   295,   656,   575,   514,   856,   902,   502,   857,   858,
     903,   625,   668,   874,   515,   669,   626,   859,   904,   209,
     860,   861,   666,   905,   862,   863,   195,   627,   516,   676,
     271,   678,  1118,   679,   906,  1119,  1120,   682,   683,   375,
     376,   544,   545,   628,  1121,   546,   218,  1122,  1123,   261,
     907,  1124,  1125,   616,   629,   702,   318,   630,   631,   319,
     931,   908,   632,   771,   909,   910,   686,   688,   950,   911,
     721,   312,   951,  1002,  1003,   243,   244,   577,   311,   689,
     314,   312,   313,   315,   316,   690,  1004,   952,   691,   741,
     314,   970,  1005,   315,   316,  1006,   971,   317,   547,   548,
     549,   973,   692,   789,   268,   792,   971,   759,   218,  1007,
     283,   797,   762,   763,   764,   864,   992,   993,   625,   697,
     775,   776,   295,   777,   778,  1026,   779,   780,   781,   782,
     783,   784,   785,   786,   626,   218,   627,   628,   815,   629,
     630,  1027,  1126,  1028,  1029,   631,  1030,  1031,   991,   632,
    1037,   795,  1032,  1041,  1062,   686,  1033,  1137,  1064,   971,
     312,  1138,  1076,   688,   377,   378,   803,   804,   805,   314,
    1077,   318,   315,   316,   319,   809,  1139,   379,   953,   380,
     692,   318,   846,   847,   319,   814,   848,  1078,   816,   817,
     917,   919,   459,   460,   625,   822,   210,  1108,  1109,   211,
     212,  1156,   830,   626,   305,   155,   627,   628,   213,   156,
    1157,   214,   215,  1158,  1159,   216,   217,   629,   630,   835,
     219,   220,   221,   222,  1160,  1161,   631,   632,   841,   308,
     223,   224,   225,  1162,  1163,   852,   853,   855,   337,   849,
     850,   851,  1106,   306,   698,   700,   865,   307,   703,   704,
     706,   344,   686,   799,   226,   227,   295,   688,   870,  1164,
     318,    78,   823,   319,  1165,   824,   345,  1140,   228,   229,
     230,   231,   232,   233,   735,   736,   737,   476,   925,   477,
     234,   295,   923,   894,   895,   896,   897,   898,   899,   302,
     303,   304,   235,   930,   692,     4,     5,     6,     7,   218,
     346,  1166,   124,   125,   116,   117,   275,    13,    14,    15,
      16,    17,    18,   425,   426,   427,    96,    97,   284,   285,
     286,   287,   288,   119,    28,   368,   932,   985,   921,   933,
     986,   289,   987,   988,    95,    35,    96,    97,    98,    99,
     100,   101,   102,    43,    44,    45,   105,  1014,    48,   347,
     295,   103,   245,   246,   247,   469,   295,    52,    53,   999,
     944,   474,   475,    54,   104,   370,   105,   371,   248,   249,
     250,   429,   430,   431,   956,   957,   958,   959,   960,   961,
     962,   963,    96,    97,   284,   285,   286,   287,   288,   969,
     374,  1072,    56,    57,   295,   486,   487,   289,  1013,   476,
     875,   661,  1015,   876,   877,   878,   879,   997,    58,   488,
     489,   836,   105,  -379,  -379,    59,   295,   295,   760,   802,
     501,   295,   120,   276,   381,    62,   476,    63,   891,   277,
     521,   378,   375,   522,   375,   531,   535,   376,   866,   461,
    1009,   462,   445,   536,   537,  1011,  1012,   539,   376,   463,
     446,   464,   447,   448,   565,   376,   567,   376,   375,   642,
     465,   449,   450,   478,  1019,   715,   716,   466,  1023,  1024,
    1025,   451,   452,   750,   716,   758,   295,   794,   295,   453,
     454,   455,   480,  -460,  -460,  1034,   796,   295,   873,   806,
     295,   807,   487,   479,  1039,   481,  1045,   808,   295,   812,
     487,  1049,   410,   411,   412,   413,   414,   415,   482,   417,
     418,   419,   420,   421,   422,   423,   424,   483,  1067,  1068,
     375,   813,  -460,  -460,   485,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  1075,   818,
     674,  -460,   937,   827,   716,   519,   941,   942,   943,  -460,
    -460,  -460,  -460,  -460,  -460,   491,  -460,  1088,   500,  -460,
    -460,  -460,  -460,  -460,  -460,   559,  -460,  -460,  -460,   445,
    -460,  1100,   457,   520,   964,   540,  1104,   446,   966,   447,
     448,   831,   295,   832,   295,   833,   537,   561,   449,   450,
     883,   716,   884,   716,   912,   295,   563,   969,   451,   452,
     913,   487,  1114,  1115,  1117,   566,   453,   454,   455,   914,
     295,   916,   295,   375,   929,   445,   938,   376,   939,   537,
    1010,   295,   570,   446,   571,   447,   448,  1066,   295,  1071,
     295,  1073,   537,   572,   449,   450,  1143,  1144,  1145,  1146,
    1147,  1148,  1149,  1150,   451,   452,   573,  1016,  1017,  1082,
     716,   574,   453,   454,   455,  1153,  1154,  1155,     4,     5,
       6,     7,  1090,   716,   575,   124,   125,   116,   117,   275,
      13,    14,    15,    16,    17,    18,  1135,   716,   176,   178,
     436,   437,   576,   584,   585,   589,   119,    28,   590,   596,
     617,   618,   619,   620,   633,   621,   622,    95,    35,    96,
      97,    98,    99,   100,   101,   102,    43,    44,    45,   623,
     624,    48,   625,   626,   103,   627,   628,   629,   630,   631,
      52,    53,   632,   241,   235,   634,    54,   593,   635,   105,
     636,   648,   649,   650,   742,   657,   664,   662,   665,   663,
     670,   684,   731,   675,  1080,   335,   740,   732,    78,   686,
     687,     4,     5,     6,     7,    56,    57,   688,   124,   125,
     116,   117,   275,    13,    14,    15,    16,    17,    18,   689,
     690,    58,   691,   692,   695,   696,   722,   723,    59,   119,
      28,   724,  1111,   725,   733,   120,   653,   726,    62,   727,
      63,    35,   277,   734,   738,   743,   739,   761,   793,    43,
      44,    45,   795,   798,    48,   800,   801,   810,   811,   821,
     825,   828,   829,    52,    53,   837,   838,   839,   840,    54,
     842,   843,   844,   867,   868,   869,   871,     4,     5,     6,
       7,   880,   885,   886,   124,   125,   116,   117,   275,    13,
      14,    15,    16,    17,    18,  1152,   887,   888,    56,    57,
     915,   892,   917,   918,   919,   119,    28,   920,   927,   922,
     924,   926,   934,   928,    58,   935,   965,    35,   967,   989,
     990,    59,   994,   995,   996,    43,    44,    45,   120,   790,
      48,    62,  1000,    63,  1001,   277,  1008,  1018,  1021,    52,
      53,  1020,  1022,  1035,  1038,    54,  1065,  1042,  1043,  1044,
    1046,  1047,  1069,  1048,  1060,  1097,  1061,  1070,  1083,  1079,
    1098,  1101,  1102,  1112,  1091,  1092,  1093,   971,  1094,  1095,
    1096,  1099,  1103,   744,    56,    57,  1110,  1136,  1128,  1129,
    1130,  1107,  1131,   672,   428,   435,     0,  1132,  1133,   434,
      58,     0,     0,     0,     0,     0,   309,    59,     0,     0,
       0,     0,     0,     0,   120,     0,     0,    62,     0,    63,
       0,   277,     4,     5,     6,     7,     0,     0,     0,   382,
     383,   384,   385,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   386,   387,    24,     0,     0,    25,    26,
      27,    28,   388,   389,    31,    32,    33,    34,     0,     0,
       0,     0,    35,    36,    37,    38,    39,    40,   390,   391,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
      50,    51,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,    60,    61,
       0,     0,    62,     0,    63,   392,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   717,   718,    24,
       0,     0,    25,    26,    27,    28,    29,   719,    31,    32,
      33,    34,     0,     0,     0,     0,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,    50,    51,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,    60,    61,     0,     0,    62,     0,    63,   720,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,  1084,  1085,    24,     0,     0,    25,    26,    27,    28,
      29,  1086,    31,    32,    33,    34,     0,     0,     0,     0,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,     0,     0,    50,    51,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,    55,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,    60,    61,    48,     0,
      62,     0,    63,  1087,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,   151,   120,    48,     0,    62,     0,    63,   600,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,   151,   120,    48,     0,
      62,     0,    63,   854,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,   151,   120,     0,     0,    62,     0,    63,  1116,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,     0,     0,     0,     0,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,     0,     0,    50,    51,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,    55,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    35,     0,     0,     0,     0,    59,
       0,     0,    43,    44,    45,    60,    61,    48,     0,    62,
       0,    63,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   124,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    35,
       0,     0,     0,     0,    59,     0,     0,    43,    44,    45,
     126,   127,    48,     0,    62,     0,    63,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    35,     0,     0,     0,     0,    59,
       0,     0,    43,    44,    45,   151,   120,    48,     0,    62,
       0,    63,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   193,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    35,
       0,     0,     0,     0,    59,     0,     0,    43,    44,    45,
     153,   120,    48,     0,    62,     0,    63,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    56,    57,     0,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,    35,     0,     0,     0,    59,
       0,     0,     0,    43,    44,    45,   120,   194,    48,    62,
       0,    63,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   120,    48,     0,    62,   197,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    56,    57,     0,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,    35,     0,     0,     0,
      59,     0,     0,     0,    43,    44,    45,   120,   262,    48,
      62,     0,    63,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,   266,     0,    43,
      44,    45,     0,   120,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    56,    57,
       0,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,    35,     0,     0,
       0,    59,     0,     0,     0,    43,    44,    45,   120,   194,
      48,    62,     0,    63,     0,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     124,   125,   116,   117,   118,    13,    14,    15,    16,    17,
      18,     0,     0,   358,    56,    57,     0,     0,     0,     0,
       0,   119,    28,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,    35,     0,     0,     0,    59,     0,     0,
       0,    43,    44,    45,   120,   291,    48,    62,     0,    63,
       0,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   124,   125,   116,   117,   118,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      56,    57,     0,     0,     0,     0,   119,    28,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    35,     0,
       0,     0,     0,    59,     0,     0,    43,    44,    45,     0,
     120,    48,     0,    62,     0,    63,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,   199,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     124,   125,   116,   117,   118,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,   119,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    35,     0,     0,     0,     0,    59,     0,
       0,    43,    44,    45,     0,   120,    48,     0,    62,     0,
      63,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   124,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      56,    57,     0,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,    35,
       0,     0,     0,    59,     0,     0,     0,    43,    44,    45,
     120,   587,    48,    62,     0,    63,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    35,     0,     0,     0,     0,    59,
       0,     0,    43,    44,    45,     0,   120,    48,     0,    62,
     591,    63,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    56,    57,     0,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
      35,     0,     0,     0,    59,     0,     0,     0,    43,    44,
      45,   120,   637,    48,    62,     0,    63,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    56,    57,     0,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,    35,     0,     0,     0,
      59,     0,     0,     0,    43,    44,    45,   120,   640,    48,
      62,     0,    63,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     4,     5,   506,     7,     0,     0,     0,   124,
     125,   116,   117,   507,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,    35,     0,     0,     0,    59,     0,     0,     0,
      43,    44,    45,   120,   651,    48,    62,     0,    63,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   124,   125,   116,   117,   118,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,   119,    28,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,    35,     0,
       0,     0,    59,     0,     0,     0,    43,    44,    45,   120,
     194,    48,    62,     0,    63,     0,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    56,    57,     0,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,    35,     0,     0,     0,    59,     0,
       0,     0,    43,    44,    45,   120,   788,    48,    62,     0,
      63,     0,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    56,    57,     0,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
      35,     0,     0,     0,    59,     0,     0,     0,    43,    44,
      45,   120,   998,    48,    62,     0,    63,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   137,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   139,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   141,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   143,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   120,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   161,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   163,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   120,    48,     0,    62,     0,   177,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   264,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,   506,     7,     0,     0,     0,   124,   125,   116,
     117,   507,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   504,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   508,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   677,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   115,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   701,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,   120,     0,     0,    62,     0,    63
};

static const yytype_int16 yycheck[] =
{
       2,   253,   225,     0,   698,   536,    14,   483,    26,    14,
      26,    10,    11,   489,     0,    14,    22,    22,   305,    14,
      22,    23,    24,    25,    26,    26,    22,    29,    30,    31,
     874,   371,    34,    10,    11,    37,    38,    14,    11,    14,
      10,    80,    81,    45,    45,   130,    48,    39,    40,    10,
     135,    10,    11,    12,   105,    14,   115,    10,    11,    61,
      62,    14,   113,    10,    11,   115,    14,    14,    67,    68,
      69,    82,    31,    76,    77,    78,   127,    36,    14,   135,
      14,    14,   121,   133,    17,    14,    22,    26,    22,    64,
      67,    68,    69,   100,   101,    82,    98,    99,   100,   101,
     102,   103,    72,    73,   106,   107,    45,   109,   110,   115,
     121,    72,    73,    12,    67,    68,    69,   135,   120,   115,
      67,    68,    69,   130,    14,   127,    82,    60,   129,   137,
     135,    26,   131,   136,    70,   137,    70,   139,    14,   141,
     135,   143,   836,    15,   136,   132,    22,   991,   135,    26,
      45,   129,    58,    59,   131,   128,    62,   130,   136,   161,
     636,   163,    12,    26,   166,   121,   168,   169,    45,   171,
     172,   130,   174,   175,   176,   177,   178,   517,   131,   115,
      52,   115,    45,   135,   131,   129,    58,    59,   132,   117,
     129,     2,  1036,   129,    70,   129,  1040,   136,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   135,   210,   211,
     212,   213,   214,   215,   216,   217,    27,    28,   115,    80,
      81,   130,   115,   239,   240,   117,   135,   115,    39,    40,
     136,   117,   117,   130,   129,    46,    47,   132,  1082,   115,
     133,   136,   130,   135,   584,   585,  1090,    10,    11,   135,
     135,    14,   129,   129,    14,   132,    26,    26,    26,   136,
     121,  1105,   264,   550,   129,    14,   129,   132,   319,   132,
      58,    59,   115,   324,    62,    45,    45,    45,   280,   330,
     282,    79,   284,   285,   286,   287,   288,   289,   290,    14,
     133,  1135,    10,   295,   296,   582,   298,    22,   300,    10,
      11,    14,   833,    14,    67,    68,    69,   115,   119,   311,
     312,   313,   314,   315,   316,   317,   332,   333,    31,   321,
     322,   129,  1016,    10,    11,   376,     5,    14,   379,    10,
     128,   142,   130,   144,   145,    14,   338,   339,   335,   341,
     342,   107,   344,   345,    26,    70,    79,   349,   136,   351,
      26,   162,    26,   164,   356,   357,    67,    68,    69,   129,
     129,   129,   132,    45,    26,    74,   136,   136,   131,    45,
     117,    45,    26,   245,   246,   247,   248,   249,   250,   251,
      67,    68,    69,    45,   386,   387,   388,   389,   135,   200,
     115,    45,   115,   680,   114,   128,    26,   130,   130,    29,
      30,    29,   128,   135,   129,   115,   129,   115,    38,   460,
     633,    41,    42,   119,   120,    45,    46,   468,    46,   129,
     131,   129,   473,    26,    82,   476,    29,    30,   715,   128,
     481,   130,   483,    26,   129,    38,    29,    30,    41,    42,
     113,   136,    45,    46,   131,    38,   112,   129,    41,    42,
     133,   129,    45,   129,   136,   129,   129,   128,   136,   132,
     136,   129,   136,   750,   751,   467,    14,   129,   136,   471,
     115,   522,   474,   475,   132,   129,   113,   135,   129,   129,
     531,   132,   484,   129,   129,    26,   136,   489,    29,    30,
     136,   129,   129,   745,   115,   132,   129,    38,   136,   129,
      41,    42,   504,   136,    45,    46,   508,   129,   129,   511,
     129,   513,    26,   515,   136,    29,    30,   519,   520,   131,
     132,    10,    11,   129,    38,    14,   129,    41,    42,    14,
     136,    45,    46,   136,   129,   537,   129,   129,   129,   132,
     827,   136,   129,   136,   136,   136,   129,   129,    26,   136,
     552,    29,    30,   136,   136,   122,   123,   368,    26,   129,
      38,    29,    30,    41,    42,   129,   136,    45,   129,   571,
      38,   130,   136,    41,    42,   136,   135,    45,    67,    68,
      69,   130,   129,   634,    14,   636,   135,   589,   129,   136,
     129,   642,   594,   595,   596,   136,   883,   884,   129,   129,
     602,   603,   132,   605,   606,   136,   608,   609,   610,   611,
     612,   613,   614,   615,   129,   129,   129,   129,   669,   129,
     129,   136,   136,   136,   136,   129,   136,   136,   880,   129,
     970,   129,   136,   973,   130,   129,   136,    26,   136,   135,
      29,    30,   136,   129,   131,   132,   648,   649,   650,    38,
     136,   129,    41,    42,   132,   657,    45,   132,   136,   134,
     129,   129,    10,    11,   132,   667,    14,   136,   670,   671,
     129,   129,   129,   130,   129,   677,    26,   136,   136,    29,
      30,   136,   684,   129,    14,    10,   129,   129,    38,    14,
     136,    41,    42,   136,   136,    45,    46,   129,   129,   701,
      83,    84,    85,    86,   136,   136,   129,   129,   710,    10,
      93,    94,    95,   136,   136,   717,   718,   719,   129,    67,
      68,    69,  1062,    10,   535,   536,   728,    14,   539,   540,
     541,   115,   129,   129,   117,   118,   132,   129,   740,   136,
     129,   552,   129,   132,   136,   132,   115,   136,    87,    88,
      89,    90,    91,    92,   565,   566,   567,   132,   129,   134,
      99,   132,   813,   765,   766,   767,   768,   769,   770,    10,
      11,    12,   111,   824,   129,     3,     4,     5,     6,   129,
      71,   136,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     7,     8,     9,   100,   101,   102,   103,
     104,   105,   106,    31,    32,    55,   129,    10,   810,   132,
      13,   115,    15,    16,    98,    43,   100,   101,   102,   103,
     104,   105,   106,    51,    52,    53,   130,   129,    56,   135,
     132,   115,   108,   109,   110,   131,   132,    65,    66,   890,
     842,   122,   123,    71,   128,   135,   130,   130,   124,   125,
     126,   236,   237,   238,   856,   857,   858,   859,   860,   861,
     862,   863,   100,   101,   102,   103,   104,   105,   106,   871,
     135,   129,   100,   101,   132,   131,   132,   115,   929,   132,
      10,   134,   933,    13,    14,    15,    16,   889,   116,   131,
     132,   702,   130,   131,   132,   123,   132,   132,   134,   134,
     131,   132,   130,   131,   136,   133,   132,   135,   134,   137,
     131,   132,   131,   132,   131,   132,   131,   132,   729,   129,
     922,   129,    14,    27,    28,   927,   928,   131,   132,   129,
      22,   129,    24,    25,   131,   132,   131,   132,   131,   132,
     129,    33,    34,   118,   946,   131,   132,   129,   950,   951,
     952,    43,    44,   131,   132,   131,   132,   131,   132,    51,
      52,    53,   129,    41,    42,   967,   131,   132,    60,   131,
     132,   131,   132,   136,   971,   130,   978,   131,   132,   131,
     132,   983,   219,   220,   221,   222,   223,   224,    14,   226,
     227,   228,   229,   230,   231,   232,   233,   130,  1000,  1001,
     131,   132,    80,    81,    14,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,  1020,   131,
     132,    99,   833,   131,   132,   115,   837,   838,   839,   107,
     108,   109,   110,   111,   112,    14,   114,  1039,   129,   117,
     118,   119,   120,   121,   122,   129,   124,   125,   126,    14,
     128,  1053,    17,   115,   865,    10,  1058,    22,   869,    24,
      25,   131,   132,   131,   132,    27,    28,    14,    33,    34,
     131,   132,   131,   132,   131,   132,    14,  1079,    43,    44,
     131,   132,  1084,  1085,  1086,    27,    51,    52,    53,   131,
     132,   131,   132,   131,   132,    14,   131,   132,    27,    28,
     131,   132,    10,    22,   129,    24,    25,   131,   132,   131,
     132,    27,    28,   129,    33,    34,  1118,  1119,  1120,  1121,
    1122,  1123,  1124,  1125,    43,    44,   129,   938,   939,   131,
     132,   129,    51,    52,    53,  1137,  1138,  1139,     3,     4,
       5,     6,   131,   132,   129,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   131,   132,    44,    45,
     243,   244,   136,   130,   130,   115,    31,    32,    10,   115,
     129,   129,   129,   129,    96,   129,   129,    98,    43,   100,
     101,   102,   103,   104,   105,   106,    51,    52,    53,   129,
     129,    56,   129,   129,   115,   129,   129,   129,   129,   129,
      65,    66,   129,   121,   111,   130,    71,   128,    14,   130,
     130,   115,   115,   115,    14,   130,   129,   118,   129,   136,
      79,   115,   118,   129,  1035,   135,   115,   136,  1039,   129,
     129,     3,     4,     5,     6,   100,   101,   129,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   129,
     129,   116,   129,   129,   129,   129,   129,   129,   123,    31,
      32,   129,  1073,   129,   118,   130,   131,   129,   133,   129,
     135,    43,   137,   136,   129,    14,   129,    14,   129,    51,
      52,    53,   129,   129,    56,   134,   134,    79,    14,   129,
     129,   129,   129,    65,    66,    10,    10,    10,    14,    71,
     115,    10,    10,   129,   129,    27,   129,     3,     4,     5,
       6,    58,    14,    14,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,  1136,    14,   129,   100,   101,
     129,   136,   129,   129,   129,    31,    32,   131,    79,   130,
     129,   129,   129,    79,   116,   129,   136,    43,   129,   129,
     129,   123,     7,     7,     7,    51,    52,    53,   130,   131,
      56,   133,   115,   135,   115,   137,   129,    14,    10,    65,
      66,   115,    10,   131,    74,    71,   136,    14,    14,   129,
      14,    14,   131,   129,   129,    14,   129,   129,   136,   129,
      14,    14,    14,  1079,   129,   129,   129,   135,   129,   129,
     129,   129,   129,   580,   100,   101,   129,   131,   129,   129,
     129,   136,   129,   508,   235,   242,    -1,   129,   129,   241,
     116,    -1,    -1,    -1,    -1,    -1,   136,   123,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      61,    62,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,   129,   130,
      -1,    -1,   133,    -1,   135,   136,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    -1,    -1,    -1,    -1,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    61,    62,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,   129,   130,    -1,    -1,   133,    -1,   135,   136,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    -1,    61,    62,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,   129,   130,    56,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,   129,   130,    56,    -1,   133,    -1,   135,   136,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,   129,   130,    56,    -1,
     133,    -1,   135,   136,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,   129,   130,    -1,    -1,   133,    -1,   135,   136,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    61,    62,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    51,    52,    53,   129,   130,    56,    -1,   133,
      -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    43,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,    53,
     129,   130,    56,    -1,   133,    -1,   135,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    51,    52,    53,   129,   130,    56,    -1,   133,
      -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    43,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,    53,
     129,   130,    56,    -1,   133,    -1,   135,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    43,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    51,    52,    53,   130,   131,    56,   133,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,   134,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    43,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    51,    52,    53,   130,   131,    56,
     133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,   124,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    43,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    51,    52,    53,   130,   131,
      56,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    23,   100,   101,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    43,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    51,    52,    53,   130,   131,    56,   133,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,
     130,    56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    51,    52,    53,    -1,   130,    56,    -1,   133,    -1,
     135,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    43,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    51,    52,    53,
     130,   131,    56,   133,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,   133,
     134,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      43,    -1,    -1,    -1,   123,    -1,    -1,    -1,    51,    52,
      53,   130,   131,    56,   133,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    43,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    51,    52,    53,   130,   131,    56,
     133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    43,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      51,    52,    53,   130,   131,    56,   133,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    43,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    51,    52,    53,   130,
     131,    56,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    43,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    51,    52,    53,   130,   131,    56,   133,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      43,    -1,    -1,    -1,   123,    -1,    -1,    -1,    51,    52,
      53,   130,   131,    56,   133,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,   133,    -1,   135
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,   140,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      61,    62,    65,    66,    71,    75,   100,   101,   116,   123,
     129,   130,   133,   135,   141,   142,   144,   145,   146,   147,
     148,   150,   151,   153,   154,   157,   158,   161,   166,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   183,   184,   186,    98,   100,   101,   102,   103,
     104,   105,   106,   115,   128,   130,   115,   133,   117,   135,
     115,    11,   128,   130,    10,    11,    12,    13,    14,    31,
     130,   166,   179,    14,    10,    11,   129,   130,   168,   185,
      10,    11,    12,    14,    31,    36,   130,   130,   168,   130,
     168,   130,   168,   130,   168,   168,   175,    14,   166,   166,
     168,   129,   168,   129,   168,    10,    14,    14,   168,    12,
      12,   130,   168,   130,   168,   166,   130,   166,    26,    45,
     129,    26,    45,   129,   135,   167,   167,   135,   167,   168,
     166,   166,   168,    14,    14,    14,    31,   179,    14,    10,
      10,   179,   179,    10,   131,   168,   181,   134,   168,    74,
     140,    26,    29,    30,    38,    41,    42,    45,    46,   129,
      26,    29,    30,    38,    41,    42,    45,    46,   129,    83,
      84,    85,    86,    93,    94,    95,   117,   118,    87,    88,
      89,    90,    91,    92,    99,   111,   119,   120,   107,    80,
      81,   121,   112,   122,   123,   108,   109,   110,   124,   125,
     126,   114,   128,   128,    14,   168,   168,   168,   168,   168,
     168,    14,   131,   185,   130,   168,   124,   168,    14,   168,
     168,   129,    14,    17,    60,    14,   131,   137,   163,   185,
     133,   117,   135,   129,   102,   103,   104,   105,   106,   115,
     115,   131,   168,   185,   129,   132,   115,   129,   115,   129,
     115,   129,    10,    11,    12,    14,    10,    14,    10,   181,
     168,    26,    29,    30,    38,    41,    42,    45,   129,   132,
     168,    26,    45,   129,   132,   168,   166,   168,   166,    82,
     132,   166,    80,    81,   130,   135,   155,   129,    26,    45,
     129,    26,    45,   129,   115,   115,    71,   135,   117,   135,
     117,   135,   168,   166,   168,   166,    29,    46,    23,   168,
     168,   168,   168,   168,   168,   168,   168,   168,    55,   159,
     135,   130,    14,    64,   135,   131,   132,   131,   132,   132,
     134,   136,    10,    11,    12,    13,    24,    25,    33,    34,
      49,    50,   136,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     171,   171,   171,   171,   171,   171,   169,   171,   171,   171,
     171,   171,   171,   171,   171,     7,     8,     9,   172,   174,
     174,   174,   175,   175,   176,   177,   178,   178,   179,   179,
     179,   179,   179,   179,   179,    14,    22,    24,    25,    33,
      34,    43,    44,    51,    52,    53,   182,    17,   182,   129,
     130,   129,   129,   129,   129,   129,   129,   115,   130,   131,
     168,   113,   129,   132,   122,   123,   132,   134,   118,   136,
     129,   130,    14,   130,    79,    14,   131,   132,   131,   132,
     168,    14,   168,   168,   168,   168,   168,   168,   168,   168,
     129,   131,   168,   168,   130,   168,     5,    14,   130,   164,
     168,   115,   129,   115,   129,   115,   129,   130,   155,   115,
     115,   131,   132,   168,   168,   168,   168,   168,   168,   168,
     185,   132,   168,   168,   185,   131,    27,    28,   143,   131,
      10,   185,   175,   175,    10,    11,    14,    67,    68,    69,
     131,   165,   140,   168,   168,   168,   168,   168,   168,   129,
     152,    14,   168,    14,   168,   131,    27,   131,   168,   168,
      10,   129,   129,   129,   129,   129,   136,   166,    14,    22,
     135,   160,   131,   165,   130,   130,   162,   131,   185,   115,
      10,   134,   185,   128,   133,   135,   115,   168,   168,   168,
     136,   168,    26,    45,   136,    26,    45,   136,    26,    29,
      30,    38,    41,    42,    45,    46,   136,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,    96,   130,    14,   130,   131,   185,   168,
     131,   185,   132,   168,   185,   168,   168,   185,   115,   115,
     115,   131,   185,   131,   163,   185,   168,   130,    14,   137,
     163,   134,   118,   136,   129,   129,   168,   113,   129,   132,
      79,    79,   164,   129,   132,   129,   168,   130,   168,   168,
     131,   165,   168,   168,   115,   185,   129,   129,   129,   129,
     129,   129,   129,   129,   185,   129,   129,   129,   166,   142,
     166,   130,   168,   166,   166,    82,   166,    82,    82,    22,
     115,    10,    72,    73,   155,   131,   132,    24,    25,    34,
     136,   168,   129,   129,   129,   129,   129,   129,    39,    40,
     136,   118,   136,   118,   136,   166,   166,   166,   129,   129,
     115,   168,    14,    14,   160,    58,    59,    62,   136,   155,
     131,   131,   165,   165,    76,    77,    78,   136,   131,   168,
     134,    14,   168,   168,   168,    26,    30,    38,    41,    42,
      45,   136,   136,   136,   136,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   169,   131,   185,
     131,   163,   185,   129,   131,   129,   131,   185,   129,   129,
     134,   134,   134,   168,   168,   168,   131,   131,   131,   168,
      79,    14,   131,   132,   168,   185,   168,   168,   131,     5,
      14,   129,   168,   129,   132,   129,   155,   131,   129,   129,
     168,   131,   131,    27,   143,   168,   166,    10,    10,    10,
      14,   168,   115,    10,    10,   155,    10,    11,    14,    67,
      68,    69,   168,   168,   136,   168,    26,    29,    30,    38,
      41,    42,    45,    46,   136,   168,   166,   129,   129,    27,
     168,   129,   136,    60,   182,    10,    13,    14,    15,    16,
      58,   155,   155,   131,   131,    14,    14,    14,   129,   115,
     130,   134,   136,   136,   168,   168,   168,   168,   168,   168,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   131,   131,   131,   129,   131,   129,   129,   129,
     131,   168,   130,   185,   129,   129,   129,    79,    79,   132,
     185,   155,   129,   132,   129,   129,   142,   166,   131,    27,
     143,   166,   166,   166,   168,    22,   115,    10,    72,    73,
      26,    30,    45,   136,   136,   136,   168,   168,   168,   168,
     168,   168,   168,   168,   166,   136,   166,   129,   149,   168,
     130,   135,   156,   130,   156,    14,    22,    70,   115,   129,
      14,    22,    70,   115,   129,    10,    13,    15,    16,   129,
     129,   182,   155,   155,     7,     7,     7,   168,   131,   185,
     115,   115,   136,   136,   136,   136,   136,   136,   129,   168,
     131,   168,   168,   185,   129,   185,   166,   166,    14,   168,
     115,    10,    10,   168,   168,   168,   136,   136,   136,   136,
     136,   136,   136,   136,   168,   131,   131,   165,    74,   140,
     131,   165,    14,    14,   129,   168,    14,    14,   129,   168,
      14,    22,    70,   115,   129,    14,    22,    70,   115,   129,
     129,   129,   130,   156,   136,   136,   131,   168,   168,   131,
     129,   131,   129,    27,   143,   168,   136,   136,   136,   129,
     166,   156,   131,   136,    24,    25,    34,   136,   168,   156,
     131,   129,   129,   129,   129,   129,   129,    14,    14,   129,
     168,    14,    14,   129,   168,   131,   165,   136,   136,   136,
     129,   166,   149,   156,   168,   168,   136,   168,    26,    29,
      30,    38,    41,    42,    45,    46,   136,   156,   129,   129,
     129,   129,   129,   129,   156,   131,   131,    26,    30,    45,
     136,   136,   136,   168,   168,   168,   168,   168,   168,   168,
     168,   156,   166,   168,   168,   168,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   140,   140,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   142,   142,   142,   142,   142,   142,   142,
     142,   143,   143,   143,   143,   143,   143,   144,   144,   145,
     145,   145,   145,   146,   146,   147,   147,   147,   148,   148,
     148,   149,   150,   150,   150,   150,   150,   150,   151,   151,
     152,   152,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   154,   154,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     157,   158,   159,   159,   159,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   161,
     162,   162,   162,   162,   163,   163,   163,   163,   164,   164,
     164,   164,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   167,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   169,   169,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   171,   171,
     172,   172,   172,   173,   173,   174,   174,   174,   175,   175,
     176,   176,   177,   177,   177,   178,   178,   178,   178,   178,
     178,   178,   179,   179,   179,   179,   179,   180,   180,   181,
     181,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   183,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   185,   185,   185,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     7,     9,     4,     6,     6,     8,
       5,     5,     7,     6,     6,     8,    10,     6,     4,     4,
       4,     3,     5,     5,     6,     6,     4,     2,     5,     4,
       3,     3,     3,     5,     7,     3,     5,     7,     3,     3,
       2,     5,     5,     3,     2,     5,     5,     4,     7,     6,
       4,     6,     4,     2,     4,     4,     4,     4,     4,     4,
       4,     4,     7,     7,     7,     6,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     4,     4,     4,     4,     4,     2,
       1,     1,     1,     1,     1,     2,     4,     2,     1,     1,
       1,     1,     1,     2,     2,     4,     4,     4,     4,     1,
       1,     1,     1,     5,     7,     7,     3,     5,     5,     4,
       6,     3,     5,     5,     7,     4,     6,     5,     3,     5,
       7,     3,     5,     5,     3,     2,     5,     5,     2,    12,
       9,     1,     7,     7,     5,     7,     5,     3,     5,     7,
       0,     4,     6,     5,     3,     7,     6,     4,     7,     6,
       7,     6,     5,     6,     5,     3,     4,     5,     4,     5,
       5,     6,     6,     6,     6,     6,     6,     6,     6,     7,
       7,     7,     3,     3,     4,     5,     4,     5,     5,     6,
       6,     6,     6,     6,     6,     6,     6,     7,     7,     7,
       6,     5,     0,     3,     3,     0,     4,     4,     4,     5,
       5,     4,     5,     5,     6,     6,     7,     7,     6,     6,
       6,     6,     5,     5,     6,     6,     7,     7,     7,     7,
       7,     6,     4,     7,     6,     4,     8,     7,     5,     5,
       0,     4,     4,     4,     3,     5,     5,     7,     3,     3,
       5,     5,     1,     3,     5,     1,     3,     2,     3,     3,
       3,     4,     5,     5,     3,     5,     4,     6,     1,     3,
       1,     3,     1,     3,     3,     3,     4,     6,     6,     6,
       6,     6,     6,     6,     6,     5,     7,     7,     7,     7,
       7,     7,     5,     9,     8,     8,     6,     9,     9,     5,
       5,     4,     4,     4,     6,     6,     6,     6,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     5,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     1,     2,     2,     2,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     4,     6,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     4,
       6,     6,     6,     1,     4,     4,     5,     5,     1,     1,
       1,     1,     2,     3,     4,     5,     2,     3,     4,     5,
       1,     2
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
#line 456 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3078 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 466 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3084 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 467 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3090 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 471 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3096 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 473 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3102 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 475 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3108 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 477 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3116 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3122 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3128 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3134 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 487 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3140 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 489 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3148 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 493 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3155 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 496 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3163 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 500 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3169 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 502 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3177 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 506 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3184 "raku.tab.c"
    break;

  case 19: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 509 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3192 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 513 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3198 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 515 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3204 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 517 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3210 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 519 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3216 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 521 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3222 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 523 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3230 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 527 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3238 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 531 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3244 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 533 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3250 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 535 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3256 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 537 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3262 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE IDENT ';'  */
#line 539 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3268 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3274 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 543 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3280 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 545 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3286 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 547 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3292 "raku.tab.c"
    break;

  case 36: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 549 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3304 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP ';'  */
#line 557 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3310 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 559 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3316 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP '(' ')' ';'  */
#line 561 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3322 "raku.tab.c"
    break;

  case 40: /* stmt: TESTOP arg_list ';'  */
#line 563 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3328 "raku.tab.c"
    break;

  case 41: /* stmt: IDENT VAR_ARRAY ';'  */
#line 565 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3334 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ';'  */
#line 567 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3340 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 569 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3347 "raku.tab.c"
    break;

  case 44: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 572 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3354 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ';'  */
#line 575 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3360 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 577 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3367 "raku.tab.c"
    break;

  case 47: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 580 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3374 "raku.tab.c"
    break;

  case 48: /* stmt: KW_TAKE expr ';'  */
#line 583 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3380 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr ';'  */
#line 585 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3386 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN ';'  */
#line 587 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3392 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 589 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3399 "raku.tab.c"
    break;

  case 52: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 592 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3406 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT expr ';'  */
#line 595 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3412 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT ';'  */
#line 597 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3419 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 600 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3426 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 603 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3433 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 606 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3439 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 608 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3448 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 613 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3456 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 617 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3464 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 621 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3473 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 626 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3481 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall ';'  */
#line 629 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3487 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3493 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 633 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3499 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 635 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3505 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 637 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3511 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 639 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3518 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 642 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3524 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 644 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3530 "raku.tab.c"
    break;

  case 71: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 646 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3536 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3543 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3550 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 654 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3557 "raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 657 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3564 "raku.tab.c"
    break;

  case 76: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 660 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3571 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_IF expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3577 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_UNLESS expr ';'  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3583 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_WHILE expr ';'  */
#line 667 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3589 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_UNTIL expr ';'  */
#line 669 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3595 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_FOR expr ';'  */
#line 671 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3602 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITH expr ';'  */
#line 674 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3608 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WITHOUT expr ';'  */
#line 676 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3614 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_GIVEN expr ';'  */
#line 678 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3620 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 680 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3627 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 683 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3634 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 686 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3642 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 690 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3649 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 693 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3655 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 695 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3661 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 697 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3667 "raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 699 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3674 "raku.tab.c"
    break;

  case 93: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 702 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3681 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 705 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3688 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 708 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3695 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 711 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3702 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 714 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3709 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 717 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3716 "raku.tab.c"
    break;

  case 99: /* stmt: expr ';'  */
#line 719 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3722 "raku.tab.c"
    break;

  case 100: /* stmt: ';'  */
#line 720 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3728 "raku.tab.c"
    break;

  case 101: /* stmt: if_stmt  */
#line 721 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3734 "raku.tab.c"
    break;

  case 102: /* stmt: while_stmt  */
#line 722 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3740 "raku.tab.c"
    break;

  case 103: /* stmt: for_stmt  */
#line 723 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3746 "raku.tab.c"
    break;

  case 104: /* stmt: given_stmt  */
#line 724 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3752 "raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block  */
#line 726 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3758 "raku.tab.c"
    break;

  case 106: /* stmt: KW_TRY block KW_CATCH block  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3764 "raku.tab.c"
    break;

  case 107: /* stmt: KW_CATCH block  */
#line 730 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3770 "raku.tab.c"
    break;

  case 108: /* stmt: block  */
#line 732 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3776 "raku.tab.c"
    break;

  case 109: /* stmt: unless_stmt  */
#line 733 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3782 "raku.tab.c"
    break;

  case 110: /* stmt: until_stmt  */
#line 734 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3788 "raku.tab.c"
    break;

  case 111: /* stmt: repeat_stmt  */
#line 735 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3794 "raku.tab.c"
    break;

  case 112: /* stmt: loop_stmt  */
#line 736 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3800 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST ';'  */
#line 737 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3806 "raku.tab.c"
    break;

  case 114: /* stmt: KW_NEXT ';'  */
#line 738 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3812 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_IF expr ';'  */
#line 740 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3818 "raku.tab.c"
    break;

  case 116: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 742 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3824 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 744 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3830 "raku.tab.c"
    break;

  case 118: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 746 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3836 "raku.tab.c"
    break;

  case 119: /* stmt: sub_decl  */
#line 747 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3842 "raku.tab.c"
    break;

  case 120: /* stmt: class_decl  */
#line 748 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3848 "raku.tab.c"
    break;

  case 121: /* stmt: role_decl  */
#line 749 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3854 "raku.tab.c"
    break;

  case 122: /* stmt: grammar_decl  */
#line 750 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3860 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block  */
#line 754 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3866 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3872 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3878 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block  */
#line 760 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3884 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 762 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3890 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 764 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3896 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block elsif_tail  */
#line 766 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3902 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3908 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block  */
#line 772 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3914 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3920 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3926 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 778 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3932 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 780 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3938 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 782 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3944 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 786 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3950 "raku.tab.c"
    break;

  case 138: /* while_stmt: KW_WHILE expr block  */
#line 788 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3956 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3962 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 794 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3968 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3974 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3980 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3986 "raku.tab.c"
    break;

  case 144: /* until_stmt: KW_UNTIL expr block  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3992 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3998 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4004 "raku.tab.c"
    break;

  case 147: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4010 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP block  */
#line 816 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4016 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 818 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4022 "raku.tab.c"
    break;

  case 150: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 820 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4028 "raku.tab.c"
    break;

  case 151: /* loop_incr: expr  */
#line 823 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4034 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 827 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4044 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 833 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4054 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 839 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4063 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 844 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4073 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 850 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4082 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr block  */
#line 855 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4089 "raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 860 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4101 "raku.tab.c"
    break;

  case 159: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 868 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4114 "raku.tab.c"
    break;

  case 160: /* when_list: %empty  */
#line 878 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4120 "raku.tab.c"
    break;

  case 161: /* when_list: when_list KW_WHEN expr block  */
#line 880 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4128 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 886 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4140 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 894 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4150 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_SUB IDENT sub_body  */
#line 900 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4160 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 906 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4172 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 914 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4182 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 920 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4192 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 926 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 935 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4216 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 942 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 951 "raku.y"
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

  case 172: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 960 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4253 "raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 969 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4264 "raku.tab.c"
    break;

  case 174: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 976 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4273 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list '}'  */
#line 982 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4279 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr '}'  */
#line 984 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4286 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 987 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4293 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 990 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4299 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 992 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4305 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 994 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4311 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 996 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4317 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 998 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4323 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1000 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4329 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1002 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4335 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1004 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4342 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1007 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4348 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1009 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4354 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1011 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4360 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1013 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4367 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1016 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4374 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1019 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4382 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list '}'  */
#line 1024 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4388 "raku.tab.c"
    break;

  case 193: /* method_body: '{' YADA '}'  */
#line 1025 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4394 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr '}'  */
#line 1027 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4401 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1030 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4408 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1033 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4414 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1035 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4420 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1037 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4426 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4432 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1041 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4438 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1043 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4444 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1045 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4450 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1047 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4457 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1050 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4463 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1052 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4469 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1054 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1056 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4482 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1059 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4489 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1062 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4497 "raku.tab.c"
    break;

  case 210: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1068 "raku.y"
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
#line 4515 "raku.tab.c"
    break;

  case 211: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1084 "raku.y"
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
#line 4532 "raku.tab.c"
    break;

  case 212: /* is_clauses: %empty  */
#line 1098 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4538 "raku.tab.c"
    break;

  case 213: /* is_clauses: is_clauses IDENT IDENT  */
#line 1100 "raku.y"
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
#line 4554 "raku.tab.c"
    break;

  case 214: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1112 "raku.y"
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
#line 4570 "raku.tab.c"
    break;

  case 215: /* class_body_list: %empty  */
#line 1125 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4576 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1127 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4583 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1130 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4590 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1133 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4597 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1136 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4604 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1139 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4611 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1142 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4618 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1145 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4625 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1148 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4632 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1151 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4639 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1154 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4646 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1157 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4653 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1160 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4660 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1163 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4671 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1170 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4682 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1177 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4693 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1184 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4704 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1191 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4712 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1195 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4720 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1199 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4728 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1203 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4736 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1207 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4747 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1214 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4758 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1221 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4769 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1228 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4780 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1235 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4794 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1245 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4806 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1253 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4818 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1261 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4831 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1270 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4842 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1277 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4853 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1284 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4867 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1294 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4879 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1302 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4891 "raku.tab.c"
    break;

  case 249: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1312 "raku.y"
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
#line 4908 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: %empty  */
#line 1326 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4914 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1328 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4923 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1333 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4932 "raku.tab.c"
    break;

  case 253: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1338 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4941 "raku.tab.c"
    break;

  case 254: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1345 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4949 "raku.tab.c"
    break;

  case 255: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1349 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4957 "raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1353 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4965 "raku.tab.c"
    break;

  case 257: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1357 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4973 "raku.tab.c"
    break;

  case 258: /* pair_list: IDENT OP_FATARROW expr  */
#line 1363 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4979 "raku.tab.c"
    break;

  case 259: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1365 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4985 "raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1367 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4991 "raku.tab.c"
    break;

  case 261: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1369 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4997 "raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR  */
#line 1372 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5003 "raku.tab.c"
    break;

  case 263: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1373 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5009 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1374 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5015 "raku.tab.c"
    break;

  case 265: /* param_list: VAR_ARRAY  */
#line 1375 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5021 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' VAR_ARRAY  */
#line 1376 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5027 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT VAR_SCALAR  */
#line 1377 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5033 "raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1378 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5039 "raku.tab.c"
    break;

  case 269: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1379 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5045 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR  */
#line 1380 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5051 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1381 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5057 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1382 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5063 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1383 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5069 "raku.tab.c"
    break;

  case 274: /* param_list: VAR_SCALAR '=' expr  */
#line 1384 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5075 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1385 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5081 "raku.tab.c"
    break;

  case 276: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1386 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5087 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1387 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5093 "raku.tab.c"
    break;

  case 278: /* param_list: SLURPY_POS  */
#line 1388 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5099 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' SLURPY_POS  */
#line 1389 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5105 "raku.tab.c"
    break;

  case 280: /* param_list: SLURPY_LOL  */
#line 1390 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5111 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' SLURPY_LOL  */
#line 1391 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5117 "raku.tab.c"
    break;

  case 282: /* param_list: SLURPY_NAMED  */
#line 1392 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5123 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1393 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5129 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list '}'  */
#line 1396 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5135 "raku.tab.c"
    break;

  case 285: /* block: '{' YADA '}'  */
#line 1397 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5141 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr '}'  */
#line 1399 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5147 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1401 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5153 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1403 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5159 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1405 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5165 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1407 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5171 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1409 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5178 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1412 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5184 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1414 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5190 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1416 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5196 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1418 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5202 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1420 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5209 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1423 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5216 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1426 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5224 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1430 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5231 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1433 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5238 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1436 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5245 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1439 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5251 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1441 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5259 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1445 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5266 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1448 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5273 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1451 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5280 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1454 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5287 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1457 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5294 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1460 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5300 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1462 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5306 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1464 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5312 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_LAST '}'  */
#line 1466 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5318 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1468 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5324 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1470 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5330 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1472 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5336 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1474 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1476 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5348 "raku.tab.c"
    break;

  case 318: /* closure: '{' expr '}'  */
#line 1479 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5354 "raku.tab.c"
    break;

  case 319: /* expr: VAR_SCALAR '=' expr  */
#line 1482 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5360 "raku.tab.c"
    break;

  case 320: /* expr: VAR_ARRAY '=' expr  */
#line 1483 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5366 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1484 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5372 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1485 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5378 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1486 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5384 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1487 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5390 "raku.tab.c"
    break;

  case 325: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1488 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5396 "raku.tab.c"
    break;

  case 326: /* expr: KW_GATHER block  */
#line 1489 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5406 "raku.tab.c"
    break;

  case 327: /* expr: tern_expr  */
#line 1494 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5412 "raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1498 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5418 "raku.tab.c"
    break;

  case 329: /* tern_expr: cmp_expr  */
#line 1499 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5424 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1502 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5430 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1503 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5436 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1504 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5442 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1505 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5448 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1506 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5454 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1507 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5460 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1508 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5466 "raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1509 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5472 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1510 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5478 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1511 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5484 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1512 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5490 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1513 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5496 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1514 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5502 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1515 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5508 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1517 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5518 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5528 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1529 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5538 "raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr  */
#line 1534 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5544 "raku.tab.c"
    break;

  case 348: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1537 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5550 "raku.tab.c"
    break;

  case 349: /* divis_expr: jct_expr  */
#line 1538 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5556 "raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '|' range_expr  */
#line 1541 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5562 "raku.tab.c"
    break;

  case 351: /* jct_expr: jct_expr '&' range_expr  */
#line 1542 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5568 "raku.tab.c"
    break;

  case 352: /* jct_expr: dor_expr  */
#line 1543 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5574 "raku.tab.c"
    break;

  case 353: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1547 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5580 "raku.tab.c"
    break;

  case 354: /* dor_expr: range_expr  */
#line 1548 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5586 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1551 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5592 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1552 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5598 "raku.tab.c"
    break;

  case 357: /* range_expr: add_expr  */
#line 1553 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5604 "raku.tab.c"
    break;

  case 358: /* add_expr: add_expr '~' repl_expr  */
#line 1556 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5610 "raku.tab.c"
    break;

  case 359: /* add_expr: repl_expr  */
#line 1557 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5616 "raku.tab.c"
    break;

  case 360: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1560 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5622 "raku.tab.c"
    break;

  case 361: /* repl_expr: addsub_expr  */
#line 1561 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5628 "raku.tab.c"
    break;

  case 362: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1564 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5634 "raku.tab.c"
    break;

  case 363: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1565 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5640 "raku.tab.c"
    break;

  case 364: /* addsub_expr: mul_expr  */
#line 1566 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5646 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr '*' unary_expr  */
#line 1569 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5652 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '/' unary_expr  */
#line 1570 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5658 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '%' unary_expr  */
#line 1571 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5664 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1572 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5670 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1574 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5676 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1576 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5682 "raku.tab.c"
    break;

  case 371: /* mul_expr: unary_expr  */
#line 1577 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5688 "raku.tab.c"
    break;

  case 372: /* unary_expr: '-' unary_expr  */
#line 1580 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5694 "raku.tab.c"
    break;

  case 373: /* unary_expr: '!' unary_expr  */
#line 1581 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5700 "raku.tab.c"
    break;

  case 374: /* unary_expr: CARET unary_expr  */
#line 1582 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5706 "raku.tab.c"
    break;

  case 375: /* unary_expr: OP_REDUCE unary_expr  */
#line 1584 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5715 "raku.tab.c"
    break;

  case 376: /* unary_expr: pow_expr  */
#line 1588 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5721 "raku.tab.c"
    break;

  case 377: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1591 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5727 "raku.tab.c"
    break;

  case 378: /* pow_expr: postfix_expr  */
#line 1592 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5733 "raku.tab.c"
    break;

  case 379: /* scalar_list: VAR_SCALAR  */
#line 1595 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5739 "raku.tab.c"
    break;

  case 380: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1596 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5745 "raku.tab.c"
    break;

  case 381: /* meth_name: IDENT  */
#line 1599 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5751 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_SORT  */
#line 1600 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5757 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_MAP  */
#line 1601 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5763 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_GREP  */
#line 1602 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5769 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_SAY  */
#line 1603 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5775 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_PRINT  */
#line 1604 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5781 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_TAKE  */
#line 1605 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5787 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_RETURN  */
#line 1606 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5793 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_EXISTS  */
#line 1607 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5799 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_DELETE  */
#line 1608 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5805 "raku.tab.c"
    break;

  case 391: /* meth_name: TESTOP  */
#line 1609 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5811 "raku.tab.c"
    break;

  case 392: /* postfix_expr: call_expr  */
#line 1611 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5817 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '(' arg_list ')'  */
#line 1614 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5826 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '(' ')'  */
#line 1618 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5832 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1620 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5838 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1622 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5844 "raku.tab.c"
    break;

  case 397: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1624 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5853 "raku.tab.c"
    break;

  case 398: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1629 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5859 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5869 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1637 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5879 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1643 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5887 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' KW_NEW  */
#line 1647 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5895 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' IDENT  */
#line 1651 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5904 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1656 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5915 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1663 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5924 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' CARET IDENT  */
#line 1668 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5934 "raku.tab.c"
    break;

  case 407: /* call_expr: atom '.' CARET IDENT  */
#line 1674 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5944 "raku.tab.c"
    break;

  case 408: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1680 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5955 "raku.tab.c"
    break;

  case 409: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1687 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5966 "raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1694 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5975 "raku.tab.c"
    break;

  case 411: /* call_expr: atom '.' meth_name  */
#line 1699 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5984 "raku.tab.c"
    break;

  case 412: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1704 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5995 "raku.tab.c"
    break;

  case 413: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1711 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6004 "raku.tab.c"
    break;

  case 414: /* call_expr: call_expr '.' meth_name  */
#line 1716 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6013 "raku.tab.c"
    break;

  case 415: /* call_expr: KW_DIE expr  */
#line 1721 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6019 "raku.tab.c"
    break;

  case 416: /* call_expr: KW_MAP closure expr  */
#line 1723 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6025 "raku.tab.c"
    break;

  case 417: /* call_expr: KW_GREP closure expr  */
#line 1725 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6031 "raku.tab.c"
    break;

  case 418: /* call_expr: KW_SORT expr  */
#line 1727 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6037 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_SORT closure expr  */
#line 1729 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6043 "raku.tab.c"
    break;

  case 420: /* call_expr: atom  */
#line 1730 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6049 "raku.tab.c"
    break;

  case 421: /* arg_list: expr  */
#line 1733 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6055 "raku.tab.c"
    break;

  case 422: /* arg_list: arg_list ',' expr  */
#line 1734 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6061 "raku.tab.c"
    break;

  case 423: /* arg_list: arg_list ','  */
#line 1735 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6067 "raku.tab.c"
    break;

  case 424: /* atom: LIT_INT  */
#line 1738 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6073 "raku.tab.c"
    break;

  case 425: /* atom: LIT_FLOAT  */
#line 1739 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6079 "raku.tab.c"
    break;

  case 426: /* atom: LIT_STR  */
#line 1740 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6085 "raku.tab.c"
    break;

  case 427: /* atom: WORDLIST  */
#line 1742 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6098 "raku.tab.c"
    break;

  case 428: /* atom: LIT_INTERP_STR  */
#line 1750 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6104 "raku.tab.c"
    break;

  case 429: /* atom: VAR_SCALAR  */
#line 1751 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6110 "raku.tab.c"
    break;

  case 430: /* atom: OP_INC VAR_SCALAR  */
#line 1752 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6116 "raku.tab.c"
    break;

  case 431: /* atom: OP_DEC VAR_SCALAR  */
#line 1753 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6122 "raku.tab.c"
    break;

  case 432: /* atom: VAR_SCALAR OP_INC  */
#line 1754 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6128 "raku.tab.c"
    break;

  case 433: /* atom: VAR_SCALAR OP_DEC  */
#line 1755 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6134 "raku.tab.c"
    break;

  case 434: /* atom: VAR_ARRAY  */
#line 1756 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6140 "raku.tab.c"
    break;

  case 435: /* atom: VAR_HASH  */
#line 1757 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6146 "raku.tab.c"
    break;

  case 436: /* atom: VAR_CAPTURE  */
#line 1759 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6154 "raku.tab.c"
    break;

  case 437: /* atom: VAR_FH  */
#line 1763 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6162 "raku.tab.c"
    break;

  case 438: /* atom: VAR_NAMED_CAPTURE  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6169 "raku.tab.c"
    break;

  case 439: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1770 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6175 "raku.tab.c"
    break;

  case 440: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1772 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6181 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1774 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6187 "raku.tab.c"
    break;

  case 442: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1776 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6193 "raku.tab.c"
    break;

  case 443: /* atom: ARR_ALL_SLICE  */
#line 1778 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6199 "raku.tab.c"
    break;

  case 444: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1780 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6205 "raku.tab.c"
    break;

  case 445: /* atom: VAR_HASH '{' expr '}'  */
#line 1782 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6211 "raku.tab.c"
    break;

  case 446: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1784 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6217 "raku.tab.c"
    break;

  case 447: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1786 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6223 "raku.tab.c"
    break;

  case 448: /* atom: IDENT  */
#line 1787 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6229 "raku.tab.c"
    break;

  case 449: /* atom: VAR_TWIGIL  */
#line 1789 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6237 "raku.tab.c"
    break;

  case 450: /* atom: VAR_ARRAY_TWIGIL  */
#line 1793 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6245 "raku.tab.c"
    break;

  case 451: /* atom: VAR_HASH_TWIGIL  */
#line 1797 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6253 "raku.tab.c"
    break;

  case 452: /* atom: '[' ']'  */
#line 1800 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6259 "raku.tab.c"
    break;

  case 453: /* atom: '[' expr ']'  */
#line 1802 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6265 "raku.tab.c"
    break;

  case 454: /* atom: '[' expr ',' ']'  */
#line 1804 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6271 "raku.tab.c"
    break;

  case 455: /* atom: '[' expr ',' arg_list ']'  */
#line 1806 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6278 "raku.tab.c"
    break;

  case 456: /* atom: '(' ')'  */
#line 1808 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6284 "raku.tab.c"
    break;

  case 457: /* atom: '(' expr ')'  */
#line 1809 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6290 "raku.tab.c"
    break;

  case 458: /* atom: '(' expr ',' ')'  */
#line 1811 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6296 "raku.tab.c"
    break;

  case 459: /* atom: '(' expr ',' arg_list ')'  */
#line 1813 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6303 "raku.tab.c"
    break;

  case 460: /* atom: block  */
#line 1815 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6309 "raku.tab.c"
    break;

  case 461: /* atom: KW_SUB block  */
#line 1816 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6315 "raku.tab.c"
    break;


#line 6319 "raku.tab.c"

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

#line 1818 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
