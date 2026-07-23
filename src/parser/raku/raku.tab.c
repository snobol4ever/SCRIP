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

#line 343 "raku.tab.c"

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
#define YYLAST   3606

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  413
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1051

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
       0,   356,   356,   367,   368,   371,   373,   375,   377,   381,
     383,   385,   387,   391,   394,   398,   400,   402,   404,   406,
     408,   412,   416,   418,   420,   422,   424,   426,   428,   430,
     432,   434,   436,   438,   440,   442,   444,   447,   450,   452,
     455,   458,   460,   462,   464,   466,   471,   475,   479,   484,
     488,   489,   491,   493,   495,   497,   500,   502,   504,   506,
     509,   512,   515,   518,   521,   523,   525,   527,   529,   532,
     534,   536,   538,   541,   544,   548,   551,   553,   555,   557,
     560,   563,   566,   569,   572,   575,   578,   579,   580,   581,
     582,   583,   584,   586,   588,   590,   592,   593,   594,   595,
     596,   597,   598,   600,   602,   604,   606,   607,   608,   609,
     612,   614,   616,   618,   620,   622,   624,   626,   630,   632,
     634,   636,   638,   640,   644,   646,   650,   652,   654,   656,
     660,   662,   666,   668,   670,   674,   676,   678,   682,   685,
     691,   697,   702,   708,   713,   718,   726,   737,   738,   744,
     752,   758,   764,   772,   778,   784,   793,   800,   809,   818,
     827,   834,   841,   842,   845,   848,   850,   852,   854,   856,
     858,   860,   862,   865,   867,   869,   871,   874,   877,   883,
     884,   885,   888,   891,   893,   895,   897,   899,   901,   903,
     905,   908,   910,   912,   914,   917,   920,   926,   942,   957,
     958,   970,   984,   985,   988,   991,   994,   997,  1000,  1003,
    1006,  1009,  1012,  1015,  1018,  1021,  1028,  1035,  1042,  1049,
    1053,  1057,  1061,  1065,  1072,  1079,  1086,  1093,  1103,  1111,
    1119,  1128,  1135,  1142,  1152,  1160,  1170,  1185,  1186,  1191,
    1196,  1203,  1207,  1213,  1215,  1217,  1219,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1233,  1234,  1235,  1237,  1239,
    1241,  1243,  1245,  1248,  1250,  1252,  1254,  1256,  1259,  1262,
    1266,  1269,  1272,  1275,  1277,  1281,  1284,  1287,  1290,  1293,
    1296,  1298,  1300,  1302,  1304,  1306,  1308,  1310,  1312,  1316,
    1319,  1320,  1325,  1328,  1330,  1333,  1334,  1335,  1336,  1337,
    1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1346,  1347,
    1353,  1359,  1365,  1368,  1369,  1372,  1373,  1374,  1377,  1379,
    1382,  1383,  1384,  1387,  1388,  1391,  1392,  1395,  1396,  1397,
    1400,  1401,  1402,  1403,  1404,  1407,  1408,  1409,  1410,  1415,
    1418,  1419,  1422,  1423,  1426,  1427,  1428,  1429,  1430,  1431,
    1432,  1433,  1434,  1435,  1436,  1438,  1440,  1445,  1446,  1451,
    1453,  1459,  1465,  1469,  1473,  1478,  1485,  1490,  1496,  1502,
    1509,  1516,  1521,  1526,  1533,  1538,  1543,  1545,  1547,  1549,
    1551,  1553,  1556,  1557,  1560,  1561,  1562,  1563,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1583,  1587,  1590,
    1592,  1594,  1596,  1598,  1600,  1601,  1605,  1609,  1613,  1614,
    1615,  1617,  1620,  1621
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

