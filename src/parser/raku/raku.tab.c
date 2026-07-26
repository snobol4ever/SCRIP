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
#define YYLAST   3630

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  416
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1061

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
     402,   404,   406,   410,   413,   417,   419,   421,   423,   425,
     427,   431,   435,   437,   439,   441,   443,   445,   447,   449,
     451,   453,   455,   457,   459,   461,   463,   466,   469,   471,
     474,   477,   479,   481,   483,   485,   490,   494,   498,   503,
     507,   508,   510,   512,   514,   516,   519,   521,   523,   525,
     528,   531,   534,   537,   540,   542,   544,   546,   548,   551,
     553,   555,   557,   560,   563,   567,   570,   572,   574,   576,
     579,   582,   585,   588,   591,   594,   597,   598,   599,   600,
     601,   602,   603,   605,   607,   609,   611,   612,   613,   614,
     615,   616,   617,   619,   621,   623,   625,   626,   627,   628,
     631,   633,   635,   637,   639,   641,   643,   645,   649,   651,
     653,   655,   657,   659,   663,   665,   669,   671,   673,   675,
     679,   681,   685,   687,   689,   693,   695,   697,   701,   704,
     710,   716,   721,   727,   732,   737,   745,   756,   757,   763,
     771,   777,   783,   791,   797,   803,   812,   819,   828,   837,
     846,   853,   860,   861,   864,   867,   869,   871,   873,   875,
     877,   879,   881,   884,   886,   888,   890,   893,   896,   902,
     903,   904,   907,   910,   912,   914,   916,   918,   920,   922,
     924,   927,   929,   931,   933,   936,   939,   945,   961,   976,
     977,   989,  1003,  1004,  1007,  1010,  1013,  1016,  1019,  1022,
    1025,  1028,  1031,  1034,  1037,  1040,  1047,  1054,  1061,  1068,
    1072,  1076,  1080,  1084,  1091,  1098,  1105,  1112,  1122,  1130,
    1138,  1147,  1154,  1161,  1171,  1179,  1189,  1204,  1205,  1210,
    1215,  1222,  1226,  1232,  1234,  1236,  1238,  1242,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1252,  1253,  1254,  1256,  1258,
    1260,  1262,  1264,  1267,  1269,  1271,  1273,  1275,  1278,  1281,
    1285,  1288,  1291,  1294,  1296,  1300,  1303,  1306,  1309,  1312,
    1315,  1317,  1319,  1321,  1323,  1325,  1327,  1329,  1331,  1335,
    1338,  1339,  1344,  1347,  1349,  1352,  1353,  1354,  1355,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,
    1372,  1378,  1384,  1387,  1388,  1391,  1392,  1393,  1396,  1398,
    1401,  1402,  1403,  1406,  1407,  1410,  1411,  1414,  1415,  1416,
    1419,  1420,  1421,  1422,  1423,  1426,  1427,  1428,  1429,  1434,
    1437,  1438,  1441,  1442,  1445,  1446,  1447,  1448,  1449,  1450,
    1451,  1452,  1453,  1454,  1455,  1457,  1459,  1464,  1465,  1470,
    1472,  1478,  1484,  1488,  1492,  1497,  1504,  1509,  1515,  1521,
    1528,  1535,  1540,  1545,  1552,  1557,  1562,  1564,  1566,  1568,
    1570,  1572,  1575,  1576,  1579,  1580,  1581,  1582,  1590,  1591,
    1592,  1593,  1594,  1595,  1596,  1597,  1598,  1602,  1606,  1609,
    1611,  1613,  1615,  1617,  1619,  1621,  1623,  1625,  1626,  1630,
    1634,  1638,  1639,  1640,  1642,  1645,  1646
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

