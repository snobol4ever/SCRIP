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
#line 13 "raku.y"

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
/*--------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_call(const char *name) {
    tree_t *e = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(e, n);
    return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rk_multi_mangle(const char *base, ExprList *params) {
    static char buf[512]; int np = params ? params->count : 0;
    int pos = snprintf(buf, sizeof buf, "%s$%d", base, np);
    for (int i = 0; i < np; i++) { tree_t *p = params->items[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        char safe[64]; int j = 0;
        for (const char *c = ty; *c && j < 63; c++, j++) safe[j] = (*c == ':') ? '_' : *c; safe[j] = 0;
        pos += snprintf(buf + pos, sizeof buf - pos, "$%s", safe); }
    return intern(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_param(const char *type, const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_def_param(const char *type, const char *def, const char *name) {
    char buf[160]; snprintf(buf, sizeof buf, "%s%s", type, def);
    return rk_typed_param(intern(buf), name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_seq(ExprList *stmts) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
}/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_range_ex(tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return expr_binary(TT_TO, lo, d); }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_TO || rhs->n < 2) return rhs;
    tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]);
    return call;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_index(const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]);
        return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, idx); return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_pick(const char *arr, tree_t *i0, ExprList *rest) {
    tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(arr)); expr_add_child(call, i0);
    if (rest) { for (int i = 0; i < rest->count; i++) expr_add_child(call, rest->items[i]); exprlist_free(rest); }
    return call;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_end_index(const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_all(const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *lo = ast_node_new(TT_ILIT); lo->v.ival = 0;
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, lo); expr_add_child(call, rk_dec(el));
    return call;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_param_default(tree_t *p, tree_t *dflt) {
    return expr_binary(TT_ASSIGN, p, dflt);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_is_chain_cmp(tree_e k) {
    return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return expr_binary(TT_SEQ, left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/

#line 391 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 65,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 66,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 67,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 68,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 69,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 70,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 71,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 72,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 73,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 74,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 75,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 76,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 77,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 78,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 79,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 80,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 81,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 82,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 83,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 84,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 85,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 86,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 87,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 88,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 89,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 90,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 91,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 92,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 93,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 94,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 95,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 96,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 97,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 98,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 99,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 100,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 101,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 102,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 103,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 104,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 105,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 106,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 107,                   /* OP_POW  */
  YYSYMBOL_108_ = 108,                     /* '='  */
  YYSYMBOL_109_ = 109,                     /* '!'  */
  YYSYMBOL_110_ = 110,                     /* '<'  */
  YYSYMBOL_111_ = 111,                     /* '>'  */
  YYSYMBOL_112_ = 112,                     /* '|'  */
  YYSYMBOL_113_ = 113,                     /* '&'  */
  YYSYMBOL_114_ = 114,                     /* '~'  */
  YYSYMBOL_115_ = 115,                     /* '+'  */
  YYSYMBOL_116_ = 116,                     /* '-'  */
  YYSYMBOL_117_ = 117,                     /* '*'  */
  YYSYMBOL_118_ = 118,                     /* '/'  */
  YYSYMBOL_119_ = 119,                     /* '%'  */
  YYSYMBOL_UMINUS = 120,                   /* UMINUS  */
  YYSYMBOL_121_ = 121,                     /* '.'  */
  YYSYMBOL_122_ = 122,                     /* ';'  */
  YYSYMBOL_123_ = 123,                     /* '('  */
  YYSYMBOL_124_ = 124,                     /* ')'  */
  YYSYMBOL_125_ = 125,                     /* ','  */
  YYSYMBOL_126_ = 126,                     /* '['  */
  YYSYMBOL_127_ = 127,                     /* ']'  */
  YYSYMBOL_128_ = 128,                     /* '{'  */
  YYSYMBOL_129_ = 129,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 130,                 /* $accept  */
  YYSYMBOL_program = 131,                  /* program  */
  YYSYMBOL_stmt_list = 132,                /* stmt_list  */
  YYSYMBOL_stmt = 133,                     /* stmt  */
  YYSYMBOL_if_stmt = 134,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 135,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 136,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 137,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 138,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 139,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 140,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 141,                /* loop_incr  */
  YYSYMBOL_for_stmt = 142,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 143,               /* given_stmt  */
  YYSYMBOL_when_list = 144,                /* when_list  */
  YYSYMBOL_sub_decl = 145,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 146,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 147,                 /* sub_body  */
  YYSYMBOL_method_body = 148,              /* method_body  */
  YYSYMBOL_class_decl = 149,               /* class_decl  */
  YYSYMBOL_role_decl = 150,                /* role_decl  */
  YYSYMBOL_is_clauses = 151,               /* is_clauses  */
  YYSYMBOL_class_body_list = 152,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 153,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 154,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 155,           /* named_arg_list  */
  YYSYMBOL_pair_list = 156,                /* pair_list  */
  YYSYMBOL_param_list = 157,               /* param_list  */
  YYSYMBOL_block = 158,                    /* block  */
  YYSYMBOL_closure = 159,                  /* closure  */
  YYSYMBOL_expr = 160,                     /* expr  */
  YYSYMBOL_tern_expr = 161,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 162,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 163,               /* divis_expr  */
  YYSYMBOL_jct_expr = 164,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 165,                 /* dor_expr  */
  YYSYMBOL_range_expr = 166,               /* range_expr  */
  YYSYMBOL_add_expr = 167,                 /* add_expr  */
  YYSYMBOL_repl_expr = 168,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 169,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 170,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 171,               /* unary_expr  */
  YYSYMBOL_pow_expr = 172,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 173,              /* scalar_list  */
  YYSYMBOL_meth_name = 174,                /* meth_name  */
  YYSYMBOL_postfix_expr = 175,             /* postfix_expr  */
  YYSYMBOL_call_expr = 176,                /* call_expr  */
  YYSYMBOL_arg_list = 177,                 /* arg_list  */
  YYSYMBOL_atom = 178                      /* atom  */
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
#define YYLAST   3976

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1081

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   363


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
       2,     2,     2,   109,     2,     2,     2,   119,   113,     2,
     123,   124,   117,   115,   125,   116,   121,   118,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   122,
     110,   108,   111,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   128,   112,   129,   114,     2,     2,     2,
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
     105,   106,   107,   120
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   405,   405,   416,   417,   420,   422,   424,   426,   430,
     432,   434,   436,   440,   443,   447,   449,   453,   456,   460,
     462,   464,   466,   468,   470,   474,   478,   480,   482,   484,
     486,   488,   490,   492,   494,   496,   498,   500,   502,   504,
     506,   509,   512,   514,   517,   520,   522,   524,   526,   528,
     533,   537,   541,   546,   550,   551,   553,   555,   557,   559,
     562,   564,   566,   568,   571,   574,   577,   580,   583,   585,
     587,   589,   591,   594,   596,   598,   600,   603,   606,   610,
     613,   615,   617,   619,   622,   625,   628,   631,   634,   637,
     640,   641,   642,   643,   644,   645,   646,   648,   650,   652,
     654,   655,   656,   657,   658,   659,   660,   662,   664,   666,
     668,   669,   670,   671,   674,   676,   678,   680,   682,   684,
     686,   688,   692,   694,   696,   698,   700,   702,   706,   708,
     712,   714,   716,   718,   722,   724,   728,   730,   732,   736,
     738,   740,   744,   747,   753,   759,   764,   770,   775,   780,
     788,   799,   800,   806,   814,   820,   826,   834,   840,   846,
     855,   862,   871,   880,   889,   896,   903,   904,   907,   910,
     912,   914,   916,   918,   920,   922,   924,   927,   929,   931,
     933,   936,   939,   945,   946,   947,   950,   953,   955,   957,
     959,   961,   963,   965,   967,   970,   972,   974,   976,   979,
     982,   988,  1004,  1019,  1020,  1032,  1046,  1047,  1050,  1053,
    1056,  1059,  1062,  1065,  1068,  1071,  1074,  1077,  1080,  1083,
    1090,  1097,  1104,  1111,  1115,  1119,  1123,  1127,  1134,  1141,
    1148,  1155,  1165,  1173,  1181,  1190,  1197,  1204,  1214,  1222,
    1232,  1247,  1248,  1253,  1258,  1265,  1269,  1275,  1277,  1279,
    1281,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1297,  1298,  1299,  1301,  1303,  1305,  1307,  1309,  1312,
    1314,  1316,  1318,  1320,  1323,  1326,  1330,  1333,  1336,  1339,
    1341,  1345,  1348,  1351,  1354,  1357,  1360,  1362,  1364,  1366,
    1368,  1370,  1372,  1374,  1376,  1380,  1383,  1384,  1389,  1392,
    1394,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,
    1406,  1407,  1408,  1409,  1410,  1411,  1417,  1423,  1429,  1432,
    1433,  1436,  1437,  1438,  1441,  1443,  1446,  1447,  1448,  1451,
    1452,  1455,  1456,  1459,  1460,  1461,  1464,  1465,  1466,  1467,
    1468,  1471,  1472,  1473,  1474,  1479,  1482,  1483,  1486,  1487,
    1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,  1499,
    1500,  1502,  1504,  1509,  1510,  1515,  1517,  1523,  1529,  1533,
    1537,  1542,  1549,  1554,  1560,  1566,  1573,  1580,  1585,  1590,
    1597,  1602,  1607,  1609,  1611,  1613,  1615,  1617,  1620,  1621,
    1622,  1625,  1626,  1627,  1628,  1636,  1637,  1638,  1639,  1640,
    1641,  1642,  1643,  1644,  1648,  1652,  1655,  1657,  1659,  1661,
    1663,  1665,  1667,  1669,  1671,  1673,  1674,  1678,  1682,  1686,
    1687,  1688,  1690,  1693,  1694
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
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT", "OP_SGE",
  "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_DIVIS", "OP_REP_X",
  "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "'('", "')'",
  "','", "'['", "']'", "'{'", "'}'", "$accept", "program", "stmt_list",
  "stmt", "if_stmt", "elsif_tail", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "loop_stmt", "loop_incr", "for_stmt",
  "given_stmt", "when_list", "sub_decl", "scalar_methcall", "sub_body",
  "method_body", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "cmp_expr", "divis_expr", "jct_expr", "dor_expr", "range_expr",
  "add_expr", "repl_expr", "addsub_expr", "mul_expr", "unary_expr",
  "pow_expr", "scalar_list", "meth_name", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-785)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-424)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -785,    26,  1595,  -785,  -785,  -785,  -785,  -785,   461,   124,
     129,   -47,   211,  -785,  -785,  3848,  -785,  -785,  -785,    60,
    1665,    38,  2938,  3008,  3078,  3148,  3218,    15,   -20,  3218,
    1735,     4,  3218,   104,   130,  3288,  3358,   -20,   181,    25,
      33,    81,    81,  3428,   -20,   -20,  3218,   122,   212,    51,
    3848,  -785,  -785,   247,   296,   345,  3848,  3848,  -785,  1805,
     252,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785,   332,  -785,  -785,  -785,   597,   484,  -785,   641,   362,
     653,    76,  -785,   139,   272,   642,   291,  -785,  -785,   319,
     264,   340,   406,  -785,  -785,  3218,  3218,  3218,  3218,  3218,
    3218,   437,  1876,  3498,  1947,   477,  3218,  3218,    75,  2017,
     146,   358,   164,  -785,   -20,  -785,  -785,   377,   257,  -785,
    2088,  -785,   -38,    -5,   109,   235,   678,   511,   196,   506,
    1805,   466,  1805,    57,  1805,   -20,  1805,   -20,   -40,   162,
     360,  -785,  -785,   407,  -785,   416,   436,   521,   540,   167,
     168,  1805,   -20,  1805,   -20,    31,  2159,  -785,  3218,  3218,
    -785,  3218,  3218,  -785,  3218,  3218,  3218,  2229,  3218,  -785,
     512,  -785,  -785,  -785,   546,   510,    -6,  -785,   575,  -785,
    -785,  -785,  -785,  -785,   667,   594,  1042,  3218,  3218,  3218,
    3218,  3218,  3218,  3218,  3218,  -785,  3218,  3218,  3218,  3218,
    3218,  3218,  3218,  3218,  -785,  3848,  3848,  3848,  3848,  3848,
    3848,  3848,  3848,  3848,  3848,  3848,  3848,  3848,  3848,  3848,
     730,  3848,  3848,  3848,  3848,  3848,  3848,  3848,  3848,  3848,
    3848,  3848,  3848,  3848,  3848,  3848,   908,   904,   689,   605,
     628,   650,   652,   684,   696,   -37,  -785,   697,  1805,   157,
     710,   313,   712,   700,   709,   725,   850,   748,  -785,   703,
    1947,   863,  3218,  -785,  3218,   732,   667,   711,  -785,  3218,
    3218,  -785,  3568,  -785,  3638,  -785,   279,   289,   311,   502,
     782,   796,  -785,   714,   716,  3218,  3218,  3218,  3218,  3218,
    3218,  3218,  -785,  3218,   718,  3218,  3218,  -785,  3218,   726,
     825,   731,  -785,   897,  3218,  -785,  3848,  3848,    -1,  -785,
    -785,  -785,  -785,  3218,  3218,  -785,   896,  3218,   900,  3218,
     733,   890,   735,  -785,  3218,  3218,   913,   803,   805,   809,
     814,   829,   827,  -785,  -785,  -785,   -20,     3,  -785,     2,
     867,   876,  -785,  -785,  2299,  -785,   879,   290,   276,   865,
    2938,  3008,  3218,   783,    19,   185,  -785,   204,   881,   892,
     893,   894,   898,   899,   901,   902,   903,   905,   906,   916,
     918,   919,   920,   922,   925,   925,   925,   925,   925,   925,
     910,   925,   925,   925,   925,   925,   925,   925,   925,  -785,
    -785,  -785,   653,  -785,  -785,  -785,   912,   912,   272,   642,
     291,   291,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,   889,  1008,
     946,  -785,  2370,  -785,  -785,  -785,  -785,  -785,  -785,  3218,
    2441,  -785,   738,  3218,  -785,  3218,  3218,  3218,  3218,   962,
     971,   972,  -785,  2512,  -785,  2583,  -785,   428,   970,   953,
    -785,  -785,   976,  -785,   977,  1805,   278,  1026,   128,  2654,
      55,   981,  3218,  -785,  3708,  -785,  3218,  -785,    21,  -785,
    3218,  3218,   996,  1097,  2299,   987,   991,   998,  1007,  1009,
    1010,  1011,   292,  2299,  1012,  1013,   456,   -20,   -19,  3778,
    -785,   -20,   -20,    48,   -24,    61,  1022,    45,  1014,   745,
    1213,  1017,  1018,    40,  1025,  1015,  1030,  1016,   -20,   -20,
     -20,  1021,  1024,  1039,  3218,  -785,  -785,  -785,  -785,  -785,
    -785,  1134,  1135,  -785,    10,  1014,   749,    22,   427,     8,
    -785,   751,  1136,  1947,  3218,  3218,   299,   277,    52,  -785,
     142,  3218,  3218,  -785,  3218,  3218,  -785,  3218,  3218,  3218,
    3218,  3218,  3218,  3218,  3218,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  3848,  2725,  -785,  2796,  1031,   756,  1032,  -785,
     759,  2299,  1033,   491,  1029,  1034,   469,  3218,  3218,  3218,
    -785,   761,   131,  -785,   763,   770,  -785,  -785,  -785,  -785,
    -785,   772,  3218,  -785,  3218,  3218,  3218,   776,  -785,   231,
    -785,  1035,  1805,   634,  1037,  1014,   778,  1038,  1040,  3218,
    -785,   784,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
     795,  -785,  -785,  -785,   907,  -785,  -785,  1805,   -20,  -785,
    -785,  1142,  -785,  1153,  1156,  3218,  -785,  1157,  1158,  -785,
    1014,   507,  2938,  3008,  1404,  -785,   301,  -785,  -785,  3218,
     -20,  -785,  -785,  -785,  1047,  1050,  1146,  -785,  -785,  -785,
    -785,  3218,  1052,  -785,  -785,    54,   598,   457,  1119,  -785,
    -785,  1014,  1014,   815,   819,  1162,  1163,  1164,  -785,  -785,
     -17,   517,  1051,   144,  3218,  3218,  3218,  3218,  3218,  3218,
    -785,  -785,  -785,  -785,   179,   225,   303,   307,   375,   378,
     379,   389,   393,   397,   408,   412,  -785,  -785,   823,  -785,
     836,   838,  -785,  1057,  -785,  -785,   840,  -785,  -785,  -785,
    -785,  -785,  1059,  1060,  1061,  -785,  3218,  -785,  1170,  -785,
    2299,  1063,   683,  -785,  -785,  1064,  1113,  1114,  -785,   843,
    -785,  3218,  -785,  -785,  1014,  -785,  -785,   688,  1067,  1068,
     -19,  -785,   845,   954,   -20,   -20,   -20,  -785,  -785,  -785,
    -785,  1083,    53,   353,   293,  -785,   442,  3218,  3218,  3218,
    3218,  3218,  3218,  3218,  3218,  -785,   -20,  1065,  -785,  -785,
     -20,  1070,  3218,  -785,   518,   645,    50,    85,   754,  1071,
    1073,   908,  -785,  -785,  1014,  1014,  1189,  1190,  1191,  3218,
    2867,  1091,  1092,  -785,   445,   448,   451,   454,   468,   470,
    -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785,  -785,  -785,  -785,  -785,  -785,  1079,  -785,  -785,  -785,
    -785,  1128,   859,  -785,  -785,  -785,  3218,  3218,  2299,   694,
    -785,  -785,  3218,  -785,  -785,  -785,  -785,   -20,   -20,  -785,
    -785,  -785,  -785,  3218,  -785,  1194,  1195,  3218,  3218,  3218,
    -785,  -785,  -785,   476,   480,   485,   486,   489,   497,   499,
     505,  -785,  -785,  -785,  3218,  1082,  -785,    28,  1139,  -785,
      29,  -785,  1196,  1197,  1087,  3218,  -785,  1198,  1199,  1093,
    3218,  -785,   105,   194,  1098,  1099,  -785,  -785,   681,  -785,
    -785,  -785,  -785,  -785,   516,  1085,   861,  3218,  3218,  -785,
    -785,  -785,  -785,  -785,  -785,  -785,  3218,  1100,  -785,  -785,
     864,  -785,   695,   965,  -785,  -785,  -785,  -785,   532,   533,
     536,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  1118,
     -20,  1122,   870,  1123,  1334,  1122,   880,  1129,  1131,  -785,
    1147,  1148,  1149,  -785,  1152,  1261,  1264,  1159,  3218,  -785,
    1266,  1268,  1165,  3218,  -785,  -785,  -785,    44,  -785,  -785,
    -785,  1155,   537,   538,  -785,  -785,  1166,  -785,   -20,  -785,
    -785,  -785,  -785,  3218,  -785,  -785,  1122,  -785,  2938,  3008,
    1474,  -785,   330,  -785,  1122,  -785,  -785,  -785,  -785,  -785,
    -785,  1167,  1168,  -785,  1169,  1171,  1172,  -785,  1173,  1122,
     882,  -785,  -785,  -785,  -785,  -785,  1161,  -785,   450,   352,
    -785,   541,  3218,  3218,  3218,  3218,  3218,  3218,  3218,  3218,
    -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  1122,
     -20,  3218,  3218,  3218,  -785,  -785,  -785,   542,   576,   584,
     595,   603,   604,   606,   614,  -785,  -785,   620,   625,   626,
    -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,  -785,
    -785
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   391,   392,   393,   395,   396,   401,
     402,   416,   415,   417,   418,     0,   403,   404,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   410,   394,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   298,   300,   318,
     320,   323,   325,   328,   330,   332,   335,   340,   345,   347,
     361,   387,     0,   399,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,   401,   402,   416,     0,   423,   343,     0,   396,    35,
       0,   388,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
       0,   424,   297,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   139,     0,     0,
     104,     0,     0,   105,     0,     0,     0,     3,     0,   385,
      96,    98,   382,   203,     0,     0,     0,   344,     0,   397,
     398,   342,   341,   419,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   365,     0,     0,     0,
       0,     0,     0,     0,     0,   370,     0,   369,   363,     0,
       0,     0,     0,    30,     0,   419,     0,     0,    38,   390,
       0,     6,     0,     9,     0,    10,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    42,     0,     0,
     117,     0,   129,     0,     0,   148,     0,     0,     0,     3,
     155,    45,    46,     0,     0,   151,     0,     0,     0,     0,
       0,   132,     0,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,   384,   386,     0,     0,   206,     0,
       0,     0,   241,   420,     0,   262,   396,   401,   402,   416,
       0,     0,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   303,   304,   307,   308,   301,   302,
       0,   305,   306,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   319,   321,   322,   324,   326,   327,   329,   331,
     333,   334,   339,   336,   337,   338,   346,   350,   360,   354,
     355,   356,   357,   358,   359,   352,   353,   351,   381,     0,
     378,    51,     0,    85,    86,    87,    88,    89,    48,     0,
       0,   364,     0,     0,    15,     0,     0,     0,     0,   406,
     411,   412,    53,     0,   373,     0,   362,     0,     0,     0,
     296,    37,     0,   389,     0,     0,     0,   393,   415,     0,
       0,     0,     0,    27,     0,    28,     0,    29,     0,   158,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   420,     0,     0,
     120,   420,     0,     0,   326,   327,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   420,     0,
     420,     0,     0,     0,     0,   106,   107,   108,   109,   295,
      97,     0,     0,   206,     0,     0,     0,     0,     0,     0,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   288,
       0,     0,     0,   289,     0,     0,   290,     0,     0,     0,
       0,     0,     0,     0,     0,   263,    55,    57,    59,    62,
      60,    61,    56,    58,    68,    70,    72,    75,    73,    74,
      69,    71,     0,     0,   374,     0,     0,     0,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   415,   368,     0,     0,   406,   411,   412,    36,
       5,     0,     0,    11,     0,     0,     0,     0,    20,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,     0,    76,    79,    78,    82,    80,    81,    77,    40,
       0,    83,    84,    43,   114,   119,   118,     0,     0,   128,
     145,     0,   147,     0,     0,     0,   252,     0,     0,   154,
       0,     0,     0,     0,     0,   166,     0,    32,    31,     0,
       0,   149,   413,   414,     0,     0,   130,   133,   134,   137,
     138,     0,     0,   204,   205,     0,     0,     0,     0,   202,
     163,     0,     0,     0,     0,     0,     0,     0,   240,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     272,   279,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   299,   380,     0,   377,
       0,     0,    50,     0,    52,   164,     0,    17,    16,   409,
     408,   407,     0,     0,     0,   371,     0,   366,     0,   367,
       0,     0,     0,   248,   247,     0,     0,     0,    22,     0,
      23,     0,    26,   157,     0,    34,    33,     0,   422,   422,
       0,   121,     0,   122,     0,     0,     0,   259,   253,   254,
     153,   255,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,    66,    67,
       0,     0,     0,   201,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   160,     0,     0,     0,     0,     0,     0,
       0,   406,   412,   283,     0,     0,     0,     0,     0,     0,
     291,   292,   293,   294,   264,   266,   268,   271,   269,   270,
     265,   267,   379,   376,   375,    49,   422,    63,    64,    65,
     245,     0,     0,    13,    12,    21,     0,     0,     0,     0,
     156,     7,     0,    41,    44,   116,   115,   420,     0,   126,
     146,   143,   144,     0,   256,     0,     0,     0,     0,     0,
     170,   171,   168,     0,     0,     0,     0,     0,     0,     0,
       0,   152,   150,   131,     0,     0,   142,     0,     3,   236,
       0,   233,     0,     0,     0,     0,   212,     0,     0,     0,
       0,   207,     0,     0,     0,     0,   208,   209,     0,   159,
     161,   242,   243,   244,     0,   165,     0,     0,     0,   273,
     275,   278,   276,   277,   274,    18,     0,   422,   250,   249,
       0,    24,     0,   123,   124,   260,   257,   258,     0,     0,
       0,   172,   174,   176,   179,   177,   178,   173,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,   213,   210,   211,     0,   239,   282,
     281,   164,     0,     0,   246,    14,   422,     8,     0,   127,
     180,   182,   181,     0,   141,   235,     0,   184,     0,     0,
       0,   183,     0,   232,     0,   221,   222,   216,   219,   220,
     215,     0,     0,   225,     0,     0,     0,   226,     0,     0,
       0,   280,   284,   285,    25,   125,     0,   234,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,   231,   229,   230,   218,   227,   228,   217,   238,     0,
       0,     0,     0,     0,   188,   189,   186,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   140,     0,     0,     0,
     190,   192,   194,   197,   195,   196,   191,   193,   198,   200,
     199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -785,  -785,     1,  -785,  -485,  -628,  -785,  -785,  -785,  -785,
    -785,   304,  -785,  -785,  -785,  -785,  -785,  -269,  -784,  -785,
    -785,  -785,   769,  -785,  -785,   721,   839,  -324,   253,   966,
      -2,  -209,  -785,   909,  1078,  -785,   561,    -7,  1074,  1058,
     781,   -10,  -785,  -785,  -233,  -785,  -785,    20,  -785
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   186,    61,    62,   490,    63,    64,    65,    66,
      67,   885,    68,    69,   503,    70,    71,   310,   889,    72,
      73,   337,   524,    74,   529,   594,   460,   499,   115,   165,
     121,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   283,   418,    89,    90,   122,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,     2,   380,   635,   420,   116,   761,    24,   340,   496,
     469,   891,   496,   497,   146,   526,   497,   521,   147,   139,
     131,   133,   135,   137,   138,   522,     3,   143,   145,   140,
     148,   496,   496,   152,   154,   497,   497,   303,   496,   496,
     177,   169,   497,   497,   172,   541,   181,   182,   123,   124,
     125,   158,   126,   643,   496,   646,   341,   184,   497,   161,
     324,   107,   542,   864,   892,   175,   676,   677,   159,   127,
     678,   429,   893,   128,   117,   325,   162,   659,   660,   685,
     686,   687,   176,   295,   268,   304,   430,   269,    60,   255,
     227,   809,   256,   239,   240,   241,   242,   243,   244,   897,
     296,   249,   251,   270,   253,   254,   810,   898,    60,    60,
     676,   677,   647,   648,   678,   894,   149,   271,   266,   965,
     865,   866,   247,   498,   978,   641,   525,   966,   284,   259,
     294,   523,   299,   257,   301,   859,   173,   688,   644,   679,
     267,   160,   150,    60,   616,   615,   682,   160,   543,   320,
     899,   322,   951,   955,   327,   163,   328,   329,   895,   330,
     331,   129,   332,   333,   334,   332,   335,   995,  1019,   661,
     967,  1003,   896,   269,   311,   227,    60,   608,   224,   297,
     609,   702,   298,   793,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   900,   366,   367,   368,   369,   370,   371,
     372,   373,   606,   683,   684,   736,   280,   901,   970,   164,
     281,   544,  1027,   968,   225,   226,   971,   272,   396,   397,
    1041,   402,   403,   404,   405,   406,   174,   969,   545,   649,
     547,   273,   103,   548,   549,  1048,   746,   306,   307,   105,
     550,    93,    94,   551,   552,   747,   432,   553,   554,   108,
     104,   109,   108,   227,   109,    75,   680,   106,   447,   972,
     449,   178,   450,   433,   312,  1065,   442,   453,   454,   102,
     456,   703,   461,   813,   261,   855,   227,   316,   318,   434,
     141,   142,   435,   475,   476,   477,   478,   479,   480,   481,
     155,   157,   262,   484,   485,   317,   319,   170,   171,   494,
     495,   515,   973,   295,   156,   989,   179,   163,   820,    60,
     500,   501,   502,   482,   546,   505,   974,   507,   486,   295,
     296,   185,   511,   512,   493,   694,   204,   777,   286,   695,
     778,   779,   108,   555,   109,   696,   296,   780,   697,   698,
     781,   782,   699,   274,   783,   784,   753,   516,   536,   537,
     538,   540,    93,    94,   821,   180,  1032,   275,   187,  1033,
    1034,   188,   189,   716,   531,   264,  1035,   141,   190,  1036,
    1037,   191,   192,  1038,  1039,   193,   194,   228,   295,   867,
     102,   770,   286,   868,   602,   236,   105,   462,   300,   288,
     302,   305,   289,   290,   231,   296,   869,   464,   103,   297,
     603,   463,   298,   604,   534,   321,   701,   323,   232,   233,
     234,   465,   802,   803,   629,   297,   533,   269,   298,   466,
     238,   292,   871,   204,   293,   517,   235,   578,   700,   518,
     785,   582,   822,   467,   584,   585,   823,   496,   438,    75,
     439,   497,   577,   795,   214,   215,   216,   217,   218,   219,
     580,   245,   204,   601,   195,   583,   220,   184,   586,  1040,
     611,   237,   613,   591,   614,   595,   221,   796,   617,   618,
     797,   798,   799,   800,   297,   292,  1051,   298,   293,   286,
    1052,  1055,   870,   308,   260,   850,   288,   638,   309,   289,
     290,   252,   285,  1053,   621,   286,   287,   564,   656,   263,
     565,   566,   288,   630,   824,   289,   290,   825,   826,   291,
     196,   567,   672,   197,   198,   568,   282,   771,   827,   569,
     199,   772,   828,   200,   201,   279,   829,   202,   203,   311,
     570,   691,   692,   693,   571,   909,   910,   830,   312,   704,
     705,   831,   706,   707,   313,   708,   709,   710,   711,   712,
     713,   714,   715,   438,    92,   596,    93,    94,    95,    96,
      97,    98,    99,   952,   312,   336,   956,   622,   908,   100,
     624,   872,   292,   625,   919,   293,   626,   920,   633,  1054,
     921,   269,   101,   922,   102,   732,   733,   734,   292,   520,
     627,   293,   628,   718,   269,   721,   731,   923,   564,   924,
     741,   726,   565,   743,   744,   941,   204,   566,   567,   942,
     749,   568,   407,   728,   943,   944,   269,   757,   945,   569,
     408,   570,   409,   410,   742,   468,   946,   571,   947,   314,
     309,   411,   412,   339,   948,   762,  -423,  -423,   724,   413,
     414,   887,   438,   767,   811,   979,   888,   415,   416,   417,
     773,   774,   776,  1020,   622,   624,   794,   786,   628,   837,
     839,   990,   991,   312,   564,   992,  1022,  1023,   315,   791,
    1056,  1070,  -423,  -423,   338,  -423,  -423,  -423,  -423,  -423,
    -423,  -423,  -423,  -423,  -423,  -423,  -423,  -423,   276,   277,
     278,  -423,   814,   815,   816,   817,   818,   819,   565,  -423,
    -423,  -423,  -423,   342,  -423,  1071,   566,  -423,  -423,  -423,
    -423,  -423,  -423,  1072,  -423,  -423,  -423,   567,  -423,   205,
     206,   207,   208,   345,  1073,   568,   569,   423,   570,   209,
     210,   211,  1074,  1075,   840,  1076,   571,   389,   390,   391,
     634,   636,   622,  1077,   639,   640,   642,   624,   628,  1078,
     424,   212,   213,    75,  1079,  1080,   750,   229,   230,   751,
     842,   666,   667,   668,   902,   222,   223,   903,   890,   904,
     905,   849,   425,   888,   426,   873,   874,   875,   876,   877,
     878,   879,   880,   393,   394,   395,     4,     5,     6,     7,
     886,   343,   344,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,   977,   844,   427,   914,   269,   888,
     851,   421,   422,   852,   114,    28,   931,   987,   428,   269,
     269,   431,   269,   440,    33,   436,   437,   446,   269,   441,
     916,   442,    41,    42,    43,   452,   269,    46,   472,   473,
     343,   474,   343,   483,   928,   929,    50,    51,   443,    52,
     487,   344,   488,   489,   451,   491,   344,   508,   344,   510,
     344,   935,   343,   581,   444,   938,   939,   940,   930,   650,
     651,   445,   932,   681,   651,   689,   269,   448,    54,    55,
     723,   269,   949,   725,   269,   735,   269,   737,   738,   954,
     470,   763,    56,   960,   739,   269,   343,   740,   964,    57,
     745,   609,   754,   651,   471,   144,    59,   492,   758,   269,
     504,    60,   539,   787,   506,   982,   983,   509,   407,   759,
     269,   419,   407,   513,   984,   514,   408,   515,   409,   410,
     408,   516,   409,   410,   760,   489,   517,   411,   412,   804,
     651,   411,   412,   805,   651,   413,   414,   832,   269,   413,
     414,   518,  1002,   415,   416,   417,   519,   415,   416,   417,
     833,   738,   834,   269,   836,   269,  1014,   343,   848,   857,
     344,  1018,    92,   535,    93,    94,    95,    96,    97,    98,
      99,   858,   489,   927,   269,   981,   269,   100,   986,   269,
     527,   886,   988,   489,   996,   651,  1028,  1029,  1031,   528,
     532,   572,   102,   556,  1004,   651,  1049,   651,   166,   168,
     400,   401,   573,   856,   557,   558,   559,   860,   861,   862,
     560,   561,   574,   562,   563,   564,   227,   565,   566,   221,
    1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,   567,   881,
     568,   569,   570,   883,   571,     4,     5,     6,     7,  1067,
    1068,  1069,   346,   347,   348,   349,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   350,   351,    24,   575,
     587,    25,    26,    27,    28,   352,   353,    31,    32,   588,
     589,   597,   598,    33,    34,    35,    36,    37,    38,   354,
     355,    41,    42,    43,    44,    45,    46,    47,   599,   600,
     605,    48,    49,   610,   619,    50,    51,   620,    52,   622,
     933,   934,    53,   623,   374,   375,   376,   377,   378,   379,
     624,   381,   382,   383,   384,   385,   386,   387,   388,   625,
     645,   626,   627,   628,   631,   632,   662,    54,    55,   657,
     658,   664,   309,   669,   663,   665,   670,   671,   673,   674,
     690,    56,   764,   722,   724,   727,   729,   748,    57,   752,
     755,   730,   756,   765,    58,    59,   766,   768,   769,   788,
      60,   356,   789,   790,   792,   801,   806,   807,   808,   835,
     812,   837,   838,   839,   841,   843,   845,   846,   847,   853,
     854,   863,   884,   906,   882,   907,   911,   912,   913,   917,
     918,   925,   926,   994,   936,   937,   950,    75,   953,   959,
     957,   958,   961,   962,   980,   963,     4,     5,     6,     7,
     975,   976,   985,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   652,   653,    24,
     993,  1025,    25,    26,    27,    28,    29,   654,    31,    32,
     888,  1005,   997,  1006,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,  1007,
    1008,  1009,    48,    49,  1010,  1011,    50,    51,  1012,    52,
    1015,  1013,  1016,    53,  1021,  1050,   399,  1017,  1024,  1042,
    1043,  1044,   675,  1045,  1046,  1047,   720,  1026,   607,   392,
       0,   398,     0,  1066,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,    58,    59,     4,     5,     6,
       7,    60,   655,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   998,   999,
      24,     0,     0,    25,    26,    27,    28,    29,  1000,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,    50,    51,     0,
      52,     0,     0,     0,    53,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,    58,    59,    46,     0,
       0,     0,    60,  1001,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,   144,    59,    46,     0,
       0,     0,    60,   775,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,   144,    59,     4,     5,
       6,     7,    60,  1030,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,     0,    25,    26,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,    48,    49,     0,     0,    50,    51,
       0,    52,     0,     0,     0,    53,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,    58,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   119,   120,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   144,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   183,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     246,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,   250,     0,    41,    42,    43,     0,
      59,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   258,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,   326,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   265,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,   185,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   530,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   576,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   579,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,   592,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   590,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,   457,
       7,     0,     0,     0,   118,   111,   112,   113,   458,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   593,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   183,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
     592,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   717,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     719,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   915,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   130,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   132,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   134,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   136,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,    59,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   151,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   153,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,    59,    46,     0,     0,     0,   167,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   248,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,   457,     7,     0,     0,     0,   118,   111,
     112,   113,   458,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   455,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   459,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   612,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   110,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
       0,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   637,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,    60
};

