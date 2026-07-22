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

#line 314 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 63,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 64,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 65,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 66,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 67,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 68,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 69,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 70,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 71,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 72,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 73,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 74,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 75,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 76,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 77,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 78,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 79,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 80,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 81,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 82,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 83,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 84,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 85,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 86,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 87,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 88,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 89,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 90,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 91,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 92,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 93,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 94,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 95,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 96,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 97,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 98,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 99,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 100,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 101,                   /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 102,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 103,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 104,                   /* OP_POW  */
  YYSYMBOL_105_ = 105,                     /* '='  */
  YYSYMBOL_106_ = 106,                     /* '!'  */
  YYSYMBOL_107_ = 107,                     /* '<'  */
  YYSYMBOL_108_ = 108,                     /* '>'  */
  YYSYMBOL_109_ = 109,                     /* '|'  */
  YYSYMBOL_110_ = 110,                     /* '&'  */
  YYSYMBOL_111_ = 111,                     /* '~'  */
  YYSYMBOL_112_ = 112,                     /* '+'  */
  YYSYMBOL_113_ = 113,                     /* '-'  */
  YYSYMBOL_114_ = 114,                     /* '*'  */
  YYSYMBOL_115_ = 115,                     /* '/'  */
  YYSYMBOL_116_ = 116,                     /* '%'  */
  YYSYMBOL_UMINUS = 117,                   /* UMINUS  */
  YYSYMBOL_118_ = 118,                     /* '.'  */
  YYSYMBOL_119_ = 119,                     /* ';'  */
  YYSYMBOL_120_ = 120,                     /* ','  */
  YYSYMBOL_121_ = 121,                     /* '('  */
  YYSYMBOL_122_ = 122,                     /* ')'  */
  YYSYMBOL_123_ = 123,                     /* '['  */
  YYSYMBOL_124_ = 124,                     /* ']'  */
  YYSYMBOL_125_ = 125,                     /* '{'  */
  YYSYMBOL_126_ = 126,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 127,                 /* $accept  */
  YYSYMBOL_program = 128,                  /* program  */
  YYSYMBOL_stmt_list = 129,                /* stmt_list  */
  YYSYMBOL_stmt = 130,                     /* stmt  */
  YYSYMBOL_if_stmt = 131,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 132,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 133,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 134,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 135,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 136,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 137,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 138,                /* loop_incr  */
  YYSYMBOL_for_stmt = 139,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 140,               /* given_stmt  */
  YYSYMBOL_when_list = 141,                /* when_list  */
  YYSYMBOL_sub_decl = 142,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 143,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 144,                 /* sub_body  */
  YYSYMBOL_method_body = 145,              /* method_body  */
  YYSYMBOL_class_decl = 146,               /* class_decl  */
  YYSYMBOL_role_decl = 147,                /* role_decl  */
  YYSYMBOL_is_clauses = 148,               /* is_clauses  */
  YYSYMBOL_class_body_list = 149,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 150,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 151,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 152,           /* named_arg_list  */
  YYSYMBOL_pair_list = 153,                /* pair_list  */
  YYSYMBOL_param_list = 154,               /* param_list  */
  YYSYMBOL_block = 155,                    /* block  */
  YYSYMBOL_closure = 156,                  /* closure  */
  YYSYMBOL_expr = 157,                     /* expr  */
  YYSYMBOL_tern_expr = 158,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 159,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 160,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 161,                 /* dor_expr  */
  YYSYMBOL_range_expr = 162,               /* range_expr  */
  YYSYMBOL_add_expr = 163,                 /* add_expr  */
  YYSYMBOL_repl_expr = 164,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 165,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 166,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 167,               /* unary_expr  */
  YYSYMBOL_pow_expr = 168,                 /* pow_expr  */
  YYSYMBOL_meth_name = 169,                /* meth_name  */
  YYSYMBOL_postfix_expr = 170,             /* postfix_expr  */
  YYSYMBOL_call_expr = 171,                /* call_expr  */
  YYSYMBOL_arg_list = 172,                 /* arg_list  */
  YYSYMBOL_atom = 173                      /* atom  */
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
#define YYLAST   3554

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  408
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1039

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   360


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   106,     2,     2,     2,   116,   110,     2,
     121,   122,   114,   112,   120,   113,   118,   115,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   119,
     107,   105,   108,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   123,     2,   124,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   125,   109,   126,   111,     2,     2,     2,
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
     117
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   323,   323,   334,   335,   338,   340,   342,   344,   346,
     348,   352,   355,   359,   361,   363,   365,   367,   369,   373,
     377,   379,   381,   383,   385,   387,   389,   391,   393,   395,
     397,   399,   401,   403,   405,   408,   411,   413,   416,   419,
     421,   423,   425,   427,   432,   436,   440,   445,   449,   450,
     452,   454,   456,   458,   461,   463,   465,   467,   470,   473,
     476,   479,   482,   484,   486,   488,   490,   493,   495,   497,
     499,   502,   505,   509,   512,   514,   516,   518,   521,   524,
     527,   530,   533,   536,   539,   542,   545,   548,   551,   552,
     553,   554,   555,   556,   557,   559,   561,   563,   565,   566,
     567,   568,   569,   570,   571,   573,   575,   577,   579,   580,
     581,   582,   585,   587,   589,   591,   593,   595,   597,   599,
     603,   605,   607,   609,   611,   613,   617,   619,   623,   625,
     627,   629,   633,   635,   639,   643,   645,   647,   651,   652,
     653,   654,   655,   658,   664,   670,   675,   681,   686,   691,
     699,   710,   711,   717,   725,   731,   737,   745,   751,   757,
     766,   773,   782,   791,   800,   807,   814,   815,   818,   821,
     823,   825,   827,   829,   831,   833,   835,   838,   840,   842,
     844,   847,   850,   856,   857,   858,   861,   864,   866,   868,
     870,   872,   874,   876,   878,   881,   883,   885,   887,   890,
     893,   899,   915,   930,   931,   943,   957,   958,   961,   964,
     967,   970,   973,   976,   979,   982,   985,   988,   991,   994,
    1001,  1008,  1015,  1022,  1026,  1030,  1034,  1038,  1045,  1052,
    1059,  1066,  1076,  1084,  1092,  1101,  1108,  1115,  1125,  1133,
    1143,  1158,  1159,  1164,  1169,  1176,  1180,  1186,  1188,  1190,
    1192,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1206,
    1207,  1208,  1210,  1212,  1214,  1216,  1218,  1221,  1223,  1225,
    1227,  1229,  1232,  1235,  1239,  1242,  1245,  1248,  1250,  1254,
    1257,  1260,  1263,  1266,  1269,  1271,  1273,  1275,  1277,  1279,
    1281,  1283,  1285,  1289,  1292,  1293,  1298,  1301,  1303,  1306,
    1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,
    1317,  1318,  1319,  1320,  1326,  1332,  1338,  1341,  1342,  1343,
    1346,  1348,  1351,  1352,  1353,  1356,  1357,  1360,  1361,  1364,
    1365,  1366,  1369,  1370,  1371,  1372,  1373,  1376,  1377,  1378,
    1379,  1382,  1383,  1386,  1387,  1388,  1389,  1390,  1391,  1392,
    1393,  1394,  1395,  1396,  1398,  1400,  1405,  1406,  1411,  1413,
    1419,  1425,  1429,  1433,  1438,  1445,  1450,  1456,  1462,  1469,
    1476,  1481,  1486,  1493,  1498,  1503,  1505,  1507,  1509,  1511,
    1513,  1516,  1517,  1520,  1521,  1522,  1523,  1531,  1532,  1533,
    1534,  1535,  1539,  1543,  1546,  1548,  1550,  1552,  1554,  1556,
    1557,  1561,  1565,  1569,  1570,  1571,  1573,  1576,  1577
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
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT",
  "OP_SLE", "OP_SGT", "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt",
  "loop_incr", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "scalar_methcall", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "jct_expr", "dor_expr", "range_expr",
  "add_expr", "repl_expr", "addsub_expr", "mul_expr", "unary_expr",
  "pow_expr", "meth_name", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-747)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-408)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -747,    69,  1383,  -747,  -747,  -747,  -747,  -747,   878,  -108,
     -40,    61,   145,  -747,  -747,  3429,  -747,  -747,  -747,    89,
    1515,   597,  2637,  2703,  2769,  2835,  2901,    -3,    79,  2901,
    1581,    95,  2901,   226,   284,  2967,  3033,    79,    84,    17,
      92,   130,   130,  3099,    79,    79,  2901,   295,   300,   210,
    -747,   303,   314,   342,  3429,  3429,  -747,  1647,   288,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,   397,
    -747,  -747,  -747,  3418,   744,  -747,   256,   592,   289,  -747,
     -27,   277,   390,   211,  -747,  -747,   294,   311,   316,   399,
     299,   313,  2901,  2901,  2901,  2901,  2901,  2901,   429,  1713,
    2901,   445,  2901,  2901,   200,  1779,   341,   352,   -36,  -747,
      79,  -747,  -747,   368,   127,  -747,  1845,  -747,   432,   -46,
     194,   201,   643,   479,   243,  1647,   538,  1647,    56,  1647,
      79,  1647,    79,   124,     4,   259,  -747,  -747,   418,  -747,
     428,   450,   460,   455,   -28,   108,  1647,    79,  1647,    79,
    -747,  1911,  -747,  2901,  2901,  -747,  2901,  2901,  -747,  2901,
    2901,  2901,  1977,  2901,  -747,   530,  -747,  -747,  -747,   493,
     471,    44,   516,   532,   537,  -747,  -747,  -747,   -39,   557,
    1010,  2901,  2901,  2901,  2901,  2901,  2901,  2901,  2901,  -747,
    2901,  2901,  2901,  2901,  2901,  2901,  2901,  2901,  -747,  3429,
    3429,  3429,  3429,  3429,  3429,  3429,  3429,  3429,  3429,  3429,
    3429,  3429,  3429,  3429,   506,  3429,  3429,  3429,  3429,  3429,
    3429,  3429,  3429,  3429,  3429,  3429,  3429,  3429,  3429,   917,
     858,   109,  -747,  -747,   575,   577,   589,   623,   632,   652,
     164,  -747,   492,   657,   668,   659,   705,   673,   819,   683,
    -747,   630,  2901,   831,  2901,  -747,  2901,   716,   -39,   675,
    -747,  2901,  2901,  -747,  3165,  -747,  3231,  -747,   232,   257,
     272,   278,   749,   755,   685,  2901,  2901,  2901,  2901,  2901,
    2901,  2901,  -747,  2901,   688,  2901,  2901,  -747,  2901,   691,
     533,   692,  -747,   854,  2901,  -747,  3429,  3429,     2,  -747,
    -747,  -747,  -747,  2901,  2901,  -747,   853,  2901,   857,  2901,
     695,   849,   696,  -747,   874,   770,   818,   826,   829,   838,
     814,  -747,  -747,  -747,    79,    -8,  -747,    15,   843,   844,
    -747,  -747,  -747,  2043,  -747,  -747,   988,   847,   140,   873,
    2637,  2703,  2901,   355,   197,   203,  -747,   381,   865,   866,
     867,   868,   879,   881,   882,   883,   884,   885,   886,   887,
     891,   898,   899,   900,   461,   461,   461,   461,   461,   461,
     908,   461,   461,   461,   461,   461,   461,   461,   461,  -747,
    -747,  -747,  -747,  -747,  -747,   926,   926,   277,   390,   211,
     211,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,   927,  1024,   928,
    -747,  2109,  -747,  -747,  -747,  -747,  -747,  -747,  2901,  2175,
    -747,   942,   945,   961,  -747,  2241,  -747,  2307,  -747,   943,
     960,   946,  -747,  -747,   952,  -747,   954,  1647,   148,  1003,
     -55,  2373,   548,   957,  2901,  -747,  3297,  -747,  2901,  -747,
      21,  -747,  2901,  2901,  2043,   958,   969,   970,   971,   972,
     973,   975,   602,  2043,   976,   977,   639,    79,   -13,  3363,
    -747,    79,    79,   170,    63,   123,  -747,    22,   955,   703,
    1135,   978,   979,    14,   991,   974,   993,   982,    79,    79,
      79,   997,  2901,  -747,  -747,  -747,  -747,  -747,  -747,  1091,
    1096,  -747,     8,   955,   708,    26,   279,     5,  -747,   709,
    1098,  2901,  2901,  2901,   443,    13,  -100,  -747,   168,  2901,
    2901,  -747,  2901,  2901,  -747,  2901,  2901,  2901,  2901,  2901,
    2901,  2901,  2901,  -747,  -747,  -747,  -747,  -747,  -747,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,
    3429,  2439,  -747,  2505,   994,   724,   995,  -747,   735,  2901,
    2901,  2901,  -747,   746,   -11,  -747,   765,   768,  -747,  -747,
    -747,  -747,  -747,   773,  2901,  -747,  2901,  2901,  2901,   774,
    -747,   142,  -747,   996,  1647,   681,   998,   955,   781,   999,
    1000,   782,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,
     790,  -747,  -747,  -747,   684,  -747,  -747,  1647,    79,  -747,
    -747,  1110,  -747,  1111,  1112,  -747,  1114,  1115,  -747,   305,
     955,  2637,  2703,   584,  -747,   505,  -747,  -747,  2901,    79,
    -747,  -747,  -747,  1007,  1009,  1100,  -747,  -747,  2901,  1011,
    -747,  -747,   113,   828,   776,  1076,  -747,  -747,   955,   955,
     793,   794,  1119,  1120,  1123,  -747,  -747,   167,  1018,  1017,
     192,  2901,  2901,  2901,  2901,  2901,  2901,  -747,  -747,  -747,
    -747,   227,   230,   231,   262,   266,   271,   274,   275,   276,
     312,   326,   328,  -747,  -747,   797,  -747,   806,   807,  -747,
    1043,  -747,  -747,  1044,  1053,  1054,  -747,  2901,  1160,  -747,
    -747,  2043,  1056,   721,  -747,  -747,  1072,  1121,  1124,  -747,
     812,  -747,  2901,  -747,  -747,   955,  -747,  -747,  1073,  1078,
     -13,  -747,   813,   821,    79,    79,    79,  -747,  -747,  -747,
     106,  -747,   465,    87,  -747,   329,  2901,  2901,  2901,  2901,
    2901,  2901,  2901,  2901,  -747,    79,  1074,  -747,  -747,    79,
    1079,  1449,  -747,   287,   363,    23,    48,   783,  1082,  1083,
     917,  -747,  -747,   955,   955,  1197,  1198,  1206,  2901,  2571,
    1117,  1118,  -747,   334,   337,   339,   345,   351,   366,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  1152,
     816,  -747,  -747,  -747,  2901,  2901,  2043,   778,  -747,  -747,
    -747,  -747,  -747,    79,    79,  -747,  -747,  -747,  -747,  -747,
    1215,  1216,  2901,  2901,  2901,  -747,  -747,  -747,   370,   380,
     393,   414,   424,   427,   447,   467,  -747,  -747,  -747,  2901,
     177,  1217,  1220,  1109,  -747,    28,  1165,  -747,    40,  -747,
    1219,  1221,  1125,  2901,  -747,  1222,  1223,  1126,  2901,  -747,
      58,    82,  1127,  1128,  -747,  -747,   417,  -747,  -747,  -747,
    -747,  -747,   487,  1108,   824,  2901,  2901,  -747,  -747,  -747,
    -747,  -747,  -747,  2901,  1130,  -747,  -747,   840,  -747,   893,
    -747,  -747,  -747,   491,   503,   504,  -747,  -747,  -747,  -747,
    -747,  -747,  -747,  -747,  1131,  -747,  -747,  -747,  -747,    79,
    1113,   841,  1116,  1259,  1113,   859,  1132,  1133,  -747,  1134,
    1136,  1138,  -747,  1139,  1225,  1226,  1140,  2901,  -747,  1229,
    1252,  1148,  2901,  -747,  -747,  -747,    43,  -747,  -747,  -747,
    1142,   518,   520,  -747,  -747,  1167,    79,  -747,  -747,  -747,
    -747,  1449,  -747,  -747,  1113,  -747,  2637,  2703,   713,  -747,
     717,  -747,  1113,  -747,  -747,  -747,  -747,  -747,  -747,  1168,
    1177,  -747,  1178,  1179,  1180,  -747,  1196,  1113,   860,  -747,
    -747,  -747,  -747,  -747,  1194,  -747,   739,   184,  -747,   521,
    2901,  2901,  2901,  2901,  2901,  2901,  2901,  2901,  -747,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  1113,    79,  2901,
    2901,  2901,  -747,  -747,  -747,   523,   524,   526,   563,   566,
     572,   574,   580,  -747,  -747,   588,   594,   629,  -747,  -747,
    -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   383,   384,   385,   387,   388,   389,
     390,   400,   399,   401,   402,     0,   391,   392,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     386,     0,     0,     0,     0,     0,    89,     0,     3,     4,
      90,    91,    98,    99,   100,   101,    92,    93,   108,     0,
     109,   110,   111,    97,     0,   296,   298,   316,   319,   321,
     324,   326,   328,   331,   336,   340,   342,   354,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   388,   389,   390,   400,
       0,   407,   339,     0,   388,    29,     0,   381,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,   408,   295,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,     0,   135,     0,     0,   102,     0,     0,   103,     0,
       0,     0,     3,     0,   378,    94,    96,   375,   203,     0,
       0,     0,     0,     0,     0,   338,   337,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,    85,     0,     0,     0,     0,     0,   294,
       0,   358,     0,     0,     0,     0,     0,   363,     0,   362,
     356,     0,     0,     0,     0,    24,     0,   403,     0,     0,
      32,     0,     0,     6,     0,     7,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,    36,     0,     0,
     115,     0,   127,     0,     0,   148,     0,     0,     0,     3,
     155,    39,    40,     0,     0,   151,     0,     0,     0,     0,
       0,   130,     0,   133,     0,     0,     0,     0,     0,     0,
       0,   376,   377,   379,     0,     0,   206,     0,     0,     0,
     241,    86,    87,     0,   404,   260,   388,   389,   390,   400,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,   302,   305,   306,   299,   300,
       0,   303,   304,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   317,   318,   320,   322,   323,   325,   327,   329,
     330,   335,   332,   333,   334,   341,   343,   353,   347,   348,
     349,   350,   351,   352,   345,   346,   344,   374,     0,   371,
      45,     0,    79,    80,    81,    82,    83,    42,     0,     0,
     357,   394,   395,   396,    47,     0,   366,     0,   355,     0,
       0,     0,   294,    31,     0,   382,     0,     0,     0,   385,
     399,     0,     0,     0,     0,    21,     0,    22,     0,    23,
       0,   158,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
     118,   404,     0,     0,   322,   323,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
     404,     0,     0,   104,   105,   106,   107,   293,    95,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,   286,     0,     0,
       0,   287,     0,     0,   288,     0,     0,     0,     0,     0,
       0,     0,     0,   261,    49,    51,    53,    56,    54,    55,
      50,    52,    62,    64,    66,    69,    67,    68,    63,    65,
       0,     0,   367,     0,     0,     0,     0,   165,     0,     0,
       0,     0,   365,     0,   399,   361,     0,     0,   394,   395,
     396,    30,     5,     0,     0,     9,     0,     0,     0,     0,
      14,     0,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    70,    73,    72,    76,    74,    75,    71,    34,
       0,    77,    78,    37,   112,   117,   116,     0,     0,   126,
     145,     0,   147,     0,     0,   252,     0,     0,   154,     0,
       0,     0,     0,     0,   166,     0,    26,    25,     0,     0,
     149,   397,   398,     0,     0,   128,   131,   132,     0,     0,
     204,   205,     0,     0,     0,     0,   202,   163,     0,     0,
       0,     0,     0,     0,     0,   240,   406,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   277,   284,
     285,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   297,   373,     0,   370,     0,     0,    44,
       0,    46,   164,     0,     0,     0,   364,     0,     0,   359,
     360,     0,     0,     0,   248,   247,     0,     0,     0,    16,
       0,    17,     0,    20,   157,     0,    28,    27,   406,   406,
       0,   119,     0,   120,     0,     0,     0,   253,   254,   255,
       0,   153,     0,     0,   169,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,    60,    61,     0,
       0,     0,   201,     0,     0,     0,     0,     0,     0,     0,
       0,   162,   160,     0,     0,     0,     0,     0,     0,     0,
     394,   396,   281,     0,     0,     0,     0,     0,     0,   289,
     290,   291,   292,   262,   264,   266,   269,   267,   268,   263,
     265,   372,   369,   368,    43,    57,    58,    59,   245,     0,
       0,    11,    10,    15,     0,     0,     0,     0,   156,    35,
      38,   114,   113,   404,     0,   124,   146,   143,   144,   256,
       0,     0,     0,     0,     0,   170,   171,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   152,   150,   129,     0,
     388,     0,     0,     0,   138,     0,     3,   236,     0,   233,
       0,     0,     0,     0,   212,     0,     0,     0,     0,   207,
       0,     0,     0,     0,   208,   209,     0,   159,   161,   242,
     243,   244,     0,   165,     0,     0,     0,   271,   273,   276,
     274,   275,   272,     0,   406,   250,   249,     0,    18,   121,
     122,   257,   258,     0,     0,     0,   172,   174,   176,   179,
     177,   178,   173,   175,     0,   139,   140,   141,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,   224,     0,     0,     0,     0,     0,   214,     0,
       0,     0,     0,   213,   210,   211,     0,   239,   280,   279,
     164,     0,     0,   246,    12,   406,     0,   125,   180,   182,
     181,     0,   137,   235,     0,   184,     0,     0,     0,   183,
       0,   232,     0,   221,   222,   216,   219,   220,   215,     0,
       0,   225,     0,     0,     0,   226,     0,     0,     0,   278,
     282,   283,    19,   123,     0,   234,     0,     0,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,   231,
     229,   230,   218,   227,   228,   217,   238,     0,     0,     0,
       0,     0,   188,   189,   186,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   136,     0,     0,     0,   190,   192,
     194,   197,   195,   196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -747,  -747,     1,  -747,  -464,  -597,  -747,  -747,  -747,  -747,
    -747,   369,  -747,  -747,  -747,  -747,  -747,  -266,  -746,  -747,
    -747,  -747,   820,  -747,  -747,   764,   888,  -309,   198,   863,
      -2,  -202,  -747,  1008,  -747,   464,   -16,  1102,  1103,   730,
      -6,  -747,  -228,  -747,  -747,     3,  -747
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   180,    59,    60,   470,    61,    62,    63,    64,
      65,   843,    66,    67,   483,    68,    69,   300,   847,    70,
      71,   325,   502,    72,   507,   566,   442,   479,   111,   160,
     117,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   407,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   409,   370,   605,   451,   499,   721,   849,   112,
     134,   135,   476,    24,   500,   100,   477,   578,   504,   301,
     126,   128,   130,   132,   133,   476,   669,   138,   140,   477,
     143,   476,   615,   147,   149,   477,   476,   850,   476,   285,
     477,   164,   477,   153,   167,   851,   218,   219,   175,   176,
     476,   628,   629,   476,   477,   178,   286,   477,   328,   262,
     154,   697,   855,   104,   643,   644,   105,   101,   645,     3,
     856,   253,   924,   263,   652,   653,   654,   296,   297,   306,
     925,   333,   285,   334,   220,   102,   852,   616,   617,   254,
     234,   235,   236,   237,   238,   239,   929,   307,   243,   286,
     245,   246,   242,   113,   930,   141,   329,   104,   251,   142,
     105,   857,    58,   285,   258,   220,   819,   501,   156,   259,
     937,   926,    58,   274,   478,   284,   815,   289,   853,   291,
     286,   655,   287,   288,   646,   157,   155,   503,   613,   668,
     630,   588,   854,   587,   310,   931,   312,   707,   649,   315,
     910,   316,   317,   858,   318,   319,   708,   320,   321,   322,
     320,   323,   914,   927,   953,   977,   103,   859,   961,   643,
     644,   820,   821,   645,   220,   287,   288,   928,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   932,   356,   357,
     358,   359,   360,   361,   362,   363,   650,   651,   614,   293,
      73,   933,   385,   386,    58,   151,   287,   288,   985,    58,
     285,   158,   618,   826,   247,   308,   999,   248,   391,   392,
     393,   394,   395,   519,   170,   136,   137,   286,   410,   522,
     411,  1006,   256,   309,   220,   150,   152,   647,   144,   752,
     520,   171,   165,   166,   294,   611,   523,   101,    99,    58,
     429,   574,   431,   272,   432,   159,   811,   273,   249,   435,
     436,  1023,   438,   104,   443,   512,   105,   575,   576,   418,
     905,   906,   768,   455,   456,   457,   458,   459,   460,   461,
     474,   475,   256,   464,   465,   419,   462,   302,   769,   476,
     261,   466,   947,   477,   670,    58,   145,   473,    99,   264,
     480,   481,   482,   287,   288,   485,   266,   487,   136,   168,
    1013,   424,   224,   265,   169,   729,   155,   172,   772,   730,
     267,   714,   158,   521,   173,   225,   226,   227,   290,   524,
     292,   295,   199,   200,   201,   202,   509,   444,   514,   515,
     516,   518,   203,   204,   205,   311,   493,   313,   683,   494,
     495,   445,   174,   779,   731,   179,   780,   781,     4,     5,
       6,     7,   446,   206,   207,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,   447,   448,    73,   221,
     298,   496,   761,   762,   299,   542,   110,    28,   782,   217,
     543,   449,   783,   544,   545,   546,    33,   784,   228,   450,
     785,   786,   787,   299,    41,    42,    43,   525,   845,    46,
     526,   527,   846,   231,   555,   754,   556,   528,   232,    50,
     529,   530,   558,   181,   531,   532,   182,   183,   563,   229,
     567,   547,   233,   184,   230,   573,   185,   186,   788,   178,
     187,   188,   583,   240,   585,   548,   586,   549,   302,   808,
     589,   590,   789,   592,   790,   827,   594,   591,   595,   244,
     877,    54,    99,   878,   596,   879,   600,   608,    55,   661,
     597,   880,   276,   662,   139,   252,    57,   881,   625,   663,
      58,   517,   664,   665,   848,   598,   666,   255,   846,   542,
     639,   822,   882,   271,   276,   823,   896,   867,   868,   543,
     198,   278,   222,   223,   279,   280,   897,   533,   824,   658,
     659,   660,   544,   379,   380,   381,   189,   671,   672,   898,
     673,   674,   498,   675,   676,   677,   678,   679,   680,   681,
     682,   736,   866,   545,   737,   738,   911,   301,   936,   915,
     899,   739,   846,   546,   740,   741,   547,   302,   742,   743,
     900,   260,   261,   901,   685,   303,   688,   693,   694,   695,
     468,   469,   282,   283,   275,   304,   548,   276,   277,   667,
     215,   216,   702,   902,   278,   704,   705,   279,   280,   703,
     305,   281,   710,   324,   282,   283,   549,     4,     5,     6,
       7,   825,   327,   903,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,   722,   691,   119,   120,   121,
     592,   122,   261,   938,   420,   110,    28,   948,   326,   732,
     733,   735,   594,   598,   198,    33,   745,   978,   123,   949,
     950,   744,   124,    41,    42,    43,   750,   795,    46,   797,
     302,   330,   542,   543,   980,   544,   981,  1014,    50,  1028,
    1029,   331,  1030,   268,   269,   270,   332,   282,   283,   773,
     774,   775,   776,   777,   778,   604,   606,   580,   581,   609,
     610,   612,   208,   209,   210,   211,   212,   213,    73,   382,
     383,   384,   545,   335,   214,   546,   635,   636,   637,  1031,
      54,   547,  1032,   548,   412,   798,   413,    55,  1033,   549,
    1034,   215,   216,   139,   800,    57,  1035,   592,   414,    58,
     734,   720,   469,   594,  1036,   807,     4,     5,     6,     7,
    1037,   599,   261,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,   828,   829,   830,   831,   832,   833,
     834,   835,   415,   990,   110,    28,   991,   992,   598,   844,
     261,   416,   428,   993,    33,  1038,   994,   995,   603,   261,
     996,   997,    41,    42,    43,  1009,   872,    46,   276,  1010,
     190,   417,   874,   191,   192,   278,   422,    50,   279,   280,
     193,   421,  1011,   194,   195,   423,   755,   196,   197,   756,
     757,   758,   759,   860,   425,   261,   861,   434,   862,   863,
     711,   712,   885,   886,   427,   454,   723,   334,   463,   887,
     334,   333,   333,   467,   471,   333,   333,   488,   490,    54,
     893,   894,   895,   619,   424,   620,    55,   746,   619,   261,
     648,   656,   139,   426,    57,   433,   198,   904,    58,   988,
     802,   261,   396,   998,   261,   430,   690,   913,   814,   469,
     397,   919,   398,   399,   452,   261,   923,   692,   282,   283,
     453,   400,   401,   198,   472,  1012,   261,   484,   696,   402,
     403,   486,   396,   941,   942,   408,   489,   404,   405,   406,
     397,   943,   398,   399,   491,   698,   753,   699,   261,   492,
     700,   400,   401,   701,   581,   334,   706,   888,   261,   402,
     403,   619,   261,   715,   718,   161,   163,   404,   405,   406,
     261,   960,   719,   619,   619,   763,   764,   261,   812,   791,
     946,   469,   816,   817,   818,   972,   698,   261,   792,   793,
     976,   396,   806,   333,   334,   813,   261,   493,   884,   397,
     497,   398,   399,   836,   261,   494,   940,   838,   495,   844,
     400,   401,   389,   390,   986,   987,   989,   496,   402,   403,
     261,   619,   945,   954,   505,   506,   404,   405,   406,    89,
     511,    90,    91,    92,    93,    94,    95,    96,   513,   619,
     619,   962,  1007,    97,   534,   535,   536,   537,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,    98,   550,   538,    99,
     539,   540,   541,   542,   543,   544,   545,  1025,  1026,  1027,
     546,   889,   890,     4,     5,     6,     7,   547,   548,   549,
     336,   337,   338,   339,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   340,   341,    24,   220,   552,    25,
      26,    27,    28,   342,   343,    31,    32,   559,   551,   553,
     560,    33,    34,    35,    36,    37,    38,   344,   345,    41,
      42,    43,    44,    45,    46,    47,   561,   568,   569,    48,
      49,   571,   570,   572,    50,   577,   582,   592,    51,    89,
     299,    90,    91,    92,    93,    94,    95,    96,   593,   594,
     595,   596,   597,    97,   598,   601,   602,   626,   627,   631,
     632,   633,   638,    52,    53,   640,   510,   952,   634,    99,
     641,    73,   657,   689,   691,   709,    54,   713,   716,   717,
     724,   725,   726,    55,   727,   728,   747,   749,   748,    56,
     751,    57,   760,   765,   766,    58,   346,   767,     4,     5,
       6,     7,   770,   771,   983,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   621,
     622,    24,   794,   795,    25,    26,    27,    28,    29,   623,
      31,    32,   796,   797,   799,   801,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,   803,   809,   804,    48,    49,   805,   810,   839,    50,
     837,   864,   865,    51,   869,   870,  1024,   364,   365,   366,
     367,   368,   369,   871,   371,   372,   373,   374,   375,   376,
     377,   378,   875,   876,   883,   891,   892,   907,    52,    53,
     908,   909,   912,   916,   939,   917,   920,   921,   846,   969,
     970,    54,   955,   973,   918,   922,   934,   935,    55,   944,
     951,   963,   964,   965,    56,   966,    57,   967,   968,   971,
      58,   624,     4,     5,     6,     7,   974,   975,   979,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   956,   957,    24,   982,  1000,    25,    26,
      27,    28,    29,   958,    31,    32,  1001,  1002,  1003,  1004,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,  1005,  1008,   687,    48,    49,
     984,   642,   387,    50,   388,     0,     0,    51,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,    56,     0,
      57,     0,     0,     0,    58,   959,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,     0,    25,    26,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,    48,    49,     0,     0,     0,    50,     0,     0,
       0,    51,     4,     5,     6,     7,     0,     0,     0,   840,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,    52,    53,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,    56,    46,    57,     0,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,   841,   842,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,     0,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,   115,    46,   116,     0,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
     139,    46,    57,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,    57,   177,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   241,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   250,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,   314,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   257,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,    57,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,   179,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,    57,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   508,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   554,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   557,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,   564,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,    57,   562,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,   439,     7,
       0,     0,     0,   114,   107,   108,   109,   440,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,    57,   565,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   177,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,   564,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   684,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   686,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,    57,   873,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,   125,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,   127,     0,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
     129,     0,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,   131,     0,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,    57,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,   146,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,   148,     0,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,     0,     0,     0,   162,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,   439,     7,     0,     0,
       0,   114,   107,   108,   109,   440,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,   437,     0,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,   441,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,   584,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   106,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,  -407,  -407,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,   607,     0,     0,     0,    58,     0,
       0,  -407,  -407,    50,  -407,  -407,  -407,  -407,  -407,  -407,
    -407,  -407,  -407,  -407,  -407,  -407,  -407,     0,     0,     0,
    -407,     0,     0,     0,     0,     0,     0,     0,  -407,  -407,
    -407,     0,  -407,     0,     0,  -407,  -407,  -407,  -407,  -407,
    -407,     0,  -407,  -407,  -407,    54,  -407,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   230,   205,   468,   271,    14,   604,   754,    15,
      26,    14,    10,    26,    22,   123,    14,    72,   327,   119,
      22,    23,    24,    25,    26,    10,   126,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    10,    14,    10,    26,
      14,    43,    14,    26,    46,    22,    73,    74,    54,    55,
      10,    37,    38,    10,    14,    57,    43,    14,    14,   105,
      43,    72,    14,   118,    56,    57,   121,   107,    60,     0,
      22,   107,    14,   119,    69,    70,    71,    73,    74,   107,
      22,   120,    26,   122,   111,   125,    63,    65,    66,   125,
      92,    93,    94,    95,    96,    97,    14,   125,   100,    43,
     102,   103,    99,    14,    22,    10,    62,   118,   105,    14,
     121,    63,   125,    26,   116,   111,    10,   125,    26,   116,
     866,    63,   125,   125,   122,   127,   723,   129,   105,   131,
      43,   126,   119,   120,   126,    43,   119,   122,    75,   126,
     126,   450,   119,   122,   146,    63,   148,     5,   122,   151,
     122,   153,   154,   105,   156,   157,    14,   159,   160,   161,
     162,   163,   122,   105,   910,   122,   105,   119,   914,    56,
      57,    65,    66,    60,   111,   119,   120,   119,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   105,   190,   191,
     192,   193,   194,   195,   196,   197,   505,   506,    75,    75,
       2,   119,   218,   219,   125,   121,   119,   120,   954,   125,
      26,   119,   478,   126,    14,   107,   962,    17,   224,   225,
     226,   227,   228,    26,    14,    27,    28,    43,   119,    26,
     121,   977,   105,   125,   111,    37,    38,   503,    12,   126,
      43,    31,    44,    45,   120,    75,    43,   107,   121,   125,
     252,   103,   254,    10,   256,   125,   720,    14,    58,   261,
     262,  1007,   264,   118,   266,   125,   121,   119,   120,   105,
      93,    94,   105,   275,   276,   277,   278,   279,   280,   281,
     296,   297,   105,   285,   286,   121,   283,   119,   121,    10,
     120,   288,   889,    14,   126,   125,    12,   294,   121,   105,
     299,   303,   304,   119,   120,   307,   105,   309,   110,    14,
     126,   119,   101,   119,    14,    10,   119,    14,   126,    14,
     119,   587,   119,   126,    10,   114,   115,   116,   130,   126,
     132,   133,    76,    77,    78,    79,   333,   105,   340,   341,
     342,   343,    86,    87,    88,   147,   119,   149,   550,   119,
     119,   119,    10,   126,   620,    67,   126,   126,     3,     4,
       5,     6,   105,   107,   108,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   119,   105,   180,   102,
     121,   119,   648,   649,   125,   119,    31,    32,   126,   100,
     119,   119,   126,   119,   119,   119,    41,   126,   104,   121,
     126,   126,   126,   125,    49,    50,    51,    26,   121,    54,
      29,    30,   125,    14,   411,   643,   418,    36,   119,    64,
      39,    40,   419,    26,    43,    44,    29,    30,   425,   118,
     427,   119,   119,    36,   118,   437,    39,    40,   126,   441,
      43,    44,   444,    14,   446,   119,   448,   119,   119,   715,
     452,   453,   126,   119,   126,   126,   119,   454,   119,    14,
     126,   106,   121,   126,   119,   126,   463,   469,   113,    26,
     119,   126,    29,    30,   119,   123,   121,   126,   480,    36,
     125,   126,    39,    40,   121,   119,    43,   119,   125,   119,
     492,    26,   126,    14,    29,    30,   126,   763,   764,   119,
     119,    36,   112,   113,    39,    40,   126,   126,    43,   511,
     512,   513,   119,     7,     8,     9,   119,   519,   520,   126,
     522,   523,   324,   525,   526,   527,   528,   529,   530,   531,
     532,    26,   760,   119,    29,    30,   845,   119,   121,   848,
     126,    36,   125,   119,    39,    40,   119,   119,    43,    44,
     126,   119,   120,   126,   551,   105,   553,   559,   560,   561,
      27,    28,   119,   120,    26,   105,   119,    29,    30,   126,
     109,   110,   574,   126,    36,   577,   578,    39,    40,   576,
     125,    43,   584,    53,   119,   120,   119,     3,     4,     5,
       6,   126,   121,   126,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   607,   119,    10,    11,    12,
     119,    14,   120,   126,   122,    31,    32,   126,   125,   621,
     622,   623,   119,   119,   119,    41,   628,   936,    31,   126,
     126,   126,    35,    49,    50,    51,   638,   119,    54,   119,
     119,   125,   119,   119,   126,   119,   126,   126,    64,   126,
     126,   119,   126,    10,    11,    12,   119,   119,   120,   661,
     662,   663,   664,   665,   666,   467,   468,   119,   120,   471,
     472,   473,    80,    81,    82,    83,    84,    85,   480,   215,
     216,   217,   119,   126,    92,   119,   488,   489,   490,   126,
     106,   119,   126,   119,   119,   697,   119,   113,   126,   119,
     126,   109,   110,   119,   701,   121,   126,   119,   119,   125,
     126,    27,    28,   119,   126,   712,     3,     4,     5,     6,
     126,   119,   120,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   736,   737,   738,   739,   740,   741,
     742,   743,   119,    26,    31,    32,    29,    30,   119,   751,
     120,   119,   122,    36,    41,   126,    39,    40,   119,   120,
      43,    44,    49,    50,    51,    26,   768,    54,    29,    30,
      26,   119,   769,    29,    30,    36,   108,    64,    39,    40,
      36,   124,    43,    39,    40,   126,    10,    43,    44,    13,
      14,    15,    16,    10,   121,   120,    13,   122,    15,    16,
     119,   120,   804,   805,   121,   120,   608,   122,   120,   806,
     122,   120,   120,   122,   122,   120,   120,   122,   122,   106,
     822,   823,   824,   120,   119,   122,   113,   629,   120,   120,
     122,   122,   119,    14,   121,   119,   119,   839,   125,   126,
     119,   120,    14,   126,   120,    14,   122,   846,    27,    28,
      22,   853,    24,    25,   105,   120,   858,   122,   119,   120,
     105,    33,    34,   119,    10,   126,   120,    14,   122,    41,
      42,    14,    14,   875,   876,    17,    27,    49,    50,    51,
      22,   883,    24,    25,    10,   120,    58,   122,   120,   119,
     122,    33,    34,   120,   120,   122,   122,   119,   120,    41,
      42,   120,   120,   122,   122,    42,    43,    49,    50,    51,
     120,   913,   122,   120,   120,   122,   122,   120,   720,   122,
      27,    28,   724,   725,   726,   927,   120,   120,   122,   122,
     932,    14,   120,   120,   122,   122,   120,   119,   122,    22,
     126,    24,    25,   745,   120,   119,   122,   749,   119,   951,
      33,    34,   222,   223,   956,   957,   958,   119,    41,    42,
     120,   120,   122,   122,   121,   121,    49,    50,    51,    91,
     123,    93,    94,    95,    96,    97,    98,    99,   105,   120,
     120,   122,   122,   105,   119,   119,   119,   119,   990,   991,
     992,   993,   994,   995,   996,   997,   118,    89,   119,   121,
     119,   119,   119,   119,   119,   119,   119,  1009,  1010,  1011,
     119,   813,   814,     3,     4,     5,     6,   119,   119,   119,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   111,    14,    29,
      30,    31,    32,    33,    34,    35,    36,   105,   121,   121,
     105,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,   105,   124,   108,    59,
      60,   119,   126,   119,    64,    72,   119,   119,    68,    91,
     125,    93,    94,    95,    96,    97,    98,    99,   119,   119,
     119,   119,   119,   105,   119,   119,   119,   119,   119,   108,
     126,   108,   105,    93,    94,    14,   118,   909,   126,   121,
      14,   913,    14,   119,   119,   119,   106,   119,   119,   119,
      10,    10,    10,   113,    10,    10,   119,    27,   119,   119,
     119,   121,    56,    14,    14,   125,   126,    14,     3,     4,
       5,     6,   124,   126,   946,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   119,   119,    29,    30,    31,    32,    33,    34,
      35,    36,   119,   119,    14,   119,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,   119,   119,    72,    59,    60,    72,   119,   119,    64,
     126,   119,   119,    68,     7,     7,  1008,   199,   200,   201,
     202,   203,   204,     7,   206,   207,   208,   209,   210,   211,
     212,   213,   105,   105,    72,    10,    10,    10,    93,    94,
      10,   122,    67,    14,   126,    14,    14,    14,   125,    14,
      14,   106,   126,    14,   119,   119,   119,   119,   113,   119,
     119,   119,   119,   119,   119,   119,   121,   119,   119,   119,
     125,   126,     3,     4,     5,     6,    14,   119,   126,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   119,   119,    29,    30,
      31,    32,    33,    34,    35,    36,   119,   119,   119,   119,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,   119,   122,   553,    59,    60,
     951,   501,   220,    64,   221,    -1,    -1,    68,    -1,   441,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,    -1,    -1,   125,   126,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    68,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,   119,    54,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,   119,    54,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
     119,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
      -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    67,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
      -1,    54,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
      -1,    54,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
      -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,
      -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,
      51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,
      -1,    73,    74,    64,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,    -1,   104,    -1,    -1,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,   106,   118,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   128,   129,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      64,    68,    93,    94,   106,   113,   119,   121,   125,   130,
     131,   133,   134,   135,   136,   137,   139,   140,   142,   143,
     146,   147,   150,   155,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   170,   171,   173,    91,
      93,    94,    95,    96,    97,    98,    99,   105,   118,   121,
     123,   107,   125,   105,   118,   121,    10,    11,    12,    13,
      31,   155,   167,    14,    10,   119,   121,   157,   172,    10,
      11,    12,    14,    31,    35,   121,   157,   121,   157,   121,
     157,   121,   157,   157,   163,    14,   155,   155,   157,   119,
     157,    10,    14,   157,    12,    12,   121,   157,   121,   157,
     155,   121,   155,    26,    43,   119,    26,    43,   119,   125,
     156,   156,   125,   156,   157,   155,   155,   157,    14,    14,
      14,    31,    14,    10,    10,   167,   167,   122,   157,    67,
     129,    26,    29,    30,    36,    39,    40,    43,    44,   119,
      26,    29,    30,    36,    39,    40,    43,    44,   119,    76,
      77,    78,    79,    86,    87,    88,   107,   108,    80,    81,
      82,    83,    84,    85,    92,   109,   110,   100,    73,    74,
     111,   102,   112,   113,   101,   114,   115,   116,   104,   118,
     118,    14,   119,   119,   157,   157,   157,   157,   157,   157,
      14,   122,   172,   157,    14,   157,   157,    14,    17,    58,
     122,   172,   123,   107,   125,   119,   105,   122,   157,   172,
     119,   120,   105,   119,   105,   119,   105,   119,    10,    11,
      12,    14,    10,    14,   157,    26,    29,    30,    36,    39,
      40,    43,   119,   120,   157,    26,    43,   119,   120,   157,
     155,   157,   155,    75,   120,   155,    73,    74,   121,   125,
     144,   119,   119,   105,   105,   125,   107,   125,   107,   125,
     157,   155,   157,   155,    23,   157,   157,   157,   157,   157,
     157,   157,   157,   157,    53,   148,   125,   121,    14,    62,
     125,   119,   119,   120,   122,   126,    10,    11,    12,    13,
      24,    25,    33,    34,    47,    48,   126,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   160,   160,   160,   160,   160,   160,
     158,   160,   160,   160,   160,   160,   160,   160,   160,     7,
       8,     9,   162,   162,   162,   163,   163,   164,   165,   166,
     166,   167,   167,   167,   167,   167,    14,    22,    24,    25,
      33,    34,    41,    42,    49,    50,    51,   169,    17,   169,
     119,   121,   119,   119,   119,   119,   119,   119,   105,   121,
     122,   124,   108,   126,   119,   121,    14,   121,   122,   157,
      14,   157,   157,   119,   122,   157,   157,   121,   157,     5,
      14,   121,   153,   157,   105,   119,   105,   119,   105,   119,
     121,   144,   105,   105,   120,   157,   157,   157,   157,   157,
     157,   157,   172,   120,   157,   157,   172,   122,    27,    28,
     132,   122,    10,   172,   163,   163,    10,    14,   122,   154,
     129,   157,   157,   141,    14,   157,    14,   157,   122,    27,
     122,    10,   119,   119,   119,   119,   119,   126,   155,    14,
      22,   125,   149,   122,   154,   121,   121,   151,   122,   172,
     118,   123,   125,   105,   157,   157,   157,   126,   157,    26,
      43,   126,    26,    43,   126,    26,    29,    30,    36,    39,
      40,    43,    44,   126,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
      89,   121,    14,   121,   122,   172,   157,   122,   172,   105,
     105,   105,   122,   172,    14,   122,   152,   172,   124,   108,
     126,   119,   119,   157,   103,   119,   120,    72,    72,   153,
     119,   120,   119,   157,   121,   157,   157,   122,   154,   157,
     157,   172,   119,   119,   119,   119,   119,   119,   119,   119,
     172,   119,   119,   119,   155,   131,   155,   121,   157,   155,
     155,    75,   155,    75,    75,    10,    65,    66,   144,   120,
     122,    24,    25,    34,   126,   157,   119,   119,    37,    38,
     126,   108,   126,   108,   126,   155,   155,   155,   105,   157,
      14,    14,   149,    56,    57,    60,   126,   144,   122,   122,
     154,   154,    69,    70,    71,   126,   122,    14,   157,   157,
     157,    26,    30,    36,    39,    40,    43,   126,   126,   126,
     126,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   158,   122,   172,   122,   152,   172,   119,
     122,   119,   122,   157,   157,   157,   122,    72,   120,   122,
     122,   120,   157,   172,   157,   157,   122,     5,    14,   119,
     157,   119,   120,   119,   144,   122,   119,   119,   122,   122,
      27,   132,   157,   155,    10,    10,    10,    10,    10,    10,
      14,   144,   157,   157,   126,   157,    26,    29,    30,    36,
      39,    40,    43,    44,   126,   157,   155,   119,   119,    27,
     157,   119,   126,    58,   169,    10,    13,    14,    15,    16,
      56,   144,   144,   122,   122,    14,    14,    14,   105,   121,
     124,   126,   126,   157,   157,   157,   157,   157,   157,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   122,   122,   122,   119,   119,   119,   119,   157,    14,
     172,   119,   119,   119,    72,    72,   120,   172,   144,   119,
     119,   131,   155,   122,    27,   132,   155,   155,   155,    10,
      65,    66,    26,    30,    43,   126,   126,   126,   157,   157,
     157,   157,   157,   157,   157,   157,   155,   126,   155,   119,
      10,    93,    94,   138,   157,   121,   125,   145,   121,   145,
      14,    22,    63,   105,   119,    14,    22,    63,   105,   119,
      10,    13,    15,    16,   119,   119,   169,   144,   144,     7,
       7,     7,   157,   122,   172,   105,   105,   126,   126,   126,
     126,   126,   126,    72,   122,   157,   157,   172,   119,   155,
     155,    10,    10,   157,   157,   157,   126,   126,   126,   126,
     126,   126,   126,   126,   157,    93,    94,    10,    10,   122,
     122,   154,    67,   129,   122,   154,    14,    14,   119,   157,
      14,    14,   119,   157,    14,    22,    63,   105,   119,    14,
      22,    63,   105,   119,   119,   119,   121,   145,   126,   126,
     122,   157,   157,   157,   119,   122,    27,   132,   126,   126,
     126,   119,   155,   145,   122,   126,    24,    25,    34,   126,
     157,   145,   122,   119,   119,   119,   119,   119,   119,    14,
      14,   119,   157,    14,    14,   119,   157,   122,   154,   126,
     126,   126,   119,   155,   138,   145,   157,   157,   126,   157,
      26,    29,    30,    36,    39,    40,    43,    44,   126,   145,
     119,   119,   119,   119,   119,   119,   145,   122,   122,    26,
      30,    43,   126,   126,   126,   157,   157,   157,   157,   157,
     157,   157,   157,   145,   155,   157,   157,   157,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   127,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   131,   131,   131,   131,   131,   131,   131,   131,
     132,   132,   132,   132,   132,   132,   133,   133,   134,   134,
     134,   134,   135,   135,   136,   137,   137,   137,   138,   138,
     138,   138,   138,   139,   139,   139,   139,   139,   139,   140,
     140,   141,   141,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   143,   143,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   146,   147,   148,   148,   148,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     150,   151,   151,   151,   151,   152,   152,   153,   153,   153,
     153,   154,   154,   154,   154,   154,   154,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   156,   157,   157,   157,   158,   158,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   160,
     161,   161,   162,   162,   162,   163,   163,   164,   164,   165,
     165,   165,   166,   166,   166,   166,   166,   167,   167,   167,
     167,   168,   168,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   170,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   172,   172,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     7,     9,     5,     5,     7,     6,     6,     8,    10,
       6,     4,     4,     4,     3,     5,     5,     6,     6,     2,
       5,     4,     3,     3,     5,     7,     3,     5,     7,     3,
       3,     2,     4,     7,     6,     4,     6,     4,     2,     4,
       4,     4,     4,     4,     4,     4,     4,     7,     7,     7,
       6,     6,     4,     4,     4,     4,     4,     4,     4,     4,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     4,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     4,     2,     1,     1,     1,
       1,     1,     2,     2,     4,     4,     4,     4,     1,     1,
       1,     1,     5,     7,     7,     3,     5,     5,     4,     6,
       3,     5,     5,     7,     4,     6,     5,     3,     5,     7,
       3,     5,     5,     3,     2,     2,    12,     9,     1,     2,
       2,     2,     2,     7,     7,     5,     7,     5,     3,     5,
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
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     2,     2,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     5,     5,     1,
       1,     1,     1,     2,     3,     4,     5,     1,     2
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
#line 324 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2653 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 334 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2659 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 335 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2665 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 339 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2671 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 341 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2677 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 343 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2683 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 345 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2689 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 347 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2695 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 349 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2703 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 353 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2710 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 356 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2718 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 360 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2724 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 362 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2730 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 364 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2736 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 366 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2742 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 368 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2748 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 370 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2756 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 374 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2764 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2770 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 380 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2776 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 382 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2782 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 384 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2788 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 386 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2794 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 388 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2800 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 390 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2806 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 392 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2812 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 394 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2818 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 396 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2824 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 398 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2830 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 400 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2836 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 402 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2842 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 404 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2848 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 406 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2855 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 409 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2862 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 412 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2868 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 414 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2875 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 417 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2882 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 420 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2888 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 422 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2894 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 424 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2900 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 426 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2906 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 428 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2915 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 433 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2923 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 437 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2931 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 441 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2940 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 446 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2948 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 449 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2954 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 451 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2960 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 453 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2966 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 455 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2972 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 457 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2978 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 459 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2985 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 462 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2991 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 464 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2997 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 466 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3003 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 468 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3010 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 471 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3017 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 474 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3024 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 477 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3031 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 480 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3038 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 483 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3044 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 485 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3050 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3056 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 489 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3062 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 491 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3069 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 494 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3075 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 496 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3081 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 498 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3087 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 500 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3094 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 503 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3101 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 506 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3109 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 510 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3116 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 513 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3122 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 515 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3128 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 517 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3134 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 519 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3141 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 522 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3148 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 525 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3155 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 528 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3162 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 531 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3169 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 534 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3176 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 537 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3183 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 540 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3190 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 543 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3197 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 546 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3204 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 549 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3211 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 551 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3217 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 552 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3223 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 553 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3229 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 554 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3235 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 555 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3241 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3247 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 558 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3253 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 560 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3259 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 562 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3265 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 564 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3271 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 565 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3277 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 566 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3283 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 567 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3289 "raku.tab.c"
    break;

  case 101: /* stmt: loop_stmt  */