#define YYPACT_NINF (-675)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-416)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -675,    31,  1552,  -675,  -675,  -675,  -675,  -675,   571,  -110,
     -42,   -47,   -34,  -675,  -675,   873,  -675,  -675,  -675,    85,
    1621,    37,  2765,  2834,  2890,  2959,  3015,    12,   -63,  3015,
    1690,   287,  3015,    92,   134,  3084,  3140,   -63,   204,    58,
      88,    79,    79,  3209,   -63,   -63,  3015,   157,   236,   105,
     873,  -675,   257,   289,   408,   873,   873,  -675,  1759,   292,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
     543,  -675,  -675,  -675,  3510,   565,  -675,   720,   311,    65,
     329,  -675,   136,   334,   266,   198,  -675,  -675,   391,   363,
     402,   433,  -675,  -675,  3015,  3015,  3015,  3015,  3015,  3015,
     522,  1815,  1885,   544,  3015,  3015,   230,  1941,   151,   437,
     -18,  -675,   -63,  -675,  -675,   464,   210,  -675,  2011,  -675,
     406,   119,   170,   291,   665,   588,   369,   626,  1759,   525,
    1759,    19,  1759,   -63,  1759,   -63,   138,   154,   214,  -675,
    -675,   510,  -675,   529,   549,   551,   535,    64,   142,  1759,
     -63,  1759,   -63,   106,  2067,  -675,  3015,  3015,  -675,  3015,
    3015,  -675,  3015,  3015,  3015,  2136,  3015,  -675,   620,  -675,
    -675,  -675,   569,   586,    36,  -675,   587,  -675,  -675,  -675,
    -675,  -675,   285,   590,  1047,  3015,  3015,  3015,  3015,  3015,
    3015,  3015,  3015,  -675,  3015,  3015,  3015,  3015,  3015,  3015,
    3015,  3015,  -675,   873,   873,   873,   873,   873,   873,   873,
     873,   873,   873,   873,   873,   873,   873,   873,   724,   873,
     873,   873,   873,   873,   873,   873,   873,   873,   873,   873,
     873,   873,   873,   873,   771,   687,   341,   603,   606,   625,
     647,   652,   669,   171,  -675,   487,   574,   113,   676,   666,
     681,   684,   796,   693,  -675,   611,  1885,   817,  3015,  -675,
    3015,   722,   285,   616,  -675,  3015,  3015,  -675,  3265,  -675,
    3334,  -675,   298,   303,   318,   237,   745,   751,  -675,   660,
     695,  3015,  3015,  3015,  3015,  3015,  3015,  3015,  -675,  3015,
     700,  3015,  3015,  -675,  3015,   702,   807,   713,  -675,   864,
    3015,  -675,   873,   873,    -3,  -675,  -675,  -675,  -675,  3015,
    3015,  -675,   880,  3015,   889,  3015,   715,   885,   717,  -675,
    3015,  3015,   921,   814,   831,   836,   842,   863,   809,  -675,
    -675,  -675,   -63,    -6,  -675,    -1,   820,   865,  -675,  -675,
    2205,  -675,   821,   861,   165,   883,  2765,  2834,  3015,   738,
     175,   314,  -675,   294,   870,   871,   872,   875,   876,   877,
     878,   881,   882,   884,   886,   887,   888,   897,   898,   899,
     891,   891,   891,   891,   891,   891,   911,   891,   891,   891,
     891,   891,   891,   891,   891,  -675,  -675,  -675,    65,  -675,
    -675,  -675,   893,   893,   334,   266,   198,   198,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,   900,   990,   901,  -675,  2261,  -675,
    -675,  -675,  -675,  -675,  -675,  3015,  2331,  -675,  3015,  3015,
    3015,   914,   918,   919,  -675,  2387,  -675,  2457,  -675,   365,
     928,   896,  -675,  -675,   906,  -675,   907,  1759,   117,   974,
     -61,  2513,   424,   927,  3015,  -675,  3390,  -675,  3015,  -675,
       4,  -675,  3015,  3015,   942,  1044,  2205,   953,   954,   963,
     964,   965,   966,   982,   425,  2205,   983,   984,   566,   -63,
      14,  3459,  -675,   -63,   -63,   148,   -30,   -23,  -675,    45,
     981,   721,  1174,   988,   992,    41,  1001,   986,  1005,   989,
     -63,   -63,   -63,   997,   998,  1013,  3015,  -675,  -675,  -675,
    -675,  -675,  -675,  1107,  1108,  -675,     9,   981,   727,    15,
     418,     5,  -675,   731,  1109,  1885,  3015,  3015,   313,    44,
     -13,  -675,   200,  3015,  3015,  -675,  3015,  3015,  -675,  3015,
    3015,  3015,  3015,  3015,  3015,  3015,  3015,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,   873,  2583,  -675,  2639,  1003,   733,
    1004,  -675,   744,  1000,  1002,   474,  3015,  3015,  3015,  -675,
     746,    80,  -675,   749,   758,  -675,  -675,  -675,  -675,  -675,
     772,  3015,  -675,  3015,  3015,  3015,   776,  -675,    75,  -675,
    1006,  1759,   581,  1008,   981,   778,  1009,  1010,  3015,  -675,
     783,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,   785,
    -675,  -675,  -675,   834,  -675,  -675,  1759,   -63,  -675,  -675,
    1122,  -675,  1123,  1124,  -675,  1125,  1126,  -675,   981,   502,
    2765,  2834,  1363,  -675,   346,  -675,  -675,  3015,   -63,  -675,
    -675,  -675,  1016,  1017,  1112,  -675,  -675,  -675,  -675,  3015,
    1019,  -675,  -675,    50,   593,   762,  1087,  -675,  -675,   981,
     981,   787,   802,  1130,  1132,  1133,  -675,  -675,   187,   621,
    1020,   201,  3015,  3015,  3015,  3015,  3015,  3015,  -675,  -675,
    -675,  -675,   240,   242,   308,   312,   322,   323,   325,   327,
     336,   337,   338,   347,  -675,  -675,   806,  -675,   810,   816,
    -675,  1029,  -675,  -675,  -675,  -675,  -675,  1030,  1031,  1032,
    -675,  3015,  -675,  1140,  -675,  2205,  1035,   582,  -675,  -675,
    1036,  1085,  1086,  -675,   824,  -675,  3015,  -675,  -675,   981,
    -675,  -675,   589,  1039,  1040,    14,  -675,   826,   917,   -63,
     -63,   -63,  -675,  -675,  -675,  -675,   108,   348,    91,  -675,
     350,  3015,  3015,  3015,  3015,  3015,  3015,  3015,  3015,  -675,
     -63,  1037,  -675,  -675,   -63,  1042,  3015,  -675,   506,   552,
      49,    59,   801,  1043,  1045,   771,  -675,  -675,   981,   981,
    1160,  1163,  1164,  3015,  2709,  1065,  1066,  -675,   352,   356,
     360,   366,   372,   381,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675,  -675,  -675,  -675,  1103,   830,  -675,  -675,  -675,  3015,
    3015,  2205,   601,  -675,  -675,  3015,  -675,  -675,  -675,  -675,
     -63,   -63,  -675,  -675,  -675,  -675,  -675,  1171,  1172,  3015,
    3015,  3015,  -675,  -675,  -675,   385,   386,   387,   389,   390,
     400,   405,   429,  -675,  -675,  -675,  3015,  1078,  -675,    22,
    1134,  -675,    25,  -675,  1197,  1198,  1092,  3015,  -675,  1200,
    1216,  1110,  3015,  -675,    60,   191,  1111,  1114,  -675,  -675,
     570,  -675,  -675,  -675,  -675,  -675,   438,  1113,   832,  3015,
    3015,  -675,  -675,  -675,  -675,  -675,  -675,  3015,  1115,  -675,
    -675,   835,  -675,   602,   934,  -675,  -675,  -675,   439,   442,
     450,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  1117,
     -63,  1119,   846,  1116,  1294,  1119,   849,  1121,  1127,  -675,
    1128,  1129,  1131,  -675,  1135,  1226,  1233,  1136,  3015,  -675,
    1237,  1239,  1137,  3015,  -675,  -675,  -675,    28,  -675,  -675,
    -675,  1138,   460,   469,  -675,  -675,  1139,  -675,   -63,  -675,
    -675,  -675,  -675,  3015,  -675,  -675,  1119,  -675,  2765,  2834,
    1432,  -675,   377,  -675,  1119,  -675,  -675,  -675,  -675,  -675,
    -675,  1141,  1142,  -675,  1143,  1144,  1146,  -675,  1149,  1119,
     851,  -675,  -675,  -675,  -675,  -675,  1148,  -675,   456,    99,
    -675,   475,  3015,  3015,  3015,  3015,  3015,  3015,  3015,  3015,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  1119,
     -63,  3015,  3015,  3015,  -675,  -675,  -675,   491,   492,   493,
     495,   501,   504,   509,   526,  -675,  -675,   527,   531,   532,
    -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,  -675,
    -675
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   384,   385,   386,   388,   389,   394,
     395,   408,   407,   409,   410,     0,   396,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,     0,     0,     0,     0,    87,     0,     3,
       4,    88,    89,    96,    97,    98,    99,    90,    91,   106,
       0,   107,   108,   109,    95,     0,   292,   294,   312,   314,
     317,   319,   322,   324,   326,   329,   334,   339,   341,   355,
     381,     0,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,   394,
     395,   408,     0,   415,   337,     0,   389,    31,     0,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,   416,
     291,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,   100,     0,
       0,   101,     0,     0,     0,     3,     0,   379,    92,    94,
     376,   199,     0,     0,     0,   338,     0,   390,   391,   336,
     335,   411,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,   359,     0,     0,     0,     0,     0,
       0,   364,     0,   363,   357,     0,     0,     0,     0,    26,
       0,   411,     0,     0,    34,     0,     0,     6,     0,     9,
       0,    10,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    38,     0,     0,   113,     0,   125,     0,
       0,   144,     0,     0,     0,     3,   151,    41,    42,     0,
       0,   147,     0,     0,     0,     0,     0,   128,     0,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   380,     0,     0,   202,     0,     0,     0,   237,   412,
       0,   256,   389,   394,   395,   408,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,   298,   301,   302,   295,   296,     0,   299,   300,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   313,   315,
     316,   318,   320,   321,   323,   325,   327,   328,   333,   330,
     331,   332,   340,   344,   354,   348,   349,   350,   351,   352,
     353,   346,   347,   345,   375,     0,   372,    47,     0,    81,
      82,    83,    84,    85,    44,     0,     0,   358,     0,     0,
       0,   399,   403,   404,    49,     0,   367,     0,   356,     0,
       0,     0,   290,    33,     0,   383,     0,     0,     0,   386,
     407,     0,     0,     0,     0,    23,     0,    24,     0,    25,
       0,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   412,
       0,     0,   116,   412,     0,     0,   320,   321,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     412,     0,   412,     0,     0,     0,     0,   102,   103,   104,
     105,   289,    93,     0,     0,   202,     0,     0,     0,     0,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,   282,     0,     0,     0,   283,     0,     0,   284,     0,
       0,     0,     0,     0,     0,     0,     0,   257,    51,    53,
      55,    58,    56,    57,    52,    54,    64,    66,    68,    71,
      69,    70,    65,    67,     0,     0,   368,     0,     0,     0,
       0,   161,     0,     0,     0,     0,     0,     0,     0,   366,
       0,   407,   362,     0,     0,   399,   403,   404,    32,     5,
       0,     0,    11,     0,     0,     0,     0,    16,     0,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,    72,    75,    74,    78,    76,    77,    73,    36,     0,
      79,    80,    39,   110,   115,   114,     0,     0,   124,   141,
       0,   143,     0,     0,   248,     0,     0,   150,     0,     0,
       0,     0,     0,   162,     0,    28,    27,     0,     0,   145,
     405,   406,     0,     0,   126,   129,   130,   133,   134,     0,
       0,   200,   201,     0,     0,     0,     0,   198,   159,     0,
       0,     0,     0,     0,     0,     0,   236,   414,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,   273,
     280,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   374,     0,   371,     0,     0,
      46,     0,    48,   160,   402,   401,   400,     0,     0,     0,
     365,     0,   360,     0,   361,     0,     0,     0,   244,   243,
       0,     0,     0,    18,     0,    19,     0,    22,   153,     0,
      30,    29,     0,   414,   414,     0,   117,     0,   118,     0,
       0,     0,   249,   250,   149,   251,     0,     0,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,     0,    62,    63,     0,     0,     0,   197,     0,     0,
       0,     0,     0,     0,     0,     0,   158,   156,     0,     0,
       0,     0,     0,     0,     0,   399,   404,   277,     0,     0,
       0,     0,     0,     0,   285,   286,   287,   288,   258,   260,
     262,   265,   263,   264,   259,   261,   373,   370,   369,    45,
      59,    60,    61,   241,     0,     0,    13,    12,    17,     0,
       0,     0,     0,   152,     7,     0,    37,    40,   112,   111,
     412,     0,   122,   142,   139,   140,   252,     0,     0,     0,
       0,     0,   166,   167,   164,     0,     0,     0,     0,     0,
       0,     0,     0,   148,   146,   127,     0,     0,   138,     0,
       3,   232,     0,   229,     0,     0,     0,     0,   208,     0,
       0,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,   155,   157,   238,   239,   240,     0,   161,     0,     0,
       0,   267,   269,   272,   270,   271,   268,     0,   414,   246,
     245,     0,    20,     0,   119,   120,   253,   254,     0,     0,
       0,   168,   170,   172,   175,   173,   174,   169,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   219,
       0,     0,     0,   220,     0,     0,     0,     0,     0,   210,
       0,     0,     0,     0,   209,   206,   207,     0,   235,   276,
     275,   160,     0,     0,   242,    14,   414,     8,     0,   123,
     176,   178,   177,     0,   137,   231,     0,   180,     0,     0,
       0,   179,     0,   228,     0,   217,   218,   212,   215,   216,
     211,     0,     0,   221,     0,     0,     0,   222,     0,     0,
       0,   274,   278,   279,    21,   121,     0,   230,     0,     0,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   227,   225,   226,   214,   223,   224,   213,   234,     0,
       0,     0,     0,     0,   184,   185,   182,     0,     0,     0,
       0,     0,     0,     0,     0,   233,   136,     0,     0,     0,
     186,   188,   190,   193,   191,   192,   187,   189,   194,   196,
     195
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -675,  -675,     1,  -675,  -475,  -619,  -675,  -675,  -675,  -675,
    -675,   281,  -675,  -675,  -675,  -675,  -675,  -269,  -674,  -675,
    -675,  -675,   740,  -675,  -675,   692,   822,  -316,   215,   937,
      -2,  -206,  -675,   829,  1053,  -675,   560,   -16,  1049,  1050,
     755,     2,  -675,  -675,  -233,  -675,  -675,   -64,  -675
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   184,    60,    61,   482,    62,    63,    64,    65,
      66,   867,    67,    68,   495,    69,    70,   306,   871,    71,
      72,   333,   516,    73,   521,   583,   452,   491,   113,   163,
     119,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   279,   414,    88,    89,   120,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,     2,   416,   376,   746,   624,   461,   488,   513,   488,
     137,   489,   595,   489,   488,   102,   514,   114,   489,   518,
     129,   131,   133,   135,   136,   488,   138,   141,   143,   489,
     146,     3,   488,   150,   152,   488,   489,   245,   488,   489,
      24,   167,   489,   255,   170,   291,   632,   121,   122,   123,
     336,   124,   175,   633,   263,   634,   182,   179,   180,   106,
     105,   107,   292,   874,    59,   664,   665,   103,   125,   666,
     291,   875,   126,   879,   945,   673,   674,   675,   647,   648,
     731,   880,   946,   225,   156,   104,   106,   292,   107,   732,
     225,   257,   237,   238,   239,   240,   241,   242,   337,   115,
     247,   157,   249,   250,   147,   873,   664,   665,   307,   258,
     666,   635,   636,   876,   159,   690,   262,   291,   846,   173,
     490,   515,   517,   881,   947,   291,   280,   604,   290,   842,
     295,   160,   297,   676,   292,   320,   174,   667,   670,    59,
     293,    59,   292,   294,   605,   931,   148,   316,   935,   318,
     321,   999,   323,   721,   324,   325,   877,   326,   327,   127,
     328,   329,   330,   328,   331,   293,   882,   948,   294,   649,
     878,   171,   689,   312,   847,   848,   220,   221,   777,   158,
     883,   949,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   313,   362,   363,   364,   365,   366,   367,   368,   369,
     106,   533,   107,   671,   672,   950,   162,   392,   393,   161,
     223,   224,   293,   951,   299,   294,   958,    74,   534,   853,
     293,   637,   591,   294,   630,   474,   266,  1035,   302,   303,
     478,   398,   399,   400,   401,   402,   485,   430,   592,   431,
     267,   593,   139,   140,   251,    92,    93,   252,   668,   225,
     172,   314,   153,   155,   439,   952,   441,   975,   442,   168,
     169,   983,   300,   445,   446,    59,   448,   225,   453,   315,
     838,   176,   265,   101,   103,    59,   523,   268,   425,   467,
     468,   469,   470,   471,   472,   473,   486,   487,   253,   476,
     477,   269,   526,   426,   793,   969,   158,   144,   953,   177,
     229,   145,  1007,   535,    92,    93,   492,   493,   494,   794,
    1021,   497,   954,   499,   230,   231,   232,   260,   503,   504,
     539,   308,   434,   540,   541,  1028,   154,   139,   691,   797,
     542,    59,   101,   543,   544,   738,   304,   545,   546,   682,
     536,   305,   282,   683,   528,   529,   530,   532,   296,   684,
     298,   301,   685,   686,   569,  1045,   687,   537,   704,   460,
     183,   507,   572,   508,   305,   317,   575,   319,   804,   754,
     805,   580,   761,   584,   849,   762,   763,   282,   850,   276,
     227,   228,   764,   277,   284,   765,   766,   285,   286,   767,
     768,   851,   212,   213,   214,   215,   216,   217,   270,    74,
     786,   787,   610,  1012,   218,   454,  1013,  1014,   339,   340,
     456,   619,   271,  1015,   219,   202,  1016,  1017,   178,   455,
    1018,  1019,   547,   570,   457,   458,   573,   574,   488,   509,
     222,   779,   489,   510,   288,   161,   806,   289,   226,   459,
     807,   688,   538,   556,   557,   590,   558,   236,   559,   182,
     808,   809,   600,   810,   602,   811,   603,   560,   561,   562,
     606,   607,   417,   418,   812,   813,   814,   202,   563,   288,
     833,   308,   289,   611,   769,   815,   852,   613,   854,   627,
     901,   614,  1031,   234,   902,   282,  1032,   615,   903,   430,
     644,   585,   284,   616,   904,   285,   286,   233,   202,  1033,
     905,   706,   617,   709,   660,  1020,   556,   557,   558,   906,
     559,   560,   755,   921,   922,   923,   756,   924,   925,   891,
     892,   561,   235,   679,   680,   681,   562,   264,   926,   727,
     265,   692,   693,   927,   694,   695,   243,   696,   697,   698,
     699,   700,   701,   702,   703,   597,   618,   512,   598,   265,
     563,   281,   890,   932,   282,   283,   936,   928,   248,   712,
     611,   284,   256,   613,   285,   286,   959,   970,   287,   185,
     971,   617,   186,   187,   717,   718,   719,   288,   972,   188,
     289,   820,   189,   190,  1034,   259,   191,   192,  1002,   726,
     822,   194,   728,   729,   195,   196,   308,  1003,   265,   734,
     716,   197,   275,  1036,   198,   199,   742,   403,   200,   201,
     427,   265,   556,   557,   558,   404,   559,   405,   406,  1050,
    1051,  1052,   560,  1053,   747,   561,   407,   408,   869,  1054,
     562,   307,  1055,   870,   409,   410,   278,  1056,   757,   758,
     760,  1000,   411,   412,   413,   770,   288,   563,   611,   289,
     308,   778,   613,   617,  1057,  1058,   309,   775,   310,  1059,
    1060,   825,   311,    91,   193,    92,    93,    94,    95,    96,
      97,    98,   832,   332,   872,   272,   273,   274,    99,   870,
     798,   799,   800,   801,   802,   803,   202,   622,   428,   429,
     265,   100,   957,   101,   623,   625,   334,   870,   628,   629,
     631,   403,   735,   827,   415,   736,   265,    74,   335,   404,
     834,   405,   406,   835,   338,   654,   655,   656,   341,   823,
     407,   408,   912,   967,   419,   265,   265,   420,   409,   410,
     898,   385,   386,   387,   438,   265,   411,   412,   413,   444,
     265,     4,     5,     6,     7,   430,   421,   795,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,   855,
     856,   857,   858,   859,   860,   861,   862,   911,   422,   112,
      28,   913,   780,   423,   868,   781,   782,   783,   784,    33,
     389,   390,   391,   464,   465,   403,   432,    41,    42,    43,
     424,   896,    46,   404,   433,   405,   406,   203,   204,   205,
     206,    50,   434,    51,   407,   408,   435,   207,   208,   209,
     436,   884,   409,   410,   885,   437,   886,   887,   339,   466,
     411,   412,   413,   339,   475,   479,   340,   909,   910,   210,
     211,   440,    53,    54,   480,   481,   483,   340,   500,   340,
     502,   340,   748,   443,   638,   639,    55,   918,   919,   920,
     669,   639,   462,    56,   677,   265,   711,   265,   463,   142,
      58,   745,   481,   771,   929,    59,   531,   713,   265,   720,
     265,   934,   722,   723,   484,   940,     4,     5,     6,     7,
     944,   724,   265,   108,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,   496,   339,   725,   962,   963,   730,
     598,   739,   639,   498,   112,   964,   743,   265,   744,   265,
     788,   639,   501,    91,    33,    92,    93,    94,    95,    96,
      97,    98,    41,    42,    43,   789,   639,    46,    99,   816,
     265,   505,   982,   817,   723,   506,    50,   511,    51,   818,
     265,   524,   519,   101,   841,   481,   994,   339,   831,   840,
     340,   998,   507,   908,   265,   961,   265,   508,   966,   265,
     839,   968,   481,   509,   843,   844,   845,    53,    54,   976,
     639,   868,   984,   639,  1029,   639,  1008,  1009,  1011,   164,
     166,    55,   396,   397,   510,   863,   525,   520,    56,   865,
     527,   548,   549,   550,   219,    58,   551,   552,   553,   554,
      59,   564,   555,   556,   566,   557,   225,   558,   559,   560,
    1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,   561,   562,
     563,   576,   565,   567,   587,   577,   578,   588,   589,  1047,
    1048,  1049,   370,   371,   372,   373,   374,   375,   586,   377,
     378,   379,   380,   381,   382,   383,   384,   594,   599,   608,
       4,     5,     6,     7,   609,   914,   915,   342,   343,   344,
     345,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   346,   347,    24,   611,   612,    25,    26,    27,    28,
     348,   349,    31,    32,   613,   614,   615,   616,    33,    34,
      35,    36,    37,    38,   350,   351,    41,    42,    43,    44,
      45,    46,    47,   617,   620,   621,    48,    49,   305,   645,
      50,   650,    51,   646,   651,   652,    52,   653,   657,   658,
     659,   661,   662,   678,   710,   712,   714,   733,   715,   737,
     740,   741,   749,   750,   751,   752,   753,   772,   773,   774,
     776,    53,    54,   785,   790,   974,   791,   792,   796,    74,
     819,   820,   821,   822,   824,    55,   826,   828,   829,   830,
     836,   837,    56,   866,   888,   864,   889,   893,    57,    58,
     894,   895,   899,   900,    59,   352,   907,     4,     5,     6,
       7,   916,   917,  1005,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   640,   641,
      24,   930,   933,    25,    26,    27,    28,    29,   642,    31,
      32,   937,   938,   939,   941,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
     942,   943,   955,    48,    49,   956,   965,    50,   973,    51,
     991,   960,   985,    52,   977,  1046,   870,   992,   986,   987,
     988,   995,   989,   996,  1006,   663,   990,   993,   997,   708,
    1004,     0,  1022,  1023,  1024,  1025,  1001,  1026,    53,    54,
    1027,  1030,   388,   596,   394,     0,   395,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,    57,    58,     4,     5,     6,
       7,    59,   643,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   978,   979,
      24,     0,     0,    25,    26,    27,    28,    29,   980,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,    50,     0,    51,
       0,     0,     0,    52,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,     0,    55,     0,    33,     0,     0,     0,     0,    56,
       0,     0,    41,    42,    43,    57,    58,    46,     0,     0,
       0,    59,   981,     0,     0,     0,    50,     0,    51,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,   142,    58,    46,     0,     0,     0,
      59,   759,     0,     0,     0,    50,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,   142,    58,     4,     5,     6,     7,    59,
    1010,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,    26,    27,    28,    29,    30,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,     0,    51,     0,     0,
       0,    52,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,     0,
      55,     0,    33,     0,     0,     0,     0,    56,     0,     0,
      41,    42,    43,    57,    58,    46,     0,     0,     0,    59,
       0,     0,     0,     0,    50,     0,    51,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,     0,    55,
       0,    33,     0,     0,     0,     0,    56,     0,     0,    41,
      42,    43,   117,   118,    46,     0,     0,     0,    59,     0,
       0,     0,     0,    50,     0,    51,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,   142,    58,    46,     0,     0,     0,    59,     4,     5,
       6,     7,    50,     0,    51,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,   181,     0,     0,     0,    59,     0,     4,     5,
       6,     7,     0,     0,     0,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    53,
      54,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    55,     0,     0,    33,     0,     0,     0,
      56,     0,     0,     0,    41,    42,    43,    58,   244,    46,
       0,     0,    59,     0,     4,     5,     6,     7,    50,     0,
      51,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,    53,
      54,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    55,     0,    46,     0,     0,     0,     0,
      56,   246,     0,     0,    50,     0,    51,    58,     0,     0,
       0,     0,    59,     0,     4,     5,     6,     7,     0,     0,
       0,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,    55,
       0,     0,    33,     0,     0,     0,    56,     0,     0,     0,
      41,    42,    43,    58,   254,    46,     0,     0,    59,     0,
       4,     5,     6,     7,    50,     0,    51,   116,   109,   110,
     111,    12,    13,    14,    15,    16,    17,    18,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,    53,    54,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    55,
       0,    46,     0,     0,     0,     0,    56,     0,     0,     0,
      50,     0,    51,    58,   261,     0,     0,     0,    59,     4,
       5,     6,     7,     0,     0,     0,   116,   109,   110,   111,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,   112,    28,     0,
       0,     0,     0,     0,     0,    55,     0,    33,     0,     0,
       0,     0,    56,     0,     0,    41,    42,    43,     0,    58,
      46,     0,     0,     0,    59,     0,     0,     0,     0,    50,
       0,    51,     0,     0,   183,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,     0,    55,     0,    33,     0,     0,     0,
       0,    56,     0,     0,    41,    42,    43,     0,    58,    46,
       0,     0,     0,    59,     4,     5,     6,     7,    50,     0,
      51,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,    53,
      54,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    55,     0,    46,     0,     0,     0,     0,
      56,     0,     0,     0,    50,     0,    51,    58,   522,     0,
       0,     0,    59,     0,     4,     5,     6,     7,     0,     0,
       0,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,    55,
       0,     0,    33,     0,     0,     0,    56,     0,     0,     0,
      41,    42,    43,    58,   568,    46,     0,     0,    59,     0,
       4,     5,     6,     7,    50,     0,    51,   116,   109,   110,
     111,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,    53,    54,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    55,
       0,    46,     0,     0,     0,     0,    56,     0,     0,     0,
      50,     0,    51,    58,   571,     0,     0,     0,    59,     0,
       4,     5,     6,     7,     0,     0,     0,   116,   109,   110,
     111,   581,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,    55,     0,     0,    33,     0,
       0,     0,    56,     0,     0,     0,    41,    42,    43,    58,
     579,    46,     0,     0,    59,     0,     4,     5,   449,     7,
      50,     0,    51,   116,   109,   110,   111,   450,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,    58,
     582,     0,     0,     0,    59,     0,     4,     5,     6,     7,
       0,     0,     0,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    53,    54,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    55,     0,     0,    33,     0,     0,     0,    56,     0,
       0,     0,    41,    42,    43,    58,   181,    46,     0,     0,
      59,     0,     4,     5,     6,     7,    50,     0,    51,   116,
     109,   110,   111,   581,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,    53,    54,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    55,     0,    46,     0,     0,     0,     0,    56,     0,
       0,     0,    50,     0,    51,    58,   705,     0,     0,     0,
      59,     0,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,    55,     0,     0,
      33,     0,     0,     0,    56,     0,     0,     0,    41,    42,
      43,    58,   707,    46,     0,     0,    59,     0,     4,     5,
       6,     7,    50,     0,    51,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,   897,     0,     0,     0,    59,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,     0,   128,    46,     0,
       0,     0,    59,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,   130,     0,     0,     0,
       0,    59,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,     0,   132,    46,     0,     0,     0,    59,     4,     5,
       6,     7,    50,     0,    51,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,   134,     0,     0,     0,     0,    59,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,     0,    58,    46,     0,
       0,     0,    59,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,   149,     0,     0,     0,
       0,    59,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,     0,   151,    46,     0,     0,     0,    59,     4,     5,
       6,     7,    50,     0,    51,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,     0,     0,     0,     0,   165,     4,     5,   449,
       7,     0,     0,     0,   116,   109,   110,   111,   450,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,     0,   447,    46,     0,
       0,     0,    59,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,   451,     0,     0,     0,
       0,    59,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,     0,   601,    46,     0,     0,     0,    59,     0,     0,
       0,     0,    50,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -415,
    -415,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,   626,     0,     0,  -415,  -415,    59,  -415,  -415,  -415,
    -415,  -415,  -415,  -415,  -415,  -415,  -415,  -415,  -415,  -415,
       0,     0,     0,  -415,     0,     0,     0,     0,     0,     0,
       0,  -415,  -415,  -415,  -415,     0,  -415,     0,     0,  -415,
    -415,  -415,  -415,  -415,  -415,     0,  -415,  -415,  -415,     0,
    -415
};

