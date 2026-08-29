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
#define YYLAST   4643

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  460
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1163

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
     482,   484,   486,   488,   492,   496,   498,   502,   506,   508,
     510,   512,   514,   516,   520,   524,   526,   528,   530,   532,
     534,   536,   538,   540,   542,   550,   552,   554,   556,   558,
     560,   562,   565,   568,   570,   573,   576,   578,   580,   582,
     585,   588,   590,   593,   596,   599,   601,   606,   610,   614,
     619,   623,   624,   626,   628,   630,   632,   635,   637,   639,
     641,   644,   647,   650,   653,   656,   658,   660,   662,   664,
     667,   669,   671,   673,   676,   679,   683,   686,   688,   690,
     692,   695,   698,   701,   704,   707,   710,   713,   714,   715,
     716,   717,   718,   719,   721,   723,   725,   727,   728,   729,
     730,   731,   732,   733,   735,   737,   739,   741,   742,   743,
     744,   747,   749,   751,   753,   755,   757,   759,   761,   765,
     767,   769,   771,   773,   775,   779,   781,   785,   787,   789,
     791,   795,   797,   801,   803,   805,   809,   811,   813,   817,
     820,   826,   832,   837,   843,   848,   853,   861,   872,   873,
     879,   887,   893,   899,   907,   913,   919,   928,   935,   944,
     953,   962,   969,   976,   977,   980,   983,   985,   987,   989,
     991,   993,   995,   997,  1000,  1002,  1004,  1006,  1009,  1012,
    1018,  1019,  1020,  1023,  1026,  1028,  1030,  1032,  1034,  1036,
    1038,  1040,  1043,  1045,  1047,  1049,  1052,  1055,  1061,  1077,
    1092,  1093,  1105,  1119,  1120,  1123,  1126,  1129,  1132,  1135,
    1138,  1141,  1144,  1147,  1150,  1153,  1156,  1163,  1170,  1177,
    1184,  1188,  1192,  1196,  1200,  1207,  1214,  1221,  1228,  1238,
    1246,  1254,  1263,  1270,  1277,  1287,  1295,  1305,  1320,  1321,
    1326,  1331,  1338,  1342,  1346,  1350,  1356,  1358,  1360,  1362,
    1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,
    1386,  1387,  1390,  1391,  1392,  1394,  1396,  1398,  1400,  1402,
    1405,  1407,  1409,  1411,  1413,  1416,  1419,  1423,  1426,  1429,
    1432,  1434,  1438,  1441,  1444,  1447,  1450,  1453,  1455,  1457,
    1459,  1461,  1463,  1465,  1467,  1469,  1473,  1476,  1477,  1478,
    1479,  1480,  1481,  1482,  1483,  1488,  1491,  1493,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1516,  1522,  1528,  1531,  1532,  1535,  1536,
    1537,  1540,  1542,  1545,  1546,  1547,  1550,  1551,  1554,  1555,
    1556,  1559,  1560,  1561,  1564,  1565,  1566,  1567,  1568,  1570,
    1572,  1575,  1576,  1577,  1578,  1583,  1586,  1587,  1590,  1591,
    1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,
    1604,  1606,  1608,  1613,  1614,  1616,  1618,  1623,  1625,  1631,
    1637,  1641,  1645,  1650,  1657,  1662,  1668,  1674,  1681,  1688,
    1693,  1698,  1705,  1710,  1715,  1717,  1719,  1721,  1723,  1725,
    1728,  1729,  1730,  1733,  1734,  1735,  1736,  1745,  1746,  1747,
    1748,  1749,  1750,  1751,  1752,  1753,  1757,  1761,  1764,  1766,
    1768,  1770,  1772,  1774,  1776,  1778,  1780,  1782,  1783,  1787,
    1791,  1795,  1796,  1798,  1800,  1803,  1804,  1805,  1807,  1810,
    1811
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

