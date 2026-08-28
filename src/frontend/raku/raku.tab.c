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

#line 380 "raku.tab.c"

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
  YYSYMBOL_KW_CONSTANT = 35,               /* KW_CONSTANT  */
  YYSYMBOL_KW_GIVEN = 36,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 37,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 38,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 39,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 40,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 41,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 42,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 43,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 44,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 45,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 46,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 47,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 48,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 49,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 50,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 51,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 52,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 53,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 54,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 55,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 56,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 57,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 58,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 59,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 60,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 61,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 62,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 63,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 64,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 65,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 66,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 67,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 68,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 69,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 70,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 71,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 72,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 73,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 74,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 75,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 76,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 77,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 78,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 79,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 80,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 81,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 82,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 83,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 84,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 85,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 86,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 87,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 88,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 89,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 90,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 91,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 92,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 93,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 94,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 95,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 96,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 97,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 98,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 99,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 100,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 101,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 102,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 103,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 104,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 105,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 106,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 107,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 108,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 109,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 110,                   /* OP_POW  */
  YYSYMBOL_111_ = 111,                     /* '='  */
  YYSYMBOL_112_ = 112,                     /* '!'  */
  YYSYMBOL_113_ = 113,                     /* '<'  */
  YYSYMBOL_114_ = 114,                     /* '>'  */
  YYSYMBOL_115_ = 115,                     /* '|'  */
  YYSYMBOL_116_ = 116,                     /* '&'  */
  YYSYMBOL_117_ = 117,                     /* '~'  */
  YYSYMBOL_118_ = 118,                     /* '+'  */
  YYSYMBOL_119_ = 119,                     /* '-'  */
  YYSYMBOL_120_ = 120,                     /* '*'  */
  YYSYMBOL_121_ = 121,                     /* '/'  */
  YYSYMBOL_122_ = 122,                     /* '%'  */
  YYSYMBOL_UMINUS = 123,                   /* UMINUS  */
  YYSYMBOL_124_ = 124,                     /* '.'  */
  YYSYMBOL_125_ = 125,                     /* ';'  */
  YYSYMBOL_126_ = 126,                     /* '('  */
  YYSYMBOL_127_ = 127,                     /* ')'  */
  YYSYMBOL_128_ = 128,                     /* ','  */
  YYSYMBOL_129_ = 129,                     /* '['  */
  YYSYMBOL_130_ = 130,                     /* ']'  */
  YYSYMBOL_131_ = 131,                     /* '{'  */
  YYSYMBOL_132_ = 132,                     /* '}'  */
  YYSYMBOL_133_ = 133,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 134,                 /* $accept  */
  YYSYMBOL_program = 135,                  /* program  */
  YYSYMBOL_stmt_list = 136,                /* stmt_list  */
  YYSYMBOL_stmt = 137,                     /* stmt  */
  YYSYMBOL_if_stmt = 138,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 139,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 140,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 141,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 142,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 143,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 144,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 145,                /* loop_incr  */
  YYSYMBOL_for_stmt = 146,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 147,               /* given_stmt  */
  YYSYMBOL_when_list = 148,                /* when_list  */
  YYSYMBOL_sub_decl = 149,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 150,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 151,                 /* sub_body  */
  YYSYMBOL_method_body = 152,              /* method_body  */
  YYSYMBOL_class_decl = 153,               /* class_decl  */
  YYSYMBOL_role_decl = 154,                /* role_decl  */
  YYSYMBOL_is_clauses = 155,               /* is_clauses  */
  YYSYMBOL_class_body_list = 156,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 157,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 158,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 159,           /* named_arg_list  */
  YYSYMBOL_pair_list = 160,                /* pair_list  */
  YYSYMBOL_param_list = 161,               /* param_list  */
  YYSYMBOL_block = 162,                    /* block  */
  YYSYMBOL_closure = 163,                  /* closure  */
  YYSYMBOL_expr = 164,                     /* expr  */
  YYSYMBOL_tern_expr = 165,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 166,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 167,               /* divis_expr  */
  YYSYMBOL_jct_expr = 168,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 169,                 /* dor_expr  */
  YYSYMBOL_range_expr = 170,               /* range_expr  */
  YYSYMBOL_add_expr = 171,                 /* add_expr  */
  YYSYMBOL_repl_expr = 172,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 173,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 174,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 175,               /* unary_expr  */
  YYSYMBOL_pow_expr = 176,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 177,              /* scalar_list  */
  YYSYMBOL_meth_name = 178,                /* meth_name  */
  YYSYMBOL_postfix_expr = 179,             /* postfix_expr  */
  YYSYMBOL_call_expr = 180,                /* call_expr  */
  YYSYMBOL_arg_list = 181,                 /* arg_list  */
  YYSYMBOL_atom = 182                      /* atom  */
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
#define YYLAST   4124

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  446
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1127

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   366


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
       2,     2,     2,   112,     2,     2,     2,   122,   116,     2,
     126,   127,   120,   118,   128,   119,   124,   121,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   133,   125,
     113,   111,   114,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   129,     2,   130,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   131,   115,   132,   117,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   123
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   394,   394,   405,   406,   409,   411,   413,   415,   419,
     421,   423,   425,   429,   432,   436,   438,   442,   445,   449,
     451,   453,   455,   457,   459,   463,   467,   469,   471,   473,
     475,   477,   479,   481,   483,   485,   487,   489,   491,   493,
     495,   498,   501,   503,   506,   509,   511,   513,   515,   518,
     521,   523,   528,   532,   536,   541,   545,   546,   548,   550,
     552,   554,   557,   559,   561,   563,   566,   569,   572,   575,
     578,   580,   582,   584,   586,   589,   591,   593,   595,   598,
     601,   605,   608,   610,   612,   614,   617,   620,   623,   626,
     629,   632,   635,   636,   637,   638,   639,   640,   641,   643,
     645,   647,   649,   650,   651,   652,   653,   654,   655,   657,
     659,   661,   663,   664,   665,   666,   669,   671,   673,   675,
     677,   679,   681,   683,   687,   689,   691,   693,   695,   697,
     701,   703,   707,   709,   711,   713,   717,   719,   723,   725,
     727,   731,   733,   735,   739,   742,   748,   754,   759,   765,
     770,   775,   783,   794,   795,   801,   809,   815,   821,   829,
     835,   841,   850,   857,   866,   875,   884,   891,   898,   899,
     902,   905,   907,   909,   911,   913,   915,   917,   919,   922,
     924,   926,   928,   931,   934,   940,   941,   942,   945,   948,
     950,   952,   954,   956,   958,   960,   962,   965,   967,   969,
     971,   974,   977,   983,   999,  1014,  1015,  1027,  1041,  1042,
    1045,  1048,  1051,  1054,  1057,  1060,  1063,  1066,  1069,  1072,
    1075,  1078,  1085,  1092,  1099,  1106,  1110,  1114,  1118,  1122,
    1129,  1136,  1143,  1150,  1160,  1168,  1176,  1185,  1192,  1199,
    1209,  1217,  1227,  1242,  1243,  1248,  1253,  1260,  1264,  1268,
    1272,  1278,  1280,  1282,  1284,  1288,  1289,  1290,  1291,  1292,
    1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,
    1303,  1304,  1305,  1306,  1307,  1310,  1311,  1312,  1314,  1316,
    1318,  1320,  1322,  1325,  1327,  1329,  1331,  1333,  1336,  1339,
    1343,  1346,  1349,  1352,  1354,  1358,  1361,  1364,  1367,  1370,
    1373,  1375,  1377,  1379,  1381,  1383,  1385,  1387,  1389,  1393,
    1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1408,  1411,
    1413,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
    1425,  1426,  1427,  1428,  1429,  1430,  1436,  1442,  1448,  1451,
    1452,  1455,  1456,  1457,  1460,  1462,  1465,  1466,  1467,  1470,
    1471,  1474,  1475,  1478,  1479,  1480,  1483,  1484,  1485,  1486,
    1487,  1490,  1491,  1492,  1493,  1498,  1501,  1502,  1505,  1506,
    1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,
    1519,  1521,  1523,  1528,  1529,  1531,  1533,  1538,  1540,  1546,
    1552,  1556,  1560,  1565,  1572,  1577,  1583,  1589,  1596,  1603,
    1608,  1613,  1620,  1625,  1630,  1632,  1634,  1636,  1638,  1640,
    1643,  1644,  1645,  1648,  1649,  1650,  1651,  1660,  1661,  1662,
    1663,  1664,  1665,  1666,  1667,  1668,  1672,  1676,  1679,  1681,
    1683,  1685,  1687,  1689,  1691,  1693,  1695,  1697,  1698,  1702,
    1706,  1710,  1711,  1712,  1714,  1717,  1718
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
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_CONSTANT", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_ROLE",
  "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE", "ARR_ALL_SLICE",
  "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT",
  "OP_SLE", "OP_SGT", "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='",
  "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'",
  "'}'", "':'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "scalar_methcall", "sub_body", "method_body", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "pair_list", "param_list",
  "block", "closure", "expr", "tern_expr", "cmp_expr", "divis_expr",
  "jct_expr", "dor_expr", "range_expr", "add_expr", "repl_expr",
  "addsub_expr", "mul_expr", "unary_expr", "pow_expr", "scalar_list",
  "meth_name", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-821)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-446)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -821,    78,  1822,  -821,  -821,  -821,  -821,  -821,  1092,   -73,
     -41,   -22,   422,  -821,  -821,  3949,  -821,  -821,  -821,    95,
    1895,   226,  3000,  3073,  3146,  3219,  3292,    -5,     6,  3292,
    1968,     0,  3292,   135,   238,  3365,  3438,     6,   -61,    20,
     191,    47,    47,  3511,     6,     6,  3292,   204,   230,   180,
    3949,  -821,  -821,   262,   336,   345,  3949,  3949,  -821,  2041,
     321,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,   854,  -821,  -821,  -821,  4000,   878,  -821,   413,   542,
     -16,   266,  -821,   319,   358,   166,   423,  -821,  -821,   334,
     355,   366,   505,  -821,  -821,  3292,  3292,  3292,  3292,  3292,
    3292,   524,  2115,  3584,  2189,   536,  3292,  3292,   165,   409,
     433,   444,   -10,  -821,     6,  -821,  -821,   474,   727,     1,
    -821,  2262,  -821,   373,   219,   228,   292,   742,   604,   298,
     630,  2041,   618,  2041,   127,  2041,     6,  2041,     6,    76,
     332,    88,  -821,  -821,   526,  -821,   264,   567,   571,   562,
      12,    38,  2041,     6,  2041,     6,   176,  2336,  -821,  3292,
    3292,  -821,  3292,  3292,  -821,  3292,  3292,  3292,  2409,  3292,
    -821,   644,  -821,  -821,  -821,   587,   588,    29,  -821,   650,
    -821,  -821,  -821,  -821,  -821,   281,   668,  1223,  3292,  3292,
    3292,  3292,  3292,  3292,  3292,  3292,  -821,  3292,  3292,  3292,
    3292,  3292,  3292,  3292,  3292,  -821,  3949,  3949,  3949,  3949,
    3949,  3949,  3949,  3949,  3949,  3949,  3949,  3949,  3949,  3949,
    3949,   803,  3949,  3949,  3949,  3949,  3949,  3949,  3949,  3949,
    3949,  3949,  3949,  3949,  3949,  3949,  3949,  1266,  1131,   439,
     723,   750,   752,   754,   756,   776,   163,  -821,   485,  2041,
     276,   651,   424,   720,   731,   781,   741,   875,   799,   202,
    -821,   922,   688,   738,  2189,   944,  3292,  -821,  3292,  3292,
    3292,  3292,  3292,  3292,  3292,   840,   281,   746,  -821,  3292,
    3292,  -821,  3657,  -821,  3730,  -821,   327,   343,   346,   205,
     866,   894,  -821,   758,   764,  3292,  3292,  3292,  3292,  3292,
    3292,  3292,  -821,  3292,   768,  3292,  3292,  -821,  3292,   772,
     827,   775,  -821,   986,  3292,  -821,  3949,  3949,    15,  -821,
    -821,  -821,  3292,  3292,  -821,  3292,  3292,  -821,   996,  3292,
    1016,  3292,   785,  1025,   788,  -821,  3292,  3292,  1043,   930,
     934,   938,   939,   942,   928,  -821,  -821,  -821,     6,    -3,
    -821,    21,   943,   945,  -821,  -821,  2482,  -821,  1333,    92,
     150,   959,  3000,  3073,  3292,  1552,   215,   217,  -821,   473,
     947,   948,   949,   950,   960,   961,   962,   963,   964,   965,
     984,   985,   988,   989,   990,   991,   977,   977,   977,   977,
     977,   977,  1017,   977,   977,   977,   977,   977,   977,   977,
     977,  -821,  -821,  -821,   -16,  -821,  -821,  -821,   995,   995,
     358,   166,   423,   423,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
     992,  1103,   998,  -821,  2556,  -821,  -821,  -821,  -821,  -821,
    -821,  3292,  2630,  -821,   792,  3292,  -821,  3292,  3292,  3292,
    3292,  1010,  1011,  1012,  -821,  2704,  -821,   745,  3292,   999,
    -821,    -6,  -821,  1088,   663,  1013,   994,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  1005,  -821,  1006,  2041,   309,
    1058,   248,  2778,   386,  1015,  3292,  -821,  3803,  -821,  3292,
    -821,    41,  -821,  3292,  3292,  1030,  1126,  2482,  1018,  1019,
    1021,  1022,  1024,  1029,  1033,   463,  2482,  1035,  1036,   475,
       6,   -13,  3876,  -821,     6,     6,   239,   -64,    66,  1039,
    -821,    40,  -821,  -821,  -821,  1031,   796,  1355,  1041,  1042,
    1044,  1045,     2,  1054,  1046,  1057,  1047,     6,     6,     6,
    1049,  1050,  1065,  3292,  -821,  -821,  -821,  -821,  -821,  -821,
    1163,  1169,  -821,     4,  1031,   800,    48,   299,     9,  -821,
     802,  1170,  2189,  3292,  3292,   494,    49,   -56,  -821,   330,
    3292,  3292,  -821,  3292,  3292,  -821,  3292,  3292,  3292,  3292,
    3292,  3292,  3292,  3292,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  3949,  2852,  -821,   935,  1060,   804,  1064,  -821,   806,
    2482,  1072,   611,  1068,  1069,   705,  3292,  3292,  3292,  -821,
     816,  -821,   829,   834,  -821,  3292,  1124,  1188,   836,  -821,
    -821,  -821,  -821,  -821,   841,  3292,  -821,  3292,  3292,  3292,
     843,  -821,    31,  -821,  1079,  2041,   654,  1080,  1031,   845,
    1081,  1083,  3292,  -821,   847,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,   853,  -821,  -821,  -821,   955,  -821,  -821,
    2041,     6,  -821,  -821,  1199,  -821,  1200,  1201,  3292,  1101,
    1203,  1205,  -821,  1031,   340,  3000,  3073,  1625,  -821,   581,
    -821,  -821,  -821,  -821,  3292,     6,  -821,  -821,  -821,  1095,
    1097,  1190,  -821,  -821,  -821,  -821,  3292,  1098,  -821,  -821,
      17,  1007,   846,  1168,  -821,  -821,  1031,  1031,   860,   863,
    1211,  1216,  1236,  -821,  -821,   206,   709,  1119,    81,  3292,
    3292,  3292,  3292,  3292,  3292,  -821,  -821,  -821,  -821,   117,
     252,   256,   258,   368,   426,   458,   460,   461,   464,   472,
     476,  -821,  -821,   865,  -821,   867,   873,  -821,  1135,  -821,
    -821,   879,  -821,  -821,  -821,  -821,  -821,  1136,  1137,  1138,
    -821,  -821,  -821,  1152,  3292,  1155,  -821,  2482,  1159,   655,
    -821,  -821,  1160,  1212,  1217,  -821,   881,  -821,  3292,  -821,
    -821,  1031,  -821,  -821,   673,  1172,  1173,   -13,  -821,   885,
     987,     6,     6,     6,  -821,  3292,  -821,  -821,  -821,  1182,
    -821,    69,  -821,  -821,  -821,   527,   186,  -821,   356,  3292,
    3292,  3292,  3292,  3292,  3292,  3292,  3292,  -821,     6,  1171,
    -821,  -821,     6,  1176,  3292,  -821,   304,   322,   140,   232,
     774,  1177,  1179,  1266,  -821,  -821,  1031,  1031,  1288,  1298,
    1299,  3292,  2926,  1198,  1202,  -821,   477,   509,   510,   513,
     521,   531,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821,  -821,  1185,  -821,
    -821,  -821,  -821,  -821,  3292,   890,  -821,  -821,  -821,  3292,
    3292,  2482,   677,  -821,  -821,  3292,  -821,  -821,  -821,  -821,
       6,     6,  -821,  -821,  -821,  -821,  -821,  3292,  1207,  1301,
    1302,  3292,  3292,  3292,  -821,  -821,  -821,   535,   537,   539,
     540,   541,   545,   549,   555,  -821,  -821,  -821,  3292,  1187,
    -821,    57,  1247,  -821,   106,  -821,  1306,  1309,  1204,  3292,
    -821,  1310,  1311,  1206,  3292,  -821,   269,   364,  1208,  1209,
    -821,  -821,   351,  -821,  -821,  -821,  -821,  -821,   563,  1195,
     892,  3292,  3292,  -821,  -821,  -821,  -821,  -821,  -821,  -821,
    1210,  1213,  -821,  -821,   895,  -821,   679,   997,  -821,  -821,
    3292,  -821,  -821,   566,   569,   577,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  1214,     6,  1197,   900,  1215,  1479,
    1197,   908,  1218,  1219,  -821,  1220,  1221,  1225,  -821,  1226,
    1318,  1322,  1227,  3292,  -821,  1326,  1327,  1228,  3292,  -821,
    -821,  -821,   221,  -821,  -821,  -821,  1224,   580,   583,  -821,
    -821,  1237,  -821,     6,  -821,  -821,  -821,  -821,  -821,  3292,
    -821,  -821,  1197,  -821,  3000,  3073,  1698,  -821,   681,  -821,
    1197,  -821,  -821,  -821,  -821,  -821,  -821,  1238,  1257,  -821,
    1258,  1267,  1268,  -821,  1269,  1197,   911,  -821,  -821,  -821,
    -821,  -821,  1230,  -821,   660,   213,  -821,   390,  3292,  3292,
    3292,  3292,  3292,  3292,  3292,  3292,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  1197,     6,  3292,  3292,  3292,
    -821,  -821,  -821,   591,   594,   609,   610,   612,   613,   615,
     641,  -821,  -821,   642,   643,   646,  -821,  -821,  -821,  -821,
    -821,  -821,  -821,  -821,  -821,  -821,  -821
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   413,   414,   415,   417,   418,   423,
     424,   438,   437,   439,   440,     0,   425,   426,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   416,     0,     0,     0,     0,     0,    93,     0,
       3,     4,    94,    95,   102,   103,   104,   105,    96,    97,
     112,     0,   113,   114,   115,   101,     0,   318,   320,   338,
     340,   343,   345,   348,   350,   352,   355,   360,   365,   367,
     381,   409,     0,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     418,   423,   424,   438,     0,   445,   363,     0,   418,   423,
      35,     0,   410,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,   446,   317,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,     0,   141,     0,
       0,   106,     0,     0,   107,     0,     0,     0,     3,     0,
     407,    98,   100,   404,   205,     0,     0,     0,   364,     0,
     419,   420,   362,   361,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,   313,   314,   315,   316,   310,     0,   387,     0,     0,
     311,     0,     0,     0,     0,     0,   392,     0,   391,   437,
     383,     0,     0,     0,     0,     0,     0,    30,     0,     0,
       0,     0,     0,     0,     0,   441,     0,     0,    38,   412,
       0,     6,     0,     9,     0,    10,     0,     0,     0,     0,
       0,     0,   368,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    42,     0,     0,
     119,     0,   131,     0,     0,   150,     0,     0,     0,     3,
     157,    45,     0,     0,    46,     0,     0,   153,     0,     0,
       0,     0,     0,   134,     0,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,   406,   408,     0,     0,
     208,     0,     0,     0,   243,   442,     0,   276,   418,   423,
     424,   438,     0,     0,     0,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,   324,   327,   328,
     321,   322,     0,   325,   326,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   339,   341,   342,   344,   346,   347,
     349,   351,   353,   354,   359,   356,   357,   358,   366,   370,
     380,   374,   375,   376,   377,   378,   379,   372,   373,   371,
     403,     0,   400,    53,     0,    87,    88,    89,    90,    91,
      50,     0,     0,   386,     0,     0,    15,     0,     0,     0,
       0,   428,   433,   434,    55,     0,   395,     0,     0,     0,
     384,     0,   382,   412,     0,     0,     0,   312,   313,   314,
     315,   316,   310,   311,    37,     0,   411,     0,     0,     0,
     415,   437,     0,     0,     0,     0,    27,     0,    28,     0,
      29,     0,   160,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,   122,   442,     0,     0,   346,   347,   255,
     256,     0,   269,   271,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,   442,
       0,     0,     0,     0,   108,   109,   110,   111,   309,    99,
       0,     0,   208,     0,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
       0,     0,   303,     0,     0,   304,     0,     0,     0,     0,
       0,     0,     0,     0,   277,    57,    59,    61,    64,    62,
      63,    58,    60,    70,    72,    74,    77,    75,    76,    71,
      73,     0,     0,   396,     0,     0,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,   390,     0,     0,   247,     0,     0,     0,     0,   428,
     433,   434,    36,     5,     0,     0,    11,     0,     0,     0,
       0,    20,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   369,     0,    78,    81,    80,    84,    82,
      83,    79,    40,     0,    85,    86,    43,   116,   121,   120,
       0,     0,   130,   147,     0,   149,     0,     0,     0,   258,
       0,     0,   156,     0,     0,     0,     0,     0,   168,     0,
      48,    49,    32,    31,     0,     0,   151,   435,   436,     0,
       0,   132,   135,   136,   139,   140,     0,     0,   206,   207,
       0,     0,     0,     0,   204,   165,     0,     0,     0,     0,
       0,     0,     0,   242,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   286,   293,   300,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,   402,     0,   399,     0,     0,    52,     0,    54,
     166,     0,    17,    16,   431,   430,   429,     0,     0,     0,
     393,   388,   389,     0,     0,     0,   385,     0,     0,     0,
     252,   251,     0,     0,     0,    22,     0,    23,     0,    26,
     159,     0,    34,    33,     0,   444,   444,     0,   123,     0,
     124,     0,     0,     0,   265,     0,   259,   260,   155,   261,
     257,     0,   270,   272,   274,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
      68,    69,     0,     0,     0,   203,     0,     0,     0,     0,
       0,     0,     0,     0,   164,   162,     0,     0,     0,     0,
       0,     0,     0,   428,   434,   297,     0,     0,     0,     0,
       0,     0,   305,   306,   307,   308,   278,   280,   282,   285,
     283,   284,   279,   281,   401,   398,   397,    51,   444,    65,
      66,    67,   248,   249,     0,     0,    13,    12,    21,     0,
       0,     0,     0,   158,     7,     0,    41,    44,   118,   117,
     442,     0,   128,   148,   145,   146,   267,     0,   262,     0,
       0,     0,     0,     0,   172,   173,   170,     0,     0,     0,
       0,     0,     0,     0,     0,   154,   152,   133,     0,     0,
     144,     0,     3,   238,     0,   235,     0,     0,     0,     0,
     214,     0,     0,     0,     0,   209,     0,     0,     0,     0,
     210,   211,     0,   161,   163,   244,   245,   246,     0,   167,
       0,     0,     0,   287,   289,   292,   290,   291,   288,    18,
       0,   444,   254,   253,     0,    24,     0,   125,   126,   266,
       0,   263,   264,     0,     0,     0,   174,   176,   178,   181,
     179,   180,   175,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   225,     0,     0,     0,   226,     0,
       0,     0,     0,     0,   216,     0,     0,     0,     0,   215,
     212,   213,     0,   241,   296,   295,   166,     0,     0,   250,
      14,   444,     8,     0,   129,   268,   182,   184,   183,     0,
     143,   237,     0,   186,     0,     0,     0,   185,     0,   234,
       0,   223,   224,   218,   221,   222,   217,     0,     0,   227,
       0,     0,     0,   228,     0,     0,     0,   294,   298,   299,
      25,   127,     0,   236,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   233,   231,   232,
     220,   229,   230,   219,   240,     0,     0,     0,     0,     0,
     190,   191,   188,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   142,     0,     0,     0,   192,   194,   196,   199,
     197,   198,   193,   195,   200,   202,   201
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -821,  -821,     3,  -821,  -505,  -662,  -821,  -821,  -821,  -821,
    -821,   372,  -821,  -821,  -821,  -821,  -821,  -252,  -820,  -821,
    -821,  -821,   861,  -821,  -821,  -445,   913,  -347,   331,  1003,
      -2,  -210,  -821,  1232,  1194,  -821,   627,   -11,  1184,  1191,
     820,    -8,  -821,  -821,  -237,  -821,  -821,   -55,  -821
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   187,    61,    62,   513,    63,    64,    65,    66,
      67,   929,    68,    69,   532,    70,    71,   320,   933,    72,
      73,   349,   553,    74,   558,   262,   483,   526,   115,   166,
     122,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   293,   430,    89,    90,   123,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,   432,   392,     2,   555,   798,   668,   116,   626,   141,
     147,   550,   622,    24,   148,   140,   676,   935,   628,   551,
     132,   134,   136,   138,   139,   519,   520,   144,   146,   521,
     149,   519,   520,   153,   155,   521,   783,   492,   103,   694,
     695,   170,   178,   352,   173,   784,   159,   248,   182,   183,
     679,   519,   520,   228,   263,   521,   104,   185,   519,   520,
     711,   712,   521,   160,   713,   157,   277,   519,   520,   321,
      60,   521,   105,   711,   712,   305,   737,   713,     3,   908,
     522,   523,   524,   720,   721,   722,   522,   523,   524,   107,
     106,   353,   306,   240,   241,   242,   243,   244,   245,   223,
     224,   250,   252,   265,   254,   255,   522,   523,   524,   117,
     680,   681,   274,   522,   523,   524,   519,   520,    60,   276,
     521,   266,   522,   523,   524,   328,    60,   627,   552,   294,
     264,   304,  1023,   309,   696,   311,   714,    60,   902,   909,
     910,   723,   525,   329,   649,   161,   677,   150,   554,   835,
     332,   330,   334,   305,   936,   339,   313,   340,   341,   755,
     342,   343,   937,   344,   345,   346,   344,   347,   648,   331,
     306,   522,   523,   524,   307,   717,  1041,   308,   165,   256,
    1049,   736,   257,   228,   996,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   176,   378,   379,   380,   381,   382,
     383,   384,   385,   103,   314,   336,   454,    60,   938,   718,
     719,   177,   305,   855,   318,   408,   409,   162,   174,   319,
     337,   562,  1073,   258,   414,   415,   416,   417,   418,   306,
    1087,   519,   520,  1000,   163,   521,   124,   125,   126,   305,
     127,   570,   544,   573,   175,  1094,   941,   444,   505,   862,
     151,   939,   307,   509,   942,   308,   306,   128,   571,   516,
     574,   129,   464,   105,   466,   940,   467,   468,   469,   470,
     471,   472,   473,   682,   441,  1111,   179,   476,   477,   458,
     479,   563,   484,  1010,   230,   231,   522,   523,   524,   442,
     322,  1011,   898,   498,   499,   500,   501,   502,   503,   504,
     943,   560,   715,   507,   508,   517,   518,   323,   290,   519,
     520,   307,   291,   521,   308,  1034,   164,   851,   915,   674,
     528,   529,   527,   530,   531,   639,   108,   534,   109,   536,
     280,   491,   852,    75,   540,   541,   319,  1012,   307,   282,
     161,   308,   164,   944,   281,  1101,   180,   572,  1065,   575,
     809,   810,   130,   283,   811,   181,   322,   945,   142,   143,
     565,   566,   567,   569,   522,   523,   524,   279,   156,   158,
      60,   225,   108,   323,   109,   171,   172,   545,  1015,   606,
    1013,   546,   322,   547,   863,   445,  1016,   609,   864,   324,
     865,   751,   612,   186,  1014,   615,   790,   226,   227,   323,
     620,   446,   623,   284,   447,   812,   813,   814,   355,   356,
     316,   317,     4,     5,     6,     7,   322,   285,   635,   118,
     119,   112,   113,   259,    13,    14,    15,    16,    17,    18,
     931,   808,  1017,   323,   636,   932,   228,   637,   485,   607,
     114,    28,   654,   611,   236,   142,   613,   614,   934,   228,
      33,   663,   486,   932,   487,   324,   624,   489,    41,    42,
      43,   476,   738,    46,   844,   845,   229,   310,   488,   312,
     315,   490,    50,    51,   837,  1018,   634,  1022,    52,   237,
     185,   324,   932,   644,   333,   646,   335,   647,   916,  1019,
     238,   650,   651,   593,   206,   207,   208,   209,   278,   576,
     866,   279,   577,   578,   210,   211,   212,    54,    55,   579,
     671,   641,   580,   581,   642,   324,   582,   583,    75,   239,
     729,    56,  1102,   296,   730,   689,   213,   214,    57,   232,
     731,    93,    94,   732,   733,    59,   260,   734,   246,   893,
      60,   707,   261,   233,   234,   235,   108,   753,   109,   756,
     253,   594,   450,   911,   451,   761,   296,   912,   867,   102,
     726,   727,   728,   298,   433,   434,   299,   300,   739,   740,
     913,   741,   742,   264,   743,   744,   745,   746,   747,   748,
     749,   750,   779,   595,   997,   596,   597,  1001,   662,   598,
     868,   279,   869,   870,   953,   954,   871,   599,   205,   267,
     666,   600,   655,   279,   872,   584,   952,   819,   873,   963,
     820,   821,   443,   279,   767,   768,   769,   822,   289,   302,
     823,   824,   303,   773,   825,   826,   735,   215,   216,   217,
     218,   219,   220,   778,   657,   658,   780,   781,   659,   221,
     292,   964,   965,   786,   295,   966,   660,   296,   297,   222,
     794,   321,   302,   967,   298,   303,   661,   299,   300,   914,
     593,   301,   594,   968,   595,   596,   597,   986,   799,   987,
     598,   988,   989,   990,   599,  1066,   804,   991,   325,   549,
     600,   992,   326,   815,   816,   818,  1097,   993,   759,   296,
    1098,   655,   828,   327,   657,  1024,   298,   348,  1036,   299,
     300,  1037,   661,  1099,   833,   879,   205,  1078,   881,  1038,
    1079,  1080,  1068,   827,   351,  1069,   593,  1081,   350,   594,
    1082,  1083,   885,  1116,  1084,  1085,  1117,   856,   857,   858,
     859,   860,   861,   892,   595,   596,   763,   597,   598,   279,
     599,  1118,  1119,   302,  1120,  1121,   303,  1122,     4,     5,
       6,     7,   286,   287,   288,   118,   119,   112,   113,   259,
      13,    14,    15,    16,    17,    18,   600,   655,   657,   448,
     449,   661,   883,  1123,  1124,  1125,   114,    28,  1126,   787,
     887,   354,   788,   279,   946,   302,    33,   947,   303,   948,
     949,   450,  1100,   629,    41,    42,    43,   960,   894,    46,
     357,   895,   975,   906,  1032,   279,   205,   279,    50,    51,
     401,   402,   403,  1086,    52,   460,   461,   917,   918,   919,
     920,   921,   922,   923,   924,    93,    94,   268,   269,   270,
     271,   272,   930,   279,   452,   766,   974,   450,   273,   853,
     976,   667,   669,    54,    55,   672,   673,   675,   435,   958,
     405,   406,   407,   102,   511,   512,   838,    56,    75,   839,
     840,   841,   842,   453,    57,   462,   463,   455,   701,   702,
     703,    59,   621,   475,   279,   436,    60,   437,   261,   438,
     188,   439,   970,   189,   190,   495,   496,   972,   973,   456,
     191,   355,   497,   192,   193,   355,   506,   194,   195,   510,
     356,   440,   514,   356,   197,   979,   454,   198,   199,   983,
     984,   985,   537,   356,   200,   539,   356,   201,   202,   355,
     610,   203,   204,   683,   684,   457,   994,   716,   684,   724,
     279,   758,   279,   760,   279,   999,   459,  1005,     4,     5,
       6,     7,  1009,   770,   279,   118,   119,   112,   113,   259,
      13,    14,    15,    16,    17,    18,   771,   461,   465,  1027,
    1028,   772,   279,   776,   461,   474,   114,    28,   355,   777,
     782,   642,   791,   684,   795,   279,    33,   493,  1035,   196,
     796,   279,   797,   512,    41,    42,    43,   846,   684,    46,
     847,   684,   874,   279,   875,   461,   515,  1048,    50,    51,
     876,   279,   800,   205,    52,   494,   878,   279,   355,   891,
     533,  1060,   900,   356,   901,   512,  1064,   971,   279,  1026,
     279,   419,  1031,   279,  1033,   512,   829,  1042,   684,   420,
     535,   421,   422,    54,    55,  1050,   684,   930,  1095,   684,
     423,   424,  1074,  1075,  1077,   167,   169,    56,   425,   426,
     412,   413,   538,   542,    57,   543,   427,   428,   429,   544,
     548,    59,   754,   545,   546,   836,    60,   547,   261,   556,
     564,   557,   585,   586,   587,   588,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,  1110,   222,   589,   590,   591,   592,   593,
     594,     4,     5,     6,     7,  1113,  1114,  1115,   118,   119,
     112,   113,   259,    13,    14,    15,    16,    17,    18,   595,
     596,   601,   228,   597,   598,   599,   600,   603,   602,   114,
      28,   616,   617,   618,   604,   625,   631,   630,   899,    33,
     632,   633,   903,   904,   905,   638,   653,    41,    42,    43,
     643,   652,    46,   655,   656,   419,   657,   658,   431,   659,
     678,    50,    51,   420,   660,   421,   422,    52,   661,   925,
     664,   665,   319,   927,   423,   424,   690,   691,   697,   692,
     693,   699,   425,   426,   704,   705,   706,   708,   698,   700,
     427,   428,   429,   709,   725,   757,    54,    55,    92,   759,
      93,    94,    95,    96,    97,    98,    99,   762,   764,   765,
      56,   774,   775,   100,   785,   789,   792,    57,   793,   801,
     802,   803,   805,   806,    59,   807,   101,   832,   102,    60,
     830,   261,   831,   834,   843,   848,     4,     5,     6,     7,
     849,   977,   978,   358,   359,   360,   361,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   362,   363,    24,
     850,   854,    25,    26,    27,    28,   364,   365,    31,    32,
     877,   879,   880,   881,    33,    34,    35,    36,    37,    38,
     366,   367,    41,    42,    43,    44,    45,    46,    47,   882,
     419,   884,    48,    49,   886,   888,    50,    51,   420,   889,
     421,   422,    52,   907,   890,   955,    53,   896,   897,   423,
     424,   928,   950,   926,   951,   956,   957,   425,   426,   961,
     969,   981,   982,   962,   995,   427,   428,   429,   980,   998,
    1002,    54,    55,  1003,  1006,  1007,  1040,  1025,   932,  1004,
      75,  1008,  1057,  1020,  1021,    56,  1058,  1029,  1030,  1039,
    1061,  1062,    57,  1051,  1052,  1053,  1054,  1043,    58,    59,
    1055,  1056,  1059,  1063,    60,   368,  1067,  1096,     4,     5,
       6,     7,  1070,  1088,  1071,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   685,
     686,    24,  1089,  1090,    25,    26,    27,    28,    29,   687,
      31,    32,  1091,  1092,  1093,   640,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,  1072,   410,   710,    48,    49,   404,     0,    50,    51,
     411,     0,     0,     0,    52,     0,     0,  1112,    53,    92,
       0,    93,    94,    95,    96,    97,    98,    99,   386,   387,
     388,   389,   390,   391,   100,   393,   394,   395,   396,   397,
     398,   399,   400,    54,    55,     0,     0,   561,     0,   102,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
      58,    59,     4,     5,     6,     7,    60,   688,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,  1044,  1045,    24,     0,     0,    25,    26,
      27,    28,    29,  1046,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,    53,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,    58,    59,    46,     0,     0,     0,
      60,  1047,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   145,    59,    46,
       0,     0,     0,    60,   568,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
     145,    59,    46,     0,     0,     0,    60,   817,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,   145,    59,     4,     5,     6,     7,    60,
    1076,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,    26,    27,    28,    29,    30,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,    53,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,    58,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
     120,   121,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,   145,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   184,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   247,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,   251,
       0,    41,    42,    43,     0,    59,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,   338,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   275,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,   186,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,    59,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   559,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   605,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   608,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,   480,     7,     0,     0,     0,   118,   119,
     112,   113,   481,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   619,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   184,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   752,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   959,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   131,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   133,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   135,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   137,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   152,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   154,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,   168,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
     249,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,   480,     7,     0,     0,     0,
     118,   119,   112,   113,   481,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   478,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   482,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   645,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   110,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,     0,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   670,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -445,
    -445,     0,     0,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,    59,     0,     0,  -445,  -445,
      60,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,  -445,
    -445,  -445,  -445,  -445,     0,     0,     0,  -445,     0,     0,
       0,     0,     0,     0,     0,  -445,  -445,  -445,  -445,     0,
    -445,     0,     0,  -445,  -445,  -445,  -445,  -445,  -445,     0,
    -445,  -445,  -445,     0,  -445
};