#line 568 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3295 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST ';'  */
#line 569 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3301 "raku.tab.c"
    break;

  case 103: /* stmt: KW_NEXT ';'  */
#line 570 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3307 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST KW_IF expr ';'  */
#line 572 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3313 "raku.tab.c"
    break;

  case 105: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 574 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3319 "raku.tab.c"
    break;

  case 106: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 576 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3325 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3331 "raku.tab.c"
    break;

  case 108: /* stmt: sub_decl  */
#line 579 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3337 "raku.tab.c"
    break;

  case 109: /* stmt: class_decl  */
#line 580 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3343 "raku.tab.c"
    break;

  case 110: /* stmt: role_decl  */
#line 581 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3349 "raku.tab.c"
    break;

  case 111: /* stmt: grammar_decl  */
#line 582 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3355 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3361 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 588 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3367 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 590 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3373 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3379 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3385 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 596 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3391 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block elsif_tail  */
#line 598 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3397 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 600 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3403 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3409 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3415 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3421 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3427 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3433 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 614 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3439 "raku.tab.c"
    break;

  case 126: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 618 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3445 "raku.tab.c"
    break;

  case 127: /* while_stmt: KW_WHILE expr block  */
#line 620 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3451 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3457 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3463 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS expr block  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3469 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 630 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3475 "raku.tab.c"
    break;

  case 132: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 634 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3481 "raku.tab.c"
    break;

  case 133: /* until_stmt: KW_UNTIL expr block  */
