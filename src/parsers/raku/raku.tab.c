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
#define YYLAST   4690

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  463
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1169

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
       0,   456,   456,   467,   468,   471,   473,   475,   477,   481,
     483,   485,   487,   489,   493,   497,   499,   503,   507,   509,
     511,   513,   515,   517,   521,   525,   527,   529,   531,   533,
     535,   537,   539,   541,   543,   551,   553,   555,   557,   559,
     561,   563,   566,   569,   571,   574,   577,   579,   581,   583,
     586,   589,   591,   594,   597,   600,   602,   607,   611,   615,
     620,   624,   625,   627,   629,   631,   633,   636,   638,   640,
     642,   645,   648,   651,   654,   657,   659,   661,   663,   665,
     668,   670,   672,   674,   677,   680,   684,   687,   689,   691,
     693,   696,   699,   702,   705,   708,   711,   714,   715,   716,
     717,   718,   719,   720,   722,   724,   726,   728,   729,   730,
     731,   732,   733,   734,   736,   738,   740,   742,   743,   744,
     745,   748,   750,   752,   754,   756,   758,   760,   762,   766,
     768,   770,   772,   774,   776,   780,   782,   786,   788,   790,
     792,   796,   798,   802,   804,   806,   810,   812,   814,   818,
     821,   827,   833,   838,   844,   849,   854,   862,   873,   874,
     880,   888,   894,   900,   908,   914,   920,   929,   936,   945,
     954,   963,   970,   977,   978,   981,   984,   986,   988,   990,
     992,   994,   996,   998,  1001,  1003,  1005,  1007,  1010,  1013,
    1019,  1020,  1021,  1024,  1027,  1029,  1031,  1033,  1035,  1037,
    1039,  1041,  1044,  1046,  1048,  1050,  1053,  1056,  1062,  1078,
    1093,  1094,  1106,  1120,  1121,  1124,  1127,  1130,  1133,  1136,
    1139,  1142,  1145,  1148,  1151,  1154,  1157,  1164,  1171,  1178,
    1185,  1189,  1193,  1197,  1201,  1208,  1215,  1222,  1229,  1239,
    1247,  1255,  1264,  1271,  1278,  1288,  1296,  1306,  1321,  1322,
    1327,  1332,  1339,  1343,  1347,  1351,  1357,  1359,  1361,  1363,
    1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,
    1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,  1386,
    1387,  1388,  1391,  1392,  1393,  1395,  1397,  1399,  1401,  1403,
    1406,  1408,  1410,  1412,  1414,  1417,  1420,  1424,  1427,  1430,
    1433,  1435,  1439,  1442,  1445,  1448,  1451,  1454,  1456,  1458,
    1460,  1462,  1464,  1466,  1468,  1470,  1474,  1477,  1478,  1479,
    1480,  1481,  1482,  1483,  1484,  1489,  1494,  1497,  1499,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1514,  1515,  1516,  1522,  1528,  1534,  1537,  1538,  1541,
    1542,  1543,  1546,  1548,  1551,  1552,  1553,  1556,  1557,  1560,
    1561,  1562,  1565,  1566,  1567,  1570,  1571,  1572,  1573,  1574,
    1576,  1578,  1581,  1582,  1583,  1584,  1589,  1592,  1593,  1596,
    1597,  1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,
    1609,  1610,  1611,  1613,  1615,  1621,  1626,  1627,  1629,  1631,
    1636,  1638,  1644,  1650,  1654,  1658,  1663,  1670,  1675,  1681,
    1687,  1694,  1701,  1706,  1711,  1718,  1723,  1728,  1730,  1732,
    1734,  1736,  1738,  1741,  1742,  1743,  1746,  1747,  1748,  1749,
    1758,  1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,  1770,
    1774,  1777,  1779,  1781,  1783,  1785,  1787,  1789,  1791,  1793,
    1795,  1796,  1800,  1804,  1808,  1809,  1811,  1813,  1816,  1817,
    1818,  1820,  1823,  1824
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