static const yytype_int16 yycheck[] =
{
       2,     0,   235,   209,   623,   480,   275,    10,    14,    10,
      26,    14,    73,    14,    10,   125,    22,    15,    14,   335,
      22,    23,    24,    25,    26,    10,    14,    29,    30,    14,
      32,     0,    10,    35,    36,    10,    14,   101,    10,    14,
      26,    43,    14,   107,    46,    26,    76,    10,    11,    12,
      14,    14,    50,    76,   118,    10,    58,    55,    56,   120,
     107,   122,    43,    14,   127,    56,    57,   109,    31,    60,
      26,    22,    35,    14,    14,    70,    71,    72,    37,    38,
       5,    22,    22,   113,    26,   127,   120,    43,   122,    14,
     113,   109,    94,    95,    96,    97,    98,    99,    62,    14,
     102,    43,   104,   105,    12,   779,    56,    57,   121,   127,
      60,    66,    67,    64,    26,   128,   118,    26,    10,    14,
     123,   127,   123,    64,    64,    26,   128,   123,   130,   748,
     132,    43,   134,   128,    43,    29,    31,   128,   123,   127,
     121,   127,    43,   124,   460,   123,    12,   149,   123,   151,
      44,   123,   154,    73,   156,   157,   107,   159,   160,   122,
     162,   163,   164,   165,   166,   121,   107,   107,   124,   128,
     121,    14,   128,   109,    66,    67,   111,   112,   128,   121,
     121,   121,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   127,   194,   195,   196,   197,   198,   199,   200,   201,
     120,    26,   122,   519,   520,    14,   127,   223,   224,   121,
      74,    75,   121,    22,    76,   124,   890,     2,    43,   128,
     121,   490,   105,   124,    76,   289,   107,   128,    74,    75,
     294,   229,   230,   231,   232,   233,   300,   124,   121,   126,
     121,   124,    27,    28,    14,    94,    95,    17,   517,   113,
      14,   109,    37,    38,   256,    64,   258,   931,   260,    44,
      45,   935,   124,   265,   266,   127,   268,   113,   270,   127,
     745,    14,   124,   122,   109,   127,   340,   107,   107,   281,
     282,   283,   284,   285,   286,   287,   302,   303,    58,   291,
     292,   121,   127,   122,   107,   914,   121,    10,   107,    10,
     102,    14,   976,   128,    94,    95,   305,   309,   310,   122,
     984,   313,   121,   315,   116,   117,   118,   107,   320,   321,
      26,   121,   121,    29,    30,   999,   122,   112,   128,   128,
      36,   127,   122,    39,    40,   604,   122,    43,    44,    26,
      26,   127,    29,    30,   346,   347,   348,   349,   133,    36,
     135,   136,    39,    40,   418,  1029,    43,    43,   564,   122,
      68,   121,   426,   121,   127,   150,   430,   152,   128,   638,
     128,   435,    26,   437,    26,    29,    30,    29,    30,    10,
     114,   115,    36,    14,    36,    39,    40,    39,    40,    43,
      44,    43,    81,    82,    83,    84,    85,    86,   107,   184,
     669,   670,   466,    26,    93,   107,    29,    30,   123,   124,
     107,   475,   121,    36,   103,   121,    39,    40,    10,   121,
      43,    44,   128,   425,   121,   107,   428,   429,    10,   121,
     101,   664,    14,   121,   121,   121,   128,   124,   104,   121,
     128,   128,   128,   121,   121,   447,   121,    14,   121,   451,
     128,   128,   454,   128,   456,   128,   458,   121,   121,   121,
     462,   463,   121,   122,   128,   128,   128,   121,   121,   121,
     739,   121,   124,   121,   128,   128,   128,   121,   128,   481,
     128,   121,    26,   120,   128,    29,    30,   121,   128,   124,
     492,   126,    36,   121,   128,    39,    40,   106,   121,    43,
     128,   565,   121,   567,   506,   128,   121,   121,   121,   128,
     121,   121,    10,   128,   128,   128,    14,   128,   128,   788,
     789,   121,   120,   525,   526,   527,   121,   121,   128,   593,
     124,   533,   534,   128,   536,   537,    14,   539,   540,   541,
     542,   543,   544,   545,   546,   121,   121,   332,   124,   124,
     121,    26,   785,   869,    29,    30,   872,   128,    14,   121,
     121,    36,   125,   121,    39,    40,   128,   128,    43,    26,
     128,   121,    29,    30,   576,   577,   578,   121,   128,    36,
     124,   121,    39,    40,   128,   121,    43,    44,   128,   591,
     121,    26,   594,   595,    29,    30,   121,   128,   124,   601,
     126,    36,    14,   128,    39,    40,   608,    14,    43,    44,
     123,   124,   121,   121,   121,    22,   121,    24,    25,   128,
     128,   128,   121,   128,   626,   121,    33,    34,   122,   128,
     121,   121,   128,   127,    41,    42,    10,   128,   640,   641,
     642,   957,    49,    50,    51,   647,   121,   121,   121,   124,
     121,    58,   121,   121,   128,   128,   107,   659,   107,   128,
     128,   725,   127,    92,   121,    94,    95,    96,    97,    98,
      99,   100,   736,    53,   122,    10,    11,    12,   107,   127,
     682,   683,   684,   685,   686,   687,   121,   121,   114,   115,
     124,   120,   122,   122,   479,   480,   127,   127,   483,   484,
     485,    14,   121,   121,    17,   124,   124,   492,   122,    22,
     121,    24,    25,   124,   127,   500,   501,   502,   128,   721,
      33,    34,   121,   121,   121,   124,   124,   121,    41,    42,
     794,     7,     8,     9,   123,   124,    49,    50,    51,   123,
     124,     3,     4,     5,     6,   124,   121,   126,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   761,
     762,   763,   764,   765,   766,   767,   768,   831,   121,    31,
      32,   835,    10,   121,   776,    13,    14,    15,    16,    41,
     220,   221,   222,   123,   124,    14,   110,    49,    50,    51,
     121,   793,    54,    22,   128,    24,    25,    77,    78,    79,
      80,    63,   121,    65,    33,    34,   122,    87,    88,    89,
      14,    10,    41,    42,    13,   122,    15,    16,   123,   124,
      49,    50,    51,   123,   124,   123,   124,   829,   830,   109,
     110,    14,    94,    95,    27,    28,   123,   124,   123,   124,
     123,   124,   627,   121,   123,   124,   108,   849,   850,   851,
     123,   124,   107,   115,   123,   124,   123,   124,   107,   121,
     122,    27,    28,   648,   866,   127,   128,   123,   124,   123,
     124,   870,   123,   124,    10,   877,     3,     4,     5,     6,
     882,   123,   124,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    14,   123,   124,   899,   900,   123,
     124,   123,   124,    14,    31,   907,   123,   124,   123,   124,
     123,   124,    27,    92,    41,    94,    95,    96,    97,    98,
      99,   100,    49,    50,    51,   123,   124,    54,   107,   123,
     124,    10,   934,   123,   124,   121,    63,   128,    65,   123,
     124,   120,   122,   122,    27,    28,   948,   123,   124,   123,
     124,   953,   121,   123,   124,   123,   124,   121,   123,   124,
     745,    27,    28,   121,   749,   750,   751,    94,    95,   123,
     124,   973,   123,   124,   123,   124,   978,   979,   980,    42,
      43,   108,   227,   228,   121,   770,   125,   122,   115,   774,
     107,   121,   121,   121,   103,   122,   121,   121,   121,   121,
     127,    90,   121,   121,    14,   121,   113,   121,   121,   121,
    1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,   121,   121,
     121,   107,   122,   122,   128,   107,   107,   121,   121,  1031,
    1032,  1033,   203,   204,   205,   206,   207,   208,   110,   210,
     211,   212,   213,   214,   215,   216,   217,    73,   121,   107,
       3,     4,     5,     6,    10,   840,   841,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   121,   121,    29,    30,    31,    32,
      33,    34,    35,    36,   121,   121,   121,   121,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,   121,   121,   121,    59,    60,   127,   121,
      63,   110,    65,   121,   128,   110,    69,   128,   121,   121,
     107,    14,    14,    14,   121,   121,   126,   121,   126,   121,
     121,   121,    10,    10,    10,    10,    10,   121,   121,    27,
     121,    94,    95,    56,    14,   930,    14,    14,   128,   934,
     121,   121,   121,   121,    14,   108,   121,   121,    73,    73,
     121,   121,   115,   121,   121,   128,   121,     7,   121,   122,
       7,     7,   107,   107,   127,   128,    73,     3,     4,     5,
       6,    10,    10,   968,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   123,    68,    29,    30,    31,    32,    33,    34,    35,
      36,    14,    14,   121,    14,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      14,   121,   121,    59,    60,   121,   121,    63,   121,    65,
      14,   128,   121,    69,   128,  1030,   127,    14,   121,   121,
     121,    14,   121,    14,   973,   515,   121,   121,   121,   567,
     121,    -1,   121,   121,   121,   121,   128,   121,    94,    95,
     121,   123,   219,   451,   225,    -1,   226,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,   121,   122,     3,     4,     5,
       6,   127,   128,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    49,    50,    51,   121,   122,    54,    -1,    -1,
      -1,   127,   128,    -1,    -1,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,   121,   122,    54,    -1,    -1,    -1,
     127,   128,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
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
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,
      50,    51,   121,   122,    54,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,   121,   122,    54,    -1,    -1,    -1,   127,     3,     4,
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
     115,   116,    -1,    -1,    63,    -1,    65,   122,    -1,    -1,
      -1,    -1,   127,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    41,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      49,    50,    51,   122,   123,    54,    -1,    -1,   127,    -1,
       3,     4,     5,     6,    63,    -1,    65,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,   108,
      -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      63,    -1,    65,   122,   123,    -1,    -1,    -1,   127,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,   122,
      54,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    -1,    68,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    49,    50,    51,    -1,   122,    54,
      -1,    -1,    -1,   127,     3,     4,     5,     6,    63,    -1,
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
      65,   122,   123,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    49,    50,    51,    -1,   122,    54,    -1,
      -1,    -1,   127,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,    -1,    -1,    -1,
      -1,   127,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,    -1,   122,    54,    -1,    -1,    -1,   127,     3,     4,
       5,     6,    63,    -1,    65,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,
      65,   122,    -1,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    49,    50,    51,    -1,   122,    54,    -1,
      -1,    -1,   127,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,    -1,    -1,    -1,
      -1,   127,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,    -1,   122,    54,    -1,    -1,    -1,   127,     3,     4,
       5,     6,    63,    -1,    65,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,
      65,   122,    -1,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    49,    50,    51,    -1,   122,    54,    -1,
      -1,    -1,   127,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,    -1,    -1,    -1,
      -1,   127,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,    -1,   122,    54,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      40,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    74,    75,   127,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,   102,   103,   104,    -1,   106,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120
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
     120,   122,   125,   109,   127,   107,   120,   122,    10,    11,
      12,    13,    31,   157,   170,    14,    10,   121,   122,   159,
     176,    10,    11,    12,    14,    31,    35,   122,   122,   159,
     122,   159,   122,   159,   122,   159,   159,   166,    14,   157,
     157,   159,   121,   159,    10,    14,   159,    12,    12,   122,
     159,   122,   159,   157,   122,   157,    26,    43,   121,    26,
      43,   121,   127,   158,   158,   127,   158,   159,   157,   157,
     159,    14,    14,    14,    31,   170,    14,    10,    10,   170,
     170,   123,   159,    68,   131,    26,    29,    30,    36,    39,
      40,    43,    44,   121,    26,    29,    30,    36,    39,    40,
      43,    44,   121,    77,    78,    79,    80,    87,    88,    89,
     109,   110,    81,    82,    83,    84,    85,    86,    93,   103,
     111,   112,   101,    74,    75,   113,   104,   114,   115,   102,
     116,   117,   118,   106,   120,   120,    14,   159,   159,   159,
     159,   159,   159,    14,   123,   176,   116,   159,    14,   159,
     159,    14,    17,    58,   123,   176,   125,   109,   127,   121,
     107,   123,   159,   176,   121,   124,   107,   121,   107,   121,
     107,   121,    10,    11,    12,    14,    10,    14,    10,   172,
     159,    26,    29,    30,    36,    39,    40,    43,   121,   124,
     159,    26,    43,   121,   124,   159,   157,   159,   157,    76,
     124,   157,    74,    75,   122,   127,   146,   121,   121,   107,
     107,   127,   109,   127,   109,   127,   159,   157,   159,   157,
      29,    44,    23,   159,   159,   159,   159,   159,   159,   159,
     159,   159,    53,   150,   127,   122,    14,    62,   127,   123,
     124,   128,    10,    11,    12,    13,    24,    25,    33,    34,
      47,    48,   128,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     162,   162,   162,   162,   162,   162,   160,   162,   162,   162,
     162,   162,   162,   162,   162,     7,     8,     9,   163,   165,
     165,   165,   166,   166,   167,   168,   169,   169,   170,   170,
     170,   170,   170,    14,    22,    24,    25,    33,    34,    41,
      42,    49,    50,    51,   173,    17,   173,   121,   122,   121,
     121,   121,   121,   121,   121,   107,   122,   123,   114,   115,
     124,   126,   110,   128,   121,   122,    14,   122,   123,   159,
      14,   159,   159,   121,   123,   159,   159,   122,   159,     5,
      14,   122,   155,   159,   107,   121,   107,   121,   107,   121,
     122,   146,   107,   107,   123,   124,   124,   159,   159,   159,
     159,   159,   159,   159,   176,   124,   159,   159,   176,   123,
      27,    28,   134,   123,    10,   176,   166,   166,    10,    14,
     123,   156,   131,   159,   159,   143,    14,   159,    14,   159,
     123,    27,   123,   159,   159,    10,   121,   121,   121,   121,
     121,   128,   157,    14,    22,   127,   151,   123,   156,   122,
     122,   153,   123,   176,   120,   125,   127,   107,   159,   159,
     159,   128,   159,    26,    43,   128,    26,    43,   128,    26,
      29,    30,    36,    39,    40,    43,    44,   128,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,    90,   122,    14,   122,   123,   176,
     159,   123,   176,   159,   159,   176,   107,   107,   107,   123,
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
     121,   123,   121,   123,   126,   126,   126,   159,   159,   159,
     123,    73,   123,   124,   123,   124,   159,   176,   159,   159,
     123,     5,    14,   121,   159,   121,   124,   121,   146,   123,
     121,   121,   159,   123,   123,    27,   134,   159,   157,    10,
      10,    10,    10,    10,   146,    10,    14,   159,   159,   128,
     159,    26,    29,    30,    36,    39,    40,    43,    44,   128,
     159,   157,   121,   121,    27,   159,   121,   128,    58,   173,
      10,    13,    14,    15,    16,    56,   146,   146,   123,   123,
      14,    14,    14,   107,   122,   126,   128,   128,   159,   159,
     159,   159,   159,   159,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   123,   123,   123,   121,
     121,   121,   121,   159,    14,   176,   121,   121,   121,    73,
      73,   124,   176,   146,   121,   124,   121,   121,   133,   157,
     123,    27,   134,   157,   157,   157,    10,    66,    67,    26,
      30,    43,   128,   128,   128,   159,   159,   159,   159,   159,
     159,   159,   159,   157,   128,   157,   121,   140,   159,   122,
     127,   147,   122,   147,    14,    22,    64,   107,   121,    14,
      22,    64,   107,   121,    10,    13,    15,    16,   121,   121,
     173,   146,   146,     7,     7,     7,   159,   123,   176,   107,
     107,   128,   128,   128,   128,   128,   128,    73,   123,   159,
     159,   176,   121,   176,   157,   157,    10,    10,   159,   159,
     159,   128,   128,   128,   128,   128,   128,   128,   128,   159,
     123,   123,   156,    68,   131,   123,   156,    14,    14,   121,
     159,    14,    14,   121,   159,    14,    22,    64,   107,   121,
      14,    22,    64,   107,   121,   121,   121,   122,   147,   128,
     128,   123,   159,   159,   159,   121,   123,   121,    27,   134,
     128,   128,   128,   121,   157,   147,   123,   128,    24,    25,
      34,   128,   159,   147,   123,   121,   121,   121,   121,   121,
     121,    14,    14,   121,   159,    14,    14,   121,   159,   123,
     156,   128,   128,   128,   121,   157,   140,   147,   159,   159,
     128,   159,    26,    29,    30,    36,    39,    40,    43,    44,
     128,   147,   121,   121,   121,   121,   121,   121,   147,   123,
     123,    26,    30,    43,   128,   128,   128,   159,   159,   159,
     159,   159,   159,   159,   159,   147,   157,   159,   159,   159,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128
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
     133,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     134,   134,   134,   134,   135,   135,   136,   136,   136,   136,
     137,   137,   138,   138,   138,   139,   139,   139,   140,   141,
     141,   141,   141,   141,   141,   142,   142,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   148,   149,   150,
     150,   150,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   152,   153,   153,   153,
     153,   154,   154,   155,   155,   155,   155,   156,   156,   156,
     156,   156,   156,   156,   156,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     159,   159,   159,   160,   160,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   162,   163,   163,   163,   164,   164,
     165,   165,   165,   166,   166,   167,   167,   168,   168,   168,
     169,   169,   169,   169,   169,   170,   170,   170,   170,   170,
     171,   171,   172,   172,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   174,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   176,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     5,     5,     7,     6,     6,
       8,    10,     6,     4,     4,     4,     3,     5,     5,     6,
       6,     2,     5,     4,     3,     3,     5,     7,     3,     5,
       7,     3,     3,     2,     4,     7,     6,     4,     6,     4,
       2,     4,     4,     4,     4,     4,     4,     4,     4,     7,
       7,     7,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     4,     4,     4,     4,     4,     2,     1,     1,     1,
       1,     1,     2,     4,     2,     1,     1,     1,     1,     1,
       2,     2,     4,     4,     4,     4,     1,     1,     1,     1,
       5,     7,     7,     3,     5,     5,     4,     6,     3,     5,
       5,     7,     4,     6,     5,     3,     5,     7,     3,     5,
       5,     3,     2,     5,     5,     2,    12,     9,     1,     7,
       7,     5,     7,     5,     3,     5,     7,     0,     4,     6,
       5,     3,     7,     6,     4,     7,     6,     7,     6,     5,
       6,     5,     3,     4,     5,     4,     5,     5,     6,     6,
       6,     6,     6,     6,     6,     6,     7,     7,     7,     3,
       3,     4,     5,     4,     5,     5,     6,     6,     6,     6,
       6,     6,     6,     6,     7,     7,     7,     6,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     4,
       7,     6,     4,     8,     7,     5,     5,     0,     4,     4,
       4,     3,     5,     3,     3,     5,     5,     1,     2,     3,
       3,     3,     4,     5,     5,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     2,     1,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     2,     2,     2,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     4,     3,
       6,     6,     5,     3,     3,     6,     5,     4,     4,     6,
       6,     5,     3,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     4,
       6,     6,     6,     4,     4,     5,     5,     1,     1,     1,
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
#line 376 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2734 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 386 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2740 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 387 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2746 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 391 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2752 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 393 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2758 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 395 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2764 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 397 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2772 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 401 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2778 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 403 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2784 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 405 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2790 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 407 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2798 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 411 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2805 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 414 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2813 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 418 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2819 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 420 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2825 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 422 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2831 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 424 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2837 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 426 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2843 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 428 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2851 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 432 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2859 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 436 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2865 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 438 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2871 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 440 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2877 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 442 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2883 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 444 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2889 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 446 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2895 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 448 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2901 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 450 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2907 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 452 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2913 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 454 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2919 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 456 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2925 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 458 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2931 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 460 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2937 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 462 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2943 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 464 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2950 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 467 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2957 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 470 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2963 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 472 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2970 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 475 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2977 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 478 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2983 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 480 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2989 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 482 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2995 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 484 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 3001 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 486 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3010 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 491 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3018 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 495 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3026 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 499 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3035 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 504 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3043 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 507 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3049 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 509 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3055 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 511 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3061 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 513 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3067 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 515 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3073 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 517 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3080 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 520 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3086 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 522 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3092 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 524 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3098 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 526 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3105 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 529 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3112 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 532 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3119 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 535 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3126 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 538 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3133 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 541 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3139 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 543 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3145 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 545 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3151 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3157 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 549 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3164 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 552 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3170 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 554 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3176 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 556 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3182 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 558 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3189 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 561 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3196 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 564 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3204 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 568 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3211 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 571 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3217 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 573 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3223 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 575 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3229 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 577 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3236 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 580 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3243 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 583 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3250 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 586 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3257 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 589 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3264 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 592 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3271 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 595 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3278 "raku.tab.c"
    break;

  case 86: /* stmt: expr ';'  */
#line 597 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3284 "raku.tab.c"
    break;

  case 87: /* stmt: ';'  */
#line 598 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3290 "raku.tab.c"
    break;

  case 88: /* stmt: if_stmt  */
#line 599 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3296 "raku.tab.c"
    break;

  case 89: /* stmt: while_stmt  */
#line 600 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3302 "raku.tab.c"
    break;

  case 90: /* stmt: for_stmt  */
#line 601 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3308 "raku.tab.c"
    break;

  case 91: /* stmt: given_stmt  */
#line 602 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3314 "raku.tab.c"
    break;

  case 92: /* stmt: KW_TRY block  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3320 "raku.tab.c"
    break;

  case 93: /* stmt: KW_TRY block KW_CATCH block  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3326 "raku.tab.c"
    break;

  case 94: /* stmt: KW_CATCH block  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3332 "raku.tab.c"
    break;

  case 95: /* stmt: block  */
#line 610 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3338 "raku.tab.c"
    break;

  case 96: /* stmt: unless_stmt  */
#line 611 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3344 "raku.tab.c"
    break;

  case 97: /* stmt: until_stmt  */
#line 612 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3350 "raku.tab.c"
    break;

  case 98: /* stmt: repeat_stmt  */
#line 613 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3356 "raku.tab.c"
    break;

  case 99: /* stmt: loop_stmt  */
#line 614 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3362 "raku.tab.c"
    break;

  case 100: /* stmt: KW_LAST ';'  */
#line 615 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3368 "raku.tab.c"
    break;

  case 101: /* stmt: KW_NEXT ';'  */
#line 616 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3374 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST KW_IF expr ';'  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3380 "raku.tab.c"
    break;

  case 103: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 620 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3386 "raku.tab.c"
    break;

  case 104: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 622 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3392 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3398 "raku.tab.c"
    break;

  case 106: /* stmt: sub_decl  */
#line 625 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3404 "raku.tab.c"
    break;

  case 107: /* stmt: class_decl  */
#line 626 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3410 "raku.tab.c"
    break;

  case 108: /* stmt: role_decl  */
#line 627 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3416 "raku.tab.c"
    break;

  case 109: /* stmt: grammar_decl  */
#line 628 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3422 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF '(' expr ')' block  */
#line 632 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3428 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 634 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3434 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 636 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3440 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF expr block  */
#line 638 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3446 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3452 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 642 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3458 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block elsif_tail  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3464 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3470 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF expr block  */
#line 650 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3476 "raku.tab.c"
    break;

  case 119: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3482 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3488 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3494 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 658 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3500 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3506 "raku.tab.c"
    break;

  case 124: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 664 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3512 "raku.tab.c"
    break;

  case 125: /* while_stmt: KW_WHILE expr block  */
#line 666 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3518 "raku.tab.c"
    break;

  case 126: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3524 "raku.tab.c"
    break;

  case 127: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3530 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS expr block  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3536 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3542 "raku.tab.c"
    break;

  case 130: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3548 "raku.tab.c"
    break;

  case 131: /* until_stmt: KW_UNTIL expr block  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3554 "raku.tab.c"
    break;

  case 132: /* repeat_stmt: KW_REPEAT block  */
#line 686 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3560 "raku.tab.c"
    break;

  case 133: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3566 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3572 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 694 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3578 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 696 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3584 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 698 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3590 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 701 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3596 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 705 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3606 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 711 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3616 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 717 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3625 "raku.tab.c"
    break;

  case 142: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 722 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3635 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 728 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3644 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR expr block  */
#line 733 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3651 "raku.tab.c"
    break;

  case 145: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 738 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3663 "raku.tab.c"
    break;

  case 146: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 746 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3676 "raku.tab.c"
    break;

  case 147: /* when_list: %empty  */
#line 756 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3682 "raku.tab.c"
    break;

  case 148: /* when_list: when_list KW_WHEN expr block  */
#line 758 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3690 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 764 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3702 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 772 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_SUB IDENT sub_body  */
#line 778 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3722 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 784 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3734 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 792 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3744 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 798 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3754 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 804 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3767 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 813 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3778 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 820 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3791 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 829 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3804 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 838 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3815 "raku.tab.c"
    break;

  case 160: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 847 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3826 "raku.tab.c"
    break;

  case 161: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 854 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3835 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list '}'  */
#line 860 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3841 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list expr '}'  */
#line 862 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3848 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 865 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3855 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 868 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3861 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 870 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3867 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 872 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3873 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 874 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3879 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 876 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3885 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 878 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3891 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 880 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3897 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 882 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3904 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 885 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3910 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 887 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3916 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 889 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3922 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 891 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3929 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 894 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3936 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 897 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3944 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list '}'  */
#line 902 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3950 "raku.tab.c"
    break;

  case 180: /* method_body: '{' YADA '}'  */
#line 903 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3956 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list expr '}'  */
#line 905 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3963 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 908 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3970 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 911 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3976 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 913 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3982 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 915 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3988 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3994 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 919 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4000 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 921 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4006 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 923 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4012 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 925 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4019 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 928 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4025 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 930 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4031 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 932 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4037 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 934 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4044 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 937 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4051 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 940 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4059 "raku.tab.c"
    break;

  case 197: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 946 "raku.y"
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
#line 4077 "raku.tab.c"
    break;

  case 198: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 962 "raku.y"
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
#line 4094 "raku.tab.c"
    break;

  case 199: /* is_clauses: %empty  */
#line 976 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4100 "raku.tab.c"
    break;

  case 200: /* is_clauses: is_clauses IDENT IDENT  */
#line 978 "raku.y"
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
#line 4116 "raku.tab.c"
    break;

  case 201: /* is_clauses: is_clauses TESTOP IDENT  */
#line 990 "raku.y"
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
#line 4132 "raku.tab.c"
    break;

  case 202: /* class_body_list: %empty  */
#line 1003 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4138 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1005 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4145 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1008 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4152 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1011 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4159 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1014 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4166 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1017 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4173 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1020 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4180 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1023 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4187 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1026 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4194 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1029 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4201 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1032 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4208 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1035 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4215 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1038 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4222 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1041 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4233 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1048 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4244 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1055 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4255 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1062 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4266 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1069 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4274 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1073 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4282 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1077 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4290 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1081 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4298 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1085 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4309 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1092 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4320 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1099 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4331 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1106 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4342 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1113 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4356 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1123 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4368 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1131 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4380 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1139 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4393 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1148 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4404 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1155 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4415 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1162 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4429 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1172 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4441 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1180 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4453 "raku.tab.c"
    break;

  case 236: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1190 "raku.y"
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
#line 4470 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: %empty  */
#line 1204 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4476 "raku.tab.c"
    break;

  case 238: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1206 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4485 "raku.tab.c"
    break;

  case 239: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1211 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4494 "raku.tab.c"
    break;

  case 240: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1216 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4503 "raku.tab.c"
    break;

  case 241: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1223 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4511 "raku.tab.c"
    break;

  case 242: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1227 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4519 "raku.tab.c"
    break;

  case 243: /* pair_list: IDENT OP_FATARROW expr  */
#line 1233 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4525 "raku.tab.c"
    break;

  case 244: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1235 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4531 "raku.tab.c"
    break;

  case 245: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1237 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4537 "raku.tab.c"
    break;

  case 246: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1239 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4543 "raku.tab.c"
    break;

  case 247: /* param_list: VAR_SCALAR  */
#line 1242 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4549 "raku.tab.c"
    break;

  case 248: /* param_list: IDENT VAR_SCALAR  */
#line 1243 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4555 "raku.tab.c"
    break;

  case 249: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1244 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4561 "raku.tab.c"
    break;

  case 250: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1245 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4567 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' VAR_SCALAR  */
#line 1246 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4573 "raku.tab.c"
    break;

  case 252: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1247 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4579 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1248 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4585 "raku.tab.c"
    break;

  case 254: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1249 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4591 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list '}'  */
#line 1252 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4597 "raku.tab.c"
    break;

  case 256: /* block: '{' YADA '}'  */
#line 1253 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4603 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr '}'  */
#line 1255 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4609 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1257 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4615 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1259 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4621 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1261 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4627 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1263 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4633 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1265 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4640 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1268 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4646 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1270 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4652 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1272 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4658 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1274 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4664 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1276 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4671 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1279 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4678 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1282 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1286 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1289 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4700 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1292 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4707 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1295 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4713 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1297 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4721 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1301 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4728 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1304 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4735 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1307 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4742 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1310 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4749 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1313 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4756 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1316 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4762 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1318 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4768 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1320 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4774 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST '}'  */
#line 1322 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4780 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1324 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4786 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4792 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1328 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4798 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1330 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4804 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1332 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4810 "raku.tab.c"
    break;

  case 289: /* closure: '{' expr '}'  */
#line 1335 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4816 "raku.tab.c"
    break;

  case 290: /* expr: VAR_SCALAR '=' expr  */
#line 1338 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4822 "raku.tab.c"
    break;

  case 291: /* expr: KW_GATHER block  */
#line 1339 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4832 "raku.tab.c"
    break;

  case 292: /* expr: tern_expr  */
#line 1344 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4838 "raku.tab.c"
    break;

  case 293: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1348 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4844 "raku.tab.c"
    break;

  case 294: /* tern_expr: cmp_expr  */
#line 1349 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4850 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1352 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4856 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1353 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4862 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1354 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4868 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1355 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4874 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1356 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4880 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1357 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4886 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1358 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4892 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1359 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4898 "raku.tab.c"
    break;

  case 303: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1360 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4904 "raku.tab.c"
    break;

  case 304: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1361 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4910 "raku.tab.c"
    break;

  case 305: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1362 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4916 "raku.tab.c"
    break;

  case 306: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1363 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4922 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1364 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4928 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1365 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4934 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1367 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4944 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1373 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4954 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1379 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4964 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr  */
#line 1384 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 4970 "raku.tab.c"
    break;

  case 313: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1387 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4976 "raku.tab.c"
    break;

  case 314: /* divis_expr: jct_expr  */
#line 1388 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4982 "raku.tab.c"
    break;

  case 315: /* jct_expr: jct_expr '|' range_expr  */
#line 1391 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4988 "raku.tab.c"
    break;

  case 316: /* jct_expr: jct_expr '&' range_expr  */
#line 1392 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4994 "raku.tab.c"
    break;

  case 317: /* jct_expr: dor_expr  */
#line 1393 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5000 "raku.tab.c"
    break;

  case 318: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1397 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5006 "raku.tab.c"
    break;

  case 319: /* dor_expr: range_expr  */
#line 1398 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5012 "raku.tab.c"
    break;

  case 320: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1401 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5018 "raku.tab.c"
    break;

  case 321: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1402 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5024 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr  */
#line 1403 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5030 "raku.tab.c"
    break;

  case 323: /* add_expr: add_expr '~' repl_expr  */
#line 1406 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5036 "raku.tab.c"
    break;

  case 324: /* add_expr: repl_expr  */
#line 1407 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5042 "raku.tab.c"
    break;

  case 325: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1410 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5048 "raku.tab.c"
    break;

  case 326: /* repl_expr: addsub_expr  */
#line 1411 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5054 "raku.tab.c"
    break;

  case 327: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1414 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5060 "raku.tab.c"
    break;

  case 328: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1415 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5066 "raku.tab.c"
    break;

  case 329: /* addsub_expr: mul_expr  */
#line 1416 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5072 "raku.tab.c"
    break;

  case 330: /* mul_expr: mul_expr '*' unary_expr  */
#line 1419 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5078 "raku.tab.c"
    break;

  case 331: /* mul_expr: mul_expr '/' unary_expr  */
#line 1420 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5084 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '%' unary_expr  */
#line 1421 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5090 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1422 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5096 "raku.tab.c"
    break;

  case 334: /* mul_expr: unary_expr  */
#line 1423 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5102 "raku.tab.c"
    break;

  case 335: /* unary_expr: '-' unary_expr  */
#line 1426 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5108 "raku.tab.c"
    break;

  case 336: /* unary_expr: '!' unary_expr  */
#line 1427 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5114 "raku.tab.c"
    break;

  case 337: /* unary_expr: CARET unary_expr  */
#line 1428 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5120 "raku.tab.c"
    break;

  case 338: /* unary_expr: OP_REDUCE unary_expr  */
#line 1430 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5129 "raku.tab.c"
    break;

  case 339: /* unary_expr: pow_expr  */
#line 1434 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5135 "raku.tab.c"
    break;

  case 340: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1437 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5141 "raku.tab.c"
    break;

  case 341: /* pow_expr: postfix_expr  */
#line 1438 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5147 "raku.tab.c"
    break;

  case 342: /* scalar_list: VAR_SCALAR  */
#line 1441 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5153 "raku.tab.c"
    break;

  case 343: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1442 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5159 "raku.tab.c"
    break;

  case 344: /* meth_name: IDENT  */
#line 1445 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5165 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_SORT  */
#line 1446 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5171 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_MAP  */
#line 1447 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5177 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_GREP  */
#line 1448 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5183 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_SAY  */
#line 1449 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5189 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_PRINT  */
#line 1450 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5195 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_TAKE  */
#line 1451 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5201 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_RETURN  */
#line 1452 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5207 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_EXISTS  */
#line 1453 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5213 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_DELETE  */
#line 1454 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5219 "raku.tab.c"
    break;

  case 354: /* meth_name: TESTOP  */
#line 1455 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5225 "raku.tab.c"
    break;

  case 355: /* postfix_expr: call_expr  */
#line 1457 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5231 "raku.tab.c"
    break;

  case 356: /* call_expr: IDENT '(' arg_list ')'  */
#line 1460 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5240 "raku.tab.c"
    break;

  case 357: /* call_expr: IDENT '(' ')'  */
#line 1464 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5246 "raku.tab.c"
    break;

  case 358: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1466 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5255 "raku.tab.c"
    break;

  case 359: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1471 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5261 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1473 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5271 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5281 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1485 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5289 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' KW_NEW  */
#line 1489 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5297 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' IDENT  */
#line 1493 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5306 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1498 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5317 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1505 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5326 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' CARET IDENT  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5336 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' CARET IDENT  */
#line 1516 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5346 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1522 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5357 "raku.tab.c"
    break;

  case 370: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1529 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5368 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1536 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5377 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' meth_name  */
#line 1541 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5386 "raku.tab.c"
    break;

  case 373: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1546 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5397 "raku.tab.c"
    break;

  case 374: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1553 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5406 "raku.tab.c"
    break;

  case 375: /* call_expr: call_expr '.' meth_name  */
#line 1558 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5415 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_DIE expr  */
#line 1563 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5421 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_MAP closure expr  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5427 "raku.tab.c"
    break;

  case 378: /* call_expr: KW_GREP closure expr  */
#line 1567 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5433 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_SORT expr  */
#line 1569 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5439 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_SORT closure expr  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5445 "raku.tab.c"
    break;

  case 381: /* call_expr: atom  */
#line 1572 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5451 "raku.tab.c"
    break;

  case 382: /* arg_list: expr  */
#line 1575 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5457 "raku.tab.c"
    break;

  case 383: /* arg_list: arg_list ',' expr  */
#line 1576 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5463 "raku.tab.c"
    break;

  case 384: /* atom: LIT_INT  */
#line 1579 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5469 "raku.tab.c"
    break;

  case 385: /* atom: LIT_FLOAT  */
#line 1580 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5475 "raku.tab.c"
    break;

  case 386: /* atom: LIT_STR  */
#line 1581 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5481 "raku.tab.c"
    break;

  case 387: /* atom: WORDLIST  */
#line 1583 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5493 "raku.tab.c"
    break;

  case 388: /* atom: LIT_INTERP_STR  */
#line 1590 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5499 "raku.tab.c"
    break;

  case 389: /* atom: VAR_SCALAR  */
#line 1591 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5505 "raku.tab.c"
    break;

  case 390: /* atom: OP_INC VAR_SCALAR  */
#line 1592 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5511 "raku.tab.c"
    break;

  case 391: /* atom: OP_DEC VAR_SCALAR  */
#line 1593 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5517 "raku.tab.c"
    break;

  case 392: /* atom: VAR_SCALAR OP_INC  */
#line 1594 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5523 "raku.tab.c"
    break;

  case 393: /* atom: VAR_SCALAR OP_DEC  */
#line 1595 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5529 "raku.tab.c"
    break;

  case 394: /* atom: VAR_ARRAY  */
#line 1596 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5535 "raku.tab.c"
    break;

  case 395: /* atom: VAR_HASH  */
#line 1597 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5541 "raku.tab.c"
    break;

  case 396: /* atom: VAR_CAPTURE  */
#line 1599 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5549 "raku.tab.c"
    break;

  case 397: /* atom: VAR_FH  */
#line 1603 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5557 "raku.tab.c"
    break;

  case 398: /* atom: VAR_NAMED_CAPTURE  */
#line 1607 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5564 "raku.tab.c"
    break;

  case 399: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1610 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5570 "raku.tab.c"
    break;

  case 400: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1612 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5576 "raku.tab.c"
    break;

  case 401: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1614 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5582 "raku.tab.c"
    break;

  case 402: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1616 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5588 "raku.tab.c"
    break;

  case 403: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1618 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5594 "raku.tab.c"
    break;

  case 404: /* atom: VAR_HASH '{' expr '}'  */
#line 1620 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5600 "raku.tab.c"
    break;

  case 405: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1622 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5606 "raku.tab.c"
    break;

  case 406: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1624 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5612 "raku.tab.c"
    break;

  case 407: /* atom: IDENT  */
#line 1625 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5618 "raku.tab.c"
    break;

  case 408: /* atom: VAR_TWIGIL  */
#line 1627 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5626 "raku.tab.c"
    break;

  case 409: /* atom: VAR_ARRAY_TWIGIL  */
#line 1631 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5634 "raku.tab.c"
    break;

  case 410: /* atom: VAR_HASH_TWIGIL  */
#line 1635 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5642 "raku.tab.c"
    break;

  case 411: /* atom: '(' ')'  */
#line 1638 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5648 "raku.tab.c"
    break;

  case 412: /* atom: '(' expr ')'  */
#line 1639 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5654 "raku.tab.c"
    break;

  case 413: /* atom: '(' expr ',' ')'  */
#line 1641 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5660 "raku.tab.c"
    break;

  case 414: /* atom: '(' expr ',' arg_list ')'  */
#line 1643 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5667 "raku.tab.c"
    break;

  case 415: /* atom: block  */
#line 1645 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5673 "raku.tab.c"
    break;

  case 416: /* atom: KW_SUB block  */
#line 1646 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5679 "raku.tab.c"
    break;


#line 5683 "raku.tab.c"

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

#line 1648 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