#line 636 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3487 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3493 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 644 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3499 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 646 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3505 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 648 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3511 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 651 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3517 "raku.tab.c"
    break;

  case 139: /* loop_incr: VAR_SCALAR OP_INC  */
#line 652 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),1); }
#line 3523 "raku.tab.c"
    break;

  case 140: /* loop_incr: VAR_SCALAR OP_DEC  */
#line 653 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),0); }
#line 3529 "raku.tab.c"
    break;

  case 141: /* loop_incr: OP_INC VAR_SCALAR  */
#line 654 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 3535 "raku.tab.c"
    break;

  case 142: /* loop_incr: OP_DEC VAR_SCALAR  */
#line 655 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 3541 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 659 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3551 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 665 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3561 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 671 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3570 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 676 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3580 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 682 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3589 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 687 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3596 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 692 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3608 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 700 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3621 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 710 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3627 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 712 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3635 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 718 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3647 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 726 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3657 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 732 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3667 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 738 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3679 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 746 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3689 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 752 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3699 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 758 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 767 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3723 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 774 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 783 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3749 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 792 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3760 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 801 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3771 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 808 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3780 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 814 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3786 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 816 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3793 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 819 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3800 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 822 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3806 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 824 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3812 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 826 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3818 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3824 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 830 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3830 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 832 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3836 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3842 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 836 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3849 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 839 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3855 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 841 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3861 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 843 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3867 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 845 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3874 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 848 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3881 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 851 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3889 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 856 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3895 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 857 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3901 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 859 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3908 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 862 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3915 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 865 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3921 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 867 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3927 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 869 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3933 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3939 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 873 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3945 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 875 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3951 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 877 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3957 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 879 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3964 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 882 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3970 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 884 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3976 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 886 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3982 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 888 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3989 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 891 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3996 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 894 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4004 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 900 "raku.y"
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
#line 4022 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 916 "raku.y"
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
#line 4039 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 930 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4045 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 932 "raku.y"
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
#line 4061 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 944 "raku.y"
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
#line 4077 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 957 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4083 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 959 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4090 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 962 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4097 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 965 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4104 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 968 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4111 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 971 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4118 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 974 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4125 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 977 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4132 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 980 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4139 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 983 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4146 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 986 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4153 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 989 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4160 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 992 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4167 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 995 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4178 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1002 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4189 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1009 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4200 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1016 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4211 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1023 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4219 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1027 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4227 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1031 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4235 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1035 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4243 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1039 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4254 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1046 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4265 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1053 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4276 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1060 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4287 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1067 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4301 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1077 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4313 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1085 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4325 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1093 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4338 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1102 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4349 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1109 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4360 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1116 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4374 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1126 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4386 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1134 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4398 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1144 "raku.y"
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
#line 4415 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1158 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4421 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1160 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4430 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1165 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4439 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1170 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4448 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1177 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4456 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1181 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4464 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1187 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4470 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1189 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4476 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1191 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4482 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1193 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4488 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1196 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4494 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1197 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4500 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1198 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4506 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1199 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4512 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1200 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4518 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1201 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4524 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1202 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4530 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1203 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4536 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list '}'  */
#line 1206 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4542 "raku.tab.c"
    break;

  case 260: /* block: '{' YADA '}'  */
