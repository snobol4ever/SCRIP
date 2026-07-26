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
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
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

#line 362 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 64,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 65,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 66,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 67,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 68,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 69,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 70,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 71,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 72,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 73,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 74,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 75,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 76,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 77,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 78,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 79,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 80,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 81,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 82,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 83,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 84,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 85,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 86,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 87,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 88,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 89,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 90,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 91,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 92,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 93,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 94,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 95,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 96,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 97,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 98,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 99,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 100,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 101,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 102,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 103,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 104,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 105,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 106,                   /* OP_POW  */
  YYSYMBOL_107_ = 107,                     /* '='  */
  YYSYMBOL_108_ = 108,                     /* '!'  */
  YYSYMBOL_109_ = 109,                     /* '<'  */
  YYSYMBOL_110_ = 110,                     /* '>'  */
  YYSYMBOL_111_ = 111,                     /* '|'  */
  YYSYMBOL_112_ = 112,                     /* '&'  */
  YYSYMBOL_113_ = 113,                     /* '~'  */
  YYSYMBOL_114_ = 114,                     /* '+'  */
  YYSYMBOL_115_ = 115,                     /* '-'  */
  YYSYMBOL_116_ = 116,                     /* '*'  */
  YYSYMBOL_117_ = 117,                     /* '/'  */
  YYSYMBOL_118_ = 118,                     /* '%'  */
  YYSYMBOL_UMINUS = 119,                   /* UMINUS  */
  YYSYMBOL_120_ = 120,                     /* '.'  */
  YYSYMBOL_121_ = 121,                     /* ';'  */
  YYSYMBOL_122_ = 122,                     /* '('  */
  YYSYMBOL_123_ = 123,                     /* ')'  */
  YYSYMBOL_124_ = 124,                     /* ','  */
  YYSYMBOL_125_ = 125,                     /* '['  */
  YYSYMBOL_126_ = 126,                     /* ']'  */
  YYSYMBOL_127_ = 127,                     /* '{'  */
  YYSYMBOL_128_ = 128,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 129,                 /* $accept  */
  YYSYMBOL_program = 130,                  /* program  */
  YYSYMBOL_stmt_list = 131,                /* stmt_list  */
  YYSYMBOL_stmt = 132,                     /* stmt  */
  YYSYMBOL_if_stmt = 133,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 134,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 135,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 136,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 137,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 138,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 139,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 140,                /* loop_incr  */
  YYSYMBOL_for_stmt = 141,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 142,               /* given_stmt  */
  YYSYMBOL_when_list = 143,                /* when_list  */
  YYSYMBOL_sub_decl = 144,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 145,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 146,                 /* sub_body  */
  YYSYMBOL_method_body = 147,              /* method_body  */
  YYSYMBOL_class_decl = 148,               /* class_decl  */
  YYSYMBOL_role_decl = 149,                /* role_decl  */
  YYSYMBOL_is_clauses = 150,               /* is_clauses  */
  YYSYMBOL_class_body_list = 151,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 152,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 153,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 154,           /* named_arg_list  */
  YYSYMBOL_pair_list = 155,                /* pair_list  */
  YYSYMBOL_param_list = 156,               /* param_list  */
  YYSYMBOL_block = 157,                    /* block  */
  YYSYMBOL_closure = 158,                  /* closure  */
  YYSYMBOL_expr = 159,                     /* expr  */
  YYSYMBOL_tern_expr = 160,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 161,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 162,               /* divis_expr  */
  YYSYMBOL_jct_expr = 163,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 164,                 /* dor_expr  */
  YYSYMBOL_range_expr = 165,               /* range_expr  */
  YYSYMBOL_add_expr = 166,                 /* add_expr  */
  YYSYMBOL_repl_expr = 167,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 168,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 169,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 170,               /* unary_expr  */
  YYSYMBOL_pow_expr = 171,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 172,              /* scalar_list  */
  YYSYMBOL_meth_name = 173,                /* meth_name  */
  YYSYMBOL_postfix_expr = 174,             /* postfix_expr  */
  YYSYMBOL_call_expr = 175,                /* call_expr  */
  YYSYMBOL_arg_list = 176,                 /* arg_list  */
  YYSYMBOL_atom = 177                      /* atom  */
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
#define YYLAST   3799

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  421
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1076

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   362


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
       2,     2,     2,   108,     2,     2,     2,   118,   112,     2,
     122,   123,   116,   114,   124,   115,   120,   117,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   121,
     109,   107,   110,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   125,     2,   126,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   127,   111,   128,   113,     2,     2,     2,
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
     105,   106,   119
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   375,   375,   386,   387,   390,   392,   394,   396,   400,
     402,   404,   406,   410,   413,   417,   419,   423,   426,   430,
     432,   434,   436,   438,   440,   444,   448,   450,   452,   454,
     456,   458,   460,   462,   464,   466,   468,   470,   472,   474,
     476,   479,   482,   484,   487,   490,   492,   494,   496,   498,
     503,   507,   511,   516,   520,   521,   523,   525,   527,   529,
     532,   534,   536,   538,   541,   544,   547,   550,   553,   555,
     557,   559,   561,   564,   566,   568,   570,   573,   576,   580,
     583,   585,   587,   589,   592,   595,   598,   601,   604,   607,
     610,   611,   612,   613,   614,   615,   616,   618,   620,   622,
     624,   625,   626,   627,   628,   629,   630,   632,   634,   636,
     638,   639,   640,   641,   644,   646,   648,   650,   652,   654,
     656,   658,   662,   664,   666,   668,   670,   672,   676,   678,
     682,   684,   686,   688,   692,   694,   698,   700,   702,   706,
     708,   710,   714,   717,   723,   729,   734,   740,   745,   750,
     758,   769,   770,   776,   784,   790,   796,   804,   810,   816,
     825,   832,   841,   850,   859,   866,   873,   874,   877,   880,
     882,   884,   886,   888,   890,   892,   894,   897,   899,   901,
     903,   906,   909,   915,   916,   917,   920,   923,   925,   927,
     929,   931,   933,   935,   937,   940,   942,   944,   946,   949,
     952,   958,   974,   989,   990,  1002,  1016,  1017,  1020,  1023,
    1026,  1029,  1032,  1035,  1038,  1041,  1044,  1047,  1050,  1053,
    1060,  1067,  1074,  1081,  1085,  1089,  1093,  1097,  1104,  1111,
    1118,  1125,  1135,  1143,  1151,  1160,  1167,  1174,  1184,  1192,
    1202,  1217,  1218,  1223,  1228,  1235,  1239,  1245,  1247,  1249,
    1251,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1265,
    1266,  1267,  1269,  1271,  1273,  1275,  1277,  1280,  1282,  1284,
    1286,  1288,  1291,  1294,  1298,  1301,  1304,  1307,  1309,  1313,
    1316,  1319,  1322,  1325,  1328,  1330,  1332,  1334,  1336,  1338,
    1340,  1342,  1344,  1348,  1351,  1352,  1357,  1360,  1362,  1365,
    1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,
    1376,  1377,  1378,  1379,  1385,  1391,  1397,  1400,  1401,  1404,
    1405,  1406,  1409,  1411,  1414,  1415,  1416,  1419,  1420,  1423,
    1424,  1427,  1428,  1429,  1432,  1433,  1434,  1435,  1436,  1439,
    1440,  1441,  1442,  1447,  1450,  1451,  1454,  1455,  1458,  1459,
    1460,  1461,  1462,  1463,  1464,  1465,  1466,  1467,  1468,  1470,
    1472,  1477,  1478,  1483,  1485,  1491,  1497,  1501,  1505,  1510,
    1517,  1522,  1528,  1534,  1541,  1548,  1553,  1558,  1565,  1570,
    1575,  1577,  1579,  1581,  1583,  1585,  1588,  1589,  1590,  1593,
    1594,  1595,  1596,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1616,  1620,  1623,  1625,  1627,  1629,  1631,  1633,
    1635,  1637,  1639,  1640,  1644,  1648,  1652,  1653,  1654,  1656,
    1659,  1660
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
  "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT",
  "OP_SLE", "OP_SGT", "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='",
  "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'",
  "'}'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
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

