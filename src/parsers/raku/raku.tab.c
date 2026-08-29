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
#line 11 "/home/claude06/SCRIP/src/parsers/raku/raku.y"

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

#line 439 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"

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
#define YYLAST   4728

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  464
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1172

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
     561,   563,   566,   569,   571,   574,   577,   579,   583,   585,
     587,   590,   593,   595,   598,   601,   604,   606,   611,   615,
     619,   624,   628,   629,   631,   633,   635,   637,   640,   642,
     644,   646,   649,   652,   655,   658,   661,   663,   665,   667,
     669,   672,   674,   676,   678,   681,   684,   688,   691,   693,
     695,   697,   700,   703,   706,   709,   712,   715,   718,   719,
     720,   721,   722,   723,   724,   726,   728,   730,   732,   733,
     734,   735,   736,   737,   738,   740,   742,   744,   746,   747,
     748,   749,   752,   754,   756,   758,   760,   762,   764,   766,
     770,   772,   774,   776,   778,   780,   784,   786,   790,   792,
     794,   796,   800,   802,   806,   808,   810,   814,   816,   818,
     822,   825,   831,   837,   842,   848,   853,   858,   866,   877,
     878,   884,   892,   898,   904,   912,   918,   924,   933,   940,
     949,   958,   967,   974,   981,   982,   985,   988,   990,   992,
     994,   996,   998,  1000,  1002,  1005,  1007,  1009,  1011,  1014,
    1017,  1023,  1024,  1025,  1028,  1031,  1033,  1035,  1037,  1039,
    1041,  1043,  1045,  1048,  1050,  1052,  1054,  1057,  1060,  1066,
    1082,  1097,  1098,  1110,  1124,  1125,  1128,  1131,  1134,  1137,
    1140,  1143,  1146,  1149,  1152,  1155,  1158,  1161,  1168,  1175,
    1182,  1189,  1193,  1197,  1201,  1205,  1212,  1219,  1226,  1233,
    1243,  1251,  1259,  1268,  1275,  1282,  1292,  1300,  1310,  1325,
    1326,  1331,  1336,  1343,  1347,  1351,  1355,  1361,  1363,  1365,
    1367,  1371,  1372,  1373,  1374,  1375,  1376,  1377,  1378,  1379,
    1380,  1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,
    1390,  1391,  1392,  1395,  1396,  1397,  1399,  1401,  1403,  1405,
    1407,  1410,  1412,  1414,  1416,  1418,  1421,  1424,  1428,  1431,
    1434,  1437,  1439,  1443,  1446,  1449,  1452,  1455,  1458,  1460,
    1462,  1464,  1466,  1468,  1470,  1472,  1474,  1478,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1488,  1493,  1498,  1501,  1503,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,  1526,  1532,  1538,  1541,  1542,
    1545,  1546,  1547,  1550,  1552,  1555,  1556,  1557,  1560,  1561,
    1564,  1565,  1566,  1569,  1570,  1571,  1574,  1575,  1576,  1577,
    1578,  1580,  1582,  1585,  1586,  1587,  1588,  1593,  1596,  1597,
    1600,  1601,  1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,
    1612,  1613,  1614,  1615,  1617,  1619,  1625,  1630,  1631,  1633,
    1635,  1640,  1642,  1648,  1654,  1658,  1662,  1667,  1674,  1679,
    1685,  1691,  1698,  1705,  1710,  1715,  1722,  1727,  1732,  1734,
    1736,  1738,  1740,  1742,  1745,  1746,  1747,  1750,  1751,  1752,
    1753,  1762,  1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,
    1774,  1778,  1781,  1783,  1785,  1787,  1789,  1791,  1793,  1795,
    1797,  1799,  1800,  1804,  1808,  1812,  1813,  1815,  1817,  1820,
    1821,  1822,  1824,  1827,  1828
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