#line 1207 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4548 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr '}'  */
#line 1209 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4554 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1211 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4560 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1213 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4566 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1215 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4572 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1217 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4578 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1219 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4585 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1222 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4591 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1224 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4597 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1226 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4603 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1228 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4609 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1230 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4616 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1233 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4623 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1236 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4631 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1240 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4638 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1243 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4645 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1246 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4652 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1249 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4658 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1251 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4666 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1255 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4673 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1258 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4680 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1261 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4687 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1264 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4694 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1267 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4701 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1270 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4707 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1272 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4713 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1274 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4719 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_LAST '}'  */
#line 1276 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4725 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1278 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4731 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1280 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4737 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1282 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4743 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1284 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4749 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1286 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4755 "raku.tab.c"
    break;

  case 293: /* closure: '{' expr '}'  */
#line 1289 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4761 "raku.tab.c"
    break;

  case 294: /* expr: VAR_SCALAR '=' expr  */
#line 1292 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4767 "raku.tab.c"
    break;

  case 295: /* expr: KW_GATHER block  */
#line 1293 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4777 "raku.tab.c"
    break;

  case 296: /* expr: tern_expr  */
#line 1298 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4783 "raku.tab.c"
    break;

  case 297: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1302 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4789 "raku.tab.c"
    break;

  case 298: /* tern_expr: cmp_expr  */