static const yytype_int16 yycheck[] =
{
       2,   238,   212,     0,   351,   667,   511,    15,    14,    14,
      10,    14,   457,    26,    14,    26,    80,   837,   463,    22,
      22,    23,    24,    25,    26,    10,    11,    29,    30,    14,
      32,    10,    11,    35,    36,    14,     5,   289,   111,    37,
      38,    43,    50,    14,    46,    14,    26,   102,    56,    57,
      10,    10,    11,   117,   109,    14,   129,    59,    10,    11,
      56,    57,    14,    43,    60,   126,   121,    10,    11,   125,
     131,    14,   113,    56,    57,    26,   132,    60,     0,    10,
      65,    66,    67,    74,    75,    76,    65,    66,    67,   111,
     131,    62,    43,    95,    96,    97,    98,    99,   100,   115,
     116,   103,   104,   113,   106,   107,    65,    66,    67,    14,
      70,    71,   111,    65,    66,    67,    10,    11,   131,   121,
      14,   131,    65,    66,    67,   113,   131,   133,   131,   131,
     129,   133,   952,   135,   132,   137,   132,   131,   800,    70,
      71,   132,   127,   131,   491,   125,    80,    12,   127,   132,
     152,   113,   154,    26,    14,   157,    80,   159,   160,   604,
     162,   163,    22,   165,   166,   167,   168,   169,   127,   131,
      43,    65,    66,    67,   125,   127,   996,   128,   131,    14,
    1000,   132,    17,   117,   127,   187,   188,   189,   190,   191,
     192,   193,   194,   195,    14,   197,   198,   199,   200,   201,
     202,   203,   204,   111,   128,    29,   125,   131,    68,   556,
     557,    31,    26,   132,   126,   226,   227,    26,    14,   131,
      44,   129,  1042,    58,   232,   233,   234,   235,   236,    43,
    1050,    10,    11,   127,    43,    14,    10,    11,    12,    26,
      14,    26,   125,    26,    14,  1065,    14,   249,   303,   132,
      12,   111,   125,   308,    22,   128,    43,    31,    43,   314,
      43,    35,   264,   113,   266,   125,   268,   269,   270,   271,
     272,   273,   274,   525,   111,  1095,    14,   279,   280,    77,
     282,   131,   284,    14,   118,   119,    65,    66,    67,   126,
      26,    22,   797,   295,   296,   297,   298,   299,   300,   301,
      68,   356,   554,   305,   306,   316,   317,    43,    10,    10,
      11,   125,    14,    14,   128,   977,   125,   111,   132,    80,
     322,   323,   319,   325,   326,    77,   124,   329,   126,   331,
     111,   126,   126,     2,   336,   337,   131,    68,   125,   111,
     125,   128,   125,   111,   125,   132,    10,   132,   127,   132,
      10,    11,   126,   125,    14,    10,    26,   125,    27,    28,
     362,   363,   364,   365,    65,    66,    67,   128,    37,    38,
     131,   105,   124,    43,   126,    44,    45,   125,    14,   434,
     111,   125,    26,   125,   132,   109,    22,   442,   132,   125,
     132,   601,   447,    72,   125,   450,   648,    78,    79,    43,
     455,   125,   457,   111,   128,    65,    66,    67,   127,   128,
      78,    79,     3,     4,     5,     6,    26,   125,   109,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     126,   683,    68,    43,   125,   131,   117,   128,   111,   441,
      31,    32,   497,   445,   110,   114,   448,   449,   126,   117,
      41,   506,   125,   131,   111,   125,   458,   111,    49,    50,
      51,   463,   132,    54,   716,   717,   108,   136,   125,   138,
     139,   125,    63,    64,   711,   111,   478,   126,    69,   124,
     482,   125,   131,   485,   153,   487,   155,   489,   132,   125,
     124,   493,   494,   125,    81,    82,    83,    84,   125,    26,
     132,   128,    29,    30,    91,    92,    93,    98,    99,    36,
     512,   125,    39,    40,   128,   125,    43,    44,   187,    14,
      26,   112,   132,    29,    30,   527,   113,   114,   119,   106,
      36,    98,    99,    39,    40,   126,   127,    43,    14,   791,
     131,   543,   133,   120,   121,   122,   124,   602,   126,   604,
      14,   125,   128,    26,   130,   610,    29,    30,   132,   126,
     562,   563,   564,    36,   125,   126,    39,    40,   570,   571,
      43,   573,   574,   129,   576,   577,   578,   579,   580,   581,
     582,   583,   637,   125,   931,   125,   125,   934,   125,   125,
     132,   128,   132,   132,   846,   847,   132,   125,   125,   125,
     125,   125,   125,   128,   132,   132,   843,    26,   132,   132,
      29,    30,   127,   128,   616,   617,   618,    36,    14,   125,
      39,    40,   128,   625,    43,    44,   132,    85,    86,    87,
      88,    89,    90,   635,   125,   125,   638,   639,   125,    97,
      10,   132,   132,   645,    26,   132,   125,    29,    30,   107,
     652,   125,   125,   132,    36,   128,   125,    39,    40,   132,
     125,    43,   125,   132,   125,   125,   125,   132,   670,   132,
     125,   132,   132,   132,   125,  1022,   678,   132,   111,   348,
     125,   132,   111,   685,   686,   687,    26,   132,   125,    29,
      30,   125,   694,   131,   125,   132,    36,    53,   132,    39,
      40,   132,   125,    43,   706,   125,   125,    26,   125,   132,
      29,    30,   132,   132,   126,   132,   125,    36,   131,   125,
      39,    40,   777,   132,    43,    44,   132,   729,   730,   731,
     732,   733,   734,   788,   125,   125,   125,   125,   125,   128,
     125,   132,   132,   125,   132,   132,   128,   132,     3,     4,
       5,     6,    10,    11,    12,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   125,   125,   125,   118,
     119,   125,   774,   132,   132,   132,    31,    32,   132,   125,
     125,   131,   128,   128,    10,   125,    41,    13,   128,    15,
      16,   128,   132,   130,    49,    50,    51,   852,   125,    54,
     132,   128,   125,   805,   125,   128,   125,   128,    63,    64,
       7,     8,     9,   132,    69,   127,   128,   819,   820,   821,
     822,   823,   824,   825,   826,    98,    99,   100,   101,   102,
     103,   104,   834,   128,   114,   130,   891,   128,   111,   130,
     895,   510,   511,    98,    99,   514,   515,   516,   125,   851,
     223,   224,   225,   126,    27,    28,    10,   112,   527,    13,
      14,    15,    16,   132,   119,   127,   128,   126,   537,   538,
     539,   126,   127,   127,   128,   125,   131,   125,   133,   125,
      26,   125,   884,    29,    30,   127,   128,   889,   890,    14,
      36,   127,   128,    39,    40,   127,   128,    43,    44,   127,
     128,   125,   127,   128,    26,   907,   125,    29,    30,   911,
     912,   913,   127,   128,    36,   127,   128,    39,    40,   127,
     128,    43,    44,   127,   128,   126,   928,   127,   128,   127,
     128,   127,   128,   127,   128,   932,    14,   939,     3,     4,
       5,     6,   944,   127,   128,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   127,   128,    14,   961,
     962,   127,   128,   127,   128,   125,    31,    32,   127,   128,
     127,   128,   127,   128,   127,   128,    41,   111,   980,   125,
     127,   128,    27,    28,    49,    50,    51,   127,   128,    54,
     127,   128,   127,   128,   127,   128,    10,   999,    63,    64,
     127,   128,   671,   125,    69,   111,   127,   128,   127,   128,
      14,  1013,   127,   128,    27,    28,  1018,   127,   128,   127,
     128,    14,   127,   128,    27,    28,   695,   127,   128,    22,
      14,    24,    25,    98,    99,   127,   128,  1039,   127,   128,
      33,    34,  1044,  1045,  1046,    42,    43,   112,    41,    42,
     230,   231,    27,    10,   119,   125,    49,    50,    51,   125,
     132,   126,   127,   125,   125,    58,   131,   125,   133,   126,
     111,   126,   125,   125,   125,   125,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,   107,   125,   125,   125,   125,   125,
     125,     3,     4,     5,     6,  1097,  1098,  1099,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   125,
     125,    94,   117,   125,   125,   125,   125,    14,   126,    31,
      32,   111,   111,   111,   126,   126,   132,   114,   797,    41,
     125,   125,   801,   802,   803,    77,    10,    49,    50,    51,
     125,   111,    54,   125,   125,    14,   125,   125,    17,   125,
     111,    63,    64,    22,   125,    24,    25,    69,   125,   828,
     125,   125,   131,   832,    33,    34,   125,   125,   114,   125,
     125,   114,    41,    42,   125,   125,   111,    14,   132,   132,
      49,    50,    51,    14,    14,   125,    98,    99,    96,   125,
      98,    99,   100,   101,   102,   103,   104,   125,   130,   130,
     112,    77,    14,   111,   125,   125,   125,   119,   125,    10,
      10,    10,   111,    10,   126,    10,   124,    27,   126,   131,
     125,   133,   125,   125,    56,    14,     3,     4,     5,     6,
      14,   900,   901,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      14,   132,    29,    30,    31,    32,    33,    34,    35,    36,
     125,   125,   125,   125,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,   127,
      14,   126,    59,    60,   125,   125,    63,    64,    22,    77,
      24,    25,    69,   111,    77,     7,    73,   125,   125,    33,
      34,   125,   125,   132,   125,     7,     7,    41,    42,   111,
     125,    10,    10,   111,   127,    49,    50,    51,   111,    72,
      14,    98,    99,    14,    14,    14,   995,   132,   131,   125,
     999,   125,    14,   125,   125,   112,    14,   127,   125,   125,
      14,    14,   119,   125,   125,   125,   125,   132,   125,   126,
     125,   125,   125,   125,   131,   132,   132,   127,     3,     4,
       5,     6,   125,   125,  1033,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   125,   125,    29,    30,    31,    32,    33,    34,
      35,    36,   125,   125,   125,   482,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,  1039,   228,   552,    59,    60,   222,    -1,    63,    64,
     229,    -1,    -1,    -1,    69,    -1,    -1,  1096,    73,    96,
      -1,    98,    99,   100,   101,   102,   103,   104,   206,   207,
     208,   209,   210,   211,   111,   213,   214,   215,   216,   217,
     218,   219,   220,    98,    99,    -1,    -1,   124,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,
     125,   126,     3,     4,     5,     6,   131,   132,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    73,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,   125,   126,    54,    -1,    -1,    -1,
     131,   132,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    49,    50,    51,   125,   126,    54,
      -1,    -1,    -1,   131,   132,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,
     125,   126,    54,    -1,    -1,    -1,   131,   132,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    -1,    -1,   125,   126,     3,     4,     5,     6,   131,
     132,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    73,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    49,    50,    51,   125,   126,    54,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,
     125,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      49,    50,    51,   125,   126,    54,    -1,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    49,    50,    51,   126,   127,    54,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,
      51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,   120,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    23,
      98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,   127,
      54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    72,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,   127,
      54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,
      50,    51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    41,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    49,    50,    51,   126,   127,    54,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    41,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,    51,
     126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    41,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    49,    50,    51,   126,   127,    54,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,   127,
      54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,
      50,    51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,    54,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,
      -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,
     126,    54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,
      50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    78,    79,
     131,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,   122,    -1,   124
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   135,   136,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    69,    73,    98,    99,   112,   119,   125,   126,
     131,   137,   138,   140,   141,   142,   143,   144,   146,   147,
     149,   150,   153,   154,   157,   162,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   179,
     180,   182,    96,    98,    99,   100,   101,   102,   103,   104,
     111,   124,   126,   111,   129,   113,   131,   111,   124,   126,
      10,    11,    12,    13,    31,   162,   175,    14,    10,    11,
     125,   126,   164,   181,    10,    11,    12,    14,    31,    35,
     126,   126,   164,   126,   164,   126,   164,   126,   164,   164,
     171,    14,   162,   162,   164,   125,   164,    10,    14,   164,
      12,    12,   126,   164,   126,   164,   162,   126,   162,    26,
      43,   125,    26,    43,   125,   131,   163,   163,   131,   163,
     164,   162,   162,   164,    14,    14,    14,    31,   175,    14,
      10,    10,   175,   175,   127,   164,    72,   136,    26,    29,
      30,    36,    39,    40,    43,    44,   125,    26,    29,    30,
      36,    39,    40,    43,    44,   125,    81,    82,    83,    84,
      91,    92,    93,   113,   114,    85,    86,    87,    88,    89,
      90,    97,   107,   115,   116,   105,    78,    79,   117,   108,
     118,   119,   106,   120,   121,   122,   110,   124,   124,    14,
     164,   164,   164,   164,   164,   164,    14,   127,   181,   126,
     164,   120,   164,    14,   164,   164,    14,    17,    58,    14,
     127,   133,   159,   181,   129,   113,   131,   125,   100,   101,
     102,   103,   104,   111,   111,   127,   164,   181,   125,   128,
     111,   125,   111,   125,   111,   125,    10,    11,    12,    14,
      10,    14,    10,   177,   164,    26,    29,    30,    36,    39,
      40,    43,   125,   128,   164,    26,    43,   125,   128,   164,
     162,   164,   162,    80,   128,   162,    78,    79,   126,   131,
     151,   125,    26,    43,   125,   111,   111,   131,   113,   131,
     113,   131,   164,   162,   164,   162,    29,    44,    23,   164,
     164,   164,   164,   164,   164,   164,   164,   164,    53,   155,
     131,   126,    14,    62,   131,   127,   128,   132,    10,    11,
      12,    13,    24,    25,    33,    34,    47,    48,   132,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   167,   167,   167,   167,
     167,   167,   165,   167,   167,   167,   167,   167,   167,   167,
     167,     7,     8,     9,   168,   170,   170,   170,   171,   171,
     172,   173,   174,   174,   175,   175,   175,   175,   175,    14,
      22,    24,    25,    33,    34,    41,    42,    49,    50,    51,
     178,    17,   178,   125,   126,   125,   125,   125,   125,   125,
     125,   111,   126,   127,   164,   109,   125,   128,   118,   119,
     128,   130,   114,   132,   125,   126,    14,   126,    77,    14,
     127,   128,   127,   128,   164,    14,   164,   164,   164,   164,
     164,   164,   164,   164,   125,   127,   164,   164,   126,   164,
       5,    14,   126,   160,   164,   111,   125,   111,   125,   111,
     125,   126,   151,   111,   111,   127,   128,   128,   164,   164,
     164,   164,   164,   164,   164,   181,   128,   164,   164,   181,
     127,    27,    28,   139,   127,    10,   181,   171,   171,    10,
      11,    14,    65,    66,    67,   127,   161,   136,   164,   164,
     164,   164,   148,    14,   164,    14,   164,   127,    27,   127,
     164,   164,    10,   125,   125,   125,   125,   125,   132,   162,
      14,    22,   131,   156,   127,   161,   126,   126,   158,   127,
     181,   124,   129,   131,   111,   164,   164,   164,   132,   164,
      26,    43,   132,    26,    43,   132,    26,    29,    30,    36,
      39,    40,    43,    44,   132,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,    94,   126,    14,   126,   127,   181,   164,   127,   181,
     128,   164,   181,   164,   164,   181,   111,   111,   111,   127,
     181,   127,   159,   181,   164,   126,    14,   133,   159,   130,
     114,   132,   125,   125,   164,   109,   125,   128,    77,    77,
     160,   125,   128,   125,   164,   126,   164,   164,   127,   161,
     164,   164,   111,    10,   181,   125,   125,   125,   125,   125,
     125,   125,   125,   181,   125,   125,   125,   162,   138,   162,
     126,   164,   162,   162,    80,   162,    80,    80,   111,    10,
      70,    71,   151,   127,   128,    24,    25,    34,   132,   164,
     125,   125,   125,   125,    37,    38,   132,   114,   132,   114,
     132,   162,   162,   162,   125,   125,   111,   164,    14,    14,
     156,    56,    57,    60,   132,   151,   127,   127,   161,   161,
      74,    75,    76,   132,   127,    14,   164,   164,   164,    26,
      30,    36,    39,    40,    43,   132,   132,   132,   132,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   165,   127,   181,   127,   159,   181,   125,   127,   125,
     127,   181,   125,   125,   130,   130,   130,   164,   164,   164,
     127,   127,   127,   164,    77,    14,   127,   128,   164,   181,
     164,   164,   127,     5,    14,   125,   164,   125,   128,   125,
     151,   127,   125,   125,   164,   127,   127,    27,   139,   164,
     162,    10,    10,    10,   164,   111,    10,    10,   151,    10,
      11,    14,    65,    66,    67,   164,   164,   132,   164,    26,
      29,    30,    36,    39,    40,    43,    44,   132,   164,   162,
     125,   125,    27,   164,   125,   132,    58,   178,    10,    13,
      14,    15,    16,    56,   151,   151,   127,   127,    14,    14,
      14,   111,   126,   130,   132,   132,   164,   164,   164,   164,
     164,   164,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   127,   127,   127,   125,   127,   125,
     125,   125,   127,   164,   126,   181,   125,   125,   125,    77,
      77,   128,   181,   151,   125,   128,   125,   125,   138,   162,
     127,    27,   139,   162,   162,   162,   164,   111,    10,    70,
      71,    26,    30,    43,   132,   132,   132,   164,   164,   164,
     164,   164,   164,   164,   164,   162,   132,   162,   125,   145,
     164,   126,   131,   152,   126,   152,    14,    22,    68,   111,
     125,    14,    22,    68,   111,   125,    10,    13,    15,    16,
     125,   125,   178,   151,   151,     7,     7,     7,   164,   127,
     181,   111,   111,   132,   132,   132,   132,   132,   132,   125,
     164,   127,   164,   164,   181,   125,   181,   162,   162,   164,
     111,    10,    10,   164,   164,   164,   132,   132,   132,   132,
     132,   132,   132,   132,   164,   127,   127,   161,    72,   136,
     127,   161,    14,    14,   125,   164,    14,    14,   125,   164,
      14,    22,    68,   111,   125,    14,    22,    68,   111,   125,
     125,   125,   126,   152,   132,   132,   127,   164,   164,   127,
     125,   127,   125,    27,   139,   164,   132,   132,   132,   125,
     162,   152,   127,   132,    24,    25,    34,   132,   164,   152,
     127,   125,   125,   125,   125,   125,   125,    14,    14,   125,
     164,    14,    14,   125,   164,   127,   161,   132,   132,   132,
     125,   162,   145,   152,   164,   164,   132,   164,    26,    29,
      30,    36,    39,    40,    43,    44,   132,   152,   125,   125,
     125,   125,   125,   125,   152,   127,   127,    26,    30,    43,
     132,   132,   132,   164,   164,   164,   164,   164,   164,   164,
     164,   152,   162,   164,   164,   164,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   134,   135,   136,   136,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   138,   138,   138,   138,
     138,   138,   138,   138,   139,   139,   139,   139,   139,   139,
     140,   140,   141,   141,   141,   141,   142,   142,   143,   143,
     143,   144,   144,   144,   145,   146,   146,   146,   146,   146,
     146,   147,   147,   148,   148,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   150,   150,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   153,   154,   155,   155,   155,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   158,   158,   158,   158,   159,   159,   159,
     159,   160,   160,   160,   160,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   163,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   165,
     165,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   168,   168,   168,   169,   169,   170,   170,   170,   171,
     171,   172,   172,   173,   173,   173,   174,   174,   174,   174,
     174,   175,   175,   175,   175,   175,   176,   176,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     181,   181,   181,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     4,     6,     6,     8,     5,
       5,     7,     6,     6,     8,    10,     6,     4,     4,     4,
       3,     5,     5,     6,     6,     2,     5,     4,     3,     3,
       5,     7,     3,     5,     7,     3,     3,     2,     5,     5,
       4,     7,     6,     4,     6,     4,     2,     4,     4,     4,
       4,     4,     4,     4,     4,     7,     7,     7,     6,     6,
       4,     4,     4,     4,     4,     4,     4,     4,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     4,     4,     4,
       4,     4,     2,     1,     1,     1,     1,     1,     2,     4,
       2,     1,     1,     1,     1,     1,     2,     2,     4,     4,
       4,     4,     1,     1,     1,     1,     5,     7,     7,     3,
       5,     5,     4,     6,     3,     5,     5,     7,     4,     6,
       5,     3,     5,     7,     3,     5,     5,     3,     2,     5,
       5,     2,    12,     9,     1,     7,     7,     5,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     3,     7,     6,
       4,     7,     6,     7,     6,     5,     6,     5,     3,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     3,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     6,     5,     0,     3,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     4,     7,     6,     4,     8,
       7,     5,     5,     0,     4,     4,     4,     3,     5,     5,
       7,     3,     3,     5,     5,     1,     1,     3,     2,     3,
       3,     3,     4,     5,     5,     3,     5,     4,     6,     1,
       3,     1,     3,     1,     3,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
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
#line 395 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2883 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 405 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2889 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 406 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2895 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 410 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2901 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 412 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2907 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 414 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2913 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 416 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2921 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 420 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2927 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 422 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2933 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 424 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2939 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 426 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2947 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 430 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2954 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 433 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2962 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 437 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2968 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 439 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2976 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 443 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2983 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 446 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2991 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 450 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2997 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 452 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3003 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 454 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3009 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 456 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3015 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 458 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3021 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 460 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3029 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 464 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3037 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 468 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3043 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 470 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3049 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 472 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3055 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3061 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 476 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3067 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 478 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3073 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 480 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3079 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 482 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3085 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 484 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3091 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 486 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3097 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 488 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3103 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 490 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3109 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 492 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3115 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 494 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3121 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 496 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3128 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 499 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3135 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 502 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3141 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 504 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3148 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3155 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 510 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3161 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 512 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3167 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 514 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3173 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 516 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3180 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 519 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3187 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 522 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3193 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 524 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3202 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 529 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3210 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 533 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3218 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 537 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3227 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 542 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3235 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall ';'  */
#line 545 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3241 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3247 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 549 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3253 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 551 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3259 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 553 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3265 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 555 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3272 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 558 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3278 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 560 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3284 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 562 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3290 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 564 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3297 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3304 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 570 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3311 "raku.tab.c"
    break;

  case 68: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 573 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3318 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 576 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3325 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_IF expr ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3331 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNLESS expr ';'  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3337 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_WHILE expr ';'  */
#line 583 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3343 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_UNTIL expr ';'  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3349 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_FOR expr ';'  */
#line 587 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3356 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_WITH expr ';'  */
#line 590 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3362 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITHOUT expr ';'  */
#line 592 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3368 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_GIVEN expr ';'  */
#line 594 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3374 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 596 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3381 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 599 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3388 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 602 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3396 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 606 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3403 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 609 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3409 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 611 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3415 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 613 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3421 "raku.tab.c"
    break;

  case 85: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 615 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3428 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 618 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3435 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 621 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3442 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 624 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3449 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 627 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3456 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 630 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3463 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 633 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3470 "raku.tab.c"
    break;

  case 92: /* stmt: expr ';'  */
#line 635 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3476 "raku.tab.c"
    break;

  case 93: /* stmt: ';'  */
#line 636 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3482 "raku.tab.c"
    break;

  case 94: /* stmt: if_stmt  */
#line 637 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3488 "raku.tab.c"
    break;

  case 95: /* stmt: while_stmt  */
#line 638 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3494 "raku.tab.c"
    break;

  case 96: /* stmt: for_stmt  */
#line 639 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3500 "raku.tab.c"
    break;

  case 97: /* stmt: given_stmt  */
#line 640 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3506 "raku.tab.c"
    break;

  case 98: /* stmt: KW_TRY block  */
#line 642 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3512 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block KW_CATCH block  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3518 "raku.tab.c"
    break;

  case 100: /* stmt: KW_CATCH block  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3524 "raku.tab.c"
    break;

  case 101: /* stmt: block  */
#line 648 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3530 "raku.tab.c"
    break;

  case 102: /* stmt: unless_stmt  */
#line 649 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3536 "raku.tab.c"
    break;

  case 103: /* stmt: until_stmt  */
#line 650 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3542 "raku.tab.c"
    break;

  case 104: /* stmt: repeat_stmt  */
#line 651 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3548 "raku.tab.c"
    break;

  case 105: /* stmt: loop_stmt  */
#line 652 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3554 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST ';'  */
#line 653 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3560 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT ';'  */
#line 654 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3566 "raku.tab.c"
    break;

  case 108: /* stmt: KW_LAST KW_IF expr ';'  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3572 "raku.tab.c"
    break;

  case 109: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 658 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3578 "raku.tab.c"
    break;

  case 110: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3584 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 662 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3590 "raku.tab.c"
    break;

  case 112: /* stmt: sub_decl  */
#line 663 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3596 "raku.tab.c"
    break;

  case 113: /* stmt: class_decl  */
#line 664 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3602 "raku.tab.c"
    break;

  case 114: /* stmt: role_decl  */
#line 665 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3608 "raku.tab.c"
    break;

  case 115: /* stmt: grammar_decl  */
#line 666 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3614 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3620 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3626 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3632 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3638 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3644 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3650 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF expr block elsif_tail  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3656 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 684 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3662 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3668 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3674 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3680 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 694 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3686 "raku.tab.c"
    break;

  case 128: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 696 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3692 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 698 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3698 "raku.tab.c"
    break;

  case 130: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 702 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3704 "raku.tab.c"
    break;

  case 131: /* while_stmt: KW_WHILE expr block  */
#line 704 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3710 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3716 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3722 "raku.tab.c"
    break;

  case 134: /* unless_stmt: KW_UNLESS expr block  */
#line 712 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3728 "raku.tab.c"
    break;

  case 135: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3734 "raku.tab.c"
    break;

  case 136: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3740 "raku.tab.c"
    break;

  case 137: /* until_stmt: KW_UNTIL expr block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3752 "raku.tab.c"
    break;

  case 139: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 726 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3758 "raku.tab.c"
    break;

  case 140: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3764 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP block  */
#line 732 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3770 "raku.tab.c"
    break;

  case 142: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 734 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3776 "raku.tab.c"
    break;

  case 143: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 736 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3782 "raku.tab.c"
    break;

  case 144: /* loop_incr: expr  */
#line 739 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3788 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 743 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3798 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 749 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3808 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 755 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3817 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 760 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3827 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 766 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3836 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr block  */
#line 771 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3843 "raku.tab.c"
    break;

  case 151: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 776 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3855 "raku.tab.c"
    break;

  case 152: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 784 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3868 "raku.tab.c"
    break;

  case 153: /* when_list: %empty  */
#line 794 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3874 "raku.tab.c"
    break;

  case 154: /* when_list: when_list KW_WHEN expr block  */
#line 796 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3882 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 802 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3894 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 810 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT sub_body  */
#line 816 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3914 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 822 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3926 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 830 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3936 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 836 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3946 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 842 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3959 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 851 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3970 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 858 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3983 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 867 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3996 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 876 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4007 "raku.tab.c"
    break;

  case 166: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 885 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4018 "raku.tab.c"
    break;

  case 167: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 892 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4027 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list '}'  */
#line 898 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4033 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr '}'  */
#line 900 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4040 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 903 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4047 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 906 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4053 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 908 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4059 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 910 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4065 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 912 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4071 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 914 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4077 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 916 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4083 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 918 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4089 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 920 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4096 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 923 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4102 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 925 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4108 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 927 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4114 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 929 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4121 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 932 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4128 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 935 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4136 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list '}'  */
#line 940 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4142 "raku.tab.c"
    break;

  case 186: /* method_body: '{' YADA '}'  */
#line 941 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4148 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr '}'  */
#line 943 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4155 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 946 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4162 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 949 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4168 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 951 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4174 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 953 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4180 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 955 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4186 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 957 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4192 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 959 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4198 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 961 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4204 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 963 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4211 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 966 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4217 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 968 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4223 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 970 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4229 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 972 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4236 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 975 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4243 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 978 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4251 "raku.tab.c"
    break;

  case 203: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 984 "raku.y"
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
#line 4269 "raku.tab.c"
    break;

  case 204: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1000 "raku.y"
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
#line 4286 "raku.tab.c"
    break;

  case 205: /* is_clauses: %empty  */
#line 1014 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4292 "raku.tab.c"
    break;

  case 206: /* is_clauses: is_clauses IDENT IDENT  */
#line 1016 "raku.y"
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
#line 4308 "raku.tab.c"
    break;

  case 207: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1028 "raku.y"
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
#line 4324 "raku.tab.c"
    break;

  case 208: /* class_body_list: %empty  */
#line 1041 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4330 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1043 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4337 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1046 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4344 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1049 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4351 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1052 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4358 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1055 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4365 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1058 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4372 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1061 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4379 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1064 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4386 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1067 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4393 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1070 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4400 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1073 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4407 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1076 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4414 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1079 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4425 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1086 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4436 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1093 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4447 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1100 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4458 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1107 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4466 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1111 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4474 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1115 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4482 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1119 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4490 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1123 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4501 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1130 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4512 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1137 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4523 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1144 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4534 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1151 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4548 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1161 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4560 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1169 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4572 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1177 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4585 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1186 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4596 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1193 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4607 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1200 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4621 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1210 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4633 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1218 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4645 "raku.tab.c"
    break;

  case 242: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1228 "raku.y"
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
#line 4662 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: %empty  */
#line 1242 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4668 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1244 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4677 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1249 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4686 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1254 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4695 "raku.tab.c"
    break;

  case 247: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1261 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4703 "raku.tab.c"
    break;

  case 248: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1265 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4711 "raku.tab.c"
    break;

  case 249: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1269 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4719 "raku.tab.c"
    break;

  case 250: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1273 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4727 "raku.tab.c"
    break;

  case 251: /* pair_list: IDENT OP_FATARROW expr  */
#line 1279 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4733 "raku.tab.c"
    break;

  case 252: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1281 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4739 "raku.tab.c"
    break;

  case 253: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1283 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4745 "raku.tab.c"
    break;

  case 254: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1285 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4751 "raku.tab.c"
    break;

  case 255: /* param_list: VAR_SCALAR  */
#line 1288 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4757 "raku.tab.c"
    break;

  case 256: /* param_list: VAR_ARRAY  */
#line 1289 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4763 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' VAR_ARRAY  */
#line 1290 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4769 "raku.tab.c"
    break;

  case 258: /* param_list: IDENT VAR_SCALAR  */
#line 1291 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4775 "raku.tab.c"
    break;

  case 259: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1292 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4781 "raku.tab.c"
    break;

  case 260: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1293 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4787 "raku.tab.c"
    break;

  case 261: /* param_list: param_list ',' VAR_SCALAR  */
#line 1294 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4793 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1295 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4799 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1296 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4805 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1297 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4811 "raku.tab.c"
    break;

  case 265: /* param_list: VAR_SCALAR '=' expr  */
#line 1298 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4817 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1299 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4823 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1300 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4829 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1301 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4835 "raku.tab.c"
    break;

  case 269: /* param_list: SLURPY_POS  */
#line 1302 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4841 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' SLURPY_POS  */
#line 1303 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4847 "raku.tab.c"
    break;

  case 271: /* param_list: SLURPY_LOL  */
#line 1304 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4853 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' SLURPY_LOL  */
#line 1305 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4859 "raku.tab.c"
    break;

  case 273: /* param_list: SLURPY_NAMED  */
#line 1306 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4865 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1307 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4871 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list '}'  */
#line 1310 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4877 "raku.tab.c"
    break;

  case 276: /* block: '{' YADA '}'  */
#line 1311 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4883 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list expr '}'  */
#line 1313 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4889 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1315 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4895 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1317 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4901 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1319 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4907 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1321 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4913 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1323 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4920 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4926 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1328 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4932 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1330 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4938 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1332 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4944 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1334 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4951 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1337 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4958 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1340 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4966 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1344 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4973 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1347 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4980 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1350 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4987 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1353 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4993 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1355 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5001 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1359 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5008 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1362 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5015 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1365 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5022 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1368 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5029 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1371 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5036 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1374 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5042 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1376 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5048 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1378 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5054 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_LAST '}'  */
#line 1380 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5060 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1382 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5066 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1384 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5072 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1386 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5078 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1388 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5084 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1390 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5090 "raku.tab.c"
    break;

  case 309: /* closure: '{' expr '}'  */
#line 1393 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5096 "raku.tab.c"
    break;

  case 310: /* expr: VAR_SCALAR '=' expr  */
#line 1396 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5102 "raku.tab.c"
    break;

  case 311: /* expr: VAR_ARRAY '=' expr  */
#line 1397 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5108 "raku.tab.c"
    break;

  case 312: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1398 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5114 "raku.tab.c"
    break;

  case 313: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1399 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5120 "raku.tab.c"
    break;

  case 314: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1400 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5126 "raku.tab.c"
    break;

  case 315: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1401 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5132 "raku.tab.c"
    break;

  case 316: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1402 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5138 "raku.tab.c"
    break;

  case 317: /* expr: KW_GATHER block  */
#line 1403 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5148 "raku.tab.c"
    break;

  case 318: /* expr: tern_expr  */
#line 1408 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5154 "raku.tab.c"
    break;

  case 319: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1412 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5160 "raku.tab.c"
    break;

  case 320: /* tern_expr: cmp_expr  */
#line 1413 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5166 "raku.tab.c"
    break;

  case 321: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1416 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5172 "raku.tab.c"
    break;

  case 322: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1417 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5178 "raku.tab.c"
    break;

  case 323: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1418 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5184 "raku.tab.c"
    break;

  case 324: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1419 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5190 "raku.tab.c"
    break;

  case 325: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1420 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5196 "raku.tab.c"
    break;

  case 326: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1421 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5202 "raku.tab.c"
    break;

  case 327: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1422 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 328: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1423 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5214 "raku.tab.c"
    break;

  case 329: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1424 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5220 "raku.tab.c"
    break;

  case 330: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1425 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5226 "raku.tab.c"
    break;

  case 331: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1426 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5232 "raku.tab.c"
    break;

  case 332: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1427 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5238 "raku.tab.c"
    break;

  case 333: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1428 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5244 "raku.tab.c"
    break;

  case 334: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1429 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5250 "raku.tab.c"
    break;

  case 335: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1431 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5260 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1437 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5270 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1443 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5280 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr  */
#line 1448 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5286 "raku.tab.c"
    break;

  case 339: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1451 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5292 "raku.tab.c"
    break;

  case 340: /* divis_expr: jct_expr  */
#line 1452 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5298 "raku.tab.c"
    break;

  case 341: /* jct_expr: jct_expr '|' range_expr  */
#line 1455 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5304 "raku.tab.c"
    break;

  case 342: /* jct_expr: jct_expr '&' range_expr  */
#line 1456 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5310 "raku.tab.c"
    break;

  case 343: /* jct_expr: dor_expr  */
#line 1457 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5316 "raku.tab.c"
    break;

  case 344: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1461 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5322 "raku.tab.c"
    break;

  case 345: /* dor_expr: range_expr  */
#line 1462 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5328 "raku.tab.c"
    break;

  case 346: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1465 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5334 "raku.tab.c"
    break;

  case 347: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1466 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5340 "raku.tab.c"
    break;

  case 348: /* range_expr: add_expr  */
#line 1467 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5346 "raku.tab.c"
    break;

  case 349: /* add_expr: add_expr '~' repl_expr  */
#line 1470 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5352 "raku.tab.c"
    break;

  case 350: /* add_expr: repl_expr  */
#line 1471 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5358 "raku.tab.c"
    break;

  case 351: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1474 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5364 "raku.tab.c"
    break;

  case 352: /* repl_expr: addsub_expr  */
#line 1475 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5370 "raku.tab.c"
    break;

  case 353: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1478 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5376 "raku.tab.c"
    break;

  case 354: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1479 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5382 "raku.tab.c"
    break;

  case 355: /* addsub_expr: mul_expr  */
#line 1480 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5388 "raku.tab.c"
    break;

  case 356: /* mul_expr: mul_expr '*' unary_expr  */
#line 1483 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5394 "raku.tab.c"
    break;

  case 357: /* mul_expr: mul_expr '/' unary_expr  */
#line 1484 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5400 "raku.tab.c"
    break;

  case 358: /* mul_expr: mul_expr '%' unary_expr  */
#line 1485 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5406 "raku.tab.c"
    break;

  case 359: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1486 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5412 "raku.tab.c"
    break;

  case 360: /* mul_expr: unary_expr  */
#line 1487 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5418 "raku.tab.c"
    break;

  case 361: /* unary_expr: '-' unary_expr  */
#line 1490 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5424 "raku.tab.c"
    break;

  case 362: /* unary_expr: '!' unary_expr  */
#line 1491 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5430 "raku.tab.c"
    break;

  case 363: /* unary_expr: CARET unary_expr  */
#line 1492 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5436 "raku.tab.c"
    break;

  case 364: /* unary_expr: OP_REDUCE unary_expr  */
#line 1494 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5445 "raku.tab.c"
    break;

  case 365: /* unary_expr: pow_expr  */
#line 1498 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5451 "raku.tab.c"
    break;

  case 366: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1501 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5457 "raku.tab.c"
    break;

  case 367: /* pow_expr: postfix_expr  */
#line 1502 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5463 "raku.tab.c"
    break;

  case 368: /* scalar_list: VAR_SCALAR  */
#line 1505 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5469 "raku.tab.c"
    break;

  case 369: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1506 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5475 "raku.tab.c"
    break;

  case 370: /* meth_name: IDENT  */
#line 1509 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5481 "raku.tab.c"
    break;

  case 371: /* meth_name: KW_SORT  */
#line 1510 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5487 "raku.tab.c"
    break;

  case 372: /* meth_name: KW_MAP  */
#line 1511 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5493 "raku.tab.c"
    break;

  case 373: /* meth_name: KW_GREP  */
#line 1512 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5499 "raku.tab.c"
    break;

  case 374: /* meth_name: KW_SAY  */
#line 1513 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5505 "raku.tab.c"
    break;

  case 375: /* meth_name: KW_PRINT  */
#line 1514 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5511 "raku.tab.c"
    break;

  case 376: /* meth_name: KW_TAKE  */
#line 1515 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5517 "raku.tab.c"
    break;

  case 377: /* meth_name: KW_RETURN  */
#line 1516 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5523 "raku.tab.c"
    break;

  case 378: /* meth_name: KW_EXISTS  */
#line 1517 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5529 "raku.tab.c"
    break;

  case 379: /* meth_name: KW_DELETE  */
#line 1518 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5535 "raku.tab.c"
    break;

  case 380: /* meth_name: TESTOP  */
#line 1519 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5541 "raku.tab.c"
    break;

  case 381: /* postfix_expr: call_expr  */
#line 1521 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5547 "raku.tab.c"
    break;

  case 382: /* call_expr: IDENT '(' arg_list ')'  */
#line 1524 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5556 "raku.tab.c"
    break;

  case 383: /* call_expr: IDENT '(' ')'  */
#line 1528 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5562 "raku.tab.c"
    break;

  case 384: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1530 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5568 "raku.tab.c"
    break;

  case 385: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1532 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5574 "raku.tab.c"
    break;

  case 386: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1534 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5583 "raku.tab.c"
    break;

  case 387: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1539 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5589 "raku.tab.c"
    break;

  case 388: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1541 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5599 "raku.tab.c"
    break;

  case 389: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1547 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5609 "raku.tab.c"
    break;

  case 390: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1553 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5617 "raku.tab.c"
    break;

  case 391: /* call_expr: IDENT '.' KW_NEW  */
#line 1557 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5625 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '.' IDENT  */
#line 1561 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5634 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5645 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1573 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5654 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '.' CARET IDENT  */
#line 1578 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5664 "raku.tab.c"
    break;

  case 396: /* call_expr: atom '.' CARET IDENT  */
#line 1584 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5674 "raku.tab.c"
    break;

  case 397: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1590 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5685 "raku.tab.c"
    break;

  case 398: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1597 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5696 "raku.tab.c"
    break;

  case 399: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1604 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5705 "raku.tab.c"
    break;

  case 400: /* call_expr: atom '.' meth_name  */
#line 1609 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5714 "raku.tab.c"
    break;

  case 401: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5725 "raku.tab.c"
    break;

  case 402: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1621 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5734 "raku.tab.c"
    break;

  case 403: /* call_expr: call_expr '.' meth_name  */
#line 1626 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5743 "raku.tab.c"
    break;

  case 404: /* call_expr: KW_DIE expr  */
#line 1631 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5749 "raku.tab.c"
    break;

  case 405: /* call_expr: KW_MAP closure expr  */
#line 1633 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5755 "raku.tab.c"
    break;

  case 406: /* call_expr: KW_GREP closure expr  */
#line 1635 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5761 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_SORT expr  */
#line 1637 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5767 "raku.tab.c"
    break;

  case 408: /* call_expr: KW_SORT closure expr  */
#line 1639 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5773 "raku.tab.c"
    break;

  case 409: /* call_expr: atom  */
#line 1640 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5779 "raku.tab.c"
    break;

  case 410: /* arg_list: expr  */
#line 1643 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5785 "raku.tab.c"
    break;

  case 411: /* arg_list: arg_list ',' expr  */
#line 1644 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5791 "raku.tab.c"
    break;

  case 412: /* arg_list: arg_list ','  */
#line 1645 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5797 "raku.tab.c"
    break;

  case 413: /* atom: LIT_INT  */
#line 1648 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5803 "raku.tab.c"
    break;

  case 414: /* atom: LIT_FLOAT  */
#line 1649 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5809 "raku.tab.c"
    break;

  case 415: /* atom: LIT_STR  */
#line 1650 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5815 "raku.tab.c"
    break;

  case 416: /* atom: WORDLIST  */
#line 1652 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5828 "raku.tab.c"
    break;

  case 417: /* atom: LIT_INTERP_STR  */
#line 1660 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5834 "raku.tab.c"
    break;

  case 418: /* atom: VAR_SCALAR  */
#line 1661 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5840 "raku.tab.c"
    break;

  case 419: /* atom: OP_INC VAR_SCALAR  */
#line 1662 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5846 "raku.tab.c"
    break;

  case 420: /* atom: OP_DEC VAR_SCALAR  */
#line 1663 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5852 "raku.tab.c"
    break;

  case 421: /* atom: VAR_SCALAR OP_INC  */
#line 1664 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5858 "raku.tab.c"
    break;

  case 422: /* atom: VAR_SCALAR OP_DEC  */
#line 1665 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5864 "raku.tab.c"
    break;

  case 423: /* atom: VAR_ARRAY  */
#line 1666 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5870 "raku.tab.c"
    break;

  case 424: /* atom: VAR_HASH  */
#line 1667 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5876 "raku.tab.c"
    break;

  case 425: /* atom: VAR_CAPTURE  */
#line 1669 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5884 "raku.tab.c"
    break;

  case 426: /* atom: VAR_FH  */
#line 1673 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5892 "raku.tab.c"
    break;

  case 427: /* atom: VAR_NAMED_CAPTURE  */
#line 1677 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5899 "raku.tab.c"
    break;

  case 428: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1680 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5905 "raku.tab.c"
    break;

  case 429: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1682 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5911 "raku.tab.c"
    break;

  case 430: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1684 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5917 "raku.tab.c"
    break;

  case 431: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1686 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5923 "raku.tab.c"
    break;

  case 432: /* atom: ARR_ALL_SLICE  */
#line 1688 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5929 "raku.tab.c"
    break;

  case 433: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1690 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5935 "raku.tab.c"
    break;

  case 434: /* atom: VAR_HASH '{' expr '}'  */
#line 1692 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5941 "raku.tab.c"
    break;

  case 435: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1694 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5947 "raku.tab.c"
    break;

  case 436: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1696 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5953 "raku.tab.c"
    break;

  case 437: /* atom: IDENT  */
#line 1697 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5959 "raku.tab.c"
    break;

  case 438: /* atom: VAR_TWIGIL  */
#line 1699 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5967 "raku.tab.c"
    break;

  case 439: /* atom: VAR_ARRAY_TWIGIL  */
#line 1703 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5975 "raku.tab.c"
    break;

  case 440: /* atom: VAR_HASH_TWIGIL  */
#line 1707 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5983 "raku.tab.c"
    break;

  case 441: /* atom: '(' ')'  */
#line 1710 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5989 "raku.tab.c"
    break;

  case 442: /* atom: '(' expr ')'  */
#line 1711 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5995 "raku.tab.c"
    break;

  case 443: /* atom: '(' expr ',' ')'  */
#line 1713 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6001 "raku.tab.c"
    break;

  case 444: /* atom: '(' expr ',' arg_list ')'  */
#line 1715 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6008 "raku.tab.c"
    break;

  case 445: /* atom: block  */
#line 1717 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6014 "raku.tab.c"
    break;

  case 446: /* atom: KW_SUB block  */
#line 1718 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6020 "raku.tab.c"
    break;


#line 6024 "raku.tab.c"

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

#line 1720 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