#define YYPACT_NINF (-859)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-463)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -859,    12,  2068,  -859,  -859,  -859,  -859,  -859,   716,   -76,
     -59,   -70,    14,  -859,  -859,  4554,  -859,  -859,  -859,    48,
    2143,   245,  3579,  3654,  3729,  3804,  3879,     1,   -20,  3879,
    2218,  2293,   395,    73,  3879,  3879,    62,   113,  3954,  4029,
     100,   222,    19,    30,   147,   147,  4104,   100,   100,  3879,
     140,   175,   167,  4554,  -859,  -859,   250,   264,   283,  4554,
    4554,  -859,  2368,  2444,   253,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,   621,  -859,  -859,  -859,   980,
     649,  -859,   662,   626,   502,   242,  -859,    49,   571,   671,
     758,  -859,  -859,   260,   278,   305,   382,  -859,  -859,  3879,
    3879,  3879,  3879,  3879,  3879,   426,  2519,  4179,  2595,   457,
    3879,  3879,   328,    24,   772,   231,   414,   -52,  -859,   -36,
     100,  2670,  -859,  -859,   366,  1010,   -48,  -859,  2746,  -859,
      18,   -34,   138,   169,   551,   508,   506,   530,  2670,   600,
    2670,    25,  2670,   100,  2670,   100,   -26,   288,   248,  -859,
    -859,  -859,   442,  -859,    65,  -859,    66,   487,   490,   564,
     479,   521,    47,    50,  2670,   100,  2670,   100,   190,  2822,
    -859,  3879,  3879,  -859,  3879,  3879,  -859,  3879,  3879,  3879,
    2897,  3879,  -859,   588,  -859,  -859,  -859,   553,   581,    39,
    -859,   566,  -859,  -859,  -859,  -859,   924,  -859,   719,   738,
    -859,   399,   534,  1438,  3879,  3879,  3879,  3879,  3879,  3879,
    3879,  3879,  -859,  3879,  3879,  3879,  3879,  3879,  3879,  3879,
    3879,  -859,  4554,  4554,  4554,  4554,  4554,  4554,  4554,  4554,
    4554,  4554,  4554,  4554,  4554,  4554,  4554,   793,  4554,  4554,
    4554,  4554,  4554,  4554,  4554,  4554,  4554,  4554,  4554,  4554,
    4554,  4554,  4554,  4554,  4554,  4554,   935,   960,   745,   593,
     605,   609,   620,   629,   635,    94,  -859,   746,  2670,   350,
     768,   678,   655,   670,   684,  -859,   696,   820,   710,   165,
    -859,   842,   761,   765,  2595,   851,  3879,  -859,  3879,  3879,
    3879,  3879,  3879,  3879,  3879,   756,   719,   767,  -859,  3879,
    3879,  -859,  4254,  -859,  4329,  -859,   179,   221,   265,   251,
     779,   789,  -859,   781,   784,  3879,  3879,  3879,  3879,  3879,
    3879,  3879,  -859,  3879,   786,  3879,  3879,  -859,  3879,   797,
     906,   803,  -859,   892,  3879,  -859,  4554,  4554,    20,  -859,
    -859,  -859,  3879,  3879,  -859,  3879,  3879,  -859,  3879,  3879,
     777,  3879,  -859,   901,  3879,   908,  3879,   805,   861,   810,
    -859,  3879,  3879,   914,   796,   817,   826,   828,   845,   835,
    -859,  -859,  -859,   100,    21,  -859,    53,   850,   855,  -859,
    -859,  2972,   883,   982,  3048,  -859,  -859,  1094,   114,   115,
     905,  3579,  3654,  3879,  1783,   227,   354,  -859,   373,   860,
     894,   933,   953,   957,   974,   978,  1017,  1029,  1041,  1044,
    1045,  1047,  1048,  1049,  1053,   926,   926,   926,   926,   926,
     926,   944,   926,   926,   926,   926,   926,   926,   926,   926,
    -859,  -859,  -859,   502,  -859,  -859,  -859,   932,   932,   571,
     671,   671,   758,   758,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,   979,  1126,  1054,  -859,  3123,  -859,  -859,
    -859,  -859,  -859,  -859,  3879,  3199,  -859,   812,  -859,  3879,
    3879,  3879,  3879,  1068,  1070,  1071,  -859,  3275,  -859,  1150,
    3879,  1058,  -859,    -6,  -859,  1302,   695,  1072,  1055,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  1060,  -859,  1075,
    2670,   460,  1128,   232,  3351,   462,  1076,  3879,  -859,  4404,
    -859,  3879,  -859,    59,  -859,  3879,  3879,  1093,  2972,  1081,
    1082,  1083,  1084,  1085,  1088,  1089,   515,  2972,  1090,  1091,
     577,   100,   -12,  4479,  -859,   100,   100,    99,   -28,    -5,
      17,  -859,   187,  -859,  -859,  -859,  1092,   822,  1573,  1096,
    1103,  1104,  1105,  1117,  1120,  -859,  1121,   -24,  1108,  1087,
    1134,  1118,   100,   100,   100,  1129,  1130,  1140,  3879,  -859,
    -859,  -859,  -859,  -859,  -859,  1247,  1248,  -859,    55,  1092,
     829,   236,   476,     2,  -859,   831,  3879,  -859,  -859,   698,
    1249,  2595,  3879,  3879,   396,    26,    63,  -859,   391,  3879,
    3879,  -859,  3879,  3879,  -859,  3879,  3879,  3879,  3879,  3879,
    3879,  3879,  3879,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    4554,  3427,  -859,  1226,  1135,   833,  1136,  -859,   838,  2972,
     578,  1133,  1137,   707,  3879,  3879,  3879,  -859,   864,  -859,
     870,   874,  -859,  3879,  1189,  1257,   876,  -859,  -859,  -859,
    -859,  -859,   878,  -859,  3879,  3879,  3879,   884,  -859,   215,
    -859,  1143,  2670,   594,  1145,  1092,   886,  1146,  1147,  3879,
     900,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,   902,
    -859,  -859,  -859,  1009,  -859,  -859,  2670,   100,  -859,  -859,
    1275,  -859,  1277,  1279,  1276,  3879,  1175,  1284,  1285,  -859,
    1092,   672,  3579,  3654,  1858,  -859,   420,  -859,  -859,  -859,
    -859,  -859,  -859,  3879,   100,  -859,  -859,  -859,  1166,  1167,
    1272,  -859,  -859,  -859,  -859,  3879,  1170,  -859,  -859,    87,
     887,   783,  1242,  -859,  -859,  1092,  1092,   911,   913,  1288,
    1289,  1290,  -859,  -859,  1179,  -859,   136,   713,  1173,   128,
    3879,  3879,  3879,  3879,  3879,  3879,  -859,  -859,  -859,  -859,
     208,   267,   271,   276,   306,   315,   327,   339,   340,   344,
     362,   367,  -859,  -859,   915,  -859,   917,   919,  -859,  1181,
    -859,  -859,   927,  -859,  -859,  -859,  -859,  1193,  1194,  1195,
    -859,  -859,  -859,  1197,  3879,  1199,  -859,  2972,   633,  -859,
    -859,  1196,  1251,  1252,  -859,   945,  -859,  3879,  -859,  -859,
    1092,  -859,  -859,   634,  1205,  1206,   -12,  -859,   949,  1069,
     100,   100,   100,  -859,  -859,  3879,  -859,  -859,  -859,    28,
    -859,   189,  -859,  -859,  -859,   525,    57,  -859,   401,  3879,
    3879,  3879,  3879,  3879,  3879,  3879,  3879,  -859,   100,  1200,
    -859,  -859,   100,  1208,  3879,  -859,   324,   325,     4,   209,
     408,  1209,  1211,   935,  -859,  -859,  1092,  1092,  1335,  1337,
    1340,  -859,  3879,  3503,  1232,  1235,  -859,   368,   372,   376,
     377,   388,   400,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  1222,
    -859,  -859,  -859,  -859,  -859,  3879,   995,  -859,  -859,  3879,
    3879,  2972,   675,  -859,  -859,  3879,  -859,  -859,  -859,  -859,
     100,   100,  -859,  -859,  -859,  -859,  -859,  1339,  3879,  1245,
    1353,  1355,  3879,  3879,  3879,  -859,  -859,  -859,   406,   423,
     428,   443,   444,   447,   448,   459,  -859,  -859,  -859,  3879,
    1234,  -859,   261,  1292,  -859,   297,  -859,  1356,  1357,  1243,
    3879,  -859,  1358,  1361,  1246,  3879,  -859,   255,   256,  1262,
    1263,  -859,  -859,   473,  -859,  -859,  -859,  -859,  -859,   461,
    1258,   997,  3879,  3879,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  1264,  1267,  -859,  -859,   999,  -859,   676,  1106,  -859,
    -859,  -859,  3879,  -859,  -859,   498,   503,   519,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  1268,   100,  1265,  1003,
    1269,  1708,  1265,  1013,  1270,  1278,  -859,  1280,  1281,  1282,
    -859,  1283,  1380,  1385,  1286,  3879,  -859,  1388,  1391,  1287,
    3879,  -859,  -859,  -859,   363,  -859,  -859,  -859,  1291,   529,
     535,  -859,  -859,  1293,  -859,   100,  -859,  -859,  -859,  -859,
    -859,  3879,  -859,  -859,  1265,  -859,  3579,  3654,  1933,  -859,
     540,  -859,  1265,  -859,  -859,  -859,  -859,  -859,  -859,  1294,
    1295,  -859,  1297,  1299,  1300,  -859,  1301,  1265,  1019,  -859,
    -859,  -859,  -859,  -859,  1303,  -859,   595,   352,  -859,   422,
    3879,  3879,  3879,  3879,  3879,  3879,  3879,  3879,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  1265,   100,  3879,
    3879,  3879,  -859,  -859,  -859,   539,   560,   563,   568,   569,
     599,   607,   615,  -859,  -859,   623,   624,   625,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   426,   427,   428,   430,   431,   436,
     437,   451,   450,   452,   453,     0,   438,   439,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,   429,     0,     0,     0,     0,
       0,    98,     0,     0,     3,     4,    99,   100,   107,   108,
     109,   110,   101,   102,   117,     0,   118,   119,   120,   106,
       0,   326,   328,   346,   348,   351,   353,   356,   358,   361,
     364,   371,   376,   378,   393,   422,     0,   434,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,   436,   437,   451,   450,
       0,     0,   462,   374,     0,   431,   436,    35,     0,   423,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   356,     0,   463,
     325,   324,     0,    48,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
     146,     0,     0,   111,     0,     0,   112,     0,     0,     0,
       3,     0,   420,   103,   105,   417,   210,     0,     0,     0,
     375,     0,   432,   433,   373,   372,   431,   458,     0,     0,
     454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   319,
     320,   321,   322,   323,   317,     0,   400,     0,     0,   318,
       0,     0,     0,     0,     0,    39,   405,     0,   404,   450,
     396,     0,     0,     0,     0,     0,     0,    29,     0,     0,
       0,     0,     0,     0,     0,   458,     0,     0,    38,   425,
       0,     6,     0,    10,     0,    11,     0,     0,     0,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,    43,     0,     0,
     124,     0,   136,     0,     0,   155,     0,     0,     0,     3,
     162,    46,     0,     0,    47,     0,     0,    51,     0,     0,
       0,     0,   158,     0,     0,     0,     0,     0,   139,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     418,   419,   421,     0,     0,   213,     0,     0,     0,   248,
     459,     0,     0,     0,     0,   455,   283,   431,   436,   437,
     451,     0,     0,     0,     0,     0,     0,   282,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   331,   332,   335,   336,   329,
     330,     0,   333,   334,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   347,   349,   350,   352,   354,   355,   357,
     359,   360,   362,   363,   368,   369,   370,   365,   366,   367,
     377,   381,   392,   385,   386,   387,   388,   391,   389,   390,
     383,   384,   382,   416,     0,   413,    58,     0,    92,    93,
      94,    95,    96,    55,     0,     0,   399,     0,    15,     0,
       0,     0,     0,   441,   446,   447,    60,     0,   408,     0,
       0,     0,   397,     0,   395,   425,     0,     0,     0,   319,
     320,   321,   322,   323,   317,   318,    37,     0,   424,     0,
       0,     0,   428,   450,     0,     0,     0,     0,    26,     0,
      27,     0,    28,     0,   165,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   459,     0,     0,   127,   459,     0,     0,   354,   355,
     260,   263,     0,   276,   278,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,   394,     0,     0,     0,
       0,     0,   459,     0,   459,     0,     0,     0,     0,   113,
     114,   115,   116,   316,   104,     0,     0,   213,     0,     0,
       0,     0,     0,     0,   460,     0,     0,   380,   456,     0,
       0,     0,     0,     0,     0,     0,     0,   309,     0,     0,
       0,   310,     0,     0,   311,     0,     0,     0,     0,     0,
       0,     0,     0,   284,    62,    64,    66,    69,    67,    68,
      63,    65,    75,    77,    79,    82,    80,    81,    76,    78,
       0,     0,   409,     0,     0,     0,     0,   172,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,   403,
       0,     0,   252,     0,     0,     0,     0,   441,   446,   447,
      36,     5,     0,    12,     0,     0,     0,     0,    19,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    86,    85,    89,    87,    88,    84,    41,     0,
      90,    91,    44,   121,   126,   125,     0,     0,   135,   152,
       0,   154,     0,     0,     0,     0,   265,     0,     0,   161,
       0,     0,     0,     0,     0,   173,     0,    49,    50,    53,
      54,    31,    30,     0,     0,   156,   448,   449,     0,     0,
     137,   140,   141,   144,   145,     0,     0,   211,   212,     0,
       0,     0,     0,   209,   170,     0,     0,     0,     0,     0,
       0,     0,   247,   461,     0,   457,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,   300,   307,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,   415,     0,   412,     0,     0,    57,     0,
      59,   171,     0,    16,   444,   443,   442,     0,     0,     0,
     406,   401,   402,     0,     0,     0,   398,     0,     0,   257,
     256,     0,     0,     0,    21,     0,    22,     0,    25,   164,
       0,    33,    32,     0,   461,   461,     0,   128,     0,   129,
       0,     0,     0,   261,   272,     0,   266,   267,   160,   268,
     264,     0,   277,   279,   281,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   174,     0,     0,
      73,    74,     0,     0,     0,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   169,   167,     0,     0,     0,     0,
       0,     9,     0,     0,   441,   447,   304,     0,     0,     0,
       0,     0,     0,   312,   313,   314,   315,   285,   287,   289,
     292,   290,   291,   286,   288,   414,   411,   410,    56,   461,
      70,    71,    72,   253,   254,     0,     0,    13,    20,     0,
       0,     0,     0,   163,     7,     0,    42,    45,   123,   122,
     459,     0,   133,   153,   150,   151,   274,     0,     0,   269,
       0,     0,     0,     0,     0,   177,   178,   175,     0,     0,
       0,     0,     0,     0,     0,     0,   159,   157,   138,     0,
       0,   149,     0,     3,   243,     0,   240,     0,     0,     0,
       0,   219,     0,     0,     0,     0,   214,     0,     0,     0,
       0,   215,   216,     0,   166,   168,   249,   250,   251,     0,
     172,     0,     0,     0,   294,   296,   299,   297,   298,   295,
      17,     0,   461,   259,   258,     0,    23,     0,   130,   131,
     262,   273,     0,   270,   271,     0,     0,     0,   179,   181,
     183,   186,   184,   185,   180,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,     0,     0,     0,   221,     0,     0,     0,
       0,   220,   217,   218,     0,   246,   303,   302,   171,     0,
       0,   255,    14,   461,     8,     0,   134,   275,   187,   189,
     188,     0,   148,   242,     0,   191,     0,     0,     0,   190,
       0,   239,     0,   228,   229,   223,   226,   227,   222,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,   301,
     305,   306,    24,   132,     0,   241,     0,     0,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,   238,
     236,   237,   225,   234,   235,   224,   245,     0,     0,     0,
       0,     0,   195,   196,   193,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   147,     0,     0,     0,   197,   199,
     201,   204,   202,   203,   198,   200,   205,   207,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -859,  -859,     3,  -859,  -533,  -698,  -859,  -859,  -859,  -859,
    -859,   326,  1381,  -859,  -859,  -859,  -859,  -255,  -858,  -859,
    -859,  -859,   827,  -859,  -859,  -482,   904,  -370,   308,  1097,
      -2,  -226,  -859,  1155,  1177,  -859,   596,   -15,  1176,   912,
     925,   -11,  -859,  1308,  -256,  -859,  -859,    60,  -859
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   203,    65,    66,   544,    67,    68,    69,    70,
      71,   970,    72,    73,   567,    74,    75,   340,   974,    76,
      77,   374,   588,    78,   593,   282,   515,   557,   122,   178,
     129,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   199,   463,    93,    94,   130,    95
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      80,   465,   421,     2,   123,   837,   590,   660,   664,   704,
      26,   147,     3,   666,    24,   148,   733,   734,   977,   976,
     139,   141,   143,   145,   146,   112,   978,   152,   154,   156,
     550,   551,   160,   161,   552,   585,   165,   167,   276,   714,
     107,   277,   190,   586,   182,   171,   111,   185,   194,   195,
     947,   325,   325,   377,   524,   712,   174,   333,   108,   109,
     198,   201,   124,   550,   551,   172,   285,   552,   294,   550,
     551,   326,   326,   552,   162,   979,   175,   110,   713,   759,
     760,   761,   300,   325,   286,   278,   284,   159,   553,   554,
     555,   342,   345,   113,   244,   114,   301,   259,   260,   261,
     262,   263,   264,   326,   378,   269,   271,   334,   273,   274,
      64,   343,   346,   735,   750,   751,    64,   244,   752,   198,
     980,   553,   554,   555,    64,   163,   296,   553,   554,   555,
     242,   243,   665,   715,   981,  1065,   314,    64,   324,   762,
     329,   942,   331,   113,   948,   114,   750,   751,   298,   173,
     752,   299,   556,   686,   186,   327,   327,   587,   328,   328,
     176,   796,   357,   777,   359,   353,   267,   364,   355,   365,
     366,   244,   367,   368,   283,   369,   370,   371,   369,   372,
    1083,   188,   710,   354,  1091,   589,   356,   327,   297,   187,
     328,   685,   753,   341,   956,   344,   347,   716,   189,   949,
     778,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     474,   407,   408,   409,   410,   411,   412,   413,   414,   361,
     822,   757,   758,   982,   875,   475,  1115,   437,   438,   823,
     107,   983,   299,   109,  1129,    64,    64,   362,   444,   445,
     446,   447,   448,   449,   450,   490,   550,   551,   601,  1136,
     552,   602,   892,   609,   302,   131,   132,   133,   486,   134,
     717,   718,   950,   951,   191,   896,   477,   893,   303,  1052,
    1057,   550,   551,   610,   192,   552,   135,  1053,  1058,  1153,
     984,   136,   496,   177,   498,   304,   499,   500,   501,   502,
     503,   504,   505,   193,   113,   517,   114,   508,   509,   305,
     511,   719,   516,   938,   553,   554,   555,   550,   551,   518,
      79,   552,   676,   529,   530,   531,   532,   533,   534,   535,
    1076,   548,   549,   538,   539,   985,  1054,  1059,   202,   553,
     554,   555,    97,    98,   754,   149,   151,   519,   579,   986,
     559,   560,   558,   561,   562,   903,   563,   564,   168,   170,
     241,   520,   569,   169,   571,   183,   184,   173,    64,   575,
     576,   113,   106,   114,   611,   553,   554,   555,   756,   336,
     337,  1055,  1060,   550,   551,   255,   137,   552,   325,   338,
     612,   521,   523,   536,   339,  1056,  1061,   339,   540,   604,
     605,   606,   608,  1038,   547,   522,   258,   580,   326,   615,
     613,   581,   616,   617,   904,   157,   582,   256,   905,   158,
     244,   566,   618,   906,   792,   619,   620,   342,   987,   621,
     622,   988,   770,   989,   990,   316,   771,   342,   149,  1042,
     829,   553,   554,   555,   257,   772,   632,   343,   773,   774,
     265,   595,   775,   907,   599,   633,   859,   343,   342,   860,
     861,   330,   908,   332,   335,   972,   975,   634,   275,   862,
     973,   973,   863,   864,   909,   848,   865,   866,   343,   635,
     636,   272,   646,   358,   637,   360,   910,   911,   651,   652,
     478,   912,   327,   479,   176,   328,   550,   551,   662,  1143,
     552,   614,   638,   508,   877,  1107,   287,   639,   691,   913,
     884,   885,   693,   221,   914,  1004,   694,   695,   672,  1005,
     623,    79,   198,  1006,  1007,   681,   310,   683,   696,   684,
     311,   344,   309,   687,   688,  1008,   322,   645,   779,   323,
     697,   344,   384,   776,   385,   648,   632,  1009,   957,   650,
     312,   707,   653,  1028,   553,   554,   555,   658,   284,   661,
     221,   952,   344,   633,   316,   953,   726,   867,   634,  1144,
    1029,   306,   307,   308,   318,  1030,  1120,   319,   320,  1121,
    1122,   954,   341,   635,   636,   933,   746,   637,   638,  1123,
    1031,  1032,  1124,  1125,  1033,  1034,  1126,  1127,   690,   639,
     673,   800,   678,   674,   764,   679,  1035,   699,  1066,   767,
     768,   769,  1039,   348,  1064,  1043,   349,   780,   781,   973,
     782,   783,   351,   784,   785,   786,   787,   788,   789,   790,
     791,  1139,   239,   240,   316,  1140,   315,   993,   691,   316,
     317,   994,   995,   693,   318,  1078,   350,   319,   320,   318,
    1079,  1141,   319,   320,   373,   698,   321,   204,   299,   697,
     205,   206,   807,   808,   809,   322,  1080,   352,   323,   920,
     207,   813,   955,   208,   209,   922,  1110,   210,   211,   632,
     221,   386,  1111,   819,   820,   213,  1158,  1128,   214,   215,
     825,   584,   849,   850,   245,   246,   851,   833,   216,   375,
     633,   217,   218,   634,  1108,   219,   220,  1159,   635,   636,
    1160,   794,   379,   797,   838,  1161,  1162,   702,   803,   802,
     299,   299,   376,   844,   231,   232,   233,   234,   235,   236,
     855,   856,   858,   468,   826,   322,   237,   827,   323,   637,
     322,   868,  1142,   323,   818,   469,  1163,   638,   238,   470,
     852,   853,   854,   873,  1164,   639,   222,   223,   224,   225,
     471,   212,  1165,   691,   693,   697,   226,   227,   228,   472,
    1166,  1167,  1168,   927,   934,   473,   299,   935,   897,   898,
     899,   900,   901,   902,   484,     4,     5,     6,     7,   221,
     229,   230,   125,   126,   117,   118,   279,    13,    14,    15,
      16,    17,    18,   878,   247,   248,   879,   880,   881,   882,
     430,   431,   432,   120,    28,  1016,  1074,   485,   299,   299,
      34,   482,   924,   483,   486,    96,    36,    97,    98,    99,
     100,   101,   102,   103,    44,    45,    46,   487,   482,    49,
     667,   299,   104,   765,   488,   434,   435,   436,    53,    54,
     299,   489,   806,   946,    55,   105,   482,   106,   894,   703,
     705,   380,   381,   708,   709,   711,   491,   958,   959,   960,
     961,   962,   963,   964,   965,   497,    79,   249,   250,   251,
     382,   383,   971,    57,    58,   466,   467,   926,   476,   299,
     740,   741,   742,   252,   253,   254,   506,   932,   573,    59,
     999,   480,   481,   492,   493,   525,    60,   494,   495,   507,
     299,   451,   546,   121,   280,   526,    63,   565,    64,   452,
     281,   453,   454,   527,   383,   568,   380,   528,   380,   537,
     455,   456,   570,  1011,   577,   457,   578,  1013,  1014,   541,
     381,   458,   459,   542,   543,   545,   381,   572,   381,   460,
     461,   462,   574,   381,   380,   649,  1021,   579,   876,   451,
    1025,  1026,  1027,  1001,   720,   721,   580,   452,   581,   453,
     454,   755,   721,   763,   299,   799,   299,  1036,   455,   456,
     801,   299,   583,   457,   451,   582,  1041,   464,  1047,   458,
     459,   591,   452,  1051,   453,   454,   592,   460,   461,   462,
     624,  1015,   597,   455,   456,  1017,   810,   299,   457,   596,
    1069,  1070,   811,   493,   458,   459,   812,   299,   816,   493,
     380,   817,   460,   461,   462,   839,   821,   679,   830,   721,
    1077,   603,  -462,  -462,   625,    97,    98,   288,   289,   290,
     291,   292,   834,   299,   835,   299,   836,   543,   238,  1090,
     293,   640,   869,   886,   721,   887,   721,   915,   299,   916,
     493,   917,   299,  1102,   244,   106,  -379,  -379,  1106,   919,
     299,  -462,  -462,   626,  -462,  -462,  -462,  -462,  -462,  -462,
    -462,  -462,  -462,  -462,  -462,  -462,  -462,   380,   931,   971,
    -462,   940,   381,   627,  1116,  1117,  1119,   628,  -462,  -462,
    -462,  -462,  -462,  -462,  -462,  -462,   941,   543,  -462,  -462,
    -462,  -462,  -462,  -462,   629,  -462,  -462,  -462,   630,  -462,
     641,    97,    98,   288,   289,   290,   291,   292,  1145,  1146,
    1147,  1148,  1149,  1150,  1151,  1152,   293,  1012,   299,  1068,
     299,  1073,   299,  1075,   543,  1084,   721,  1155,  1156,  1157,
     642,   106,   179,   181,   939,  1092,   721,   631,   943,   944,
     945,  1137,   721,     4,     5,     6,     7,   440,   441,   632,
     125,   126,   117,   118,   279,    13,    14,    15,    16,    17,
      18,   633,   442,   443,   634,   635,   966,   636,   637,   638,
     968,   120,    28,   639,   654,   643,   655,   656,    34,   663,
     670,   668,   669,    96,    36,    97,    98,    99,   100,   101,
     102,   103,    44,    45,    46,   671,   680,    49,   675,   689,
     104,   691,   692,   693,   694,   695,    53,    54,   696,   697,
     700,   701,    55,   600,   737,   106,   727,   736,   339,     4,
       5,     6,     7,   728,   729,   730,   125,   126,   117,   118,
     279,    13,    14,    15,    16,    17,    18,   731,  1018,  1019,
     732,    57,    58,   738,   299,   739,   745,   120,    28,   743,
     744,   747,   748,   766,    34,   798,   800,    59,   804,   814,
      36,   815,   805,   824,    60,   828,   831,   832,    44,    45,
      46,   121,   659,    49,    63,   840,    64,   841,   281,   842,
     843,   845,    53,    54,   846,   847,   870,   871,    55,   872,
     874,   883,   888,   889,   890,     4,     5,     6,     7,   891,
     895,   918,   125,   126,   117,   118,   279,    13,    14,    15,
      16,    17,    18,   920,   921,   922,   928,    57,    58,   923,
     925,   929,   930,   120,    28,   936,   937,   967,   969,   991,
      34,   992,   996,    59,   997,  1082,    36,   998,  1002,    79,
      60,  1003,  1010,  1020,    44,    45,    46,   121,   795,    49,
      63,  1022,    64,  1023,   281,  1024,  1037,  1040,    53,    54,
    1044,  1045,  1048,  1046,    55,  1049,  1050,   415,   416,   417,
     418,   419,   420,  1113,   422,   423,   424,   425,   426,   427,
     428,   429,  1062,  1063,  1099,  1067,  1071,  1072,  1081,  1100,
    1093,   973,  1103,    57,    58,  1104,  1085,  1114,  1094,   150,
    1095,  1096,  1097,  1098,   749,   433,  1101,  1105,   677,    59,
     439,     0,     0,  1112,  1130,  1131,    60,  1132,  1109,  1133,
    1134,  1135,     0,   121,     0,  1138,    63,     0,    64,     0,
     281,     4,     5,     6,     7,   313,  1154,     0,   387,   388,
     389,   390,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   391,   392,    24,     0,     0,    25,    26,    27,
      28,   393,   394,    31,    32,    33,    34,    35,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,   395,   396,
      44,    45,    46,    47,    48,    49,    50,     0,     0,     0,
      51,    52,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,    61,    62,
       0,     0,    63,     0,    64,   397,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   722,   723,    24,
       0,     0,    25,    26,    27,    28,    29,   724,    31,    32,
      33,    34,    35,     0,     0,     0,     0,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     0,     0,     0,    51,    52,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,    61,    62,     0,     0,    63,     0,    64,
     725,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,  1086,  1087,    24,     0,     0,    25,    26,    27,
      28,    29,  1088,    31,    32,    33,    34,    35,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     0,     0,     0,
      51,    52,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,    56,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,    61,    62,
      49,     0,    63,     0,    64,  1089,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,   153,   121,    49,     0,    63,     0,    64,
     607,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,   153,   121,
      49,     0,    63,     0,    64,   857,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,   153,   121,     0,     0,    63,     0,    64,
    1118,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,     0,     0,
       0,     0,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     0,     0,     0,
      51,    52,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,    56,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,    61,    62,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,   127,   128,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,   153,   121,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   196,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,   155,   121,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,    59,     0,     0,    36,     0,
       0,     0,    60,     0,     0,     0,    44,    45,    46,   121,
     197,    49,    63,     0,    64,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   121,    49,     0,    63,   200,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   125,   126,   117,   118,   119,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   120,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,     0,    60,     0,     0,     0,    44,    45,    46,
     121,   266,    49,    63,     0,    64,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     125,   126,   117,   118,   119,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,   120,    28,     0,     0,     0,     0,     0,    34,     0,
       0,     0,    59,     0,    36,     0,     0,     0,     0,    60,
     270,     0,    44,    45,    46,     0,   121,    49,     0,    63,
       0,    64,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   125,   126,   117,   118,
     119,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,   120,    28,     0,
       0,     0,     0,     0,    34,     0,     0,    59,     0,     0,
      36,     0,     0,     0,    60,     0,     0,     0,    44,    45,
      46,   121,   197,    49,    63,     0,    64,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,   363,     0,    57,    58,     0,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,    59,     0,     0,    36,     0,     0,     0,
      60,     0,     0,     0,    44,    45,    46,   121,   295,    49,
      63,     0,    64,     0,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   125,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,     0,    60,     0,     0,    44,
      45,    46,     0,   121,    49,     0,    63,     0,    64,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,   202,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,     0,   121,    49,
       0,    63,     0,    64,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,    59,
       0,     0,    36,     0,     0,     0,    60,     0,     0,     0,
      44,    45,    46,   121,   594,    49,    63,     0,    64,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   121,
      49,     0,    63,   598,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
      59,     0,     0,    36,     0,     0,     0,    60,     0,     0,
       0,    44,    45,    46,   121,   644,    49,    63,     0,    64,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   125,   126,   117,   118,   119,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   120,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,     0,    60,     0,     0,     0,    44,    45,    46,
     121,   647,    49,    63,     0,    64,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     4,     5,   512,     7,     0,     0,
       0,   125,   126,   117,   118,   513,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,   120,    28,     0,     0,     0,     0,     0,    34,
       0,     0,    59,     0,     0,    36,     0,     0,     0,    60,
       0,     0,     0,    44,    45,    46,   121,   657,    49,    63,
       0,    64,     0,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   125,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,     0,    60,     0,     0,     0,    44,
      45,    46,   121,   197,    49,    63,     0,    64,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,    59,     0,     0,    36,     0,     0,
       0,    60,     0,     0,     0,    44,    45,    46,   121,   793,
      49,    63,     0,    64,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
      59,     0,     0,    36,     0,     0,     0,    60,     0,     0,
       0,    44,    45,    46,   121,  1000,    49,    63,     0,    64,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     138,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   140,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     142,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   144,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     121,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   164,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     166,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   121,    49,     0,    63,     0,
     180,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     268,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,   512,     7,     0,     0,     0,   125,
     126,   117,   118,   513,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   510,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     514,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,     0,
       0,    44,    45,    46,     0,   682,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   115,   116,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,    34,     0,     0,     0,    59,     0,    36,     0,
       0,     0,     0,    60,     0,     0,    44,    45,    46,     0,
     706,    49,     0,    63,     0,    64,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,    60,     0,
       0,     0,     0,     0,     0,   121,     0,     0,    63,     0,
      64
};