#line 1303 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4795 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1306 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4801 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1307 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4807 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1308 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4813 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1309 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4819 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1310 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4825 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1311 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4831 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1312 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4837 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1313 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4843 "raku.tab.c"
    break;

  case 307: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1314 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4849 "raku.tab.c"
    break;

  case 308: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1315 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4855 "raku.tab.c"
    break;

  case 309: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1316 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4861 "raku.tab.c"
    break;

  case 310: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1317 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4867 "raku.tab.c"
    break;

  case 311: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1318 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4873 "raku.tab.c"
    break;

  case 312: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1319 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4879 "raku.tab.c"
    break;

  case 313: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1321 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4889 "raku.tab.c"
    break;

  case 314: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1327 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4899 "raku.tab.c"
    break;

  case 315: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1333 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4909 "raku.tab.c"
    break;

  case 316: /* cmp_expr: jct_expr  */
#line 1338 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4915 "raku.tab.c"
    break;

  case 317: /* jct_expr: jct_expr '|' range_expr  */
#line 1341 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4921 "raku.tab.c"
    break;

  case 318: /* jct_expr: jct_expr '&' range_expr  */
#line 1342 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4927 "raku.tab.c"
    break;

  case 319: /* jct_expr: dor_expr  */
#line 1343 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4933 "raku.tab.c"
    break;

  case 320: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1347 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4939 "raku.tab.c"
    break;

  case 321: /* dor_expr: range_expr  */
