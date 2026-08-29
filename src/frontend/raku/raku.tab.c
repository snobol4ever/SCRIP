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
  YYSYMBOL_KW_GIVEN = 37,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 38,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 39,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 40,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 41,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 42,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 43,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 44,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 45,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 46,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 47,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 48,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 49,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 50,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 51,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 52,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 53,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 54,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 55,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 56,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 57,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 58,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 59,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 60,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 61,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 62,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 63,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 64,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 65,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 66,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 67,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 68,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 69,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 70,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 71,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 72,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 73,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 74,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 75,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 76,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 77,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 78,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 79,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 80,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 81,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 82,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 83,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 84,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 85,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 86,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 87,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 88,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 89,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 90,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 91,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 92,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 93,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 94,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 95,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 96,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 97,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 98,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 99,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 100,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 101,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 102,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 103,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 104,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 105,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 106,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 107,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 108,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 109,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 110,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 111,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 112,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 113,                   /* OP_POW  */
  YYSYMBOL_114_ = 114,                     /* '='  */
  YYSYMBOL_115_ = 115,                     /* '!'  */
  YYSYMBOL_116_ = 116,                     /* '<'  */
  YYSYMBOL_117_ = 117,                     /* '>'  */
  YYSYMBOL_118_ = 118,                     /* '|'  */
  YYSYMBOL_119_ = 119,                     /* '&'  */
  YYSYMBOL_120_ = 120,                     /* '~'  */
  YYSYMBOL_121_ = 121,                     /* '+'  */
  YYSYMBOL_122_ = 122,                     /* '-'  */
  YYSYMBOL_123_ = 123,                     /* '*'  */
  YYSYMBOL_124_ = 124,                     /* '/'  */
  YYSYMBOL_125_ = 125,                     /* '%'  */
  YYSYMBOL_UMINUS = 126,                   /* UMINUS  */
  YYSYMBOL_127_ = 127,                     /* '.'  */
  YYSYMBOL_128_ = 128,                     /* ';'  */
  YYSYMBOL_129_ = 129,                     /* '('  */
  YYSYMBOL_130_ = 130,                     /* ')'  */
  YYSYMBOL_131_ = 131,                     /* ','  */
  YYSYMBOL_132_ = 132,                     /* '['  */
  YYSYMBOL_133_ = 133,                     /* ']'  */
  YYSYMBOL_134_ = 134,                     /* '{'  */
  YYSYMBOL_135_ = 135,                     /* '}'  */
  YYSYMBOL_136_ = 136,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 137,                 /* $accept  */
  YYSYMBOL_program = 138,                  /* program  */
  YYSYMBOL_stmt_list = 139,                /* stmt_list  */
  YYSYMBOL_stmt = 140,                     /* stmt  */
  YYSYMBOL_if_stmt = 141,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 142,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 143,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 144,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 145,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 146,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 147,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 148,                /* loop_incr  */
  YYSYMBOL_for_stmt = 149,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 150,               /* given_stmt  */
  YYSYMBOL_when_list = 151,                /* when_list  */
  YYSYMBOL_sub_decl = 152,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 153,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 154,                 /* sub_body  */
  YYSYMBOL_method_body = 155,              /* method_body  */
  YYSYMBOL_class_decl = 156,               /* class_decl  */
  YYSYMBOL_role_decl = 157,                /* role_decl  */
  YYSYMBOL_is_clauses = 158,               /* is_clauses  */
  YYSYMBOL_class_body_list = 159,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 160,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 161,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 162,           /* named_arg_list  */
  YYSYMBOL_pair_list = 163,                /* pair_list  */
  YYSYMBOL_param_list = 164,               /* param_list  */
  YYSYMBOL_block = 165,                    /* block  */
  YYSYMBOL_closure = 166,                  /* closure  */
  YYSYMBOL_expr = 167,                     /* expr  */
  YYSYMBOL_tern_expr = 168,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 169,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 170,               /* divis_expr  */
  YYSYMBOL_jct_expr = 171,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 172,                 /* dor_expr  */
  YYSYMBOL_range_expr = 173,               /* range_expr  */
  YYSYMBOL_add_expr = 174,                 /* add_expr  */
  YYSYMBOL_repl_expr = 175,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 176,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 177,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 178,               /* unary_expr  */
  YYSYMBOL_pow_expr = 179,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 180,              /* scalar_list  */
  YYSYMBOL_meth_name = 181,                /* meth_name  */
  YYSYMBOL_postfix_expr = 182,             /* postfix_expr  */
  YYSYMBOL_call_expr = 183,                /* call_expr  */
  YYSYMBOL_arg_list = 184,                 /* arg_list  */
  YYSYMBOL_atom = 185                      /* atom  */
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
#define YYLAST   4653

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  456
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1155

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   369


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
       2,     2,     2,   115,     2,     2,     2,   125,   119,     2,
     129,   130,   123,   121,   131,   122,   127,   124,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   136,   128,
     116,   114,   117,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   132,     2,   133,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   134,   118,   135,   120,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   126
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   454,   454,   465,   466,   469,   471,   473,   475,   479,
     481,   483,   485,   487,   491,   494,   498,   500,   504,   507,
     511,   513,   515,   517,   519,   521,   525,   529,   531,   533,
     535,   537,   539,   541,   543,   545,   547,   549,   551,   553,
     555,   557,   559,   562,   565,   567,   570,   573,   575,   577,
     579,   582,   585,   587,   590,   593,   596,   598,   603,   607,
     611,   616,   620,   621,   623,   625,   627,   629,   632,   634,
     636,   638,   641,   644,   647,   650,   653,   655,   657,   659,
     661,   664,   666,   668,   670,   673,   676,   680,   683,   685,
     687,   689,   692,   695,   698,   701,   704,   707,   710,   711,
     712,   713,   714,   715,   716,   718,   720,   722,   724,   725,
     726,   727,   728,   729,   730,   732,   734,   736,   738,   739,
     740,   741,   744,   746,   748,   750,   752,   754,   756,   758,
     762,   764,   766,   768,   770,   772,   776,   778,   782,   784,
     786,   788,   792,   794,   798,   800,   802,   806,   808,   810,
     814,   817,   823,   829,   834,   840,   845,   850,   858,   869,
     870,   876,   884,   890,   896,   904,   910,   916,   925,   932,
     941,   950,   959,   966,   973,   974,   977,   980,   982,   984,
     986,   988,   990,   992,   994,   997,   999,  1001,  1003,  1006,
    1009,  1015,  1016,  1017,  1020,  1023,  1025,  1027,  1029,  1031,
    1033,  1035,  1037,  1040,  1042,  1044,  1046,  1049,  1052,  1058,
    1074,  1089,  1090,  1102,  1116,  1117,  1120,  1123,  1126,  1129,
    1132,  1135,  1138,  1141,  1144,  1147,  1150,  1153,  1160,  1167,
    1174,  1181,  1185,  1189,  1193,  1197,  1204,  1211,  1218,  1225,
    1235,  1243,  1251,  1260,  1267,  1274,  1284,  1292,  1302,  1317,
    1318,  1323,  1328,  1335,  1339,  1343,  1347,  1353,  1355,  1357,
    1359,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1387,  1388,  1389,  1391,  1393,  1395,  1397,
    1399,  1402,  1404,  1406,  1408,  1410,  1413,  1416,  1420,  1423,
    1426,  1429,  1431,  1435,  1438,  1441,  1444,  1447,  1450,  1452,
    1454,  1456,  1458,  1460,  1462,  1464,  1466,  1470,  1473,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1485,  1488,  1490,  1493,
    1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1506,  1507,  1513,  1519,  1525,  1528,  1529,  1532,
    1533,  1534,  1537,  1539,  1542,  1543,  1544,  1547,  1548,  1551,
    1552,  1555,  1556,  1557,  1560,  1561,  1562,  1563,  1564,  1566,
    1568,  1571,  1572,  1573,  1574,  1579,  1582,  1583,  1586,  1587,
    1590,  1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,
    1600,  1602,  1604,  1609,  1610,  1612,  1614,  1619,  1621,  1627,
    1633,  1637,  1641,  1646,  1653,  1658,  1664,  1670,  1677,  1684,
    1689,  1694,  1701,  1706,  1711,  1713,  1715,  1717,  1719,  1721,
    1724,  1725,  1726,  1729,  1730,  1731,  1732,  1741,  1742,  1743,
    1744,  1745,  1746,  1747,  1748,  1749,  1753,  1757,  1760,  1762,
    1764,  1766,  1768,  1770,  1772,  1774,  1776,  1778,  1779,  1783,
    1787,  1791,  1792,  1793,  1795,  1798,  1799
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
  "KW_GIVEN", "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT",
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