static const yytype_int16 yycheck[] =
{
       2,     0,   211,   488,   237,    15,   634,    26,    14,    10,
     279,   795,    10,    14,    10,   339,    14,    14,    14,    26,
      22,    23,    24,    25,    26,    22,     0,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    77,    10,    10,
      50,    43,    14,    14,    46,    26,    56,    57,    10,    11,
      12,    26,    14,    77,    10,    10,    62,    59,    14,    26,
      29,   108,    43,    10,    14,    14,    56,    57,    43,    31,
      60,   108,    22,    35,    14,    44,    43,    37,    38,    71,
      72,    73,    31,    26,   122,   125,   123,   125,   128,    14,
     114,   108,    17,    95,    96,    97,    98,    99,   100,    14,
      43,   103,   104,   108,   106,   107,   123,    22,   128,   128,
      56,    57,    67,    68,    60,    65,    12,   122,   120,    14,
      67,    68,   102,   124,   908,    77,   124,    22,   130,   109,
     132,   128,   134,    58,   136,   763,    14,   129,    77,   129,
     120,   122,    12,   128,   468,   124,   124,   122,   129,   151,
      65,   153,   124,   124,   156,   122,   158,   159,   108,   161,
     162,   123,   164,   165,   166,   167,   168,   951,   124,   129,
      65,   955,   122,   125,   122,   114,   128,   122,   102,   122,
     125,   129,   125,   129,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   108,   196,   197,   198,   199,   200,   201,
     202,   203,    74,   527,   528,    74,    10,   122,    14,   128,
      14,    26,   996,   108,    75,    76,    22,   108,   225,   226,
    1004,   231,   232,   233,   234,   235,    14,   122,    43,   498,
      26,   122,   108,    29,    30,  1019,     5,    75,    76,   110,
      36,    95,    96,    39,    40,    14,   248,    43,    44,   121,
     126,   123,   121,   114,   123,     2,   525,   128,   260,    65,
     262,    14,   264,   106,   122,  1049,   122,   269,   270,   123,
     272,   129,   274,   129,   110,   760,   114,   110,   110,   122,
      27,    28,   125,   285,   286,   287,   288,   289,   290,   291,
      37,    38,   128,   295,   296,   128,   128,    44,    45,   306,
     307,   122,   108,    26,   123,   933,    10,   122,   129,   128,
     309,   313,   314,   293,   129,   317,   122,   319,   298,    26,
      43,    69,   324,   325,   304,    26,   122,    26,    29,    30,
      29,    30,   121,   129,   123,    36,    43,    36,    39,    40,
      39,    40,    43,   108,    43,    44,   615,   122,   350,   351,
     352,   353,    95,    96,   129,    10,    26,   122,    26,    29,
      30,    29,    30,   572,   344,   108,    36,   114,    36,    39,
      40,    39,    40,    43,    44,    43,    44,   105,    26,    26,
     123,   650,    29,    30,   106,   121,   110,   108,   135,    36,
     137,   138,    39,    40,   103,    43,    43,   108,   108,   122,
     122,   122,   125,   125,   128,   152,   129,   154,   117,   118,
     119,   122,   681,   682,   122,   122,   126,   125,   125,   108,
      14,   122,   129,   122,   125,   122,   107,   429,   129,   122,
     129,   433,   129,   122,   436,   437,   129,    10,   125,   186,
     127,    14,   422,   676,    82,    83,    84,    85,    86,    87,
     430,    14,   122,   455,   122,   435,    94,   459,   438,   129,
     462,   121,   464,   443,   466,   445,   104,    10,   470,   471,
      13,    14,    15,    16,   122,   122,    26,   125,   125,    29,
      30,   129,   129,   123,   126,   754,    36,   489,   128,    39,
      40,    14,    26,    43,   474,    29,    30,   122,   500,   122,
     122,   122,    36,   483,   129,    39,    40,   129,   129,    43,
      26,   122,   514,    29,    30,   122,    10,    10,   129,   122,
      36,    14,   129,    39,    40,    14,   129,    43,    44,   122,
     122,   533,   534,   535,   122,   804,   805,   129,   122,   541,
     542,   129,   544,   545,   108,   547,   548,   549,   550,   551,
     552,   553,   554,   125,    93,   127,    95,    96,    97,    98,
      99,   100,   101,   887,   122,    53,   890,   122,   801,   108,
     122,   129,   122,   122,   129,   125,   122,   129,   122,   129,
     129,   125,   121,   129,   123,   587,   588,   589,   122,   336,
     122,   125,   122,   573,   125,   575,   127,   129,   122,   129,
     602,   581,   122,   605,   606,   129,   122,   122,   122,   129,
     612,   122,    14,   122,   129,   129,   125,   619,   129,   122,
      22,   122,    24,    25,   604,   123,   129,   122,   129,   108,
     128,    33,    34,   123,   129,   637,    39,    40,   122,    41,
      42,   123,   125,   645,   127,   129,   128,    49,    50,    51,
     652,   653,   654,   977,   122,   122,    58,   659,   122,   122,
     122,   129,   129,   122,   122,   129,   129,   129,   128,   671,
     129,   129,    75,    76,   128,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    10,    11,
      12,    94,   694,   695,   696,   697,   698,   699,   122,   102,
     103,   104,   105,   128,   107,   129,   122,   110,   111,   112,
     113,   114,   115,   129,   117,   118,   119,   122,   121,    78,
      79,    80,    81,   129,   129,   122,   122,   122,   122,    88,
      89,    90,   129,   129,   736,   129,   122,     7,     8,     9,
     487,   488,   122,   129,   491,   492,   493,   122,   122,   129,
     122,   110,   111,   500,   129,   129,   122,   115,   116,   125,
     740,   508,   509,   510,    10,   112,   113,    13,   123,    15,
      16,   751,   122,   128,   122,   777,   778,   779,   780,   781,
     782,   783,   784,   222,   223,   224,     3,     4,     5,     6,
     792,   124,   125,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   123,   122,   122,   809,   125,   128,
     122,   122,   123,   125,    31,    32,   122,   122,   122,   125,
     125,   124,   125,   111,    41,   115,   116,   124,   125,   129,
     810,   122,    49,    50,    51,   124,   125,    54,   124,   125,
     124,   125,   124,   125,   846,   847,    63,    64,   123,    66,
     124,   125,    27,    28,   122,   124,   125,   124,   125,   124,
     125,   863,   124,   125,    14,   867,   868,   869,   848,   124,
     125,   123,   852,   124,   125,   124,   125,    14,    95,    96,
     124,   125,   884,   124,   125,   124,   125,   124,   125,   888,
     108,   638,   109,   895,   124,   125,   124,   125,   900,   116,
     124,   125,   124,   125,   108,   122,   123,    10,   124,   125,
      14,   128,   129,   660,    14,   917,   918,    27,    14,   124,
     125,    17,    14,    10,   926,   122,    22,   122,    24,    25,
      22,   122,    24,    25,    27,    28,   122,    33,    34,   124,
     125,    33,    34,   124,   125,    41,    42,   124,   125,    41,
      42,   122,   954,    49,    50,    51,   129,    49,    50,    51,
     124,   125,   124,   125,   124,   125,   968,   124,   125,   124,
     125,   973,    93,   108,    95,    96,    97,    98,    99,   100,
     101,    27,    28,   124,   125,   124,   125,   108,   124,   125,
     123,   993,    27,    28,   124,   125,   998,   999,  1000,   123,
     121,    91,   123,   122,   124,   125,   124,   125,    42,    43,
     229,   230,   123,   760,   122,   122,   122,   764,   765,   766,
     122,   122,    14,   122,   122,   122,   114,   122,   122,   104,
    1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,   122,   786,
     122,   122,   122,   790,   122,     3,     4,     5,     6,  1051,
    1052,  1053,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   123,
     108,    29,    30,    31,    32,    33,    34,    35,    36,   108,
     108,   111,   129,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   122,   122,
      74,    59,    60,   122,   108,    63,    64,    10,    66,   122,
     857,   858,    70,   122,   205,   206,   207,   208,   209,   210,
     122,   212,   213,   214,   215,   216,   217,   218,   219,   122,
     108,   122,   122,   122,   122,   122,   111,    95,    96,   122,
     122,   111,   128,   122,   129,   129,   122,   108,    14,    14,
      14,   109,    10,   122,   122,   122,   127,   122,   116,   122,
     122,   127,   122,    10,   122,   123,    10,    10,    10,   122,
     128,   129,   122,    27,   122,    56,    14,    14,    14,   122,
     129,   122,   122,   122,    14,   122,   122,    74,    74,   122,
     122,   108,   122,   122,   129,   122,     7,     7,     7,   108,
     108,   122,    74,   950,    10,    10,   124,   954,    69,   122,
      14,    14,    14,    14,   129,   122,     3,     4,     5,     6,
     122,   122,   122,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     122,   988,    29,    30,    31,    32,    33,    34,    35,    36,
     128,   122,   129,   122,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,   122,
     122,   122,    59,    60,   122,    14,    63,    64,    14,    66,
      14,   122,    14,    70,   129,   124,   228,   122,   122,   122,
     122,   122,   523,   122,   122,   122,   575,   993,   459,   221,
      -1,   227,    -1,  1050,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,   122,   123,     3,     4,     5,
       6,   128,   129,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    70,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    49,    50,    51,   122,   123,    54,    -1,
      -1,    -1,   128,   129,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    49,    50,    51,   122,   123,    54,    -1,
      -1,    -1,   128,   129,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,   122,   123,     3,     4,
       5,     6,   128,   129,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    70,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    49,    50,    51,   122,   123,    54,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    49,    50,    51,   122,   123,    54,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,
      -1,   116,    -1,    -1,    49,    50,    51,   122,   123,    54,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,   124,
      54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,
     124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,    -1,
      -1,    -1,    -1,   116,   117,    -1,    49,    50,    51,    -1,
     123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,
     123,   124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    23,    95,    96,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      41,    -1,    -1,    -1,   116,    -1,    -1,    -1,    49,    50,
      51,   123,   124,    54,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,
      51,    -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,
      51,    -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    41,    -1,    -1,    -1,   116,    -1,    -1,    -1,    49,
      50,    51,   123,   124,    54,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      49,    50,    51,   123,   124,    54,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    49,    50,    51,   123,   124,    54,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    49,    50,    51,   123,   124,    54,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    41,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    49,    50,    51,   123,   124,    54,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    41,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    49,    50,    51,   123,   124,    54,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,   124,
      54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,
     124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,
     123,   124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    41,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,    51,
      -1,   123,    54,    -1,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,   128
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   131,   132,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    66,    70,    95,    96,   109,   116,   122,   123,
     128,   133,   134,   136,   137,   138,   139,   140,   142,   143,
     145,   146,   149,   150,   153,   158,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   175,
     176,   178,    93,    95,    96,    97,    98,    99,   100,   101,
     108,   121,   123,   108,   126,   110,   128,   108,   121,   123,
      10,    11,    12,    13,    31,   158,   171,    14,    10,   122,
     123,   160,   177,    10,    11,    12,    14,    31,    35,   123,
     123,   160,   123,   160,   123,   160,   123,   160,   160,   167,
      14,   158,   158,   160,   122,   160,    10,    14,   160,    12,
      12,   123,   160,   123,   160,   158,   123,   158,    26,    43,
     122,    26,    43,   122,   128,   159,   159,   128,   159,   160,
     158,   158,   160,    14,    14,    14,    31,   171,    14,    10,
      10,   171,   171,   124,   160,    69,   132,    26,    29,    30,
      36,    39,    40,    43,    44,   122,    26,    29,    30,    36,
      39,    40,    43,    44,   122,    78,    79,    80,    81,    88,
      89,    90,   110,   111,    82,    83,    84,    85,    86,    87,
      94,   104,   112,   113,   102,    75,    76,   114,   105,   115,
     116,   103,   117,   118,   119,   107,   121,   121,    14,   160,
     160,   160,   160,   160,   160,    14,   124,   177,   123,   160,
     117,   160,    14,   160,   160,    14,    17,    58,   124,   177,
     126,   110,   128,   122,   108,   124,   160,   177,   122,   125,
     108,   122,   108,   122,   108,   122,    10,    11,    12,    14,
      10,    14,    10,   173,   160,    26,    29,    30,    36,    39,
      40,    43,   122,   125,   160,    26,    43,   122,   125,   160,
     158,   160,   158,    77,   125,   158,    75,    76,   123,   128,
     147,   122,   122,   108,   108,   128,   110,   128,   110,   128,
     160,   158,   160,   158,    29,    44,    23,   160,   160,   160,
     160,   160,   160,   160,   160,   160,    53,   151,   128,   123,
      14,    62,   128,   124,   125,   129,    10,    11,    12,    13,
      24,    25,    33,    34,    47,    48,   129,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   163,   163,   163,   163,   163,   163,
     161,   163,   163,   163,   163,   163,   163,   163,   163,     7,
       8,     9,   164,   166,   166,   166,   167,   167,   168,   169,
     170,   170,   171,   171,   171,   171,   171,    14,    22,    24,
      25,    33,    34,    41,    42,    49,    50,    51,   174,    17,
     174,   122,   123,   122,   122,   122,   122,   122,   122,   108,
     123,   124,   160,   106,   122,   125,   115,   116,   125,   127,
     111,   129,   122,   123,    14,   123,   124,   160,    14,   160,
     160,   122,   124,   160,   160,   123,   160,     5,    14,   123,
     156,   160,   108,   122,   108,   122,   108,   122,   123,   147,
     108,   108,   124,   125,   125,   160,   160,   160,   160,   160,
     160,   160,   177,   125,   160,   160,   177,   124,    27,    28,
     135,   124,    10,   177,   167,   167,    10,    14,   124,   157,
     132,   160,   160,   144,    14,   160,    14,   160,   124,    27,
     124,   160,   160,    10,   122,   122,   122,   122,   122,   129,
     158,    14,    22,   128,   152,   124,   157,   123,   123,   154,
     124,   177,   121,   126,   128,   108,   160,   160,   160,   129,
     160,    26,    43,   129,    26,    43,   129,    26,    29,    30,
      36,    39,    40,    43,    44,   129,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,    91,   123,    14,   123,   124,   177,   160,   124,
     177,   125,   160,   177,   160,   160,   177,   108,   108,   108,
     124,   177,    14,   124,   155,   177,   127,   111,   129,   122,
     122,   160,   106,   122,   125,    74,    74,   156,   122,   125,
     122,   160,   123,   160,   160,   124,   157,   160,   160,   108,
      10,   177,   122,   122,   122,   122,   122,   122,   122,   122,
     177,   122,   122,   122,   158,   134,   158,   123,   160,   158,
     158,    77,   158,    77,    77,   108,    10,    67,    68,   147,
     124,   125,    24,    25,    34,   129,   160,   122,   122,    37,
      38,   129,   111,   129,   111,   129,   158,   158,   158,   122,
     122,   108,   160,    14,    14,   152,    56,    57,    60,   129,
     147,   124,   124,   157,   157,    71,    72,    73,   129,   124,
      14,   160,   160,   160,    26,    30,    36,    39,    40,    43,
     129,   129,   129,   129,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   161,   124,   177,   124,
     155,   177,   122,   124,   122,   124,   177,   122,   122,   127,
     127,   127,   160,   160,   160,   124,    74,   124,   125,   124,
     125,   160,   177,   160,   160,   124,     5,    14,   122,   160,
     122,   125,   122,   147,   124,   122,   122,   160,   124,   124,
      27,   135,   160,   158,    10,    10,    10,   160,    10,    10,
     147,    10,    14,   160,   160,   129,   160,    26,    29,    30,
      36,    39,    40,    43,    44,   129,   160,   158,   122,   122,
      27,   160,   122,   129,    58,   174,    10,    13,    14,    15,
      16,    56,   147,   147,   124,   124,    14,    14,    14,   108,
     123,   127,   129,   129,   160,   160,   160,   160,   160,   160,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   124,   124,   124,   122,   124,   122,   122,   122,
     160,    14,   177,   122,   122,   122,    74,    74,   125,   177,
     147,   122,   125,   122,   122,   134,   158,   124,    27,   135,
     158,   158,   158,   108,    10,    67,    68,    26,    30,    43,
     129,   129,   129,   160,   160,   160,   160,   160,   160,   160,
     160,   158,   129,   158,   122,   141,   160,   123,   128,   148,
     123,   148,    14,    22,    65,   108,   122,    14,    22,    65,
     108,   122,    10,    13,    15,    16,   122,   122,   174,   147,
     147,     7,     7,     7,   160,   124,   177,   108,   108,   129,
     129,   129,   129,   129,   129,   122,    74,   124,   160,   160,
     177,   122,   177,   158,   158,   160,    10,    10,   160,   160,
     160,   129,   129,   129,   129,   129,   129,   129,   129,   160,
     124,   124,   157,    69,   132,   124,   157,    14,    14,   122,
     160,    14,    14,   122,   160,    14,    22,    65,   108,   122,
      14,    22,    65,   108,   122,   122,   122,   123,   148,   129,
     129,   124,   160,   160,   160,   122,   124,   122,    27,   135,
     129,   129,   129,   122,   158,   148,   124,   129,    24,    25,
      34,   129,   160,   148,   124,   122,   122,   122,   122,   122,
     122,    14,    14,   122,   160,    14,    14,   122,   160,   124,
     157,   129,   129,   129,   122,   158,   141,   148,   160,   160,
     129,   160,    26,    29,    30,    36,    39,    40,    43,    44,
     129,   148,   122,   122,   122,   122,   122,   122,   148,   124,
     124,    26,    30,    43,   129,   129,   129,   160,   160,   160,
     160,   160,   160,   160,   160,   148,   158,   160,   160,   160,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   130,   131,   132,   132,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   134,   134,   134,   134,
     134,   134,   135,   135,   135,   135,   135,   135,   136,   136,
     137,   137,   137,   137,   138,   138,   139,   139,   139,   140,
     140,   140,   141,   142,   142,   142,   142,   142,   142,   143,
     143,   144,   144,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   146,   146,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   149,   150,   151,   151,   151,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     153,   154,   154,   154,   154,   155,   155,   156,   156,   156,
     156,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   160,   160,   160,   161,
     161,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   163,
     163,   164,   164,   164,   165,   165,   166,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   170,   170,   170,   170,
     170,   171,   171,   171,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   175,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   177,   177,
     177,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     4,     6,     6,     8,     5,
       5,     7,     6,     6,     8,    10,     6,     4,     4,     4,
       3,     5,     5,     6,     6,     2,     5,     4,     3,     3,
       5,     7,     3,     5,     7,     3,     3,     2,     4,     7,
       6,     4,     6,     4,     2,     4,     4,     4,     4,     4,
       4,     4,     4,     7,     7,     7,     6,     6,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     1,     1,     2,     4,     2,     1,
       1,     1,     1,     1,     2,     2,     4,     4,     4,     4,
       1,     1,     1,     1,     5,     7,     7,     3,     5,     5,
       4,     6,     3,     5,     5,     7,     4,     6,     5,     3,
       5,     7,     3,     5,     5,     3,     2,     5,     5,     2,
      12,     9,     1,     7,     7,     5,     7,     5,     3,     5,
       7,     0,     4,     6,     5,     3,     7,     6,     4,     7,
       6,     7,     6,     5,     6,     5,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     3,     3,     4,     5,     4,     5,     5,
       6,     6,     6,     6,     6,     6,     6,     6,     7,     7,
       7,     6,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       6,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       7,     7,     6,     4,     7,     6,     4,     8,     7,     5,
       5,     0,     4,     4,     4,     3,     5,     3,     3,     5,
       5,     1,     2,     3,     3,     3,     4,     5,     5,     3,
       5,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       9,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     1,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     4,     3,     6,     6,     5,     3,
       3,     6,     5,     4,     4,     6,     6,     5,     3,     6,
       5,     3,     2,     3,     3,     2,     3,     1,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     4,     6,     6,     6,
       1,     4,     4,     5,     5,     1,     1,     1,     1,     2,
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
#line 406 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2842 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 416 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2848 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 417 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2854 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 421 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2860 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 423 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2866 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 425 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2872 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 427 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2880 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 431 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2886 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 433 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2892 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 435 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2898 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 437 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2906 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 441 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2913 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 444 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2921 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 448 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2927 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 450 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2935 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 454 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2942 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 457 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2950 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 461 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2956 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 463 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2962 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 465 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2968 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 467 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2974 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 469 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2980 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 471 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2988 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 475 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2996 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 479 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3002 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 481 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3008 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 483 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3014 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 485 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3020 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 487 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3026 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 489 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3032 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 491 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3038 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 493 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3044 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 495 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3050 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 497 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3056 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 499 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3062 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 501 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3068 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 503 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3074 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 505 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3080 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3087 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 510 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3094 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 513 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3100 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 515 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3107 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 518 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3114 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 521 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3120 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 523 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3126 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 525 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3132 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 527 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 3138 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 529 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3147 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 534 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3155 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 538 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3163 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 542 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3172 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3180 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 550 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3186 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 552 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3192 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 554 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3198 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 556 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3204 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 558 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3210 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 560 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3217 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 563 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3223 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 565 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3229 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 567 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3235 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 569 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3242 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 572 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3249 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 575 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3256 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 578 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3263 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 581 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3270 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 584 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3276 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3282 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 588 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3288 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 590 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3294 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 592 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3301 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 595 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3307 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 597 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3313 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 599 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3319 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 601 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3326 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 604 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3333 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 607 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3341 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 611 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3348 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 614 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3354 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 616 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3360 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 618 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3366 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 620 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3373 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 623 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3380 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 626 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3387 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 629 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3394 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 632 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3401 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 635 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3408 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 638 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3415 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 640 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3421 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 641 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3427 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 642 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3433 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 643 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3439 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 644 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3445 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 645 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3451 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3457 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 649 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3463 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3469 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 653 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3475 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 654 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3481 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 655 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3487 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 656 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3493 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 657 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3499 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 658 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3505 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 659 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3511 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3517 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3523 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3529 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3535 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 668 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3541 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 669 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3547 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 670 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3553 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 671 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3559 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 675 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3565 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 677 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3571 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 679 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3577 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3583 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3589 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 685 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3595 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 687 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3601 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 689 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3607 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3613 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3619 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 697 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3625 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3631 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3637 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3643 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 707 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3649 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 709 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3655 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 713 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3661 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 715 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3667 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 717 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3673 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3679 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 723 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3685 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 725 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3691 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 729 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3697 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 731 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3703 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 733 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3709 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 737 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3715 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 739 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3721 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 741 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3727 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 744 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3733 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 748 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3743 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 754 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3753 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 760 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3762 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 765 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3772 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 771 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3781 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 776 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3788 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 781 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3800 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 789 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3813 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 799 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3819 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 801 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3827 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 807 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3839 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 815 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3849 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 821 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3859 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 827 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3871 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 835 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3881 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 841 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3891 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 847 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 856 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3915 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 863 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3928 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 872 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3941 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 881 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3952 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 890 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3963 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 897 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3972 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 903 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3978 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 905 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3985 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 908 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3992 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 911 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3998 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 913 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4004 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 915 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4010 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4016 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 919 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4022 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 921 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4028 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 923 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4034 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 925 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4041 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 928 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4047 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 930 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4053 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 932 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4059 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 934 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4066 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 937 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4073 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 940 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4081 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 945 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4087 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 946 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4093 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 948 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4100 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 951 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4107 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 954 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4113 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 956 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4119 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 958 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4125 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 960 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4131 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 962 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4137 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 964 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4143 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 966 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4149 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 968 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4156 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 971 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4162 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 973 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4168 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 975 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4174 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 977 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4181 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 980 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4188 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 983 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4196 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 989 "raku.y"
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
#line 4214 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1005 "raku.y"
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
#line 4231 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 1019 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4237 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 1021 "raku.y"
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
#line 4253 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1033 "raku.y"
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
#line 4269 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1046 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4275 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1048 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4282 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1051 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4289 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1054 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4296 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1057 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4303 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1060 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4310 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1063 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4317 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1066 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4324 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1069 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4331 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1072 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4338 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1075 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4345 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1078 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4352 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1081 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4359 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1084 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4370 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1091 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4381 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1098 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4392 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1105 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4403 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1112 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4411 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1116 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4419 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1120 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4427 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1124 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4435 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1128 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4446 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1135 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4457 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1142 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4468 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1149 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4479 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1156 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4493 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1166 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4505 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1174 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4517 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1182 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4530 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1191 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4541 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1198 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4552 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1205 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4566 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1215 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4578 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1223 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4590 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1233 "raku.y"
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
#line 4607 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1247 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4613 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1249 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4622 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1254 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4631 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1259 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4640 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1266 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4648 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1270 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4656 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1276 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4662 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1278 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4668 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1280 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4674 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1282 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4680 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1285 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4686 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1286 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4692 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1287 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4698 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1288 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4704 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1289 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4710 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1290 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4716 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1291 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4722 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1292 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4728 "raku.tab.c"
    break;

  case 259: /* param_list: VAR_SCALAR '=' expr  */
#line 1293 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4734 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1294 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4740 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list '}'  */
#line 1297 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4746 "raku.tab.c"
    break;

  case 262: /* block: '{' YADA '}'  */
#line 1298 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4752 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr '}'  */
#line 1300 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4758 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1302 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4764 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1304 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4770 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1306 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4776 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1308 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4782 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1310 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4789 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1313 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4795 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1315 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4801 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1317 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4807 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1319 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4813 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1321 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4820 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1324 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4827 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1327 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4835 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1331 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4842 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1334 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4849 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1337 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4856 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1340 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4862 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1342 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4870 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1346 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4877 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1349 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4884 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1352 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4891 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1355 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4898 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1358 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4905 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1361 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4911 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1363 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4917 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1365 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4923 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST '}'  */
#line 1367 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4929 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1369 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4935 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1371 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4941 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1373 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4947 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1375 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4953 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1377 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4959 "raku.tab.c"
    break;

  case 295: /* closure: '{' expr '}'  */
#line 1380 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4965 "raku.tab.c"
    break;

  case 296: /* expr: VAR_SCALAR '=' expr  */
#line 1383 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4971 "raku.tab.c"
    break;

  case 297: /* expr: KW_GATHER block  */
#line 1384 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4981 "raku.tab.c"
    break;

  case 298: /* expr: tern_expr  */
#line 1389 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4987 "raku.tab.c"
    break;

  case 299: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1393 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4993 "raku.tab.c"
    break;

  case 300: /* tern_expr: cmp_expr  */
#line 1394 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4999 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1397 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5005 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1398 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5011 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1399 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5017 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1400 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5023 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1401 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5029 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1402 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5035 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1403 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5041 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1404 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5047 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1405 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5053 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1406 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5059 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1407 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5065 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1408 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5071 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1409 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5077 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1410 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5083 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1412 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5093 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1418 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5103 "raku.tab.c"
    break;

  case 317: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1424 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5113 "raku.tab.c"
    break;

  case 318: /* cmp_expr: divis_expr  */
#line 1429 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5119 "raku.tab.c"
    break;

  case 319: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1432 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5125 "raku.tab.c"
    break;

  case 320: /* divis_expr: jct_expr  */
#line 1433 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5131 "raku.tab.c"
    break;

  case 321: /* jct_expr: jct_expr '|' range_expr  */
#line 1436 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5137 "raku.tab.c"
    break;

  case 322: /* jct_expr: jct_expr '&' range_expr  */
#line 1437 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5143 "raku.tab.c"
    break;

  case 323: /* jct_expr: dor_expr  */
#line 1438 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5149 "raku.tab.c"
    break;

  case 324: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1442 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5155 "raku.tab.c"
    break;

  case 325: /* dor_expr: range_expr  */
#line 1443 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5161 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1446 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5167 "raku.tab.c"
    break;

  case 327: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1447 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5173 "raku.tab.c"
    break;

  case 328: /* range_expr: add_expr  */
#line 1448 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5179 "raku.tab.c"
    break;

  case 329: /* add_expr: add_expr '~' repl_expr  */
#line 1451 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5185 "raku.tab.c"
    break;

  case 330: /* add_expr: repl_expr  */
#line 1452 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5191 "raku.tab.c"
    break;

  case 331: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1455 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5197 "raku.tab.c"
    break;

  case 332: /* repl_expr: addsub_expr  */
#line 1456 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5203 "raku.tab.c"
    break;

  case 333: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1459 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5209 "raku.tab.c"
    break;

  case 334: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1460 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5215 "raku.tab.c"
    break;

  case 335: /* addsub_expr: mul_expr  */
#line 1461 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5221 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '*' unary_expr  */
#line 1464 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5227 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr '/' unary_expr  */
#line 1465 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5233 "raku.tab.c"
    break;

  case 338: /* mul_expr: mul_expr '%' unary_expr  */
#line 1466 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5239 "raku.tab.c"
    break;

  case 339: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1467 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5245 "raku.tab.c"
    break;

  case 340: /* mul_expr: unary_expr  */
#line 1468 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5251 "raku.tab.c"
    break;

  case 341: /* unary_expr: '-' unary_expr  */
#line 1471 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5257 "raku.tab.c"
    break;

  case 342: /* unary_expr: '!' unary_expr  */
#line 1472 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5263 "raku.tab.c"
    break;

  case 343: /* unary_expr: CARET unary_expr  */
#line 1473 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5269 "raku.tab.c"
    break;

  case 344: /* unary_expr: OP_REDUCE unary_expr  */
#line 1475 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5278 "raku.tab.c"
    break;

  case 345: /* unary_expr: pow_expr  */
#line 1479 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5284 "raku.tab.c"
    break;

  case 346: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1482 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5290 "raku.tab.c"
    break;

  case 347: /* pow_expr: postfix_expr  */
#line 1483 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5296 "raku.tab.c"
    break;

  case 348: /* scalar_list: VAR_SCALAR  */
#line 1486 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5302 "raku.tab.c"
    break;

  case 349: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1487 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5308 "raku.tab.c"
    break;

  case 350: /* meth_name: IDENT  */
#line 1490 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5314 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_SORT  */
#line 1491 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5320 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_MAP  */
#line 1492 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5326 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_GREP  */
#line 1493 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5332 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_SAY  */
#line 1494 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5338 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_PRINT  */
#line 1495 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5344 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_TAKE  */
#line 1496 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5350 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_RETURN  */
#line 1497 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5356 "raku.tab.c"
    break;

  case 358: /* meth_name: KW_EXISTS  */
#line 1498 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5362 "raku.tab.c"
    break;

  case 359: /* meth_name: KW_DELETE  */
#line 1499 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5368 "raku.tab.c"
    break;

  case 360: /* meth_name: TESTOP  */
#line 1500 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5374 "raku.tab.c"
    break;

  case 361: /* postfix_expr: call_expr  */
#line 1502 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5380 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '(' arg_list ')'  */
#line 1505 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5389 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '(' ')'  */
#line 1509 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5395 "raku.tab.c"
    break;

  case 364: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1511 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5404 "raku.tab.c"
    break;

  case 365: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1516 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5410 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1518 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5420 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1524 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5430 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1530 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5438 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' KW_NEW  */
#line 1534 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5446 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' IDENT  */
#line 1538 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5455 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1543 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5466 "raku.tab.c"
    break;

  case 372: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5475 "raku.tab.c"
    break;

  case 373: /* call_expr: IDENT '.' CARET IDENT  */
#line 1555 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5485 "raku.tab.c"
    break;

  case 374: /* call_expr: atom '.' CARET IDENT  */
#line 1561 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5495 "raku.tab.c"
    break;

  case 375: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1567 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5506 "raku.tab.c"
    break;

  case 376: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1574 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5517 "raku.tab.c"
    break;

  case 377: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1581 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5526 "raku.tab.c"
    break;

  case 378: /* call_expr: atom '.' meth_name  */
#line 1586 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5535 "raku.tab.c"
    break;

  case 379: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1591 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5546 "raku.tab.c"
    break;

  case 380: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1598 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5555 "raku.tab.c"
    break;

  case 381: /* call_expr: call_expr '.' meth_name  */
#line 1603 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5564 "raku.tab.c"
    break;

  case 382: /* call_expr: KW_DIE expr  */
#line 1608 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5570 "raku.tab.c"
    break;

  case 383: /* call_expr: KW_MAP closure expr  */
#line 1610 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5576 "raku.tab.c"
    break;

  case 384: /* call_expr: KW_GREP closure expr  */
#line 1612 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5582 "raku.tab.c"
    break;

  case 385: /* call_expr: KW_SORT expr  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5588 "raku.tab.c"
    break;

  case 386: /* call_expr: KW_SORT closure expr  */
#line 1616 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5594 "raku.tab.c"
    break;

  case 387: /* call_expr: atom  */
#line 1617 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5600 "raku.tab.c"
    break;

  case 388: /* arg_list: expr  */
#line 1620 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5606 "raku.tab.c"
    break;

  case 389: /* arg_list: arg_list ',' expr  */
#line 1621 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5612 "raku.tab.c"
    break;

  case 390: /* arg_list: arg_list ','  */
#line 1622 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5618 "raku.tab.c"
    break;

  case 391: /* atom: LIT_INT  */
#line 1625 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5624 "raku.tab.c"
    break;

  case 392: /* atom: LIT_FLOAT  */
#line 1626 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5630 "raku.tab.c"
    break;

  case 393: /* atom: LIT_STR  */
#line 1627 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5636 "raku.tab.c"
    break;

  case 394: /* atom: WORDLIST  */
#line 1629 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5648 "raku.tab.c"
    break;

  case 395: /* atom: LIT_INTERP_STR  */
#line 1636 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5654 "raku.tab.c"
    break;

  case 396: /* atom: VAR_SCALAR  */
#line 1637 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5660 "raku.tab.c"
    break;

  case 397: /* atom: OP_INC VAR_SCALAR  */
#line 1638 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5666 "raku.tab.c"
    break;

  case 398: /* atom: OP_DEC VAR_SCALAR  */
#line 1639 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5672 "raku.tab.c"
    break;

  case 399: /* atom: VAR_SCALAR OP_INC  */
#line 1640 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5678 "raku.tab.c"
    break;

  case 400: /* atom: VAR_SCALAR OP_DEC  */
#line 1641 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5684 "raku.tab.c"
    break;

  case 401: /* atom: VAR_ARRAY  */
#line 1642 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5690 "raku.tab.c"
    break;

  case 402: /* atom: VAR_HASH  */
#line 1643 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5696 "raku.tab.c"
    break;

  case 403: /* atom: VAR_CAPTURE  */
#line 1645 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5704 "raku.tab.c"
    break;

  case 404: /* atom: VAR_FH  */
#line 1649 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5712 "raku.tab.c"
    break;

  case 405: /* atom: VAR_NAMED_CAPTURE  */
#line 1653 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5719 "raku.tab.c"
    break;

  case 406: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1656 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5725 "raku.tab.c"
    break;

  case 407: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1658 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5731 "raku.tab.c"
    break;

  case 408: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1660 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5737 "raku.tab.c"
    break;

  case 409: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1662 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5743 "raku.tab.c"
    break;

  case 410: /* atom: ARR_ALL_SLICE  */
#line 1664 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5749 "raku.tab.c"
    break;

  case 411: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1666 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5755 "raku.tab.c"
    break;

  case 412: /* atom: VAR_HASH '{' expr '}'  */
#line 1668 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5761 "raku.tab.c"
    break;

  case 413: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1670 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5767 "raku.tab.c"
    break;

  case 414: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1672 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5773 "raku.tab.c"
    break;

  case 415: /* atom: IDENT  */
#line 1673 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5779 "raku.tab.c"
    break;

  case 416: /* atom: VAR_TWIGIL  */
#line 1675 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5787 "raku.tab.c"
    break;

  case 417: /* atom: VAR_ARRAY_TWIGIL  */
#line 1679 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5795 "raku.tab.c"
    break;

  case 418: /* atom: VAR_HASH_TWIGIL  */
#line 1683 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5803 "raku.tab.c"
    break;

  case 419: /* atom: '(' ')'  */
#line 1686 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5809 "raku.tab.c"
    break;

  case 420: /* atom: '(' expr ')'  */
#line 1687 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5815 "raku.tab.c"
    break;

  case 421: /* atom: '(' expr ',' ')'  */
#line 1689 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5821 "raku.tab.c"
    break;

  case 422: /* atom: '(' expr ',' arg_list ')'  */
#line 1691 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5828 "raku.tab.c"
    break;

  case 423: /* atom: block  */
#line 1693 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5834 "raku.tab.c"
    break;

  case 424: /* atom: KW_SUB block  */
#line 1694 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5840 "raku.tab.c"
    break;


#line 5844 "raku.tab.c"

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

#line 1696 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