#line 1348 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4945 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1351 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4951 "raku.tab.c"
    break;

  case 323: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1352 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4957 "raku.tab.c"
    break;

  case 324: /* range_expr: add_expr  */
#line 1353 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4963 "raku.tab.c"
    break;

  case 325: /* add_expr: add_expr '~' repl_expr  */
#line 1356 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4969 "raku.tab.c"
    break;

  case 326: /* add_expr: repl_expr  */
#line 1357 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4975 "raku.tab.c"
    break;

  case 327: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1360 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4981 "raku.tab.c"
    break;

  case 328: /* repl_expr: addsub_expr  */
#line 1361 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4987 "raku.tab.c"
    break;

  case 329: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1364 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4993 "raku.tab.c"
    break;

  case 330: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1365 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4999 "raku.tab.c"
    break;

  case 331: /* addsub_expr: mul_expr  */
#line 1366 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5005 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '*' unary_expr  */
#line 1369 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5011 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr '/' unary_expr  */
#line 1370 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5017 "raku.tab.c"
    break;

  case 334: /* mul_expr: mul_expr '%' unary_expr  */
#line 1371 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5023 "raku.tab.c"
    break;

  case 335: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1372 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5029 "raku.tab.c"
    break;

  case 336: /* mul_expr: unary_expr  */