#define YYPACT_NINF (-735)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-456)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -735,    13,  2127,  -735,  -735,  -735,  -735,  -735,   795,   -50,
      11,   -70,     5,  -735,  -735,  4519,  -735,  -735,  -735,    59,
    2202,    73,  3544,  3619,  3694,  3769,  3844,    -3,   -19,  3844,
    2277,  2352,   337,  3844,   167,   273,  3919,  3994,   -19,    57,
      -7,    27,     8,     8,  4069,   -19,   -19,  3844,   282,   331,
     234,  4519,  -735,  -735,   347,   364,   375,  4519,  4519,  -735,
    2427,   321,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,   602,  -735,  -735,  -735,   691,   636,  -735,   794,
     562,   414,   299,  -735,   -41,   302,   813,   561,  -735,  -735,
     310,   312,   341,   444,  -735,  -735,  3844,  3844,  3844,  3844,
    3844,  3844,   477,  2503,  4144,  2579,   490,  3844,  3844,   368,
     168,   962,   117,   382,   137,  -735,    24,   -19,  2654,  -735,
    -735,   392,   802,   140,  -735,  2730,  -735,   130,    98,   243,
     268,   712,   529,   446,   557,  2654,   585,  2654,    49,  2654,
     -19,  2654,   -19,   -24,   110,    84,  -735,  -735,   443,  -735,
      37,  -735,   148,   465,   480,   441,   179,   236,  2654,   -19,
    2654,   -19,   257,  2862,  -735,  3844,  3844,  -735,  3844,  3844,
    -735,  3844,  3844,  3844,  2937,  3844,  -735,   550,  -735,  -735,
    -735,   478,   501,    86,  -735,   521,  -735,  -735,  -735,  -735,
     721,  -735,   840,   853,   524,  1521,  3844,  3844,  3844,  3844,
    3844,  3844,  3844,  3844,  -735,  3844,  3844,  3844,  3844,  3844,
    3844,  3844,  3844,  -735,  4519,  4519,  4519,  4519,  4519,  4519,
    4519,  4519,  4519,  4519,  4519,  4519,  4519,  4519,  4519,   783,
    4519,  4519,  4519,  4519,  4519,  4519,  4519,  4519,  4519,  4519,
    4519,  4519,  4519,  4519,  4519,  4519,  4519,   715,   839,   860,
     533,   551,   572,   581,   618,   631,   135,  -735,   861,  2654,
     199,   874,   195,   651,   637,   689,  -735,   690,   820,   751,
      30,  -735,   827,   867,   869,  2579,   909,  3844,  -735,  3844,
    3844,  3844,  3844,  3844,  3844,  3844,   765,   840,   871,  -735,
    3844,  3844,  -735,  4219,  -735,  4294,  -735,   297,   301,   348,
      85,   803,   819,  -735,   877,   879,  3844,  3844,  3844,  3844,
    3844,  3844,  3844,  -735,  3844,   885,  3844,  3844,  -735,  3844,
     889,   994,   893,  -735,   902,  3844,  -735,  4519,  4519,    22,
    -735,  -735,  -735,  3844,  3844,  -735,  3844,  3844,  -735,  3844,
    3844,  -735,   976,  3844,   989,  3844,   898,   914,   900,  -735,
    3844,  3844,   953,   857,   883,   907,   919,   947,   870,  -735,
    -735,  -735,   -19,    -5,  -735,    45,   966,   968,  -735,  -735,
    3012,   924,  1054,  -735,   949,   248,   274,   965,  3544,  3619,
    3844,  1850,   225,   320,  -735,   362,   975,  1000,  1007,  1023,
    1047,  1051,  1055,  1057,  1058,  1059,  1060,  1061,  1064,  1065,
    1067,  1069,  1012,  1012,  1012,  1012,  1012,  1012,  1052,  1012,
    1012,  1012,  1012,  1012,  1012,  1012,  1012,  -735,  -735,  -735,
     414,  -735,  -735,  -735,  1062,  1062,   302,   813,   561,   561,
    -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  1038,  1180,
    1070,  -735,  3088,  -735,  -735,  -735,  -735,  -735,  -735,  3844,
    3164,  -735,   903,  3844,  -735,  3844,  3844,  3844,  3844,  1084,
    1086,  1087,  -735,  3240,  -735,  1126,  3844,  1073,  -735,    -6,
    -735,  1394,   350,  1088,  1068,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  1076,  -735,  1078,  2654,   269,  1129,   133,
    3316,   430,  1080,  3844,  -735,  4369,  -735,  3844,  -735,    51,
    -735,  3844,  3844,  1095,  3012,  1082,  1083,  1085,  1089,  1090,
    1091,  1092,   559,  3012,  1093,  1094,   616,   -19,   -20,  4444,
    -735,   -19,   -19,    97,   -40,    95,    26,  -735,    20,  -735,
    -735,  -735,  1081,   906,  1648,  1096,  1099,  1100,  1101,  1102,
    1103,   -13,  1097,  1077,  1115,  1098,   -19,   -19,   -19,  1107,
    1108,  1109,  3844,  -735,  -735,  -735,  -735,  -735,  -735,  1202,
    1220,  -735,     9,  1081,   910,    58,   497,     1,  -735,   912,
    3844,  -735,  1223,  2579,  3844,  3844,   390,    94,   -88,  -735,
     324,  3844,  3844,  -735,  3844,  3844,  -735,  3844,  3844,  3844,
    3844,  3844,  3844,  3844,  3844,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  4519,  3392,  -735,  1260,  1110,   925,  1111,  -735,
     927,  3012,  1112,   632,  1113,  1114,   565,  3844,  3844,  3844,
    -735,   929,  -735,   936,   938,  -735,  3844,  1164,  1229,   943,
    -735,  -735,  -735,  -735,  -735,   950,  3844,  -735,  3844,  3844,
    3844,   952,  -735,   461,  -735,  1116,  2654,   797,  1117,  1081,
     955,  1121,  1122,  3844,   957,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,   959,  -735,  -735,  -735,  1066,  -735,  -735,
    2654,   -19,  -735,  -735,  1241,  -735,  1242,  1247,  1244,  3844,
    1145,  1251,  1257,  -735,  1081,   502,  3544,  3619,  1925,  -735,
     406,  -735,  -735,  -735,  -735,  -735,  -735,  3844,   -19,  -735,
    -735,  -735,  1140,  1141,  1254,  -735,  -735,  -735,  -735,  3844,
    1154,  -735,  -735,   380,   669,   905,  1226,  -735,  -735,  1081,
    1081,   969,   971,  1270,  1271,  1272,  -735,  -735,  1159,   281,
     672,  1153,   190,  3844,  3844,  3844,  3844,  3844,  3844,  -735,
    -735,  -735,  -735,   293,   305,   345,   354,   357,   365,   367,
     371,   389,   391,   395,   400,  -735,  -735,   982,  -735,   984,
     986,  -735,  1161,  -735,  -735,   988,  -735,  -735,  -735,  -735,
    -735,  1162,  1165,  1166,  -735,  -735,  -735,  1167,  3844,  1169,
    -735,  3012,  1168,   798,  -735,  -735,  1171,  1217,  1222,  -735,
     990,  -735,  3844,  -735,  -735,  1081,  -735,  -735,   799,  1173,
    1175,   -20,  -735,   996,  1106,   -19,   -19,   -19,  -735,  -735,
    3844,  -735,  -735,  -735,    29,  -735,    76,  -735,  -735,  -735,
     510,   124,  -735,   343,  3844,  3844,  3844,  3844,  3844,  3844,
    3844,  3844,  -735,   -19,  1170,  -735,  -735,   -19,  1176,  3844,
    -735,   428,   484,    32,   228,   852,  1178,  1179,   715,  -735,
    -735,  1081,  1081,  1301,  1302,  1306,  -735,  3844,  3468,  1200,
    1203,  -735,   403,   409,   417,   418,   420,   421,  -735,  -735,
    -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  -735,  1188,  -735,  -735,  -735,  -735,  -735,
    3844,  1019,  -735,  -735,  -735,  3844,  3844,  3012,   811,  -735,
    -735,  3844,  -735,  -735,  -735,  -735,   -19,   -19,  -735,  -735,
    -735,  -735,  -735,  1304,  3844,  1205,  1310,  1311,  3844,  3844,
    3844,  -735,  -735,  -735,   431,   482,   499,   505,   528,   539,
     564,   573,  -735,  -735,  -735,  3844,  1192,  -735,   173,  1250,
    -735,   233,  -735,  1312,  1313,  1201,  3844,  -735,  1314,  1317,
    1204,  3844,  -735,   270,   298,  1206,  1207,  -735,  -735,   740,
    -735,  -735,  -735,  -735,  -735,   582,  1198,  1025,  3844,  3844,
    -735,  -735,  -735,  -735,  -735,  -735,  -735,  1208,  1209,  -735,
    -735,  1029,  -735,   812,  1134,  -735,  -735,  -735,  3844,  -735,
    -735,   590,   598,   606,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  1211,   -19,  1210,  1033,  1212,  1775,  1210,  1035,
    1213,  1214,  -735,  1215,  1218,  1221,  -735,  1224,  1322,  1326,
    1225,  3844,  -735,  1331,  1334,  1227,  3844,  -735,  -735,  -735,
     256,  -735,  -735,  -735,  1216,   607,   608,  -735,  -735,  1228,
    -735,   -19,  -735,  -735,  -735,  -735,  -735,  3844,  -735,  -735,
    1210,  -735,  3544,  3619,  2000,  -735,   579,  -735,  1210,  -735,
    -735,  -735,  -735,  -735,  -735,  1230,  1233,  -735,  1234,  1235,
    1236,  -735,  1237,  1210,  1039,  -735,  -735,  -735,  -735,  -735,
    1238,  -735,   547,   314,  -735,   349,  3844,  3844,  3844,  3844,
    3844,  3844,  3844,  3844,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  1210,   -19,  3844,  3844,  3844,  -735,  -735,
    -735,   610,   634,   658,   659,   660,   678,   701,   702,  -735,
    -735,   703,   704,   705,  -735,  -735,  -735,  -735,  -735,  -735,
    -735,  -735,  -735,  -735,  -735
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
       0,     0,   442,   426,     0,     0,     0,     0,     0,    99,
       0,     3,     4,   100,   101,   108,   109,   110,   111,   102,
     103,   118,     0,   119,   120,   121,   107,     0,   326,   328,
     346,   348,   351,   353,   356,   358,   360,   363,   370,   375,
     377,   391,   419,     0,   431,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,   433,   434,   448,   447,     0,     0,   455,
     373,     0,   428,   433,    36,     0,   420,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   356,     0,   456,   325,     0,    49,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   144,     0,   147,     0,     0,   112,     0,     0,
     113,     0,     0,     0,     3,     0,   417,   104,   106,   414,
     211,     0,     0,     0,   374,     0,   429,   430,   372,   371,
     428,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     320,   321,   322,   323,   324,   318,     0,   397,     0,     0,
     319,     0,     0,     0,     0,     0,    40,   402,     0,   401,
     447,   393,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,     0,     0,     0,   451,     0,     0,    39,
     422,     0,     6,     0,    10,     0,    11,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,    44,     0,
       0,   125,     0,   137,     0,     0,   156,     0,     0,     0,
       3,   163,    47,     0,     0,    48,     0,     0,    52,     0,
       0,   159,     0,     0,     0,     0,     0,   140,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,   418,     0,     0,   214,     0,     0,     0,   249,   452,
       0,     0,     0,   284,   428,   433,   434,   448,     0,     0,
       0,     0,     0,     0,   283,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   331,   332,   335,   336,   329,   330,     0,   333,
     334,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     347,   349,   350,   352,   354,   355,   357,   359,   361,   362,
     367,   368,   369,   364,   365,   366,   376,   380,   390,   384,
     385,   386,   387,   388,   389,   382,   383,   381,   413,     0,
     410,    59,     0,    93,    94,    95,    96,    97,    56,     0,
       0,   396,     0,     0,    16,     0,     0,     0,     0,   438,
     443,   444,    61,     0,   405,     0,     0,     0,   394,     0,
     392,   422,     0,     0,     0,   320,   321,   322,   323,   324,
     318,   319,    38,     0,   421,     0,     0,     0,   425,   447,
       0,     0,     0,     0,    28,     0,    29,     0,    30,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   452,     0,     0,
     128,   452,     0,     0,   354,   355,   261,   264,     0,   277,
     279,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   452,     0,   452,     0,
       0,     0,     0,   114,   115,   116,   117,   317,   105,     0,
       0,   214,     0,     0,     0,     0,     0,     0,   453,     0,
       0,   379,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,   311,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,   285,    63,    65,    67,    70,
      68,    69,    64,    66,    76,    78,    80,    83,    81,    82,
      77,    79,     0,     0,   406,     0,     0,     0,     0,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,   400,     0,     0,   253,     0,     0,     0,     0,
     438,   443,   444,    37,     5,     0,     0,    12,     0,     0,
       0,     0,    21,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,    87,    86,    90,    88,
      89,    85,    42,     0,    91,    92,    45,   122,   127,   126,
       0,     0,   136,   153,     0,   155,     0,     0,     0,     0,
     266,     0,     0,   162,     0,     0,     0,     0,     0,   174,
       0,    50,    51,    54,    55,    33,    32,     0,     0,   157,
     445,   446,     0,     0,   138,   141,   142,   145,   146,     0,
       0,   212,   213,     0,     0,     0,     0,   210,   171,     0,
       0,     0,     0,     0,     0,     0,   248,   454,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     301,   308,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   327,   412,     0,   409,     0,
       0,    58,     0,    60,   172,     0,    18,    17,   441,   440,
     439,     0,     0,     0,   403,   398,   399,     0,     0,     0,
     395,     0,     0,     0,   258,   257,     0,     0,     0,    23,
       0,    24,     0,    27,   165,     0,    35,    34,     0,   454,
     454,     0,   129,     0,   130,     0,     0,     0,   262,   273,
       0,   267,   268,   161,   269,   265,     0,   278,   280,   282,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,     0,    74,    75,     0,     0,     0,
     209,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     168,     0,     0,     0,     0,     0,     9,     0,     0,   438,
     444,   305,     0,     0,     0,     0,     0,     0,   313,   314,
     315,   316,   286,   288,   290,   293,   291,   292,   287,   289,
     411,   408,   407,    57,   454,    71,    72,    73,   254,   255,
       0,     0,    14,    13,    22,     0,     0,     0,     0,   164,
       7,     0,    43,    46,   124,   123,   452,     0,   134,   154,
     151,   152,   275,     0,     0,   270,     0,     0,     0,     0,
       0,   178,   179,   176,     0,     0,     0,     0,     0,     0,
       0,     0,   160,   158,   139,     0,     0,   150,     0,     3,
     244,     0,   241,     0,     0,     0,     0,   220,     0,     0,
       0,     0,   215,     0,     0,     0,     0,   216,   217,     0,
     167,   169,   250,   251,   252,     0,   173,     0,     0,     0,
     295,   297,   300,   298,   299,   296,    19,     0,   454,   260,
     259,     0,    25,     0,   131,   132,   263,   274,     0,   271,
     272,     0,     0,     0,   180,   182,   184,   187,   185,   186,
     181,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   222,     0,     0,     0,     0,   221,   218,   219,
       0,   247,   304,   303,   172,     0,     0,   256,    15,   454,
       8,     0,   135,   276,   188,   190,   189,     0,   149,   243,
       0,   192,     0,     0,     0,   191,     0,   240,     0,   229,
     230,   224,   227,   228,   223,     0,     0,   233,     0,     0,
       0,   234,     0,     0,     0,   302,   306,   307,    26,   133,
       0,   242,     0,     0,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   193,   239,   237,   238,   226,   235,
     236,   225,   246,     0,     0,     0,     0,     0,   196,   197,
     194,     0,     0,     0,     0,     0,     0,     0,     0,   245,
     148,     0,     0,     0,   198,   200,   202,   205,   203,   204,
     199,   201,   206,   208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -735,  -735,     3,  -735,  -523,  -683,  -735,  -735,  -735,  -735,
    -735,   290,  -735,  -735,  -735,  -735,  -735,  -286,  -734,  -735,
    -735,  -735,   796,  -735,  -735,  -463,   866,  -355,   327,  1128,
      -2,  -218,  -735,   733,  1139,  -735,   713,   -11,  1135,  1133,
     935,    -8,  -735,  1239,  -247,  -735,  -735,   -51,  -735
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   195,    62,    63,   530,    64,    65,    66,    67,
      68,   956,    69,    70,   551,    71,    72,   331,   960,    73,
      74,   363,   572,    75,   577,   273,   501,   543,   119,   172,
     126,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,   193,   448,    90,    91,   127,    92
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,   450,   408,     2,   822,   688,    24,   120,   647,   569,
     574,   145,   643,     3,   510,   144,   109,   570,   649,   165,
     136,   138,   140,   142,   143,   717,   718,   148,   150,   152,
     700,   155,   536,   537,   159,   161,   538,   166,   234,   235,
     332,   696,   176,   184,   108,   179,   963,   761,   698,   188,
     189,   933,   258,   168,   964,   536,   537,   324,   192,   538,
     274,   536,   537,   333,   104,   538,   734,   735,   536,   537,
     736,   169,   538,   121,   288,   316,   743,   744,   745,   236,
     236,   334,   105,   128,   129,   130,   935,   131,   539,   540,
     541,   701,   702,   317,   250,   251,   252,   253,   254,   255,
     366,   965,   260,   262,   132,   264,   265,   325,   476,   133,
      61,   539,   540,   541,    61,    61,   192,   539,   540,   541,
     316,   167,   719,   287,   539,   540,   541,   106,   962,   571,
     648,    61,   110,   305,   111,   315,   746,   320,   317,   322,
     699,   928,   171,   934,   737,   107,   966,   936,   937,   367,
     316,   110,   542,   111,   670,   170,   346,   110,   348,   111,
     967,   353,   779,   354,   355,   335,   356,   357,   317,   358,
     359,   360,   358,   361,   336,   573,   697,   318,   694,   156,
     319,   669,   267,   536,   537,   268,   163,   538,   740,   327,
     328,    61,   337,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   134,   394,   395,   396,   397,   398,   399,   400,
     401,   660,   291,   329,   509,   236,    94,    95,   330,   330,
     741,   742,   318,   424,   425,   319,   292,   269,   290,   760,
     236,    61,   430,   431,   432,   433,   434,   435,   436,   539,
     540,   541,   968,   536,   537,  1051,   103,   538,   182,   459,
     969,   591,   318,   276,   285,   319,   703,   462,   289,   942,
     110,   290,   111,   522,   460,   183,   536,   537,   526,   592,
     538,   277,   275,   482,   533,   484,   338,   485,   486,   487,
     488,   489,   490,   491,  1038,   157,   350,   738,   494,   495,
    1069,   497,  1039,   502,  1077,   342,   180,   970,   924,   539,
     540,   541,   351,  1024,   515,   516,   517,   518,   519,   520,
     521,   463,  1043,   343,   524,   525,   534,   535,   472,   579,
    1044,  1062,   539,   540,   541,   881,   468,   464,   469,    76,
     465,   545,   546,   544,   547,   548,  1101,   549,   550,  1040,
     316,   553,   971,   555,  1115,   181,   594,   153,   559,   560,
     333,   154,   344,   167,   146,   147,   972,   293,   317,  1122,
     593,   185,   104,  1028,   595,   162,   164,  1045,   334,   333,
     345,   294,   177,   178,   186,   333,   586,   587,   588,   590,
     583,   656,   295,   814,  1041,   187,  1093,   334,   597,  1139,
     106,   598,   599,   334,   194,   877,   296,   657,  1042,   600,
     658,   627,   601,   602,   775,   233,   603,   604,   584,   630,
     878,   503,  1046,   237,   633,   505,   753,   636,   833,   307,
     754,   563,   641,   246,   644,   504,  1047,   755,   888,   506,
     756,   757,   844,   564,   758,   845,   846,   734,   735,   247,
     889,   736,   318,   847,   146,   319,   848,   849,   170,  1129,
     850,   851,   335,   869,   870,   596,   301,   628,   249,   762,
     302,   632,   507,   674,   634,   635,   807,   321,   248,   323,
     326,   335,   683,   565,   645,   808,   508,   335,   943,   494,
     890,   468,   566,   650,  1130,   614,   347,   862,   349,   891,
     213,   256,   892,   615,   655,   616,   266,   605,   192,   617,
     893,   665,   894,   667,   263,   668,   895,   536,   537,   671,
     672,   538,   834,   835,   275,   860,   836,   618,   313,   619,
     278,   314,    76,   620,   896,   759,   897,   691,   621,   919,
     898,   675,   231,   232,   213,   899,   938,   677,   990,   307,
     939,   852,   710,   300,   991,   678,   679,   309,   680,   681,
     310,   311,   992,   993,   940,   994,   995,   958,   662,   614,
     730,   663,   959,   539,   540,   541,  1014,   303,   837,   838,
     839,   332,   777,  1125,   780,   341,   307,  1126,   748,   339,
     785,   750,   751,   752,   309,   980,   981,   310,   311,   763,
     764,  1127,   765,   766,   340,   767,   768,   769,   770,   771,
     772,   773,   774,  1025,   362,  1106,  1029,   803,  1107,  1108,
     615,   306,   364,   961,   307,   308,  1109,  1015,   959,  1110,
    1111,   979,   309,  1112,  1113,   310,   311,   616,   196,   312,
     365,   197,   198,   617,  1016,   791,   792,   793,   313,   199,
    1017,   314,   200,   201,   797,   941,   202,   203,   223,   224,
     225,   226,   227,   228,   802,   368,   618,   804,   805,   373,
     229,   453,   205,  1018,   810,   206,   207,   619,   240,   241,
     242,   818,   230,   208,  1019,   313,   209,   210,   314,   454,
     211,   212,  1128,   437,   243,   244,   245,   682,   823,   568,
     290,   438,   620,   439,   440,  1094,   290,   829,   790,  1020,
     455,   621,   441,   442,   840,   841,   843,   213,  1021,   456,
     783,   443,   444,   313,  1114,   853,   314,  1052,   675,   445,
     446,   447,   297,   298,   299,  1064,   677,   858,   861,   437,
     204,  -455,  -455,  1065,   681,   905,   907,   438,   614,   439,
     440,  1066,  1096,  1097,   686,  1144,   457,   290,   441,   442,
     911,   882,   883,   884,   885,   886,   887,   443,   444,   458,
     787,   918,   615,   290,   213,   445,   446,   447,   470,  1145,
    -455,  -455,   471,  -455,  -455,  -455,  -455,  -455,  -455,  -455,
    -455,  -455,  -455,  -455,  -455,  -455,   616,   617,   618,  -455,
     417,   418,   419,  1146,  1147,  1148,   909,  -455,  -455,  -455,
    -455,  -455,  -455,   468,  -455,   879,   619,  -455,  -455,  -455,
    -455,  -455,  -455,  1149,  -455,  -455,  -455,   472,  -455,   473,
      94,    95,   279,   280,   281,   282,   283,   987,   932,   620,
     621,   675,   677,   681,   474,   284,  1150,  1151,  1152,  1153,
    1154,   477,   944,   945,   946,   947,   948,   949,   950,   951,
     103,  -378,  -378,   437,   687,   689,   449,   957,   692,   693,
     695,   438,   973,   439,   440,   974,  1001,   975,   976,  1050,
    1003,    76,   441,   442,   959,   985,   214,   215,   216,   217,
     475,   443,   444,   724,   725,   726,   218,   219,   220,   445,
     446,   447,    93,   492,    94,    95,    96,    97,    98,    99,
     100,    94,    95,   279,   280,   281,   282,   283,   997,   101,
     221,   222,   532,   999,  1000,   863,   284,   511,   864,   865,
     866,   867,   102,   483,   103,   811,   913,   920,   812,   290,
     921,   103,  1007,   512,   238,   239,  1011,  1012,  1013,  1002,
    1060,   557,   290,   290,   421,   422,   423,   402,   403,   404,
     405,   406,   407,  1022,   409,   410,   411,   412,   413,   414,
     415,   416,  1027,   561,  1033,     4,     5,     6,     7,  1037,
     369,   370,   122,   123,   114,   115,   270,    13,    14,    15,
      16,    17,    18,   371,   372,   562,  1055,  1056,   451,   452,
     552,   461,   290,   117,    28,   466,   467,   478,   479,   480,
     481,   493,   290,   554,    34,   567,  1063,   513,   372,   369,
     514,   563,    42,    43,    44,   369,   523,    47,   824,   527,
     370,   528,   529,   531,   370,  1076,    51,    52,   556,   370,
     558,   370,    53,   369,   631,   564,   704,   705,   580,  1088,
     739,   705,   747,   290,  1092,   854,    93,   565,    94,    95,
      96,    97,    98,    99,   100,   782,   290,   784,   290,   794,
     290,    55,    56,   101,   581,   957,   795,   479,   796,   290,
    1102,  1103,  1105,   800,   479,   566,   582,    57,   103,   585,
     369,   801,   806,   663,    58,   815,   705,   819,   290,   820,
     290,   118,   271,   821,   529,   575,    61,   576,   272,   871,
     705,   872,   705,   606,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,   900,   290,   901,   479,   902,   290,   904,   290,
     369,   917,   230,  1141,  1142,  1143,   926,   370,   607,     4,
       5,     6,     7,   927,   529,   608,   122,   123,   114,   115,
     270,    13,    14,    15,    16,    17,    18,   622,   925,   998,
     290,   609,   929,   930,   931,  1054,   290,   117,    28,  1059,
     290,  1061,   529,  1070,   705,  1078,   705,   623,    34,  1123,
     705,   173,   175,   428,   429,   610,    42,    43,    44,   611,
     952,    47,   236,   612,   954,   613,   614,   615,   616,   617,
      51,    52,   618,   619,   624,   620,    53,   621,   637,   625,
     638,   639,   646,   652,   653,   651,   654,   659,   664,   673,
     675,   676,   721,   677,   720,   330,   731,   678,   679,   680,
     681,   684,   685,   729,   711,    55,    56,   712,   713,   714,
     715,   716,   722,   723,   732,   727,   728,   749,   781,   783,
     786,    57,   798,   799,   809,   813,   788,   789,    58,   816,
     817,   825,   826,  1004,  1005,   118,   642,   827,   828,   830,
      61,   831,   272,     4,     5,     6,     7,   832,   855,   856,
     122,   123,   114,   115,   270,    13,    14,    15,    16,    17,
      18,   857,   859,   868,   873,   874,   875,   876,   880,   903,
     905,   117,    28,   906,   907,   915,   912,   908,   910,   914,
     916,   922,    34,   923,   955,   953,   977,   978,   982,   983,
      42,    43,    44,   984,   988,    47,   996,   989,  1006,  1008,
    1009,  1010,  1023,  1026,    51,    52,  1030,  1031,  1034,  1032,
      53,  1035,  1036,  1053,  1048,  1049,  1085,  1058,  1057,  1067,
    1086,  1079,  1080,  1081,   959,  1089,  1082,  1071,  1090,  1083,
    1068,  1095,  1084,  1087,    76,  1091,  1098,  1100,  1116,    55,
      56,  1117,  1118,  1119,  1120,  1121,   661,   733,  1124,   420,
     427,   426,     0,   304,     0,    57,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,  1099,   118,
     778,     0,     0,     0,    61,     0,   272,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   270,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,     0,     0,
       0,     0,     0,     0,    42,    43,    44,     0,     0,    47,
       0,  1140,     0,     0,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,    58,     0,     0,     0,
       0,     0,     0,   118,     4,     5,     6,     7,    61,     0,
     272,   374,   375,   376,   377,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   378,   379,    24,     0,     0,
      25,    26,    27,    28,   380,   381,    31,    32,    33,     0,
       0,     0,     0,    34,    35,    36,    37,    38,    39,   382,
     383,    42,    43,    44,    45,    46,    47,    48,     0,     0,
       0,    49,    50,     0,     0,    51,    52,     0,     0,     0,
       0,    53,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    59,
      60,     4,     5,     6,     7,    61,   384,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   706,   707,    24,     0,     0,    25,    26,    27,
      28,    29,   708,    31,    32,    33,     0,     0,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,     0,     0,     0,    49,    50,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,    59,    60,     4,     5,
       6,     7,    61,   709,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,  1072,
    1073,    24,     0,     0,    25,    26,    27,    28,    29,  1074,
      31,    32,    33,     0,     0,     0,     0,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     0,     0,     0,    49,    50,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,    54,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,    59,    60,    47,     0,     0,     0,    61,
    1075,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,   149,   118,
      47,     0,     0,     0,    61,   589,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,   149,   118,    47,     0,     0,     0,    61,
     842,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,   149,   118,
       4,     5,     6,     7,    61,  1104,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     0,     0,     0,    49,    50,     0,
       0,    51,    52,     0,     0,     0,     0,    53,     0,     0,
       0,    54,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   122,   123,   114,   115,   116,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    55,    56,     0,     0,
       0,     0,     0,   117,    28,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    34,     0,     0,     0,     0,    58,
       0,     0,    42,    43,    44,    59,    60,    47,     0,     0,
       0,    61,     0,     0,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   122,   123,   114,
     115,   116,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    55,    56,     0,     0,     0,     0,     0,   117,    28,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    34,
       0,     0,     0,     0,    58,     0,     0,    42,    43,    44,
     124,   125,    47,     0,     0,     0,    61,     0,     0,     0,
       0,    51,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   122,   123,   114,   115,   116,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    55,    56,     0,     0,
       0,     0,     0,   117,    28,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    34,     0,     0,     0,     0,    58,
       0,     0,    42,    43,    44,   149,   118,    47,     0,     0,
       0,    61,     0,     0,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   190,   123,   114,
     115,   116,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    55,    56,     0,     0,     0,     0,     0,   117,    28,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    34,
       0,     0,     0,     0,    58,     0,     0,    42,    43,    44,
     151,   118,    47,     0,     0,     0,    61,     0,     0,     0,
       0,    51,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,    55,    56,     0,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,    57,     0,     0,    34,     0,     0,     0,    58,
       0,     0,     0,    42,    43,    44,   118,   191,    47,     0,
       0,    61,     0,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,    57,     0,
       0,    34,     0,     0,     0,    58,     0,     0,     0,    42,
      43,    44,   118,   257,    47,     0,     0,    61,     0,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,   261,     0,    42,    43,    44,     0,   118,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,    55,    56,     0,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,    57,
       0,     0,    34,     0,     0,     0,    58,     0,     0,     0,
      42,    43,    44,   118,   191,    47,     0,     0,    61,     0,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,   118,
     286,     0,     0,     0,    61,     4,     5,     6,     7,     0,
       0,     0,   122,   123,   114,   115,   116,    13,    14,    15,
      16,    17,    18,     0,     0,   352,     0,     0,     0,     0,
       0,     0,     0,   117,    28,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,     0,     0,     0,
       0,     0,    42,    43,    44,     0,     0,    47,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   122,   123,   114,
     115,   116,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    55,    56,     0,     0,     0,     0,     0,   117,    28,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    34,
       0,     0,     0,     0,    58,     0,     0,    42,    43,    44,
       0,   118,    47,     0,     0,     0,    61,     0,     0,     0,
       0,    51,    52,     0,     0,     0,     0,    53,     0,     0,
     194,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   122,   123,   114,   115,   116,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    55,    56,     0,     0,
       0,     0,     0,   117,    28,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    34,     0,     0,     0,     0,    58,
       0,     0,    42,    43,    44,     0,   118,    47,     0,     0,
       0,    61,     0,     0,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   122,   123,
     114,   115,   116,    13,    14,    15,    16,    17,    18,     0,
       0,    55,    56,     0,     0,     0,     0,     0,     0,   117,
      28,     0,     0,     0,     0,     0,     0,    57,     0,     0,
      34,     0,     0,     0,    58,     0,     0,     0,    42,    43,
      44,   118,   578,    47,     0,     0,    61,     0,     0,     0,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,    55,    56,     0,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,    57,     0,     0,    34,     0,     0,     0,
      58,     0,     0,     0,    42,    43,    44,   118,   626,    47,
       0,     0,    61,     0,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,    55,    56,     0,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,    57,
       0,     0,    34,     0,     0,     0,    58,     0,     0,     0,
      42,    43,    44,   118,   629,    47,     0,     0,    61,     0,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,   498,     7,     0,     0,     0,   122,   123,   114,   115,
     499,    13,    14,    15,    16,    17,    18,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,   117,    28,     0,
       0,     0,     0,     0,     0,    57,     0,     0,    34,     0,
       0,     0,    58,     0,     0,     0,    42,    43,    44,   118,
     640,    47,     0,     0,    61,     0,     0,     0,     0,     0,
      51,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   122,   123,   114,   115,   116,    13,    14,    15,
      16,    17,    18,     0,     0,    55,    56,     0,     0,     0,
       0,     0,     0,   117,    28,     0,     0,     0,     0,     0,
       0,    57,     0,     0,    34,     0,     0,     0,    58,     0,
       0,     0,    42,    43,    44,   118,   191,    47,     0,     0,
      61,     0,     0,     0,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   122,   123,
     114,   115,   116,    13,    14,    15,    16,    17,    18,     0,
       0,    55,    56,     0,     0,     0,     0,     0,     0,   117,
      28,     0,     0,     0,     0,     0,     0,    57,     0,     0,
      34,     0,     0,     0,    58,     0,     0,     0,    42,    43,
      44,   118,   776,    47,     0,     0,    61,     0,     0,     0,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,    55,    56,     0,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,    57,     0,     0,    34,     0,     0,     0,
      58,     0,     0,     0,    42,    43,    44,   118,   986,    47,
       0,     0,    61,     0,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   135,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   137,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   139,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   141,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   118,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   158,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   160,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   118,    47,
       0,     0,     0,   174,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   259,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,   498,
       7,     0,     0,     0,   122,   123,   114,   115,   499,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   496,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   500,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    34,     0,     0,     0,
       0,    58,     0,     0,    42,    43,    44,     0,   666,    47,
       0,     0,     0,    61,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   112,
     113,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    55,    56,     0,     0,     0,     0,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    34,     0,     0,     0,     0,    58,     0,     0,    42,
      43,    44,     0,   690,    47,     0,     0,     0,    61,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,   118,     0,
       0,     0,     0,    61
};