#define YYPACT_NINF (-752)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-413)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -752,    46,  1541,  -752,  -752,  -752,  -752,  -752,   548,   -73,
     -61,   -47,   327,  -752,  -752,  3435,  -752,  -752,  -752,    84,
    1610,    51,  2685,  2754,  2810,  2879,  2935,    -8,   -19,  2935,
    1679,   653,  2935,   131,   138,  3004,  3060,   -19,   113,   110,
     314,    19,    19,  3129,   -19,   -19,  2935,   167,   177,   205,
    3435,  -752,   190,   236,   254,  3435,  3435,  -752,  1748,   202,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
     462,  -752,  -752,  -752,  3486,   536,  -752,   437,   598,   -95,
     195,  -752,     5,   218,   162,   326,  -752,  -752,   203,   217,
     258,   445,  -752,  -752,  2935,  2935,  2935,  2935,  2935,  2935,
     459,  1804,  2935,   494,  2935,  2935,   216,  1874,   178,   385,
     -37,  -752,   -19,  -752,  -752,   313,   305,  -752,  1930,  -752,
     270,    58,   180,   187,   483,   509,   729,   471,  1748,   440,
    1748,   366,  1748,   -19,  1748,   -19,     7,   136,   170,  -752,
    -752,   423,  -752,   436,   466,   487,   489,    12,    94,  1748,
     -19,  1748,   -19,    91,  2000,  -752,  2935,  2935,  -752,  2935,
    2935,  -752,  2935,  2935,  2935,  2056,  2935,  -752,   555,  -752,
    -752,  -752,   508,   488,    23,  -752,   527,  -752,  -752,  -752,
    -752,  -752,   235,   497,  1036,  2935,  2935,  2935,  2935,  2935,
    2935,  2935,  2935,  -752,  2935,  2935,  2935,  2935,  2935,  2935,
    2935,  2935,  -752,  3435,  3435,  3435,  3435,  3435,  3435,  3435,
    3435,  3435,  3435,  3435,  3435,  3435,  3435,  3435,   799,  3435,
    3435,  3435,  3435,  3435,  3435,  3435,  3435,  3435,  3435,  3435,
    3435,  3435,  3435,  3435,   674,   573,   299,   582,   605,   607,
     619,   640,   648,   122,  -752,   542,   685,   664,   688,   700,
     707,   822,   711,  -752,   563,  2935,   831,  2935,  -752,  2935,
     753,   235,   659,  -752,  2935,  2935,  -752,  3185,  -752,  3254,
    -752,   209,   212,   286,   478,   746,   757,  -752,   680,   686,
    2935,  2935,  2935,  2935,  2935,  2935,  2935,  -752,  2935,   691,
    2935,  2935,  -752,  2935,   696,   796,   702,  -752,   850,  2935,
    -752,  3435,  3435,    -1,  -752,  -752,  -752,  -752,  2935,  2935,
    -752,   867,  2935,   872,  2935,   704,   844,   717,  -752,  2935,
    2935,   884,   780,   783,   786,   827,   835,   807,  -752,  -752,
    -752,   -19,   -10,  -752,    15,   830,   836,  -752,  -752,  2125,
    -752,   637,   832,   134,   852,  2685,  2754,  2935,   781,   260,
     310,  -752,   285,   839,   841,   842,   843,   848,   849,   851,
     854,   855,   856,   858,   859,   860,   861,   862,   863,   868,
     868,   868,   868,   868,   868,   875,   868,   868,   868,   868,
     868,   868,   868,   868,  -752,  -752,  -752,   -95,  -752,  -752,
    -752,   873,   873,   218,   162,   326,   326,  -752,  -752,  -752,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
    -752,  -752,  -752,   865,   959,   866,  -752,  2181,  -752,  -752,
    -752,  -752,  -752,  -752,  2935,  2251,  -752,   878,   882,   883,
    -752,  2307,  -752,  2377,  -752,   869,   881,   864,  -752,  -752,
     876,  -752,   877,  1748,   199,   920,   128,  2433,   434,   888,
    2935,  -752,  3310,  -752,  2935,  -752,    21,  -752,  2935,  2935,
     889,   984,  2125,   890,   891,   892,   893,   894,   895,   896,
     467,  2125,   897,   901,   617,   -19,   -15,  3379,  -752,   -19,
     -19,    13,    -9,    65,  -752,     4,   898,   719,  1163,   902,
     903,    31,   900,   871,   916,   880,   -19,   -19,   -19,   906,
     907,   922,  2935,  -752,  -752,  -752,  -752,  -752,  -752,  1016,
    1017,  -752,   -17,   898,   724,    22,   748,    43,  -752,   726,
    1018,  2935,  2935,  2935,    48,    81,   206,  -752,   239,  2935,
    2935,  -752,  2935,  2935,  -752,  2935,  2935,  2935,  2935,  2935,
    2935,  2935,  2935,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
    3435,  2503,  -752,  2559,   913,   728,   914,  -752,   733,  2935,
    2935,  2935,  -752,   735,   149,  -752,   739,   743,  -752,  -752,
    -752,  -752,  -752,   745,  2935,  -752,  2935,  2935,  2935,   749,
    -752,    96,  -752,   915,  1748,   649,   917,   898,   754,   942,
     943,  2935,  -752,   756,  -752,  -752,  -752,  -752,  -752,  -752,
    -752,  -752,   759,  -752,  -752,  -752,   857,  -752,  -752,  1748,
     -19,  -752,  -752,  1023,  -752,  1027,  1033,  -752,  1063,  1064,
    -752,   898,   758,  2685,  2754,  1352,  -752,   312,  -752,  -752,
    2935,   -19,  -752,  -752,  -752,   954,   955,  1065,  -752,  -752,
    -752,  -752,  2935,   972,  -752,  -752,    49,   579,   732,  1038,
    -752,  -752,   898,   898,   767,   769,  1083,  1084,  1086,  -752,
    -752,   153,   976,   975,   240,  2935,  2935,  2935,  2935,  2935,
    2935,  -752,  -752,  -752,  -752,   241,   274,   282,   295,   298,
     304,   308,   309,   318,   330,   332,   333,  -752,  -752,   774,
    -752,   776,   782,  -752,   983,  -752,  -752,   985,   986,   987,
    -752,  2935,  -752,  1095,  -752,  2125,   989,   654,  -752,  -752,
     990,  1039,  1040,  -752,   787,  -752,  2935,  -752,  -752,   898,
    -752,  -752,   655,   993,   994,   -15,  -752,   789,   887,   -19,
     -19,   -19,  -752,  -752,  -752,  -752,     9,   344,   174,  -752,
     334,  2935,  2935,  2935,  2935,  2935,  2935,  2935,  2935,  -752,
     -19,   988,  -752,  -752,   -19,   996,  2935,  -752,   514,   578,
      35,    59,   750,   997,   998,   674,  -752,  -752,   898,   898,
    1113,  1114,  1115,  2935,  2629,  1019,  1020,  -752,   343,   356,
     357,   361,   376,   390,  -752,  -752,  -752,  -752,  -752,  -752,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
    -752,  -752,  -752,  -752,  1050,   808,  -752,  -752,  -752,  2935,
    2935,  2125,   656,  -752,  -752,  2935,  -752,  -752,  -752,  -752,
     -19,   -19,  -752,  -752,  -752,  -752,  -752,  1118,  1119,  2935,
    2935,  2935,  -752,  -752,  -752,   401,   420,   422,   428,   431,
     432,   442,   450,  -752,  -752,  -752,  2935,  1001,  -752,    28,
    1057,  -752,    44,  -752,  1121,  1122,  1011,  2935,  -752,  1123,
    1125,  1012,  2935,  -752,   111,   192,  1021,  1022,  -752,  -752,
     595,  -752,  -752,  -752,  -752,  -752,   453,  1013,   810,  2935,
    2935,  -752,  -752,  -752,  -752,  -752,  -752,  2935,  1024,  -752,
    -752,   814,  -752,   681,   912,  -752,  -752,  -752,   456,   468,
     481,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  1025,
     -19,  1026,   819,  1028,  1283,  1026,   821,  1029,  1031,  -752,
    1034,  1041,  1044,  -752,  1049,  1126,  1133,  1069,  2935,  -752,
    1134,  1135,  1070,  2935,  -752,  -752,  -752,    45,  -752,  -752,
    -752,  1032,   490,   498,  -752,  -752,  1079,  -752,   -19,  -752,
    -752,  -752,  -752,  2935,  -752,  -752,  1026,  -752,  2685,  2754,
    1421,  -752,   346,  -752,  1026,  -752,  -752,  -752,  -752,  -752,
    -752,  1080,  1081,  -752,  1082,  1098,  1099,  -752,  1100,  1026,
     823,  -752,  -752,  -752,  -752,  -752,  1048,  -752,   375,   211,
    -752,   506,  2935,  2935,  2935,  2935,  2935,  2935,  2935,  2935,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  1026,
     -19,  2935,  2935,  2935,  -752,  -752,  -752,   528,   530,   531,
     532,   541,   543,   564,   576,  -752,  -752,   585,   593,   599,
    -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,  -752,
    -752
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   384,   385,   386,   388,   389,   394,
     395,   405,   404,   406,   407,     0,   396,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,     0,     0,     0,     0,    87,     0,     3,
       4,    88,    89,    96,    97,    98,    99,    90,    91,   106,
       0,   107,   108,   109,    95,     0,   292,   294,   312,   314,
     317,   319,   322,   324,   326,   329,   334,   339,   341,   355,
     381,     0,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,   394,
     395,   405,     0,   412,   337,     0,   389,    31,     0,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,   413,
     291,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,   100,     0,
       0,   101,     0,     0,     0,     3,     0,   379,    92,    94,
     376,   199,     0,     0,     0,   338,     0,   390,   391,   336,
     335,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,   359,     0,     0,     0,     0,     0,
     364,     0,   363,   357,     0,     0,     0,     0,    26,     0,
     408,     0,     0,    34,     0,     0,     6,     0,     9,     0,
      10,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,    38,     0,     0,   113,     0,   125,     0,     0,
     144,     0,     0,     0,     3,   151,    41,    42,     0,     0,
     147,     0,     0,     0,     0,     0,   128,     0,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   378,
     380,     0,     0,   202,     0,     0,     0,   237,   409,     0,
     256,   389,   394,   395,   405,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   297,
     298,   301,   302,   295,   296,     0,   299,   300,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   313,   315,   316,
     318,   320,   321,   323,   325,   327,   328,   333,   330,   331,
     332,   340,   344,   354,   348,   349,   350,   351,   352,   353,
     346,   347,   345,   375,     0,   372,    47,     0,    81,    82,
      83,    84,    85,    44,     0,     0,   358,   399,   400,   401,
      49,     0,   367,     0,   356,     0,     0,     0,   290,    33,
       0,   383,     0,     0,     0,   386,   404,     0,     0,     0,
       0,    23,     0,    24,     0,    25,     0,   154,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   409,     0,     0,   116,   409,
       0,     0,   320,   321,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,     0,   409,     0,
       0,     0,     0,   102,   103,   104,   105,   289,    93,     0,
       0,   202,     0,     0,     0,     0,     0,     0,   410,     0,
       0,     0,     0,     0,     0,     0,     0,   282,     0,     0,
       0,   283,     0,     0,   284,     0,     0,     0,     0,     0,
       0,     0,     0,   257,    51,    53,    55,    58,    56,    57,
      52,    54,    64,    66,    68,    71,    69,    70,    65,    67,
       0,     0,   368,     0,     0,     0,     0,   161,     0,     0,
       0,     0,   366,     0,   404,   362,     0,     0,   399,   400,
     401,    32,     5,     0,     0,    11,     0,     0,     0,     0,
      16,     0,    15,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,    72,    75,    74,    78,    76,    77,
      73,    36,     0,    79,    80,    39,   110,   115,   114,     0,
       0,   124,   141,     0,   143,     0,     0,   248,     0,     0,
     150,     0,     0,     0,     0,     0,   162,     0,    28,    27,
       0,     0,   145,   402,   403,     0,     0,   126,   129,   130,
     133,   134,     0,     0,   200,   201,     0,     0,     0,     0,
     198,   159,     0,     0,     0,     0,     0,     0,     0,   236,
     411,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,   273,   280,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   293,   374,     0,
     371,     0,     0,    46,     0,    48,   160,     0,     0,     0,
     365,     0,   360,     0,   361,     0,     0,     0,   244,   243,
       0,     0,     0,    18,     0,    19,     0,    22,   153,     0,
      30,    29,     0,   411,   411,     0,   117,     0,   118,     0,
       0,     0,   249,   250,   149,   251,     0,     0,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,     0,    62,    63,     0,     0,     0,   197,     0,     0,
       0,     0,     0,     0,     0,     0,   158,   156,     0,     0,
       0,     0,     0,     0,     0,   399,   401,   277,     0,     0,
       0,     0,     0,     0,   285,   286,   287,   288,   258,   260,
     262,   265,   263,   264,   259,   261,   373,   370,   369,    45,
      59,    60,    61,   241,     0,     0,    13,    12,    17,     0,
       0,     0,     0,   152,     7,     0,    37,    40,   112,   111,
     409,     0,   122,   142,   139,   140,   252,     0,     0,     0,
       0,     0,   166,   167,   164,     0,     0,     0,     0,     0,
       0,     0,     0,   148,   146,   127,     0,     0,   138,     0,
       3,   232,     0,   229,     0,     0,     0,     0,   208,     0,
       0,     0,     0,   203,     0,     0,     0,     0,   204,   205,
       0,   155,   157,   238,   239,   240,     0,   161,     0,     0,
       0,   267,   269,   272,   270,   271,   268,     0,   411,   246,
     245,     0,    20,     0,   119,   120,   253,   254,     0,     0,
       0,   168,   170,   172,   175,   173,   174,   169,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   219,
       0,     0,     0,   220,     0,     0,     0,     0,     0,   210,
       0,     0,     0,     0,   209,   206,   207,     0,   235,   276,
     275,   160,     0,     0,   242,    14,   411,     8,     0,   123,
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
    -752,  -752,     1,  -752,  -469,  -611,  -752,  -752,  -752,  -752,
    -752,   191,  -752,  -752,  -752,  -752,  -752,  -266,  -751,  -752,
    -752,  -752,   650,  -752,  -752,   596,   777,  -308,   214,   676,
      -2,  -206,  -752,   713,  1006,  -752,   568,   -11,  1002,  1003,
     723,    -5,  -752,  -752,  -233,  -752,  -752,   -54,  -752
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   184,    60,    61,   478,    62,    63,    64,    65,
      66,   857,    67,    68,   491,    69,    70,   305,   861,    71,
      72,   332,   512,    73,   517,   576,   448,   487,   113,   163,
     119,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   278,   413,    88,    89,   120,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,     2,   415,   375,   509,   736,   138,   617,   457,   484,
     114,    24,   510,   485,   627,   137,   220,   221,   863,   836,
     129,   131,   133,   135,   136,   484,   514,   141,   143,   485,
     146,   484,   484,   150,   152,   485,   485,   335,   484,   657,
     658,   167,   485,   659,   170,   175,     3,   245,   103,   864,
     179,   180,   102,   254,   484,   484,   182,   865,   485,   485,
     105,   121,   122,   123,   262,   124,   104,   625,   640,   641,
     628,   629,   256,   869,   675,   837,   838,   281,   676,   223,
     224,   870,   125,   298,   677,   336,   126,   678,   679,   623,
     257,   680,   237,   238,   239,   240,   241,   242,   115,   866,
     246,   721,   248,   249,   225,   657,   658,   290,    59,   659,
     722,   660,    59,   666,   667,   668,   261,   511,   225,    59,
     319,   311,   486,   871,   291,   935,   279,   832,   289,   948,
     294,   299,   296,   936,    59,   320,   156,   264,   513,   312,
      59,   626,   867,   147,   597,   663,   162,   315,   598,   317,
     148,   921,   322,   157,   323,   324,   868,   325,   326,   642,
     327,   328,   329,   327,   330,   265,   872,   925,   989,   287,
     965,   669,   288,   127,   973,   937,   681,   767,   225,   266,
     873,   171,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   172,   361,   362,   363,   364,   365,   366,   367,   368,
     290,   588,   292,   313,   176,   293,   940,   664,   665,   682,
     301,   302,   391,   392,   941,   997,    74,   291,   938,   173,
     630,   314,   711,  1011,   397,   398,   399,   400,   401,   424,
     250,   158,   939,   251,   470,   154,   174,   290,  1018,   474,
      59,   139,   140,   103,   425,   481,   177,   661,   106,   225,
     107,   153,   155,   435,   291,   437,   942,   438,   168,   169,
     783,   522,   441,   442,   178,   444,   828,   449,  1035,   106,
     183,   107,    92,    93,   252,   784,   227,   228,   463,   464,
     465,   466,   467,   468,   469,   519,   529,   267,   472,   473,
     482,   483,   303,   959,   269,   292,   222,   304,   293,   943,
     101,   268,   843,   530,   584,   488,   489,   490,   270,   233,
     493,   535,   495,   944,   536,   537,   450,   499,   500,   452,
     585,   538,   226,   586,   539,   540,   139,   306,   541,   542,
     451,   728,   292,   453,   683,   293,   532,   234,   751,  1025,
     159,   752,   753,   524,   525,   526,   528,   295,   754,   297,
     300,   755,   756,   533,   697,   757,   758,   160,   338,   339,
     307,   430,   503,   565,   316,   744,   318,   684,   787,   794,
     839,   568,  1002,   281,   840,  1003,  1004,   573,   235,   577,
     283,   158,  1005,   284,   285,  1006,  1007,   841,   531,  1008,
    1009,   263,   290,   454,   264,   504,   776,   777,    74,    92,
      93,  1021,   795,   505,   281,  1022,   202,   455,   603,   291,
     796,   283,   259,   543,   284,   285,   506,   612,  1023,   552,
     416,   417,   566,   797,   769,   553,   798,   101,   229,   554,
     555,   161,   799,   202,   258,   161,   800,   801,   534,   556,
     759,   583,   230,   231,   232,   182,   802,   106,   593,   107,
     595,   557,   596,   558,   559,   307,   599,   600,   803,   236,
     804,   805,   844,   823,   604,   287,   280,   202,   288,   281,
     282,   891,   842,   243,  1010,   620,   283,   606,   607,   284,
     285,   277,   608,   286,   892,   893,   637,   292,   185,   894,
     293,   186,   187,   271,   272,   273,   287,   609,   188,   288,
     653,   189,   190,  1024,   895,   191,   192,   699,   247,   702,
     255,   610,   881,   882,   203,   204,   205,   206,   896,   672,
     673,   674,   552,   274,   207,   208,   209,   685,   686,   911,
     687,   688,   717,   689,   690,   691,   692,   693,   694,   695,
     696,   553,   880,   554,   306,   508,   210,   211,   912,   555,
     913,   922,   556,   557,   926,   590,   914,   307,   591,   915,
     916,   287,   194,   558,   288,   195,   196,   707,   708,   709,
     917,   559,   197,   308,   705,   198,   199,   604,   918,   200,
     201,   949,   716,   193,   960,   718,   719,   402,   611,   606,
     414,   264,   724,   402,   309,   403,   961,   404,   405,   732,
     456,   403,   610,   404,   405,   304,   406,   407,   331,   962,
     334,   810,   406,   407,   408,   409,   310,   737,   992,   812,
     408,   409,   410,   411,   412,   340,   993,   307,   410,   411,
     412,   747,   748,   750,  1026,   333,   859,   768,   760,   990,
      91,   860,    92,    93,    94,    95,    96,    97,    98,   552,
     765,   553,   554,   555,   337,    99,  1040,   202,  1041,  1042,
    1043,   815,   556,   144,   557,   426,   264,   145,   100,  1044,
     101,  1045,   822,   788,   789,   790,   791,   792,   793,   212,
     213,   214,   215,   216,   217,   558,   434,   264,   402,   616,
     618,   218,  1046,   621,   622,   624,   403,   559,   404,   405,
     862,   219,    74,   418,  1047,   860,   604,   406,   407,   813,
     647,   648,   649,  1048,   606,   408,   409,   947,   164,   166,
     610,  1049,   860,   410,   411,   412,   419,  1050,   420,    91,
     888,    92,    93,    94,    95,    96,    97,    98,   615,   275,
     421,   264,   770,   276,    99,   771,   772,   773,   774,   845,
     846,   847,   848,   849,   850,   851,   852,   520,   484,   101,
     874,   422,   485,   875,   858,   876,   877,   901,   745,   423,
     725,   903,   746,   726,   428,   817,   824,   902,   264,   825,
     264,   886,   440,   264,     4,     5,     6,     7,   388,   389,
     390,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,   957,   460,   461,   264,   384,   385,   386,   338,
     462,   427,   112,    28,   338,   471,   429,   899,   900,   475,
     339,   430,    33,   476,   477,   479,   339,   496,   339,   431,
      41,    42,    43,   433,   738,    46,   432,   908,   909,   910,
     498,   339,   631,   632,    50,   436,    51,   662,   632,   670,
     264,   704,   264,   458,   919,   761,   706,   264,   710,   264,
     480,   924,   712,   713,   459,   930,   714,   264,   338,   715,
     934,   497,   720,   591,   439,    53,    54,   729,   632,   733,
     264,   492,   734,   264,   735,   477,   494,   952,   953,    55,
     778,   632,   779,   632,   501,   954,    56,   806,   264,   807,
     713,   502,   142,    58,   503,   808,   264,   504,    59,   527,
     338,   821,   830,   339,   831,   477,   369,   370,   371,   372,
     373,   374,   972,   376,   377,   378,   379,   380,   381,   382,
     383,   898,   264,   951,   264,   507,   984,   956,   264,   958,
     477,   988,   966,   632,   974,   632,  1019,   632,   505,   829,
     395,   396,   515,   833,   834,   835,   506,   521,   516,   523,
     544,   858,   545,   546,   547,   560,   998,   999,  1001,   548,
     549,   219,   550,   562,   853,   551,   552,   553,   855,   554,
     555,   556,   557,   558,   559,   569,   225,   561,   563,   570,
     571,   579,   580,   587,   602,   578,   601,   581,   582,   644,
    1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,   646,   592,
     643,   604,   605,   606,   607,   608,   609,   610,   613,  1037,
    1038,  1039,   614,   638,   639,   304,   645,   650,   651,   652,
     654,   655,   671,   739,   703,   705,   723,   740,   727,     4,
       5,     6,     7,   741,   904,   905,   341,   342,   343,   344,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     345,   346,    24,   730,   731,    25,    26,    27,    28,   347,
     348,    31,    32,   742,   743,   762,   763,    33,    34,    35,
      36,    37,    38,   349,   350,    41,    42,    43,    44,    45,
      46,    47,   764,   766,   775,    48,    49,   780,   781,    50,
     782,    51,   785,   786,   809,    52,   810,   811,   812,   814,
     816,   818,   819,   820,   826,   827,   854,   856,   878,   879,
     883,   884,   885,   897,   920,   923,   889,   890,   906,   907,
      53,    54,   929,   933,   964,   927,   928,   931,    74,   932,
     981,   950,   945,   946,    55,   955,   963,   982,   985,   986,
     975,    56,   976,   860,   996,   977,   967,    57,    58,   701,
     991,   656,   978,    59,   351,   979,     4,     5,     6,     7,
     980,  1020,   995,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   633,   634,    24,
     983,   987,    25,    26,    27,    28,    29,   635,    31,    32,
     994,  1012,  1013,  1014,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,  1015,
    1016,  1017,    48,    49,   589,   387,    50,   393,    51,   394,
       0,     0,    52,     0,  1036,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,    57,    58,     4,     5,     6,     7,
      59,   636,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   968,   969,    24,
       0,     0,    25,    26,    27,    28,    29,   970,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,    48,    49,     0,     0,    50,     0,    51,     0,
       0,     0,    52,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,    57,    58,    46,     0,     0,     0,
      59,   971,     0,     0,     0,    50,     0,    51,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    53,    54,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,     0,
      55,     0,    33,     0,     0,     0,     0,    56,     0,     0,
      41,    42,    43,   142,    58,    46,     0,     0,     0,    59,
     749,     0,     0,     0,    50,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,   142,    58,     4,     5,     6,     7,    59,  1000,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     0,
       0,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
      48,    49,     0,     0,    50,     0,    51,     0,     0,     0,
      52,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    53,    54,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,     0,    55,
       0,    33,     0,     0,     0,     0,    56,     0,     0,    41,
      42,    43,    57,    58,    46,     0,     0,     0,    59,     0,
       0,     0,     0,    50,     0,    51,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,   117,   118,    46,     0,     0,     0,    59,     0,     0,
       0,     0,    50,     0,    51,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,     0,    55,     0,    33,
       0,     0,     0,     0,    56,     0,     0,    41,    42,    43,
     142,    58,    46,     0,     0,     0,    59,     4,     5,     6,
       7,    50,     0,    51,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    53,    54,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    55,     0,    46,     0,
       0,     0,     0,    56,     0,     0,     0,    50,     0,    51,
      58,   181,     0,     0,     0,    59,     0,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    53,    54,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    55,     0,     0,    33,     0,     0,     0,    56,
       0,     0,     0,    41,    42,    43,    58,   244,    46,     0,
       0,    59,     0,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,    58,   253,     0,     0,
       0,    59,     0,     4,     5,     6,     7,     0,     0,     0,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,   321,    53,    54,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    55,     0,
       0,    33,     0,     0,     0,    56,     0,     0,     0,    41,
      42,    43,    58,   260,    46,     0,     0,    59,     0,     4,
       5,     6,     7,    50,     0,    51,   116,   109,   110,   111,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,    28,     0,
       0,     0,     0,     0,    53,    54,     0,    33,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    55,     0,
      46,     0,     0,     0,     0,    56,     0,     0,     0,    50,
       0,    51,    58,     0,   183,     0,     0,    59,     4,     5,
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
      56,     0,     0,     0,    50,     0,    51,    58,   518,     0,
       0,     0,    59,     0,     4,     5,     6,     7,     0,     0,
       0,   116,   109,   110,   111,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    53,    54,     0,     0,     0,
       0,     0,   112,    28,     0,     0,     0,     0,     0,    55,
       0,     0,    33,     0,     0,     0,    56,     0,     0,     0,
      41,    42,    43,    58,   564,    46,     0,     0,    59,     0,
       4,     5,     6,     7,    50,     0,    51,   116,   109,   110,
     111,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,    53,    54,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    55,
       0,    46,     0,     0,     0,     0,    56,     0,     0,     0,
      50,     0,    51,    58,   567,     0,     0,     0,    59,     0,
       4,     5,     6,     7,     0,     0,     0,   116,   109,   110,
     111,   574,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,    55,     0,     0,    33,     0,
       0,     0,    56,     0,     0,     0,    41,    42,    43,    58,
     572,    46,     0,     0,    59,     0,     4,     5,   445,     7,
      50,     0,    51,   116,   109,   110,   111,   446,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,    58,
     575,     0,     0,     0,    59,     0,     4,     5,     6,     7,
       0,     0,     0,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    53,    54,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    55,     0,     0,    33,     0,     0,     0,    56,     0,
       0,     0,    41,    42,    43,    58,   181,    46,     0,     0,
      59,     0,     4,     5,     6,     7,    50,     0,    51,   116,
     109,   110,   111,   574,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,    53,    54,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    55,     0,    46,     0,     0,     0,     0,    56,     0,
       0,     0,    50,     0,    51,    58,   698,     0,     0,     0,
      59,     0,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    53,    54,     0,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,    55,     0,     0,
      33,     0,     0,     0,    56,     0,     0,     0,    41,    42,
      43,    58,   700,    46,     0,     0,    59,     0,     4,     5,
       6,     7,    50,     0,    51,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,    58,   887,     0,     0,     0,    59,     4,     5,     6,
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
      51,    58,     0,     0,     0,     0,   165,     4,     5,   445,
       7,     0,     0,     0,   116,   109,   110,   111,   446,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,     0,   443,    46,     0,
       0,     0,    59,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,   447,     0,     0,     0,
       0,    59,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,     0,   594,    46,     0,     0,     0,    59,     4,     5,
       6,     7,    50,     0,    51,   108,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   112,     0,     0,     0,
       0,     0,     0,    53,    54,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    55,     0,    46,
       0,     0,     0,     0,    56,     0,     0,     0,    50,     0,
      51,   619,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -412,  -412,     0,     0,    53,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    58,     0,     0,
    -412,  -412,    59,  -412,  -412,  -412,  -412,  -412,  -412,  -412,
    -412,  -412,  -412,  -412,  -412,  -412,     0,     0,     0,  -412,
       0,     0,     0,     0,     0,     0,     0,  -412,  -412,  -412,
    -412,     0,  -412,     0,     0,  -412,  -412,  -412,  -412,  -412,
    -412,     0,  -412,  -412,  -412,     0,  -412
};