#line 1373 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5035 "raku.tab.c"
    break;

  case 337: /* unary_expr: '-' unary_expr  */
#line 1376 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5041 "raku.tab.c"
    break;

  case 338: /* unary_expr: '!' unary_expr  */
#line 1377 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5047 "raku.tab.c"
    break;

  case 339: /* unary_expr: CARET unary_expr  */
#line 1378 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5053 "raku.tab.c"
    break;

  case 340: /* unary_expr: pow_expr  */
#line 1379 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5059 "raku.tab.c"
    break;

  case 341: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1382 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5065 "raku.tab.c"
    break;

  case 342: /* pow_expr: postfix_expr  */
#line 1383 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5071 "raku.tab.c"
    break;

  case 343: /* meth_name: IDENT  */
#line 1386 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5077 "raku.tab.c"
    break;

  case 344: /* meth_name: KW_SORT  */
#line 1387 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5083 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_MAP  */
#line 1388 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5089 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_GREP  */
#line 1389 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5095 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_SAY  */
#line 1390 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5101 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_PRINT  */
#line 1391 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5107 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_TAKE  */
#line 1392 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5113 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_RETURN  */
#line 1393 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5119 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_EXISTS  */
#line 1394 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5125 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_DELETE  */
#line 1395 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5131 "raku.tab.c"
    break;

  case 353: /* meth_name: TESTOP  */