#define YYPACT_NINF (-847)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-464)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -847,    12,  2057,  -847,  -847,  -847,  -847,  -847,   906,   -68,
     -23,   -77,    20,  -847,  -847,  4543,  -847,  -847,  -847,   130,
    2132,    40,  3568,  3643,  3718,  3793,  3868,     1,   -19,  3868,
    2207,  2282,   444,   175,  3868,  3868,    46,   104,  3943,  4018,
     -91,   242,     0,    29,    83,    83,  4093,   -91,   -91,  3868,
     186,   216,    11,  4543,  -847,  -847,   221,   249,   298,  4543,
    4543,  -847,  2357,  2433,   168,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  -847,   718,  -847,  -847,  -847,  4599,
     852,  -847,   614,   872,   150,   332,  -847,   156,   311,     8,
     454,  -847,  -847,   213,   247,   260,   484,  -847,  -847,  3868,
    3868,  3868,  3868,  3868,  3868,   497,  2508,  4168,  2584,   501,
    3868,  3868,   314,   179,   787,   119,   336,   126,  -847,   518,
     -91,  2659,  -847,  -847,   391,   707,   -67,  -847,  2735,  -847,
     166,    67,   241,   266,   394,   521,   473,   581,  2659,   648,
    2659,    57,  2659,   -91,  2659,   -91,   -42,   253,   284,  -847,
    -847,  -847,   197,  -847,    31,  -847,    33,   471,   490,   475,
     481,   487,   145,   157,  2659,   -91,  2659,   -91,   278,  2811,
    -847,  3868,  3868,  -847,  3868,  3868,  -847,  3868,  3868,  3868,
    2886,  3868,  -847,   578,  -847,  -847,  -847,   502,   527,    93,
    -847,   535,  -847,  -847,  -847,  -847,  1001,  -847,   606,   637,
    -847,   623,   555,  1427,  3868,  3868,  3868,  3868,  3868,  3868,
    3868,  3868,  -847,  3868,  3868,  3868,  3868,  3868,  3868,  3868,
    3868,  -847,  4543,  4543,  4543,  4543,  4543,  4543,  4543,  4543,
    4543,  4543,  4543,  4543,  4543,  4543,  4543,   825,  4543,  4543,
    4543,  4543,  4543,  4543,  4543,  4543,  4543,  4543,  4543,  4543,
    4543,  4543,  4543,  4543,  4543,  4543,  1214,  1180,   658,   566,
     574,   587,   599,   621,   686,   -44,  -847,   723,  2659,   341,
     734,   695,   626,   685,   697,  -847,   705,   828,   720,    24,
    -847,   860,   736,   738,  2584,   865,  3868,  -847,  3868,  3868,
    3868,  3868,  3868,  3868,  3868,   757,   606,   740,  -847,  3868,
    3868,  -847,  4243,  -847,  4318,  -847,   268,   270,   283,   326,
     776,   804,  -847,   744,   751,  3868,  3868,  3868,  3868,  3868,
    3868,  3868,  -847,  3868,   753,  3868,  3868,  -847,  3868,   764,
     873,   770,  -847,   912,  3868,  -847,  4543,  4543,    54,  -847,
    -847,  -847,  3868,  3868,  3868,  -847,  3868,  3868,  -847,  3868,
    3868,   794,  3868,  -847,   913,  3868,   923,  3868,   773,   919,
     775,  -847,  3868,  3868,   959,   876,   886,   891,   908,   921,
     834,  -847,  -847,  -847,   -91,    16,  -847,    59,   854,   866,
    -847,  -847,  2961,   877,   973,  3037,  -847,  -847,  1235,   243,
     244,   920,  3568,  3643,  3868,  1772,    99,   219,  -847,   392,
     925,   927,   930,   932,   955,   970,   971,   981,   982,   983,
     984,   988,   989,   990,   999,  1000,   929,   929,   929,   929,
     929,   929,   993,   929,   929,   929,   929,   929,   929,   929,
     929,  -847,  -847,  -847,   150,  -847,  -847,  -847,   972,   972,
     311,     8,     8,   454,   454,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  1004,  1122,  1006,  -847,  3112,  -847,
    -847,  -847,  -847,  -847,  -847,  3868,  3188,  -847,   777,  -847,
    3868,  3868,  3868,  3868,  1023,  1027,  1028,  -847,  3264,  -847,
    1014,  3868,  1016,  -847,    -5,  -847,  1291,   702,  1030,  1017,
    -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  1021,  -847,
    1025,  2659,   455,  1073,   102,  3340,   537,  1032,  3868,  -847,
    4393,  -847,  3868,  -847,    78,  -847,  3868,  3868,  1040,  2961,
    1046,  1047,  1048,  1050,  1052,  1053,  1054,   597,  2961,  1055,
    1058,   620,   -91,   -24,  4468,  -847,   -91,   -91,   -40,   -69,
     -48,    -6,  -847,     7,  -847,  -847,  -847,  1043,   780,  1562,
     647,  1059,  1060,  1061,  1062,  1065,  1066,  -847,  1024,    44,
    1079,  1044,  1081,  1064,   -91,   -91,   -91,  1076,  1080,  1087,
    3868,  -847,  -847,  -847,  -847,  -847,  -847,  1197,  1201,  -847,
      55,  1043,   782,   263,   563,   233,  -847,   784,  3868,  -847,
    -847,   710,  1202,  2584,  3868,  3868,   421,    95,   299,  -847,
     415,  3868,  3868,  -847,  3868,  3868,  -847,  3868,  3868,  3868,
    3868,  3868,  3868,  3868,  3868,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  4543,  3416,  -847,  1155,  1089,   800,  1090,  -847,
     802,  2961,   652,  1082,  1088,   714,  3868,  3868,  3868,  -847,
     808,  -847,   810,   812,  -847,  3868,  1146,  1215,   818,  -847,
    -847,  -847,  -847,  -847,   841,  -847,  3868,  3868,  3868,   844,
    -847,   106,  -847,  1100,  2659,   653,  1101,  1043,   846,  1105,
    1107,  3868,   855,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    -847,   857,  -847,  -847,  -847,   964,  -847,  -847,  2659,   -91,
    -847,  -847,  1230,  -847,  1231,  1232,  1229,  3868,  1128,  1236,
    1239,  -847,  1043,   615,  3568,  3643,  1847,  -847,   439,  -847,
    -847,  -847,  -847,  -847,  -847,  -847,  3868,   -91,  -847,  -847,
    -847,  1115,  1120,  1224,  -847,  -847,  -847,  -847,  3868,  1123,
    -847,  -847,    97,   914,   727,  1195,  -847,  -847,  1043,  1043,
     863,   867,  1241,  1247,  1248,  -847,  -847,  1133,  -847,   248,
     717,  1132,   109,  3868,  3868,  3868,  3868,  3868,  3868,  -847,
    -847,  -847,  -847,   199,   237,   277,   329,   347,   363,   365,
     366,   369,   377,   380,   382,  -847,  -847,   869,  -847,   882,
     907,  -847,  1135,  -847,  -847,   911,  -847,  -847,  -847,  -847,
    1140,  1141,  1143,  -847,  -847,  -847,  1142,  3868,  1144,  -847,
    2961,   654,  -847,  -847,  1147,  1196,  1198,  -847,   915,  -847,
    3868,  -847,  -847,  1043,  -847,  -847,   687,  1150,  1151,   -24,
    -847,   917,  1026,   -91,   -91,   -91,  -847,  -847,  3868,  -847,
    -847,  -847,    18,  -847,   232,  -847,  -847,  -847,   462,   139,
    -847,   429,  3868,  3868,  3868,  3868,  3868,  3868,  3868,  3868,
    -847,   -91,  1148,  -847,  -847,   -91,  1152,  3868,  -847,   585,
     595,    64,    68,   433,  1153,  1154,  1214,  -847,  -847,  1043,
    1043,  1281,  1283,  1285,  -847,  3868,  3492,  1183,  1184,  -847,
     393,   396,   401,   402,   404,   407,  -847,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    -847,  -847,  1182,  -847,  -847,  -847,  -847,  -847,  3868,   937,
    -847,  -847,  3868,  3868,  2961,   691,  -847,  -847,  3868,  -847,
    -847,  -847,  -847,   -91,   -91,  -847,  -847,  -847,  -847,  -847,
    1299,  3868,  1199,  1304,  1306,  3868,  3868,  3868,  -847,  -847,
    -847,   416,   419,   425,   431,   445,   456,   460,   470,  -847,
    -847,  -847,  3868,  1185,  -847,   269,  1243,  -847,   340,  -847,
    1305,  1307,  1190,  3868,  -847,  1310,  1311,  1200,  3868,  -847,
     281,   287,  1203,  1216,  -847,  -847,   618,  -847,  -847,  -847,
    -847,  -847,   474,  1189,   940,  3868,  3868,  -847,  -847,  -847,
    -847,  -847,  -847,  -847,  1217,  1220,  -847,  -847,   942,  -847,
     696,  1049,  -847,  -847,  -847,  3868,  -847,  -847,   505,   506,
     509,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  1222,
     -91,  1191,   946,  1194,  1697,  1191,   951,  1223,  1225,  -847,
    1226,  1252,  1253,  -847,  1254,  1314,  1318,  1255,  3868,  -847,
    1333,  1340,  1256,  3868,  -847,  -847,  -847,   358,  -847,  -847,
    -847,  1228,   520,   523,  -847,  -847,  1257,  -847,   -91,  -847,
    -847,  -847,  -847,  -847,  3868,  -847,  -847,  1191,  -847,  3568,
    3643,  1922,  -847,   598,  -847,  1191,  -847,  -847,  -847,  -847,
    -847,  -847,  1258,  1259,  -847,  1260,  1261,  1264,  -847,  1265,
    1191,   960,  -847,  -847,  -847,  -847,  -847,  1227,  -847,   622,
     300,  -847,   430,  3868,  3868,  3868,  3868,  3868,  3868,  3868,
    3868,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    1191,   -91,  3868,  3868,  3868,  -847,  -847,  -847,   529,   532,
     542,   543,   551,   556,   565,   577,  -847,  -847,   582,   583,
     588,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,  -847,
    -847,  -847
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   427,   428,   429,   431,   432,   437,
     438,   452,   451,   453,   454,     0,   439,   440,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   446,   430,     0,     0,     0,     0,
       0,    99,     0,     0,     3,     4,   100,   101,   108,   109,
     110,   111,   102,   103,   118,     0,   119,   120,   121,   107,
       0,   327,   329,   347,   349,   352,   354,   357,   359,   362,
     365,   372,   377,   379,   394,   423,     0,   435,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   437,   438,   452,   451,
       0,     0,   463,   375,     0,   432,   437,    35,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   357,     0,   464,
     326,   325,     0,    49,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   144,     0,
     147,     0,     0,   112,     0,     0,   113,     0,     0,     0,
       3,     0,   421,   104,   106,   418,   211,     0,     0,     0,
     376,     0,   433,   434,   374,   373,   432,   459,     0,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   320,
     321,   322,   323,   324,   318,     0,   401,     0,     0,   319,
       0,     0,     0,     0,     0,    39,   406,     0,   405,   451,
     397,     0,     0,     0,     0,     0,     0,    29,     0,     0,
       0,     0,     0,     0,     0,   459,     0,     0,    38,   426,
       0,     6,     0,    10,     0,    11,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,    43,     0,     0,
     125,     0,   137,     0,     0,   156,     0,     0,     0,     3,
     163,    46,     0,     0,     0,    48,     0,     0,    52,     0,
       0,     0,     0,   159,     0,     0,     0,     0,     0,   140,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   419,   420,   422,     0,     0,   214,     0,     0,     0,
     249,   460,     0,     0,     0,     0,   456,   284,   432,   437,
     438,   452,     0,     0,     0,     0,     0,     0,   283,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,   333,   336,   337,
     330,   331,     0,   334,   335,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   348,   350,   351,   353,   355,   356,
     358,   360,   361,   363,   364,   369,   370,   371,   366,   367,
     368,   378,   382,   393,   386,   387,   388,   389,   392,   390,
     391,   384,   385,   383,   417,     0,   414,    59,     0,    93,
      94,    95,    96,    97,    56,     0,     0,   400,     0,    15,
       0,     0,     0,     0,   442,   447,   448,    61,     0,   409,
       0,     0,     0,   398,     0,   396,   426,     0,     0,     0,
     320,   321,   322,   323,   324,   318,   319,    37,     0,   425,
       0,     0,     0,   429,   451,     0,     0,     0,     0,    26,
       0,    27,     0,    28,     0,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   460,     0,     0,   128,   460,     0,     0,   355,
     356,   261,   264,     0,   277,   279,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,   395,     0,
       0,     0,     0,     0,   460,     0,   460,     0,     0,     0,
       0,   114,   115,   116,   117,   317,   105,     0,     0,   214,
       0,     0,     0,     0,     0,     0,   461,     0,     0,   381,
     457,     0,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,   311,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,   285,    63,    65,    67,    70,
      68,    69,    64,    66,    76,    78,    80,    83,    81,    82,
      77,    79,     0,     0,   410,     0,     0,     0,     0,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   408,
       0,   404,     0,     0,   253,     0,     0,     0,     0,   442,
     447,   448,    36,     5,     0,    12,     0,     0,     0,     0,
      19,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,    87,    86,    90,    88,    89,    85,
      41,     0,    91,    92,    44,   122,   127,   126,     0,     0,
     136,   153,     0,   155,     0,     0,     0,     0,   266,     0,
       0,   162,     0,     0,     0,     0,     0,   174,     0,    47,
      50,    51,    54,    55,    31,    30,     0,     0,   157,   449,
     450,     0,     0,   138,   141,   142,   145,   146,     0,     0,
     212,   213,     0,     0,     0,     0,   210,   171,     0,     0,
       0,     0,     0,     0,     0,   248,   462,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   294,
     301,   308,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   328,   416,     0,   413,     0,
       0,    58,     0,    60,   172,     0,    16,   445,   444,   443,
       0,     0,     0,   407,   402,   403,     0,     0,     0,   399,
       0,     0,   258,   257,     0,     0,     0,    21,     0,    22,
       0,    25,   165,     0,    33,    32,     0,   462,   462,     0,
     129,     0,   130,     0,     0,     0,   262,   273,     0,   267,
     268,   161,   269,   265,     0,   278,   280,   282,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,     0,    74,    75,     0,     0,     0,   209,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   168,     0,
       0,     0,     0,     0,     9,     0,     0,   442,   448,   305,
       0,     0,     0,     0,     0,     0,   313,   314,   315,   316,
     286,   288,   290,   293,   291,   292,   287,   289,   415,   412,
     411,    57,   462,    71,    72,    73,   254,   255,     0,     0,
      13,    20,     0,     0,     0,     0,   164,     7,     0,    42,
      45,   124,   123,   460,     0,   134,   154,   151,   152,   275,
       0,     0,   270,     0,     0,     0,     0,     0,   178,   179,
     176,     0,     0,     0,     0,     0,     0,     0,     0,   160,
     158,   139,     0,     0,   150,     0,     3,   244,     0,   241,
       0,     0,     0,     0,   220,     0,     0,     0,     0,   215,
       0,     0,     0,     0,   216,   217,     0,   167,   169,   250,
     251,   252,     0,   173,     0,     0,     0,   295,   297,   300,
     298,   299,   296,    17,     0,   462,   260,   259,     0,    23,
       0,   131,   132,   263,   274,     0,   271,   272,     0,     0,
       0,   180,   182,   184,   187,   185,   186,   181,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   222,
       0,     0,     0,     0,   221,   218,   219,     0,   247,   304,
     303,   172,     0,     0,   256,    14,   462,     8,     0,   135,
     276,   188,   190,   189,     0,   149,   243,     0,   192,     0,
       0,     0,   191,     0,   240,     0,   229,   230,   224,   227,
     228,   223,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   302,   306,   307,    24,   133,     0,   242,     0,
       0,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   193,   239,   237,   238,   226,   235,   236,   225,   246,
       0,     0,     0,     0,     0,   196,   197,   194,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   148,     0,     0,
       0,   198,   200,   202,   205,   203,   204,   199,   201,   206,
     208,   207
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -847,  -847,     4,  -847,  -535,  -699,  -847,  -847,  -847,  -847,
    -847,   276,  1334,  -847,  -847,  -847,  -847,  -306,  -846,  -847,
    -847,  -847,   807,  -847,  -847,  -477,   858,  -370,   220,  1051,
      -2,  -223,  -847,  1145,  1159,  -847,   331,   -16,  1156,   853,
     862,     3,  -847,  1262,  -256,  -847,  -847,    60,  -847
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   203,    65,    66,   545,    67,    68,    69,    70,
      71,   973,    72,    73,   569,    74,    75,   340,   977,    76,
      77,   375,   590,    78,   595,   282,   516,   558,   122,   178,
     129,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   199,   464,    93,    94,   130,    95
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      80,   466,    24,   525,     2,   422,   840,   592,   706,   666,
     147,    26,     3,   662,   714,   148,   716,   718,   123,   668,
     139,   141,   143,   145,   146,   188,   171,   152,   154,   156,
     587,   112,   160,   161,   979,   715,   165,   167,   588,   111,
     950,   333,   189,   712,   182,    64,   172,   185,   107,   294,
     131,   132,   133,   244,   134,   174,   190,   343,   162,   346,
     198,   201,   194,   195,   551,   552,   108,   284,   553,   551,
     552,   135,   475,   553,   244,   175,   136,   344,   980,   347,
     719,   720,   985,   325,   736,   737,   981,   476,   551,   552,
     986,   334,   553,   299,    64,   109,    64,   259,   260,   261,
     262,   263,   264,   326,   491,   269,   271,   378,   273,   274,
     717,   825,    64,   110,   753,   754,   163,    64,   755,   198,
     826,   325,   554,   555,   556,   611,   296,   554,   555,   556,
     173,   247,   248,   667,   951,   982,   314,    64,   324,   987,
     329,   326,   331,   945,   124,   612,   554,   555,   556,   113,
    1068,   114,   589,   113,   688,   114,   753,   754,   379,   176,
     755,   345,   358,   348,   360,   325,   267,   365,   799,   366,
     367,   137,   368,   369,   283,   370,   371,   372,   370,   373,
     983,   738,   678,   300,   988,   326,   557,   327,   297,   159,
     328,   591,   756,   276,   984,  1086,   277,   301,   989,  1094,
     186,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     687,   408,   409,   410,   411,   412,   413,   414,   415,   177,
      97,    98,    79,   760,   761,   327,   438,   439,   328,   173,
     187,   113,   780,   114,   878,   191,   613,   242,   243,   487,
     278,  1118,   952,   202,   285,   614,   899,   149,   151,  1132,
     106,   721,   445,   446,   447,   448,   449,   450,   451,   192,
     168,   170,   286,   354,  1139,   615,   478,   183,   184,   327,
     239,   240,   328,   551,   552,   356,   959,   553,   244,   551,
     552,   355,   497,   553,   499,   757,   500,   501,   502,   503,
     504,   505,   506,   357,  1156,  1055,   298,   509,   510,   299,
     512,  1060,   517,  1056,   941,   953,   954,   362,   193,  1061,
     762,   763,   764,   530,   531,   532,   533,   534,   535,   536,
     549,   550,  1079,   539,   540,   363,   325,   341,   255,   581,
     342,   554,   555,   556,   336,   337,   906,   554,   555,   556,
     149,   561,   562,   559,   563,   564,   326,   565,   566,   176,
     551,   552,  1057,   571,   553,   573,   616,   302,  1062,   107,
     577,   578,   109,   330,   895,   332,   335,   582,   551,   552,
     765,   303,   553,   169,   907,   244,   256,   603,    64,   896,
     604,   832,   304,   537,   518,   359,   520,   361,   541,   257,
     606,   607,   608,   610,   548,   759,   305,  1058,   519,   522,
     521,  1041,   560,  1063,   306,   307,   308,   583,   554,   555,
     556,  1059,   568,   523,   908,   338,   851,  1064,   617,   795,
     339,   618,   619,    79,   245,   246,   554,   555,   556,   341,
     327,   620,   342,   328,   621,   622,   781,  1146,   623,   624,
     241,   343,   597,   990,   275,   601,   991,   773,   992,   993,
     316,   774,   887,   888,   157,   343,   343,   524,   158,   584,
     775,   344,   339,   776,   777,   862,   909,   778,   863,   864,
     284,   479,  1045,   648,   480,   344,   344,   634,   865,   653,
     654,   866,   867,   310,   910,   868,   869,   311,   955,   664,
    1110,   316,   956,   635,   509,   636,   637,   880,   258,   638,
     911,   318,   912,   913,   319,   320,   914,   639,   957,   674,
     640,   265,   641,   198,   915,   272,   683,   916,   685,   917,
     686,   287,   221,   693,   689,   690,   695,   936,   647,   625,
    1007,   696,   697,  1008,   698,   309,   650,   699,  1009,  1010,
     652,  1011,   709,   655,  1012,   345,   634,   351,   660,   635,
     663,   322,   782,  1031,   323,   636,  1032,   728,   779,   345,
     345,   637,  1033,   249,   250,   251,   960,  1147,  1034,   221,
     435,   436,   437,   551,   552,   638,   870,   553,   749,   252,
     253,   254,  1035,   997,   998,   675,   639,   349,   676,   692,
     640,   312,   322,  1036,   586,   323,   767,  1037,   701,   958,
     641,   770,   771,   772,   803,  1042,   350,  1038,  1046,   783,
     784,  1069,   785,   786,   352,   787,   788,   789,   790,   791,
     792,   793,   794,   353,  1123,   852,   853,  1124,  1125,   854,
     996,   554,   555,   556,   374,   693,   695,  1126,   376,   699,
    1127,  1128,  1081,  1082,  1129,  1130,  1083,   113,  1142,   114,
     923,   316,  1143,   925,   810,   811,   812,  1113,   377,   634,
    1114,   318,   635,   816,   319,   320,  1161,   680,  1144,  1162,
     681,   380,   636,   637,   315,   822,   823,   316,   317,  1163,
    1164,   638,   828,   855,   856,   857,   639,   318,  1165,   836,
     319,   320,   387,  1166,   321,   640,   469,  1111,   222,   223,
     224,   225,  1167,   797,   470,   800,   841,   641,   226,   227,
     228,   805,   693,   695,  1168,   847,   975,   471,   699,  1169,
    1170,   976,   858,   859,   861,  1171,   978,   700,   221,   472,
     299,   976,   229,   230,   871,  1131,   821,   881,   381,   382,
     882,   883,   884,   885,   204,   485,   876,   205,   206,  1067,
     704,   473,   322,   299,   976,   323,   385,   207,   386,  1145,
     208,   209,   705,   707,   210,   211,   710,   711,   713,   383,
     384,   900,   901,   902,   903,   904,   905,   729,   322,    79,
     299,   323,   806,   829,   930,   299,   830,   299,   467,   468,
       4,     5,     6,     7,   743,   744,   745,   125,   126,   117,
     118,   279,    13,    14,    15,    16,    17,    18,    97,    98,
     288,   289,   290,   291,   292,   927,   474,   937,   120,    28,
     938,  1019,   486,   293,   299,    34,  1077,   487,   483,   299,
     484,    36,   431,   432,   433,   483,   488,   669,   106,    44,
      45,    46,   489,   299,    49,   768,   949,   299,   212,   809,
     483,   490,   897,    53,    54,   477,   299,   481,   482,    55,
     961,   962,   963,   964,   965,   966,   967,   968,   493,   494,
     495,   496,   508,   299,   492,   974,   528,   384,   213,   498,
     929,   214,   215,   381,   529,   381,   538,   507,    57,    58,
     935,   216,   526,  1002,   217,   218,   542,   382,   219,   220,
     543,   544,   546,   382,    59,   574,   382,   576,   382,   381,
     651,    60,   722,   723,   758,   723,   766,   299,   121,   280,
     527,    63,   547,    64,   567,   281,  1014,   570,   452,   842,
    1016,  1017,   802,   299,   804,   299,   453,   572,   454,   455,
     813,   299,   814,   494,   815,   299,   575,   456,   457,  1024,
     819,   494,   458,  1028,  1029,  1030,  1004,   872,   459,   460,
     231,   232,   233,   234,   235,   236,   461,   462,   463,   579,
    1039,   585,   237,   381,   820,   879,   824,   681,   833,   723,
    1044,  1050,   221,   599,   238,   593,  1054,   837,   299,   838,
     299,   839,   544,   598,  1018,   889,   723,   594,  1020,   890,
     723,   918,   299,  1072,  1073,    96,   580,    97,    98,    99,
     100,   101,   102,   103,   919,   494,   581,     4,     5,     6,
       7,   582,   104,  1080,   125,   126,   117,   118,   279,    13,
      14,    15,    16,    17,    18,   105,   605,   106,   583,   920,
     299,   238,  1093,   922,   299,   120,    28,   381,   934,   943,
     382,   584,    34,   944,   544,   626,  1105,   627,    36,   942,
     628,  1109,   629,   946,   947,   948,    44,    45,    46,  1015,
     299,    49,  1071,   299,  1076,   299,  1078,   544,  1087,   723,
      53,    54,   974,  1095,   723,   630,    55,  1119,  1120,  1122,
     642,   969,  1140,   723,   244,   971,   179,   181,   441,   442,
     631,   632,    97,    98,   288,   289,   290,   291,   292,   443,
     444,   633,   634,   635,   636,    57,    58,   293,   637,   638,
     639,  1148,  1149,  1150,  1151,  1152,  1153,  1154,  1155,   640,
     641,    59,   106,  -380,  -380,   643,   644,   645,    60,   656,
    1158,  1159,  1160,   657,   658,   121,   661,   665,    63,   670,
      64,   672,   281,   677,   671,   673,   691,   299,     4,     5,
       6,     7,   682,  1021,  1022,   125,   126,   117,   118,   279,
      13,    14,    15,    16,    17,    18,   693,   694,   695,   339,
     696,   740,   697,   698,   699,   702,   120,    28,   703,   730,
     731,   732,   733,    34,   452,   734,   735,   465,   739,    36,
     741,   742,   453,   748,   454,   455,   746,    44,    45,    46,
     747,   750,    49,   456,   457,   751,   769,   807,   458,   801,
     803,    53,    54,   808,   459,   460,   817,    55,   452,   818,
     827,   831,   461,   462,   463,   834,   453,   835,   454,   455,
     843,   844,   845,   846,   848,   873,   849,   456,   457,   850,
     874,   875,   458,   877,   886,   891,    57,    58,   459,   460,
    1085,   892,   893,   894,    79,   921,   461,   462,   463,   898,
     923,   924,    59,   925,   926,   928,   932,   931,   933,    60,
     939,   940,   972,   994,   995,   970,   121,   798,   999,    63,
    1000,    64,  1001,   281,     4,     5,     6,     7,  1116,  1005,
    1006,   125,   126,   117,   118,   279,    13,    14,    15,    16,
      17,    18,  1013,  1023,  1026,  1025,  1027,  1040,  1043,  1047,
    1049,  1048,   120,    28,  1051,  1052,  1070,   976,  1102,    34,
    1053,  1088,  1103,  1065,    96,    36,    97,    98,    99,   100,
     101,   102,   103,    44,    45,    46,  1066,  1106,    49,  1074,
    1075,   104,  1084,  1096,  1107,  1097,  1098,    53,    54,  1141,
    1117,  1157,   150,    55,   602,  1112,   106,   416,   417,   418,
     419,   420,   421,   679,   423,   424,   425,   426,   427,   428,
     429,   430,  1099,  1100,  1101,  1104,  1108,  1115,  1133,  1134,
    1135,  1136,    57,    58,  1137,  1138,   752,   434,     0,   313,
     440,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,   121,     0,     0,    63,     0,    64,     0,   281,
       4,     5,     6,     7,     0,     0,     0,   388,   389,   390,
     391,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   392,   393,    24,     0,     0,    25,    26,    27,    28,
     394,   395,    31,    32,    33,    34,    35,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,   396,   397,    44,
      45,    46,    47,    48,    49,    50,     0,     0,     0,    51,
      52,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,    61,    62,     0,
       0,    63,     0,    64,   398,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   724,   725,    24,     0,
       0,    25,    26,    27,    28,    29,   726,    31,    32,    33,
      34,    35,     0,     0,     0,     0,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,     0,     0,     0,    51,    52,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,    61,    62,     0,     0,    63,     0,    64,   727,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,  1089,  1090,    24,     0,     0,    25,    26,    27,    28,
      29,  1091,    31,    32,    33,    34,    35,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,     0,    51,
      52,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,    56,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,    61,    62,    49,
       0,    63,     0,    64,  1092,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   125,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,     0,    60,     0,     0,    44,
      45,    46,   153,   121,    49,     0,    63,     0,    64,   609,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,   153,   121,    49,
       0,    63,     0,    64,   860,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,     0,   153,   121,     0,     0,    63,     0,    64,  1121,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,     0,     0,     0,
       0,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,     0,    51,
      52,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,    56,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,    61,    62,    49,
       0,    63,     0,    64,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   125,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,     0,    60,     0,     0,    44,
      45,    46,   127,   128,    49,     0,    63,     0,    64,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,   153,   121,    49,
       0,    63,     0,    64,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   196,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,     0,    59,
       0,    36,     0,     0,     0,     0,    60,     0,     0,    44,
      45,    46,   155,   121,    49,     0,    63,     0,    64,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,    59,     0,     0,    36,     0,     0,
       0,    60,     0,     0,     0,    44,    45,    46,   121,   197,
      49,    63,     0,    64,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   121,    49,     0,    63,   200,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,    59,     0,     0,    36,     0,
       0,     0,    60,     0,     0,     0,    44,    45,    46,   121,
     266,    49,    63,     0,    64,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   125,
     126,   117,   118,   119,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
     120,    28,     0,     0,     0,     0,     0,    34,     0,     0,
       0,    59,     0,    36,     0,     0,     0,     0,    60,   270,
       0,    44,    45,    46,     0,   121,    49,     0,    63,     0,
      64,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   125,   126,   117,   118,   119,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,   120,    28,     0,     0,
       0,     0,     0,    34,     0,     0,    59,     0,     0,    36,
       0,     0,     0,    60,     0,     0,     0,    44,    45,    46,
     121,   197,    49,    63,     0,    64,     0,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   125,   126,   117,   118,   119,    13,    14,    15,    16,
      17,    18,     0,     0,   364,     0,    57,    58,     0,     0,
       0,     0,   120,    28,     0,     0,     0,     0,     0,    34,
       0,     0,    59,     0,     0,    36,     0,     0,     0,    60,
       0,     0,     0,    44,    45,    46,   121,   295,    49,    63,
       0,    64,     0,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   125,   126,   117,   118,
     119,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   120,    28,     0,
       0,     0,     0,     0,    34,     0,     0,     0,    59,     0,
      36,     0,     0,     0,     0,    60,     0,     0,    44,    45,
      46,     0,   121,    49,     0,    63,     0,    64,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,    55,     0,
       0,   202,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   125,   126,   117,   118,   119,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,   120,    28,     0,     0,     0,     0,     0,    34,
       0,     0,     0,    59,     0,    36,     0,     0,     0,     0,
      60,     0,     0,    44,    45,    46,     0,   121,    49,     0,
      63,     0,    64,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   125,   126,   117,
     118,   119,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,   120,    28,
       0,     0,     0,     0,     0,    34,     0,     0,    59,     0,
       0,    36,     0,     0,     0,    60,     0,     0,     0,    44,
      45,    46,   121,   596,    49,    63,     0,    64,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   125,   126,   117,   118,   119,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,     0,    59,     0,    36,     0,     0,     0,
       0,    60,     0,     0,    44,    45,    46,     0,   121,    49,
       0,    63,   600,    64,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,    59,
       0,     0,    36,     0,     0,     0,    60,     0,     0,     0,
      44,    45,    46,   121,   646,    49,    63,     0,    64,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   125,   126,   117,   118,   119,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,   120,    28,     0,     0,     0,
       0,     0,    34,     0,     0,    59,     0,     0,    36,     0,
       0,     0,    60,     0,     0,     0,    44,    45,    46,   121,
     649,    49,    63,     0,    64,     0,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     4,     5,   513,     7,     0,     0,     0,
     125,   126,   117,   118,   514,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,   120,    28,     0,     0,     0,     0,     0,    34,     0,
       0,    59,     0,     0,    36,     0,     0,     0,    60,     0,
       0,     0,    44,    45,    46,   121,   659,    49,    63,     0,
      64,     0,     0,     0,     0,     0,    53,    54,     0,     0,
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
      16,    17,    18,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,   120,    28,     0,     0,     0,     0,     0,
      34,     0,     0,    59,     0,     0,    36,     0,     0,     0,
      60,     0,     0,     0,    44,    45,    46,   121,   796,    49,
      63,     0,    64,     0,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,    59,
       0,     0,    36,     0,     0,     0,    60,     0,     0,     0,
      44,    45,    46,   121,  1003,    49,    63,     0,    64,     0,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   138,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   140,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   142,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   144,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   121,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   164,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   166,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   121,    49,     0,    63,     0,   180,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   268,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,   513,     7,     0,     0,     0,   125,   126,
     117,   118,   514,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   511,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   125,   126,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,    28,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   515,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   125,   126,
     117,   118,   119,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   120,
      28,     0,     0,     0,     0,     0,    34,     0,     0,     0,
      59,     0,    36,     0,     0,     0,     0,    60,     0,     0,
      44,    45,    46,     0,   684,    49,     0,    63,     0,    64,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   115,   116,   117,   118,   119,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,   120,     0,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    59,     0,    36,     0,     0,
       0,     0,    60,     0,     0,    44,    45,    46,     0,   708,
      49,     0,    63,     0,    64,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -463,  -463,     0,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,   121,     0,     0,    63,     0,    64,
    -463,  -463,     0,  -463,  -463,  -463,  -463,  -463,  -463,  -463,
    -463,  -463,  -463,  -463,  -463,  -463,     0,     0,     0,  -463,
       0,     0,     0,     0,     0,     0,     0,  -463,  -463,  -463,
    -463,  -463,  -463,  -463,  -463,     0,     0,  -463,  -463,  -463,
    -463,  -463,  -463,     0,  -463,  -463,  -463,     0,  -463
};