#define YYPACT_NINF (-854)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-460)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -854,    73,  2022,  -854,  -854,  -854,  -854,  -854,   775,   143,
     -29,    32,    20,  -854,  -854,  4508,  -854,  -854,  -854,   223,
    2097,   295,  3533,  3608,  3683,  3758,  3833,    11,    81,  3833,
    2172,  2247,   351,   235,  3833,   340,   372,  3908,  3983,    81,
      33,    66,    67,   155,   155,  4058,    81,    81,  3833,   386,
     398,    37,  4508,  -854,  -854,   404,   414,   433,  4508,  4508,
    -854,  2322,  2398,   371,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,   531,  -854,  -854,  -854,   704,   599,
    -854,   578,   772,   406,   355,  -854,   159,   441,   495,   596,
    -854,  -854,   407,   437,   450,   521,  -854,  -854,  3833,  3833,
    3833,  3833,  3833,  3833,   582,  2473,  4133,  2549,   639,  3833,
    3833,   360,    25,   882,   202,   545,   101,  -854,   129,    81,
    2624,  -854,  -854,   560,   671,   158,  -854,  2700,  -854,  -118,
     267,   301,   312,   361,   679,   465,   693,  2624,   504,  2624,
     364,  2624,    81,  2624,    81,   103,   283,   370,  -854,  -854,
     610,  -854,    68,  -854,    93,   634,   644,   729,   609,   241,
     245,  2624,    81,  2624,    81,   125,  2776,  -854,  3833,  3833,
    -854,  3833,  3833,  -854,  3833,  3833,  3833,  2851,  3833,  -854,
     750,  -854,  -854,  -854,   684,   680,    31,  -854,   698,  -854,
    -854,  -854,  -854,   938,  -854,   534,   638,  -854,    23,   703,
    1395,  3833,  3833,  3833,  3833,  3833,  3833,  3833,  3833,  -854,
    3833,  3833,  3833,  3833,  3833,  3833,  3833,  3833,  -854,  4508,
    4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,
    4508,  4508,  4508,  4508,   771,  4508,  4508,  4508,  4508,  4508,
    4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,  4508,
    4508,  4508,  4508,   977,   935,   782,   715,   720,   745,   753,
     777,   790,   -78,  -854,   805,  2624,   -85,   828,   344,   797,
     787,   810,  -854,   812,   941,   841,   -39,  -854,   944,   831,
     833,  2549,   986,  3833,  -854,  3833,  3833,  3833,  3833,  3833,
    3833,  3833,   875,   534,   835,  -854,  3833,  3833,  -854,  4208,
    -854,  4283,  -854,   313,   341,   343,   420,   892,   901,  -854,
     842,   844,  3833,  3833,  3833,  3833,  3833,  3833,  3833,  -854,
    3833,   849,  3833,  3833,  -854,  3833,   853,   962,   861,  -854,
    1036,  3833,  -854,  4508,  4508,    47,  -854,  -854,  -854,  3833,
    3833,  -854,  3833,  3833,  -854,  3833,  3833,   889,  -854,  1034,
    3833,  1062,  3833,   865,  1073,   891,  -854,  3833,  3833,  1114,
    1012,  1019,  1026,  1040,  1045,  1029,  -854,  -854,  -854,    81,
      16,  -854,    53,  1038,  1046,  -854,  -854,  2926,  1060,  1178,
    3002,  -854,  -854,   826,   236,   258,  1074,  3533,  3608,  3833,
    1738,    29,    57,  -854,   391,  1061,  1064,  1065,  1066,  1067,
    1068,  1071,  1072,  1075,  1076,  1077,  1079,  1080,  1082,  1083,
    1084,  1092,  1092,  1092,  1092,  1092,  1092,  1106,  1092,  1092,
    1092,  1092,  1092,  1092,  1092,  1092,  -854,  -854,  -854,   406,
    -854,  -854,  -854,  1094,  1094,   441,   495,   495,   596,   596,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  1086,  1203,
    1095,  -854,  3077,  -854,  -854,  -854,  -854,  -854,  -854,  3833,
    3153,  -854,   893,  -854,  3833,  3833,  3833,  3833,  1112,  1115,
    1119,  -854,  3229,  -854,  1091,  3833,  1099,  -854,    -6,  -854,
    1260,   735,  1117,  1100,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  1108,  -854,  1110,  2624,   265,  1152,    98,  3305,
     322,  1111,  3833,  -854,  4358,  -854,  3833,  -854,    61,  -854,
    3833,  3833,  1126,  2926,  1120,  1122,  1123,  1124,  1125,  1127,
    1128,   334,  2926,  1129,  1130,   358,    81,    -9,  4433,  -854,
      81,    81,   192,   -41,   106,    27,  -854,     6,  -854,  -854,
    -854,  1107,   895,  1529,  1131,  1132,  1133,  1140,  1153,  1156,
    -854,   -27,  1137,  1109,  1163,  1150,    81,    81,    81,  1158,
    1159,  1174,  3833,  -854,  -854,  -854,  -854,  -854,  -854,  1229,
    1230,  -854,   172,  1107,   900,    76,   552,   461,  -854,   903,
    3833,  -854,  -854,   738,  1233,  2549,  3833,  3833,    24,   278,
     -19,  -854,    58,  3833,  3833,  -854,  3833,  3833,  -854,  3833,
    3833,  3833,  3833,  3833,  3833,  3833,  3833,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  4508,  3381,  -854,  1167,  1164,   905,
    1165,  -854,   923,  2926,   525,  1161,  1162,   757,  3833,  3833,
    3833,  -854,   925,  -854,   927,   929,  -854,  3833,  1169,  1285,
     931,  -854,  -854,  -854,  -854,  -854,   933,  -854,  3833,  3833,
    3833,   940,  -854,   242,  -854,  1172,  2624,   606,  1176,  1107,
     943,  1177,  1179,  3833,   951,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,   953,  -854,  -854,  -854,  1039,  -854,  -854,
    2624,    81,  -854,  -854,  1297,  -854,  1299,  1300,  1301,  3833,
    1199,  1307,  1308,  -854,  1107,   616,  3533,  3608,  1813,  -854,
     393,  -854,  -854,  -854,  -854,  -854,  -854,  3833,    81,  -854,
    -854,  -854,  1190,  1191,  1294,  -854,  -854,  -854,  -854,  3833,
    1193,  -854,  -854,   234,   666,   894,  1265,  -854,  -854,  1107,
    1107,   955,   957,  1310,  1313,  1314,  -854,  -854,  1200,  -854,
     130,   786,  1194,   238,  3833,  3833,  3833,  3833,  3833,  3833,
    -854,  -854,  -854,  -854,   270,   335,   375,   377,   379,   380,
     388,   412,   423,   438,   454,   462,  -854,  -854,   959,  -854,
     961,   967,  -854,  1209,  -854,  -854,   989,  -854,  -854,  -854,
    -854,  1219,  1220,  1221,  -854,  -854,  -854,  1222,  3833,  1224,
    -854,  2926,   629,  -854,  -854,  1223,  1272,  1276,  -854,   994,
    -854,  3833,  -854,  -854,  1107,  -854,  -854,   675,  1227,  1228,
      -9,  -854,   996,  1102,    81,    81,    81,  -854,  -854,  3833,
    -854,  -854,  -854,    34,  -854,     9,  -854,  -854,  -854,   439,
     323,  -854,   206,  3833,  3833,  3833,  3833,  3833,  3833,  3833,
    3833,  -854,    81,  1226,  -854,  -854,    81,  1234,  3833,  -854,
     451,   452,   207,   311,   993,  1235,  1236,   977,  -854,  -854,
    1107,  1107,  1351,  1352,  1359,  -854,  3833,  3457,  1252,  1253,
    -854,   470,   486,   487,   503,   506,   514,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  1240,  -854,  -854,  -854,  -854,  -854,  3833,
    1004,  -854,  -854,  3833,  3833,  2926,   716,  -854,  -854,  3833,
    -854,  -854,  -854,  -854,    81,    81,  -854,  -854,  -854,  -854,
    -854,  1356,  3833,  1256,  1362,  1363,  3833,  3833,  3833,  -854,
    -854,  -854,   520,   522,   523,   540,   541,   546,   550,   558,
    -854,  -854,  -854,  3833,  1243,  -854,   113,  1303,  -854,   122,
    -854,  1361,  1364,  1250,  3833,  -854,  1366,  1367,  1255,  3833,
    -854,   331,   332,  1257,  1258,  -854,  -854,   567,  -854,  -854,
    -854,  -854,  -854,   572,  1246,  1006,  3833,  3833,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  1254,  1259,  -854,  -854,  1008,
    -854,   736,  1118,  -854,  -854,  -854,  3833,  -854,  -854,   594,
     595,   598,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    1262,    81,  1261,  1020,  1266,  1663,  1261,  1022,  1263,  1274,
    -854,  1275,  1293,  1305,  -854,  1306,  1375,  1380,  1324,  3833,
    -854,  1409,  1422,  1325,  3833,  -854,  -854,  -854,   261,  -854,
    -854,  -854,  1319,   600,   604,  -854,  -854,  1329,  -854,    81,
    -854,  -854,  -854,  -854,  -854,  3833,  -854,  -854,  1261,  -854,
    3533,  3608,  1888,  -854,   456,  -854,  1261,  -854,  -854,  -854,
    -854,  -854,  -854,  1330,  1333,  -854,  1334,  1335,  1336,  -854,
    1338,  1261,  1027,  -854,  -854,  -854,  -854,  -854,  1337,  -854,
     502,   363,  -854,   321,  3833,  3833,  3833,  3833,  3833,  3833,
    3833,  3833,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  1261,    81,  3833,  3833,  3833,  -854,  -854,  -854,   612,
     614,   618,   624,   673,   691,   702,   705,  -854,  -854,   706,
     707,   711,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   423,   424,   425,   427,   428,   433,
     434,   448,   447,   449,   450,     0,   435,   436,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,   426,     0,     0,     0,     0,     0,
      98,     0,     0,     3,     4,    99,   100,   107,   108,   109,
     110,   101,   102,   117,     0,   118,   119,   120,   106,     0,
     325,   327,   345,   347,   350,   352,   355,   357,   360,   363,
     370,   375,   377,   391,   419,     0,   431,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   433,   434,   448,   447,     0,
       0,   459,   373,     0,   428,   433,    35,     0,   420,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   355,     0,   460,   324,
       0,    48,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,   146,     0,     0,
     111,     0,     0,   112,     0,     0,     0,     3,     0,   417,
     103,   105,   414,   210,     0,     0,     0,   374,     0,   429,
     430,   372,   371,   428,   455,     0,     0,   451,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   319,   320,   321,   322,
     323,   317,     0,   397,     0,     0,   318,     0,     0,     0,
       0,     0,    39,   402,     0,   401,   447,   393,     0,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,   455,     0,     0,    38,   422,     0,     6,     0,
      10,     0,    11,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
       0,     0,     0,     0,    43,     0,     0,   124,     0,   136,
       0,     0,   155,     0,     0,     0,     3,   162,    46,     0,
       0,    47,     0,     0,    51,     0,     0,     0,   158,     0,
       0,     0,     0,     0,   139,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   418,     0,
       0,   213,     0,     0,     0,   248,   456,     0,     0,     0,
       0,   452,   283,   428,   433,   434,   448,     0,     0,     0,
       0,     0,     0,   282,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,   331,   334,   335,   328,   329,     0,   332,   333,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   346,
     348,   349,   351,   353,   354,   356,   358,   359,   361,   362,
     367,   368,   369,   364,   365,   366,   376,   380,   390,   384,
     385,   386,   387,   388,   389,   382,   383,   381,   413,     0,
     410,    58,     0,    92,    93,    94,    95,    96,    55,     0,
       0,   396,     0,    15,     0,     0,     0,     0,   438,   443,
     444,    60,     0,   405,     0,     0,     0,   394,     0,   392,
     422,     0,     0,     0,   319,   320,   321,   322,   323,   317,
     318,    37,     0,   421,     0,     0,     0,   425,   447,     0,
       0,     0,     0,    26,     0,    27,     0,    28,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,     0,     0,   127,
     456,     0,     0,   353,   354,   260,   263,     0,   276,   278,
     280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,     0,     0,     0,     0,   456,     0,   456,     0,
       0,     0,     0,   113,   114,   115,   116,   316,   104,     0,
       0,   213,     0,     0,     0,     0,     0,     0,   457,     0,
       0,   379,   453,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,     0,     0,   310,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,   284,    62,    64,
      66,    69,    67,    68,    63,    65,    75,    77,    79,    82,
      80,    81,    76,    78,     0,     0,   406,     0,     0,     0,
       0,   172,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,   400,     0,     0,   252,     0,     0,     0,
       0,   438,   443,   444,    36,     5,     0,    12,     0,     0,
       0,     0,    19,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    86,    85,    89,    87,
      88,    84,    41,     0,    90,    91,    44,   121,   126,   125,
       0,     0,   135,   152,     0,   154,     0,     0,     0,     0,
     265,     0,     0,   161,     0,     0,     0,     0,     0,   173,
       0,    49,    50,    53,    54,    31,    30,     0,     0,   156,
     445,   446,     0,     0,   137,   140,   141,   144,   145,     0,
       0,   211,   212,     0,     0,     0,     0,   209,   170,     0,
       0,     0,     0,     0,     0,     0,   247,   458,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     293,   300,   307,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   412,     0,   409,
       0,     0,    57,     0,    59,   171,     0,    16,   441,   440,
     439,     0,     0,     0,   403,   398,   399,     0,     0,     0,
     395,     0,     0,   257,   256,     0,     0,     0,    21,     0,
      22,     0,    25,   164,     0,    33,    32,     0,   458,   458,
       0,   128,     0,   129,     0,     0,     0,   261,   272,     0,
     266,   267,   160,   268,   264,     0,   277,   279,   281,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   174,     0,     0,    73,    74,     0,     0,     0,   208,
       0,     0,     0,     0,     0,     0,     0,     0,   169,   167,
       0,     0,     0,     0,     0,     9,     0,     0,   438,   444,
     304,     0,     0,     0,     0,     0,     0,   312,   313,   314,
     315,   285,   287,   289,   292,   290,   291,   286,   288,   411,
     408,   407,    56,   458,    70,    71,    72,   253,   254,     0,
       0,    13,    20,     0,     0,     0,     0,   163,     7,     0,
      42,    45,   123,   122,   456,     0,   133,   153,   150,   151,
     274,     0,     0,   269,     0,     0,     0,     0,     0,   177,
     178,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     159,   157,   138,     0,     0,   149,     0,     3,   243,     0,
     240,     0,     0,     0,     0,   219,     0,     0,     0,     0,
     214,     0,     0,     0,     0,   215,   216,     0,   166,   168,
     249,   250,   251,     0,   172,     0,     0,     0,   294,   296,
     299,   297,   298,   295,    17,     0,   458,   259,   258,     0,
      23,     0,   130,   131,   262,   273,     0,   270,   271,     0,
       0,     0,   179,   181,   183,   186,   184,   185,   180,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     221,     0,     0,     0,     0,   220,   217,   218,     0,   246,
     303,   302,   171,     0,     0,   255,    14,   458,     8,     0,
     134,   275,   187,   189,   188,     0,   148,   242,     0,   191,
       0,     0,     0,   190,     0,   239,     0,   228,   229,   223,
     226,   227,   222,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,   301,   305,   306,    24,   132,     0,   241,
       0,     0,   194,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   192,   238,   236,   237,   225,   234,   235,   224,
     245,     0,     0,     0,     0,     0,   195,   196,   193,     0,
       0,     0,     0,     0,     0,     0,     0,   244,   147,     0,
       0,     0,   197,   199,   201,   204,   202,   203,   198,   200,
     205,   207,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -854,  -854,     3,  -854,  -530,  -693,  -854,  -854,  -854,  -854,
    -854,   362,  -854,  -854,  -854,  -854,  -854,  -301,  -853,  -854,
    -854,  -854,   888,  -854,  -854,  -475,   963,  -366,   215,  1116,
      -2,  -223,  -854,  1113,  1238,  -854,   475,   -16,  1237,   921,
     922,    18,  -854,  1339,  -253,  -854,  -854,   -79,  -854
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   200,    64,    65,   539,    66,    67,    68,    69,
      70,   964,    71,    72,   561,    73,    74,   337,   968,    75,
      76,   370,   582,    77,   587,   279,   510,   552,   121,   175,
     128,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   196,   458,    92,    93,   129,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,   460,   417,     2,   831,   519,   584,   698,   658,   654,
     146,   295,   727,   728,   296,   660,   710,    24,   970,   943,
     138,   140,   142,   144,   145,   147,   264,   150,   152,   154,
     579,   111,   158,   122,   280,   162,   164,   469,   580,   273,
     485,   706,   274,   179,   473,   373,   182,   474,   294,   708,
     764,   185,   470,   313,   765,   603,   941,   545,   546,   195,
     198,   547,   766,   545,   546,   767,   768,   547,   186,   769,
     187,   545,   546,     3,   604,   547,   191,   192,   711,   712,
     241,   944,   945,   606,   339,   275,   545,   546,   108,   112,
     547,   113,   168,   171,   339,   374,   256,   257,   258,   259,
     260,   261,   607,   340,   266,   268,   109,   270,   271,   729,
     338,   169,   172,   340,   548,   549,   550,   772,   195,   342,
     548,   549,   550,   545,   546,   293,    63,   547,   548,   549,
     550,   659,   545,   546,  1059,   311,   547,   321,   343,   326,
     936,   328,   709,   548,   549,   550,    63,   110,   112,   942,
     113,   581,   680,   319,   357,   380,   320,   381,   170,   353,
     770,   355,   790,   166,   360,   605,   361,   362,    63,   363,
     364,   358,   365,   366,   367,   365,   368,   670,   551,  1077,
     548,   549,   550,  1085,   583,   330,   173,   341,   707,   548,
     549,   550,   679,   608,   773,   170,   173,   341,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   750,   403,   404,
     405,   406,   407,   408,   409,   410,    63,    78,   282,   751,
     752,   971,   344,   433,   434,  1109,   112,   241,   113,   972,
     744,   745,   339,  1123,   746,   331,   283,   123,    63,   239,
     240,   531,   148,   149,  1032,   886,   535,   816,  1130,   157,
     713,   340,   542,  1036,   165,   167,   817,   112,   106,   113,
     887,   180,   181,   472,   440,   441,   442,   443,   444,   445,
     446,   545,   546,   291,   704,   547,   107,   973,  1147,   491,
     241,   493,   748,   494,   495,   496,   497,   498,   499,   500,
     174,   281,   744,   745,   503,   504,   746,   506,   589,   511,
     932,   593,    96,    97,   322,   130,   131,   132,   747,   133,
     524,   525,   526,   527,   528,   529,   530,   543,   544,  1070,
     533,   534,   974,   323,   296,   976,   134,    63,   548,   549,
     550,   135,   105,   977,   148,   341,   975,   554,   555,   553,
     556,   557,   951,   558,   559,  1046,  1051,   339,   563,   322,
     565,   106,   159,  1047,  1052,   569,   570,   327,   349,   329,
     332,   155,   351,   333,   334,   156,   340,   481,   323,   595,
     869,   303,   304,   305,   890,   108,   350,   354,   823,   356,
     352,   978,   297,   639,   160,   598,   599,   600,   602,   322,
     322,   642,  1101,   596,   667,   644,   298,   668,   647,   573,
     183,  1048,  1053,   652,   241,   655,   897,   324,   323,   323,
     325,   786,   184,   842,   771,    78,   299,   609,   188,   853,
     610,   611,   854,   855,   189,   136,   979,   301,   512,   612,
     300,   856,   613,   614,   857,   858,   615,   616,   859,   860,
     980,   302,   513,   190,   684,   199,  1049,  1054,   878,   879,
     341,   672,   324,   693,   673,   325,   514,  1138,   516,   950,
    1050,  1055,   238,   692,   574,   946,   296,   640,   313,   947,
     515,   898,   517,   645,   646,   307,   477,   315,   478,   308,
     316,   317,  1114,   656,   948,  1115,  1116,   696,   503,   272,
     296,   871,   324,   324,  1117,   325,   325,  1118,  1119,  1137,
     335,  1120,  1121,   666,   575,   336,   576,   195,   626,   627,
     675,   899,   677,   900,   678,   901,   902,   628,   681,   682,
     218,   252,   218,   927,   903,   236,   237,   617,  1133,   861,
     312,   313,  1134,   313,   314,   255,   701,   753,   754,   755,
     315,   629,   315,   316,   317,   316,   317,  1135,   904,   318,
     518,   720,   630,   242,   243,   336,   788,   201,   791,   905,
     202,   203,   545,   546,   796,   253,   547,   631,   319,   204,
     740,   320,   205,   206,   906,   949,   207,   208,   254,   988,
     989,   966,   969,   632,   578,   218,   967,   967,   758,   812,
     907,   633,  1122,   761,   762,   763,   262,   756,   908,   685,
    1033,   774,   775,  1037,   776,   777,   998,   778,   779,   780,
     781,   782,   783,   784,   785,   687,   688,   244,   245,   548,
     549,   550,   999,  1000,   987,   210,   843,   844,   211,   212,
     845,   319,   689,   319,   320,   690,   320,   213,  1136,  1001,
     214,   215,  1002,   691,   216,   217,   801,   802,   803,   626,
    1003,   627,   628,   269,   797,   807,  1022,   296,  1023,  1024,
     209,   219,   220,   221,   222,   376,   377,   813,   814,   629,
     630,   223,   224,   225,   819,   631,  1025,  1026,   281,   632,
     447,   827,  1027,   846,   847,   848,  1028,   633,   448,   284,
     449,   450,  1102,   306,  1029,   226,   227,  1058,   832,   451,
     452,   794,   967,   309,   246,   247,   248,   838,  1060,   453,
     454,   430,   431,   432,   849,   850,   852,   455,   456,   457,
     249,   250,   251,   685,   687,   862,   870,   691,   218,   914,
    1072,  1073,   920,   916,  1074,   820,  1104,   867,   821,   338,
    1105,   626,   926,   627,   348,  -459,  -459,   628,  1152,   345,
    1153,   697,   699,   629,  1154,   702,   703,   705,   921,   346,
    1155,   296,   891,   892,   893,   894,   895,   896,    78,   378,
     379,    96,    97,   285,   286,   287,   288,   289,   426,   427,
     428,   734,   735,   736,  -459,  -459,   290,  -459,  -459,  -459,
    -459,  -459,  -459,  -459,  -459,  -459,  -459,  -459,  -459,  -459,
     347,   105,   630,  -459,   928,   369,   918,   929,   995,  1156,
     372,  -459,  -459,  -459,  -459,  -459,  -459,  -459,  -459,   371,
     631,  -459,  -459,  -459,  -459,  -459,  -459,  1157,  -459,  -459,
    -459,   632,  -459,   375,   633,   685,   687,   940,  1158,   382,
     691,  1159,  1160,  1161,   463,  1010,  1009,  1162,   296,   464,
    1011,   952,   953,   954,   955,   956,   957,   958,   959,   228,
     229,   230,   231,   232,   233,  1068,   965,   477,   296,   661,
     296,   234,   759,    95,   465,    96,    97,    98,    99,   100,
     101,   102,   466,   235,   993,     4,     5,     6,     7,   296,
     103,   800,   124,   125,   116,   117,   276,    13,    14,    15,
      16,    17,    18,   104,   872,   105,   467,   873,   874,   875,
     876,   461,   462,   119,    28,   479,   833,  1005,   477,   468,
     888,  1007,  1008,   480,    95,    35,    96,    97,    98,    99,
     100,   101,   102,    43,    44,    45,   471,   296,    48,   481,
    1015,   103,   482,   863,  1019,  1020,  1021,    52,    53,   447,
     475,   476,   459,    54,   594,   483,   105,   448,   486,   449,
     450,  1030,   487,   488,   489,   490,   502,   296,   451,   452,
    1035,   484,  1041,   522,   379,   376,   523,  1045,   453,   454,
     376,   532,    56,    57,   536,   377,   455,   456,   457,   537,
     538,   447,   540,   377,  1063,  1064,   566,   377,    58,   448,
     492,   449,   450,   981,   501,    59,   982,   520,   983,   984,
     451,   452,   120,   277,  1071,    62,   521,    63,   560,   278,
     453,   454,   568,   377,   376,   643,   714,   715,   455,   456,
     457,   749,   715,  1084,   757,   296,   793,   296,    96,    97,
     285,   286,   287,   288,   289,   933,   541,  1096,   562,   937,
     938,   939,  1100,   290,   795,   296,   804,   296,   805,   488,
     806,   296,   810,   488,   376,   811,   830,   538,   105,  -378,
    -378,   815,   673,   965,   824,   715,   564,   960,  1110,  1111,
    1113,   962,   828,   296,   829,   296,   880,   715,   881,   715,
     909,   296,   910,   488,     4,     5,     6,     7,   911,   296,
     567,   124,   125,   116,   117,   276,    13,    14,    15,    16,
      17,    18,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
     913,   296,   119,    28,   571,   376,   925,   934,   377,   935,
     538,  1149,  1150,  1151,    35,  1006,   296,  1062,   296,  1067,
     296,   572,    43,    44,    45,  1069,   538,    48,   573,  1012,
    1013,  1078,   715,  1086,   715,   574,    52,    53,  1131,   715,
     176,   178,    54,   436,   437,   577,   438,   439,   585,   575,
       4,     5,     6,     7,   576,   590,   586,   124,   125,   116,
     117,   276,    13,    14,    15,    16,    17,    18,   591,   597,
     618,    56,    57,   619,   620,   621,   622,   623,   119,    28,
     624,   625,   634,   235,   626,   627,   628,    58,   629,   630,
      35,   631,   632,   633,    59,   241,   635,   636,    43,    44,
      45,   120,   653,    48,    62,   637,    63,   648,   278,   657,
     649,   669,    52,    53,   650,   662,   663,   664,    54,   665,
     674,   683,   336,   741,   742,   731,  1076,   760,   808,   685,
      78,   686,   687,   688,   689,   730,   690,   691,   694,   695,
     721,   722,   723,     4,     5,     6,     7,    56,    57,   724,
     124,   125,   116,   117,   276,    13,    14,    15,    16,    17,
      18,   732,   725,    58,  1107,   726,   733,   737,   738,   739,
      59,   119,    28,   792,   794,   798,   799,   120,   789,   809,
      62,   818,    63,    35,   278,   822,   825,   834,   826,   835,
     836,    43,    44,    45,   839,   837,    48,   840,   841,   864,
     865,   866,   868,   877,   882,    52,    53,   883,   884,   885,
     889,    54,   411,   412,   413,   414,   415,   416,   912,   418,
     419,   420,   421,   422,   423,   424,   425,  1148,   914,   915,
     916,   923,   922,   917,   919,   924,   930,   931,   990,   991,
      56,    57,   961,   963,   985,   986,   992,   996,   997,  1004,
    1014,  1016,  1017,  1018,  1031,  1038,    58,  1034,  1039,  1040,
    1042,  1043,  1061,    59,  1044,  1065,  1056,  1057,  1066,  1093,
     120,  1075,  1087,    62,  1094,    63,   967,   278,     4,     5,
       6,     7,  1079,  1088,  1089,   383,   384,   385,   386,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   387,
     388,    24,  1090,  1097,    25,    26,    27,    28,   389,   390,
      31,    32,    33,    34,  1091,  1092,  1098,  1108,    35,    36,
      37,    38,    39,    40,   391,   392,    43,    44,    45,    46,
      47,    48,    49,  1095,  1099,  1103,    50,    51,  1106,  1124,
      52,    53,  1125,  1126,  1127,  1128,    54,  1129,  1132,   743,
      55,     0,   671,   429,     0,   310,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,    60,    61,     0,     0,    62,     0,
      63,   393,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   716,   717,    24,     0,     0,    25,    26,
      27,    28,    29,   718,    31,    32,    33,    34,     0,     0,
       0,     0,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
      50,    51,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,    60,    61,
       0,     0,    62,     0,    63,   719,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,  1080,  1081,    24,
       0,     0,    25,    26,    27,    28,    29,  1082,    31,    32,
      33,    34,     0,     0,     0,     0,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     0,     0,     0,    50,    51,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,    55,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,    60,    61,    48,     0,    62,     0,    63,  1083,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,   151,   120,    48,
       0,    62,     0,    63,   601,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,   151,   120,    48,     0,    62,     0,    63,   851,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,   151,   120,     0,
       0,    62,     0,    63,  1112,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,     0,     0,     0,     0,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,    50,    51,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,    55,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,    60,    61,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,   126,   127,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,   151,   120,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   193,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,   153,   120,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    56,    57,     0,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,    35,     0,     0,     0,    59,     0,     0,     0,    43,
      44,    45,   120,   194,    48,    62,     0,    63,     0,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   120,    48,
       0,    62,   197,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,    35,     0,     0,     0,    59,     0,     0,     0,
      43,    44,    45,   120,   263,    48,    62,     0,    63,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,   267,     0,    43,    44,    45,     0,   120,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     124,   125,   116,   117,   118,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    56,    57,     0,     0,     0,     0,
       0,   119,    28,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,    35,     0,     0,     0,    59,     0,     0,
       0,    43,    44,    45,   120,   194,    48,    62,     0,    63,
       0,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   124,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,   359,
      56,    57,     0,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,    35,
       0,     0,     0,    59,     0,     0,     0,    43,    44,    45,
     120,   292,    48,    62,     0,    63,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    35,     0,     0,     0,     0,    59,
       0,     0,    43,    44,    45,     0,   120,    48,     0,    62,
       0,    63,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,   199,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   124,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    35,
       0,     0,     0,     0,    59,     0,     0,    43,    44,    45,
       0,   120,    48,     0,    62,     0,    63,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    56,    57,     0,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,    35,     0,     0,     0,    59,
       0,     0,     0,    43,    44,    45,   120,   588,    48,    62,
       0,    63,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   120,    48,     0,    62,   592,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    56,    57,     0,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,    35,     0,     0,     0,
      59,     0,     0,     0,    43,    44,    45,   120,   638,    48,
      62,     0,    63,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,    35,     0,     0,     0,    59,     0,     0,     0,
      43,    44,    45,   120,   641,    48,    62,     0,    63,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     4,     5,
     507,     7,     0,     0,     0,   124,   125,   116,   117,   508,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,   119,    28,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,    35,     0,
       0,     0,    59,     0,     0,     0,    43,    44,    45,   120,
     651,    48,    62,     0,    63,     0,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   124,   125,   116,   117,   118,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    56,    57,     0,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,    35,     0,     0,     0,    59,     0,
       0,     0,    43,    44,    45,   120,   194,    48,    62,     0,
      63,     0,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    56,    57,     0,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
      35,     0,     0,     0,    59,     0,     0,     0,    43,    44,
      45,   120,   787,    48,    62,     0,    63,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    56,    57,     0,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,    35,     0,     0,     0,
      59,     0,     0,     0,    43,    44,    45,   120,   994,    48,
      62,     0,    63,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   137,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   139,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   141,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   143,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   120,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   161,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   163,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   120,    48,
       0,    62,     0,   177,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   265,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,   507,     7,
       0,     0,     0,   124,   125,   116,   117,   508,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   505,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   509,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   676,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   115,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,     0,   700,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,   120,     0,
       0,    62,     0,    63
};