static const yytype_int16 yycheck[] =
{
       2,     0,   235,   209,    14,   616,    14,   476,   274,    10,
      15,    26,    22,    14,    10,    26,   111,   112,   769,    10,
      22,    23,    24,    25,    26,    10,   334,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    14,    10,    56,
      57,    43,    14,    60,    46,    50,     0,   101,   109,    14,
      55,    56,   125,   107,    10,    10,    58,    22,    14,    14,
     107,    10,    11,    12,   118,    14,   127,    76,    37,    38,
      66,    67,   109,    14,    26,    66,    67,    29,    30,    74,
      75,    22,    31,    76,    36,    62,    35,    39,    40,    76,
     127,    43,    94,    95,    96,    97,    98,    99,    14,    64,
     102,     5,   104,   105,   113,    56,    57,    26,   127,    60,
      14,   128,   127,    70,    71,    72,   118,   127,   113,   127,
      29,   109,   123,    64,    43,    14,   128,   738,   130,   880,
     132,   124,   134,    22,   127,    44,    26,   124,   123,   127,
     127,    76,   107,    12,   123,   123,   127,   149,   456,   151,
      12,   123,   154,    43,   156,   157,   121,   159,   160,   128,
     162,   163,   164,   165,   166,   107,   107,   123,   123,   121,
     921,   128,   124,   122,   925,    64,   128,   128,   113,   121,
     121,    14,   184,   185,   186,   187,   188,   189,   190,   191,
     192,    14,   194,   195,   196,   197,   198,   199,   200,   201,
      26,    73,   121,   109,    14,   124,    14,   515,   516,   128,
      74,    75,   223,   224,    22,   966,     2,    43,   107,    14,
     486,   127,    73,   974,   229,   230,   231,   232,   233,   107,
      14,   121,   121,    17,   288,   122,    31,    26,   989,   293,
     127,    27,    28,   109,   122,   299,    10,   513,   120,   113,
     122,    37,    38,   255,    43,   257,    64,   259,    44,    45,
     107,   127,   264,   265,    10,   267,   735,   269,  1019,   120,
      68,   122,    94,    95,    58,   122,   114,   115,   280,   281,
     282,   283,   284,   285,   286,   339,    26,   107,   290,   291,
     301,   302,   122,   904,   107,   121,   101,   127,   124,   107,
     122,   121,   128,    43,   105,   304,   308,   309,   121,   106,
     312,    26,   314,   121,    29,    30,   107,   319,   320,   107,
     121,    36,   104,   124,    39,    40,   112,   121,    43,    44,
     121,   597,   121,   121,   128,   124,    26,   120,    26,   128,
      26,    29,    30,   345,   346,   347,   348,   133,    36,   135,
     136,    39,    40,    43,   560,    43,    44,    43,   123,   124,
     121,   121,   121,   417,   150,   631,   152,   128,   128,   128,
      26,   425,    26,    29,    30,    29,    30,   431,   120,   433,
      36,   121,    36,    39,    40,    39,    40,    43,   128,    43,
      44,   121,    26,   107,   124,   121,   662,   663,   184,    94,
      95,    26,   128,   121,    29,    30,   121,   121,   462,    43,
     128,    36,   107,   128,    39,    40,   121,   471,    43,   121,
     121,   122,   424,   128,   657,   121,   128,   122,   102,   121,
     121,   121,   128,   121,   121,   121,   128,   128,   128,   121,
     128,   443,   116,   117,   118,   447,   128,   120,   450,   122,
     452,   121,   454,   121,   121,   121,   458,   459,   128,    14,
     128,   128,   128,   729,   121,   121,    26,   121,   124,    29,
      30,   128,   128,    14,   128,   477,    36,   121,   121,    39,
      40,    10,   121,    43,   128,   128,   488,   121,    26,   128,
     124,    29,    30,    10,    11,    12,   121,   121,    36,   124,
     502,    39,    40,   128,   128,    43,    44,   561,    14,   563,
     125,   121,   778,   779,    77,    78,    79,    80,   128,   521,
     522,   523,   121,    14,    87,    88,    89,   529,   530,   128,
     532,   533,   586,   535,   536,   537,   538,   539,   540,   541,
     542,   121,   775,   121,   121,   331,   109,   110,   128,   121,
     128,   859,   121,   121,   862,   121,   128,   121,   124,   128,
     128,   121,    26,   121,   124,    29,    30,   569,   570,   571,
     128,   121,    36,   107,   121,    39,    40,   121,   128,    43,
      44,   128,   584,   121,   128,   587,   588,    14,   121,   121,
      17,   124,   594,    14,   107,    22,   128,    24,    25,   601,
     122,    22,   121,    24,    25,   127,    33,    34,    53,   128,
     122,   121,    33,    34,    41,    42,   127,   619,   128,   121,
      41,    42,    49,    50,    51,   128,   128,   121,    49,    50,
      51,   633,   634,   635,   128,   127,   122,    58,   640,   947,
      92,   127,    94,    95,    96,    97,    98,    99,   100,   121,
     652,   121,   121,   121,   127,   107,   128,   121,   128,   128,
     128,   715,   121,    10,   121,   123,   124,    14,   120,   128,
     122,   128,   726,   675,   676,   677,   678,   679,   680,    81,
      82,    83,    84,    85,    86,   121,   123,   124,    14,   475,
     476,    93,   128,   479,   480,   481,    22,   121,    24,    25,
     122,   103,   488,   121,   128,   127,   121,    33,    34,   711,
     496,   497,   498,   128,   121,    41,    42,   122,    42,    43,
     121,   128,   127,    49,    50,    51,   121,   128,   121,    92,
     784,    94,    95,    96,    97,    98,    99,   100,   121,    10,
     121,   124,    10,    14,   107,    13,    14,    15,    16,   751,
     752,   753,   754,   755,   756,   757,   758,   120,    10,   122,
      10,   121,    14,    13,   766,    15,    16,   821,    10,   121,
     121,   825,    14,   124,   110,   121,   121,   121,   124,   124,
     124,   783,   123,   124,     3,     4,     5,     6,   220,   221,
     222,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,   121,   123,   124,   124,     7,     8,     9,   123,
     124,   126,    31,    32,   123,   124,   128,   819,   820,   123,
     124,   121,    41,    27,    28,   123,   124,   123,   124,   122,
      49,    50,    51,   122,   620,    54,    14,   839,   840,   841,
     123,   124,   123,   124,    63,    14,    65,   123,   124,   123,
     124,   123,   124,   107,   856,   641,   123,   124,   123,   124,
      10,   860,   123,   124,   107,   867,   123,   124,   123,   124,
     872,    27,   123,   124,   121,    94,    95,   123,   124,   123,
     124,    14,   123,   124,    27,    28,    14,   889,   890,   108,
     123,   124,   123,   124,    10,   897,   115,   123,   124,   123,
     124,   121,   121,   122,   121,   123,   124,   121,   127,   128,
     123,   124,   123,   124,    27,    28,   203,   204,   205,   206,
     207,   208,   924,   210,   211,   212,   213,   214,   215,   216,
     217,   123,   124,   123,   124,   128,   938,   123,   124,    27,
      28,   943,   123,   124,   123,   124,   123,   124,   121,   735,
     227,   228,   122,   739,   740,   741,   121,   125,   122,   107,
     121,   963,   121,   121,   121,    90,   968,   969,   970,   121,
     121,   103,   121,    14,   760,   121,   121,   121,   764,   121,
     121,   121,   121,   121,   121,   107,   113,   122,   122,   107,
     107,   110,   128,    73,    10,   126,   107,   121,   121,   128,
    1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,   128,   121,
     110,   121,   121,   121,   121,   121,   121,   121,   121,  1021,
    1022,  1023,   121,   121,   121,   127,   110,   121,   121,   107,
      14,    14,    14,    10,   121,   121,   121,    10,   121,     3,
       4,     5,     6,    10,   830,   831,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   121,   121,    29,    30,    31,    32,    33,
      34,    35,    36,    10,    10,   121,   121,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    27,   121,    56,    59,    60,    14,    14,    63,
      14,    65,   126,   128,   121,    69,   121,   121,   121,    14,
     121,   121,    73,    73,   121,   121,   128,   121,   121,   121,
       7,     7,     7,    73,   123,    68,   107,   107,    10,    10,
      94,    95,   121,   121,   920,    14,    14,    14,   924,    14,
      14,   128,   121,   121,   108,   121,   121,    14,    14,    14,
     121,   115,   121,   127,   963,   121,   128,   121,   122,   563,
     128,   511,   121,   127,   128,   121,     3,     4,     5,     6,
     121,   123,   958,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     121,   121,    29,    30,    31,    32,    33,    34,    35,    36,
     121,   121,   121,   121,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,   121,
     121,   121,    59,    60,   447,   219,    63,   225,    65,   226,
      -1,    -1,    69,    -1,  1020,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   121,   122,     3,     4,     5,     6,
     127,   128,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      -1,    -1,    69,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,   121,   122,    54,    -1,    -1,    -1,
     127,   128,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      49,    50,    51,   121,   122,    54,    -1,    -1,    -1,   127,
     128,    -1,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,   121,   122,     3,     4,     5,     6,   127,   128,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    -1,    -1,
      69,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
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
      51,   121,   122,    54,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,
     121,   122,    54,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    63,    -1,    65,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,
     122,   123,    -1,    -1,    -1,   127,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    41,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    49,    50,    51,   122,   123,    54,    -1,
      -1,   127,    -1,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,   123,    -1,    -1,
      -1,   127,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    23,    94,    95,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    41,    -1,    -1,    -1,   115,    -1,    -1,    -1,    49,
      50,    51,   122,   123,    54,    -1,    -1,   127,    -1,     3,
       4,     5,     6,    63,    -1,    65,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,
      54,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,
      -1,    65,   122,    -1,    68,    -1,    -1,   127,     3,     4,
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
      51,    -1,   122,    54,    -1,    -1,    -1,   127,     3,     4,
       5,     6,    63,    -1,    65,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,
      65,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      74,    75,   127,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,    -1,   106,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120
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
     159,   159,   159,    14,   123,   176,   159,    14,   159,   159,
      14,    17,    58,   123,   176,   125,   109,   127,   121,   107,
     123,   159,   176,   121,   124,   107,   121,   107,   121,   107,
     121,    10,    11,    12,    14,    10,    14,    10,   172,   159,
      26,    29,    30,    36,    39,    40,    43,   121,   124,   159,
      26,    43,   121,   124,   159,   157,   159,   157,    76,   124,
     157,    74,    75,   122,   127,   146,   121,   121,   107,   107,
     127,   109,   127,   109,   127,   159,   157,   159,   157,    29,
      44,    23,   159,   159,   159,   159,   159,   159,   159,   159,
     159,    53,   150,   127,   122,    14,    62,   127,   123,   124,
     128,    10,    11,    12,    13,    24,    25,    33,    34,    47,
      48,   128,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   162,
     162,   162,   162,   162,   162,   160,   162,   162,   162,   162,
     162,   162,   162,   162,     7,     8,     9,   163,   165,   165,
     165,   166,   166,   167,   168,   169,   169,   170,   170,   170,
     170,   170,    14,    22,    24,    25,    33,    34,    41,    42,
      49,    50,    51,   173,    17,   173,   121,   122,   121,   121,
     121,   121,   121,   121,   107,   122,   123,   126,   110,   128,
     121,   122,    14,   122,   123,   159,    14,   159,   159,   121,
     123,   159,   159,   122,   159,     5,    14,   122,   155,   159,
     107,   121,   107,   121,   107,   121,   122,   146,   107,   107,
     123,   124,   124,   159,   159,   159,   159,   159,   159,   159,
     176,   124,   159,   159,   176,   123,    27,    28,   134,   123,
      10,   176,   166,   166,    10,    14,   123,   156,   131,   159,
     159,   143,    14,   159,    14,   159,   123,    27,   123,   159,
     159,    10,   121,   121,   121,   121,   121,   128,   157,    14,
      22,   127,   151,   123,   156,   122,   122,   153,   123,   176,
     120,   125,   127,   107,   159,   159,   159,   128,   159,    26,
      43,   128,    26,    43,   128,    26,    29,    30,    36,    39,
      40,    43,    44,   128,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
      90,   122,    14,   122,   123,   176,   159,   123,   176,   107,
     107,   107,   123,   176,    14,   123,   154,   176,   126,   110,
     128,   121,   121,   159,   105,   121,   124,    73,    73,   155,
     121,   124,   121,   159,   122,   159,   159,   123,   156,   159,
     159,   107,    10,   176,   121,   121,   121,   121,   121,   121,
     121,   121,   176,   121,   121,   121,   157,   133,   157,   122,
     159,   157,   157,    76,   157,    76,    76,    10,    66,    67,
     146,   123,   124,    24,    25,    34,   128,   159,   121,   121,
      37,    38,   128,   110,   128,   110,   128,   157,   157,   157,
     121,   121,   107,   159,    14,    14,   151,    56,    57,    60,
     128,   146,   123,   123,   156,   156,    70,    71,    72,   128,
     123,    14,   159,   159,   159,    26,    30,    36,    39,    40,
      43,   128,   128,   128,   128,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   123,   176,
     123,   154,   176,   121,   123,   121,   123,   159,   159,   159,
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
     177,   177,   177,   177
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
       4,     4,     5,     5,     1,     1,     1,     1,     2,     3,
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
#line 357 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2706 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 367 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2712 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 368 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2718 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 372 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2724 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 374 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2730 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 376 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2736 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 378 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2744 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 382 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2750 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 384 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2756 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 386 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2762 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 388 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2770 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 392 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2777 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 395 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2785 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 399 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2791 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 401 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2797 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 403 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2803 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 405 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2809 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 407 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2815 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 409 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2823 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 413 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2831 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 417 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2837 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 419 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2843 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 421 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2849 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 423 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2855 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 425 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2861 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 427 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2867 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 429 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2873 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 431 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2879 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 433 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2885 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 435 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2891 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 437 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2897 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 439 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2903 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 441 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2909 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 443 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2915 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 445 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2922 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 448 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2929 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 451 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2935 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 453 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2942 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 456 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2949 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 459 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2955 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 461 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2961 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 463 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2967 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 465 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2973 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 467 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2982 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 472 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2990 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 476 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2998 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 480 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3007 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 485 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3015 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 488 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3021 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 490 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3027 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 492 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3033 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 494 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3039 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 496 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3045 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 498 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3052 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 501 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3058 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 503 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3064 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 505 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3070 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3077 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 510 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3084 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 513 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3091 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 516 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3098 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 519 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3105 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 522 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3111 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3117 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 526 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3123 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 528 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3129 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 530 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3136 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 533 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3142 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 535 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3148 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 537 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3154 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 539 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3161 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 542 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3168 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 545 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3176 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 549 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3183 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 552 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3189 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 554 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3195 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 556 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3201 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 558 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3208 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 561 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3215 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 564 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3222 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 567 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3229 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 570 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3236 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 573 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3243 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 576 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3250 "raku.tab.c"
    break;

  case 86: /* stmt: expr ';'  */
#line 578 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3256 "raku.tab.c"
    break;

  case 87: /* stmt: ';'  */
#line 579 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3262 "raku.tab.c"
    break;

  case 88: /* stmt: if_stmt  */
#line 580 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3268 "raku.tab.c"
    break;

  case 89: /* stmt: while_stmt  */
#line 581 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3274 "raku.tab.c"
    break;

  case 90: /* stmt: for_stmt  */
#line 582 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3280 "raku.tab.c"
    break;

  case 91: /* stmt: given_stmt  */
#line 583 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3286 "raku.tab.c"
    break;

  case 92: /* stmt: KW_TRY block  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3292 "raku.tab.c"
    break;

  case 93: /* stmt: KW_TRY block KW_CATCH block  */
#line 587 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3298 "raku.tab.c"
    break;

  case 94: /* stmt: KW_CATCH block  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3304 "raku.tab.c"
    break;

  case 95: /* stmt: block  */
#line 591 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3310 "raku.tab.c"
    break;

  case 96: /* stmt: unless_stmt  */
#line 592 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3316 "raku.tab.c"
    break;

  case 97: /* stmt: until_stmt  */
#line 593 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3322 "raku.tab.c"
    break;

  case 98: /* stmt: repeat_stmt  */
#line 594 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3328 "raku.tab.c"
    break;

  case 99: /* stmt: loop_stmt  */
#line 595 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3334 "raku.tab.c"
    break;

  case 100: /* stmt: KW_LAST ';'  */
#line 596 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3340 "raku.tab.c"
    break;

  case 101: /* stmt: KW_NEXT ';'  */
#line 597 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3346 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST KW_IF expr ';'  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3352 "raku.tab.c"
    break;

  case 103: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 601 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3358 "raku.tab.c"
    break;

  case 104: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 603 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3364 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3370 "raku.tab.c"
    break;

  case 106: /* stmt: sub_decl  */
#line 606 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3376 "raku.tab.c"
    break;

  case 107: /* stmt: class_decl  */
#line 607 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3382 "raku.tab.c"
    break;

  case 108: /* stmt: role_decl  */
#line 608 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3388 "raku.tab.c"
    break;

  case 109: /* stmt: grammar_decl  */
#line 609 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3394 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF '(' expr ')' block  */
#line 613 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3400 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3406 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3412 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF expr block  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3418 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3424 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 623 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3430 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block elsif_tail  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3436 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3442 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF expr block  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3448 "raku.tab.c"
    break;

  case 119: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 633 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3454 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3460 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 637 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3466 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 639 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3472 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 641 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3478 "raku.tab.c"
    break;

  case 124: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 645 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3484 "raku.tab.c"
    break;

  case 125: /* while_stmt: KW_WHILE expr block  */
#line 647 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3490 "raku.tab.c"
    break;

  case 126: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3496 "raku.tab.c"
    break;

  case 127: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 653 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3502 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS expr block  */
#line 655 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3508 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3514 "raku.tab.c"
    break;

  case 130: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3520 "raku.tab.c"
    break;

  case 131: /* until_stmt: KW_UNTIL expr block  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3526 "raku.tab.c"
    break;

  case 132: /* repeat_stmt: KW_REPEAT block  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3532 "raku.tab.c"
    break;

  case 133: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 669 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3538 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3544 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 675 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3550 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 677 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3556 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 679 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3562 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 682 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3568 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 686 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3578 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 692 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3588 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 698 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3597 "raku.tab.c"
    break;

  case 142: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 703 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3607 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 709 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3616 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR expr block  */
#line 714 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3623 "raku.tab.c"
    break;

  case 145: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 719 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3635 "raku.tab.c"
    break;

  case 146: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 727 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3648 "raku.tab.c"
    break;

  case 147: /* when_list: %empty  */
#line 737 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3654 "raku.tab.c"
    break;

  case 148: /* when_list: when_list KW_WHEN expr block  */
#line 739 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3662 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 745 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3674 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 753 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3684 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_SUB IDENT sub_body  */
#line 759 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3694 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 765 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3706 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 773 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3716 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 779 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3726 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 785 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3739 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 794 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3750 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 801 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3763 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 810 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3776 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 819 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3787 "raku.tab.c"
    break;

  case 160: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 828 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3798 "raku.tab.c"
    break;

  case 161: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 835 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3807 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list '}'  */
#line 841 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3813 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list expr '}'  */
#line 843 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3820 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 846 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3827 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 849 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3833 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 851 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3839 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 853 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3845 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3851 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3857 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 859 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3863 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 861 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3869 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 863 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3876 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 866 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3882 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 868 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3888 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 870 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3894 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 872 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3901 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 875 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3908 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 878 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3916 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list '}'  */
#line 883 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3922 "raku.tab.c"
    break;

  case 180: /* method_body: '{' YADA '}'  */
#line 884 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3928 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list expr '}'  */
#line 886 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3935 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 889 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3942 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 892 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3948 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 894 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3954 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 896 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3960 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 898 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3966 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 900 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3972 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 902 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3978 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 904 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3984 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 906 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3991 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 909 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3997 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 911 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4003 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 913 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4009 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 915 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4016 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 918 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4023 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 921 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4031 "raku.tab.c"
    break;

  case 197: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 927 "raku.y"
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
#line 4049 "raku.tab.c"
    break;

  case 198: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 943 "raku.y"
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
#line 4066 "raku.tab.c"
    break;

  case 199: /* is_clauses: %empty  */
#line 957 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4072 "raku.tab.c"
    break;

  case 200: /* is_clauses: is_clauses IDENT IDENT  */
#line 959 "raku.y"
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
#line 4088 "raku.tab.c"
    break;

  case 201: /* is_clauses: is_clauses TESTOP IDENT  */
#line 971 "raku.y"
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
#line 4104 "raku.tab.c"
    break;

  case 202: /* class_body_list: %empty  */
#line 984 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4110 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 986 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4117 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 989 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4124 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 992 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4131 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 995 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4138 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 998 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4145 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1001 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4152 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1004 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4159 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1007 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4166 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1010 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4173 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1013 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4180 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1016 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4187 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1019 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4194 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1022 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4205 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1029 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4216 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1036 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4227 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1043 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4238 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1050 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4246 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1054 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4254 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1058 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4262 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1062 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4270 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1066 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4281 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1073 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4292 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1080 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4303 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1087 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4314 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1094 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4328 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1104 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4340 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1112 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4352 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1120 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4365 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1129 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4376 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1136 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4387 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1143 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4401 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1153 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4413 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1161 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4425 "raku.tab.c"
    break;

  case 236: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1171 "raku.y"
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
#line 4442 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: %empty  */
#line 1185 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4448 "raku.tab.c"
    break;

  case 238: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1187 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4457 "raku.tab.c"
    break;

  case 239: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1192 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4466 "raku.tab.c"
    break;

  case 240: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1197 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4475 "raku.tab.c"
    break;

  case 241: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1204 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4483 "raku.tab.c"
    break;

  case 242: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1208 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4491 "raku.tab.c"
    break;

  case 243: /* pair_list: IDENT OP_FATARROW expr  */
#line 1214 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4497 "raku.tab.c"
    break;

  case 244: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1216 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4503 "raku.tab.c"
    break;

  case 245: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1218 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4509 "raku.tab.c"
    break;

  case 246: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1220 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4515 "raku.tab.c"
    break;

  case 247: /* param_list: VAR_SCALAR  */
#line 1223 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4521 "raku.tab.c"
    break;

  case 248: /* param_list: IDENT VAR_SCALAR  */
#line 1224 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4527 "raku.tab.c"
    break;

  case 249: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1225 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4533 "raku.tab.c"
    break;

  case 250: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1226 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4539 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' VAR_SCALAR  */
#line 1227 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4545 "raku.tab.c"
    break;

  case 252: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1228 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4551 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1229 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4557 "raku.tab.c"
    break;

  case 254: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1230 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4563 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list '}'  */
#line 1233 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4569 "raku.tab.c"
    break;

  case 256: /* block: '{' YADA '}'  */
#line 1234 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4575 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr '}'  */
#line 1236 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4581 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1238 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4587 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1240 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4593 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1242 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4599 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1244 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4605 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1246 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4612 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1249 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4618 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1251 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4624 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1253 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4630 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1255 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4636 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1257 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4643 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1260 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4650 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1263 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4658 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1267 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4665 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1270 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4672 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1273 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4679 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1276 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4685 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1278 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1282 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4700 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1285 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4707 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1288 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4714 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1291 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4721 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1294 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4728 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1297 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4734 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1299 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4740 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1301 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4746 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST '}'  */
#line 1303 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4752 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1305 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4758 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1307 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4764 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1309 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4770 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1311 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4776 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1313 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4782 "raku.tab.c"
    break;

  case 289: /* closure: '{' expr '}'  */
#line 1316 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4788 "raku.tab.c"
    break;

  case 290: /* expr: VAR_SCALAR '=' expr  */
#line 1319 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4794 "raku.tab.c"
    break;

  case 291: /* expr: KW_GATHER block  */
#line 1320 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4804 "raku.tab.c"
    break;

  case 292: /* expr: tern_expr  */
#line 1325 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4810 "raku.tab.c"
    break;

  case 293: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1329 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4816 "raku.tab.c"
    break;

  case 294: /* tern_expr: cmp_expr  */
#line 1330 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4822 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1333 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4828 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1334 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4834 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1335 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4840 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1336 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4846 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1337 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4852 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1338 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4858 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1339 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4864 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1340 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4870 "raku.tab.c"
    break;

  case 303: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1341 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4876 "raku.tab.c"
    break;

  case 304: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1342 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4882 "raku.tab.c"
    break;

  case 305: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1343 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4888 "raku.tab.c"
    break;

  case 306: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1344 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4894 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1345 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4900 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1346 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4906 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1348 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4916 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1354 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4926 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1360 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4936 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr  */
#line 1365 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 4942 "raku.tab.c"
    break;

  case 313: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1368 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4948 "raku.tab.c"
    break;

  case 314: /* divis_expr: jct_expr  */
#line 1369 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4954 "raku.tab.c"
    break;

  case 315: /* jct_expr: jct_expr '|' range_expr  */
#line 1372 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4960 "raku.tab.c"
    break;

  case 316: /* jct_expr: jct_expr '&' range_expr  */
#line 1373 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4966 "raku.tab.c"
    break;

  case 317: /* jct_expr: dor_expr  */
#line 1374 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4972 "raku.tab.c"
    break;

  case 318: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1378 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4978 "raku.tab.c"
    break;

  case 319: /* dor_expr: range_expr  */
#line 1379 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4984 "raku.tab.c"
    break;

  case 320: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1382 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4990 "raku.tab.c"
    break;

  case 321: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1383 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4996 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr  */
#line 1384 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5002 "raku.tab.c"
    break;

  case 323: /* add_expr: add_expr '~' repl_expr  */
#line 1387 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5008 "raku.tab.c"
    break;

  case 324: /* add_expr: repl_expr  */
#line 1388 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5014 "raku.tab.c"
    break;

  case 325: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1391 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5020 "raku.tab.c"
    break;

  case 326: /* repl_expr: addsub_expr  */
#line 1392 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5026 "raku.tab.c"
    break;

  case 327: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1395 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5032 "raku.tab.c"
    break;

  case 328: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1396 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5038 "raku.tab.c"
    break;

  case 329: /* addsub_expr: mul_expr  */
#line 1397 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5044 "raku.tab.c"
    break;

  case 330: /* mul_expr: mul_expr '*' unary_expr  */
#line 1400 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5050 "raku.tab.c"
    break;

  case 331: /* mul_expr: mul_expr '/' unary_expr  */
#line 1401 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5056 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '%' unary_expr  */
#line 1402 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5062 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1403 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5068 "raku.tab.c"
    break;

  case 334: /* mul_expr: unary_expr  */
#line 1404 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5074 "raku.tab.c"
    break;

  case 335: /* unary_expr: '-' unary_expr  */
#line 1407 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5080 "raku.tab.c"
    break;

  case 336: /* unary_expr: '!' unary_expr  */
#line 1408 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5086 "raku.tab.c"
    break;

  case 337: /* unary_expr: CARET unary_expr  */
#line 1409 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5092 "raku.tab.c"
    break;

  case 338: /* unary_expr: OP_REDUCE unary_expr  */
#line 1411 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5101 "raku.tab.c"
    break;

  case 339: /* unary_expr: pow_expr  */
#line 1415 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5107 "raku.tab.c"
    break;

  case 340: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1418 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5113 "raku.tab.c"
    break;

  case 341: /* pow_expr: postfix_expr  */
#line 1419 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5119 "raku.tab.c"
    break;

  case 342: /* scalar_list: VAR_SCALAR  */
#line 1422 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5125 "raku.tab.c"
    break;

  case 343: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1423 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5131 "raku.tab.c"
    break;

  case 344: /* meth_name: IDENT  */
#line 1426 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5137 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_SORT  */
#line 1427 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5143 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_MAP  */
#line 1428 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5149 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_GREP  */
#line 1429 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5155 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_SAY  */
#line 1430 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5161 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_PRINT  */
#line 1431 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5167 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_TAKE  */
#line 1432 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5173 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_RETURN  */
#line 1433 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5179 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_EXISTS  */
#line 1434 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5185 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_DELETE  */
#line 1435 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5191 "raku.tab.c"
    break;

  case 354: /* meth_name: TESTOP  */
#line 1436 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5197 "raku.tab.c"
    break;

  case 355: /* postfix_expr: call_expr  */
#line 1438 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5203 "raku.tab.c"
    break;

  case 356: /* call_expr: IDENT '(' arg_list ')'  */
#line 1441 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5212 "raku.tab.c"
    break;

  case 357: /* call_expr: IDENT '(' ')'  */
#line 1445 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5218 "raku.tab.c"
    break;

  case 358: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1447 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5227 "raku.tab.c"
    break;

  case 359: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1452 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5233 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1454 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5243 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1460 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5253 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1466 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5261 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' KW_NEW  */
#line 1470 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5269 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' IDENT  */
#line 1474 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5278 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5289 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1486 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5298 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' CARET IDENT  */
#line 1491 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5308 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' CARET IDENT  */
#line 1497 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5318 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1503 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5329 "raku.tab.c"
    break;

  case 370: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5340 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1517 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5349 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' meth_name  */
#line 1522 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5358 "raku.tab.c"
    break;

  case 373: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1527 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5369 "raku.tab.c"
    break;

  case 374: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1534 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5378 "raku.tab.c"
    break;

  case 375: /* call_expr: call_expr '.' meth_name  */
#line 1539 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5387 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_DIE expr  */
#line 1544 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5393 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_MAP closure expr  */
#line 1546 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5399 "raku.tab.c"
    break;

  case 378: /* call_expr: KW_GREP closure expr  */
#line 1548 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5405 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_SORT expr  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5411 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_SORT closure expr  */
#line 1552 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5417 "raku.tab.c"
    break;

  case 381: /* call_expr: atom  */
#line 1553 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5423 "raku.tab.c"
    break;

  case 382: /* arg_list: expr  */
#line 1556 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5429 "raku.tab.c"
    break;

  case 383: /* arg_list: arg_list ',' expr  */
#line 1557 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5435 "raku.tab.c"
    break;

  case 384: /* atom: LIT_INT  */
#line 1560 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5441 "raku.tab.c"
    break;

  case 385: /* atom: LIT_FLOAT  */
#line 1561 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5447 "raku.tab.c"
    break;

  case 386: /* atom: LIT_STR  */
#line 1562 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5453 "raku.tab.c"
    break;

  case 387: /* atom: WORDLIST  */
#line 1564 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5465 "raku.tab.c"
    break;

  case 388: /* atom: LIT_INTERP_STR  */
#line 1571 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5471 "raku.tab.c"
    break;

  case 389: /* atom: VAR_SCALAR  */
#line 1572 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5477 "raku.tab.c"
    break;

  case 390: /* atom: OP_INC VAR_SCALAR  */
#line 1573 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5483 "raku.tab.c"
    break;

  case 391: /* atom: OP_DEC VAR_SCALAR  */
#line 1574 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5489 "raku.tab.c"
    break;

  case 392: /* atom: VAR_SCALAR OP_INC  */
#line 1575 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5495 "raku.tab.c"
    break;

  case 393: /* atom: VAR_SCALAR OP_DEC  */
#line 1576 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5501 "raku.tab.c"
    break;

  case 394: /* atom: VAR_ARRAY  */
#line 1577 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5507 "raku.tab.c"
    break;

  case 395: /* atom: VAR_HASH  */
#line 1578 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5513 "raku.tab.c"
    break;

  case 396: /* atom: VAR_CAPTURE  */
#line 1580 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5521 "raku.tab.c"
    break;

  case 397: /* atom: VAR_FH  */
#line 1584 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5529 "raku.tab.c"
    break;

  case 398: /* atom: VAR_NAMED_CAPTURE  */
#line 1588 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5536 "raku.tab.c"
    break;

  case 399: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1591 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5542 "raku.tab.c"
    break;

  case 400: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1593 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5548 "raku.tab.c"
    break;

  case 401: /* atom: VAR_HASH '{' expr '}'  */
#line 1595 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5554 "raku.tab.c"
    break;

  case 402: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1597 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5560 "raku.tab.c"
    break;

  case 403: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1599 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5566 "raku.tab.c"
    break;

  case 404: /* atom: IDENT  */
#line 1600 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5572 "raku.tab.c"
    break;

  case 405: /* atom: VAR_TWIGIL  */
#line 1602 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5580 "raku.tab.c"
    break;

  case 406: /* atom: VAR_ARRAY_TWIGIL  */
#line 1606 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5588 "raku.tab.c"
    break;

  case 407: /* atom: VAR_HASH_TWIGIL  */
#line 1610 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5596 "raku.tab.c"
    break;

  case 408: /* atom: '(' ')'  */
#line 1613 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5602 "raku.tab.c"
    break;

  case 409: /* atom: '(' expr ')'  */
#line 1614 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5608 "raku.tab.c"
    break;

  case 410: /* atom: '(' expr ',' ')'  */
#line 1616 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5614 "raku.tab.c"
    break;

  case 411: /* atom: '(' expr ',' arg_list ')'  */
#line 1618 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5621 "raku.tab.c"
    break;

  case 412: /* atom: block  */
#line 1620 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5627 "raku.tab.c"
    break;

  case 413: /* atom: KW_SUB block  */
#line 1621 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5633 "raku.tab.c"
    break;


#line 5637 "raku.tab.c"

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

#line 1623 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