#define YYPACT_NINF (-781)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-421)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -781,    70,  1709,  -781,  -781,  -781,  -781,  -781,   663,   -54,
     -22,   -93,    10,  -781,  -781,  3672,  -781,  -781,  -781,    64,
     833,    49,  2853,  2922,  2978,  3047,  3103,    12,    30,  3103,
    1778,   164,  3103,   107,   190,  3172,  3228,    30,   -55,    80,
      83,    86,    86,  3297,    30,    30,  3103,   232,   238,    67,
    3672,  -781,   287,   332,   385,  3672,  3672,  -781,  1847,   354,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
     552,  -781,  -781,  -781,   636,   581,  -781,   219,   474,   249,
     312,  -781,     2,   216,   264,   274,  -781,  -781,   339,   334,
     368,   482,  -781,  -781,  3103,  3103,  3103,  3103,  3103,  3103,
     496,  1903,  3353,  1973,   504,  3103,  3103,    32,  2029,   133,
     399,    -7,  -781,    30,  -781,  -781,   416,   236,  -781,  2099,
    -781,   562,   142,   197,   247,   770,   529,   395,   560,  1847,
     477,  1847,    56,  1847,    30,  1847,    30,   -58,   168,   114,
    -781,  -781,   462,  -781,   468,   493,   497,   527,   138,   141,
    1847,    30,  1847,    30,   229,  2155,  -781,  3103,  3103,  -781,
    3103,  3103,  -781,  3103,  3103,  3103,  2224,  3103,  -781,   565,
    -781,  -781,  -781,   557,   540,    24,  -781,   599,  -781,  -781,
    -781,  -781,  -781,   -10,   608,  1094,  3103,  3103,  3103,  3103,
    3103,  3103,  3103,  3103,  -781,  3103,  3103,  3103,  3103,  3103,
    3103,  3103,  3103,  -781,  3672,  3672,  3672,  3672,  3672,  3672,
    3672,  3672,  3672,  3672,  3672,  3672,  3672,  3672,  3672,   781,
    3672,  3672,  3672,  3672,  3672,  3672,  3672,  3672,  3672,  3672,
    3672,  3672,  3672,  3672,  3672,   881,   778,   315,   582,   622,
     630,   645,   648,   650,   193,  -781,   503,  1847,   170,   522,
      25,   674,   658,   670,   679,   782,   683,  -781,   702,  1973,
     796,  3103,  -781,  3103,   692,   -10,   707,  -781,  3103,  3103,
    -781,  3422,  -781,  3478,  -781,   248,   252,   273,   152,   710,
     753,  -781,   709,   711,  3103,  3103,  3103,  3103,  3103,  3103,
    3103,  -781,  3103,   731,  3103,  3103,  -781,  3103,   733,   618,
     735,  -781,   830,  3103,  -781,  3672,  3672,    -1,  -781,  -781,
    -781,  -781,  3103,  3103,  -781,   859,  3103,   863,  3103,   743,
     870,   745,  -781,  3103,  3103,   903,   783,   800,   832,   835,
     838,   798,  -781,  -781,  -781,    30,    -6,  -781,     5,   807,
     818,  -781,  -781,  2293,  -781,   875,   -52,   186,   857,  2853,
    2922,  3103,  1451,    14,   149,  -781,   297,   847,   873,   883,
     914,   915,   916,   917,   918,   919,   920,   921,   922,   926,
     927,   928,   929,   877,   877,   877,   877,   877,   877,   886,
     877,   877,   877,   877,   877,   877,   877,   877,  -781,  -781,
    -781,   249,  -781,  -781,  -781,   872,   872,   216,   264,   274,
     274,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,   930,   982,   931,
    -781,  2349,  -781,  -781,  -781,  -781,  -781,  -781,  3103,  2419,
    -781,   748,  3103,  -781,  3103,  3103,  3103,  3103,   944,   947,
     948,  -781,  2475,  -781,  2545,  -781,   378,   946,   932,  -781,
    -781,   936,  -781,   937,  1847,   241,   986,   134,  2601,   564,
     940,  3103,  -781,  3547,  -781,  3103,  -781,    15,  -781,  3103,
    3103,   955,  1053,  2293,   943,   945,   951,   952,   953,   954,
     957,   575,  2293,   958,   959,   576,    30,   -15,  3603,  -781,
      30,    30,   -39,   -24,     3,  -781,   163,   941,   752,  1262,
     960,   961,    13,   966,   939,   967,   963,    30,    30,    30,
     962,   964,   977,  3103,  -781,  -781,  -781,  -781,  -781,  -781,
    1072,  1073,  -781,     8,   941,   756,    21,   409,   144,  -781,
     762,  1074,  1973,  3103,  3103,   327,    19,   204,  -781,   278,
    3103,  3103,  -781,  3103,  3103,  -781,  3103,  3103,  3103,  3103,
    3103,  3103,  3103,  3103,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  3672,  2671,  -781,  2727,   968,   767,   971,  -781,   769,
    2293,   972,   577,   969,   970,   428,  3103,  3103,  3103,  -781,
     776,   158,  -781,   784,   786,  -781,  -781,  -781,  -781,  -781,
     793,  3103,  -781,  3103,  3103,  3103,   795,  -781,   239,  -781,
     980,  1847,   584,  1000,   941,   801,  1001,  1010,  3103,  -781,
     810,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,   812,
    -781,  -781,  -781,   874,  -781,  -781,  1847,    30,  -781,  -781,
    1092,  -781,  1093,  1122,  -781,  1123,  1124,  -781,   941,   509,
    2853,  2922,  1520,  -781,   345,  -781,  -781,  3103,    30,  -781,
    -781,  -781,  1029,  1030,  1125,  -781,  -781,  -781,  -781,  3103,
    1034,  -781,  -781,    51,   619,   808,  1100,  -781,  -781,   941,
     941,   814,   819,  1144,  1146,  1150,  -781,  -781,   217,   583,
    1037,   286,  3103,  3103,  3103,  3103,  3103,  3103,  -781,  -781,
    -781,  -781,   289,   300,   310,   319,   321,   325,   329,   341,
     343,   344,   349,   359,  -781,  -781,   822,  -781,   826,   828,
    -781,  1045,  -781,  -781,   834,  -781,  -781,  -781,  -781,  -781,
    1046,  1047,  1048,  -781,  3103,  -781,  1156,  -781,  2293,  1050,
     606,  -781,  -781,  1051,  1101,  1102,  -781,   839,  -781,  3103,
    -781,  -781,   941,  -781,  -781,   620,  1052,  1055,   -15,  -781,
     855,   956,    30,    30,    30,  -781,  -781,  -781,  -781,   171,
     357,    48,  -781,   361,  3103,  3103,  3103,  3103,  3103,  3103,
    3103,  3103,  -781,    30,  1049,  -781,  -781,    30,  1057,  3103,
    -781,   255,   305,    61,    96,   718,  1058,  1059,   881,  -781,
    -781,   941,   941,  1174,  1175,  1176,  3103,  2797,  1077,  1078,
    -781,   362,   363,   373,   393,   394,   406,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  1065,  -781,  -781,  -781,  -781,  1114,   864,
    -781,  -781,  -781,  3103,  3103,  2293,   643,  -781,  -781,  3103,
    -781,  -781,  -781,  -781,    30,    30,  -781,  -781,  -781,  -781,
    -781,  1180,  1181,  3103,  3103,  3103,  -781,  -781,  -781,   407,
     408,   412,   443,   447,   451,   452,   466,  -781,  -781,  -781,
    3103,  1069,  -781,    22,  1126,  -781,    29,  -781,  1179,  1182,
    1076,  3103,  -781,  1184,  1185,  1079,  3103,  -781,   196,   198,
    1080,  1082,  -781,  -781,   352,  -781,  -781,  -781,  -781,  -781,
     485,  1067,   866,  3103,  3103,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  3103,  1083,  -781,  -781,   890,  -781,   644,   988,
    -781,  -781,  -781,   487,   491,   501,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  1084,    30,  1081,   894,  1085,  1382,
    1081,   896,  1086,  1089,  -781,  1090,  1091,  1096,  -781,  1097,
    1192,  1200,  1098,  3103,  -781,  1206,  1209,  1103,  3103,  -781,
    -781,  -781,    44,  -781,  -781,  -781,  1099,   502,   507,  -781,
    -781,  1104,  -781,    30,  -781,  -781,  -781,  -781,  3103,  -781,
    -781,  1081,  -781,  2853,  2922,  1589,  -781,   372,  -781,  1081,
    -781,  -781,  -781,  -781,  -781,  -781,  1105,  1107,  -781,  1108,
    1109,  1110,  -781,  1111,  1081,   898,  -781,  -781,  -781,  -781,
    -781,  1112,  -781,   469,   111,  -781,   510,  3103,  3103,  3103,
    3103,  3103,  3103,  3103,  3103,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  1081,    30,  3103,  3103,  3103,  -781,
    -781,  -781,   511,   519,   535,   536,   537,   538,   550,   551,
    -781,  -781,   553,   559,   561,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   389,   390,   391,   393,   394,   399,
     400,   413,   412,   414,   415,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,    91,     0,     3,
       4,    92,    93,   100,   101,   102,   103,    94,    95,   110,
       0,   111,   112,   113,    99,     0,   296,   298,   316,   318,
     321,   323,   326,   328,   330,   333,   338,   343,   345,   359,
     385,     0,   397,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
     399,   400,   413,     0,   420,   341,     0,   394,    35,     0,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   326,     0,
     421,   295,     0,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   136,     0,   139,     0,     0,   104,
       0,     0,   105,     0,     0,     0,     3,     0,   383,    96,
      98,   380,   203,     0,     0,     0,   342,     0,   395,   396,
     340,   339,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,   367,   361,     0,     0,
       0,     0,    30,     0,   416,     0,     0,    38,   388,     0,
       6,     0,     9,     0,    10,     0,     0,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,    42,     0,     0,   117,
       0,   129,     0,     0,   148,     0,     0,     0,     3,   155,
      45,    46,     0,     0,   151,     0,     0,     0,     0,     0,
     132,     0,   135,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,   382,   384,     0,     0,   206,     0,     0,
       0,   241,   417,     0,   260,   394,   399,   400,   413,     0,
       0,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   302,   305,   306,   299,   300,     0,
     303,   304,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   317,   319,   320,   322,   324,   325,   327,   329,   331,
     332,   337,   334,   335,   336,   344,   348,   358,   352,   353,
     354,   355,   356,   357,   350,   351,   349,   379,     0,   376,
      51,     0,    85,    86,    87,    88,    89,    48,     0,     0,
     362,     0,     0,    15,     0,     0,     0,     0,   404,   408,
     409,    53,     0,   371,     0,   360,     0,     0,     0,   294,
      37,     0,   387,     0,     0,     0,   391,   412,     0,     0,
       0,     0,    27,     0,    28,     0,    29,     0,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,     0,     0,   120,
     417,     0,     0,   324,   325,   251,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,     0,   417,
       0,     0,     0,     0,   106,   107,   108,   109,   293,    97,
       0,     0,   206,     0,     0,     0,     0,     0,     0,   418,
       0,     0,     0,     0,     0,     0,     0,     0,   286,     0,
       0,     0,   287,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,     0,   261,    55,    57,    59,    62,    60,
      61,    56,    58,    68,    70,    72,    75,    73,    74,    69,
      71,     0,     0,   372,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   370,
       0,   412,   366,     0,     0,   404,   408,   409,    36,     5,
       0,     0,    11,     0,     0,     0,     0,    20,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,    76,    79,    78,    82,    80,    81,    77,    40,     0,
      83,    84,    43,   114,   119,   118,     0,     0,   128,   145,
       0,   147,     0,     0,   252,     0,     0,   154,     0,     0,
       0,     0,     0,   166,     0,    32,    31,     0,     0,   149,
     410,   411,     0,     0,   130,   133,   134,   137,   138,     0,
       0,   204,   205,     0,     0,     0,     0,   202,   163,     0,
       0,     0,     0,     0,     0,     0,   240,   419,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   277,
     284,   285,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   297,   378,     0,   375,     0,     0,
      50,     0,    52,   164,     0,    17,    16,   407,   406,   405,
       0,     0,     0,   369,     0,   364,     0,   365,     0,     0,
       0,   248,   247,     0,     0,     0,    22,     0,    23,     0,
      26,   157,     0,    34,    33,     0,   419,   419,     0,   121,
       0,   122,     0,     0,     0,   253,   254,   153,   255,     0,
       0,     0,   169,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,     0,     0,    66,    67,     0,     0,     0,
     201,     0,     0,     0,     0,     0,     0,     0,     0,   162,
     160,     0,     0,     0,     0,     0,     0,     0,   404,   409,
     281,     0,     0,     0,     0,     0,     0,   289,   290,   291,
     292,   262,   264,   266,   269,   267,   268,   263,   265,   377,
     374,   373,    49,   419,    63,    64,    65,   245,     0,     0,
      13,    12,    21,     0,     0,     0,     0,   156,     7,     0,
      41,    44,   116,   115,   417,     0,   126,   146,   143,   144,
     256,     0,     0,     0,     0,     0,   170,   171,   168,     0,
       0,     0,     0,     0,     0,     0,     0,   152,   150,   131,
       0,     0,   142,     0,     3,   236,     0,   233,     0,     0,
       0,     0,   212,     0,     0,     0,     0,   207,     0,     0,
       0,     0,   208,   209,     0,   159,   161,   242,   243,   244,
       0,   165,     0,     0,     0,   271,   273,   276,   274,   275,
     272,    18,     0,   419,   250,   249,     0,    24,     0,   123,
     124,   257,   258,     0,     0,     0,   172,   174,   176,   179,
     177,   178,   173,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,   224,     0,
       0,     0,     0,     0,   214,     0,     0,     0,     0,   213,
     210,   211,     0,   239,   280,   279,   164,     0,     0,   246,
      14,   419,     8,     0,   127,   180,   182,   181,     0,   141,
     235,     0,   184,     0,     0,     0,   183,     0,   232,     0,
     221,   222,   216,   219,   220,   215,     0,     0,   225,     0,
       0,     0,   226,     0,     0,     0,   278,   282,   283,    25,
     125,     0,   234,     0,     0,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   185,   231,   229,   230,   218,
     227,   228,   217,   238,     0,     0,     0,     0,     0,   188,
     189,   186,     0,     0,     0,     0,     0,     0,     0,     0,
     237,   140,     0,     0,     0,   190,   192,   194,   197,   195,
     196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -781,  -781,     4,  -781,  -482,  -627,  -781,  -781,  -781,  -781,
    -781,   245,  -781,  -781,  -781,  -781,  -781,  -276,  -780,  -781,
    -781,  -781,   712,  -781,  -781,   662,   779,  -321,   307,   981,
      -2,  -207,  -781,   794,  1018,  -781,   586,   -16,  1013,  1014,
     805,    -8,  -781,  -781,  -235,  -781,  -781,   132,  -781
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   185,    60,    61,   489,    62,    63,    64,    65,
      66,   881,    67,    68,   502,    69,    70,   309,   885,    71,
      72,   336,   523,    73,   528,   593,   459,   498,   114,   164,
     120,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   282,   417,    88,    89,   121,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,   419,   468,   379,     2,   634,   759,   115,   520,   495,
     138,    24,   887,   496,   106,   495,   521,   525,   302,   496,
     130,   132,   134,   136,   137,   495,   139,   142,   144,   496,
     147,   495,   495,   151,   153,   496,   496,   640,   339,   495,
     540,   168,   176,   496,   171,   294,   254,   180,   181,   255,
     657,   658,   642,   102,   495,   102,   183,   541,   496,   122,
     123,   124,   295,   125,   674,   675,   303,   155,   676,    59,
       3,   103,    59,   532,   294,   888,   224,   225,   116,   643,
     126,   174,   294,   889,   127,   268,   340,   104,    59,   226,
     256,   295,   238,   239,   240,   241,   242,   243,   175,   295,
     248,   250,   260,   252,   253,   105,   157,   674,   675,   160,
     893,   676,    59,   342,   343,   226,   226,   265,   894,   148,
     261,   522,   497,   158,   973,   890,   161,   283,   524,   293,
     107,   298,   108,   300,   856,   159,   677,   294,   614,    59,
     296,   659,   542,   297,   680,   946,   615,   699,   319,   437,
     321,   438,   950,   326,   295,   327,   328,    59,   329,   330,
     895,   331,   332,   333,   331,   334,   990,  1014,   891,   296,
     998,   128,   297,   644,   145,   543,   867,   296,   146,   790,
     297,   860,   892,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   544,   365,   366,   367,   368,   369,   370,   371,
     372,   159,   149,   896,   162,   681,   682,   605,   395,   396,
     960,  1022,   965,   163,   683,   684,   685,   897,   961,  1036,
     966,   647,   401,   402,   403,   404,   405,    92,    93,   645,
     646,   734,   296,   246,  1043,   297,   307,   861,   862,  1050,
     258,   308,   305,   306,   744,   431,   172,   315,   678,   269,
     317,   266,   173,   745,   107,   101,   108,   446,   323,   448,
     962,   449,   967,   270,  1060,   316,   452,   453,   318,   455,
     162,   460,   686,   324,   467,   432,   852,   545,   107,   308,
     108,   226,   474,   475,   476,   477,   478,   479,   480,   493,
     494,   433,   483,   484,   434,   104,   204,   205,   206,   207,
     428,   177,   984,   963,   271,   968,   208,   209,   210,    74,
     500,   501,   499,   533,   504,   429,   506,   964,   272,   969,
     227,   510,   511,   546,   806,   310,   547,   548,   211,   212,
      92,    93,   700,   549,   140,   141,   550,   551,   751,   807,
     552,   553,   178,   263,   154,   156,   601,   535,   536,   537,
     539,   169,   170,   692,   273,   461,   285,   693,   101,   463,
     221,   222,   602,   694,   714,   603,   695,   696,   274,   462,
     697,   774,   767,   464,   775,   776,   230,   883,   228,   229,
     465,   777,   884,   863,   778,   779,   285,   864,   780,   781,
     231,   232,   233,   287,   466,   179,   288,   289,  1027,   311,
     865,  1028,  1029,   799,   800,   279,   701,   441,  1030,   280,
     514,  1031,  1032,   223,   810,  1033,  1034,   817,   203,   495,
     140,   515,   184,   496,   481,   554,   577,   886,   818,   485,
     581,   516,   884,   583,   584,   492,   420,   421,   819,   792,
     517,   299,   563,   301,   304,   234,   564,   820,   291,   821,
     565,   292,   600,   822,   235,   698,   183,   823,   320,   610,
     322,   612,   566,   613,   567,   568,   203,   616,   617,   824,
     569,   825,   826,   782,   972,   530,   847,   827,   291,   884,
     570,   292,   311,   621,   623,   866,   637,   828,   236,   868,
     915,   916,    74,   203,   624,  1046,   237,   654,   285,  1047,
    1035,   917,   437,   284,   595,   287,   285,   286,   288,   289,
     244,   670,  1048,   287,   625,   626,   288,   289,   251,   768,
     290,   918,   919,   769,   259,   905,   906,   627,   563,   564,
     689,   690,   691,   565,   920,   936,   937,   262,   702,   703,
     938,   704,   705,   278,   706,   707,   708,   709,   710,   711,
     712,   713,   268,   576,   729,   213,   214,   215,   216,   217,
     218,   579,   947,   904,   566,   951,   582,   219,   567,   585,
     281,   939,   568,   569,   590,   940,   594,   220,   186,   941,
     942,   187,   188,   310,   730,   731,   732,   570,   189,   311,
     291,   190,   191,   292,   943,   192,   193,  1049,   291,   739,
     312,   292,   741,   742,   313,   620,   722,   195,   621,   747,
     196,   197,   623,   974,   629,   985,   755,   198,   335,   986,
     199,   200,   627,   834,   201,   202,   430,   268,   836,   987,
    1017,   311,   563,   406,   760,  1018,   435,   436,  1051,  1065,
     564,   407,   519,   408,   409,   487,   488,  1066,   770,   771,
     773,  1015,   410,   411,   314,   783,   565,   566,   567,   568,
     412,   413,   338,  1067,  1068,  1069,  1070,   788,   414,   415,
     416,   569,   570,   194,   621,  -420,  -420,   791,  1071,  1072,
     623,  1073,   627,   267,   337,   607,   268,  1074,   608,  1075,
     811,   812,   813,   814,   815,   816,   628,   632,   726,   268,
     268,   268,   203,   422,   716,   748,   719,   437,   749,   808,
    -420,  -420,   724,  -420,  -420,  -420,  -420,  -420,  -420,  -420,
    -420,  -420,  -420,  -420,  -420,  -420,   341,   841,   898,  -420,
     268,   899,   837,   900,   901,   740,   344,  -420,  -420,  -420,
    -420,   848,  -420,   423,   849,  -420,  -420,  -420,  -420,  -420,
    -420,   424,  -420,  -420,  -420,    91,  -420,    92,    93,    94,
      95,    96,    97,    98,   927,   982,   425,   268,   268,   426,
      99,   427,   869,   870,   871,   872,   873,   874,   875,   876,
     275,   276,   277,   100,   439,   101,   440,   882,   388,   389,
     390,   441,   406,   633,   635,   418,   443,   638,   639,   641,
     407,   442,   408,   409,   910,   444,    74,   392,   393,   394,
     447,   410,   411,   450,   664,   665,   666,   469,   793,   412,
     413,   794,   795,   796,   797,   445,   268,   414,   415,   416,
     451,   268,   471,   472,   342,   473,     4,     5,     6,     7,
     491,   924,   925,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,   342,   482,   486,   343,   490,   343,
     470,   933,   934,   935,   113,    28,   507,   343,   509,   343,
     839,   342,   580,   503,    33,   648,   649,   505,   944,   679,
     649,   846,    41,    42,    43,   687,   268,    46,   949,   955,
     721,   268,   723,   268,   959,   406,    50,   508,    51,   733,
     268,   758,   488,   407,   513,   408,   409,   735,   736,   737,
     268,   977,   978,   512,   410,   411,   342,   738,   743,   608,
     979,   514,   412,   413,   752,   649,   518,    53,    54,   526,
     414,   415,   416,   756,   268,   757,   268,   801,   649,   912,
     527,    55,   802,   649,   761,   829,   268,   997,    56,   830,
     736,   831,   268,   515,   118,   119,   516,   833,   268,   517,
      59,  1009,   342,   845,   534,   784,  1013,    91,   555,    92,
      93,    94,    95,    96,    97,    98,   571,   926,   854,   343,
     220,   928,    99,   855,   488,   226,   882,   923,   268,   976,
     268,  1023,  1024,  1026,   556,   531,   573,   101,   373,   374,
     375,   376,   377,   378,   557,   380,   381,   382,   383,   384,
     385,   386,   387,   981,   268,   983,   488,   991,   649,   999,
     649,  1044,   649,   165,   167,  1052,  1053,  1054,  1055,  1056,
    1057,  1058,  1059,   399,   400,   558,   559,   560,   561,   562,
     563,   564,   565,   566,  1062,  1063,  1064,   567,   568,   569,
     570,   586,   572,   574,   587,   588,   596,   598,   599,   604,
     597,   609,   618,   619,   621,   853,   622,   661,   308,   857,
     858,   859,   623,   624,   625,   626,   660,   662,   627,   630,
     631,   655,   656,   667,   669,   668,   671,   672,   688,   720,
     877,   663,   722,   725,   879,   727,   728,     4,     5,     6,
       7,   746,   762,   763,   345,   346,   347,   348,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   349,   350,
      24,   750,   753,    25,    26,    27,    28,   351,   352,    31,
      32,   754,   764,   765,   766,    33,    34,    35,    36,    37,
      38,   353,   354,    41,    42,    43,    44,    45,    46,    47,
     785,   786,   787,    48,    49,   789,   798,    50,   803,    51,
     804,   929,   930,    52,   805,   809,   832,   834,   835,   836,
     838,   840,   842,   850,   843,   844,   851,   878,   880,   902,
     903,   907,   908,   909,   913,   914,   921,   922,    53,    54,
     931,   932,   945,   952,   948,   975,   953,   954,   956,   957,
     958,   970,    55,   971,   980,   988,  1006,  1000,   884,    56,
    1001,  1002,  1003,   992,  1007,    57,    58,  1004,  1005,  1008,
    1010,    59,   355,  1011,  1012,  1019,  1037,  1016,  1038,  1039,
    1040,  1041,  1042,  1021,   673,  1045,   718,   606,   391,   397,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   989,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   650,   651,    24,     0,
    1020,    25,    26,    27,    28,    29,   652,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,     0,    51,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1061,     0,     0,     0,    53,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,    57,    58,     4,     5,     6,     7,    59,
     653,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   993,   994,    24,     0,
       0,    25,    26,    27,    28,    29,   995,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,     0,    51,     0,     0,
       0,    52,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   117,   110,   111,   112,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,   113,    28,     0,     0,     0,     0,     0,     0,
      55,     0,    33,     0,     0,     0,     0,    56,     0,     0,
      41,    42,    43,    57,    58,    46,     0,     0,     0,    59,
     996,     0,     0,     0,    50,     0,    51,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     117,   110,   111,   112,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,   113,    28,     0,     0,     0,     0,     0,     0,    55,
       0,    33,     0,     0,     0,     0,    56,     0,     0,    41,
      42,    43,   143,    58,    46,     0,     0,     0,    59,   538,
       0,     0,     0,    50,     0,    51,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   117,
     110,   111,   112,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     113,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,   143,    58,    46,     0,     0,     0,    59,   772,     0,
       0,     0,    50,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
     143,    58,     4,     5,     6,     7,    59,  1025,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,    26,
      27,    28,    29,    30,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,    50,     0,    51,     0,     0,     0,    52,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   117,   110,
     111,   112,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,   113,
      28,     0,     0,     0,     0,     0,     0,    55,     0,    33,
       0,     0,     0,     0,    56,     0,     0,    41,    42,    43,
      57,    58,    46,     0,     0,     0,    59,     0,     0,     0,
       0,    50,     0,    51,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   117,   110,   111,
     112,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,   143,
      58,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,    58,
     182,     0,     0,     0,    59,     0,     4,     5,     6,     7,
       0,     0,     0,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    53,    54,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    55,     0,     0,    33,     0,     0,     0,    56,     0,
       0,     0,    41,    42,    43,    58,   245,    46,     0,     0,
      59,     0,     4,     5,     6,     7,    50,     0,    51,   117,
     110,   111,   112,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,    28,     0,     0,     0,     0,     0,    53,    54,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    55,     0,    46,     0,     0,     0,     0,    56,   249,
       0,     0,    50,     0,    51,    58,     0,     0,     0,     0,
      59,     0,     4,     5,     6,     7,     0,     0,     0,   117,
     110,   111,   112,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
     113,    28,     0,     0,     0,     0,     0,    55,     0,     0,
      33,     0,     0,     0,    56,     0,     0,     0,    41,    42,
      43,    58,   257,    46,     0,     0,    59,     0,     4,     5,
       6,     7,    50,     0,    51,   117,   110,   111,   112,    12,
      13,    14,    15,    16,    17,    18,     0,     0,   325,     0,
       0,     0,     0,     0,     0,     0,   113,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,   264,     0,     0,     0,    59,     4,     5,     6,
       7,     0,     0,     0,   117,   110,   111,   112,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   113,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,     0,    58,    46,     0,
       0,     0,    59,     0,     0,     0,     0,    50,     0,    51,
       0,     0,   184,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,     0,    55,     0,    33,     0,     0,     0,     0,    56,
       0,     0,    41,    42,    43,     0,    58,    46,     0,     0,
       0,    59,     4,     5,     6,     7,    50,     0,    51,   117,
     110,   111,   112,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,    28,     0,     0,     0,     0,     0,    53,    54,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    55,     0,    46,     0,     0,     0,     0,    56,     0,
       0,     0,    50,     0,    51,    58,   529,     0,     0,     0,
      59,     0,     4,     5,     6,     7,     0,     0,     0,   117,
     110,   111,   112,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
     113,    28,     0,     0,     0,     0,     0,    55,     0,     0,
      33,     0,     0,     0,    56,     0,     0,     0,    41,    42,
      43,    58,   575,    46,     0,     0,    59,     0,     4,     5,
       6,     7,    50,     0,    51,   117,   110,   111,   112,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,   578,     0,     0,     0,    59,     0,     4,     5,
       6,     7,     0,     0,     0,   117,   110,   111,   112,   591,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    53,
      54,     0,     0,     0,     0,     0,   113,    28,     0,     0,
       0,     0,     0,    55,     0,     0,    33,     0,     0,     0,
      56,     0,     0,     0,    41,    42,    43,    58,   589,    46,
       0,     0,    59,     0,     4,     5,   456,     7,    50,     0,
      51,   117,   110,   111,   112,   457,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,    28,     0,     0,     0,     0,     0,    53,
      54,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    55,     0,    46,     0,     0,     0,     0,
      56,     0,     0,     0,    50,     0,    51,    58,   592,     0,
       0,     0,    59,     0,     4,     5,     6,     7,     0,     0,
       0,   117,   110,   111,   112,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,   113,    28,     0,     0,     0,     0,     0,    55,
       0,     0,    33,     0,     0,     0,    56,     0,     0,     0,
      41,    42,    43,    58,   182,    46,     0,     0,    59,     0,
       4,     5,     6,     7,    50,     0,    51,   117,   110,   111,
     112,   591,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,    53,    54,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    55,
       0,    46,     0,     0,     0,     0,    56,     0,     0,     0,
      50,     0,    51,    58,   715,     0,     0,     0,    59,     0,
       4,     5,     6,     7,     0,     0,     0,   117,   110,   111,
     112,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,    55,     0,     0,    33,     0,
       0,     0,    56,     0,     0,     0,    41,    42,    43,    58,
     717,    46,     0,     0,    59,     0,     4,     5,     6,     7,
      50,     0,    51,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,    58,
     911,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   117,   110,   111,   112,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   113,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   129,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   117,   110,
     111,   112,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,   131,     0,     0,     0,     0,    59,
       4,     5,     6,     7,     0,     0,     0,   117,   110,   111,
     112,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
     133,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,   135,
       0,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   117,   110,   111,   112,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   113,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,    58,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   117,   110,
     111,   112,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,   150,     0,     0,     0,     0,    59,
       4,     5,     6,     7,     0,     0,     0,   117,   110,   111,
     112,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
     152,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,    58,
       0,     0,     0,     0,   166,     4,     5,     6,     7,     0,
       0,     0,   117,   110,   111,   112,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   113,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   247,    46,     0,     0,     0,
      59,     4,     5,   456,     7,    50,     0,    51,   117,   110,
     111,   112,   457,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,   454,     0,     0,     0,     0,    59,
       4,     5,     6,     7,     0,     0,     0,   117,   110,   111,
     112,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   113,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
     458,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   117,   110,   111,   112,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,   611,
       0,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   109,   110,   111,   112,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   636,    46,     0,     0,     0,
      59,     0,     0,     0,     0,    50,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,    59
};