static const yytype_int16 yycheck[] =
{
       2,   254,   225,     0,   697,   306,   372,   537,    14,   484,
      26,   129,    39,    40,   132,   490,    10,    26,   871,    10,
      22,    23,    24,    25,    26,    14,   105,    29,    30,    31,
      14,    11,    34,    15,   113,    37,    38,   115,    22,    14,
      79,    82,    17,    45,   129,    14,    48,   132,   127,    22,
      26,    14,   130,    29,    30,    26,    22,    10,    11,    61,
      62,    14,    38,    10,    11,    41,    42,    14,    31,    45,
      52,    10,    11,     0,    45,    14,    58,    59,    72,    73,
     121,    72,    73,    26,    26,    60,    10,    11,   117,   128,
      14,   130,    26,    26,    26,    64,    98,    99,   100,   101,
     102,   103,    45,    45,   106,   107,   135,   109,   110,   136,
     129,    45,    45,    45,    67,    68,    69,   136,   120,    26,
      67,    68,    69,    10,    11,   127,   135,    14,    67,    68,
      69,   137,    10,    11,   987,   137,    14,   139,    45,   141,
     833,   143,   115,    67,    68,    69,   135,   115,   128,   115,
     130,   135,   518,   129,    29,   132,   132,   134,   129,   161,
     136,   163,   637,   130,   166,   136,   168,   169,   135,   171,
     172,    46,   174,   175,   176,   177,   178,    79,   131,  1032,
      67,    68,    69,  1036,   131,    82,   129,   129,    82,    67,
      68,    69,   131,   136,   136,   129,   129,   129,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   131,   210,   211,
     212,   213,   214,   215,   216,   217,   135,     2,   117,   585,
     586,    14,   129,   239,   240,  1078,   128,   121,   130,    22,
      58,    59,    26,  1086,    62,   132,   135,    14,   135,    80,
      81,   320,    27,    28,   131,   115,   325,     5,  1101,    14,
     551,    45,   331,   131,    39,    40,    14,   128,   115,   130,
     130,    46,    47,   265,   246,   247,   248,   249,   250,   251,
     252,    10,    11,   115,    82,    14,   133,    70,  1131,   281,
     121,   283,   583,   285,   286,   287,   288,   289,   290,   291,
     135,   133,    58,    59,   296,   297,    62,   299,   377,   301,
     830,   380,   100,   101,    26,    10,    11,    12,   136,    14,
     312,   313,   314,   315,   316,   317,   318,   333,   334,  1012,
     322,   323,   115,    45,   132,    14,    31,   135,    67,    68,
      69,    36,   130,    22,   119,   129,   129,   339,   340,   336,
     342,   343,   136,   345,   346,    14,    14,    26,   350,    26,
     352,   115,    12,    22,    22,   357,   358,   142,   117,   144,
     145,    10,   117,    80,    81,    14,    45,   129,    45,   133,
     136,    10,    11,    12,   136,   117,   135,   162,   679,   164,
     135,    70,   115,   462,    12,   387,   388,   389,   390,    26,
      26,   470,   131,   135,   129,   474,   129,   132,   477,   129,
      14,    70,    70,   482,   121,   484,   136,   129,    45,    45,
     132,   634,    14,   714,   136,   200,   115,    26,    14,    26,
      29,    30,    29,    30,    10,   130,   115,   115,   115,    38,
     129,    38,    41,    42,    41,    42,    45,    46,    45,    46,
     129,   129,   129,    10,   523,    74,   115,   115,   749,   750,
     129,   129,   129,   532,   132,   132,   115,   136,   115,   136,
     129,   129,   107,   129,   129,    26,   132,   469,    29,    30,
     129,   136,   129,   475,   476,    10,   132,    38,   134,    14,
      41,    42,    26,   485,    45,    29,    30,   129,   490,   129,
     132,   744,   129,   129,    38,   132,   132,    41,    42,   136,
     130,    45,    46,   505,   129,   135,   129,   509,   129,   129,
     512,   136,   514,   136,   516,   136,   136,   129,   520,   521,
     129,   114,   129,   824,   136,   119,   120,   136,    26,   136,
      26,    29,    30,    29,    30,    14,   538,    76,    77,    78,
      38,   129,    38,    41,    42,    41,    42,    45,   136,    45,
     130,   553,   129,   112,   113,   135,   635,    26,   637,   136,
      29,    30,    10,    11,   643,   128,    14,   129,   129,    38,
     572,   132,    41,    42,   136,   136,    45,    46,   128,   880,
     881,   130,   130,   129,   369,   129,   135,   135,   590,   668,
     136,   129,   136,   595,   596,   597,    14,   136,   136,   129,
     966,   603,   604,   969,   606,   607,   136,   609,   610,   611,
     612,   613,   614,   615,   616,   129,   129,   122,   123,    67,
      68,    69,   136,   136,   877,    26,    10,    11,    29,    30,
      14,   129,   129,   129,   132,   129,   132,    38,   136,   136,
      41,    42,   136,   129,    45,    46,   648,   649,   650,   129,
     136,   129,   129,    14,   129,   657,   136,   132,   136,   136,
     129,    83,    84,    85,    86,   131,   132,   669,   670,   129,
     129,    93,    94,    95,   676,   129,   136,   136,   133,   129,
      14,   683,   136,    67,    68,    69,   136,   129,    22,   129,
      24,    25,  1058,    14,   136,   117,   118,   130,   700,    33,
      34,   129,   135,    10,   108,   109,   110,   709,   136,    43,
      44,   236,   237,   238,   716,   717,   718,    51,    52,    53,
     124,   125,   126,   129,   129,   727,    60,   129,   129,   129,
     136,   136,   811,   129,   136,   129,   136,   739,   132,   129,
     136,   129,   821,   129,   135,    41,    42,   129,   136,   115,
     136,   536,   537,   129,   136,   540,   541,   542,   129,   115,
     136,   132,   764,   765,   766,   767,   768,   769,   553,   131,
     132,   100,   101,   102,   103,   104,   105,   106,     7,     8,
       9,   566,   567,   568,    80,    81,   115,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      71,   130,   129,    99,   129,    55,   808,   132,   887,   136,
     130,   107,   108,   109,   110,   111,   112,   113,   114,   135,
     129,   117,   118,   119,   120,   121,   122,   136,   124,   125,
     126,   129,   128,   135,   129,   129,   129,   839,   136,   136,
     129,   136,   136,   136,   129,   129,   925,   136,   132,   129,
     929,   853,   854,   855,   856,   857,   858,   859,   860,    87,
      88,    89,    90,    91,    92,   129,   868,   132,   132,   134,
     132,    99,   134,    98,   129,   100,   101,   102,   103,   104,
     105,   106,   129,   111,   886,     3,     4,     5,     6,   132,
     115,   134,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   128,    10,   130,   129,    13,    14,    15,
      16,   129,   130,    31,    32,   118,   701,   919,   132,   129,
     134,   923,   924,   136,    98,    43,   100,   101,   102,   103,
     104,   105,   106,    51,    52,    53,   131,   132,    56,   129,
     942,   115,   130,   728,   946,   947,   948,    65,    66,    14,
     122,   123,    17,    71,   128,    14,   130,    22,    14,    24,
      25,   963,   131,   132,   131,   132,   131,   132,    33,    34,
     967,   130,   974,   131,   132,   131,   132,   979,    43,    44,
     131,   132,   100,   101,   131,   132,    51,    52,    53,    27,
      28,    14,   131,   132,   996,   997,   131,   132,   116,    22,
      14,    24,    25,    10,   129,   123,    13,   115,    15,    16,
      33,    34,   130,   131,  1016,   133,   115,   135,   129,   137,
      43,    44,   131,   132,   131,   132,   131,   132,    51,    52,
      53,   131,   132,  1035,   131,   132,   131,   132,   100,   101,
     102,   103,   104,   105,   106,   830,    10,  1049,    14,   834,
     835,   836,  1054,   115,   131,   132,   131,   132,   131,   132,
     131,   132,   131,   132,   131,   132,    27,    28,   130,   131,
     132,   131,   132,  1075,   131,   132,    14,   862,  1080,  1081,
    1082,   866,   131,   132,   131,   132,   131,   132,   131,   132,
     131,   132,   131,   132,     3,     4,     5,     6,   131,   132,
      27,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,  1114,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
     131,   132,    31,    32,    10,   131,   132,   131,   132,    27,
      28,  1133,  1134,  1135,    43,   131,   132,   131,   132,   131,
     132,   129,    51,    52,    53,    27,    28,    56,   129,   934,
     935,   131,   132,   131,   132,   129,    65,    66,   131,   132,
      44,    45,    71,   242,   243,   136,   244,   245,   130,   129,
       3,     4,     5,     6,   129,   115,   130,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    10,   115,
     129,   100,   101,   129,   129,   129,   129,   129,    31,    32,
     129,   129,    96,   111,   129,   129,   129,   116,   129,   129,
      43,   129,   129,   129,   123,   121,   130,    14,    51,    52,
      53,   130,   131,    56,   133,   130,   135,   115,   137,   130,
     115,    79,    65,    66,   115,   118,   136,   129,    71,   129,
     129,   115,   135,    14,    14,   136,  1031,    14,    79,   129,
    1035,   129,   129,   129,   129,   118,   129,   129,   129,   129,
     129,   129,   129,     3,     4,     5,     6,   100,   101,   129,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   118,   129,   116,  1069,   129,   136,   129,   129,   115,
     123,    31,    32,   129,   129,   134,   134,   130,   131,    14,
     133,   129,   135,    43,   137,   129,   129,    10,   129,    10,
      10,    51,    52,    53,   115,    14,    56,    10,    10,   129,
     129,    27,   129,    58,    14,    65,    66,    14,    14,   129,
     136,    71,   219,   220,   221,   222,   223,   224,   129,   226,
     227,   228,   229,   230,   231,   232,   233,  1132,   129,   129,
     129,    79,   129,   131,   130,    79,   129,   129,     7,     7,
     100,   101,   136,   129,   129,   129,     7,   115,   115,   129,
      14,   115,    10,    10,   131,    14,   116,    74,    14,   129,
      14,    14,   136,   123,   129,   131,   129,   129,   129,    14,
     130,   129,   129,   133,    14,   135,   135,   137,     3,     4,
       5,     6,   136,   129,   129,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   129,    14,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,   129,   129,    14,  1075,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,   129,   129,   136,    61,    62,   129,   129,
      65,    66,   129,   129,   129,   129,    71,   129,   131,   581,
      75,    -1,   509,   235,    -1,   136,    -1,    -1,   241,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,   129,   130,    -1,    -1,   133,    -1,
     135,   136,     3,     4,     5,     6,    -1,    -1,    -1,    10,
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
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,   129,   130,    56,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,   129,   130,    56,
      -1,   133,    -1,   135,   136,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,   129,   130,    56,    -1,   133,    -1,   135,   136,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,   129,   130,    -1,
      -1,   133,    -1,   135,   136,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    61,    62,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,   129,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,   129,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,
      53,   129,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,   129,   130,    56,    -1,
     133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
      -1,    43,    -1,    -1,    -1,   123,    -1,    -1,    -1,    51,
      52,    53,   130,   131,    56,   133,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,   134,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    43,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      51,    52,    53,   130,   131,    56,   133,    -1,   135,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,   124,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    43,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    51,    52,    53,   130,   131,    56,   133,    -1,   135,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    23,
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
      -1,   135,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    74,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    43,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    51,    52,    53,
      -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,    -1,
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
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,
      -1,   133,    -1,   135
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
      81,   121,   112,   113,   122,   123,   108,   109,   110,   124,
     125,   126,   114,   128,   128,    14,   168,   168,   168,   168,
     168,   168,    14,   131,   185,   130,   168,   124,   168,    14,
     168,   168,   129,    14,    17,    60,    14,   131,   137,   163,
     185,   133,   117,   135,   129,   102,   103,   104,   105,   106,
     115,   115,   131,   168,   185,   129,   132,   115,   129,   115,
     129,   115,   129,    10,    11,    12,    14,    10,    14,    10,
     181,   168,    26,    29,    30,    38,    41,    42,    45,   129,
     132,   168,    26,    45,   129,   132,   168,   166,   168,   166,
      82,   132,   166,    80,    81,   130,   135,   155,   129,    26,
      45,   129,    26,    45,   129,   115,   115,    71,   135,   117,
     135,   117,   135,   168,   166,   168,   166,    29,    46,    23,
     168,   168,   168,   168,   168,   168,   168,   168,   168,    55,
     159,   135,   130,    14,    64,   135,   131,   132,   131,   132,
     132,   134,   136,    10,    11,    12,    13,    24,    25,    33,
      34,    49,    50,   136,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   171,   171,   171,   171,   171,   171,   169,   171,   171,
     171,   171,   171,   171,   171,   171,     7,     8,     9,   172,
     174,   174,   174,   175,   175,   176,   177,   177,   178,   178,
     179,   179,   179,   179,   179,   179,   179,    14,    22,    24,
      25,    33,    34,    43,    44,    51,    52,    53,   182,    17,
     182,   129,   130,   129,   129,   129,   129,   129,   129,   115,
     130,   131,   168,   129,   132,   122,   123,   132,   134,   118,
     136,   129,   130,    14,   130,    79,    14,   131,   132,   131,
     132,   168,    14,   168,   168,   168,   168,   168,   168,   168,
     168,   129,   131,   168,   168,   130,   168,     5,    14,   130,
     164,   168,   115,   129,   115,   129,   115,   129,   130,   155,
     115,   115,   131,   132,   168,   168,   168,   168,   168,   168,
     168,   185,   132,   168,   168,   185,   131,    27,    28,   143,
     131,    10,   185,   175,   175,    10,    11,    14,    67,    68,
      69,   131,   165,   140,   168,   168,   168,   168,   168,   168,
     129,   152,    14,   168,    14,   168,   131,    27,   131,   168,
     168,    10,   129,   129,   129,   129,   129,   136,   166,    14,
      22,   135,   160,   131,   165,   130,   130,   162,   131,   185,
     115,    10,   134,   185,   128,   133,   135,   115,   168,   168,
     168,   136,   168,    26,    45,   136,    26,    45,   136,    26,
      29,    30,    38,    41,    42,    45,    46,   136,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,    96,   130,    14,   130,   131,   185,
     168,   131,   185,   132,   185,   168,   168,   185,   115,   115,
     115,   131,   185,   131,   163,   185,   168,   130,    14,   137,
     163,   134,   118,   136,   129,   129,   168,   129,   132,    79,
      79,   164,   129,   132,   129,   168,   130,   168,   168,   131,
     165,   168,   168,   115,   185,   129,   129,   129,   129,   129,
     129,   129,   129,   185,   129,   129,   129,   166,   142,   166,
     130,   168,   166,   166,    82,   166,    82,    82,    22,   115,
      10,    72,    73,   155,   131,   132,    24,    25,    34,   136,
     168,   129,   129,   129,   129,   129,   129,    39,    40,   136,
     118,   136,   118,   136,   166,   166,   166,   129,   129,   115,
     168,    14,    14,   160,    58,    59,    62,   136,   155,   131,
     131,   165,   165,    76,    77,    78,   136,   131,   168,   134,
      14,   168,   168,   168,    26,    30,    38,    41,    42,    45,
     136,   136,   136,   136,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   169,   131,   185,   131,
     163,   185,   129,   131,   129,   131,   185,   129,   134,   134,
     134,   168,   168,   168,   131,   131,   131,   168,    79,    14,
     131,   132,   185,   168,   168,   131,     5,    14,   129,   168,
     129,   132,   129,   155,   131,   129,   129,   168,   131,   131,
      27,   143,   168,   166,    10,    10,    10,    14,   168,   115,
      10,    10,   155,    10,    11,    14,    67,    68,    69,   168,
     168,   136,   168,    26,    29,    30,    38,    41,    42,    45,
      46,   136,   168,   166,   129,   129,    27,   168,   129,   136,
      60,   182,    10,    13,    14,    15,    16,    58,   155,   155,
     131,   131,    14,    14,    14,   129,   115,   130,   134,   136,
     136,   168,   168,   168,   168,   168,   168,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   131,
     131,   131,   129,   131,   129,   129,   129,   131,   168,   130,
     185,   129,   129,    79,    79,   132,   185,   155,   129,   132,
     129,   129,   142,   166,   131,    27,   143,   166,   166,   166,
     168,    22,   115,    10,    72,    73,    26,    30,    45,   136,
     136,   136,   168,   168,   168,   168,   168,   168,   168,   168,
     166,   136,   166,   129,   149,   168,   130,   135,   156,   130,
     156,    14,    22,    70,   115,   129,    14,    22,    70,   115,
     129,    10,    13,    15,    16,   129,   129,   182,   155,   155,
       7,     7,     7,   168,   131,   185,   115,   115,   136,   136,
     136,   136,   136,   136,   129,   168,   131,   168,   168,   185,
     129,   185,   166,   166,    14,   168,   115,    10,    10,   168,
     168,   168,   136,   136,   136,   136,   136,   136,   136,   136,
     168,   131,   131,   165,    74,   140,   131,   165,    14,    14,
     129,   168,    14,    14,   129,   168,    14,    22,    70,   115,
     129,    14,    22,    70,   115,   129,   129,   129,   130,   156,
     136,   136,   131,   168,   168,   131,   129,   131,   129,    27,
     143,   168,   136,   136,   136,   129,   166,   156,   131,   136,
      24,    25,    34,   136,   168,   156,   131,   129,   129,   129,
     129,   129,   129,    14,    14,   129,   168,    14,    14,   129,
     168,   131,   165,   136,   136,   136,   129,   166,   149,   156,
     168,   168,   136,   168,    26,    29,    30,    38,    41,    42,
      45,    46,   136,   156,   129,   129,   129,   129,   129,   129,
     156,   131,   131,    26,    30,    45,   136,   136,   136,   168,
     168,   168,   168,   168,   168,   168,   168,   156,   166,   168,
     168,   168,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136
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
     141,   142,   142,   142,   142,   142,   142,   142,   142,   143,
     143,   143,   143,   143,   143,   144,   144,   145,   145,   145,
     145,   146,   146,   147,   147,   147,   148,   148,   148,   149,
     150,   150,   150,   150,   150,   150,   151,   151,   152,   152,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   154,   154,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   157,   158,
     159,   159,   159,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   161,   162,   162,
     162,   162,   163,   163,   163,   163,   164,   164,   164,   164,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   167,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   169,   169,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   171,   171,   172,   172,
     172,   173,   173,   174,   174,   174,   175,   175,   176,   176,
     176,   177,   177,   177,   178,   178,   178,   178,   178,   178,
     178,   179,   179,   179,   179,   179,   180,   180,   181,   181,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   183,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   185,   185,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       5,     5,     6,     6,     4,     2,     5,     4,     3,     3,
       3,     5,     7,     3,     5,     7,     3,     3,     2,     5,
       5,     3,     2,     5,     5,     4,     7,     6,     4,     6,
       4,     2,     4,     4,     4,     4,     4,     4,     4,     4,
       7,     7,     7,     6,     6,     4,     4,     4,     4,     4,
       4,     4,     4,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     4,     4,     4,     4,     4,     2,     1,     1,
       1,     1,     1,     2,     4,     2,     1,     1,     1,     1,
       1,     2,     2,     4,     4,     4,     4,     1,     1,     1,
       1,     5,     7,     7,     3,     5,     5,     4,     6,     3,
       5,     5,     7,     4,     6,     5,     3,     5,     7,     3,
       5,     5,     3,     2,     5,     5,     2,    12,     9,     1,
       7,     7,     5,     7,     5,     3,     5,     7,     0,     4,
       6,     5,     3,     7,     6,     4,     7,     6,     7,     6,
       5,     6,     5,     3,     4,     5,     4,     5,     5,     6,
       6,     6,     6,     6,     6,     6,     6,     7,     7,     7,
       3,     3,     4,     5,     4,     5,     5,     6,     6,     6,
       6,     6,     6,     6,     6,     7,     7,     7,     6,     5,
       0,     3,     3,     0,     4,     4,     4,     5,     5,     4,
       5,     5,     6,     6,     7,     7,     6,     6,     6,     6,
       5,     5,     6,     6,     7,     7,     7,     7,     7,     6,
       4,     7,     6,     4,     8,     7,     5,     5,     0,     4,
       4,     4,     3,     5,     5,     7,     3,     3,     5,     5,
       1,     3,     5,     1,     3,     2,     3,     3,     3,     4,
       5,     5,     3,     5,     4,     6,     1,     3,     1,     3,
       1,     3,     3,     3,     4,     6,     6,     6,     6,     6,
       6,     6,     6,     5,     7,     7,     7,     7,     7,     7,
       5,     9,     8,     8,     6,     9,     9,     5,     5,     4,
       4,     4,     6,     6,     6,     6,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     5,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     1,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     4,     6,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     4,     6,
       6,     6,     1,     4,     4,     5,     5,     1,     1,     1,
       1,     2,     3,     4,     5,     2,     3,     4,     5,     1,
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
#line 456 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3070 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 466 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3076 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 467 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3082 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 471 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3088 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 473 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3094 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 475 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3100 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 477 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3108 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3114 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3120 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3126 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 487 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3132 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 489 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3140 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 493 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3148 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 497 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3154 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 499 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3162 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 503 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3170 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 507 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3176 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 509 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3182 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 511 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3188 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 513 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3194 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 515 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3200 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 517 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3208 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 521 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3216 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 525 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3222 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 527 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3228 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 529 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3234 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 531 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3240 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 533 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3246 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 535 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3252 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 537 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3258 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3264 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3270 "raku.tab.c"
    break;

  case 34: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 543 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3282 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 551 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3288 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 553 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3294 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 555 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3300 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 557 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3306 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 559 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3312 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 561 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3318 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 563 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3325 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 566 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3332 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 569 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3338 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 571 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3345 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 574 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3352 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 577 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3358 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN expr ';'  */
#line 579 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3364 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN ';'  */
#line 581 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3370 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 583 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3377 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 586 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3384 "raku.tab.c"
    break;

  case 51: /* stmt: KW_EXIT expr ';'  */
#line 589 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3390 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT ';'  */
#line 591 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3397 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 594 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3404 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 597 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3411 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 600 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3417 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 602 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3426 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 607 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3434 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 611 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3442 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 615 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3451 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 620 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3459 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall ';'  */
#line 623 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3465 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3471 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3477 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 629 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3483 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3489 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 633 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3496 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 636 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3502 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 638 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3508 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 640 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3514 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 642 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3521 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 645 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3528 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3535 "raku.tab.c"
    break;

  case 73: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3542 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 654 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3549 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_IF expr ';'  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3555 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_UNLESS expr ';'  */
#line 659 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3561 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WHILE expr ';'  */
#line 661 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3567 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_UNTIL expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3573 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_FOR expr ';'  */
#line 665 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3580 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WITH expr ';'  */
#line 668 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3586 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITHOUT expr ';'  */
#line 670 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3592 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_GIVEN expr ';'  */
#line 672 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3598 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 674 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3605 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 677 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3612 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 680 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3620 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 684 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3627 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 687 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3633 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 689 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3639 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 691 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3645 "raku.tab.c"
    break;

  case 90: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 693 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3652 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 696 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3659 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 699 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3666 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 702 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3673 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 705 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3680 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 708 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3687 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 711 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3694 "raku.tab.c"
    break;

  case 97: /* stmt: expr ';'  */
#line 713 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3700 "raku.tab.c"
    break;

  case 98: /* stmt: ';'  */
#line 714 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3706 "raku.tab.c"
    break;

  case 99: /* stmt: if_stmt  */
#line 715 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3712 "raku.tab.c"
    break;

  case 100: /* stmt: while_stmt  */
#line 716 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3718 "raku.tab.c"
    break;

  case 101: /* stmt: for_stmt  */
#line 717 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3724 "raku.tab.c"
    break;

  case 102: /* stmt: given_stmt  */
#line 718 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3730 "raku.tab.c"
    break;

  case 103: /* stmt: KW_TRY block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block KW_CATCH block  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3742 "raku.tab.c"
    break;

  case 105: /* stmt: KW_CATCH block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3748 "raku.tab.c"
    break;

  case 106: /* stmt: block  */
#line 726 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3754 "raku.tab.c"
    break;

  case 107: /* stmt: unless_stmt  */
#line 727 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3760 "raku.tab.c"
    break;

  case 108: /* stmt: until_stmt  */
#line 728 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3766 "raku.tab.c"
    break;

  case 109: /* stmt: repeat_stmt  */
#line 729 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3772 "raku.tab.c"
    break;

  case 110: /* stmt: loop_stmt  */
#line 730 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3778 "raku.tab.c"
    break;

  case 111: /* stmt: KW_LAST ';'  */
#line 731 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3784 "raku.tab.c"
    break;

  case 112: /* stmt: KW_NEXT ';'  */
#line 732 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3790 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_IF expr ';'  */
#line 734 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3796 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 736 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3802 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 738 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3808 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 740 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 117: /* stmt: sub_decl  */
#line 741 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3820 "raku.tab.c"
    break;

  case 118: /* stmt: class_decl  */
#line 742 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3826 "raku.tab.c"
    break;

  case 119: /* stmt: role_decl  */
#line 743 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3832 "raku.tab.c"
    break;

  case 120: /* stmt: grammar_decl  */
#line 744 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3838 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3844 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 750 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3850 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 752 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3856 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block  */
#line 754 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3862 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3868 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3874 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block elsif_tail  */
#line 760 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 762 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3886 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF expr block  */
#line 766 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3892 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 770 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 772 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3910 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3916 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3922 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 780 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3928 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE expr block  */
#line 782 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3934 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 786 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3940 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 788 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3946 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block  */
#line 790 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3952 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3958 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3964 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL expr block  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3970 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3976 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3982 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3988 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP block  */
#line 810 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3994 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 812 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4000 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 814 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4006 "raku.tab.c"
    break;

  case 149: /* loop_incr: expr  */
#line 817 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4012 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 821 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4022 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 827 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4032 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 833 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4041 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 838 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4051 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 844 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4060 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr block  */
#line 849 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4067 "raku.tab.c"
    break;

  case 156: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 854 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4079 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 862 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4092 "raku.tab.c"
    break;

  case 158: /* when_list: %empty  */
#line 872 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4098 "raku.tab.c"
    break;

  case 159: /* when_list: when_list KW_WHEN expr block  */
#line 874 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4106 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 880 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4118 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 888 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4128 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT sub_body  */
#line 894 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4138 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 900 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4150 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 908 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4160 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 914 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4170 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 920 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4183 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 929 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4194 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 936 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4207 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 945 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4220 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 954 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4231 "raku.tab.c"
    break;

  case 171: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 963 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4242 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 970 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4251 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list '}'  */
#line 976 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4257 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr '}'  */
#line 978 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4264 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 981 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4271 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 984 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4277 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 986 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4283 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 988 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4289 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 990 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4295 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 992 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4301 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 994 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4307 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 996 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4313 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 998 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4320 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1001 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4326 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1003 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4332 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1005 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4338 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1007 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4345 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1010 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4352 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1013 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4360 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list '}'  */
#line 1018 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4366 "raku.tab.c"
    break;

  case 191: /* method_body: '{' YADA '}'  */
#line 1019 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4372 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr '}'  */
#line 1021 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4379 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1024 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4386 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1027 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4392 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1029 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4398 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1031 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4404 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1033 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4410 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1035 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4416 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1037 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4422 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4428 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1041 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4435 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1044 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4441 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1046 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4447 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1048 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4453 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1050 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4460 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1053 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4467 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1056 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4475 "raku.tab.c"
    break;

  case 208: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1062 "raku.y"
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
#line 4493 "raku.tab.c"
    break;

  case 209: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1078 "raku.y"
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
#line 4510 "raku.tab.c"
    break;

  case 210: /* is_clauses: %empty  */
#line 1092 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4516 "raku.tab.c"
    break;

  case 211: /* is_clauses: is_clauses IDENT IDENT  */
#line 1094 "raku.y"
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
#line 4532 "raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1106 "raku.y"
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
#line 4548 "raku.tab.c"
    break;

  case 213: /* class_body_list: %empty  */
#line 1119 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4554 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1121 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4561 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1124 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4568 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1127 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4575 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1130 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4582 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1133 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4589 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1136 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4596 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1139 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4603 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1142 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4610 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1145 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4617 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1148 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4624 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1151 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4631 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1154 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4638 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1157 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4649 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1164 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4660 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1171 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4671 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1178 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4682 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1185 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4690 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1189 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4698 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1193 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4706 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1197 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4714 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1201 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4725 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1208 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4736 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1215 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4747 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1222 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4758 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1229 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4772 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1239 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4784 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1247 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4796 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1255 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4809 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1264 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4820 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1271 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4831 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1278 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4845 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1288 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4857 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1296 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4869 "raku.tab.c"
    break;

  case 247: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1306 "raku.y"
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
#line 4886 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: %empty  */
#line 1320 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4892 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1322 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4901 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1327 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4910 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1332 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4919 "raku.tab.c"
    break;

  case 252: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1339 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4927 "raku.tab.c"
    break;

  case 253: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1343 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4935 "raku.tab.c"
    break;

  case 254: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1347 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4943 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1351 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4951 "raku.tab.c"
    break;

  case 256: /* pair_list: IDENT OP_FATARROW expr  */
#line 1357 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4957 "raku.tab.c"
    break;

  case 257: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1359 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4963 "raku.tab.c"
    break;

  case 258: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1361 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4969 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1363 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4975 "raku.tab.c"
    break;

  case 260: /* param_list: VAR_SCALAR  */
#line 1366 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4981 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1367 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 4987 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1368 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 4993 "raku.tab.c"
    break;

  case 263: /* param_list: VAR_ARRAY  */
#line 1369 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4999 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' VAR_ARRAY  */
#line 1370 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5005 "raku.tab.c"
    break;

  case 265: /* param_list: IDENT VAR_SCALAR  */
#line 1371 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5011 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1372 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5017 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1373 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5023 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR  */
#line 1374 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5029 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1375 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5035 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1376 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5041 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1377 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5047 "raku.tab.c"
    break;

  case 272: /* param_list: VAR_SCALAR '=' expr  */
#line 1378 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5053 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1379 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5059 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1380 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5065 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1381 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5071 "raku.tab.c"
    break;

  case 276: /* param_list: SLURPY_POS  */
#line 1382 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5077 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' SLURPY_POS  */
#line 1383 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5083 "raku.tab.c"
    break;

  case 278: /* param_list: SLURPY_LOL  */
#line 1384 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5089 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' SLURPY_LOL  */
#line 1385 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5095 "raku.tab.c"
    break;

  case 280: /* param_list: SLURPY_NAMED  */
#line 1386 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5101 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1387 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5107 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list '}'  */
#line 1390 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5113 "raku.tab.c"
    break;

  case 283: /* block: '{' YADA '}'  */
#line 1391 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5119 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr '}'  */
#line 1393 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5125 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1395 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5131 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1397 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5137 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1399 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5143 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1401 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5149 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1403 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5156 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1406 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5162 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1408 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5168 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1410 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5174 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1412 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5180 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1414 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5187 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1417 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5194 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1420 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5202 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1424 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5209 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1427 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5216 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1430 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5223 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1433 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5229 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1435 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5237 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1439 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5244 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1442 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5251 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1445 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5258 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1448 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5265 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1451 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5272 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1454 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5278 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1456 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5284 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1458 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5290 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_LAST '}'  */
#line 1460 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5296 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1462 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5302 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1464 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5308 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1466 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5314 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1468 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5320 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1470 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5326 "raku.tab.c"
    break;

  case 316: /* closure: '{' expr '}'  */
#line 1473 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5332 "raku.tab.c"
    break;

  case 317: /* expr: VAR_SCALAR '=' expr  */
#line 1476 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5338 "raku.tab.c"
    break;

  case 318: /* expr: VAR_ARRAY '=' expr  */
#line 1477 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5344 "raku.tab.c"
    break;

  case 319: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1478 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5350 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1479 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5356 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1480 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5362 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1481 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5368 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1482 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5374 "raku.tab.c"
    break;

  case 324: /* expr: KW_GATHER block  */
#line 1483 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5384 "raku.tab.c"
    break;

  case 325: /* expr: tern_expr  */
#line 1488 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5390 "raku.tab.c"
    break;

  case 326: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1492 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5396 "raku.tab.c"
    break;

  case 327: /* tern_expr: cmp_expr  */
#line 1493 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5402 "raku.tab.c"
    break;

  case 328: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1496 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5408 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1497 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5414 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1498 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5420 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1499 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5426 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1500 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5432 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1501 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5438 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1502 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5444 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1503 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5450 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1504 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5456 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1505 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5462 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1506 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5468 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1507 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5474 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1508 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5480 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1509 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5486 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1511 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5496 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1517 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5506 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5516 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr  */
#line 1528 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5522 "raku.tab.c"
    break;

  case 346: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1531 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5528 "raku.tab.c"
    break;

  case 347: /* divis_expr: jct_expr  */
#line 1532 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5534 "raku.tab.c"
    break;

  case 348: /* jct_expr: jct_expr '|' range_expr  */
#line 1535 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5540 "raku.tab.c"
    break;

  case 349: /* jct_expr: jct_expr '&' range_expr  */
#line 1536 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5546 "raku.tab.c"
    break;

  case 350: /* jct_expr: dor_expr  */
#line 1537 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5552 "raku.tab.c"
    break;

  case 351: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1541 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5558 "raku.tab.c"
    break;

  case 352: /* dor_expr: range_expr  */
#line 1542 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5564 "raku.tab.c"
    break;

  case 353: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1545 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5570 "raku.tab.c"
    break;

  case 354: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1546 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5576 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr  */
#line 1547 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5582 "raku.tab.c"
    break;

  case 356: /* add_expr: add_expr '~' repl_expr  */
#line 1550 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5588 "raku.tab.c"
    break;

  case 357: /* add_expr: repl_expr  */
#line 1551 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5594 "raku.tab.c"
    break;

  case 358: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1554 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5600 "raku.tab.c"
    break;

  case 359: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1555 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5606 "raku.tab.c"
    break;

  case 360: /* repl_expr: addsub_expr  */
#line 1556 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5612 "raku.tab.c"
    break;

  case 361: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1559 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5618 "raku.tab.c"
    break;

  case 362: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1560 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5624 "raku.tab.c"
    break;

  case 363: /* addsub_expr: mul_expr  */
#line 1561 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5630 "raku.tab.c"
    break;

  case 364: /* mul_expr: mul_expr '*' unary_expr  */
#line 1564 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5636 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr '/' unary_expr  */
#line 1565 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5642 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '%' unary_expr  */
#line 1566 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5648 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1567 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5654 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1569 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5660 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1571 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5666 "raku.tab.c"
    break;

  case 370: /* mul_expr: unary_expr  */
#line 1572 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5672 "raku.tab.c"
    break;

  case 371: /* unary_expr: '-' unary_expr  */
#line 1575 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5678 "raku.tab.c"
    break;

  case 372: /* unary_expr: '!' unary_expr  */
#line 1576 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5684 "raku.tab.c"
    break;

  case 373: /* unary_expr: CARET unary_expr  */
#line 1577 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5690 "raku.tab.c"
    break;

  case 374: /* unary_expr: OP_REDUCE unary_expr  */
#line 1579 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5699 "raku.tab.c"
    break;

  case 375: /* unary_expr: pow_expr  */
#line 1583 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5705 "raku.tab.c"
    break;

  case 376: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1586 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5711 "raku.tab.c"
    break;

  case 377: /* pow_expr: postfix_expr  */
#line 1587 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5717 "raku.tab.c"
    break;

  case 378: /* scalar_list: VAR_SCALAR  */
#line 1590 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5723 "raku.tab.c"
    break;

  case 379: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1591 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5729 "raku.tab.c"
    break;

  case 380: /* meth_name: IDENT  */
#line 1594 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5735 "raku.tab.c"
    break;

  case 381: /* meth_name: KW_SORT  */
#line 1595 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5741 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_MAP  */
#line 1596 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5747 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_GREP  */
#line 1597 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5753 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_SAY  */
#line 1598 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5759 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_PRINT  */
#line 1599 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5765 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_TAKE  */
#line 1600 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5771 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_RETURN  */
#line 1601 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5777 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_EXISTS  */
#line 1602 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5783 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_DELETE  */
#line 1603 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5789 "raku.tab.c"
    break;

  case 390: /* meth_name: TESTOP  */
#line 1604 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5795 "raku.tab.c"
    break;

  case 391: /* postfix_expr: call_expr  */
#line 1606 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5801 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '(' arg_list ')'  */
#line 1609 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5810 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '(' ')'  */
#line 1613 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5816 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1615 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5822 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1617 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5828 "raku.tab.c"
    break;

  case 396: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1619 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5837 "raku.tab.c"
    break;

  case 397: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1624 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5843 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1626 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5853 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1632 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5863 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1638 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5871 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' KW_NEW  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5879 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' IDENT  */
#line 1646 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5888 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1651 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5899 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1658 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5908 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' CARET IDENT  */
#line 1663 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5918 "raku.tab.c"
    break;

  case 406: /* call_expr: atom '.' CARET IDENT  */
#line 1669 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5928 "raku.tab.c"
    break;

  case 407: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1675 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5939 "raku.tab.c"
    break;

  case 408: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1682 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5950 "raku.tab.c"
    break;

  case 409: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1689 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5959 "raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' meth_name  */
#line 1694 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5968 "raku.tab.c"
    break;

  case 411: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1699 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5979 "raku.tab.c"
    break;

  case 412: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1706 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5988 "raku.tab.c"
    break;

  case 413: /* call_expr: call_expr '.' meth_name  */
#line 1711 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5997 "raku.tab.c"
    break;

  case 414: /* call_expr: KW_DIE expr  */
#line 1716 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6003 "raku.tab.c"
    break;

  case 415: /* call_expr: KW_MAP closure expr  */
#line 1718 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6009 "raku.tab.c"
    break;

  case 416: /* call_expr: KW_GREP closure expr  */
#line 1720 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6015 "raku.tab.c"
    break;

  case 417: /* call_expr: KW_SORT expr  */
#line 1722 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6021 "raku.tab.c"
    break;

  case 418: /* call_expr: KW_SORT closure expr  */
#line 1724 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6027 "raku.tab.c"
    break;

  case 419: /* call_expr: atom  */
#line 1725 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6033 "raku.tab.c"
    break;

  case 420: /* arg_list: expr  */
#line 1728 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6039 "raku.tab.c"
    break;

  case 421: /* arg_list: arg_list ',' expr  */
#line 1729 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6045 "raku.tab.c"
    break;

  case 422: /* arg_list: arg_list ','  */
#line 1730 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6051 "raku.tab.c"
    break;

  case 423: /* atom: LIT_INT  */
#line 1733 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6057 "raku.tab.c"
    break;

  case 424: /* atom: LIT_FLOAT  */
#line 1734 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6063 "raku.tab.c"
    break;

  case 425: /* atom: LIT_STR  */
#line 1735 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6069 "raku.tab.c"
    break;

  case 426: /* atom: WORDLIST  */
#line 1737 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6082 "raku.tab.c"
    break;

  case 427: /* atom: LIT_INTERP_STR  */
#line 1745 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6088 "raku.tab.c"
    break;

  case 428: /* atom: VAR_SCALAR  */
#line 1746 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6094 "raku.tab.c"
    break;

  case 429: /* atom: OP_INC VAR_SCALAR  */
#line 1747 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6100 "raku.tab.c"
    break;

  case 430: /* atom: OP_DEC VAR_SCALAR  */
#line 1748 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6106 "raku.tab.c"
    break;

  case 431: /* atom: VAR_SCALAR OP_INC  */
#line 1749 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6112 "raku.tab.c"
    break;

  case 432: /* atom: VAR_SCALAR OP_DEC  */
#line 1750 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6118 "raku.tab.c"
    break;

  case 433: /* atom: VAR_ARRAY  */
#line 1751 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6124 "raku.tab.c"
    break;

  case 434: /* atom: VAR_HASH  */
#line 1752 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6130 "raku.tab.c"
    break;

  case 435: /* atom: VAR_CAPTURE  */
#line 1754 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6138 "raku.tab.c"
    break;

  case 436: /* atom: VAR_FH  */
#line 1758 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6146 "raku.tab.c"
    break;

  case 437: /* atom: VAR_NAMED_CAPTURE  */
#line 1762 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6153 "raku.tab.c"
    break;

  case 438: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1765 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6159 "raku.tab.c"
    break;

  case 439: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1767 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6165 "raku.tab.c"
    break;

  case 440: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1769 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6171 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1771 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6177 "raku.tab.c"
    break;

  case 442: /* atom: ARR_ALL_SLICE  */
#line 1773 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6183 "raku.tab.c"
    break;

  case 443: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1775 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6189 "raku.tab.c"
    break;

  case 444: /* atom: VAR_HASH '{' expr '}'  */
#line 1777 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6195 "raku.tab.c"
    break;

  case 445: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1779 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6201 "raku.tab.c"
    break;

  case 446: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1781 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6207 "raku.tab.c"
    break;

  case 447: /* atom: IDENT  */
#line 1782 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6213 "raku.tab.c"
    break;

  case 448: /* atom: VAR_TWIGIL  */
#line 1784 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6221 "raku.tab.c"
    break;

  case 449: /* atom: VAR_ARRAY_TWIGIL  */
#line 1788 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6229 "raku.tab.c"
    break;

  case 450: /* atom: VAR_HASH_TWIGIL  */
#line 1792 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6237 "raku.tab.c"
    break;

  case 451: /* atom: '[' ']'  */
#line 1795 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6243 "raku.tab.c"
    break;

  case 452: /* atom: '[' expr ']'  */
#line 1797 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6249 "raku.tab.c"
    break;

  case 453: /* atom: '[' expr ',' ']'  */
#line 1799 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6255 "raku.tab.c"
    break;

  case 454: /* atom: '[' expr ',' arg_list ']'  */
#line 1801 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6262 "raku.tab.c"
    break;

  case 455: /* atom: '(' ')'  */
#line 1803 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6268 "raku.tab.c"
    break;

  case 456: /* atom: '(' expr ')'  */
#line 1804 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6274 "raku.tab.c"
    break;

  case 457: /* atom: '(' expr ',' ')'  */
#line 1806 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6280 "raku.tab.c"
    break;

  case 458: /* atom: '(' expr ',' arg_list ')'  */
#line 1808 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6287 "raku.tab.c"
    break;

  case 459: /* atom: block  */
#line 1810 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6293 "raku.tab.c"
    break;

  case 460: /* atom: KW_SUB block  */
#line 1811 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6299 "raku.tab.c"
    break;


#line 6303 "raku.tab.c"

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

#line 1813 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
