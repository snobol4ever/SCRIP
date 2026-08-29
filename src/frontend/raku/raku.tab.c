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
#define YYLAST   4739

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  461
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1164

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
    1479,  1480,  1481,  1482,  1483,  1488,  1493,  1496,  1498,  1501,
    1502,  1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,
    1512,  1513,  1514,  1515,  1521,  1527,  1533,  1536,  1537,  1540,
    1541,  1542,  1545,  1547,  1550,  1551,  1552,  1555,  1556,  1559,
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

#define YYPACT_NINF (-842)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-461)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -842,    55,  2118,  -842,  -842,  -842,  -842,  -842,   647,    93,
     133,   -90,    15,  -842,  -842,  4604,  -842,  -842,  -842,    54,
    2193,   222,  3629,  3704,  3779,  3854,  3929,     1,   -11,  3929,
    2268,  2343,    79,   124,  3929,   114,   145,  4004,  4079,    56,
     284,    32,    68,    86,    86,  4154,    56,    56,  3929,   241,
     253,    43,  4604,  -842,  -842,   346,   297,   316,  4604,  4604,
    -842,  2418,  2494,   290,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842,   590,  -842,  -842,  -842,   976,   613,
    -842,   795,   805,   320,   274,  -842,   -41,   549,   341,   811,
    -842,  -842,   322,   276,   296,   433,  -842,  -842,  3929,  3929,
    3929,  3929,  3929,  3929,   472,  2569,  4229,  2645,   488,  3929,
    3929,   403,    92,   768,   -15,   411,   148,  -842,    99,    56,
    2720,  -842,  -842,   435,   864,   261,  -842,  2796,  -842,   125,
     -50,   -48,   278,   804,   554,   310,   572,  2720,   531,  2720,
      57,  2720,    56,  2720,    56,   146,   -39,   285,  -842,  -842,
    -842,   470,  -842,    69,  -842,   143,   492,   502,   581,   505,
     268,   288,  2720,    56,  2720,    56,   263,  2872,  -842,  3929,
    3929,  -842,  3929,  3929,  -842,  3929,  3929,  3929,  2947,  3929,
    -842,   568,  -842,  -842,  -842,   545,   514,    64,  -842,   561,
    -842,  -842,  -842,  -842,   825,  -842,   422,   593,  -842,    47,
     570,  1491,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,
    -842,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  -842,
    4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,
    4604,  4604,  4604,  4604,  4604,   899,  4604,  4604,  4604,  4604,
    4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,  4604,
    4604,  4604,  4604,  4604,   784,   779,   626,   612,   632,   661,
     681,   696,   713,    65,  -842,   638,  2720,   565,   725,   384,
     592,   640,   731,  -842,   752,   886,   757,   -18,  -842,   888,
     734,   812,  2645,   900,  3929,  -842,  3929,  3929,  3929,  3929,
    3929,  3929,  3929,   809,   422,   818,  -842,  3929,  3929,  -842,
    4304,  -842,  4379,  -842,   302,   329,   330,   342,   837,   848,
    -842,   827,   829,  3929,  3929,  3929,  3929,  3929,  3929,  3929,
    -842,  3929,   843,  3929,  3929,  -842,  3929,   860,   970,   869,
    -842,   914,  3929,  -842,  4604,  4604,     2,  -842,  -842,  -842,
    3929,  3929,  -842,  3929,  3929,  -842,  3929,  3929,   813,  -842,
     919,  3929,   958,  3929,   871,   949,   873,  -842,  3929,  3929,
     973,   851,   882,   887,   896,   929,   857,  -842,  -842,  -842,
      56,    16,  -842,    23,   883,   948,  -842,  -842,  3022,   967,
    1089,  3098,  -842,  -842,   884,   293,   311,   988,  3629,  3704,
    3929,  1834,    19,    30,  -842,   333,  1002,  1013,  1025,  1045,
    1055,  1058,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1070,
    1071,  1073,  1059,  1059,  1059,  1059,  1059,  1059,  1107,  1059,
    1059,  1059,  1059,  1059,  1059,  1059,  1059,  -842,  -842,  -842,
     320,  -842,  -842,  -842,  1083,  1083,   549,   341,   341,   811,
     811,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  1075,
    1192,  1077,  -842,  3173,  -842,  -842,  -842,  -842,  -842,  -842,
    3929,  3249,  -842,   875,  -842,  3929,  3929,  3929,  3929,  1093,
    1094,  1097,  -842,  3325,  -842,  1145,  3929,  1084,  -842,    -5,
    -842,  1356,   663,  1095,  1079,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  1088,  -842,  1090,  2720,   614,  1141,   252,
    3401,   660,  1092,  3929,  -842,  4454,  -842,  3929,  -842,    52,
    -842,  3929,  3929,  1103,  3022,  1096,  1098,  1099,  1100,  1101,
    1102,  1104,   662,  3022,  1105,  1106,   673,    56,   -12,  4529,
    -842,    56,    56,   211,   -34,   -33,    -4,  -842,    44,  -842,
    -842,  -842,  1087,   877,  1625,  1108,  1109,  1111,  1112,  1113,
    1118,  -842,    13,  1114,  1115,  1121,  1116,    56,    56,    56,
    1119,  1120,  1135,  3929,  -842,  -842,  -842,  -842,  -842,  -842,
    1209,  1210,  -842,   161,  1087,   904,    62,   388,   225,  -842,
     907,  3929,  -842,  -842,   738,  1212,  2645,  3929,  3929,   371,
     127,   201,  -842,    58,  3929,  3929,  -842,  3929,  3929,  -842,
    3929,  3929,  3929,  3929,  3929,  3929,  3929,  3929,  -842,  -842,
    -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842,  -842,  4604,  3477,  -842,  1280,  1124,
     909,  1125,  -842,   911,  3022,   697,  1122,  1123,   783,  3929,
    3929,  3929,  -842,   913,  -842,   915,   918,  -842,  3929,  1157,
    1241,   920,  -842,  -842,  -842,  -842,  -842,   923,  -842,  3929,
    3929,  3929,   941,  -842,   217,  -842,  1129,  2720,   711,  1130,
    1087,   945,  1131,  1133,  3929,   960,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842,   974,  -842,  -842,  -842,  1080,  -842,
    -842,  2720,    56,  -842,  -842,  1253,  -842,  1254,  1255,  1252,
    3929,  1152,  1259,  1260,  -842,  1087,   456,  3629,  3704,  1909,
    -842,   392,  -842,  -842,  -842,  -842,  -842,  -842,  3929,    56,
    -842,  -842,  -842,  1142,  1143,  1246,  -842,  -842,  -842,  -842,
    3929,  1148,  -842,  -842,   307,   679,   744,  1216,  -842,  -842,
    1087,  1087,   978,   980,  1265,  1267,  1273,  -842,  -842,  1159,
    -842,   280,   800,  1153,   312,  3929,  3929,  3929,  3929,  3929,
    3929,  -842,  -842,  -842,  -842,   313,   351,   356,   359,   370,
     376,   381,   404,   419,   420,   429,   438,  -842,  -842,   990,
    -842,   992,   994,  -842,  1172,  -842,  -842,   996,  -842,  -842,
    -842,  -842,  1173,  1174,  1175,  -842,  -842,  -842,  1176,  3929,
    1178,  -842,  3022,   712,  -842,  -842,  1177,  1226,  1230,  -842,
     998,  -842,  3929,  -842,  -842,  1087,  -842,  -842,   732,  1181,
    1184,   -12,  -842,  1004,  1110,    56,    56,    56,  -842,  -842,
    3929,  -842,  -842,  -842,    25,  -842,   112,  -842,  -842,  -842,
     501,   347,  -842,   101,  3929,  3929,  3929,  3929,  3929,  3929,
    3929,  3929,  -842,    56,  1179,  -842,  -842,    56,  1185,  3929,
    -842,   366,   491,   120,   238,   938,  1187,  1188,   784,  -842,
    -842,  1087,  1087,  1311,  1312,  1313,  -842,  3929,  3553,  1206,
    1207,  -842,   441,   449,   450,   451,   454,   464,  -842,  -842,
    -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842,  1195,  -842,  -842,  -842,  -842,  -842,
    3929,  1009,  -842,  -842,  3929,  3929,  3022,   742,  -842,  -842,
    3929,  -842,  -842,  -842,  -842,    56,    56,  -842,  -842,  -842,
    -842,  -842,  1314,  3929,  1211,  1315,  1317,  3929,  3929,  3929,
    -842,  -842,  -842,   468,   489,   493,   498,   509,   535,   536,
     537,  -842,  -842,  -842,  3929,  1198,  -842,   237,  1256,  -842,
     260,  -842,  1320,  1321,  1208,  3929,  -842,  1324,  1325,  1213,
    3929,  -842,   255,   336,  1214,  1218,  -842,  -842,   511,  -842,
    -842,  -842,  -842,  -842,   538,  1205,  1015,  3929,  3929,  -842,
    -842,  -842,  -842,  -842,  -842,  -842,  1217,  1220,  -842,  -842,
    1021,  -842,   754,  1139,  -842,  -842,  -842,  3929,  -842,  -842,
     541,   547,   550,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  1221,    56,  1219,  1037,  1222,  1759,  1219,  1041,  1223,
    1224,  -842,  1227,  1228,  1234,  -842,  1235,  1341,  1351,  1248,
    3929,  -842,  1365,  1368,  1257,  3929,  -842,  -842,  -842,   265,
    -842,  -842,  -842,  1247,   552,   555,  -842,  -842,  1261,  -842,
      56,  -842,  -842,  -842,  -842,  -842,  3929,  -842,  -842,  1219,
    -842,  3629,  3704,  1984,  -842,   452,  -842,  1219,  -842,  -842,
    -842,  -842,  -842,  -842,  1262,  1263,  -842,  1264,  1266,  1268,
    -842,  1269,  1219,  1047,  -842,  -842,  -842,  -842,  -842,  1258,
    -842,   521,   409,  -842,   137,  3929,  3929,  3929,  3929,  3929,
    3929,  3929,  3929,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  1219,    56,  3929,  3929,  3929,  -842,  -842,  -842,
     556,   569,   571,   573,   582,   585,   598,   599,  -842,  -842,
     600,   604,   608,  -842,  -842,  -842,  -842,  -842,  -842,  -842,
    -842,  -842,  -842,  -842
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
      98,     0,     0,     3,     4,    99,   100,   107,   108,   109,
     110,   101,   102,   117,     0,   118,   119,   120,   106,     0,
     326,   328,   346,   348,   351,   353,   356,   358,   361,   364,
     371,   376,   378,   392,   420,     0,   432,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   429,   434,   435,   449,   448,     0,
       0,   460,   374,     0,   429,   434,    35,     0,   421,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   356,     0,   461,   325,
     324,     0,    48,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,     0,   146,     0,
       0,   111,     0,     0,   112,     0,     0,     0,     3,     0,
     418,   103,   105,   415,   210,     0,     0,     0,   375,     0,
     430,   431,   373,   372,   429,   456,     0,     0,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   319,   320,   321,
     322,   323,   317,     0,   398,     0,     0,   318,     0,     0,
       0,     0,     0,    39,   403,     0,   402,   448,   394,     0,
       0,     0,     0,     0,     0,    29,     0,     0,     0,     0,
       0,     0,     0,   456,     0,     0,    38,   423,     0,     6,
       0,    10,     0,    11,     0,     0,     0,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,     0,     0,     0,     0,    43,     0,     0,   124,     0,
     136,     0,     0,   155,     0,     0,     0,     3,   162,    46,
       0,     0,    47,     0,     0,    51,     0,     0,     0,   158,
       0,     0,     0,     0,     0,   139,     0,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   416,   417,   419,
       0,     0,   213,     0,     0,     0,   248,   457,     0,     0,
       0,     0,   453,   283,   429,   434,   435,   449,     0,     0,
       0,     0,     0,     0,   282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   331,   332,   335,   336,   329,   330,     0,   333,
     334,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     347,   349,   350,   352,   354,   355,   357,   359,   360,   362,
     363,   368,   369,   370,   365,   366,   367,   377,   381,   391,
     385,   386,   387,   388,   389,   390,   383,   384,   382,   414,
       0,   411,    58,     0,    92,    93,    94,    95,    96,    55,
       0,     0,   397,     0,    15,     0,     0,     0,     0,   439,
     444,   445,    60,     0,   406,     0,     0,     0,   395,     0,
     393,   423,     0,     0,     0,   319,   320,   321,   322,   323,
     317,   318,    37,     0,   422,     0,     0,     0,   426,   448,
       0,     0,     0,     0,    26,     0,    27,     0,    28,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,     0,
     127,   457,     0,     0,   354,   355,   260,   263,     0,   276,
     278,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,     0,     0,     0,     0,   457,     0,   457,
       0,     0,     0,     0,   113,   114,   115,   116,   316,   104,
       0,     0,   213,     0,     0,     0,     0,     0,     0,   458,
       0,     0,   380,   454,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,   310,     0,     0,   311,
       0,     0,     0,     0,     0,     0,     0,     0,   284,    62,
      64,    66,    69,    67,    68,    63,    65,    75,    77,    79,
      82,    80,    81,    76,    78,     0,     0,   407,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,   401,     0,     0,   252,     0,     0,
       0,     0,   439,   444,   445,    36,     5,     0,    12,     0,
       0,     0,     0,    19,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    86,    85,    89,
      87,    88,    84,    41,     0,    90,    91,    44,   121,   126,
     125,     0,     0,   135,   152,     0,   154,     0,     0,     0,
       0,   265,     0,     0,   161,     0,     0,     0,     0,     0,
     173,     0,    49,    50,    53,    54,    31,    30,     0,     0,
     156,   446,   447,     0,     0,   137,   140,   141,   144,   145,
       0,     0,   211,   212,     0,     0,     0,     0,   209,   170,
       0,     0,     0,     0,     0,     0,     0,   247,   459,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   300,   307,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   327,   413,     0,
     410,     0,     0,    57,     0,    59,   171,     0,    16,   442,
     441,   440,     0,     0,     0,   404,   399,   400,     0,     0,
       0,   396,     0,     0,   257,   256,     0,     0,     0,    21,
       0,    22,     0,    25,   164,     0,    33,    32,     0,   459,
     459,     0,   128,     0,   129,     0,     0,     0,   261,   272,
       0,   266,   267,   160,   268,   264,     0,   277,   279,   281,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   174,     0,     0,    73,    74,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,   169,
     167,     0,     0,     0,     0,     0,     9,     0,     0,   439,
     445,   304,     0,     0,     0,     0,     0,     0,   312,   313,
     314,   315,   285,   287,   289,   292,   290,   291,   286,   288,
     412,   409,   408,    56,   459,    70,    71,    72,   253,   254,
       0,     0,    13,    20,     0,     0,     0,     0,   163,     7,
       0,    42,    45,   123,   122,   457,     0,   133,   153,   150,
     151,   274,     0,     0,   269,     0,     0,     0,     0,     0,
     177,   178,   175,     0,     0,     0,     0,     0,     0,     0,
       0,   159,   157,   138,     0,     0,   149,     0,     3,   243,
       0,   240,     0,     0,     0,     0,   219,     0,     0,     0,
       0,   214,     0,     0,     0,     0,   215,   216,     0,   166,
     168,   249,   250,   251,     0,   172,     0,     0,     0,   294,
     296,   299,   297,   298,   295,    17,     0,   459,   259,   258,
       0,    23,     0,   130,   131,   262,   273,     0,   270,   271,
       0,     0,     0,   179,   181,   183,   186,   184,   185,   180,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   221,     0,     0,     0,     0,   220,   217,   218,     0,
     246,   303,   302,   171,     0,     0,   255,    14,   459,     8,
       0,   134,   275,   187,   189,   188,     0,   148,   242,     0,
     191,     0,     0,     0,   190,     0,   239,     0,   228,   229,
     223,   226,   227,   222,     0,     0,   232,     0,     0,     0,
     233,     0,     0,     0,   301,   305,   306,    24,   132,     0,
     241,     0,     0,   194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   192,   238,   236,   237,   225,   234,   235,
     224,   245,     0,     0,     0,     0,     0,   195,   196,   193,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   147,
       0,     0,     0,   197,   199,   201,   204,   202,   203,   198,
     200,   205,   207,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -842,  -842,     3,  -842,  -532,  -690,  -842,  -842,  -842,  -842,
    -842,   309,  1366,  -842,  -842,  -842,  -842,  -290,  -841,  -842,
    -842,  -842,   802,  -842,  -842,  -480,   890,  -369,   308,  1136,
      -2,  -224,  -842,   799,  1165,  -842,   672,   -16,  1160,   939,
     940,    -8,  -842,  1270,  -254,  -842,  -842,    51,  -842
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   201,    64,    65,   540,    66,    67,    68,    69,
      70,   965,    71,    72,   562,    73,    74,   338,   969,    75,
      76,   371,   583,    77,   588,   280,   511,   553,   121,   176,
     128,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   197,   459,    92,    93,   129,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,   461,   418,     2,   585,   655,   699,   122,   832,   659,
     146,   661,   546,   547,    24,   147,   548,   520,   709,    26,
     138,   140,   142,   144,   145,   110,   111,   151,   153,   155,
     580,   971,   159,   546,   547,   163,   165,   548,   581,   240,
     241,   334,   335,   180,   188,   604,   183,   942,   707,   708,
     192,   193,   728,   729,   711,     3,   607,   186,   169,   196,
     199,   486,   546,   547,   605,   298,   548,   300,   123,   549,
     550,   551,   546,   547,   187,   608,   548,   170,   374,   299,
     242,   301,   242,   323,   340,    96,    97,   242,   242,   156,
     549,   550,   551,   157,   172,   340,   257,   258,   259,   260,
     261,   262,   324,   341,   267,   269,   274,   271,   272,   275,
     112,   710,   113,   173,   341,   105,   712,   713,   196,   549,
     550,   551,   944,    63,    63,   294,   160,   340,   375,   549,
     550,   551,   660,   552,   972,   312,    63,   322,   158,   327,
     943,   329,   973,   112,   937,   113,   341,  1060,   171,   730,
     681,   582,   276,   323,   584,   606,   265,   161,   791,   174,
     354,   171,   356,   340,   281,   361,   609,   362,   363,   343,
     364,   365,   324,   366,   367,   368,   366,   369,   295,   381,
     470,   382,   341,   680,   945,   946,   325,   342,   344,   326,
     974,    63,  1078,   751,   774,   471,  1086,   174,   342,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   106,   404,
     405,   406,   407,   408,   409,   410,   411,   752,   753,   745,
     746,   175,   817,   747,   434,   435,   107,   112,   331,   113,
     342,   818,   130,   131,   132,   975,   133,   952,  1110,   441,
     442,   443,   444,   445,   446,   447,  1124,   546,   547,   976,
     108,   548,   977,   134,   296,   184,   325,   297,   135,   326,
     978,  1131,   714,   772,   473,   283,   342,   185,   109,  1047,
     546,   547,   345,  1139,   548,   546,   547,  1048,   332,   548,
     492,    63,   494,   284,   495,   496,   497,   498,   499,   500,
     501,  1148,   358,   705,   749,   504,   505,   748,   507,   933,
     512,   754,   755,   756,   549,   550,   551,   190,   979,   359,
      78,   525,   526,   527,   528,   529,   530,   531,   544,   545,
     308,   534,   535,  1071,   309,  1049,   191,   549,   550,   551,
     339,   671,   549,   550,   551,   148,   150,   773,   555,   556,
     554,   557,   558,   297,   559,   560,    63,   166,   168,   564,
    1052,   566,   136,   980,   181,   182,   570,   571,  1053,   610,
     189,   757,   611,   612,   200,   745,   746,   981,  1033,   747,
    1050,   613,   532,   323,   614,   615,   292,   536,   616,   617,
     112,   239,   113,   543,  1051,   350,   599,   600,   601,   603,
     824,  1037,   324,   302,   282,   887,  1102,   765,   546,   547,
     314,   766,   548,   351,   254,   352,  1054,   303,   106,   767,
     888,   787,   768,   769,   167,   336,   770,   513,   854,    63,
     337,   855,   856,   353,   255,   843,   596,   148,   108,   590,
     857,   514,   594,   858,   859,   323,   253,   860,   861,   237,
     238,   482,   574,   870,   515,   517,   597,   256,   891,   898,
     328,  1055,   330,   333,   324,   549,   550,   551,   516,   518,
     879,   880,   219,   245,   246,  1056,   844,   845,   641,   618,
     846,   355,   519,   357,   646,   647,   325,   337,  1115,   326,
     575,  1116,  1117,   951,   657,   576,   263,   899,   577,   504,
    1118,   872,   900,  1119,  1120,   901,   967,  1121,  1122,   627,
     320,   968,   270,   321,   667,   628,   902,   771,   196,    78,
     629,   676,   903,   678,   640,   679,   478,   904,   479,   682,
     683,   219,   643,   847,   848,   849,   645,   947,   862,   648,
     314,   948,   273,   630,   653,   928,   656,   702,   325,   316,
     905,   326,   317,   318,   282,  1138,   949,  1134,   631,   632,
     314,  1135,   721,   377,   378,   906,   907,   313,   633,   316,
     314,   315,   317,   318,   285,   908,  1136,   634,   307,   316,
     686,   741,   317,   318,   909,   685,   319,   999,   688,   689,
     690,   219,   310,   691,   694,  1000,  1001,  1002,  1123,   759,
    1003,   989,   990,   692,   762,   763,   764,   627,  1034,   339,
    1004,  1038,   775,   776,  1023,   777,   778,   346,   779,   780,
     781,   782,   783,   784,   785,   786,   202,   347,   628,   203,
     204,   970,   629,   370,   988,  1024,   968,   630,   205,  1025,
     320,   206,   207,   321,  1026,   208,   209,   950,   631,   211,
     349,  1059,   212,   213,   373,  1027,   968,   802,   803,   804,
     320,   214,   348,   321,   215,   216,   808,  1137,   217,   218,
     320,   243,   244,   321,   632,   633,   634,   795,   814,   815,
     686,  1028,  1029,  1030,  1061,   820,   688,  1073,   579,   692,
     372,   915,   828,  1074,   917,   627,  1075,   789,  1105,   792,
    1103,  1106,  1153,   448,   474,   797,   376,   475,   628,   833,
     629,   449,   630,   450,   451,  1154,   383,  1155,   839,  1156,
     480,   631,   452,   453,   632,   850,   851,   853,  1157,   210,
     813,  1158,   454,   455,   379,   380,   863,   633,   634,   686,
     456,   457,   458,   688,  1159,  1160,  1161,   692,   868,   871,
    1162,   464,   219,   668,  1163,    95,   669,    96,    97,    98,
      99,   100,   101,   102,   873,   462,   463,   874,   875,   876,
     877,   465,   103,   892,   893,   894,   895,   896,   897,   472,
     297,     4,     5,     6,     7,   104,   481,   105,   124,   125,
     116,   117,   277,    13,    14,    15,    16,    17,    18,   673,
     466,   693,   674,   448,   297,   478,   460,   662,   448,   119,
      28,   449,   697,   450,   451,   297,   449,   919,   450,   451,
     467,    35,   452,   453,   304,   305,   306,   452,   453,    43,
      44,    45,   454,   455,    48,   468,   798,   454,   455,   297,
     456,   457,   458,    52,    53,   456,   457,   458,   941,    54,
     821,   922,   469,   822,   297,   698,   700,   476,   477,   703,
     704,   706,   953,   954,   955,   956,   957,   958,   959,   960,
     482,   929,    78,   921,   930,   488,   489,   966,    56,    57,
     297,  1011,   760,   927,   297,   735,   736,   737,   220,   221,
     222,   223,   483,  1069,    58,   994,   297,   485,   224,   225,
     226,    59,   229,   230,   231,   232,   233,   234,   120,   278,
     484,    62,   487,    63,   235,   279,   427,   428,   429,   431,
     432,   433,   227,   228,   493,   297,   236,   801,  1006,   247,
     248,   249,  1008,  1009,   542,    96,    97,   286,   287,   288,
     289,   290,   478,   563,   889,   250,   251,   252,   502,   996,
     291,  1016,   561,   490,   491,  1020,  1021,  1022,   982,   503,
     297,   983,   521,   984,   985,   105,  -379,  -379,   523,   380,
     377,   524,  1031,   522,    96,    97,   286,   287,   288,   289,
     290,  1036,   565,  1042,   377,   533,   568,  1010,  1046,   291,
     573,  1012,    95,   572,    96,    97,    98,    99,   100,   101,
     102,   537,   378,   578,   105,  1064,  1065,   538,   539,   103,
     541,   378,   567,   378,   569,   378,   377,   644,   715,   716,
     834,   574,   595,   586,   105,  1072,   575,  -460,  -460,   412,
     413,   414,   415,   416,   417,   576,   419,   420,   421,   422,
     423,   424,   425,   426,  1085,   750,   716,   864,   758,   297,
     794,   297,   796,   297,   805,   297,   806,   489,  1097,   807,
     297,   811,   489,  1101,   377,   812,  -460,  -460,   577,  -460,
    -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,  -460,   816,   674,   966,  -460,   825,   716,   587,  1111,
    1112,  1114,   591,  -460,  -460,  -460,  -460,  -460,  -460,  -460,
    -460,   829,   297,  -460,  -460,  -460,  -460,  -460,  -460,   592,
    -460,  -460,  -460,   598,  -460,   830,   297,   831,   539,   881,
     716,   882,   716,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1147,   910,   297,   911,   489,   912,   297,   914,   297,   377,
     926,   619,  1150,  1151,  1152,   935,   378,   936,   539,   934,
    1007,   297,   620,   938,   939,   940,  1063,   297,     4,     5,
       6,     7,  1068,   297,   621,   124,   125,   116,   117,   277,
      13,    14,    15,    16,    17,    18,  1070,   539,  1079,   716,
     236,   961,  1087,   716,   622,   963,   119,    28,  1132,   716,
     177,   179,   437,   438,   623,   439,   440,   624,    35,   625,
     626,   627,   628,   629,   630,   631,    43,    44,    45,   632,
     633,    48,   634,   635,   242,   636,   637,   638,   649,   650,
      52,    53,   651,   663,   658,   664,    54,   665,   684,   666,
     670,   675,   337,   742,   743,   686,   761,   687,   688,   689,
     690,   691,   731,   692,   695,   696,   809,   722,   723,   733,
     724,   725,   726,  1013,  1014,    56,    57,   727,   738,   739,
     740,   732,   734,   793,   795,   810,   799,   800,   819,   823,
     826,    58,   827,   835,   836,   837,   838,   840,    59,   841,
     842,   865,   866,   867,   878,   120,   654,   869,    62,   883,
      63,   884,   279,     4,     5,     6,     7,   885,   886,   890,
     124,   125,   116,   117,   277,    13,    14,    15,    16,    17,
      18,   913,   915,   916,   917,   924,   923,   918,   920,   925,
     931,   119,    28,   932,   964,   962,   986,   987,   991,   992,
     993,   997,   998,    35,  1005,  1018,  1017,  1019,  1015,  1032,
    1035,    43,    44,    45,  1039,  1040,    48,  1041,  1043,  1044,
    1077,  1062,  1045,  1057,    78,    52,    53,  1058,  1066,  1067,
    1076,    54,  1088,  1089,   968,  1094,  1090,  1091,  1080,     4,
       5,     6,     7,  1092,  1093,  1095,   124,   125,   116,   117,
     277,    13,    14,    15,    16,    17,    18,  1096,  1108,  1098,
      56,    57,  1099,  1104,   744,  1109,  1100,   119,    28,  1133,
    1107,  1125,  1126,  1127,   149,  1128,    58,  1129,  1130,    35,
     672,   430,   436,    59,     0,     0,   311,    43,    44,    45,
     120,   790,    48,    62,     0,    63,     0,   279,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,   120,     0,     0,    62,
       0,    63,     0,   279,     4,     5,     6,     7,     0,     0,
       0,   384,   385,   386,   387,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   388,   389,    24,     0,     0,
      25,    26,    27,    28,   390,   391,    31,    32,    33,    34,
       0,     0,     0,     0,    35,    36,    37,    38,    39,    40,
     392,   393,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,    50,    51,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
      60,    61,     0,     0,    62,     0,    63,   394,     4,     5,
       6,     7,     0,     0,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   717,
     718,    24,     0,     0,    25,    26,    27,    28,    29,   719,
      31,    32,    33,    34,     0,     0,     0,     0,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     0,     0,     0,    50,    51,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,    60,    61,     0,     0,    62,     0,
      63,   720,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,  1081,  1082,    24,     0,     0,    25,    26,
      27,    28,    29,  1083,    31,    32,    33,    34,     0,     0,
       0,     0,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,     0,     0,
      50,    51,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,    55,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,    60,    61,
      48,     0,    62,     0,    63,  1084,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,   152,   120,    48,     0,    62,     0,    63,
     602,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,   152,   120,
      48,     0,    62,     0,    63,   852,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,   152,   120,     0,     0,    62,     0,    63,
    1113,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,     0,     0,     0,
       0,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,     0,     0,    50,
      51,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,    55,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,    60,    61,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,   126,   127,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,   152,   120,    48,
       0,    62,     0,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   194,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    56,    57,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    35,     0,     0,     0,     0,    59,     0,     0,    43,
      44,    45,   154,   120,    48,     0,    62,     0,    63,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    56,    57,
       0,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,    35,     0,     0,
       0,    59,     0,     0,     0,    43,    44,    45,   120,   195,
      48,    62,     0,    63,     0,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   120,    48,     0,    62,   198,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   124,   125,   116,   117,   118,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,   119,    28,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,    35,     0,
       0,     0,    59,     0,     0,     0,    43,    44,    45,   120,
     264,    48,    62,     0,    63,     0,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     124,   125,   116,   117,   118,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    56,    57,     0,     0,     0,
       0,   119,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    35,     0,     0,     0,     0,    59,   268,
       0,    43,    44,    45,     0,   120,    48,     0,    62,     0,
      63,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   124,   125,   116,   117,
     118,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      56,    57,     0,     0,     0,     0,     0,   119,    28,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,    35,
       0,     0,     0,    59,     0,     0,     0,    43,    44,    45,
     120,   195,    48,    62,     0,    63,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,   360,    56,    57,     0,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,    35,     0,     0,     0,    59,
       0,     0,     0,    43,    44,    45,   120,   293,    48,    62,
       0,    63,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    56,    57,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      35,     0,     0,     0,     0,    59,     0,     0,    43,    44,
      45,     0,   120,    48,     0,    62,     0,    63,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,   200,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   124,   125,   116,   117,   118,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    56,    57,     0,
       0,     0,     0,   119,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    35,     0,     0,     0,     0,
      59,     0,     0,    43,    44,    45,     0,   120,    48,     0,
      62,     0,    63,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   124,   125,
     116,   117,   118,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    56,    57,     0,     0,     0,     0,     0,   119,
      28,     0,     0,     0,     0,     0,     0,     0,    58,     0,
       0,    35,     0,     0,     0,    59,     0,     0,     0,    43,
      44,    45,   120,   589,    48,    62,     0,    63,     0,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    56,    57,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    35,     0,     0,     0,
       0,    59,     0,     0,    43,    44,    45,     0,   120,    48,
       0,    62,   593,    63,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,    35,     0,     0,     0,    59,     0,     0,     0,
      43,    44,    45,   120,   639,    48,    62,     0,    63,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   124,   125,   116,   117,   118,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    56,
      57,     0,     0,     0,     0,     0,   119,    28,     0,     0,
       0,     0,     0,     0,     0,    58,     0,     0,    35,     0,
       0,     0,    59,     0,     0,     0,    43,    44,    45,   120,
     642,    48,    62,     0,    63,     0,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     4,     5,   508,     7,     0,     0,
       0,   124,   125,   116,   117,   509,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    56,    57,     0,     0,     0,
       0,     0,   119,    28,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,    35,     0,     0,     0,    59,     0,
       0,     0,    43,    44,    45,   120,   652,    48,    62,     0,
      63,     0,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   124,   125,   116,
     117,   118,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    56,    57,     0,     0,     0,     0,     0,   119,    28,
       0,     0,     0,     0,     0,     0,     0,    58,     0,     0,
      35,     0,     0,     0,    59,     0,     0,     0,    43,    44,
      45,   120,   195,    48,    62,     0,    63,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   124,   125,   116,   117,   118,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    56,    57,     0,
       0,     0,     0,     0,   119,    28,     0,     0,     0,     0,
       0,     0,     0,    58,     0,     0,    35,     0,     0,     0,
      59,     0,     0,     0,    43,    44,    45,   120,   788,    48,
      62,     0,    63,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    56,    57,     0,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,    35,     0,     0,     0,    59,     0,     0,     0,
      43,    44,    45,   120,   995,    48,    62,     0,    63,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   137,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   139,    48,     0,    62,     0,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   141,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   143,    48,     0,    62,     0,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   120,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   162,    48,     0,    62,     0,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   164,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   120,    48,     0,    62,     0,   178,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   266,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,   508,     7,     0,     0,     0,   124,
     125,   116,   117,   509,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   506,    48,     0,    62,     0,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   124,   125,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   510,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   124,
     125,   116,   117,   118,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
     119,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    35,     0,     0,     0,     0,    59,     0,     0,
      43,    44,    45,     0,   677,    48,     0,    62,     0,    63,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   115,   116,   117,   118,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    56,
      57,     0,     0,     0,     0,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    35,     0,     0,
       0,     0,    59,     0,     0,    43,    44,    45,     0,   701,
      48,     0,    62,     0,    63,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,   120,     0,     0,    62,     0,    63
};