static const yytype_int16 yycheck[] =
{
       2,   248,   220,     0,   687,   528,    26,    15,    14,    14,
     365,    14,   475,     0,   300,    26,    11,    22,   481,    26,
      22,    23,    24,    25,    26,    38,    39,    29,    30,    31,
      10,    33,    10,    11,    36,    37,    14,    44,    79,    80,
     128,    81,    44,    51,   114,    47,    14,   135,    22,    57,
      58,    22,   103,    26,    22,    10,    11,    81,    60,    14,
     111,    10,    11,    26,   114,    14,    57,    58,    10,    11,
      61,    44,    14,    14,   125,    26,    75,    76,    77,   120,
     120,    44,   132,    10,    11,    12,    10,    14,    66,    67,
      68,    71,    72,    44,    96,    97,    98,    99,   100,   101,
      14,    69,   104,   105,    31,   107,   108,   131,    78,    36,
     134,    66,    67,    68,   134,   134,   118,    66,    67,    68,
      26,   128,   135,   125,    66,    67,    68,   116,   862,   134,
     136,   134,   127,   135,   129,   137,   135,   139,    44,   141,
     114,   824,   134,   114,   135,   134,   114,    71,    72,    63,
      26,   127,   130,   129,   509,   128,   158,   127,   160,   129,
     128,   163,   625,   165,   166,   128,   168,   169,    44,   171,
     172,   173,   174,   175,    26,   130,    81,   128,    81,    12,
     131,   130,    14,    10,    11,    17,   129,    14,   130,    79,
      80,   134,    44,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   129,   205,   206,   207,   208,   209,   210,   211,
     212,    78,   114,   129,   129,   120,    99,   100,   134,   134,
     575,   576,   128,   234,   235,   131,   128,    59,   131,   135,
     120,   134,   240,   241,   242,   243,   244,   245,   246,    66,
      67,    68,    14,    10,    11,   979,   129,    14,    14,   114,
      22,    26,   128,   116,   114,   131,   542,   259,   128,   135,
     127,   131,   129,   314,   129,    31,    10,    11,   319,    44,
      14,   134,   132,   275,   325,   277,   128,   279,   280,   281,
     282,   283,   284,   285,    14,    12,    29,   573,   290,   291,
    1024,   293,    22,   295,  1028,   116,    14,    69,   821,    66,
      67,    68,    45,   130,   306,   307,   308,   309,   310,   311,
     312,   112,    14,   134,   316,   317,   327,   328,   128,   370,
      22,  1004,    66,    67,    68,   135,   131,   128,   133,     2,
     131,   333,   334,   330,   336,   337,  1070,   339,   340,    69,
      26,   343,   114,   345,  1078,    14,    26,    10,   350,   351,
      26,    14,   116,   128,    27,    28,   128,   114,    44,  1093,
     135,    14,   114,   130,    44,    38,    39,    69,    44,    26,
     134,   128,    45,    46,    10,    26,   378,   379,   380,   381,
     132,   112,   114,   669,   114,    10,   130,    44,    26,  1123,
     116,    29,    30,    44,    73,   114,   128,   128,   128,    37,
     131,   452,    40,    41,   622,   106,    44,    45,   134,   460,
     129,   114,   114,   111,   465,   114,    26,   468,   704,    29,
      30,   128,   473,   113,   475,   128,   128,    37,   135,   128,
      40,    41,    26,   128,    44,    29,    30,    57,    58,   127,
     135,    61,   128,    37,   117,   131,    40,    41,   128,   135,
      44,    45,   128,   739,   740,   135,    10,   459,    14,   135,
      14,   463,   114,   514,   466,   467,     5,   140,   127,   142,
     143,   128,   523,   128,   476,    14,   128,   128,   135,   481,
     135,   131,   128,   133,   135,   128,   159,   734,   161,   135,
     128,    14,   135,   128,   496,   128,   128,   135,   500,   128,
     135,   503,   135,   505,    14,   507,   135,    10,    11,   511,
     512,    14,    10,    11,   132,   135,    14,   128,   128,   128,
     128,   131,   195,   128,   135,   135,   135,   529,   128,   815,
     135,   128,   118,   119,   128,   135,    26,   128,   135,    29,
      30,   135,   544,    14,   135,   128,   128,    37,   128,   128,
      40,    41,   135,   135,    44,   135,   135,   129,   128,   128,
     562,   131,   134,    66,    67,    68,   135,    10,    66,    67,
      68,   128,   623,    26,   625,   134,    29,    30,   580,   114,
     631,   583,   584,   585,    37,   871,   872,    40,    41,   591,
     592,    44,   594,   595,   114,   597,   598,   599,   600,   601,
     602,   603,   604,   958,    54,    26,   961,   658,    29,    30,
     128,    26,   134,   129,    29,    30,    37,   135,   134,    40,
      41,   868,    37,    44,    45,    40,    41,   128,    26,    44,
     129,    29,    30,   128,   135,   637,   638,   639,   128,    37,
     135,   131,    40,    41,   646,   135,    44,    45,    86,    87,
      88,    89,    90,    91,   656,   134,   128,   659,   660,   135,
      98,   128,    26,   135,   666,    29,    30,   128,   107,   108,
     109,   673,   110,    37,   135,   128,    40,    41,   131,   128,
      44,    45,   135,    14,   123,   124,   125,   128,   690,   362,
     131,    22,   128,    24,    25,  1050,   131,   699,   133,   135,
     128,   128,    33,    34,   706,   707,   708,   128,   135,   128,
     128,    42,    43,   128,   135,   717,   131,   135,   128,    50,
      51,    52,    10,    11,    12,   135,   128,   729,    59,    14,
     128,    40,    41,   135,   128,   128,   128,    22,   128,    24,
      25,   135,   135,   135,   128,   135,   128,   131,    33,    34,
     801,   753,   754,   755,   756,   757,   758,    42,    43,   128,
     128,   812,   128,   131,   128,    50,    51,    52,   117,   135,
      79,    80,   135,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,   128,   128,   128,    98,
       7,     8,     9,   135,   135,   135,   798,   106,   107,   108,
     109,   110,   111,   131,   113,   133,   128,   116,   117,   118,
     119,   120,   121,   135,   123,   124,   125,   128,   127,   129,
      99,   100,   101,   102,   103,   104,   105,   878,   830,   128,
     128,   128,   128,   128,    14,   114,   135,   135,   135,   135,
     135,    14,   844,   845,   846,   847,   848,   849,   850,   851,
     129,   130,   131,    14,   527,   528,    17,   859,   531,   532,
     533,    22,    10,    24,    25,    13,   917,    15,    16,   129,
     921,   544,    33,    34,   134,   877,    82,    83,    84,    85,
     129,    42,    43,   556,   557,   558,    92,    93,    94,    50,
      51,    52,    97,   128,    99,   100,   101,   102,   103,   104,
     105,    99,   100,   101,   102,   103,   104,   105,   910,   114,
     116,   117,    10,   915,   916,    10,   114,   114,    13,    14,
      15,    16,   127,    14,   129,   128,   128,   128,   131,   131,
     131,   129,   934,   114,   121,   122,   938,   939,   940,   128,
     128,    27,   131,   131,   231,   232,   233,   214,   215,   216,
     217,   218,   219,   955,   221,   222,   223,   224,   225,   226,
     227,   228,   959,    10,   966,     3,     4,     5,     6,   971,
     130,   131,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   130,   131,   128,   988,   989,   128,   129,
      14,   130,   131,    31,    32,   121,   122,   130,   131,   130,
     131,   130,   131,    14,    42,   135,  1008,   130,   131,   130,
     131,   128,    50,    51,    52,   130,   131,    55,   691,   130,
     131,    27,    28,   130,   131,  1027,    64,    65,   130,   131,
     130,   131,    70,   130,   131,   128,   130,   131,   114,  1041,
     130,   131,   130,   131,  1046,   718,    97,   128,    99,   100,
     101,   102,   103,   104,   105,   130,   131,   130,   131,   130,
     131,    99,   100,   114,    10,  1067,   130,   131,   130,   131,
    1072,  1073,  1074,   130,   131,   128,   127,   115,   129,   114,
     130,   131,   130,   131,   122,   130,   131,   130,   131,   130,
     131,   129,   130,    27,    28,   129,   134,   129,   136,   130,
     131,   130,   131,   128,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,   130,   131,   130,   131,   130,   131,   130,   131,
     130,   131,   110,  1125,  1126,  1127,   130,   131,   128,     3,
       4,     5,     6,    27,    28,   128,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    95,   821,   130,
     131,   128,   825,   826,   827,   130,   131,    31,    32,   130,
     131,    27,    28,   130,   131,   130,   131,   129,    42,   130,
     131,    43,    44,   238,   239,   128,    50,    51,    52,   128,
     853,    55,   120,   128,   857,   128,   128,   128,   128,   128,
      64,    65,   128,   128,    14,   128,    70,   128,   114,   129,
     114,   114,   129,   135,   128,   117,   128,    78,   128,   114,
     128,   128,   135,   128,   117,   134,    14,   128,   128,   128,
     128,   128,   128,   114,   128,    99,   100,   128,   128,   128,
     128,   128,   117,   135,    14,   128,   128,    14,   128,   128,
     128,   115,    78,    14,   128,   128,   133,   133,   122,   128,
     128,    10,    10,   926,   927,   129,   130,    10,    14,   114,
     134,    10,   136,     3,     4,     5,     6,    10,   128,   128,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    27,   128,    57,    14,    14,    14,   128,   135,   128,
     128,    31,    32,   128,   128,    78,   128,   130,   129,   128,
      78,   128,    42,   128,   128,   135,   128,   128,     7,     7,
      50,    51,    52,     7,   114,    55,   128,   114,    14,   114,
      10,    10,   130,    73,    64,    65,    14,    14,    14,   128,
      70,    14,   128,   135,   128,   128,    14,   128,   130,   128,
      14,   128,   128,   128,   134,    14,   128,   135,    14,   128,
    1023,   135,   128,   128,  1027,   128,   128,  1067,   128,    99,
     100,   128,   128,   128,   128,   128,   500,   571,   130,   230,
     237,   236,    -1,   134,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,  1061,   129,
     130,    -1,    -1,    -1,   134,    -1,   136,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    51,    52,    -1,    -1,    55,
      -1,  1124,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,   129,     3,     4,     5,     6,   134,    -1,
     136,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
      -1,    -1,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    -1,
      -1,    60,    61,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,   128,
     129,     3,     4,     5,     6,   134,   135,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    60,    61,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,   128,   129,     3,     4,
       5,     6,   134,   135,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    -1,    -1,    -1,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    -1,    60,    61,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    74,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,   128,   129,    55,    -1,    -1,    -1,   134,
     135,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,   128,   129,
      55,    -1,    -1,    -1,   134,   135,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,   128,   129,    55,    -1,    -1,    -1,   134,
     135,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,   128,   129,
       3,     4,     5,     6,   134,   135,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    -1,    -1,    60,    61,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    74,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    50,    51,    52,   128,   129,    55,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    42,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,    52,
     128,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    50,    51,    52,   128,   129,    55,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    42,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,    52,
     128,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    42,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    50,    51,    52,   129,   130,    55,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,
      51,    52,   129,   130,    55,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,   123,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      50,    51,    52,   129,   130,    55,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,    -1,    -1,    -1,   134,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    51,    52,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    42,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,    52,
      -1,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      73,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    50,    51,    52,    -1,   129,    55,    -1,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,    51,
      52,   129,   130,    55,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    50,    51,    52,   129,   130,    55,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      50,    51,    52,   129,   130,    55,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    42,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    50,    51,    52,   129,
     130,    55,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    42,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    50,    51,    52,   129,   130,    55,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,    51,
      52,   129,   130,    55,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    50,    51,    52,   129,   130,    55,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,    55,
      -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,
      51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,   134
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   138,   139,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    60,
      61,    64,    65,    70,    74,    99,   100,   115,   122,   128,
     129,   134,   140,   141,   143,   144,   145,   146,   147,   149,
     150,   152,   153,   156,   157,   160,   165,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     182,   183,   185,    97,    99,   100,   101,   102,   103,   104,
     105,   114,   127,   129,   114,   132,   116,   134,   114,    11,
     127,   129,    10,    11,    12,    13,    14,    31,   129,   165,
     178,    14,    10,    11,   128,   129,   167,   184,    10,    11,
      12,    14,    31,    36,   129,   129,   167,   129,   167,   129,
     167,   129,   167,   167,   174,    14,   165,   165,   167,   128,
     167,   128,   167,    10,    14,   167,    12,    12,   129,   167,
     129,   167,   165,   129,   165,    26,    44,   128,    26,    44,
     128,   134,   166,   166,   134,   166,   167,   165,   165,   167,
      14,    14,    14,    31,   178,    14,    10,    10,   178,   178,
      10,   130,   167,   180,    73,   139,    26,    29,    30,    37,
      40,    41,    44,    45,   128,    26,    29,    30,    37,    40,
      41,    44,    45,   128,    82,    83,    84,    85,    92,    93,
      94,   116,   117,    86,    87,    88,    89,    90,    91,    98,
     110,   118,   119,   106,    79,    80,   120,   111,   121,   122,
     107,   108,   109,   123,   124,   125,   113,   127,   127,    14,
     167,   167,   167,   167,   167,   167,    14,   130,   184,   129,
     167,   123,   167,    14,   167,   167,   128,    14,    17,    59,
      14,   130,   136,   162,   184,   132,   116,   134,   128,   101,
     102,   103,   104,   105,   114,   114,   130,   167,   184,   128,
     131,   114,   128,   114,   128,   114,   128,    10,    11,    12,
      14,    10,    14,    10,   180,   167,    26,    29,    30,    37,
      40,    41,    44,   128,   131,   167,    26,    44,   128,   131,
     167,   165,   167,   165,    81,   131,   165,    79,    80,   129,
     134,   154,   128,    26,    44,   128,    26,    44,   128,   114,
     114,   134,   116,   134,   116,   134,   167,   165,   167,   165,
      29,    45,    23,   167,   167,   167,   167,   167,   167,   167,
     167,   167,    54,   158,   134,   129,    14,    63,   134,   130,
     131,   130,   131,   135,    10,    11,    12,    13,    24,    25,
      33,    34,    48,    49,   135,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   170,   170,   170,   170,   170,   170,   168,   170,
     170,   170,   170,   170,   170,   170,   170,     7,     8,     9,
     171,   173,   173,   173,   174,   174,   175,   176,   177,   177,
     178,   178,   178,   178,   178,   178,   178,    14,    22,    24,
      25,    33,    34,    42,    43,    50,    51,    52,   181,    17,
     181,   128,   129,   128,   128,   128,   128,   128,   128,   114,
     129,   130,   167,   112,   128,   131,   121,   122,   131,   133,
     117,   135,   128,   129,    14,   129,    78,    14,   130,   131,
     130,   131,   167,    14,   167,   167,   167,   167,   167,   167,
     167,   167,   128,   130,   167,   167,   129,   167,     5,    14,
     129,   163,   167,   114,   128,   114,   128,   114,   128,   129,
     154,   114,   114,   130,   131,   167,   167,   167,   167,   167,
     167,   167,   184,   131,   167,   167,   184,   130,    27,    28,
     142,   130,    10,   184,   174,   174,    10,    11,    14,    66,
      67,    68,   130,   164,   139,   167,   167,   167,   167,   167,
     167,   151,    14,   167,    14,   167,   130,    27,   130,   167,
     167,    10,   128,   128,   128,   128,   128,   135,   165,    14,
      22,   134,   159,   130,   164,   129,   129,   161,   130,   184,
     114,    10,   127,   132,   134,   114,   167,   167,   167,   135,
     167,    26,    44,   135,    26,    44,   135,    26,    29,    30,
      37,    40,    41,    44,    45,   135,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,    95,   129,    14,   129,   130,   184,   167,   130,
     184,   131,   167,   184,   167,   167,   184,   114,   114,   114,
     130,   184,   130,   162,   184,   167,   129,    14,   136,   162,
     133,   117,   135,   128,   128,   167,   112,   128,   131,    78,
      78,   163,   128,   131,   128,   167,   129,   167,   167,   130,
     164,   167,   167,   114,   184,   128,   128,   128,   128,   128,
     128,   128,   128,   184,   128,   128,   128,   165,   141,   165,
     129,   167,   165,   165,    81,   165,    81,    81,    22,   114,
      10,    71,    72,   154,   130,   131,    24,    25,    34,   135,
     167,   128,   128,   128,   128,   128,   128,    38,    39,   135,
     117,   135,   117,   135,   165,   165,   165,   128,   128,   114,
     167,    14,    14,   159,    57,    58,    61,   135,   154,   130,
     130,   164,   164,    75,    76,    77,   135,   130,   167,    14,
     167,   167,   167,    26,    30,    37,    40,    41,    44,   135,
     135,   135,   135,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   168,   130,   184,   130,   162,
     184,   128,   130,   128,   130,   184,   128,   128,   133,   133,
     133,   167,   167,   167,   130,   130,   130,   167,    78,    14,
     130,   131,   167,   184,   167,   167,   130,     5,    14,   128,
     167,   128,   131,   128,   154,   130,   128,   128,   167,   130,
     130,    27,   142,   167,   165,    10,    10,    10,    14,   167,
     114,    10,    10,   154,    10,    11,    14,    66,    67,    68,
     167,   167,   135,   167,    26,    29,    30,    37,    40,    41,
      44,    45,   135,   167,   165,   128,   128,    27,   167,   128,
     135,    59,   181,    10,    13,    14,    15,    16,    57,   154,
     154,   130,   130,    14,    14,    14,   128,   114,   129,   133,
     135,   135,   167,   167,   167,   167,   167,   167,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     130,   130,   130,   128,   130,   128,   128,   128,   130,   167,
     129,   184,   128,   128,   128,    78,    78,   131,   184,   154,
     128,   131,   128,   128,   141,   165,   130,    27,   142,   165,
     165,   165,   167,    22,   114,    10,    71,    72,    26,    30,
      44,   135,   135,   135,   167,   167,   167,   167,   167,   167,
     167,   167,   165,   135,   165,   128,   148,   167,   129,   134,
     155,   129,   155,    14,    22,    69,   114,   128,    14,    22,
      69,   114,   128,    10,    13,    15,    16,   128,   128,   181,
     154,   154,     7,     7,     7,   167,   130,   184,   114,   114,
     135,   135,   135,   135,   135,   135,   128,   167,   130,   167,
     167,   184,   128,   184,   165,   165,    14,   167,   114,    10,
      10,   167,   167,   167,   135,   135,   135,   135,   135,   135,
     135,   135,   167,   130,   130,   164,    73,   139,   130,   164,
      14,    14,   128,   167,    14,    14,   128,   167,    14,    22,
      69,   114,   128,    14,    22,    69,   114,   128,   128,   128,
     129,   155,   135,   135,   130,   167,   167,   130,   128,   130,
     128,    27,   142,   167,   135,   135,   135,   128,   165,   155,
     130,   135,    24,    25,    34,   135,   167,   155,   130,   128,
     128,   128,   128,   128,   128,    14,    14,   128,   167,    14,
      14,   128,   167,   130,   164,   135,   135,   135,   128,   165,
     148,   155,   167,   167,   135,   167,    26,    29,    30,    37,
      40,    41,    44,    45,   135,   155,   128,   128,   128,   128,
     128,   128,   155,   130,   130,    26,    30,    44,   135,   135,
     135,   167,   167,   167,   167,   167,   167,   167,   167,   155,
     165,   167,   167,   167,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   137,   138,   139,   139,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   141,   141,   141,   141,   141,   141,   141,   141,
     142,   142,   142,   142,   142,   142,   143,   143,   144,   144,
     144,   144,   145,   145,   146,   146,   146,   147,   147,   147,
     148,   149,   149,   149,   149,   149,   149,   150,   150,   151,
     151,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   153,   153,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     157,   158,   158,   158,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   160,   161,
     161,   161,   161,   162,   162,   162,   162,   163,   163,   163,
     163,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   166,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   171,
     171,   171,   172,   172,   173,   173,   173,   174,   174,   175,
     175,   176,   176,   176,   177,   177,   177,   177,   177,   177,
     177,   178,   178,   178,   178,   178,   179,   179,   180,   180,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   182,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   184,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     7,     9,     4,     6,     6,     8,
       5,     5,     7,     6,     6,     8,    10,     6,     4,     4,
       4,     3,     5,     5,     6,     6,     2,     5,     4,     3,
       3,     3,     5,     7,     3,     5,     7,     3,     3,     2,
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
       3,     3,     3,     3,     3,     2,     1,     5,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     1,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     4,     6,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     4,     6,
       6,     6,     1,     4,     4,     5,     5,     1,     1,     1,
       1,     2,     3,     4,     5,     1,     2
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
#line 455 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3064 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 465 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3070 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 466 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3076 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 470 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3082 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 472 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3088 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 474 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3094 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 476 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3102 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 480 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3108 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 482 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3114 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 484 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3120 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 486 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3126 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 488 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3134 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 492 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3141 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 495 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3149 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 499 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3155 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 501 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3163 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 505 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3170 "raku.tab.c"
    break;

  case 19: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 508 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3178 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 512 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3184 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 514 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3190 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 516 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3196 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 518 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3202 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 520 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3208 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 522 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3216 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 526 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3224 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 530 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3230 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 532 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3236 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 534 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3242 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 536 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3248 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE IDENT ';'  */
#line 538 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3254 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 540 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3260 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 542 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3266 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 544 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3272 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 546 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3278 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 548 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3284 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 550 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3290 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 552 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3296 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 554 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3302 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 556 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3308 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 558 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3314 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 560 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3321 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 563 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3328 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 566 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3334 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 568 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3341 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 571 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3348 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 574 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3354 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 576 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3360 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 578 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3366 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 580 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3373 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 583 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3380 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 586 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3386 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 588 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3393 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 591 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3400 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 594 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3407 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 597 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3413 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 599 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3422 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 604 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3430 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 608 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3438 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 612 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3447 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 617 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3455 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall ';'  */
#line 620 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3461 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 622 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3467 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3473 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 626 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3479 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3485 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 630 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3492 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 633 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3498 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 635 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3504 "raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 637 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3510 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 639 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3517 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 642 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3524 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 645 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3531 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3538 "raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3545 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_IF expr ';'  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3551 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNLESS expr ';'  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3557 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WHILE expr ';'  */
#line 658 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3563 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNTIL expr ';'  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3569 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_FOR expr ';'  */
#line 662 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3576 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITH expr ';'  */
#line 665 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3582 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITHOUT expr ';'  */
#line 667 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3588 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_GIVEN expr ';'  */
#line 669 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3594 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 671 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3601 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 674 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3608 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 677 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3616 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 681 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3623 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 684 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3629 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 686 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3635 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 688 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3641 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 690 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3648 "raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 693 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3655 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 696 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3662 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 699 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3669 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 702 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3676 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 705 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3683 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 708 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3690 "raku.tab.c"
    break;

  case 98: /* stmt: expr ';'  */
#line 710 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3696 "raku.tab.c"
    break;

  case 99: /* stmt: ';'  */
#line 711 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3702 "raku.tab.c"
    break;

  case 100: /* stmt: if_stmt  */
#line 712 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3708 "raku.tab.c"
    break;

  case 101: /* stmt: while_stmt  */
#line 713 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3714 "raku.tab.c"
    break;

  case 102: /* stmt: for_stmt  */
#line 714 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3720 "raku.tab.c"
    break;

  case 103: /* stmt: given_stmt  */
#line 715 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3726 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block  */
#line 717 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3732 "raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block KW_CATCH block  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3738 "raku.tab.c"
    break;

  case 106: /* stmt: KW_CATCH block  */
#line 721 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3744 "raku.tab.c"
    break;

  case 107: /* stmt: block  */
#line 723 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3750 "raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 724 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3756 "raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 725 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3762 "raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 726 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3768 "raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 727 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3774 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 728 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3780 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 729 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3786 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 731 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3792 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 733 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3798 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 735 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3804 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 737 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3810 "raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 738 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3816 "raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 739 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3822 "raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 740 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3828 "raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 741 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3834 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block  */
#line 745 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3840 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 747 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3846 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 749 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3852 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block  */
#line 751 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3858 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 753 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3864 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 755 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3870 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block elsif_tail  */
#line 757 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3876 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 759 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3882 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block  */
#line 763 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3888 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 765 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3894 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 767 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3900 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 769 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3906 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 771 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3912 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 773 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3918 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 777 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3924 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE expr block  */
#line 779 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3930 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 783 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3936 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 785 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3942 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3948 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 789 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3954 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 793 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3960 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL expr block  */
#line 795 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3966 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block  */
#line 799 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3972 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 801 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3978 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 803 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3984 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP block  */
#line 807 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3990 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 809 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3996 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 811 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4002 "raku.tab.c"
    break;

  case 150: /* loop_incr: expr  */
#line 814 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4008 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 818 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4018 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 824 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4028 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 830 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4037 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 835 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4047 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 841 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4056 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr block  */
#line 846 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4063 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 851 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4075 "raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 859 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4088 "raku.tab.c"
    break;

  case 159: /* when_list: %empty  */
#line 869 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4094 "raku.tab.c"
    break;

  case 160: /* when_list: when_list KW_WHEN expr block  */
#line 871 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4102 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 877 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4114 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 885 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4124 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT sub_body  */
#line 891 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4134 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 897 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4146 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 905 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4156 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 911 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4166 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 917 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4179 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 926 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4190 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 933 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4203 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 942 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4216 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 951 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4227 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 960 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4238 "raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 967 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4247 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list '}'  */
#line 973 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4253 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr '}'  */
#line 975 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4260 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 978 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4267 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 981 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4273 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 983 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4279 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 985 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4285 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 987 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4291 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 989 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4297 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 991 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4303 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 993 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4309 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 995 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4316 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 998 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4322 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1000 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4328 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1002 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4334 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1004 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4341 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1007 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4348 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1010 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4356 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list '}'  */
#line 1015 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4362 "raku.tab.c"
    break;

  case 192: /* method_body: '{' YADA '}'  */
#line 1016 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4368 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1018 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4375 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1021 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4382 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1024 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4388 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1026 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4394 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1028 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4400 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1030 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4406 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1032 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4412 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1034 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4418 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4424 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1038 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4431 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1041 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4437 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1043 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4443 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1045 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4449 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1047 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4456 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1050 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4463 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1053 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4471 "raku.tab.c"
    break;

  case 209: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1059 "raku.y"
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
#line 4489 "raku.tab.c"
    break;

  case 210: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1075 "raku.y"
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
#line 4506 "raku.tab.c"
    break;

  case 211: /* is_clauses: %empty  */
#line 1089 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4512 "raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses IDENT IDENT  */
#line 1091 "raku.y"
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
#line 4528 "raku.tab.c"
    break;

  case 213: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1103 "raku.y"
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
#line 4544 "raku.tab.c"
    break;

  case 214: /* class_body_list: %empty  */
#line 1116 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4550 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1118 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4557 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1121 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4564 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1124 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4571 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1127 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4578 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1130 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4585 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1133 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4592 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1136 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4599 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1139 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4606 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1142 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4613 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1145 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4620 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1148 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4627 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1151 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4634 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1154 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4645 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1161 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4656 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1168 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4667 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1175 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4678 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1182 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4686 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1186 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4694 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1190 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4702 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1194 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4710 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1198 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4721 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1205 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4732 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1212 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4743 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1219 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4754 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1226 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4768 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1236 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4780 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1244 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4792 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1252 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4805 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1261 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4816 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1268 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4827 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1275 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4841 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1285 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4853 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1293 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4865 "raku.tab.c"
    break;

  case 248: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1303 "raku.y"
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
#line 4882 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: %empty  */
#line 1317 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4888 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1319 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4897 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1324 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4906 "raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1329 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4915 "raku.tab.c"
    break;

  case 253: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1336 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4923 "raku.tab.c"
    break;

  case 254: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1340 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4931 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1344 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4939 "raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1348 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4947 "raku.tab.c"
    break;

  case 257: /* pair_list: IDENT OP_FATARROW expr  */
#line 1354 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4953 "raku.tab.c"
    break;

  case 258: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1356 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4959 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1358 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4965 "raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1360 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4971 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR  */
#line 1363 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4977 "raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1364 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 4983 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1365 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 4989 "raku.tab.c"
    break;

  case 264: /* param_list: VAR_ARRAY  */
#line 1366 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4995 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_ARRAY  */
#line 1367 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5001 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT VAR_SCALAR  */
#line 1368 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5007 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1369 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5013 "raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1370 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5019 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_SCALAR  */
#line 1371 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5025 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1372 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5031 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1373 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5037 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1374 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5043 "raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR '=' expr  */
#line 1375 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5049 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1376 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5055 "raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1377 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5061 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1378 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5067 "raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_POS  */
#line 1379 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5073 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_POS  */
#line 1380 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5079 "raku.tab.c"
    break;

  case 279: /* param_list: SLURPY_LOL  */
#line 1381 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5085 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' SLURPY_LOL  */
#line 1382 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5091 "raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_NAMED  */
#line 1383 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5097 "raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1384 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5103 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list '}'  */
#line 1387 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5109 "raku.tab.c"
    break;

  case 284: /* block: '{' YADA '}'  */
#line 1388 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5115 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr '}'  */
#line 1390 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5121 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1392 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5127 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1394 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5133 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5139 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1398 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5145 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1400 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5152 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1403 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5158 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1405 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5164 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1407 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5170 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1409 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5176 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1411 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5183 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1414 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5190 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1417 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5198 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1421 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5205 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1424 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5212 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1427 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5219 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1430 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5225 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1432 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5233 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1436 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5240 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1439 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5247 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1442 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5254 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1445 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5261 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1448 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5268 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1451 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5274 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1453 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5280 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1455 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5286 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_LAST '}'  */
#line 1457 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5292 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1459 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5298 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1461 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5304 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1463 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5310 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1465 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5316 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1467 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5322 "raku.tab.c"
    break;

  case 317: /* closure: '{' expr '}'  */
#line 1470 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5328 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR '=' expr  */
#line 1473 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5334 "raku.tab.c"
    break;

  case 319: /* expr: VAR_ARRAY '=' expr  */
#line 1474 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5340 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1475 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5346 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1476 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5352 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1477 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5358 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1478 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5364 "raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1479 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5370 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER block  */
#line 1480 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5380 "raku.tab.c"
    break;

  case 326: /* expr: tern_expr  */
#line 1485 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5386 "raku.tab.c"
    break;

  case 327: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1489 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5392 "raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr  */
#line 1490 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5398 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1493 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5404 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1494 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5410 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1495 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5416 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1496 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5422 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1497 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5428 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1498 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5434 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1499 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5440 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1500 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5446 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1501 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5452 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1502 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5458 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1503 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5464 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1504 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5470 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1505 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5476 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1506 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5482 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1508 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5492 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1514 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5502 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1520 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5512 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr  */
#line 1525 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5518 "raku.tab.c"
    break;

  case 347: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1528 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5524 "raku.tab.c"
    break;

  case 348: /* divis_expr: jct_expr  */
#line 1529 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5530 "raku.tab.c"
    break;

  case 349: /* jct_expr: jct_expr '|' range_expr  */
#line 1532 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5536 "raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '&' range_expr  */
#line 1533 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5542 "raku.tab.c"
    break;

  case 351: /* jct_expr: dor_expr  */
#line 1534 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5548 "raku.tab.c"
    break;

  case 352: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1538 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5554 "raku.tab.c"
    break;

  case 353: /* dor_expr: range_expr  */
#line 1539 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5560 "raku.tab.c"
    break;

  case 354: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1542 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5566 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1543 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5572 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr  */
#line 1544 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5578 "raku.tab.c"
    break;

  case 357: /* add_expr: add_expr '~' repl_expr  */
#line 1547 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5584 "raku.tab.c"
    break;

  case 358: /* add_expr: repl_expr  */
#line 1548 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5590 "raku.tab.c"
    break;

  case 359: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1551 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5596 "raku.tab.c"
    break;

  case 360: /* repl_expr: addsub_expr  */
#line 1552 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5602 "raku.tab.c"
    break;

  case 361: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1555 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5608 "raku.tab.c"
    break;

  case 362: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1556 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5614 "raku.tab.c"
    break;

  case 363: /* addsub_expr: mul_expr  */
#line 1557 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5620 "raku.tab.c"
    break;

  case 364: /* mul_expr: mul_expr '*' unary_expr  */
#line 1560 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5626 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr '/' unary_expr  */
#line 1561 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5632 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '%' unary_expr  */
#line 1562 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5638 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1563 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5644 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1565 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5650 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1567 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5656 "raku.tab.c"
    break;

  case 370: /* mul_expr: unary_expr  */
#line 1568 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5662 "raku.tab.c"
    break;

  case 371: /* unary_expr: '-' unary_expr  */
#line 1571 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5668 "raku.tab.c"
    break;

  case 372: /* unary_expr: '!' unary_expr  */
#line 1572 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5674 "raku.tab.c"
    break;

  case 373: /* unary_expr: CARET unary_expr  */
#line 1573 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5680 "raku.tab.c"
    break;

  case 374: /* unary_expr: OP_REDUCE unary_expr  */
#line 1575 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5689 "raku.tab.c"
    break;

  case 375: /* unary_expr: pow_expr  */
#line 1579 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5695 "raku.tab.c"
    break;

  case 376: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1582 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5701 "raku.tab.c"
    break;

  case 377: /* pow_expr: postfix_expr  */
#line 1583 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5707 "raku.tab.c"
    break;

  case 378: /* scalar_list: VAR_SCALAR  */
#line 1586 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5713 "raku.tab.c"
    break;

  case 379: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1587 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5719 "raku.tab.c"
    break;

  case 380: /* meth_name: IDENT  */
#line 1590 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5725 "raku.tab.c"
    break;

  case 381: /* meth_name: KW_SORT  */
#line 1591 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5731 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_MAP  */
#line 1592 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5737 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_GREP  */
#line 1593 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5743 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_SAY  */
#line 1594 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5749 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_PRINT  */
#line 1595 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5755 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_TAKE  */
#line 1596 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5761 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_RETURN  */
#line 1597 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5767 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_EXISTS  */
#line 1598 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5773 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_DELETE  */
#line 1599 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5779 "raku.tab.c"
    break;

  case 390: /* meth_name: TESTOP  */
#line 1600 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5785 "raku.tab.c"
    break;

  case 391: /* postfix_expr: call_expr  */
#line 1602 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5791 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '(' arg_list ')'  */
#line 1605 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5800 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '(' ')'  */
#line 1609 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5806 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1611 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5812 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1613 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5818 "raku.tab.c"
    break;

  case 396: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1615 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5827 "raku.tab.c"
    break;

  case 397: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1620 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5833 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1622 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5843 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1628 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5853 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1634 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5861 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' KW_NEW  */
#line 1638 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5869 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' IDENT  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5878 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1647 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5889 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1654 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5898 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' CARET IDENT  */
#line 1659 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5908 "raku.tab.c"
    break;

  case 406: /* call_expr: atom '.' CARET IDENT  */
#line 1665 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5918 "raku.tab.c"
    break;

  case 407: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1671 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5929 "raku.tab.c"
    break;

  case 408: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1678 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5940 "raku.tab.c"
    break;

  case 409: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1685 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5949 "raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' meth_name  */
#line 1690 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5958 "raku.tab.c"
    break;

  case 411: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1695 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5969 "raku.tab.c"
    break;

  case 412: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1702 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5978 "raku.tab.c"
    break;

  case 413: /* call_expr: call_expr '.' meth_name  */
#line 1707 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5987 "raku.tab.c"
    break;

  case 414: /* call_expr: KW_DIE expr  */
#line 1712 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5993 "raku.tab.c"
    break;

  case 415: /* call_expr: KW_MAP closure expr  */
#line 1714 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5999 "raku.tab.c"
    break;

  case 416: /* call_expr: KW_GREP closure expr  */
#line 1716 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6005 "raku.tab.c"
    break;

  case 417: /* call_expr: KW_SORT expr  */
#line 1718 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6011 "raku.tab.c"
    break;

  case 418: /* call_expr: KW_SORT closure expr  */
#line 1720 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6017 "raku.tab.c"
    break;

  case 419: /* call_expr: atom  */
#line 1721 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6023 "raku.tab.c"
    break;

  case 420: /* arg_list: expr  */
#line 1724 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6029 "raku.tab.c"
    break;

  case 421: /* arg_list: arg_list ',' expr  */
#line 1725 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6035 "raku.tab.c"
    break;

  case 422: /* arg_list: arg_list ','  */
#line 1726 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6041 "raku.tab.c"
    break;

  case 423: /* atom: LIT_INT  */
#line 1729 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6047 "raku.tab.c"
    break;

  case 424: /* atom: LIT_FLOAT  */
#line 1730 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6053 "raku.tab.c"
    break;

  case 425: /* atom: LIT_STR  */
#line 1731 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6059 "raku.tab.c"
    break;

  case 426: /* atom: WORDLIST  */
#line 1733 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6072 "raku.tab.c"
    break;

  case 427: /* atom: LIT_INTERP_STR  */
#line 1741 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6078 "raku.tab.c"
    break;

  case 428: /* atom: VAR_SCALAR  */
#line 1742 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6084 "raku.tab.c"
    break;

  case 429: /* atom: OP_INC VAR_SCALAR  */
#line 1743 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6090 "raku.tab.c"
    break;

  case 430: /* atom: OP_DEC VAR_SCALAR  */
#line 1744 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6096 "raku.tab.c"
    break;

  case 431: /* atom: VAR_SCALAR OP_INC  */
#line 1745 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6102 "raku.tab.c"
    break;

  case 432: /* atom: VAR_SCALAR OP_DEC  */
#line 1746 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6108 "raku.tab.c"
    break;

  case 433: /* atom: VAR_ARRAY  */
#line 1747 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6114 "raku.tab.c"
    break;

  case 434: /* atom: VAR_HASH  */
#line 1748 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6120 "raku.tab.c"
    break;

  case 435: /* atom: VAR_CAPTURE  */
#line 1750 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6128 "raku.tab.c"
    break;

  case 436: /* atom: VAR_FH  */
#line 1754 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6136 "raku.tab.c"
    break;

  case 437: /* atom: VAR_NAMED_CAPTURE  */
#line 1758 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6143 "raku.tab.c"
    break;

  case 438: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1761 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6149 "raku.tab.c"
    break;

  case 439: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1763 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6155 "raku.tab.c"
    break;

  case 440: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1765 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6161 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1767 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6167 "raku.tab.c"
    break;

  case 442: /* atom: ARR_ALL_SLICE  */
#line 1769 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6173 "raku.tab.c"
    break;

  case 443: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1771 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6179 "raku.tab.c"
    break;

  case 444: /* atom: VAR_HASH '{' expr '}'  */
#line 1773 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6185 "raku.tab.c"
    break;

  case 445: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1775 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6191 "raku.tab.c"
    break;

  case 446: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1777 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6197 "raku.tab.c"
    break;

  case 447: /* atom: IDENT  */
#line 1778 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6203 "raku.tab.c"
    break;

  case 448: /* atom: VAR_TWIGIL  */
#line 1780 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6211 "raku.tab.c"
    break;

  case 449: /* atom: VAR_ARRAY_TWIGIL  */
#line 1784 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6219 "raku.tab.c"
    break;

  case 450: /* atom: VAR_HASH_TWIGIL  */
#line 1788 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6227 "raku.tab.c"
    break;

  case 451: /* atom: '(' ')'  */
#line 1791 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6233 "raku.tab.c"
    break;

  case 452: /* atom: '(' expr ')'  */
#line 1792 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6239 "raku.tab.c"
    break;

  case 453: /* atom: '(' expr ',' ')'  */
#line 1794 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6245 "raku.tab.c"
    break;

  case 454: /* atom: '(' expr ',' arg_list ')'  */
#line 1796 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6252 "raku.tab.c"
    break;

  case 455: /* atom: block  */
#line 1798 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6258 "raku.tab.c"
    break;

  case 456: /* atom: KW_SUB block  */
#line 1799 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6264 "raku.tab.c"
    break;


#line 6268 "raku.tab.c"

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

#line 1801 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