#line 1396 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5137 "raku.tab.c"
    break;

  case 354: /* postfix_expr: call_expr  */
#line 1398 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5143 "raku.tab.c"
    break;

  case 355: /* call_expr: IDENT '(' arg_list ')'  */
#line 1401 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5152 "raku.tab.c"
    break;

  case 356: /* call_expr: IDENT '(' ')'  */
#line 1405 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5158 "raku.tab.c"
    break;

  case 357: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1407 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5167 "raku.tab.c"
    break;

  case 358: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1412 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5173 "raku.tab.c"
    break;

  case 359: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1414 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5183 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1420 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5193 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1426 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5201 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' KW_NEW  */
#line 1430 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5209 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' IDENT  */
#line 1434 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5218 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1439 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5229 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1446 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5238 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' CARET IDENT  */
#line 1451 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5248 "raku.tab.c"
    break;

  case 367: /* call_expr: atom '.' CARET IDENT  */
#line 1457 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5258 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1463 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5269 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1470 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5280 "raku.tab.c"
    break;

  case 370: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1477 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5289 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' meth_name  */
#line 1482 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5298 "raku.tab.c"
    break;

  case 372: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1487 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5309 "raku.tab.c"
    break;

  case 373: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1494 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5318 "raku.tab.c"
    break;

  case 374: /* call_expr: call_expr '.' meth_name  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5327 "raku.tab.c"
    break;

  case 375: /* call_expr: KW_DIE expr  */
#line 1504 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5333 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_MAP closure expr  */
#line 1506 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5339 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_GREP closure expr  */
#line 1508 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5345 "raku.tab.c"
    break;

  case 378: /* call_expr: KW_SORT expr  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5351 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_SORT closure expr  */
#line 1512 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5357 "raku.tab.c"
    break;

  case 380: /* call_expr: atom  */
#line 1513 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5363 "raku.tab.c"
    break;

  case 381: /* arg_list: expr  */
#line 1516 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5369 "raku.tab.c"
    break;

  case 382: /* arg_list: arg_list ',' expr  */
#line 1517 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5375 "raku.tab.c"
    break;

  case 383: /* atom: LIT_INT  */
#line 1520 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5381 "raku.tab.c"
    break;

  case 384: /* atom: LIT_FLOAT  */
#line 1521 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5387 "raku.tab.c"
    break;

  case 385: /* atom: LIT_STR  */
#line 1522 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5393 "raku.tab.c"
    break;

  case 386: /* atom: WORDLIST  */
#line 1524 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5405 "raku.tab.c"
    break;

  case 387: /* atom: LIT_INTERP_STR  */
#line 1531 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5411 "raku.tab.c"
    break;

  case 388: /* atom: VAR_SCALAR  */
#line 1532 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5417 "raku.tab.c"
    break;

  case 389: /* atom: VAR_ARRAY  */
#line 1533 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5423 "raku.tab.c"
    break;

  case 390: /* atom: VAR_HASH  */
#line 1534 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5429 "raku.tab.c"
    break;

  case 391: /* atom: VAR_CAPTURE  */
#line 1536 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5437 "raku.tab.c"
    break;

  case 392: /* atom: VAR_FH  */
#line 1540 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5445 "raku.tab.c"
    break;

  case 393: /* atom: VAR_NAMED_CAPTURE  */
#line 1544 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5452 "raku.tab.c"
    break;

  case 394: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1547 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5458 "raku.tab.c"
    break;

  case 395: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1549 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5464 "raku.tab.c"
    break;

  case 396: /* atom: VAR_HASH '{' expr '}'  */
#line 1551 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5470 "raku.tab.c"
    break;

  case 397: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1553 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5476 "raku.tab.c"
    break;

  case 398: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1555 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5482 "raku.tab.c"
    break;

  case 399: /* atom: IDENT  */
#line 1556 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5488 "raku.tab.c"
    break;

  case 400: /* atom: VAR_TWIGIL  */
#line 1558 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5496 "raku.tab.c"
    break;

  case 401: /* atom: VAR_ARRAY_TWIGIL  */
#line 1562 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5504 "raku.tab.c"
    break;

  case 402: /* atom: VAR_HASH_TWIGIL  */
#line 1566 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5512 "raku.tab.c"
    break;

  case 403: /* atom: '(' ')'  */
#line 1569 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5518 "raku.tab.c"
    break;

  case 404: /* atom: '(' expr ')'  */
#line 1570 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5524 "raku.tab.c"
    break;

  case 405: /* atom: '(' expr ',' ')'  */
#line 1572 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5530 "raku.tab.c"
    break;

  case 406: /* atom: '(' expr ',' arg_list ')'  */
#line 1574 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5537 "raku.tab.c"
    break;

  case 407: /* atom: block  */
#line 1576 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5543 "raku.tab.c"
    break;

  case 408: /* atom: KW_SUB block  */
#line 1577 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5549 "raku.tab.c"
    break;


#line 5553 "raku.tab.c"

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

#line 1579 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