static const yytype_int16 yycheck[] =
{
       2,   257,   228,     0,    15,   703,   376,   489,    14,   542,
      30,    26,     0,   495,    26,    14,    40,    41,    14,   877,
      22,    23,    24,    25,    26,    11,    22,    29,    30,    31,
      10,    11,    34,    35,    14,    14,    38,    39,    14,    22,
     116,    17,    53,    22,    46,    26,   116,    49,    59,    60,
      22,    26,    26,    14,   309,    83,    26,    83,   134,   118,
      62,    63,    14,    10,    11,    46,   118,    14,   116,    10,
      11,    46,    46,    14,    12,    71,    46,   136,    83,    77,
      78,    79,   116,    26,   136,    61,   134,    14,    68,    69,
      70,    26,    26,   129,   122,   131,   130,    99,   100,   101,
     102,   103,   104,    46,    65,   107,   108,   133,   110,   111,
     136,    46,    46,   137,    59,    60,   136,   122,    63,   121,
     116,    68,    69,    70,   136,    12,   128,    68,    69,    70,
      81,    82,   138,   116,   130,   993,   138,   136,   140,   137,
     142,   839,   144,   129,   116,   131,    59,    60,   130,   130,
      63,   133,   132,   523,    14,   130,   130,   136,   133,   133,
     130,   643,   164,   137,   166,   118,   106,   169,   118,   171,
     172,   122,   174,   175,   114,   177,   178,   179,   180,   181,
    1038,    14,    83,   136,  1042,   132,   136,   130,   128,    14,
     133,   132,   137,   130,   137,   130,   130,    10,    31,    10,
     137,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     116,   213,   214,   215,   216,   217,   218,   219,   220,    29,
       5,   591,   592,    14,   137,   131,  1084,   242,   243,    14,
     116,    22,   133,   118,  1092,   136,   136,    47,   249,   250,
     251,   252,   253,   254,   255,    80,    10,    11,   134,  1107,
      14,   136,   116,    26,   116,    10,    11,    12,   130,    14,
      73,    74,    73,    74,    14,   137,   268,   131,   130,    14,
      14,    10,    11,    46,    10,    14,    31,    22,    22,  1137,
      71,    36,   284,   136,   286,   116,   288,   289,   290,   291,
     292,   293,   294,    10,   129,   116,   131,   299,   300,   130,
     302,   556,   304,   836,    68,    69,    70,    10,    11,   130,
       2,    14,    80,   315,   316,   317,   318,   319,   320,   321,
    1018,   336,   337,   325,   326,   116,    71,    71,    75,    68,
      69,    70,   101,   102,   589,    27,    28,   116,   130,   130,
     342,   343,   339,   345,   346,   137,   348,   349,    40,    41,
     108,   130,   354,   131,   356,    47,    48,   130,   136,   361,
     362,   129,   131,   131,   137,    68,    69,    70,   132,    81,
      82,   116,   116,    10,    11,   115,   131,    14,    26,   131,
      26,   116,   131,   323,   136,   130,   130,   136,   328,   391,
     392,   393,   394,   132,   334,   130,    14,   130,    46,    26,
      46,   130,    29,    30,   137,    10,   130,   129,   137,    14,
     122,   351,    39,   137,   640,    42,    43,    26,    10,    46,
      47,    13,    26,    15,    16,    29,    30,    26,   120,   132,
     685,    68,    69,    70,   129,    39,   130,    46,    42,    43,
      14,   381,    46,   137,   384,   130,    26,    46,    26,    29,
      30,   143,   137,   145,   146,   131,   131,   130,   130,    39,
     136,   136,    42,    43,   137,   720,    46,    47,    46,   130,
     130,    14,   474,   165,   130,   167,   137,   137,   480,   481,
     130,   137,   130,   133,   130,   133,    10,    11,   490,   137,
      14,   137,   130,   495,   750,   132,   130,   130,   130,   137,
     755,   756,   130,   130,   137,   137,   130,   130,   510,   137,
     137,   203,   514,   137,   137,   517,    10,   519,   130,   521,
      14,   130,    14,   525,   526,   137,   130,   467,   137,   133,
     130,   130,   133,   137,   135,   475,   130,   137,   137,   479,
      10,   543,   482,   137,    68,    69,    70,   487,   134,   489,
     130,    26,   130,   130,    29,    30,   558,   137,   130,   137,
     137,    10,    11,    12,    39,   137,    26,    42,    43,    29,
      30,    46,   130,   130,   130,   830,   578,   130,   130,    39,
     137,   137,    42,    43,   137,   137,    46,    47,   528,   130,
     130,   130,   130,   133,   596,   133,   137,   537,   137,   601,
     602,   603,   972,   116,   131,   975,   116,   609,   610,   136,
     612,   613,   133,   615,   616,   617,   618,   619,   620,   621,
     622,    26,   120,   121,    29,    30,    26,   883,   130,    29,
      30,   886,   887,   130,    39,   137,    72,    42,    43,    39,
     137,    46,    42,    43,    56,   130,    46,    26,   133,   130,
      29,    30,   654,   655,   656,   130,   137,   136,   133,   130,
      39,   663,   137,    42,    43,   130,   137,    46,    47,   130,
     130,   137,   137,   675,   676,    26,   137,   137,    29,    30,
     682,   373,    10,    11,   113,   114,    14,   689,    39,   136,
     130,    42,    43,   130,  1064,    46,    47,   137,   130,   130,
     137,   641,   136,   643,   706,   137,   137,   130,   130,   649,
     133,   133,   131,   715,    88,    89,    90,    91,    92,    93,
     722,   723,   724,   130,   130,   130,   100,   133,   133,   130,
     130,   733,   137,   133,   674,   130,   137,   130,   112,   130,
      68,    69,    70,   745,   137,   130,    84,    85,    86,    87,
     130,   130,   137,   130,   130,   130,    94,    95,    96,   130,
     137,   137,   137,   130,   130,   130,   133,   133,   770,   771,
     772,   773,   774,   775,   119,     3,     4,     5,     6,   130,
     118,   119,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    10,   123,   124,    13,    14,    15,    16,
       7,     8,     9,    31,    32,   130,   130,   137,   133,   133,
      38,   133,   814,   135,   130,    99,    44,   101,   102,   103,
     104,   105,   106,   107,    52,    53,    54,   131,   133,    57,
     135,   133,   116,   135,    14,   239,   240,   241,    66,    67,
     133,   131,   135,   845,    72,   129,   133,   131,   135,   541,
     542,   132,   133,   545,   546,   547,    14,   859,   860,   861,
     862,   863,   864,   865,   866,    14,   558,   109,   110,   111,
     132,   133,   874,   101,   102,   130,   131,   817,   132,   133,
     572,   573,   574,   125,   126,   127,   130,   827,    27,   117,
     892,   123,   124,   132,   133,   116,   124,   132,   133,   132,
     133,    14,    10,   131,   132,   116,   134,   130,   136,    22,
     138,    24,    25,   132,   133,    14,   132,   133,   132,   133,
      33,    34,    14,   925,    10,    38,   130,   929,   930,   132,
     133,    44,    45,    27,    28,   132,   133,   132,   133,    52,
      53,    54,   132,   133,   132,   133,   948,   130,    61,    14,
     952,   953,   954,   893,   132,   133,   130,    22,   130,    24,
      25,   132,   133,   132,   133,   132,   133,   969,    33,    34,
     132,   133,   137,    38,    14,   130,   973,    17,   980,    44,
      45,   131,    22,   985,    24,    25,   131,    52,    53,    54,
     130,   931,    10,    33,    34,   935,   132,   133,    38,   116,
    1002,  1003,   132,   133,    44,    45,   132,   133,   132,   133,
     132,   133,    52,    53,    54,   707,   132,   133,   132,   133,
    1022,   116,    42,    43,   130,   101,   102,   103,   104,   105,
     106,   107,   132,   133,   132,   133,    27,    28,   112,  1041,
     116,    97,   734,   132,   133,   132,   133,   132,   133,   132,
     133,   132,   133,  1055,   122,   131,   132,   133,  1060,   132,
     133,    81,    82,   130,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,   132,   133,  1081,
     100,   132,   133,   130,  1086,  1087,  1088,   130,   108,   109,
     110,   111,   112,   113,   114,   115,    27,    28,   118,   119,
     120,   121,   122,   123,   130,   125,   126,   127,   130,   129,
     131,   101,   102,   103,   104,   105,   106,   107,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,   116,   132,   133,   132,
     133,   132,   133,    27,    28,   132,   133,  1139,  1140,  1141,
      14,   131,    45,    46,   836,   132,   133,   130,   840,   841,
     842,   132,   133,     3,     4,     5,     6,   245,   246,   130,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   130,   247,   248,   130,   130,   868,   130,   130,   130,
     872,    31,    32,   130,   116,   131,   116,   116,    38,   131,
     130,   119,   137,    99,    44,   101,   102,   103,   104,   105,
     106,   107,    52,    53,    54,   130,   130,    57,    80,   116,
     116,   130,   130,   130,   130,   130,    66,    67,   130,   130,
     130,   130,    72,   129,   137,   131,   130,   119,   136,     3,
       4,     5,     6,   130,   130,   130,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,   130,   940,   941,
     130,   101,   102,   119,   133,   137,   116,    31,    32,   130,
     130,    14,    14,    14,    38,   130,   130,   117,   135,    80,
      44,    14,   135,   130,   124,   130,   130,   130,    52,    53,
      54,   131,   132,    57,   134,    10,   136,    10,   138,    10,
      14,   116,    66,    67,    10,    10,   130,   130,    72,    27,
     130,    59,    14,    14,    14,     3,     4,     5,     6,   130,
     137,   130,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   130,   130,   130,   130,   101,   102,   132,
     131,    80,    80,    31,    32,   130,   130,   137,   130,   130,
      38,   130,     7,   117,     7,  1037,    44,     7,   116,  1041,
     124,   116,   130,    14,    52,    53,    54,   131,   132,    57,
     134,   116,   136,    10,   138,    10,   132,    75,    66,    67,
      14,    14,    14,   130,    72,    14,   130,   222,   223,   224,
     225,   226,   227,  1075,   229,   230,   231,   232,   233,   234,
     235,   236,   130,   130,    14,   137,   132,   130,   130,    14,
     130,   136,    14,   101,   102,    14,   137,  1081,   130,    28,
     130,   130,   130,   130,   587,   238,   130,   130,   514,   117,
     244,    -1,    -1,   130,   130,   130,   124,   130,   137,   130,
     130,   130,    -1,   131,    -1,   132,   134,    -1,   136,    -1,
     138,     3,     4,     5,     6,   137,  1138,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,   134,    -1,   136,   137,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    -1,    -1,    62,    63,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,   134,    -1,   136,
     137,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,   130,   131,
      57,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,   130,   131,    57,    -1,   134,    -1,   136,
     137,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,   130,   131,
      57,    -1,   134,    -1,   136,   137,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,   134,    -1,   136,
     137,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    -1,    -1,
      62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,   130,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,   130,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,   130,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,   130,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    52,    53,    54,   131,
     132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,   135,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    52,    53,    54,
     131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,
     125,    -1,    52,    53,    54,    -1,   131,    57,    -1,   134,
      -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,
      44,    -1,    -1,    -1,   124,    -1,    -1,    -1,    52,    53,
      54,   131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,   101,   102,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    52,    53,    54,   131,   132,    57,
     134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,    52,
      53,    54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,
      -1,    -1,    44,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      52,    53,    54,   131,   132,    57,   134,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,   135,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     117,    -1,    -1,    44,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    52,    53,    54,   131,   132,    57,   134,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    52,    53,    54,
     131,   132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   117,    -1,    -1,    44,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    52,    53,    54,   131,   132,    57,   134,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,
      -1,    44,    -1,    -1,    -1,   124,    -1,    -1,    -1,    52,
      53,    54,   131,   132,    57,   134,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    52,    53,    54,   131,   132,
      57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
     117,    -1,    -1,    44,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    52,    53,    54,   131,   132,    57,   134,    -1,   136,
      -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
     101,   102,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,
     131,    57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,   131,    -1,    -1,   134,    -1,
     136
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
     124,   130,   131,   134,   136,   142,   143,   145,   146,   147,
     148,   149,   151,   152,   154,   155,   158,   159,   162,   167,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   184,   185,   187,    99,   101,   102,   103,
     104,   105,   106,   107,   116,   129,   131,   116,   134,   118,
     136,   116,    11,   129,   131,    10,    11,    12,    13,    14,
      31,   131,   167,   180,    14,    10,    11,   130,   131,   169,
     186,    10,    11,    12,    14,    31,    36,   131,   131,   169,
     131,   169,   131,   169,   131,   169,   169,   176,    14,   167,
     151,   167,   169,   130,   169,   130,   169,    10,    14,    14,
     169,   169,    12,    12,   131,   169,   131,   169,   167,   131,
     167,    26,    46,   130,    26,    46,   130,   136,   168,   168,
     136,   168,   169,   167,   167,   169,    14,    14,    14,    31,
     180,    14,    10,    10,   180,   180,    10,   132,   169,   182,
     135,   169,    75,   141,    26,    29,    30,    39,    42,    43,
      46,    47,   130,    26,    29,    30,    39,    42,    43,    46,
      47,   130,    84,    85,    86,    87,    94,    95,    96,   118,
     119,    88,    89,    90,    91,    92,    93,   100,   112,   120,
     121,   108,    81,    82,   122,   113,   114,   123,   124,   109,
     110,   111,   125,   126,   127,   115,   129,   129,    14,   169,
     169,   169,   169,   169,   169,    14,   132,   186,   131,   169,
     125,   169,    14,   169,   169,   130,    14,    17,    61,    14,
     132,   138,   164,   186,   134,   118,   136,   130,   103,   104,
     105,   106,   107,   116,   116,   132,   169,   186,   130,   133,
     116,   130,   116,   130,   116,   130,    10,    11,    12,    14,
      10,    14,    10,   182,   169,    26,    29,    30,    39,    42,
      43,    46,   130,   133,   169,    26,    46,   130,   133,   169,
     167,   169,   167,    83,   133,   167,    81,    82,   131,   136,
     156,   130,    26,    46,   130,    26,    46,   130,   116,   116,
      72,   133,   136,   118,   136,   118,   136,   169,   167,   169,
     167,    29,    47,    23,   169,   169,   169,   169,   169,   169,
     169,   169,   169,    56,   160,   136,   131,    14,    65,   136,
     132,   133,   132,   133,   133,   135,   137,    10,    11,    12,
      13,    24,    25,    33,    34,    50,    51,   137,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   172,   172,   172,   172,   172,
     172,   170,   172,   172,   172,   172,   172,   172,   172,   172,
       7,     8,     9,   173,   175,   175,   175,   176,   176,   177,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   180,
     180,    14,    22,    24,    25,    33,    34,    38,    44,    45,
      52,    53,    54,   183,    17,   183,   130,   131,   130,   130,
     130,   130,   130,   130,   116,   131,   132,   169,   130,   133,
     123,   124,   133,   135,   119,   137,   130,   131,    14,   131,
      80,    14,   132,   133,   132,   133,   169,    14,   169,   169,
     169,   169,   169,   169,   169,   169,   130,   132,   169,   169,
     131,   169,     5,    14,   131,   165,   169,   116,   130,   116,
     130,   116,   130,   131,   156,   116,   116,   132,   133,   169,
     169,   169,   169,   169,   169,   169,   186,   133,   169,   169,
     186,   132,    27,    28,   144,   132,    10,   186,   176,   176,
      10,    11,    14,    68,    69,    70,   132,   166,   141,   169,
     169,   169,   169,   169,   169,   130,   186,   153,    14,   169,
      14,   169,   132,    27,   132,   169,   169,    10,   130,   130,
     130,   130,   130,   137,   167,    14,    22,   136,   161,   132,
     166,   131,   131,   163,   132,   186,   116,    10,   135,   186,
     129,   134,   136,   116,   169,   169,   169,   137,   169,    26,
      46,   137,    26,    46,   137,    26,    29,    30,    39,    42,
      43,    46,    47,   137,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
      97,   131,    14,   131,   132,   186,   169,   132,   186,   133,
     186,   169,   169,   186,   116,   116,   116,   132,   186,   132,
     164,   186,   169,   131,    14,   138,   164,   135,   119,   137,
     130,   130,   169,   130,   133,    80,    80,   165,   130,   133,
     130,   169,   131,   169,   169,   132,   166,   169,   169,   116,
     186,   130,   130,   130,   130,   130,   130,   130,   130,   186,
     130,   130,   130,   167,   143,   167,   131,   169,   167,   167,
      83,   167,    83,    83,    22,   116,    10,    73,    74,   156,
     132,   133,    24,    25,    34,   137,   169,   130,   130,   130,
     130,   130,   130,    40,    41,   137,   119,   137,   119,   137,
     167,   167,   167,   130,   130,   116,   169,    14,    14,   161,
      59,    60,    63,   137,   156,   132,   132,   166,   166,    77,
      78,    79,   137,   132,   169,   135,    14,   169,   169,   169,
      26,    30,    39,    42,    43,    46,   137,   137,   137,   137,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   170,   132,   186,   132,   164,   186,   130,   132,
     130,   132,   186,   130,   135,   135,   135,   169,   169,   169,
     132,   132,   132,   169,    80,    14,   132,   133,   186,   169,
     169,   132,     5,    14,   130,   169,   130,   133,   130,   156,
     132,   130,   130,   169,   132,   132,    27,   144,   169,   167,
      10,    10,    10,    14,   169,   116,    10,    10,   156,    10,
      11,    14,    68,    69,    70,   169,   169,   137,   169,    26,
      29,    30,    39,    42,    43,    46,    47,   137,   169,   167,
     130,   130,    27,   169,   130,   137,    61,   183,    10,    13,
      14,    15,    16,    59,   156,   156,   132,   132,    14,    14,
      14,   130,   116,   131,   135,   137,   137,   169,   169,   169,
     169,   169,   169,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   132,   132,   132,   130,   132,
     130,   130,   130,   132,   169,   131,   186,   130,   130,    80,
      80,   133,   186,   156,   130,   133,   130,   130,   143,   167,
     132,    27,   144,   167,   167,   167,   169,    22,   116,    10,
      73,    74,    26,    30,    46,   137,   137,   137,   169,   169,
     169,   169,   169,   169,   169,   169,   167,   137,   167,   130,
     150,   169,   131,   136,   157,   131,   157,    14,    22,    71,
     116,   130,    14,    22,    71,   116,   130,    10,    13,    15,
      16,   130,   130,   183,   156,   156,     7,     7,     7,   169,
     132,   186,   116,   116,   137,   137,   137,   137,   137,   137,
     130,   169,   132,   169,   169,   186,   130,   186,   167,   167,
      14,   169,   116,    10,    10,   169,   169,   169,   137,   137,
     137,   137,   137,   137,   137,   137,   169,   132,   132,   166,
      75,   141,   132,   166,    14,    14,   130,   169,    14,    14,
     130,   169,    14,    22,    71,   116,   130,    14,    22,    71,
     116,   130,   130,   130,   131,   157,   137,   137,   132,   169,
     169,   132,   130,   132,   130,    27,   144,   169,   137,   137,
     137,   130,   167,   157,   132,   137,    24,    25,    34,   137,
     169,   157,   132,   130,   130,   130,   130,   130,   130,    14,
      14,   130,   169,    14,    14,   130,   169,   132,   166,   137,
     137,   137,   130,   167,   150,   157,   169,   169,   137,   169,
      26,    29,    30,    39,    42,    43,    46,    47,   137,   157,
     130,   130,   130,   130,   130,   130,   157,   132,   132,    26,
      30,    46,   137,   137,   137,   169,   169,   169,   169,   169,
     169,   169,   169,   157,   167,   169,   169,   169,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137
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
     142,   143,   143,   143,   143,   143,   143,   143,   143,   144,
     144,   144,   144,   144,   144,   145,   145,   146,   146,   146,
     146,   147,   147,   148,   148,   148,   149,   149,   149,   150,
     151,   151,   151,   151,   151,   151,   152,   152,   153,   153,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   155,   155,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   158,   159,
     160,   160,   160,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   162,   163,   163,
     163,   163,   164,   164,   164,   164,   165,   165,   165,   165,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   168,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   172,   172,   173,
     173,   173,   174,   174,   175,   175,   175,   176,   176,   177,
     177,   177,   178,   178,   178,   179,   179,   179,   179,   179,
     179,   179,   180,   180,   180,   180,   180,   181,   181,   182,
     182,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   184,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   186,   186,   186,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187
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
       1,     1,     1,     1,     4,     4,     3,     4,     6,     4,
       3,     6,     6,     5,     3,     3,     6,     5,     4,     4,
       6,     6,     5,     3,     6,     5,     3,     2,     3,     3,
       2,     3,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     4,     6,     6,     6,     1,     4,     4,     5,     5,
       1,     1,     1,     1,     2,     3,     4,     5,     2,     3,
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
#line 457 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3081 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 467 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3087 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 468 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3093 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 472 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3099 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 474 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3105 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 476 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3111 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 478 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3119 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 482 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3125 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 484 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3131 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 486 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3137 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 488 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3143 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 490 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3151 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 494 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3159 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 498 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3165 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 500 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3173 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 504 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3181 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 508 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3187 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 510 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3193 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 512 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3199 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 514 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3205 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 516 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3211 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 518 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3219 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 522 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3227 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 526 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3233 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 528 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3239 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 530 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3245 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 532 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3251 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 534 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3257 "raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 536 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3263 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 538 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3269 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 540 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3275 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 542 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3281 "raku.tab.c"
    break;

  case 34: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 544 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3293 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 552 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3299 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 554 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3305 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 556 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3311 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 558 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3317 "raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 560 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3323 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 562 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3329 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 564 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3336 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 567 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3343 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 570 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3349 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 572 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3356 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 575 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3363 "raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 578 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3369 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN expr ';'  */
#line 580 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3375 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN ';'  */
#line 582 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3381 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 584 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3388 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 587 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3395 "raku.tab.c"
    break;

  case 51: /* stmt: KW_EXIT expr ';'  */
#line 590 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3401 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT ';'  */
#line 592 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3408 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 595 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3415 "raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 598 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3422 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 601 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3428 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 603 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3437 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 608 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3445 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 612 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3453 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 616 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3462 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 621 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3470 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall ';'  */
#line 624 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3476 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3482 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3488 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 630 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3494 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 632 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3500 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 634 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3507 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 637 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3513 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 639 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3519 "raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 641 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3525 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 643 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3532 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 646 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3539 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 649 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3546 "raku.tab.c"
    break;

  case 73: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 652 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3553 "raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 655 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3560 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_IF expr ';'  */
#line 658 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3566 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_UNLESS expr ';'  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3572 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WHILE expr ';'  */
#line 662 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3578 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_UNTIL expr ';'  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3584 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_FOR expr ';'  */
#line 666 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3591 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WITH expr ';'  */
#line 669 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3597 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITHOUT expr ';'  */
#line 671 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3603 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_GIVEN expr ';'  */
#line 673 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3609 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 675 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3616 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 678 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3623 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 681 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3631 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 685 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3638 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 688 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3644 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 690 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3650 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 692 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3656 "raku.tab.c"
    break;

  case 90: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 694 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3663 "raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 697 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 700 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3677 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 703 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3684 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 706 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3691 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 709 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3698 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 712 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3705 "raku.tab.c"
    break;

  case 97: /* stmt: expr ';'  */
#line 714 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3711 "raku.tab.c"
    break;

  case 98: /* stmt: ';'  */
#line 715 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3717 "raku.tab.c"
    break;

  case 99: /* stmt: if_stmt  */
#line 716 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3723 "raku.tab.c"
    break;

  case 100: /* stmt: while_stmt  */
#line 717 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3729 "raku.tab.c"
    break;

  case 101: /* stmt: for_stmt  */
#line 718 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3735 "raku.tab.c"
    break;

  case 102: /* stmt: given_stmt  */
#line 719 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3741 "raku.tab.c"
    break;

  case 103: /* stmt: KW_TRY block  */
#line 721 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3747 "raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block KW_CATCH block  */
#line 723 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3753 "raku.tab.c"
    break;

  case 105: /* stmt: KW_CATCH block  */
#line 725 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3759 "raku.tab.c"
    break;

  case 106: /* stmt: block  */
#line 727 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3765 "raku.tab.c"
    break;

  case 107: /* stmt: unless_stmt  */
#line 728 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3771 "raku.tab.c"
    break;

  case 108: /* stmt: until_stmt  */
#line 729 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3777 "raku.tab.c"
    break;

  case 109: /* stmt: repeat_stmt  */
#line 730 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3783 "raku.tab.c"
    break;

  case 110: /* stmt: loop_stmt  */
#line 731 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3789 "raku.tab.c"
    break;

  case 111: /* stmt: KW_LAST ';'  */
#line 732 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3795 "raku.tab.c"
    break;

  case 112: /* stmt: KW_NEXT ';'  */
#line 733 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3801 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_IF expr ';'  */
#line 735 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3807 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 737 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3813 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 739 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3819 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 741 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3825 "raku.tab.c"
    break;

  case 117: /* stmt: sub_decl  */
#line 742 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3831 "raku.tab.c"
    break;

  case 118: /* stmt: class_decl  */
#line 743 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3837 "raku.tab.c"
    break;

  case 119: /* stmt: role_decl  */
#line 744 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3843 "raku.tab.c"
    break;

  case 120: /* stmt: grammar_decl  */
#line 745 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3849 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block  */
#line 749 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3855 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 751 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3861 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 753 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3867 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block  */
#line 755 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3873 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 757 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3879 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 759 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3885 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block elsif_tail  */
#line 761 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3891 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 763 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3897 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF expr block  */
#line 767 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3903 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 769 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3909 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 771 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3915 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 773 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3921 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 775 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3927 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 777 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3933 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 781 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3939 "raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE expr block  */
#line 783 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3945 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 787 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3951 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 789 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3957 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block  */
#line 791 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3963 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 793 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3969 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 797 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3975 "raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL expr block  */
#line 799 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3981 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block  */
#line 803 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3987 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 805 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3993 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 807 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3999 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP block  */
#line 811 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4005 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 813 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4011 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 815 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4017 "raku.tab.c"
    break;

  case 149: /* loop_incr: expr  */
#line 818 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4023 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 822 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4033 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 828 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4043 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 834 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4052 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 839 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4062 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 845 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4071 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr block  */
#line 850 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4078 "raku.tab.c"
    break;

  case 156: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 855 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4090 "raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 863 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4103 "raku.tab.c"
    break;

  case 158: /* when_list: %empty  */
#line 873 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4109 "raku.tab.c"
    break;

  case 159: /* when_list: when_list KW_WHEN expr block  */
#line 875 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4117 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 881 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4129 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 889 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4139 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT sub_body  */
#line 895 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4149 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 901 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4161 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 909 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4171 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 915 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4181 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 921 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4194 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 930 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4205 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 937 "raku.y"
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

  case 169: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 946 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4231 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 955 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4242 "raku.tab.c"
    break;

  case 171: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 964 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4253 "raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 971 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4262 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list '}'  */
#line 977 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4268 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr '}'  */
#line 979 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4275 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 982 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4282 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 985 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4288 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 987 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4294 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 989 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4300 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 991 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4306 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 993 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4312 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 995 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4318 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 997 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4324 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 999 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4331 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1002 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4337 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1004 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4343 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1006 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4349 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1008 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4356 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1011 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4363 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1014 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4371 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list '}'  */
#line 1019 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4377 "raku.tab.c"
    break;

  case 191: /* method_body: '{' YADA '}'  */
#line 1020 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4383 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr '}'  */
#line 1022 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4390 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1025 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4397 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1028 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4403 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1030 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4409 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1032 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4415 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1034 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4421 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4427 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1038 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4433 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1040 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4439 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1042 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4446 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1045 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4452 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1047 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4458 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1049 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4464 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1051 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4471 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1054 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4478 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1057 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4486 "raku.tab.c"
    break;

  case 208: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1063 "raku.y"
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
#line 4504 "raku.tab.c"
    break;

  case 209: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1079 "raku.y"
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
#line 4521 "raku.tab.c"
    break;

  case 210: /* is_clauses: %empty  */
#line 1093 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4527 "raku.tab.c"
    break;

  case 211: /* is_clauses: is_clauses IDENT IDENT  */
#line 1095 "raku.y"
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
#line 4543 "raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1107 "raku.y"
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
#line 4559 "raku.tab.c"
    break;

  case 213: /* class_body_list: %empty  */
#line 1120 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4565 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1122 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4572 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1125 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4579 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1128 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4586 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1131 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4593 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1134 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4600 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1137 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4607 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1140 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4614 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1143 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4621 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1146 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4628 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1149 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4635 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1152 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4642 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1155 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4649 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1158 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4660 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1165 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4671 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1172 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4682 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1179 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4693 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1186 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4701 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1190 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4709 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1194 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4717 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1198 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4725 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1202 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4736 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1209 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4747 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1216 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4758 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1223 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4769 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1230 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4783 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1240 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4795 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1248 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4807 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1256 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4820 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1265 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4831 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1272 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4842 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1279 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4856 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1289 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4868 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1297 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4880 "raku.tab.c"
    break;

  case 247: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1307 "raku.y"
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
#line 4897 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: %empty  */
#line 1321 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4903 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1323 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4912 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1328 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4921 "raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1333 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4930 "raku.tab.c"
    break;

  case 252: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1340 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4938 "raku.tab.c"
    break;

  case 253: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1344 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4946 "raku.tab.c"
    break;

  case 254: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1348 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4954 "raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1352 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4962 "raku.tab.c"
    break;

  case 256: /* pair_list: IDENT OP_FATARROW expr  */
#line 1358 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4968 "raku.tab.c"
    break;

  case 257: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1360 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4974 "raku.tab.c"
    break;

  case 258: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1362 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4980 "raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1364 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4986 "raku.tab.c"
    break;

  case 260: /* param_list: VAR_SCALAR  */
#line 1367 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4992 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1368 "raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 4998 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1369 "raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5004 "raku.tab.c"
    break;

  case 263: /* param_list: VAR_ARRAY  */
#line 1370 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5010 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' VAR_ARRAY  */
#line 1371 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5016 "raku.tab.c"
    break;

  case 265: /* param_list: IDENT VAR_SCALAR  */
#line 1372 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5022 "raku.tab.c"
    break;

  case 266: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1373 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5028 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1374 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5034 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR  */
#line 1375 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5040 "raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1376 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5046 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1377 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5052 "raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1378 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5058 "raku.tab.c"
    break;

  case 272: /* param_list: VAR_SCALAR '=' expr  */
#line 1379 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5064 "raku.tab.c"
    break;

  case 273: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1380 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5070 "raku.tab.c"
    break;

  case 274: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1381 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5076 "raku.tab.c"
    break;

  case 275: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1382 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5082 "raku.tab.c"
    break;

  case 276: /* param_list: SLURPY_POS  */
#line 1383 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5088 "raku.tab.c"
    break;

  case 277: /* param_list: param_list ',' SLURPY_POS  */
#line 1384 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5094 "raku.tab.c"
    break;

  case 278: /* param_list: SLURPY_LOL  */
#line 1385 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5100 "raku.tab.c"
    break;

  case 279: /* param_list: param_list ',' SLURPY_LOL  */
#line 1386 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5106 "raku.tab.c"
    break;

  case 280: /* param_list: SLURPY_NAMED  */
#line 1387 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5112 "raku.tab.c"
    break;

  case 281: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1388 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5118 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list '}'  */
#line 1391 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5124 "raku.tab.c"
    break;

  case 283: /* block: '{' YADA '}'  */
#line 1392 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5130 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr '}'  */
#line 1394 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5136 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5142 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1398 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5148 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1400 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5154 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1402 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5160 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1404 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5167 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1407 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5173 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1409 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5179 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1411 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5185 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1413 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5191 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1415 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5198 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1418 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5205 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1421 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5213 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1425 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5220 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1428 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5227 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1431 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5234 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1434 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5240 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1436 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5248 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1440 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5255 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1443 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5262 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1446 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5269 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1449 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5276 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1452 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5283 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1455 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5289 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1457 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5295 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1459 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5301 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_LAST '}'  */
#line 1461 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5307 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1463 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5313 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1465 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5319 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1467 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5325 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1469 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5331 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1471 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5337 "raku.tab.c"
    break;

  case 316: /* closure: '{' expr '}'  */
#line 1474 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5343 "raku.tab.c"
    break;

  case 317: /* expr: VAR_SCALAR '=' expr  */
#line 1477 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5349 "raku.tab.c"
    break;

  case 318: /* expr: VAR_ARRAY '=' expr  */
#line 1478 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5355 "raku.tab.c"
    break;

  case 319: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1479 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5361 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1480 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5367 "raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1481 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5373 "raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1482 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5379 "raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1483 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5385 "raku.tab.c"
    break;

  case 324: /* expr: KW_GATHER block  */
#line 1484 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5395 "raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER for_stmt  */
#line 1489 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5405 "raku.tab.c"
    break;

  case 326: /* expr: tern_expr  */
#line 1494 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5411 "raku.tab.c"
    break;

  case 327: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1498 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5417 "raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr  */
#line 1499 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5423 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1502 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5429 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1503 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5435 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1504 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5441 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1505 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5447 "raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1506 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5453 "raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1507 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5459 "raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1508 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5465 "raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1509 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5471 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1510 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5477 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1511 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5483 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1512 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5489 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1513 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5495 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1514 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5501 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1515 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5507 "raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1517 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5517 "raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5527 "raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1529 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5537 "raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr  */
#line 1534 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5543 "raku.tab.c"
    break;

  case 347: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1537 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5549 "raku.tab.c"
    break;

  case 348: /* divis_expr: jct_expr  */
#line 1538 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5555 "raku.tab.c"
    break;

  case 349: /* jct_expr: jct_expr '|' range_expr  */
#line 1541 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5561 "raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '&' range_expr  */
#line 1542 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5567 "raku.tab.c"
    break;

  case 351: /* jct_expr: dor_expr  */
#line 1543 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5573 "raku.tab.c"
    break;

  case 352: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1547 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5579 "raku.tab.c"
    break;

  case 353: /* dor_expr: range_expr  */
#line 1548 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5585 "raku.tab.c"
    break;

  case 354: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1551 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5591 "raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1552 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5597 "raku.tab.c"
    break;

  case 356: /* range_expr: add_expr  */
#line 1553 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5603 "raku.tab.c"
    break;

  case 357: /* add_expr: add_expr '~' repl_expr  */
#line 1556 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5609 "raku.tab.c"
    break;

  case 358: /* add_expr: repl_expr  */
#line 1557 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5615 "raku.tab.c"
    break;

  case 359: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1560 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5621 "raku.tab.c"
    break;

  case 360: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1561 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5627 "raku.tab.c"
    break;

  case 361: /* repl_expr: addsub_expr  */
#line 1562 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5633 "raku.tab.c"
    break;

  case 362: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1565 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5639 "raku.tab.c"
    break;

  case 363: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1566 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5645 "raku.tab.c"
    break;

  case 364: /* addsub_expr: mul_expr  */
#line 1567 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5651 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr '*' unary_expr  */
#line 1570 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5657 "raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '/' unary_expr  */
#line 1571 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5663 "raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '%' unary_expr  */
#line 1572 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5669 "raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1573 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5675 "raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1575 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5681 "raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1577 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5687 "raku.tab.c"
    break;

  case 371: /* mul_expr: unary_expr  */
#line 1578 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5693 "raku.tab.c"
    break;

  case 372: /* unary_expr: '-' unary_expr  */
#line 1581 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5699 "raku.tab.c"
    break;

  case 373: /* unary_expr: '!' unary_expr  */
#line 1582 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5705 "raku.tab.c"
    break;

  case 374: /* unary_expr: CARET unary_expr  */
#line 1583 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5711 "raku.tab.c"
    break;

  case 375: /* unary_expr: OP_REDUCE unary_expr  */
#line 1585 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5720 "raku.tab.c"
    break;

  case 376: /* unary_expr: pow_expr  */
#line 1589 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5726 "raku.tab.c"
    break;

  case 377: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1592 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5732 "raku.tab.c"
    break;

  case 378: /* pow_expr: postfix_expr  */
#line 1593 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5738 "raku.tab.c"
    break;

  case 379: /* scalar_list: VAR_SCALAR  */
#line 1596 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5744 "raku.tab.c"
    break;

  case 380: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1597 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5750 "raku.tab.c"
    break;

  case 381: /* meth_name: IDENT  */
#line 1600 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5756 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_SORT  */
#line 1601 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5762 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_MAP  */
#line 1602 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5768 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_GREP  */
#line 1603 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5774 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_SAY  */
#line 1604 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5780 "raku.tab.c"
    break;

  case 386: /* meth_name: KW_PRINT  */
#line 1605 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5786 "raku.tab.c"
    break;

  case 387: /* meth_name: KW_TAKE  */
#line 1606 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5792 "raku.tab.c"
    break;

  case 388: /* meth_name: KW_RETURN  */
#line 1607 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5798 "raku.tab.c"
    break;

  case 389: /* meth_name: KW_EXISTS  */
#line 1608 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5804 "raku.tab.c"
    break;

  case 390: /* meth_name: KW_DELETE  */
#line 1609 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5810 "raku.tab.c"
    break;

  case 391: /* meth_name: KW_JOIN  */
#line 1610 "raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 5816 "raku.tab.c"
    break;

  case 392: /* meth_name: TESTOP  */
#line 1611 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5822 "raku.tab.c"
    break;

  case 393: /* postfix_expr: call_expr  */
#line 1613 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5828 "raku.tab.c"
    break;

  case 394: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1616 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5838 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '(' arg_list ')'  */
#line 1622 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5847 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '(' ')'  */
#line 1626 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5853 "raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1628 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5859 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1630 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5865 "raku.tab.c"
    break;

  case 399: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1632 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5874 "raku.tab.c"
    break;

  case 400: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1637 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5880 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1639 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5890 "raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1645 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5900 "raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1651 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5908 "raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' KW_NEW  */
#line 1655 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5916 "raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' IDENT  */
#line 1659 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5925 "raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1664 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5936 "raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1671 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5945 "raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' CARET IDENT  */
#line 1676 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5955 "raku.tab.c"
    break;

  case 409: /* call_expr: atom '.' CARET IDENT  */
#line 1682 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5965 "raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1688 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5976 "raku.tab.c"
    break;

  case 411: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1695 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5987 "raku.tab.c"
    break;

  case 412: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1702 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5996 "raku.tab.c"
    break;

  case 413: /* call_expr: atom '.' meth_name  */
#line 1707 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6005 "raku.tab.c"
    break;

  case 414: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1712 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6016 "raku.tab.c"
    break;

  case 415: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1719 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6025 "raku.tab.c"
    break;

  case 416: /* call_expr: call_expr '.' meth_name  */
#line 1724 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6034 "raku.tab.c"
    break;

  case 417: /* call_expr: KW_DIE expr  */
#line 1729 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6040 "raku.tab.c"
    break;

  case 418: /* call_expr: KW_MAP closure expr  */
#line 1731 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6046 "raku.tab.c"
    break;

  case 419: /* call_expr: KW_GREP closure expr  */
#line 1733 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6052 "raku.tab.c"
    break;

  case 420: /* call_expr: KW_SORT expr  */
#line 1735 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6058 "raku.tab.c"
    break;

  case 421: /* call_expr: KW_SORT closure expr  */
#line 1737 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6064 "raku.tab.c"
    break;

  case 422: /* call_expr: atom  */
#line 1738 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6070 "raku.tab.c"
    break;

  case 423: /* arg_list: expr  */
#line 1741 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6076 "raku.tab.c"
    break;

  case 424: /* arg_list: arg_list ',' expr  */
#line 1742 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6082 "raku.tab.c"
    break;

  case 425: /* arg_list: arg_list ','  */
#line 1743 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6088 "raku.tab.c"
    break;

  case 426: /* atom: LIT_INT  */
#line 1746 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6094 "raku.tab.c"
    break;

  case 427: /* atom: LIT_FLOAT  */
#line 1747 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6100 "raku.tab.c"
    break;

  case 428: /* atom: LIT_STR  */
#line 1748 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6106 "raku.tab.c"
    break;

  case 429: /* atom: WORDLIST  */
#line 1750 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6119 "raku.tab.c"
    break;

  case 430: /* atom: LIT_INTERP_STR  */
#line 1758 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6125 "raku.tab.c"
    break;

  case 431: /* atom: VAR_SCALAR  */
#line 1759 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6131 "raku.tab.c"
    break;

  case 432: /* atom: OP_INC VAR_SCALAR  */
#line 1760 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6137 "raku.tab.c"
    break;

  case 433: /* atom: OP_DEC VAR_SCALAR  */
#line 1761 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6143 "raku.tab.c"
    break;

  case 434: /* atom: VAR_SCALAR OP_INC  */
#line 1762 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6149 "raku.tab.c"
    break;

  case 435: /* atom: VAR_SCALAR OP_DEC  */
#line 1763 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6155 "raku.tab.c"
    break;

  case 436: /* atom: VAR_ARRAY  */
#line 1764 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6161 "raku.tab.c"
    break;

  case 437: /* atom: VAR_HASH  */
#line 1765 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6167 "raku.tab.c"
    break;

  case 438: /* atom: VAR_CAPTURE  */
#line 1767 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6175 "raku.tab.c"
    break;

  case 439: /* atom: VAR_FH  */
#line 1771 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6183 "raku.tab.c"
    break;

  case 440: /* atom: VAR_NAMED_CAPTURE  */
#line 1775 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6190 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1778 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6196 "raku.tab.c"
    break;

  case 442: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1780 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6202 "raku.tab.c"
    break;

  case 443: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1782 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6208 "raku.tab.c"
    break;

  case 444: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1784 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6214 "raku.tab.c"
    break;

  case 445: /* atom: ARR_ALL_SLICE  */
#line 1786 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6220 "raku.tab.c"
    break;

  case 446: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1788 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6226 "raku.tab.c"
    break;

  case 447: /* atom: VAR_HASH '{' expr '}'  */
#line 1790 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6232 "raku.tab.c"
    break;

  case 448: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1792 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6238 "raku.tab.c"
    break;

  case 449: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1794 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6244 "raku.tab.c"
    break;

  case 450: /* atom: IDENT  */
#line 1795 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6250 "raku.tab.c"
    break;

  case 451: /* atom: VAR_TWIGIL  */
#line 1797 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6258 "raku.tab.c"
    break;

  case 452: /* atom: VAR_ARRAY_TWIGIL  */
#line 1801 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6266 "raku.tab.c"
    break;

  case 453: /* atom: VAR_HASH_TWIGIL  */
#line 1805 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6274 "raku.tab.c"
    break;

  case 454: /* atom: '[' ']'  */
#line 1808 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6280 "raku.tab.c"
    break;

  case 455: /* atom: '[' expr ']'  */
#line 1810 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6286 "raku.tab.c"
    break;

  case 456: /* atom: '[' expr ',' ']'  */
#line 1812 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6292 "raku.tab.c"
    break;

  case 457: /* atom: '[' expr ',' arg_list ']'  */
#line 1814 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6299 "raku.tab.c"
    break;

  case 458: /* atom: '(' ')'  */
#line 1816 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6305 "raku.tab.c"
    break;

  case 459: /* atom: '(' expr ')'  */
#line 1817 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6311 "raku.tab.c"
    break;

  case 460: /* atom: '(' expr ',' ')'  */
#line 1819 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6317 "raku.tab.c"
    break;

  case 461: /* atom: '(' expr ',' arg_list ')'  */
#line 1821 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6324 "raku.tab.c"
    break;

  case 462: /* atom: block  */
#line 1823 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6330 "raku.tab.c"
    break;

  case 463: /* atom: KW_SUB block  */
#line 1824 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6336 "raku.tab.c"
    break;


#line 6340 "raku.tab.c"

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

#line 1826 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