static const yytype_int16 yycheck[] =
{
       2,   257,    26,   309,     0,   228,   705,   377,   543,    14,
      26,    30,     0,   490,    83,    14,    22,    10,    15,   496,
      22,    23,    24,    25,    26,    14,    26,    29,    30,    31,
      14,    11,    34,    35,   880,    83,    38,    39,    22,   116,
      22,    83,    31,    83,    46,   136,    46,    49,   116,   116,
      10,    11,    12,   122,    14,    26,    53,    26,    12,    26,
      62,    63,    59,    60,    10,    11,   134,   134,    14,    10,
      11,    31,   116,    14,   122,    46,    36,    46,    14,    46,
      73,    74,    14,    26,    40,    41,    22,   131,    10,    11,
      22,   133,    14,   133,   136,   118,   136,    99,   100,   101,
     102,   103,   104,    46,    80,   107,   108,    14,   110,   111,
     116,     5,   136,   136,    59,    60,    12,   136,    63,   121,
      14,    26,    68,    69,    70,    26,   128,    68,    69,    70,
     130,   123,   124,   138,   116,    71,   138,   136,   140,    71,
     142,    46,   144,   842,    14,    46,    68,    69,    70,   129,
     996,   131,   136,   129,   524,   131,    59,    60,    65,   130,
      63,   130,   164,   130,   166,    26,   106,   169,   645,   171,
     172,   131,   174,   175,   114,   177,   178,   179,   180,   181,
     116,   137,    80,   116,   116,    46,   132,   130,   128,    14,
     133,   132,   137,    14,   130,  1041,    17,   130,   130,  1045,
      14,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     132,   213,   214,   215,   216,   217,   218,   219,   220,   136,
     101,   102,     2,   593,   594,   130,   242,   243,   133,   130,
      14,   129,   137,   131,   137,    14,   137,    81,    82,   130,
      61,  1087,    10,    75,   118,    26,   137,    27,    28,  1095,
     131,   557,   249,   250,   251,   252,   253,   254,   255,    10,
      40,    41,   136,   118,  1110,    46,   268,    47,    48,   130,
     120,   121,   133,    10,    11,   118,   137,    14,   122,    10,
      11,   136,   284,    14,   286,   591,   288,   289,   290,   291,
     292,   293,   294,   136,  1140,    14,   130,   299,   300,   133,
     302,    14,   304,    22,   839,    73,    74,    29,    10,    22,
      77,    78,    79,   315,   316,   317,   318,   319,   320,   321,
     336,   337,  1021,   325,   326,    47,    26,   130,   115,   130,
     133,    68,    69,    70,    81,    82,   137,    68,    69,    70,
     120,   343,   344,   339,   346,   347,    46,   349,   350,   130,
      10,    11,    71,   355,    14,   357,   137,   116,    71,   116,
     362,   363,   118,   143,   116,   145,   146,   130,    10,    11,
     137,   130,    14,   131,   137,   122,   129,   134,   136,   131,
     136,   687,   116,   323,   116,   165,   116,   167,   328,   129,
     392,   393,   394,   395,   334,   132,   130,   116,   130,   116,
     130,   132,   342,   116,    10,    11,    12,   130,    68,    69,
      70,   130,   352,   130,   137,   131,   722,   130,    26,   642,
     136,    29,    30,   203,   113,   114,    68,    69,    70,   130,
     130,    39,   133,   133,    42,    43,   137,   137,    46,    47,
     108,    26,   382,    10,   130,   385,    13,    26,    15,    16,
      29,    30,   758,   759,    10,    26,    26,   131,    14,   130,
      39,    46,   136,    42,    43,    26,   137,    46,    29,    30,
     134,   130,   132,   475,   133,    46,    46,   130,    39,   481,
     482,    42,    43,    10,   137,    46,    47,    14,    26,   491,
     132,    29,    30,   130,   496,   130,   130,   753,    14,   130,
     137,    39,   137,   137,    42,    43,   137,   130,    46,   511,
     130,    14,   130,   515,   137,    14,   518,   137,   520,   137,
     522,   130,   130,   130,   526,   527,   130,   833,   468,   137,
     137,   130,   130,   137,   130,    14,   476,   130,   137,   137,
     480,   137,   544,   483,   137,   130,   130,    72,   488,   130,
     490,   130,   137,   137,   133,   130,   137,   559,   137,   130,
     130,   130,   137,   109,   110,   111,   137,   137,   137,   130,
     239,   240,   241,    10,    11,   130,   137,    14,   580,   125,
     126,   127,   137,   889,   890,   130,   130,   116,   133,   529,
     130,    10,   130,   137,   374,   133,   598,   137,   538,   137,
     130,   603,   604,   605,   130,   975,   116,   137,   978,   611,
     612,   137,   614,   615,   133,   617,   618,   619,   620,   621,
     622,   623,   624,   136,    26,    10,    11,    29,    30,    14,
     886,    68,    69,    70,    56,   130,   130,    39,   136,   130,
      42,    43,   137,   137,    46,    47,   137,   129,    26,   131,
     130,    29,    30,   130,   656,   657,   658,   137,   131,   130,
     137,    39,   130,   665,    42,    43,   137,   130,    46,   137,
     133,   136,   130,   130,    26,   677,   678,    29,    30,   137,
     137,   130,   684,    68,    69,    70,   130,    39,   137,   691,
      42,    43,   137,   137,    46,   130,   130,  1067,    84,    85,
      86,    87,   137,   643,   130,   645,   708,   130,    94,    95,
      96,   651,   130,   130,   137,   717,   131,   130,   130,   137,
     137,   136,   724,   725,   726,   137,   131,   130,   130,   130,
     133,   136,   118,   119,   736,   137,   676,    10,   132,   133,
      13,    14,    15,    16,    26,   119,   748,    29,    30,   131,
     130,   130,   130,   133,   136,   133,   133,    39,   135,   137,
      42,    43,   542,   543,    46,    47,   546,   547,   548,   132,
     133,   773,   774,   775,   776,   777,   778,   130,   130,   559,
     133,   133,   130,   130,   130,   133,   133,   133,   130,   131,
       3,     4,     5,     6,   574,   575,   576,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,   101,   102,
     103,   104,   105,   106,   107,   817,   130,   130,    31,    32,
     133,   130,   137,   116,   133,    38,   130,   130,   133,   133,
     135,    44,     7,     8,     9,   133,   131,   135,   131,    52,
      53,    54,    14,   133,    57,   135,   848,   133,   130,   135,
     133,   131,   135,    66,    67,   132,   133,   123,   124,    72,
     862,   863,   864,   865,   866,   867,   868,   869,   132,   133,
     132,   133,   132,   133,    14,   877,   132,   133,    26,    14,
     820,    29,    30,   132,   133,   132,   133,   130,   101,   102,
     830,    39,   116,   895,    42,    43,   132,   133,    46,    47,
      27,    28,   132,   133,   117,   132,   133,   132,   133,   132,
     133,   124,   132,   133,   132,   133,   132,   133,   131,   132,
     116,   134,    10,   136,   130,   138,   928,    14,    14,   709,
     932,   933,   132,   133,   132,   133,    22,    14,    24,    25,
     132,   133,   132,   133,   132,   133,    27,    33,    34,   951,
     132,   133,    38,   955,   956,   957,   896,   737,    44,    45,
      88,    89,    90,    91,    92,    93,    52,    53,    54,    10,
     972,   137,   100,   132,   133,    61,   132,   133,   132,   133,
     976,   983,   130,    10,   112,   131,   988,   132,   133,   132,
     133,    27,    28,   116,   934,   132,   133,   131,   938,   132,
     133,   132,   133,  1005,  1006,    99,   130,   101,   102,   103,
     104,   105,   106,   107,   132,   133,   130,     3,     4,     5,
       6,   130,   116,  1025,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   129,   116,   131,   130,   132,
     133,   112,  1044,   132,   133,    31,    32,   132,   133,   132,
     133,   130,    38,    27,    28,   130,  1058,   130,    44,   839,
     130,  1063,   130,   843,   844,   845,    52,    53,    54,   132,
     133,    57,   132,   133,   132,   133,    27,    28,   132,   133,
      66,    67,  1084,   132,   133,   130,    72,  1089,  1090,  1091,
      97,   871,   132,   133,   122,   875,    45,    46,   245,   246,
     130,   130,   101,   102,   103,   104,   105,   106,   107,   247,
     248,   130,   130,   130,   130,   101,   102,   116,   130,   130,
     130,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,   130,
     130,   117,   131,   132,   133,   131,    14,   131,   124,   116,
    1142,  1143,  1144,   116,   116,   131,   132,   131,   134,   119,
     136,   130,   138,    80,   137,   130,   116,   133,     3,     4,
       5,     6,   130,   943,   944,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   130,   130,   130,   136,
     130,   137,   130,   130,   130,   130,    31,    32,   130,   130,
     130,   130,   130,    38,    14,   130,   130,    17,   119,    44,
     119,   137,    22,   116,    24,    25,   130,    52,    53,    54,
     130,    14,    57,    33,    34,    14,    14,   135,    38,   130,
     130,    66,    67,   135,    44,    45,    80,    72,    14,    14,
     130,   130,    52,    53,    54,   130,    22,   130,    24,    25,
      10,    10,    10,    14,   116,   130,    10,    33,    34,    10,
     130,    27,    38,   130,    59,    14,   101,   102,    44,    45,
    1040,    14,    14,   130,  1044,   130,    52,    53,    54,   137,
     130,   130,   117,   130,   132,   131,    80,   130,    80,   124,
     130,   130,   130,   130,   130,   137,   131,   132,     7,   134,
       7,   136,     7,   138,     3,     4,     5,     6,  1078,   116,
     116,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,   130,    14,    10,   116,    10,   132,    75,    14,
     130,    14,    31,    32,    14,    14,   137,   136,    14,    38,
     130,   137,    14,   130,    99,    44,   101,   102,   103,   104,
     105,   106,   107,    52,    53,    54,   130,    14,    57,   132,
     130,   116,   130,   130,    14,   130,   130,    66,    67,   132,
    1084,  1141,    28,    72,   129,   137,   131,   222,   223,   224,
     225,   226,   227,   515,   229,   230,   231,   232,   233,   234,
     235,   236,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   101,   102,   130,   130,   589,   238,    -1,   137,
     244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   131,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    -1,    -1,    62,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,   134,    -1,   136,   137,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    -1,    -1,    -1,    62,    63,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,    -1,   134,    -1,   136,   137,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    -1,    -1,    62,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,   130,   131,    57,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,    52,
      53,    54,   130,   131,    57,    -1,   134,    -1,   136,   137,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,   130,   131,    57,
      -1,   134,    -1,   136,   137,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,    -1,   134,    -1,   136,   137,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    -1,    -1,    62,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,   130,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,    52,
      53,    54,   130,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,   130,   131,    57,
      -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,
      -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,    52,
      53,    54,   130,   131,    57,    -1,   134,    -1,   136,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    52,    53,    54,   131,   132,
      57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,   135,   136,
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
      -1,   117,    -1,    44,    -1,    -1,    -1,    -1,   124,   125,
      -1,    52,    53,    54,    -1,   131,    57,    -1,   134,    -1,
     136,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
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
      19,    20,    -1,    -1,    23,    -1,   101,   102,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,   117,    -1,    -1,    44,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    52,    53,    54,   131,   132,    57,   134,
      -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,   117,    -1,
      44,    -1,    -1,    -1,    -1,   124,    -1,    -1,    52,    53,
      54,    -1,   131,    57,    -1,   134,    -1,   136,    -1,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    52,    53,    54,    -1,   131,    57,    -1,
     134,    -1,   136,    -1,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,    -1,
      -1,    44,    -1,    -1,    -1,   124,    -1,    -1,    -1,    52,
      53,    54,   131,   132,    57,   134,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,   102,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,    57,
      -1,   134,   135,   136,    -1,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,   117,
      -1,    -1,    44,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      52,    53,    54,   131,   132,    57,   134,    -1,   136,    -1,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,   117,    -1,    -1,    44,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    52,    53,    54,   131,
     132,    57,   134,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,   117,    -1,    -1,    44,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    52,    53,    54,   131,   132,    57,   134,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    66,    67,    -1,    -1,
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
      18,    19,    20,    -1,    -1,    -1,    -1,   101,   102,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,   117,    -1,    -1,    44,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    52,    53,    54,   131,   132,    57,
     134,    -1,   136,    -1,    -1,    -1,    -1,    -1,    66,    67,
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
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     117,    -1,    44,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      52,    53,    54,    -1,   131,    57,    -1,   134,    -1,   136,
      -1,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,   101,
     102,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   117,    -1,    44,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    52,    53,    54,    -1,   131,
      57,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   131,    -1,    -1,   134,    -1,   136,
      81,    82,    -1,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,   119,   120,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129
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
     156,   130,   133,    26,    46,   130,    26,    46,   130,   116,
     116,    72,   133,   136,   118,   136,   118,   136,   169,   167,
     169,   167,    29,    47,    23,   169,   169,   169,   169,   169,
     169,   169,   169,   169,    56,   160,   136,   131,    14,    65,
     136,   132,   133,   132,   133,   133,   135,   137,    10,    11,
      12,    13,    24,    25,    33,    34,    50,    51,   137,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   172,   172,   172,   172,
     172,   172,   170,   172,   172,   172,   172,   172,   172,   172,
     172,     7,     8,     9,   173,   175,   175,   175,   176,   176,
     177,   178,   178,   179,   179,   180,   180,   180,   180,   180,
     180,   180,    14,    22,    24,    25,    33,    34,    38,    44,
      45,    52,    53,    54,   183,    17,   183,   130,   131,   130,
     130,   130,   130,   130,   130,   116,   131,   132,   169,   130,
     133,   123,   124,   133,   135,   119,   137,   130,   131,    14,
     131,    80,    14,   132,   133,   132,   133,   169,    14,   169,
     169,   169,   169,   169,   169,   169,   169,   130,   132,   169,
     169,   131,   169,     5,    14,   131,   165,   169,   116,   130,
     116,   130,   116,   130,   131,   156,   116,   116,   132,   133,
     169,   169,   169,   169,   169,   169,   169,   186,   133,   169,
     169,   186,   132,    27,    28,   144,   132,    10,   186,   176,
     176,    10,    11,    14,    68,    69,    70,   132,   166,   141,
     186,   169,   169,   169,   169,   169,   169,   130,   186,   153,
      14,   169,    14,   169,   132,    27,   132,   169,   169,    10,
     130,   130,   130,   130,   130,   137,   167,    14,    22,   136,
     161,   132,   166,   131,   131,   163,   132,   186,   116,    10,
     135,   186,   129,   134,   136,   116,   169,   169,   169,   137,
     169,    26,    46,   137,    26,    46,   137,    26,    29,    30,
      39,    42,    43,    46,    47,   137,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,    97,   131,    14,   131,   132,   186,   169,   132,
     186,   133,   186,   169,   169,   186,   116,   116,   116,   132,
     186,   132,   164,   186,   169,   131,    14,   138,   164,   135,
     119,   137,   130,   130,   169,   130,   133,    80,    80,   165,
     130,   133,   130,   169,   131,   169,   169,   132,   166,   169,
     169,   116,   186,   130,   130,   130,   130,   130,   130,   130,
     130,   186,   130,   130,   130,   167,   143,   167,   131,   169,
     167,   167,    83,   167,    83,    83,    22,   116,    10,    73,
      74,   156,   132,   133,    24,    25,    34,   137,   169,   130,
     130,   130,   130,   130,   130,   130,    40,    41,   137,   119,
     137,   119,   137,   167,   167,   167,   130,   130,   116,   169,
      14,    14,   161,    59,    60,    63,   137,   156,   132,   132,
     166,   166,    77,    78,    79,   137,   132,   169,   135,    14,
     169,   169,   169,    26,    30,    39,    42,    43,    46,   137,
     137,   137,   137,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   170,   132,   186,   132,   164,
     186,   130,   132,   130,   132,   186,   130,   135,   135,   135,
     169,   169,   169,   132,   132,   132,   169,    80,    14,   132,
     133,   186,   169,   169,   132,     5,    14,   130,   169,   130,
     133,   130,   156,   132,   130,   130,   169,   132,   132,    27,
     144,   169,   167,    10,    10,    10,    14,   169,   116,    10,
      10,   156,    10,    11,    14,    68,    69,    70,   169,   169,
     137,   169,    26,    29,    30,    39,    42,    43,    46,    47,
     137,   169,   167,   130,   130,    27,   169,   130,   137,    61,
     183,    10,    13,    14,    15,    16,    59,   156,   156,   132,
     132,    14,    14,    14,   130,   116,   131,   135,   137,   137,
     169,   169,   169,   169,   169,   169,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   132,   132,
     132,   130,   132,   130,   130,   130,   132,   169,   131,   186,
     130,   130,    80,    80,   133,   186,   156,   130,   133,   130,
     130,   143,   167,   132,    27,   144,   167,   167,   167,   169,
      22,   116,    10,    73,    74,    26,    30,    46,   137,   137,
     137,   169,   169,   169,   169,   169,   169,   169,   169,   167,
     137,   167,   130,   150,   169,   131,   136,   157,   131,   157,
      14,    22,    71,   116,   130,    14,    22,    71,   116,   130,
      10,    13,    15,    16,   130,   130,   183,   156,   156,     7,
       7,     7,   169,   132,   186,   116,   116,   137,   137,   137,
     137,   137,   137,   130,   169,   132,   169,   169,   186,   130,
     186,   167,   167,    14,   169,   116,    10,    10,   169,   169,
     169,   137,   137,   137,   137,   137,   137,   137,   137,   169,
     132,   132,   166,    75,   141,   132,   166,    14,    14,   130,
     169,    14,    14,   130,   169,    14,    22,    71,   116,   130,
      14,    22,    71,   116,   130,   130,   130,   131,   157,   137,
     137,   132,   169,   169,   132,   130,   132,   130,    27,   144,
     169,   137,   137,   137,   130,   167,   157,   132,   137,    24,
      25,    34,   137,   169,   157,   132,   130,   130,   130,   130,
     130,   130,    14,    14,   130,   169,    14,    14,   130,   169,
     132,   166,   137,   137,   137,   130,   167,   150,   157,   169,
     169,   137,   169,    26,    29,    30,    39,    42,    43,    46,
      47,   137,   157,   130,   130,   130,   130,   130,   130,   157,
     132,   132,    26,    30,    46,   137,   137,   137,   169,   169,
     169,   169,   169,   169,   169,   169,   157,   167,   169,   169,
     169,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137
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
     179,   179,   179,   180,   180,   180,   180,   180,   181,   181,
     182,   182,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   184,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   186,   186,   186,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187
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
       3,     3,     1,     2,     2,     2,     2,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     3,     4,     6,
       4,     3,     6,     6,     5,     3,     3,     6,     5,     4,
       4,     6,     6,     5,     3,     6,     5,     3,     2,     3,
       3,     2,     3,     1,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     1,     4,     4,     5,
       5,     1,     1,     1,     1,     2,     3,     4,     5,     2,
       3,     4,     5,     1,     2
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
#line 457 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3090 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 467 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3096 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 468 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3102 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 472 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3108 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 474 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3114 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 476 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3120 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 478 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3128 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 482 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3134 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 484 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3140 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 486 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3146 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 488 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3152 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 490 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3160 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 494 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3168 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 498 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3174 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 500 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3182 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 504 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3190 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 508 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3196 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 510 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3202 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 512 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3208 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 514 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3214 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 516 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3220 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 518 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3228 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 522 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3236 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 526 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3242 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 528 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3248 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 530 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3254 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 532 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3260 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 534 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3266 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 30: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 536 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3272 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 538 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3278 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 32: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 540 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3284 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 542 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3290 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 34: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 544 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); free(tok); }
          free((yyvsp[-2].sval)); free((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 3302 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 552 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3308 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 554 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3314 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 556 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3320 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 558 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3326 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 39: /* stmt: IDENT VAR_ARRAY ';'  */
#line 560 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3332 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ';'  */
#line 562 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3338 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 564 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3345 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 567 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3352 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ';'  */
#line 570 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3358 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 572 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3365 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 575 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3372 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 46: /* stmt: KW_TAKE expr ';'  */
#line 578 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3378 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 580 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 3386 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 584 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3392 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 586 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3398 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 588 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3405 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 591 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3412 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 594 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3418 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 596 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3425 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 54: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 599 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 3432 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 602 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 3439 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 605 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3445 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 607 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3454 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 612 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3462 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 616 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3470 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 620 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3479 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 61: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 625 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3487 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall ';'  */
#line 628 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3493 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 630 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3499 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 632 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3505 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 634 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3511 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 636 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3517 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 638 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3524 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 641 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3530 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 69: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 643 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3536 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 70: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 645 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3542 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 71: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 647 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3549 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 72: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 650 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3556 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 73: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 653 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3563 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 74: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 656 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3570 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 75: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 659 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3577 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 76: /* stmt: expr KW_IF expr ';'  */
#line 662 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3583 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNLESS expr ';'  */
#line 664 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3589 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WHILE expr ';'  */
#line 666 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3595 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNTIL expr ';'  */
#line 668 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3601 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 80: /* stmt: expr KW_FOR expr ';'  */
#line 670 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3608 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 81: /* stmt: expr KW_WITH expr ';'  */
#line 673 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3614 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 82: /* stmt: expr KW_WITHOUT expr ';'  */
#line 675 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3620 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 83: /* stmt: expr KW_GIVEN expr ';'  */
#line 677 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3626 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 679 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3633 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 682 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3640 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 685 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3648 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 689 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3655 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 692 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3661 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 694 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3667 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 696 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3673 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 91: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 698 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3680 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 92: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 701 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3687 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 704 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3694 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 707 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3701 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 710 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3708 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 713 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3715 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 716 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3722 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 98: /* stmt: expr ';'  */
#line 718 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3728 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 99: /* stmt: ';'  */
#line 719 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3734 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 100: /* stmt: if_stmt  */
#line 720 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3740 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 101: /* stmt: while_stmt  */
#line 721 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3746 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 102: /* stmt: for_stmt  */
#line 722 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3752 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 103: /* stmt: given_stmt  */
#line 723 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3758 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 104: /* stmt: KW_TRY block  */
#line 725 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3764 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 105: /* stmt: KW_TRY block KW_CATCH block  */
#line 727 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3770 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 106: /* stmt: KW_CATCH block  */
#line 729 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3776 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 107: /* stmt: block  */
#line 731 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3782 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 732 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3788 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 733 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3794 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 734 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3800 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 735 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3806 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 736 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3812 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 737 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3818 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 739 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3824 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 741 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3830 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 743 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3836 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 745 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3842 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 746 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3848 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 747 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3854 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 748 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3860 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 749 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3866 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block  */
#line 753 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3872 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 755 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3878 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 757 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3884 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block  */
#line 759 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3890 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 761 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3896 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 763 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3902 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block elsif_tail  */
#line 765 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3908 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 767 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3914 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block  */
#line 771 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3920 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 773 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3926 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 775 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3932 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 777 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3938 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 779 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3944 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 781 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3950 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 136: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 785 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3956 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE expr block  */
#line 787 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3962 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 791 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3968 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 793 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3974 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS expr block  */
#line 795 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3980 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 797 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3986 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 142: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 801 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3992 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL expr block  */
#line 803 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3998 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block  */
#line 807 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 4004 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 809 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 4010 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 811 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 4016 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP block  */
#line 815 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 4022 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 817 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4028 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 819 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4034 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 150: /* loop_incr: expr  */
#line 822 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4040 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 826 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4050 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 832 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 4060 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 838 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4069 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 843 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4079 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 849 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4088 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr block  */
#line 854 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 4095 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 157: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 859 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4107 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 867 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4120 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 159: /* when_list: %empty  */
#line 877 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4126 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 160: /* when_list: when_list KW_WHEN expr block  */
#line 879 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4134 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 885 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4146 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 162: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 893 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4156 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 163: /* sub_decl: KW_SUB IDENT sub_body  */
#line 899 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4166 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 905 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4178 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 913 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4188 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 919 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4198 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 925 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4211 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 934 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4222 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 941 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4235 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 170: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 950 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4248 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 959 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4259 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 172: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 968 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4270 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 173: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 975 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4279 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list '}'  */
#line 981 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4285 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr '}'  */
#line 983 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4292 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 986 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4299 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 989 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4305 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 991 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4311 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 993 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4317 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 995 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4323 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 997 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4329 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 999 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4335 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1001 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4341 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1003 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4348 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1006 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4354 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1008 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4360 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1010 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4366 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1012 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4373 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1015 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4380 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1018 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4388 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list '}'  */
#line 1023 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4394 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 192: /* method_body: '{' YADA '}'  */
#line 1024 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4400 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr '}'  */
#line 1026 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4407 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1029 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4414 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1032 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4420 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1034 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4426 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1036 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4432 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1038 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4438 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1040 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4444 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1042 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4450 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1044 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4456 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1046 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4463 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1049 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4469 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1051 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4475 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1053 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4481 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1055 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4488 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1058 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4495 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1061 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4503 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 209: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1067 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
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
#line 4521 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 210: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1083 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
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
#line 4538 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 211: /* is_clauses: %empty  */
#line 1097 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
       { (yyval.sval) = (char *)0; }
#line 4544 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 212: /* is_clauses: is_clauses IDENT IDENT  */
#line 1099 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
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
#line 4560 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 213: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1111 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
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
#line 4576 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 214: /* class_body_list: %empty  */
#line 1124 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4582 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1126 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4589 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1129 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4596 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1132 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4603 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1135 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4610 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1138 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4617 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1141 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4624 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1144 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4631 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1147 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4638 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1150 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4645 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1153 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4652 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1156 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4659 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1159 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4666 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1162 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4677 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1169 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4688 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1176 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4699 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1183 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4710 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1190 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4718 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1194 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4726 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1198 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4734 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1202 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4742 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1206 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4753 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1213 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4764 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1220 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4775 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1227 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4786 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1234 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4800 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1244 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4812 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1252 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4824 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1260 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4837 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1269 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4848 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1276 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4859 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1283 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4873 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1293 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4885 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1301 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4897 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 248: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1311 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
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
#line 4914 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 249: /* grammar_body_list: %empty  */
#line 1325 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4920 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1327 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4929 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 251: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1332 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4938 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 252: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1337 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4947 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 253: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1344 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4955 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 254: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1348 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4963 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 255: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1352 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4971 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 256: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1356 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4979 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 257: /* pair_list: IDENT OP_FATARROW expr  */
#line 1362 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4985 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 258: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1364 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4991 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 259: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1366 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4997 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 260: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1368 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5003 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR  */
#line 1371 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5009 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 262: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1372 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                              { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5015 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1373 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                             { free((yyvsp[-1].sval)); free((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5021 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 264: /* param_list: VAR_ARRAY  */
#line 1374 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5027 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_ARRAY  */
#line 1375 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5033 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 266: /* param_list: IDENT VAR_SCALAR  */
#line 1376 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5039 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 267: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1377 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5045 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 268: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1378 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5051 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 269: /* param_list: param_list ',' VAR_SCALAR  */
#line 1379 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 5057 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1380 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 5063 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 271: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1381 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5069 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1382 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 5075 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 273: /* param_list: VAR_SCALAR '=' expr  */
#line 1383 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5081 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1384 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 5087 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 275: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1385 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5093 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1386 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 5099 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_POS  */
#line 1387 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 5105 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_POS  */
#line 1388 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 5111 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 279: /* param_list: SLURPY_LOL  */
#line 1389 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5117 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' SLURPY_LOL  */
#line 1390 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5123 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 281: /* param_list: SLURPY_NAMED  */
#line 1391 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5129 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 282: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1392 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5135 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list '}'  */
#line 1395 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5141 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 284: /* block: '{' YADA '}'  */
#line 1396 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5147 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr '}'  */
#line 1398 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5153 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1400 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5159 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1402 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5165 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1404 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5171 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1406 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5177 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1408 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5184 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1411 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5190 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1413 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5196 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1415 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5202 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1417 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5208 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1419 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5215 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1422 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5222 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1425 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5230 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1429 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5237 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1432 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5244 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1435 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5251 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1438 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5257 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1440 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5265 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1444 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5272 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1447 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5279 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1450 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5286 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1453 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5293 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1456 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5300 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1459 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5306 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1461 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5312 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1463 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5318 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_LAST '}'  */
#line 1465 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5324 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1467 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5330 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1469 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5336 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1471 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1473 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5348 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1475 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5354 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 317: /* closure: '{' expr '}'  */
#line 1478 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5360 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR '=' expr  */
#line 1481 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5366 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 319: /* expr: VAR_ARRAY '=' expr  */
#line 1482 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5372 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1483 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5378 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 321: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1484 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5384 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 322: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1485 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5390 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 323: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1486 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5396 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 324: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1487 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5402 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 325: /* expr: KW_GATHER block  */
#line 1488 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5412 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 326: /* expr: KW_GATHER for_stmt  */
#line 1493 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5422 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 327: /* expr: tern_expr  */
#line 1498 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5428 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 328: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1502 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5434 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 329: /* tern_expr: cmp_expr  */
#line 1503 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5440 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1506 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5446 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1507 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5452 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1508 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5458 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 333: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1509 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5464 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 334: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1510 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5470 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 335: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1511 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5476 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 336: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1512 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5482 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 337: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1513 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5488 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1514 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5494 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1515 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5500 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1516 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5506 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1517 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5512 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1518 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5518 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 343: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1519 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5524 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 344: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1521 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5534 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 345: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1527 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5544 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 346: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1533 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5554 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 347: /* cmp_expr: divis_expr  */
#line 1538 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5560 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 348: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1541 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5566 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 349: /* divis_expr: jct_expr  */
#line 1542 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5572 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 350: /* jct_expr: jct_expr '|' range_expr  */
#line 1545 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5578 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 351: /* jct_expr: jct_expr '&' range_expr  */
#line 1546 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5584 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 352: /* jct_expr: dor_expr  */
#line 1547 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5590 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 353: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1551 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5596 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 354: /* dor_expr: range_expr  */
#line 1552 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5602 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 355: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1555 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5608 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 356: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1556 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5614 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 357: /* range_expr: add_expr  */
#line 1557 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5620 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 358: /* add_expr: add_expr '~' repl_expr  */
#line 1560 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5626 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 359: /* add_expr: repl_expr  */
#line 1561 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5632 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 360: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1564 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5638 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 361: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1565 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 5644 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 362: /* repl_expr: addsub_expr  */
#line 1566 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5650 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 363: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1569 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5656 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 364: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1570 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5662 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 365: /* addsub_expr: mul_expr  */
#line 1571 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5668 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 366: /* mul_expr: mul_expr '*' unary_expr  */
#line 1574 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5674 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 367: /* mul_expr: mul_expr '/' unary_expr  */
#line 1575 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5680 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 368: /* mul_expr: mul_expr '%' unary_expr  */
#line 1576 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5686 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 369: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1577 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5692 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 370: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1579 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5698 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 371: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1581 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5704 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 372: /* mul_expr: unary_expr  */
#line 1582 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5710 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 373: /* unary_expr: '-' unary_expr  */
#line 1585 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5716 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 374: /* unary_expr: '!' unary_expr  */
#line 1586 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5722 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 375: /* unary_expr: CARET unary_expr  */
#line 1587 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5728 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 376: /* unary_expr: OP_REDUCE unary_expr  */
#line 1589 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5737 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 377: /* unary_expr: pow_expr  */
#line 1593 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5743 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 378: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1596 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5749 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 379: /* pow_expr: postfix_expr  */
#line 1597 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5755 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 380: /* scalar_list: VAR_SCALAR  */
#line 1600 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5761 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 381: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1601 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5767 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 382: /* meth_name: IDENT  */
#line 1604 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5773 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 383: /* meth_name: KW_SORT  */
#line 1605 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5779 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 384: /* meth_name: KW_MAP  */
#line 1606 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5785 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 385: /* meth_name: KW_GREP  */
#line 1607 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5791 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 386: /* meth_name: KW_SAY  */
#line 1608 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5797 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 387: /* meth_name: KW_PRINT  */
#line 1609 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5803 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 388: /* meth_name: KW_TAKE  */
#line 1610 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5809 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 389: /* meth_name: KW_RETURN  */
#line 1611 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5815 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 390: /* meth_name: KW_EXISTS  */
#line 1612 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5821 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 391: /* meth_name: KW_DELETE  */
#line 1613 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5827 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 392: /* meth_name: KW_JOIN  */
#line 1614 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=strdup("join"); }
#line 5833 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 393: /* meth_name: TESTOP  */
#line 1615 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5839 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 394: /* postfix_expr: call_expr  */
#line 1617 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5845 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 395: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1620 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5855 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '(' arg_list ')'  */
#line 1626 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5864 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '(' ')'  */
#line 1630 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5870 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1632 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5876 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1634 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5882 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 400: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1636 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5891 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 401: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1641 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5897 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 402: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1643 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5907 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 403: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1649 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5917 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 404: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1655 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5925 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 405: /* call_expr: IDENT '.' KW_NEW  */
#line 1659 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5933 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 406: /* call_expr: IDENT '.' IDENT  */
#line 1663 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5942 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 407: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1668 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5953 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 408: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1675 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5962 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 409: /* call_expr: IDENT '.' CARET IDENT  */
#line 1680 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5972 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 410: /* call_expr: atom '.' CARET IDENT  */
#line 1686 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5982 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 411: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1692 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5993 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 412: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1699 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 6004 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 413: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1706 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6013 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 414: /* call_expr: atom '.' meth_name  */
#line 1711 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6022 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 415: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1716 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 6033 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 416: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1723 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 6042 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 417: /* call_expr: call_expr '.' meth_name  */
#line 1728 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 6051 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 418: /* call_expr: KW_DIE expr  */
#line 1733 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 6057 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 419: /* call_expr: KW_MAP closure expr  */
#line 1735 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6063 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 420: /* call_expr: KW_GREP closure expr  */
#line 1737 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6069 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 421: /* call_expr: KW_SORT expr  */
#line 1739 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6075 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 422: /* call_expr: KW_SORT closure expr  */
#line 1741 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6081 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 423: /* call_expr: atom  */
#line 1742 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 6087 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 424: /* arg_list: expr  */
#line 1745 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 6093 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 425: /* arg_list: arg_list ',' expr  */
#line 1746 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 6099 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 426: /* arg_list: arg_list ','  */
#line 1747 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 6105 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 427: /* atom: LIT_INT  */
#line 1750 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 6111 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 428: /* atom: LIT_FLOAT  */
#line 1751 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 6117 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 429: /* atom: LIT_STR  */
#line 1752 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 6123 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 430: /* atom: WORDLIST  */
#line 1754 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 6136 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 431: /* atom: LIT_INTERP_STR  */
#line 1762 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 6142 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 432: /* atom: VAR_SCALAR  */
#line 1763 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6148 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 433: /* atom: OP_INC VAR_SCALAR  */
#line 1764 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6154 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 434: /* atom: OP_DEC VAR_SCALAR  */
#line 1765 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6160 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 435: /* atom: VAR_SCALAR OP_INC  */
#line 1766 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6166 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 436: /* atom: VAR_SCALAR OP_DEC  */
#line 1767 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6172 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 437: /* atom: VAR_ARRAY  */
#line 1768 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6178 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 438: /* atom: VAR_HASH  */
#line 1769 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6184 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 439: /* atom: VAR_CAPTURE  */
#line 1771 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6192 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 440: /* atom: VAR_FH  */
#line 1775 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6200 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 441: /* atom: VAR_NAMED_CAPTURE  */
#line 1779 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6207 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 442: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1782 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6213 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 443: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1784 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6219 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 444: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1786 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6225 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 445: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1788 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6231 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 446: /* atom: ARR_ALL_SLICE  */
#line 1790 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6237 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 447: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1792 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6243 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 448: /* atom: VAR_HASH '{' expr '}'  */
#line 1794 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6249 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 449: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1796 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6255 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 450: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1798 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6261 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 451: /* atom: IDENT  */
#line 1799 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6267 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 452: /* atom: VAR_TWIGIL  */
#line 1801 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6275 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 453: /* atom: VAR_ARRAY_TWIGIL  */
#line 1805 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6283 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 454: /* atom: VAR_HASH_TWIGIL  */
#line 1809 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6291 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 455: /* atom: '[' ']'  */
#line 1812 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6297 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 456: /* atom: '[' expr ']'  */
#line 1814 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 6303 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 457: /* atom: '[' expr ',' ']'  */
#line 1816 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6309 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 458: /* atom: '[' expr ',' arg_list ']'  */
#line 1818 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6316 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 459: /* atom: '(' ')'  */
#line 1820 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6322 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 460: /* atom: '(' expr ')'  */
#line 1821 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6328 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 461: /* atom: '(' expr ',' ')'  */
#line 1823 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6334 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 462: /* atom: '(' expr ',' arg_list ')'  */
#line 1825 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6341 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 463: /* atom: block  */
#line 1827 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6347 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;

  case 464: /* atom: KW_SUB block  */
#line 1828 "/home/claude06/SCRIP/src/parsers/raku/raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6353 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"
    break;


#line 6357 "/home/claude06/SCRIP/src/parsers/raku/raku.tab.c"

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

#line 1830 "/home/claude06/SCRIP/src/parsers/raku/raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