static const yytype_int16 yycheck[] =
{
       2,   255,   226,     0,   373,   485,   538,    15,   698,    14,
      26,   491,    10,    11,    26,    14,    14,   307,    22,    30,
      22,    23,    24,    25,    26,   115,    11,    29,    30,    31,
      14,   872,    34,    10,    11,    37,    38,    14,    22,    80,
      81,    80,    81,    45,    52,    26,    48,    22,    82,    82,
      58,    59,    39,    40,    10,     0,    26,    14,    26,    61,
      62,    79,    10,    11,    45,   115,    14,   115,    14,    67,
      68,    69,    10,    11,    31,    45,    14,    45,    14,   129,
     121,   129,   121,    26,    26,   100,   101,   121,   121,    10,
      67,    68,    69,    14,    26,    26,    98,    99,   100,   101,
     102,   103,    45,    45,   106,   107,    14,   109,   110,    17,
     128,   115,   130,    45,    45,   130,    72,    73,   120,    67,
      68,    69,    10,   135,   135,   127,    12,    26,    64,    67,
      68,    69,   137,   131,    14,   137,   135,   139,    14,   141,
     115,   143,    22,   128,   834,   130,    45,   988,   129,   136,
     519,   135,    60,    26,   131,   136,   105,    12,   638,   129,
     162,   129,   164,    26,   113,   167,   136,   169,   170,    26,
     172,   173,    45,   175,   176,   177,   178,   179,   127,   132,
     115,   134,    45,   131,    72,    73,   129,   129,    45,   132,
      70,   135,  1033,   131,   136,   130,  1037,   129,   129,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   115,   211,
     212,   213,   214,   215,   216,   217,   218,   586,   587,    58,
      59,   135,     5,    62,   240,   241,   133,   128,    82,   130,
     129,    14,    10,    11,    12,   115,    14,   136,  1079,   247,
     248,   249,   250,   251,   252,   253,  1087,    10,    11,   129,
     117,    14,    14,    31,   129,    14,   129,   132,    36,   132,
      22,  1102,   552,   136,   266,   117,   129,    14,   135,    14,
      10,    11,   129,   136,    14,    10,    11,    22,   132,    14,
     282,   135,   284,   135,   286,   287,   288,   289,   290,   291,
     292,  1132,    29,    82,   584,   297,   298,   136,   300,   831,
     302,    76,    77,    78,    67,    68,    69,    10,    70,    46,
       2,   313,   314,   315,   316,   317,   318,   319,   334,   335,
      10,   323,   324,  1013,    14,    70,    10,    67,    68,    69,
     129,    79,    67,    68,    69,    27,    28,   136,   340,   341,
     337,   343,   344,   132,   346,   347,   135,    39,    40,   351,
      14,   353,   130,   115,    46,    47,   358,   359,    22,    26,
      14,   136,    29,    30,    74,    58,    59,   129,   131,    62,
     115,    38,   321,    26,    41,    42,   115,   326,    45,    46,
     128,   107,   130,   332,   129,   117,   388,   389,   390,   391,
     680,   131,    45,   115,   133,   115,   131,    26,    10,    11,
      29,    30,    14,   135,   128,   117,    70,   129,   115,    38,
     130,   635,    41,    42,   130,   130,    45,   115,    26,   135,
     135,    29,    30,   135,   128,   715,   133,   119,   117,   378,
      38,   129,   381,    41,    42,    26,   114,    45,    46,   119,
     120,   129,   129,   136,   115,   115,   135,    14,   136,   136,
     142,   115,   144,   145,    45,    67,    68,    69,   129,   129,
     750,   751,   129,   122,   123,   129,    10,    11,   470,   136,
      14,   163,   130,   165,   476,   477,   129,   135,    26,   132,
     129,    29,    30,   136,   486,   129,    14,   136,   129,   491,
      38,   745,   136,    41,    42,   136,   130,    45,    46,   129,
     129,   135,    14,   132,   506,   129,   136,   136,   510,   201,
     129,   513,   136,   515,   463,   517,   132,   136,   134,   521,
     522,   129,   471,    67,    68,    69,   475,    26,   136,   478,
      29,    30,   129,   129,   483,   825,   485,   539,   129,    38,
     136,   132,    41,    42,   133,   136,    45,    26,   129,   129,
      29,    30,   554,   131,   132,   136,   136,    26,   129,    38,
      29,    30,    41,    42,   129,   136,    45,   129,    14,    38,
     129,   573,    41,    42,   136,   524,    45,   136,   129,   129,
     129,   129,    10,   129,   533,   136,   136,   136,   136,   591,
     136,   881,   882,   129,   596,   597,   598,   129,   967,   129,
     136,   970,   604,   605,   136,   607,   608,   115,   610,   611,
     612,   613,   614,   615,   616,   617,    26,   115,   129,    29,
      30,   130,   129,    55,   878,   136,   135,   129,    38,   136,
     129,    41,    42,   132,   136,    45,    46,   136,   129,    26,
     135,   130,    29,    30,   130,   136,   135,   649,   650,   651,
     129,    38,    71,   132,    41,    42,   658,   136,    45,    46,
     129,   112,   113,   132,   129,   129,   129,   129,   670,   671,
     129,   136,   136,   136,   136,   677,   129,   136,   370,   129,
     135,   129,   684,   136,   129,   129,   136,   636,   136,   638,
    1059,   136,   136,    14,   129,   644,   135,   132,   129,   701,
     129,    22,   129,    24,    25,   136,   136,   136,   710,   136,
     118,   129,    33,    34,   129,   717,   718,   719,   136,   129,
     669,   136,    43,    44,   131,   132,   728,   129,   129,   129,
      51,    52,    53,   129,   136,   136,   136,   129,   740,    60,
     136,   129,   129,   129,   136,    98,   132,   100,   101,   102,
     103,   104,   105,   106,    10,   129,   130,    13,    14,    15,
      16,   129,   115,   765,   766,   767,   768,   769,   770,   131,
     132,     3,     4,     5,     6,   128,   136,   130,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   129,
     129,   129,   132,    14,   132,   132,    17,   134,    14,    31,
      32,    22,   129,    24,    25,   132,    22,   809,    24,    25,
     129,    43,    33,    34,    10,    11,    12,    33,    34,    51,
      52,    53,    43,    44,    56,   129,   129,    43,    44,   132,
      51,    52,    53,    65,    66,    51,    52,    53,   840,    71,
     129,   129,   129,   132,   132,   537,   538,   122,   123,   541,
     542,   543,   854,   855,   856,   857,   858,   859,   860,   861,
     129,   129,   554,   812,   132,   131,   132,   869,   100,   101,
     132,   129,   134,   822,   132,   567,   568,   569,    83,    84,
      85,    86,   130,   129,   116,   887,   132,   130,    93,    94,
      95,   123,    87,    88,    89,    90,    91,    92,   130,   131,
      14,   133,    14,   135,    99,   137,     7,     8,     9,   237,
     238,   239,   117,   118,    14,   132,   111,   134,   920,   108,
     109,   110,   924,   925,    10,   100,   101,   102,   103,   104,
     105,   106,   132,    14,   134,   124,   125,   126,   129,   888,
     115,   943,   129,   131,   132,   947,   948,   949,    10,   131,
     132,    13,   115,    15,    16,   130,   131,   132,   131,   132,
     131,   132,   964,   115,   100,   101,   102,   103,   104,   105,
     106,   968,    14,   975,   131,   132,    27,   926,   980,   115,
     129,   930,    98,    10,   100,   101,   102,   103,   104,   105,
     106,   131,   132,   136,   130,   997,   998,    27,    28,   115,
     131,   132,   131,   132,   131,   132,   131,   132,   131,   132,
     702,   129,   128,   130,   130,  1017,   129,    41,    42,   220,
     221,   222,   223,   224,   225,   129,   227,   228,   229,   230,
     231,   232,   233,   234,  1036,   131,   132,   729,   131,   132,
     131,   132,   131,   132,   131,   132,   131,   132,  1050,   131,
     132,   131,   132,  1055,   131,   132,    80,    81,   129,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,   131,   132,  1076,    99,   131,   132,   130,  1081,
    1082,  1083,   115,   107,   108,   109,   110,   111,   112,   113,
     114,   131,   132,   117,   118,   119,   120,   121,   122,    10,
     124,   125,   126,   115,   128,   131,   132,    27,    28,   131,
     132,   131,   132,  1115,  1116,  1117,  1118,  1119,  1120,  1121,
    1122,   131,   132,   131,   132,   131,   132,   131,   132,   131,
     132,   129,  1134,  1135,  1136,   131,   132,    27,    28,   831,
     131,   132,   129,   835,   836,   837,   131,   132,     3,     4,
       5,     6,   131,   132,   129,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    27,    28,   131,   132,
     111,   863,   131,   132,   129,   867,    31,    32,   131,   132,
      44,    45,   243,   244,   129,   245,   246,   129,    43,   129,
     129,   129,   129,   129,   129,   129,    51,    52,    53,   129,
     129,    56,   129,    96,   121,   130,    14,   130,   115,   115,
      65,    66,   115,   118,   130,   136,    71,   129,   115,   129,
      79,   129,   135,    14,    14,   129,    14,   129,   129,   129,
     129,   129,   118,   129,   129,   129,    79,   129,   129,   118,
     129,   129,   129,   935,   936,   100,   101,   129,   129,   129,
     115,   136,   136,   129,   129,    14,   134,   134,   129,   129,
     129,   116,   129,    10,    10,    10,    14,   115,   123,    10,
      10,   129,   129,    27,    58,   130,   131,   129,   133,    14,
     135,    14,   137,     3,     4,     5,     6,    14,   129,   136,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   129,   129,   129,   129,    79,   129,   131,   130,    79,
     129,    31,    32,   129,   129,   136,   129,   129,     7,     7,
       7,   115,   115,    43,   129,    10,   115,    10,    14,   131,
      74,    51,    52,    53,    14,    14,    56,   129,    14,    14,
    1032,   136,   129,   129,  1036,    65,    66,   129,   131,   129,
     129,    71,   129,   129,   135,    14,   129,   129,   136,     3,
       4,     5,     6,   129,   129,    14,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,   129,  1070,    14,
     100,   101,    14,   136,   582,  1076,   129,    31,    32,   131,
     129,   129,   129,   129,    28,   129,   116,   129,   129,    43,
     510,   236,   242,   123,    -1,    -1,   136,    51,    52,    53,
     130,   131,    56,   133,    -1,   135,    -1,   137,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,   133,
      -1,   135,    -1,   137,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    61,    62,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,    -1,   133,    -1,   135,   136,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    -1,    -1,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    61,    62,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      71,    -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,   129,   130,
      56,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,   129,   130,    56,    -1,   133,    -1,   135,
     136,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,   129,   130,
      56,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,   129,   130,    -1,    -1,   133,    -1,   135,
     136,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    61,
      62,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,   129,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,   129,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    51,    52,    53,   129,   130,    56,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,    51,
      52,    53,   129,   130,    56,    -1,   133,    -1,   135,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    43,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    51,    52,    53,   130,   131,
      56,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,   134,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    43,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    51,    52,    53,   130,
     131,    56,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,    43,    -1,    -1,    -1,    -1,   123,   124,
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
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,   100,   101,    -1,    -1,
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
      53,    -1,   130,    56,    -1,   133,    -1,   135,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    74,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    43,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    51,    52,    53,    -1,   130,    56,    -1,
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
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    43,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      51,    52,    53,    -1,   130,    56,    -1,   133,    -1,   135,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    43,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    51,    52,    53,    -1,   130,
      56,    -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   133,    -1,   135
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
     168,   130,   168,   130,   168,   168,   175,    14,   166,   150,
     166,   168,   129,   168,   129,   168,    10,    14,    14,   168,
      12,    12,   130,   168,   130,   168,   166,   130,   166,    26,
      45,   129,    26,    45,   129,   135,   167,   167,   135,   167,
     168,   166,   166,   168,    14,    14,    14,    31,   179,    14,
      10,    10,   179,   179,    10,   131,   168,   181,   134,   168,
      74,   140,    26,    29,    30,    38,    41,    42,    45,    46,
     129,    26,    29,    30,    38,    41,    42,    45,    46,   129,
      83,    84,    85,    86,    93,    94,    95,   117,   118,    87,
      88,    89,    90,    91,    92,    99,   111,   119,   120,   107,
      80,    81,   121,   112,   113,   122,   123,   108,   109,   110,
     124,   125,   126,   114,   128,   128,    14,   168,   168,   168,
     168,   168,   168,    14,   131,   185,   130,   168,   124,   168,
      14,   168,   168,   129,    14,    17,    60,    14,   131,   137,
     163,   185,   133,   117,   135,   129,   102,   103,   104,   105,
     106,   115,   115,   131,   168,   185,   129,   132,   115,   129,
     115,   129,   115,   129,    10,    11,    12,    14,    10,    14,
      10,   181,   168,    26,    29,    30,    38,    41,    42,    45,
     129,   132,   168,    26,    45,   129,   132,   168,   166,   168,
     166,    82,   132,   166,    80,    81,   130,   135,   155,   129,
      26,    45,   129,    26,    45,   129,   115,   115,    71,   135,
     117,   135,   117,   135,   168,   166,   168,   166,    29,    46,
      23,   168,   168,   168,   168,   168,   168,   168,   168,   168,
      55,   159,   135,   130,    14,    64,   135,   131,   132,   131,
     132,   132,   134,   136,    10,    11,    12,    13,    24,    25,
      33,    34,    49,    50,   136,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   171,   171,   171,   171,   171,   171,   169,   171,
     171,   171,   171,   171,   171,   171,   171,     7,     8,     9,
     172,   174,   174,   174,   175,   175,   176,   177,   177,   178,
     178,   179,   179,   179,   179,   179,   179,   179,    14,    22,
      24,    25,    33,    34,    43,    44,    51,    52,    53,   182,
      17,   182,   129,   130,   129,   129,   129,   129,   129,   129,
     115,   130,   131,   168,   129,   132,   122,   123,   132,   134,
     118,   136,   129,   130,    14,   130,    79,    14,   131,   132,
     131,   132,   168,    14,   168,   168,   168,   168,   168,   168,
     168,   168,   129,   131,   168,   168,   130,   168,     5,    14,
     130,   164,   168,   115,   129,   115,   129,   115,   129,   130,
     155,   115,   115,   131,   132,   168,   168,   168,   168,   168,
     168,   168,   185,   132,   168,   168,   185,   131,    27,    28,
     143,   131,    10,   185,   175,   175,    10,    11,    14,    67,
      68,    69,   131,   165,   140,   168,   168,   168,   168,   168,
     168,   129,   152,    14,   168,    14,   168,   131,    27,   131,
     168,   168,    10,   129,   129,   129,   129,   129,   136,   166,
      14,    22,   135,   160,   131,   165,   130,   130,   162,   131,
     185,   115,    10,   134,   185,   128,   133,   135,   115,   168,
     168,   168,   136,   168,    26,    45,   136,    26,    45,   136,
      26,    29,    30,    38,    41,    42,    45,    46,   136,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,    96,   130,    14,   130,   131,
     185,   168,   131,   185,   132,   185,   168,   168,   185,   115,
     115,   115,   131,   185,   131,   163,   185,   168,   130,    14,
     137,   163,   134,   118,   136,   129,   129,   168,   129,   132,
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
     131,   163,   185,   129,   131,   129,   131,   185,   129,   134,
     134,   134,   168,   168,   168,   131,   131,   131,   168,    79,
      14,   131,   132,   185,   168,   168,   131,     5,    14,   129,
     168,   129,   132,   129,   155,   131,   129,   129,   168,   131,
     131,    27,   143,   168,   166,    10,    10,    10,    14,   168,
     115,    10,    10,   155,    10,    11,    14,    67,    68,    69,
     168,   168,   136,   168,    26,    29,    30,    38,    41,    42,
      45,    46,   136,   168,   166,   129,   129,    27,   168,   129,
     136,    60,   182,    10,    13,    14,    15,    16,    58,   155,
     155,   131,   131,    14,    14,    14,   129,   115,   130,   134,
     136,   136,   168,   168,   168,   168,   168,   168,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     131,   131,   131,   129,   131,   129,   129,   129,   131,   168,
     130,   185,   129,   129,    79,    79,   132,   185,   155,   129,
     132,   129,   129,   142,   166,   131,    27,   143,   166,   166,
     166,   168,    22,   115,    10,    72,    73,    26,    30,    45,
     136,   136,   136,   168,   168,   168,   168,   168,   168,   168,
     168,   166,   136,   166,   129,   149,   168,   130,   135,   156,
     130,   156,    14,    22,    70,   115,   129,    14,    22,    70,
     115,   129,    10,    13,    15,    16,   129,   129,   182,   155,
     155,     7,     7,     7,   168,   131,   185,   115,   115,   136,
     136,   136,   136,   136,   136,   129,   168,   131,   168,   168,
     185,   129,   185,   166,   166,    14,   168,   115,    10,    10,
     168,   168,   168,   136,   136,   136,   136,   136,   136,   136,
     136,   168,   131,   131,   165,    74,   140,   131,   165,    14,
      14,   129,   168,    14,    14,   129,   168,    14,    22,    70,
     115,   129,    14,    22,    70,   115,   129,   129,   129,   130,
     156,   136,   136,   131,   168,   168,   131,   129,   131,   129,
      27,   143,   168,   136,   136,   136,   129,   166,   156,   131,
     136,    24,    25,    34,   136,   168,   156,   131,   129,   129,
     129,   129,   129,   129,    14,    14,   129,   168,    14,    14,
     129,   168,   131,   165,   136,   136,   136,   129,   166,   149,
     156,   168,   168,   136,   168,    26,    29,    30,    38,    41,
      42,    45,    46,   136,   156,   129,   129,   129,   129,   129,
     129,   156,   131,   131,    26,    30,    45,   136,   136,   136,
     168,   168,   168,   168,   168,   168,   168,   168,   156,   166,
     168,   168,   168,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136
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
     168,   168,   168,   168,   168,   168,   168,   169,   169,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   171,   171,   172,
     172,   172,   173,   173,   174,   174,   174,   175,   175,   176,
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
       3,     3,     3,     3,     2,     2,     1,     5,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
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
#line 3088 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 466 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3094 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 467 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3100 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 471 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3106 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 473 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3112 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 475 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3118 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 477 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3126 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3132 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3138 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3144 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 487 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3150 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 489 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3158 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 493 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3166 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 497 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3172 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 499 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3180 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 503 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3188 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 507 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3194 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 509 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3200 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 511 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3206 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 513 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3212 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 515 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3218 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 517 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3226 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 521 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3234 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 525 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3240 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 527 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3246 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 529 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3252 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 531 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3258 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 533 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3264 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 535 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3270 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 537 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3276 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 539 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3282 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 541 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3288 "raku.tab.c"
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
#line 3300 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 551 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3306 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 553 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3312 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 555 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3318 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 557 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3324 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 559 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3330 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 561 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3336 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 563 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3343 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 566 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3350 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 569 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3356 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 571 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3363 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 574 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3370 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 577 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3376 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN expr ';'  */
#line 579 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3382 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN ';'  */
#line 581 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3388 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 583 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3395 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 586 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3402 "raku.tab.c"
    break;

  case 51: /* stmt: KW_EXIT expr ';'  */
#line 589 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3408 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT ';'  */
#line 591 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3415 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 594 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3422 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 597 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3429 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 600 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3435 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 602 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3444 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 607 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3452 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 611 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3460 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 615 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3469 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 620 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3477 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall ';'  */
#line 623 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3483 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3489 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3495 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 629 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3501 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3507 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 633 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3514 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 636 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3520 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 638 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3526 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 640 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3532 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 642 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3539 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 645 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3546 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 648 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3553 "raku.tab.c"
    break;

  case 73: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 651 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3560 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 654 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3567 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_IF expr ';'  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3573 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_UNLESS expr ';'  */
#line 659 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3579 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WHILE expr ';'  */
#line 661 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3585 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_UNTIL expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3591 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_FOR expr ';'  */
#line 665 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3598 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WITH expr ';'  */
#line 668 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3604 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITHOUT expr ';'  */
#line 670 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3610 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_GIVEN expr ';'  */
#line 672 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3616 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 674 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3623 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 677 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3630 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 680 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3638 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 684 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3645 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 687 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3651 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 689 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3657 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 691 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3663 "raku.tab.c"
    break;

  case 90: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 693 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 696 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3677 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 699 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3684 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 702 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3691 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 705 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3698 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 708 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3705 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 711 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3712 "raku.tab.c"
    break;

  case 97: /* stmt: expr ';'  */
#line 713 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3718 "raku.tab.c"
    break;

  case 98: /* stmt: ';'  */
#line 714 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3724 "raku.tab.c"
    break;

  case 99: /* stmt: if_stmt  */
#line 715 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3730 "raku.tab.c"
    break;

  case 100: /* stmt: while_stmt  */
#line 716 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3736 "raku.tab.c"
    break;

  case 101: /* stmt: for_stmt  */
#line 717 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3742 "raku.tab.c"
    break;

  case 102: /* stmt: given_stmt  */
#line 718 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3748 "raku.tab.c"
    break;

  case 103: /* stmt: KW_TRY block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3754 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block KW_CATCH block  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3760 "raku.tab.c"
    break;

  case 105: /* stmt: KW_CATCH block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3766 "raku.tab.c"
    break;

  case 106: /* stmt: block  */
#line 726 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3772 "raku.tab.c"
    break;

  case 107: /* stmt: unless_stmt  */
#line 727 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3778 "raku.tab.c"
    break;

  case 108: /* stmt: until_stmt  */
#line 728 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3784 "raku.tab.c"
    break;

  case 109: /* stmt: repeat_stmt  */
#line 729 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3790 "raku.tab.c"
    break;

  case 110: /* stmt: loop_stmt  */
#line 730 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3796 "raku.tab.c"
    break;

  case 111: /* stmt: KW_LAST ';'  */
#line 731 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3802 "raku.tab.c"
    break;

  case 112: /* stmt: KW_NEXT ';'  */
#line 732 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3808 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_IF expr ';'  */
#line 734 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 736 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3820 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 738 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3826 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 740 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3832 "raku.tab.c"
    break;

  case 117: /* stmt: sub_decl  */
#line 741 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3838 "raku.tab.c"
    break;

  case 118: /* stmt: class_decl  */
#line 742 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3844 "raku.tab.c"
    break;

  case 119: /* stmt: role_decl  */
#line 743 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3850 "raku.tab.c"
    break;

  case 120: /* stmt: grammar_decl  */
#line 744 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3856 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3862 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 750 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3868 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 752 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3874 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block  */
#line 754 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3886 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3892 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block elsif_tail  */
#line 760 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 762 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF expr block  */
#line 766 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3910 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3916 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 770 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3922 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 772 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3928 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3934 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 776 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3940 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 780 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3946 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE expr block  */
#line 782 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3952 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 786 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3958 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 788 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3964 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block  */
#line 790 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3970 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 792 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3976 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 796 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3982 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL expr block  */
#line 798 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3988 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block  */
#line 802 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3994 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 804 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4000 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 806 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4006 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP block  */
#line 810 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4012 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 812 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4018 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 814 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4024 "raku.tab.c"
    break;

  case 149: /* loop_incr: expr  */
#line 817 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4030 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 821 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4040 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 827 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4050 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 833 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4059 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 838 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4069 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 844 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4078 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr block  */
#line 849 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4085 "raku.tab.c"
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
#line 4097 "raku.tab.c"
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
#line 4110 "raku.tab.c"
    break;

  case 158: /* when_list: %empty  */
#line 872 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4116 "raku.tab.c"
    break;

  case 159: /* when_list: when_list KW_WHEN expr block  */
#line 874 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4124 "raku.tab.c"
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
#line 4136 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 888 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4146 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT sub_body  */
#line 894 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4156 "raku.tab.c"
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
#line 4168 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 908 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4178 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 914 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4188 "raku.tab.c"
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
#line 4201 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 929 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4212 "raku.tab.c"
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
#line 4225 "raku.tab.c"
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
#line 4238 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 954 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4249 "raku.tab.c"
    break;

  case 171: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 963 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4260 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 970 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4269 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list '}'  */
#line 976 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4275 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr '}'  */
#line 978 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4282 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 981 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4289 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 984 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4295 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 986 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4301 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 988 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4307 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 990 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4313 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 992 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4319 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 994 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4325 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 996 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4331 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 998 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4338 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1001 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4344 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1003 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4350 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1005 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4356 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1007 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4363 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1010 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4370 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1013 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4378 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list '}'  */
#line 1018 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4384 "raku.tab.c"
    break;

  case 191: /* method_body: '{' YADA '}'  */
#line 1019 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4390 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr '}'  */
#line 1021 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4397 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1024 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4404 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1027 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4410 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1029 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4416 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1031 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4422 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1033 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4428 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1035 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4434 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1037 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4440 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1039 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4446 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1041 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4453 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1044 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4459 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1046 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4465 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1048 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4471 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1050 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4478 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1053 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4485 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1056 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4493 "raku.tab.c"
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
#line 4511 "raku.tab.c"
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
#line 4528 "raku.tab.c"
    break;

  case 210: /* is_clauses: %empty  */
#line 1092 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4534 "raku.tab.c"
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
#line 4550 "raku.tab.c"
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
#line 4566 "raku.tab.c"
    break;

  case 213: /* class_body_list: %empty  */
#line 1119 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4572 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1121 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4579 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1124 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4586 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1127 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4593 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1130 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4600 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1133 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4607 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1136 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4614 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1139 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4621 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1142 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4628 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1145 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4635 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1148 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4642 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1151 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4649 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1154 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4656 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1157 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4667 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1164 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4678 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1171 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4689 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1178 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4700 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1185 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4708 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1189 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4716 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1193 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4724 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1197 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4732 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1201 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4743 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1208 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4754 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1215 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4765 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1222 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4776 "raku.tab.c"
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
#line 4790 "raku.tab.c"
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
#line 4802 "raku.tab.c"
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
#line 4814 "raku.tab.c"
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
#line 4827 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1264 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4838 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1271 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4849 "raku.tab.c"
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
#line 4863 "raku.tab.c"
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
#line 4875 "raku.tab.c"
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
#line 4887 "raku.tab.c"
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
#line 4904 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: %empty  */
#line 1320 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4910 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1322 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4919 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1327 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4928 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1332 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4937 "raku.tab.c"
    break;

  case 252: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1339 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4945 "raku.tab.c"
    break;

  case 253: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1343 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4953 "raku.tab.c"
    break;

  case 254: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1347 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4961 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1351 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4969 "raku.tab.c"
    break;

  case 256: /* pair_list: IDENT OP_FATARROW expr  */
#line 1357 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4975 "raku.tab.c"
    break;

  case 257: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1359 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4981 "raku.tab.c"
    break;

  case 258: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1361 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4987 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1363 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4993 "raku.tab.c"
    break;

  case 260: /* param_list: VAR_SCALAR  */
#line 1366 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4999 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1367 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5005 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1368 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5011 "raku.tab.c"
    break;

  case 263: /* param_list: VAR_ARRAY  */
#line 1369 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5017 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' VAR_ARRAY  */
#line 1370 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5023 "raku.tab.c"
    break;

  case 265: /* param_list: IDENT VAR_SCALAR  */
#line 1371 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5029 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1372 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5035 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1373 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5041 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR  */
#line 1374 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5047 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1375 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5053 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1376 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5059 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1377 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5065 "raku.tab.c"
    break;

  case 272: /* param_list: VAR_SCALAR '=' expr  */
#line 1378 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5071 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1379 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5077 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1380 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5083 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1381 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5089 "raku.tab.c"
    break;

  case 276: /* param_list: SLURPY_POS  */
#line 1382 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5095 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' SLURPY_POS  */
#line 1383 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5101 "raku.tab.c"
    break;

  case 278: /* param_list: SLURPY_LOL  */
#line 1384 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5107 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' SLURPY_LOL  */
#line 1385 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5113 "raku.tab.c"
    break;

  case 280: /* param_list: SLURPY_NAMED  */
#line 1386 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5119 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1387 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5125 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list '}'  */
#line 1390 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5131 "raku.tab.c"
    break;

  case 283: /* block: '{' YADA '}'  */
#line 1391 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5137 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr '}'  */
#line 1393 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5143 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1395 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5149 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1397 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5155 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1399 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5161 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1401 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5167 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1403 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5174 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1406 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5180 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1408 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5186 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1410 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5192 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1412 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5198 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1414 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5205 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1417 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5212 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1420 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5220 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1424 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5227 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1427 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5234 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1430 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5241 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1433 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5247 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1435 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5255 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1439 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5262 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1442 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5269 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1445 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5276 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1448 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5283 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1451 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5290 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1454 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5296 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1456 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5302 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1458 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5308 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_LAST '}'  */
#line 1460 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5314 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1462 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5320 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1464 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5326 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1466 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5332 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1468 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5338 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1470 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5344 "raku.tab.c"
    break;

  case 316: /* closure: '{' expr '}'  */
#line 1473 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5350 "raku.tab.c"
    break;

  case 317: /* expr: VAR_SCALAR '=' expr  */
#line 1476 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5356 "raku.tab.c"
    break;

  case 318: /* expr: VAR_ARRAY '=' expr  */
#line 1477 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5362 "raku.tab.c"
    break;

  case 319: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1478 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5368 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1479 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5374 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1480 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5380 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1481 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5386 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1482 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5392 "raku.tab.c"
    break;

  case 324: /* expr: KW_GATHER block  */
#line 1483 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5402 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER for_stmt  */
#line 1488 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5412 "raku.tab.c"
    break;

  case 326: /* expr: tern_expr  */
#line 1493 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5418 "raku.tab.c"
    break;

  case 327: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1497 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5424 "raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr  */
#line 1498 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5430 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1501 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5436 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1502 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5442 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1503 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5448 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1504 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5454 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1505 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5460 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1506 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5466 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1507 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5472 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1508 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5478 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1509 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5484 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1510 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5490 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1511 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5496 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1512 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5502 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1513 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5508 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1514 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5514 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1516 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5524 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1522 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5534 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1528 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5544 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr  */
#line 1533 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5550 "raku.tab.c"
    break;

  case 347: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1536 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5556 "raku.tab.c"
    break;

  case 348: /* divis_expr: jct_expr  */
#line 1537 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5562 "raku.tab.c"
    break;

  case 349: /* jct_expr: jct_expr '|' range_expr  */
#line 1540 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5568 "raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '&' range_expr  */
#line 1541 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5574 "raku.tab.c"
    break;

  case 351: /* jct_expr: dor_expr  */
#line 1542 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5580 "raku.tab.c"
    break;

  case 352: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1546 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5586 "raku.tab.c"
    break;

  case 353: /* dor_expr: range_expr  */
#line 1547 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5592 "raku.tab.c"
    break;

  case 354: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1550 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5598 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1551 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5604 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr  */
#line 1552 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5610 "raku.tab.c"
    break;

  case 357: /* add_expr: add_expr '~' repl_expr  */
#line 1555 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5616 "raku.tab.c"
    break;

  case 358: /* add_expr: repl_expr  */
#line 1556 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5622 "raku.tab.c"
    break;

  case 359: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1559 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5628 "raku.tab.c"
    break;

  case 360: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1560 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5634 "raku.tab.c"
    break;

  case 361: /* repl_expr: addsub_expr  */
#line 1561 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5640 "raku.tab.c"
    break;

  case 362: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1564 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5646 "raku.tab.c"
    break;

  case 363: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1565 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5652 "raku.tab.c"
    break;

  case 364: /* addsub_expr: mul_expr  */
#line 1566 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5658 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr '*' unary_expr  */
#line 1569 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5664 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '/' unary_expr  */
#line 1570 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5670 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '%' unary_expr  */
#line 1571 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5676 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1572 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5682 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1574 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5688 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1576 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5694 "raku.tab.c"
    break;

  case 371: /* mul_expr: unary_expr  */
#line 1577 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5700 "raku.tab.c"
    break;

  case 372: /* unary_expr: '-' unary_expr  */
#line 1580 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5706 "raku.tab.c"
    break;

  case 373: /* unary_expr: '!' unary_expr  */
#line 1581 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5712 "raku.tab.c"
    break;

  case 374: /* unary_expr: CARET unary_expr  */
#line 1582 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5718 "raku.tab.c"
    break;

  case 375: /* unary_expr: OP_REDUCE unary_expr  */
#line 1584 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5727 "raku.tab.c"
    break;

  case 376: /* unary_expr: pow_expr  */
#line 1588 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5733 "raku.tab.c"
    break;

  case 377: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1591 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5739 "raku.tab.c"
    break;

  case 378: /* pow_expr: postfix_expr  */
#line 1592 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5745 "raku.tab.c"
    break;

  case 379: /* scalar_list: VAR_SCALAR  */
#line 1595 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5751 "raku.tab.c"
    break;

  case 380: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1596 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5757 "raku.tab.c"
    break;

  case 381: /* meth_name: IDENT  */
#line 1599 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5763 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_SORT  */
#line 1600 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5769 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_MAP  */
#line 1601 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5775 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_GREP  */
#line 1602 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5781 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_SAY  */
#line 1603 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5787 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_PRINT  */
#line 1604 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5793 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_TAKE  */
#line 1605 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5799 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_RETURN  */
#line 1606 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5805 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_EXISTS  */
#line 1607 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5811 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_DELETE  */
#line 1608 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5817 "raku.tab.c"
    break;

  case 391: /* meth_name: TESTOP  */
#line 1609 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5823 "raku.tab.c"
    break;

  case 392: /* postfix_expr: call_expr  */
#line 1611 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5829 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '(' arg_list ')'  */
#line 1614 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5838 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '(' ')'  */
#line 1618 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5844 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1620 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5850 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1622 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5856 "raku.tab.c"
    break;

  case 397: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1624 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5865 "raku.tab.c"
    break;

  case 398: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1629 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5871 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5881 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1637 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5891 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1643 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5899 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' KW_NEW  */
#line 1647 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5907 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' IDENT  */
#line 1651 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5916 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1656 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5927 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1663 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5936 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' CARET IDENT  */
#line 1668 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5946 "raku.tab.c"
    break;

  case 407: /* call_expr: atom '.' CARET IDENT  */
#line 1674 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5956 "raku.tab.c"
    break;

  case 408: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1680 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5967 "raku.tab.c"
    break;

  case 409: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1687 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5978 "raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1694 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5987 "raku.tab.c"
    break;

  case 411: /* call_expr: atom '.' meth_name  */
#line 1699 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5996 "raku.tab.c"
    break;

  case 412: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1704 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6007 "raku.tab.c"
    break;

  case 413: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1711 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6016 "raku.tab.c"
    break;

  case 414: /* call_expr: call_expr '.' meth_name  */
#line 1716 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6025 "raku.tab.c"
    break;

  case 415: /* call_expr: KW_DIE expr  */
#line 1721 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6031 "raku.tab.c"
    break;

  case 416: /* call_expr: KW_MAP closure expr  */
#line 1723 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6037 "raku.tab.c"
    break;

  case 417: /* call_expr: KW_GREP closure expr  */
#line 1725 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6043 "raku.tab.c"
    break;

  case 418: /* call_expr: KW_SORT expr  */
#line 1727 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6049 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_SORT closure expr  */
#line 1729 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6055 "raku.tab.c"
    break;

  case 420: /* call_expr: atom  */
#line 1730 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6061 "raku.tab.c"
    break;

  case 421: /* arg_list: expr  */
#line 1733 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6067 "raku.tab.c"
    break;

  case 422: /* arg_list: arg_list ',' expr  */
#line 1734 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6073 "raku.tab.c"
    break;

  case 423: /* arg_list: arg_list ','  */
#line 1735 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6079 "raku.tab.c"
    break;

  case 424: /* atom: LIT_INT  */
#line 1738 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6085 "raku.tab.c"
    break;

  case 425: /* atom: LIT_FLOAT  */
#line 1739 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6091 "raku.tab.c"
    break;

  case 426: /* atom: LIT_STR  */
#line 1740 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6097 "raku.tab.c"
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
#line 6110 "raku.tab.c"
    break;

  case 428: /* atom: LIT_INTERP_STR  */
#line 1750 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6116 "raku.tab.c"
    break;

  case 429: /* atom: VAR_SCALAR  */
#line 1751 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6122 "raku.tab.c"
    break;

  case 430: /* atom: OP_INC VAR_SCALAR  */
#line 1752 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6128 "raku.tab.c"
    break;

  case 431: /* atom: OP_DEC VAR_SCALAR  */
#line 1753 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6134 "raku.tab.c"
    break;

  case 432: /* atom: VAR_SCALAR OP_INC  */
#line 1754 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6140 "raku.tab.c"
    break;

  case 433: /* atom: VAR_SCALAR OP_DEC  */
#line 1755 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6146 "raku.tab.c"
    break;

  case 434: /* atom: VAR_ARRAY  */
#line 1756 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6152 "raku.tab.c"
    break;

  case 435: /* atom: VAR_HASH  */
#line 1757 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6158 "raku.tab.c"
    break;

  case 436: /* atom: VAR_CAPTURE  */
#line 1759 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6166 "raku.tab.c"
    break;

  case 437: /* atom: VAR_FH  */
#line 1763 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6174 "raku.tab.c"
    break;

  case 438: /* atom: VAR_NAMED_CAPTURE  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6181 "raku.tab.c"
    break;

  case 439: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1770 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6187 "raku.tab.c"
    break;

  case 440: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1772 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6193 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1774 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6199 "raku.tab.c"
    break;

  case 442: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1776 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6205 "raku.tab.c"
    break;

  case 443: /* atom: ARR_ALL_SLICE  */
#line 1778 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6211 "raku.tab.c"
    break;

  case 444: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1780 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6217 "raku.tab.c"
    break;

  case 445: /* atom: VAR_HASH '{' expr '}'  */
#line 1782 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6223 "raku.tab.c"
    break;

  case 446: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1784 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6229 "raku.tab.c"
    break;

  case 447: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1786 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6235 "raku.tab.c"
    break;

  case 448: /* atom: IDENT  */
#line 1787 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6241 "raku.tab.c"
    break;

  case 449: /* atom: VAR_TWIGIL  */
#line 1789 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6249 "raku.tab.c"
    break;

  case 450: /* atom: VAR_ARRAY_TWIGIL  */
#line 1793 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6257 "raku.tab.c"
    break;

  case 451: /* atom: VAR_HASH_TWIGIL  */
#line 1797 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6265 "raku.tab.c"
    break;

  case 452: /* atom: '[' ']'  */
#line 1800 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6271 "raku.tab.c"
    break;

  case 453: /* atom: '[' expr ']'  */
#line 1802 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6277 "raku.tab.c"
    break;

  case 454: /* atom: '[' expr ',' ']'  */
#line 1804 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6283 "raku.tab.c"
    break;

  case 455: /* atom: '[' expr ',' arg_list ']'  */
#line 1806 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6290 "raku.tab.c"
    break;

  case 456: /* atom: '(' ')'  */
#line 1808 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6296 "raku.tab.c"
    break;

  case 457: /* atom: '(' expr ')'  */
#line 1809 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6302 "raku.tab.c"
    break;

  case 458: /* atom: '(' expr ',' ')'  */
#line 1811 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6308 "raku.tab.c"
    break;

  case 459: /* atom: '(' expr ',' arg_list ')'  */
#line 1813 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6315 "raku.tab.c"
    break;

  case 460: /* atom: block  */
#line 1815 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6321 "raku.tab.c"
    break;

  case 461: /* atom: KW_SUB block  */
#line 1816 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6327 "raku.tab.c"
    break;


#line 6331 "raku.tab.c"

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