static const yytype_int16 yycheck[] =
{
       2,   236,   278,   210,     0,   487,   633,    15,    14,    10,
      26,    26,   792,    14,   107,    10,    22,   338,    76,    14,
      22,    23,    24,    25,    26,    10,    14,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    76,    14,    10,
      26,    43,    50,    14,    46,    26,    14,    55,    56,    17,
      37,    38,    76,   107,    10,   107,    58,    43,    14,    10,
      11,    12,    43,    14,    56,    57,   124,   122,    60,   127,
       0,   125,   127,   125,    26,    14,    74,    75,    14,    76,
      31,    14,    26,    22,    35,   124,    62,   109,   127,   113,
      58,    43,    94,    95,    96,    97,    98,    99,    31,    43,
     102,   103,   109,   105,   106,   127,    26,    56,    57,    26,
      14,    60,   127,   123,   124,   113,   113,   119,    22,    12,
     127,   127,   123,    43,   904,    64,    43,   129,   123,   131,
     120,   133,   122,   135,   761,   121,   128,    26,   123,   127,
     121,   128,   128,   124,   123,   123,   467,   128,   150,   124,
     152,   126,   123,   155,    43,   157,   158,   127,   160,   161,
      64,   163,   164,   165,   166,   167,   946,   123,   107,   121,
     950,   122,   124,    10,    10,    26,   128,   121,    14,   128,
     124,    10,   121,   185,   186,   187,   188,   189,   190,   191,
     192,   193,    43,   195,   196,   197,   198,   199,   200,   201,
     202,   121,    12,   107,   121,   526,   527,    73,   224,   225,
      14,   991,    14,   127,    70,    71,    72,   121,    22,   999,
      22,   497,   230,   231,   232,   233,   234,    94,    95,    66,
      67,    73,   121,   101,  1014,   124,   122,    66,    67,   128,
     108,   127,    74,    75,     5,   247,    14,   109,   524,   107,
     109,   119,    14,    14,   120,   122,   122,   259,    29,   261,
      64,   263,    64,   121,  1044,   127,   268,   269,   127,   271,
     121,   273,   128,    44,   122,   105,   758,   128,   120,   127,
     122,   113,   284,   285,   286,   287,   288,   289,   290,   305,
     306,   121,   294,   295,   124,   109,    77,    78,    79,    80,
     107,    14,   929,   107,   107,   107,    87,    88,    89,     2,
     312,   313,   308,   127,   316,   122,   318,   121,   121,   121,
     104,   323,   324,    26,   107,   121,    29,    30,   109,   110,
      94,    95,   128,    36,    27,    28,    39,    40,   614,   122,
      43,    44,    10,   107,    37,    38,   105,   349,   350,   351,
     352,    44,    45,    26,   107,   107,    29,    30,   122,   107,
     111,   112,   121,    36,   571,   124,    39,    40,   121,   121,
      43,    26,   648,   121,    29,    30,   102,   122,   114,   115,
     107,    36,   127,    26,    39,    40,    29,    30,    43,    44,
     116,   117,   118,    36,   121,    10,    39,    40,    26,   121,
      43,    29,    30,   679,   680,    10,   128,   121,    36,    14,
     121,    39,    40,   101,   128,    43,    44,   128,   121,    10,
     113,   121,    68,    14,   292,   128,   428,   122,   128,   297,
     432,   121,   127,   435,   436,   303,   121,   122,   128,   674,
     121,   134,   121,   136,   137,   106,   121,   128,   121,   128,
     121,   124,   454,   128,   120,   128,   458,   128,   151,   461,
     153,   463,   121,   465,   121,   121,   121,   469,   470,   128,
     121,   128,   128,   128,   122,   343,   752,   128,   121,   127,
     121,   124,   121,   121,   121,   128,   488,   128,   120,   128,
     128,   128,   185,   121,   121,    26,    14,   499,    29,    30,
     128,   128,   124,    26,   126,    36,    29,    30,    39,    40,
      14,   513,    43,    36,   121,   121,    39,    40,    14,    10,
      43,   128,   128,    14,   125,   801,   802,   121,   121,   121,
     532,   533,   534,   121,   128,   128,   128,   121,   540,   541,
     128,   543,   544,    14,   546,   547,   548,   549,   550,   551,
     552,   553,   124,   421,   126,    81,    82,    83,    84,    85,
      86,   429,   883,   798,   121,   886,   434,    93,   121,   437,
      10,   128,   121,   121,   442,   128,   444,   103,    26,   128,
     128,    29,    30,   121,   586,   587,   588,   121,    36,   121,
     121,    39,    40,   124,   128,    43,    44,   128,   121,   601,
     107,   124,   604,   605,   107,   473,   121,    26,   121,   611,
      29,    30,   121,   128,   482,   128,   618,    36,    53,   128,
      39,    40,   121,   121,    43,    44,   123,   124,   121,   128,
     128,   121,   121,    14,   636,   128,   114,   115,   128,   128,
     121,    22,   335,    24,    25,    27,    28,   128,   650,   651,
     652,   972,    33,    34,   127,   657,   121,   121,   121,   121,
      41,    42,   122,   128,   128,   128,   128,   669,    49,    50,
      51,   121,   121,   121,   121,    39,    40,    58,   128,   128,
     121,   128,   121,   121,   127,   121,   124,   128,   124,   128,
     692,   693,   694,   695,   696,   697,   121,   121,   121,   124,
     124,   124,   121,   121,   572,   121,   574,   124,   124,   126,
      74,    75,   580,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,   127,   121,    10,    93,
     124,    13,   734,    15,    16,   603,   128,   101,   102,   103,
     104,   121,   106,   121,   124,   109,   110,   111,   112,   113,
     114,   121,   116,   117,   118,    92,   120,    94,    95,    96,
      97,    98,    99,   100,   121,   121,   121,   124,   124,   121,
     107,   121,   774,   775,   776,   777,   778,   779,   780,   781,
      10,    11,    12,   120,   110,   122,   128,   789,     7,     8,
       9,   121,    14,   486,   487,    17,    14,   490,   491,   492,
      22,   122,    24,    25,   806,   122,   499,   221,   222,   223,
      14,    33,    34,   121,   507,   508,   509,   107,    10,    41,
      42,    13,    14,    15,    16,   123,   124,    49,    50,    51,
     123,   124,   123,   124,   123,   124,     3,     4,     5,     6,
      10,   843,   844,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   123,   124,   123,   124,   123,   124,
     107,   863,   864,   865,    31,    32,   123,   124,   123,   124,
     738,   123,   124,    14,    41,   123,   124,    14,   880,   123,
     124,   749,    49,    50,    51,   123,   124,    54,   884,   891,
     123,   124,   123,   124,   896,    14,    63,    27,    65,   123,
     124,    27,    28,    22,   121,    24,    25,   123,   124,   123,
     124,   913,   914,    10,    33,    34,   123,   124,   123,   124,
     922,   121,    41,    42,   123,   124,   128,    94,    95,   122,
      49,    50,    51,   123,   124,   123,   124,   123,   124,   807,
     122,   108,   123,   124,   637,   123,   124,   949,   115,   123,
     124,   123,   124,   121,   121,   122,   121,   123,   124,   121,
     127,   963,   123,   124,   107,   658,   968,    92,   121,    94,
      95,    96,    97,    98,    99,   100,    90,   845,   123,   124,
     103,   849,   107,    27,    28,   113,   988,   123,   124,   123,
     124,   993,   994,   995,   121,   120,    14,   122,   204,   205,
     206,   207,   208,   209,   121,   211,   212,   213,   214,   215,
     216,   217,   218,   123,   124,    27,    28,   123,   124,   123,
     124,   123,   124,    42,    43,  1027,  1028,  1029,  1030,  1031,
    1032,  1033,  1034,   228,   229,   121,   121,   121,   121,   121,
     121,   121,   121,   121,  1046,  1047,  1048,   121,   121,   121,
     121,   107,   122,   122,   107,   107,   110,   121,   121,    73,
     128,   121,   107,    10,   121,   758,   121,   128,   127,   762,
     763,   764,   121,   121,   121,   121,   110,   110,   121,   121,
     121,   121,   121,   121,   107,   121,    14,    14,    14,   121,
     783,   128,   121,   121,   787,   126,   126,     3,     4,     5,
       6,   121,    10,    10,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   121,   121,    29,    30,    31,    32,    33,    34,    35,
      36,   121,    10,    10,    10,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
     121,   121,    27,    59,    60,   121,    56,    63,    14,    65,
      14,   854,   855,    69,    14,   128,   121,   121,   121,   121,
      14,   121,   121,   121,    73,    73,   121,   128,   121,   121,
     121,     7,     7,     7,   107,   107,   121,    73,    94,    95,
      10,    10,   123,    14,    68,   128,    14,   121,    14,    14,
     121,   121,   108,   121,   121,   121,    14,   121,   127,   115,
     121,   121,   121,   128,    14,   121,   122,   121,   121,   121,
      14,   127,   128,    14,   121,   121,   121,   128,   121,   121,
     121,   121,   121,   988,   522,   123,   574,   458,   220,   226,
      -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   945,    -1,    -1,    -1,   949,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
     983,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1045,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   121,   122,     3,     4,     5,     6,   127,
     128,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      49,    50,    51,   121,   122,    54,    -1,    -1,    -1,   127,
     128,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,
      50,    51,   121,   122,    54,    -1,    -1,    -1,   127,   128,
      -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,   121,   122,    54,    -1,    -1,    -1,   127,   128,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
     121,   122,     3,     4,     5,     6,   127,   128,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    69,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,
     121,   122,    54,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,   121,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
     123,    -1,    -1,    -1,   127,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    41,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    49,    50,    51,   122,   123,    54,    -1,    -1,
     127,    -1,     3,     4,     5,     6,    63,    -1,    65,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,   116,
      -1,    -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,
     127,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      41,    -1,    -1,    -1,   115,    -1,    -1,    -1,    49,    50,
      51,   122,   123,    54,    -1,    -1,   127,    -1,     3,     4,
       5,     6,    63,    -1,    65,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,
      65,   122,   123,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    49,    50,    51,    -1,   122,    54,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    -1,    68,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,
      -1,   127,     3,     4,     5,     6,    63,    -1,    65,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    63,    -1,    65,   122,   123,    -1,    -1,    -1,
     127,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      41,    -1,    -1,    -1,   115,    -1,    -1,    -1,    49,    50,
      51,   122,   123,    54,    -1,    -1,   127,    -1,     3,     4,
       5,     6,    63,    -1,    65,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,
      65,   122,   123,    -1,    -1,    -1,   127,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    41,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    49,    50,    51,   122,   123,    54,
      -1,    -1,   127,    -1,     3,     4,     5,     6,    63,    -1,
      65,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    63,    -1,    65,   122,   123,    -1,
      -1,    -1,   127,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    41,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      49,    50,    51,   122,   123,    54,    -1,    -1,   127,    -1,
       3,     4,     5,     6,    63,    -1,    65,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,   108,
      -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      63,    -1,    65,   122,   123,    -1,    -1,    -1,   127,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    41,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    49,    50,    51,   122,
     123,    54,    -1,    -1,   127,    -1,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
     123,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,   127
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   130,   131,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    65,    69,    94,    95,   108,   115,   121,   122,   127,
     132,   133,   135,   136,   137,   138,   139,   141,   142,   144,
     145,   148,   149,   152,   157,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   174,   175,
     177,    92,    94,    95,    96,    97,    98,    99,   100,   107,
     120,   122,   107,   125,   109,   127,   107,   120,   122,    10,
      11,    12,    13,    31,   157,   170,    14,    10,   121,   122,
     159,   176,    10,    11,    12,    14,    31,    35,   122,   122,
     159,   122,   159,   122,   159,   122,   159,   159,   166,    14,
     157,   157,   159,   121,   159,    10,    14,   159,    12,    12,
     122,   159,   122,   159,   157,   122,   157,    26,    43,   121,
      26,    43,   121,   127,   158,   158,   127,   158,   159,   157,
     157,   159,    14,    14,    14,    31,   170,    14,    10,    10,
     170,   170,   123,   159,    68,   131,    26,    29,    30,    36,
      39,    40,    43,    44,   121,    26,    29,    30,    36,    39,
      40,    43,    44,   121,    77,    78,    79,    80,    87,    88,
      89,   109,   110,    81,    82,    83,    84,    85,    86,    93,
     103,   111,   112,   101,    74,    75,   113,   104,   114,   115,
     102,   116,   117,   118,   106,   120,   120,    14,   159,   159,
     159,   159,   159,   159,    14,   123,   176,   122,   159,   116,
     159,    14,   159,   159,    14,    17,    58,   123,   176,   125,
     109,   127,   121,   107,   123,   159,   176,   121,   124,   107,
     121,   107,   121,   107,   121,    10,    11,    12,    14,    10,
      14,    10,   172,   159,    26,    29,    30,    36,    39,    40,
      43,   121,   124,   159,    26,    43,   121,   124,   159,   157,
     159,   157,    76,   124,   157,    74,    75,   122,   127,   146,
     121,   121,   107,   107,   127,   109,   127,   109,   127,   159,
     157,   159,   157,    29,    44,    23,   159,   159,   159,   159,
     159,   159,   159,   159,   159,    53,   150,   127,   122,    14,
      62,   127,   123,   124,   128,    10,    11,    12,    13,    24,
      25,    33,    34,    47,    48,   128,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   162,   162,   162,   162,   162,   162,   160,
     162,   162,   162,   162,   162,   162,   162,   162,     7,     8,
       9,   163,   165,   165,   165,   166,   166,   167,   168,   169,
     169,   170,   170,   170,   170,   170,    14,    22,    24,    25,
      33,    34,    41,    42,    49,    50,    51,   173,    17,   173,
     121,   122,   121,   121,   121,   121,   121,   121,   107,   122,
     123,   159,   105,   121,   124,   114,   115,   124,   126,   110,
     128,   121,   122,    14,   122,   123,   159,    14,   159,   159,
     121,   123,   159,   159,   122,   159,     5,    14,   122,   155,
     159,   107,   121,   107,   121,   107,   121,   122,   146,   107,
     107,   123,   124,   124,   159,   159,   159,   159,   159,   159,
     159,   176,   124,   159,   159,   176,   123,    27,    28,   134,
     123,    10,   176,   166,   166,    10,    14,   123,   156,   131,
     159,   159,   143,    14,   159,    14,   159,   123,    27,   123,
     159,   159,    10,   121,   121,   121,   121,   121,   128,   157,
      14,    22,   127,   151,   123,   156,   122,   122,   153,   123,
     176,   120,   125,   127,   107,   159,   159,   159,   128,   159,
      26,    43,   128,    26,    43,   128,    26,    29,    30,    36,
      39,    40,    43,    44,   128,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,    90,   122,    14,   122,   123,   176,   159,   123,   176,
     124,   159,   176,   159,   159,   176,   107,   107,   107,   123,
     176,    14,   123,   154,   176,   126,   110,   128,   121,   121,
     159,   105,   121,   124,    73,    73,   155,   121,   124,   121,
     159,   122,   159,   159,   123,   156,   159,   159,   107,    10,
     176,   121,   121,   121,   121,   121,   121,   121,   121,   176,
     121,   121,   121,   157,   133,   157,   122,   159,   157,   157,
      76,   157,    76,    76,    10,    66,    67,   146,   123,   124,
      24,    25,    34,   128,   159,   121,   121,    37,    38,   128,
     110,   128,   110,   128,   157,   157,   157,   121,   121,   107,
     159,    14,    14,   151,    56,    57,    60,   128,   146,   123,
     123,   156,   156,    70,    71,    72,   128,   123,    14,   159,
     159,   159,    26,    30,    36,    39,    40,    43,   128,   128,
     128,   128,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   160,   123,   176,   123,   154,   176,
     121,   123,   121,   123,   176,   121,   121,   126,   126,   126,
     159,   159,   159,   123,    73,   123,   124,   123,   124,   159,
     176,   159,   159,   123,     5,    14,   121,   159,   121,   124,
     121,   146,   123,   121,   121,   159,   123,   123,    27,   134,
     159,   157,    10,    10,    10,    10,    10,   146,    10,    14,
     159,   159,   128,   159,    26,    29,    30,    36,    39,    40,
      43,    44,   128,   159,   157,   121,   121,    27,   159,   121,
     128,    58,   173,    10,    13,    14,    15,    16,    56,   146,
     146,   123,   123,    14,    14,    14,   107,   122,   126,   128,
     128,   159,   159,   159,   159,   159,   159,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   123,
     123,   123,   121,   123,   121,   121,   121,   159,    14,   176,
     121,   121,   121,    73,    73,   124,   176,   146,   121,   124,
     121,   121,   133,   157,   123,    27,   134,   157,   157,   157,
      10,    66,    67,    26,    30,    43,   128,   128,   128,   159,
     159,   159,   159,   159,   159,   159,   159,   157,   128,   157,
     121,   140,   159,   122,   127,   147,   122,   147,    14,    22,
      64,   107,   121,    14,    22,    64,   107,   121,    10,    13,
      15,    16,   121,   121,   173,   146,   146,     7,     7,     7,
     159,   123,   176,   107,   107,   128,   128,   128,   128,   128,
     128,   121,    73,   123,   159,   159,   176,   121,   176,   157,
     157,    10,    10,   159,   159,   159,   128,   128,   128,   128,
     128,   128,   128,   128,   159,   123,   123,   156,    68,   131,
     123,   156,    14,    14,   121,   159,    14,    14,   121,   159,
      14,    22,    64,   107,   121,    14,    22,    64,   107,   121,
     121,   121,   122,   147,   128,   128,   123,   159,   159,   159,
     121,   123,   121,    27,   134,   128,   128,   128,   121,   157,
     147,   123,   128,    24,    25,    34,   128,   159,   147,   123,
     121,   121,   121,   121,   121,   121,    14,    14,   121,   159,
      14,    14,   121,   159,   123,   156,   128,   128,   128,   121,
     157,   140,   147,   159,   159,   128,   159,    26,    29,    30,
      36,    39,    40,    43,    44,   128,   147,   121,   121,   121,
     121,   121,   121,   147,   123,   123,    26,    30,    43,   128,
     128,   128,   159,   159,   159,   159,   159,   159,   159,   159,
     147,   157,   159,   159,   159,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   129,   130,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   133,   133,   133,   133,   133,   133,
     133,   133,   134,   134,   134,   134,   134,   134,   135,   135,
     136,   136,   136,   136,   137,   137,   138,   138,   138,   139,
     139,   139,   140,   141,   141,   141,   141,   141,   141,   142,
     142,   143,   143,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   148,   149,   150,   150,   150,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     152,   153,   153,   153,   153,   154,   154,   155,   155,   155,
     155,   156,   156,   156,   156,   156,   156,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   158,   159,   159,   159,   160,   160,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   162,   162,   163,
     163,   163,   164,   164,   165,   165,   165,   166,   166,   167,
     167,   168,   168,   168,   169,   169,   169,   169,   169,   170,
     170,   170,   170,   170,   171,   171,   172,   172,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   174,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   176,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177
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
       3,     4,     6,     6,     6,     6,     6,     6,     6,     6,
       5,     7,     7,     7,     7,     7,     7,     5,     9,     8,
       8,     6,     9,     9,     5,     5,     4,     4,     4,     6,
       6,     6,     6,     3,     3,     2,     1,     5,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       2,     3,     3,     2,     3,     1,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     4,     6,     6,     6,     4,     4,
       5,     5,     1,     1,     1,     1,     2,     3,     4,     5,
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
#line 376 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2772 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 386 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2778 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 387 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2784 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 391 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2790 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 393 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2796 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 395 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2802 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 397 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2810 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 401 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2816 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 403 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2822 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 405 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2828 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 407 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2836 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 411 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2843 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 414 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2851 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 418 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2857 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 420 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2865 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 424 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2872 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 427 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2880 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 431 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2886 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 433 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2892 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 435 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2898 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 437 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2904 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 439 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2910 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 441 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2918 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 445 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2926 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 449 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2932 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 451 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2938 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 453 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2944 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 455 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2950 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 457 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2956 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 459 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2962 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 461 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2968 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 463 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2974 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 465 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2980 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 467 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2986 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 469 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2992 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 471 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2998 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 473 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3004 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 475 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3010 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 477 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3017 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 480 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3024 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 483 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3030 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 485 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3037 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 488 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3044 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 491 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3050 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 493 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3056 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 495 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3062 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 497 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 3068 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 499 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3077 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 504 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3085 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 508 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3093 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 512 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3102 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 517 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3110 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 520 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3116 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 522 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3122 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3128 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 526 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3134 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 528 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3140 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 530 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3147 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 533 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3153 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 535 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3159 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 537 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3165 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 539 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3172 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 542 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3179 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 545 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3186 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 548 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3193 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 551 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3200 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 554 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3206 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 556 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3212 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 558 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3218 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 560 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3224 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 562 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3231 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 565 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3237 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 567 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3243 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 569 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3249 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 571 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3256 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 574 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3263 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 577 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3271 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 581 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3278 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 584 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3284 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 586 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3290 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 588 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3296 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 590 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3303 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 593 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3310 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 596 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3317 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 599 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3324 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 602 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3331 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 605 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3338 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 608 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3345 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 610 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3351 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 611 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3357 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 612 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3363 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 613 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3369 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 614 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3375 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 615 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3381 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3387 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3393 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3399 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 623 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3405 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 624 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3411 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 625 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3417 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 626 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3423 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 627 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3429 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 628 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3435 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 629 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3441 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3447 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 633 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3453 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3459 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 637 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3465 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 638 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3471 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 639 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3477 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 640 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3483 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 641 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3489 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 645 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3495 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3501 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 649 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3507 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3513 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 653 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3519 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 655 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3525 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3531 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 659 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3537 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3543 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3549 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3555 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 669 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3561 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3567 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 673 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3573 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 677 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3579 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 679 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3585 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3591 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 685 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3597 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 687 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3603 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 689 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3609 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3615 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3621 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3627 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3633 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3639 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 707 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3645 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 709 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3651 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 711 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3657 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 714 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3663 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 718 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3673 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 724 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3683 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 730 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3692 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 735 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3702 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 741 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3711 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 746 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3718 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 751 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3730 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 759 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3743 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 769 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3749 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 771 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3757 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 777 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3769 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 785 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3779 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 791 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3789 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 797 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3801 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 805 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3811 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 811 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3821 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 817 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3834 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 826 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3845 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 833 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3858 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 842 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3871 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 851 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3882 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 860 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3893 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 867 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3902 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 873 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3908 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 875 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3915 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 878 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3922 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 881 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3928 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 883 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3934 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 885 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3940 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 887 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3946 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3952 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 891 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3958 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 893 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3964 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 895 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3971 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 898 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3977 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 900 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3983 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 902 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3989 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 904 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3996 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 907 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4003 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 910 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4011 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 915 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4017 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 916 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4023 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 918 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4030 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 921 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4037 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 924 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4043 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 926 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4049 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 928 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4055 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 930 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4061 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 932 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4067 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 934 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4073 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 936 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4079 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 938 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4086 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 941 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4092 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 943 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4098 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 945 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4104 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 947 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4111 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 950 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4118 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 953 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4126 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 959 "raku.y"
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
#line 4144 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 975 "raku.y"
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
#line 4161 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 989 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4167 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 991 "raku.y"
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
#line 4183 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1003 "raku.y"
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
#line 4199 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1016 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4205 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1018 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4212 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1021 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4219 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1024 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4226 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1027 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4233 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1030 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4240 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1033 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4247 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1036 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4254 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1039 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4261 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1042 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4268 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1045 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4275 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1048 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4282 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1051 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4289 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1054 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4300 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1061 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4311 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1068 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4322 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1075 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4333 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1082 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4341 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1086 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4349 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1090 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4357 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1094 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4365 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1098 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4376 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1105 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4387 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1112 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4398 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1119 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4409 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1126 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4423 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1136 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4435 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1144 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4447 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1152 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4460 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1161 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4471 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1168 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4482 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1175 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4496 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1185 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4508 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1193 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4520 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1203 "raku.y"
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
#line 4537 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1217 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4543 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1219 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4552 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1224 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4561 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1229 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4570 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1236 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4578 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1240 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4586 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1246 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4592 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1248 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4598 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1250 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4604 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1252 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4610 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1255 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4616 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1256 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4622 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1257 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4628 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1258 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4634 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1259 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4640 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1260 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4646 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1261 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4652 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1262 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4658 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list '}'  */
#line 1265 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4664 "raku.tab.c"
    break;

  case 260: /* block: '{' YADA '}'  */
#line 1266 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4670 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr '}'  */
#line 1268 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4676 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1270 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4682 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1272 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4688 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1274 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4694 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1276 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4700 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1278 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4707 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1281 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4713 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1283 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4719 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1285 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4725 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1287 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4731 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1289 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4738 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1292 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4745 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1295 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4753 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1299 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4760 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1302 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4767 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1305 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4774 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1308 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4780 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1310 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4788 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1314 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4795 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1317 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4802 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1320 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4809 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1323 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4816 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1326 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4823 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1329 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4829 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1331 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4835 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1333 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4841 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_LAST '}'  */
#line 1335 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4847 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1337 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4853 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1339 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4859 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1341 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4865 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1343 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4871 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1345 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4877 "raku.tab.c"
    break;

  case 293: /* closure: '{' expr '}'  */
#line 1348 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4883 "raku.tab.c"
    break;

  case 294: /* expr: VAR_SCALAR '=' expr  */
#line 1351 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4889 "raku.tab.c"
    break;

  case 295: /* expr: KW_GATHER block  */
#line 1352 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4899 "raku.tab.c"
    break;

  case 296: /* expr: tern_expr  */
#line 1357 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4905 "raku.tab.c"
    break;

  case 297: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1361 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4911 "raku.tab.c"
    break;

  case 298: /* tern_expr: cmp_expr  */
#line 1362 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4917 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1365 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4923 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1366 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4929 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1367 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4935 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1368 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4941 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1369 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4947 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1370 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4953 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1371 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4959 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1372 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4965 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1373 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4971 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1374 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4977 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1375 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4983 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1376 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4989 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1377 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4995 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1378 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5001 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1380 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5011 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1386 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5021 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1392 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5031 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr  */
#line 1397 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5037 "raku.tab.c"
    break;

  case 317: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1400 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5043 "raku.tab.c"
    break;

  case 318: /* divis_expr: jct_expr  */
#line 1401 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5049 "raku.tab.c"
    break;

  case 319: /* jct_expr: jct_expr '|' range_expr  */
#line 1404 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5055 "raku.tab.c"
    break;

  case 320: /* jct_expr: jct_expr '&' range_expr  */
#line 1405 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5061 "raku.tab.c"
    break;

  case 321: /* jct_expr: dor_expr  */
#line 1406 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5067 "raku.tab.c"
    break;

  case 322: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1410 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5073 "raku.tab.c"
    break;

  case 323: /* dor_expr: range_expr  */
#line 1411 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5079 "raku.tab.c"
    break;

  case 324: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1414 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5085 "raku.tab.c"
    break;

  case 325: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1415 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5091 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr  */
#line 1416 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5097 "raku.tab.c"
    break;

  case 327: /* add_expr: add_expr '~' repl_expr  */
#line 1419 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5103 "raku.tab.c"
    break;

  case 328: /* add_expr: repl_expr  */
#line 1420 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5109 "raku.tab.c"
    break;

  case 329: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1423 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5115 "raku.tab.c"
    break;

  case 330: /* repl_expr: addsub_expr  */
#line 1424 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5121 "raku.tab.c"
    break;

  case 331: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1427 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5127 "raku.tab.c"
    break;

  case 332: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1428 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5133 "raku.tab.c"
    break;

  case 333: /* addsub_expr: mul_expr  */
#line 1429 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5139 "raku.tab.c"
    break;

  case 334: /* mul_expr: mul_expr '*' unary_expr  */
#line 1432 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5145 "raku.tab.c"
    break;

  case 335: /* mul_expr: mul_expr '/' unary_expr  */
#line 1433 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5151 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '%' unary_expr  */
#line 1434 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5157 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1435 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5163 "raku.tab.c"
    break;

  case 338: /* mul_expr: unary_expr  */
#line 1436 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5169 "raku.tab.c"
    break;

  case 339: /* unary_expr: '-' unary_expr  */
#line 1439 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5175 "raku.tab.c"
    break;

  case 340: /* unary_expr: '!' unary_expr  */
#line 1440 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5181 "raku.tab.c"
    break;

  case 341: /* unary_expr: CARET unary_expr  */
#line 1441 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5187 "raku.tab.c"
    break;

  case 342: /* unary_expr: OP_REDUCE unary_expr  */
#line 1443 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5196 "raku.tab.c"
    break;

  case 343: /* unary_expr: pow_expr  */
#line 1447 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5202 "raku.tab.c"
    break;

  case 344: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1450 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 345: /* pow_expr: postfix_expr  */
#line 1451 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5214 "raku.tab.c"
    break;

  case 346: /* scalar_list: VAR_SCALAR  */
#line 1454 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5220 "raku.tab.c"
    break;

  case 347: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1455 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5226 "raku.tab.c"
    break;

  case 348: /* meth_name: IDENT  */
#line 1458 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5232 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_SORT  */
#line 1459 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5238 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_MAP  */
#line 1460 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5244 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_GREP  */
#line 1461 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5250 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_SAY  */
#line 1462 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5256 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_PRINT  */
#line 1463 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5262 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_TAKE  */
#line 1464 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5268 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_RETURN  */
#line 1465 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5274 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_EXISTS  */
#line 1466 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5280 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_DELETE  */
#line 1467 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5286 "raku.tab.c"
    break;

  case 358: /* meth_name: TESTOP  */
#line 1468 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5292 "raku.tab.c"
    break;

  case 359: /* postfix_expr: call_expr  */
#line 1470 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5298 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '(' arg_list ')'  */
#line 1473 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5307 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '(' ')'  */
#line 1477 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5313 "raku.tab.c"
    break;

  case 362: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1479 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5322 "raku.tab.c"
    break;

  case 363: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1484 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5328 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1486 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5338 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1492 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5348 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1498 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5356 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' KW_NEW  */
#line 1502 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5364 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' IDENT  */
#line 1506 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5373 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1511 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5384 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1518 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5393 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' CARET IDENT  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5403 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' CARET IDENT  */
#line 1529 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5413 "raku.tab.c"
    break;

  case 373: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1535 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5424 "raku.tab.c"
    break;

  case 374: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1542 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5435 "raku.tab.c"
    break;

  case 375: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1549 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5444 "raku.tab.c"
    break;

  case 376: /* call_expr: atom '.' meth_name  */
#line 1554 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5453 "raku.tab.c"
    break;

  case 377: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1559 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5464 "raku.tab.c"
    break;

  case 378: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5473 "raku.tab.c"
    break;

  case 379: /* call_expr: call_expr '.' meth_name  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5482 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_DIE expr  */
#line 1576 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5488 "raku.tab.c"
    break;

  case 381: /* call_expr: KW_MAP closure expr  */
#line 1578 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5494 "raku.tab.c"
    break;

  case 382: /* call_expr: KW_GREP closure expr  */
#line 1580 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5500 "raku.tab.c"
    break;

  case 383: /* call_expr: KW_SORT expr  */
#line 1582 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5506 "raku.tab.c"
    break;

  case 384: /* call_expr: KW_SORT closure expr  */
#line 1584 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5512 "raku.tab.c"
    break;

  case 385: /* call_expr: atom  */
#line 1585 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5518 "raku.tab.c"
    break;

  case 386: /* arg_list: expr  */
#line 1588 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5524 "raku.tab.c"
    break;

  case 387: /* arg_list: arg_list ',' expr  */
#line 1589 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5530 "raku.tab.c"
    break;

  case 388: /* arg_list: arg_list ','  */
#line 1590 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5536 "raku.tab.c"
    break;

  case 389: /* atom: LIT_INT  */
#line 1593 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5542 "raku.tab.c"
    break;

  case 390: /* atom: LIT_FLOAT  */
#line 1594 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5548 "raku.tab.c"
    break;

  case 391: /* atom: LIT_STR  */
#line 1595 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5554 "raku.tab.c"
    break;

  case 392: /* atom: WORDLIST  */
#line 1597 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5566 "raku.tab.c"
    break;

  case 393: /* atom: LIT_INTERP_STR  */
#line 1604 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5572 "raku.tab.c"
    break;

  case 394: /* atom: VAR_SCALAR  */
#line 1605 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5578 "raku.tab.c"
    break;

  case 395: /* atom: OP_INC VAR_SCALAR  */
#line 1606 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5584 "raku.tab.c"
    break;

  case 396: /* atom: OP_DEC VAR_SCALAR  */
#line 1607 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5590 "raku.tab.c"
    break;

  case 397: /* atom: VAR_SCALAR OP_INC  */
#line 1608 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5596 "raku.tab.c"
    break;

  case 398: /* atom: VAR_SCALAR OP_DEC  */
#line 1609 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5602 "raku.tab.c"
    break;

  case 399: /* atom: VAR_ARRAY  */
#line 1610 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5608 "raku.tab.c"
    break;

  case 400: /* atom: VAR_HASH  */
#line 1611 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5614 "raku.tab.c"
    break;

  case 401: /* atom: VAR_CAPTURE  */
#line 1613 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5622 "raku.tab.c"
    break;

  case 402: /* atom: VAR_FH  */
#line 1617 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5630 "raku.tab.c"
    break;

  case 403: /* atom: VAR_NAMED_CAPTURE  */
#line 1621 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5637 "raku.tab.c"
    break;

  case 404: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1624 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5643 "raku.tab.c"
    break;

  case 405: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1626 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5649 "raku.tab.c"
    break;

  case 406: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1628 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5655 "raku.tab.c"
    break;

  case 407: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1630 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5661 "raku.tab.c"
    break;

  case 408: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1632 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5667 "raku.tab.c"
    break;

  case 409: /* atom: VAR_HASH '{' expr '}'  */
#line 1634 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5673 "raku.tab.c"
    break;

  case 410: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1636 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5679 "raku.tab.c"
    break;

  case 411: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1638 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5685 "raku.tab.c"
    break;

  case 412: /* atom: IDENT  */
#line 1639 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5691 "raku.tab.c"
    break;

  case 413: /* atom: VAR_TWIGIL  */
#line 1641 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5699 "raku.tab.c"
    break;

  case 414: /* atom: VAR_ARRAY_TWIGIL  */
#line 1645 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5707 "raku.tab.c"
    break;

  case 415: /* atom: VAR_HASH_TWIGIL  */
#line 1649 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5715 "raku.tab.c"
    break;

  case 416: /* atom: '(' ')'  */
#line 1652 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5721 "raku.tab.c"
    break;

  case 417: /* atom: '(' expr ')'  */
#line 1653 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5727 "raku.tab.c"
    break;

  case 418: /* atom: '(' expr ',' ')'  */
#line 1655 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5733 "raku.tab.c"
    break;

  case 419: /* atom: '(' expr ',' arg_list ')'  */
#line 1657 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5740 "raku.tab.c"
    break;

  case 420: /* atom: block  */
#line 1659 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5746 "raku.tab.c"
    break;

  case 421: /* atom: KW_SUB block  */
#line 1660 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5752 "raku.tab.c"
    break;


#line 5756 "raku.tab.c"

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

#line 1662 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
