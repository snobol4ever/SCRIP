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
  YYSYMBOL_postfix_expr = 169,             /* postfix_expr  */
  YYSYMBOL_call_expr = 170,                /* call_expr  */
  YYSYMBOL_arg_list = 171,                 /* arg_list  */
  YYSYMBOL_atom = 172                      /* atom  */
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
#define YYLAST   3675

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  397
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1028

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
    1379,  1382,  1383,  1385,  1387,  1392,  1393,  1398,  1400,  1406,
    1412,  1416,  1420,  1425,  1432,  1437,  1443,  1449,  1456,  1463,
    1468,  1473,  1480,  1485,  1490,  1492,  1494,  1496,  1498,  1500,
    1503,  1504,  1507,  1508,  1509,  1510,  1518,  1519,  1520,  1521,
    1522,  1526,  1530,  1533,  1535,  1537,  1539,  1541,  1543,  1544,
    1548,  1552,  1556,  1557,  1558,  1560,  1563,  1564
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
  "pow_expr", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-730)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-397)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -730,    63,  1504,  -730,  -730,  -730,  -730,  -730,   592,   -97,
     -56,   -27,   -32,  -730,  -730,  3550,  -730,  -730,  -730,    44,
    1636,   283,  2758,  2824,  2890,  2956,  3022,    -6,   -37,  3022,
    1702,   224,  3022,   108,   130,  3088,  3154,   -37,   132,    21,
      22,    51,    51,  3220,   -37,   -37,  3022,   157,   188,    35,
    -730,   194,   248,   253,  3550,  3550,  -730,  1768,   201,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,   537,
    -730,  -730,  -730,  1369,   549,  -730,   519,   554,   217,  -730,
      23,   220,   291,   171,  -730,  -730,   250,   262,   272,   349,
     247,   274,  3022,  3022,  3022,  3022,  3022,  3022,   412,  1834,
    3022,   441,  3022,  3022,    70,  1900,   351,   337,    80,  -730,
     -37,  -730,  -730,   366,     3,  -730,  1966,  -730,   327,    92,
     146,   184,   589,   474,   378,  1768,   513,  1768,    17,  1768,
     -37,  1768,   -37,   -16,   136,   287,  -730,  -730,   389,  -730,
     401,   424,   430,   420,   116,   120,  1768,   -37,  1768,   -37,
    -730,  2032,  -730,  3022,  3022,  -730,  3022,  3022,  -730,  3022,
    3022,  3022,  2098,  3022,  -730,   516,  -730,  -730,  -730,   457,
     481,     5,   536,   499,   503,  -730,  -730,  -730,   213,   504,
     999,  3022,  3022,  3022,  3022,  3022,  3022,  3022,  3022,  -730,
    3022,  3022,  3022,  3022,  3022,  3022,  3022,  3022,  -730,  3550,
    3550,  3550,  3550,  3550,  3550,  3550,  3550,  3550,  3550,  3550,
    3550,  3550,  3550,  3550,   634,  3550,  3550,  3550,  3550,  3550,
    3550,  3550,  3550,  3550,  3550,  3550,  3550,  3550,  3550,   656,
     341,   384,  -730,  -730,   538,   563,   577,   603,   612,   635,
     150,  -730,   585,   657,   677,   650,   671,   686,   819,   768,
    -730,   594,  3022,   824,  3022,  -730,  3022,   733,   213,   595,
    -730,  3022,  3022,  -730,  3286,  -730,  3352,  -730,   185,   227,
     245,   296,   752,   756,   598,  3022,  3022,  3022,  3022,  3022,
    3022,  3022,  -730,  3022,   599,  3022,  3022,  -730,  3022,   630,
     532,   631,  -730,   882,  3022,  -730,  3550,  3550,    -5,  -730,
    -730,  -730,  -730,  3022,  3022,  -730,   879,  3022,   880,  3022,
     636,   868,   637,  -730,   886,   778,   779,   780,   782,   783,
     777,  -730,  -730,  -730,   -37,    -7,  -730,    25,   784,   785,
    -730,  -730,  -730,  2164,  -730,  -730,   703,   781,   123,   803,
    2758,  2824,  3022,   729,   218,   308,  -730,   280,   790,   791,
     796,   797,   798,   799,   801,   802,   804,   805,   806,   807,
     808,   809,   810,   811,   477,   477,   477,   477,   477,   477,
     833,   477,   477,   477,   477,   477,   477,   477,   477,  -730,
    -730,  -730,  -730,  -730,  -730,   820,   820,   220,   291,   171,
     171,  -730,  -730,  -730,  -730,  -730,   812,   813,   921,  -730,
    2230,  -730,  -730,  -730,  -730,  -730,  -730,  3022,  2296,  -730,
     832,   834,   835,  -730,  2362,  -730,  2428,  -730,   817,   838,
     816,  -730,  -730,   828,  -730,   829,  1768,    -4,   877,   -47,
    2494,   525,   831,  3022,  -730,  3418,  -730,  3022,  -730,    26,
    -730,  3022,  3022,  2164,   836,   837,   839,   840,   841,   842,
     843,   575,  2164,   844,   846,   617,   -37,   -13,  3484,  -730,
     -37,   -37,    10,   -43,   -38,  -730,    67,   826,   642,  1124,
     847,   848,   -21,   849,   827,   856,   850,   -37,   -37,   -37,
     863,  3022,  -730,  -730,  -730,  -730,  -730,  -730,   938,   940,
    -730,    19,   826,   643,    28,   422,    12,  -730,   646,   955,
    3022,  3022,  3022,   331,   113,    47,  -730,   172,  3022,  3022,
    -730,  3022,  3022,  -730,  3022,  3022,  3022,  3022,  3022,  3022,
    3022,  3022,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  3550,
    2560,  2626,  -730,   851,   647,   852,  -730,   651,  3022,  3022,
    3022,  -730,   652,   -11,  -730,   655,   662,  -730,  -730,  -730,
    -730,  -730,   666,  3022,  -730,  3022,  3022,  3022,   667,  -730,
     348,  -730,   853,  1768,   740,   855,   826,   683,   866,   867,
     684,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,   692,
    -730,  -730,  -730,   795,  -730,  -730,  1768,   -37,  -730,  -730,
     963,  -730,   965,   977,  -730,   978,   979,  -730,   520,   826,
    2758,  2824,  1314,  -730,   343,  -730,  -730,  3022,   -37,  -730,
    -730,  -730,   871,   872,   966,  -730,  -730,  3022,   873,  -730,
    -730,    46,   117,   688,   939,  -730,  -730,   826,   826,   693,
     697,   980,   985,   992,  -730,  -730,   175,   883,   900,   173,
    3022,  3022,  3022,  3022,  3022,  3022,  -730,  -730,  -730,  -730,
     233,   249,   270,   290,   294,   299,   303,   304,   318,   319,
     322,   323,  -730,  -730,   698,  -730,   705,   708,  -730,   889,
    -730,  -730,   908,   917,   918,  -730,  3022,  1024,  -730,  -730,
    2164,   920,   760,  -730,  -730,   936,   984,   988,  -730,   709,
    -730,  3022,  -730,  -730,   826,  -730,  -730,   942,   943,   -13,
    -730,   712,   854,   -37,   -37,   -37,  -730,  -730,  -730,   103,
    -730,   355,   223,  -730,   333,  3022,  3022,  3022,  3022,  3022,
    3022,  3022,  3022,  -730,   -37,   931,  -730,  -730,   -37,   945,
    1570,  -730,   433,   447,    48,    58,   696,   946,   947,  1054,
    -730,  -730,   826,   826,  1062,  1063,  1064,  3022,  2692,   967,
     968,  -730,   339,   342,   344,   345,   347,   357,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730,  1002,   717,
    -730,  -730,  -730,  3022,  3022,  2164,   764,  -730,  -730,  -730,
    -730,  -730,   -37,   -37,  -730,  -730,  -730,  -730,  -730,  1065,
    1066,  3022,  3022,  3022,  -730,  -730,  -730,   358,   361,   363,
     367,   375,   385,   402,   405,  -730,  -730,  -730,  3022,   119,
    1067,  1068,   957,  -730,    40,  1013,  -730,    42,  -730,  1069,
    1070,   962,  3022,  -730,  1071,  1072,   969,  3022,  -730,   206,
     207,   970,   971,  -730,  -730,   449,  -730,  -730,  -730,  -730,
    -730,   406,   956,   721,  3022,  3022,  -730,  -730,  -730,  -730,
    -730,  -730,  3022,   972,  -730,  -730,   724,  -730,   858,  -730,
    -730,  -730,   425,   431,   436,  -730,  -730,  -730,  -730,  -730,
    -730,  -730,  -730,   975,  -730,  -730,  -730,  -730,   -37,   973,
     727,   961,  1248,   973,   731,   976,   982,  -730,   983,   987,
     989,  -730,   990,  1083,  1085,   991,  3022,  -730,  1089,  1090,
     994,  3022,  -730,  -730,  -730,    43,  -730,  -730,  -730,   981,
     464,   465,  -730,  -730,   995,   -37,  -730,  -730,  -730,  -730,
    1570,  -730,  -730,   973,  -730,  2758,  2824,  1380,  -730,   371,
    -730,   973,  -730,  -730,  -730,  -730,  -730,  -730,   996,   997,
    -730,   998,  1000,  1003,  -730,  1004,   973,   736,  -730,  -730,
    -730,  -730,  -730,  1009,  -730,   497,   376,  -730,   485,  3022,
    3022,  3022,  3022,  3022,  3022,  3022,  3022,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,   973,   -37,  3022,  3022,
    3022,  -730,  -730,  -730,   493,   494,   502,   505,   521,   543,
     546,   547,  -730,  -730,   555,   573,   574,  -730,  -730,  -730,
    -730,  -730,  -730,  -730,  -730,  -730,  -730,  -730
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   372,   373,   374,   376,   377,   378,
     379,   389,   388,   390,   391,     0,   380,   381,   382,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,    89,     0,     3,     4,
      90,    91,    98,    99,   100,   101,    92,    93,   108,     0,
     109,   110,   111,    97,     0,   296,   298,   316,   319,   321,
     324,   326,   328,   331,   336,   340,   342,   343,   369,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   378,   379,   389,
       0,   396,   339,     0,   377,    29,     0,   370,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   324,     0,   397,   295,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,     0,   135,     0,     0,   102,     0,     0,   103,     0,
       0,     0,     3,     0,   367,    94,    96,   364,   203,     0,
       0,     0,     0,     0,     0,   338,   337,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,    85,     0,     0,     0,     0,     0,   294,
       0,   347,     0,     0,     0,     0,     0,   352,     0,   351,
     345,     0,     0,     0,     0,    24,     0,   392,     0,     0,
      32,     0,     0,     6,     0,     7,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,    36,     0,     0,
     115,     0,   127,     0,     0,   148,     0,     0,     0,     3,
     155,    39,    40,     0,     0,   151,     0,     0,     0,     0,
       0,   130,     0,   133,     0,     0,     0,     0,     0,     0,
       0,   365,   366,   368,     0,     0,   206,     0,     0,     0,
     241,    86,    87,     0,   393,   260,   377,   378,   379,   389,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,   302,   305,   306,   299,   300,
       0,   303,   304,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   317,   318,   320,   322,   323,   325,   327,   329,
     330,   335,   332,   333,   334,   341,   363,   360,     0,    45,
       0,    79,    80,    81,    82,    83,    42,     0,     0,   346,
     383,   384,   385,    47,     0,   355,     0,   344,     0,     0,
       0,   294,    31,     0,   371,     0,     0,     0,   374,   388,
       0,     0,     0,     0,    21,     0,    22,     0,    23,     0,
     158,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,   118,
     393,     0,     0,   322,   323,   251,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,     0,   393,
       0,     0,   104,   105,   106,   107,   293,    95,     0,     0,
     206,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,   286,     0,     0,     0,
     287,     0,     0,   288,     0,     0,     0,     0,     0,     0,
       0,     0,   261,    49,    51,    53,    56,    54,    55,    50,
      52,    62,    64,    66,    69,    67,    68,    63,    65,     0,
       0,     0,   356,     0,     0,     0,   165,     0,     0,     0,
       0,   354,     0,   388,   350,     0,     0,   383,   384,   385,
      30,     5,     0,     0,     9,     0,     0,     0,     0,    14,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    73,    72,    76,    74,    75,    71,    34,     0,
      77,    78,    37,   112,   117,   116,     0,     0,   126,   145,
       0,   147,     0,     0,   252,     0,     0,   154,     0,     0,
       0,     0,     0,   166,     0,    26,    25,     0,     0,   149,
     386,   387,     0,     0,   128,   131,   132,     0,     0,   204,
     205,     0,     0,     0,     0,   202,   163,     0,     0,     0,
       0,     0,     0,     0,   240,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,   277,   284,   285,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,   362,     0,   359,     0,     0,    44,     0,
      46,   164,     0,     0,     0,   353,     0,     0,   348,   349,
       0,     0,     0,   248,   247,     0,     0,     0,    16,     0,
      17,     0,    20,   157,     0,    28,    27,   395,   395,     0,
     119,     0,   120,     0,     0,     0,   253,   254,   255,     0,
     153,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,    60,    61,     0,     0,
       0,   201,     0,     0,     0,     0,     0,     0,     0,     0,
     162,   160,     0,     0,     0,     0,     0,     0,     0,   383,
     385,   281,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   292,   262,   264,   266,   269,   267,   268,   263,   265,
     361,   358,   357,    43,    57,    58,    59,   245,     0,     0,
      11,    10,    15,     0,     0,     0,     0,   156,    35,    38,
     114,   113,   393,     0,   124,   146,   143,   144,   256,     0,
       0,     0,     0,     0,   170,   171,   168,     0,     0,     0,
       0,     0,     0,     0,     0,   152,   150,   129,     0,   377,
       0,     0,     0,   138,     0,     3,   233,     0,   236,     0,
       0,     0,     0,   212,     0,     0,     0,     0,   207,     0,
       0,     0,     0,   208,   209,     0,   159,   161,   242,   243,
     244,     0,   165,     0,     0,     0,   271,   273,   276,   274,
     275,   272,     0,   395,   250,   249,     0,    18,   121,   122,
     257,   258,     0,     0,     0,   172,   174,   176,   179,   177,
     178,   173,   175,     0,   139,   140,   141,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,   224,     0,     0,     0,     0,     0,   214,     0,     0,
       0,     0,   213,   210,   211,     0,   239,   280,   279,   164,
       0,     0,   246,    12,   395,     0,   125,   180,   182,   181,
       0,   137,   232,     0,   184,     0,     0,     0,   183,     0,
     235,     0,   221,   222,   216,   219,   220,   215,     0,     0,
     225,     0,     0,     0,   226,     0,     0,     0,   278,   282,
     283,    19,   123,     0,   231,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,   234,   229,
     230,   218,   227,   228,   217,   238,     0,     0,     0,     0,
       0,   188,   189,   186,     0,     0,     0,     0,     0,     0,
       0,     0,   237,   136,     0,     0,     0,   190,   192,   194,
     197,   195,   196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -730,  -730,     1,  -730,  -453,  -590,  -730,  -730,  -730,  -730,
    -730,   181,  -730,  -730,  -730,  -730,  -730,  -261,  -729,  -730,
    -730,  -730,   621,  -730,  -730,   591,   722,  -296,   198,   845,
      -2,  -203,  -730,   665,  -730,   463,   -15,   906,   930,   668,
      -9,  -730,  -730,  -730,   -87,  -730
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   180,    59,    60,   459,    61,    62,    63,    64,
      65,   832,    66,    67,   472,    68,    69,   300,   836,    70,
      71,   325,   491,    72,   496,   555,   431,   468,   111,   160,
     117,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   370,   710,   594,   465,   112,   488,   135,   466,
     440,   134,   242,    24,   838,   489,   617,   618,   251,   328,
     126,   128,   130,   132,   133,   567,   100,   138,   140,   259,
     143,   493,   602,   147,   149,   465,   465,   603,   465,   466,
     466,   164,   466,   285,   167,   175,   176,   153,   156,   170,
     465,   101,   465,   465,   466,   178,   466,   466,   113,   293,
     286,   686,   839,     3,   154,   157,   171,   329,   220,   102,
     840,   104,   844,   220,   105,   632,   633,   604,   103,   634,
     845,   641,   642,   643,   247,   600,   104,   248,    58,   105,
     234,   235,   236,   237,   238,   239,   218,   219,   243,   563,
     245,   246,   632,   633,   294,   619,   634,   104,   256,    58,
     105,   841,    58,   808,   258,   564,   565,   467,   490,    58,
     144,   846,   804,   274,    99,   284,   926,   289,   249,   291,
     261,   742,   605,   606,   220,    58,   287,   288,   644,   285,
     155,   158,   145,   577,   310,   635,   312,   492,   576,   315,
     638,   316,   317,   842,   318,   319,   286,   320,   321,   322,
     320,   323,   899,   847,   903,   966,   301,   843,   809,   810,
     942,   168,   741,   658,   950,   743,   159,   848,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   253,   356,   357,
     358,   359,   360,   361,   362,   363,   451,   262,   639,   640,
      73,   455,   169,   385,   386,   254,   607,   462,   172,   296,
     297,   263,   894,   895,   974,   391,   392,   393,   394,   395,
     913,   918,   988,   306,   256,   136,   137,   308,   914,   919,
     101,   636,   287,   288,   141,   150,   152,   995,   142,   657,
      99,   307,   165,   166,   508,   309,   498,   220,   501,   285,
     418,   264,   420,   151,   421,   407,   800,    58,   173,   424,
     425,   509,   427,   174,   432,   265,   286,  1012,   179,   915,
     920,   408,   224,   444,   445,   446,   447,   448,   449,   450,
     757,   463,   464,   453,   454,   225,   226,   227,   936,   266,
     433,   302,   413,   119,   120,   121,   758,   122,   659,   761,
     469,   470,   471,   267,   434,   474,   514,   476,   136,   515,
     516,   916,   921,   544,   123,   703,   517,   217,   124,   518,
     519,   547,   221,   520,   521,   917,   922,   552,   290,   556,
     292,   295,   435,   333,   511,   334,   672,   155,   503,   504,
     505,   507,   287,   288,   510,   311,   436,   313,   720,   815,
     437,   512,   482,   696,   228,   397,   580,   650,   398,   768,
     276,   651,   697,   231,   438,   589,   232,   652,   483,   725,
     653,   654,   726,   727,   655,   769,   750,   751,    73,   728,
     229,   811,   729,   730,   276,   812,   731,   732,   272,   484,
     230,   278,   273,   233,   279,   280,   770,   979,   813,   198,
     980,   981,   285,   222,   223,   545,   522,   982,   298,   485,
     983,   984,   299,   531,   985,   986,   771,   439,   532,   286,
     772,   299,   533,   534,   562,   773,   240,   158,   178,   774,
     775,   572,   465,   574,   513,   575,   466,   535,   536,   578,
     579,   537,   538,   797,   776,   777,   260,   261,   778,   779,
     282,   283,   302,   674,   677,   244,   597,   656,   581,   816,
     252,   583,   198,   584,   585,   866,   586,   614,   867,   733,
     868,   869,    99,   870,   282,   283,   587,   531,   692,   628,
     532,   814,   533,   871,   885,   255,   534,   886,   271,   887,
     198,   856,   857,   888,   535,   287,   288,   987,   647,   648,
     649,   889,  1002,   399,   536,   400,   660,   661,   301,   662,
     663,   890,   664,   665,   666,   667,   668,   669,   670,   671,
     302,   537,   487,   998,   538,   680,   276,   999,   891,   303,
     718,   892,   927,   278,   719,   304,   279,   280,   900,   275,
    1000,   904,   276,   277,   581,   305,   682,   683,   684,   278,
     583,   937,   279,   280,   834,   587,   281,   938,   835,   457,
     458,   691,   939,   181,   693,   694,   182,   183,   837,   324,
     925,   699,   835,   184,   835,   190,   185,   186,   191,   192,
     187,   188,   326,   784,   786,   193,   215,   216,   194,   195,
     969,   970,   196,   197,   711,   199,   200,   201,   202,   268,
     269,   270,   327,   789,   302,   203,   204,   205,   721,   722,
     724,  1003,   531,   532,   796,   734,   282,   283,   331,  1017,
    1018,   533,   332,  1001,   534,   739,   206,   207,  1019,   967,
     335,  1020,   282,   283,   208,   209,   210,   211,   212,   213,
     535,   379,   380,   381,   569,   570,   214,  1021,   762,   763,
     764,   765,   766,   767,   593,   595,   189,   401,   598,   599,
     601,   330,   536,   215,   216,   537,   538,    73,   198,  1022,
     396,   863,  1023,  1024,   581,   624,   625,   626,   382,   383,
     384,  1025,   402,    89,   787,    90,    91,    92,    93,    94,
      95,    96,   583,   587,   588,   261,   403,    97,   744,  1026,
    1027,   745,   746,   747,   748,   261,   849,   409,   876,   850,
      98,   851,   852,    99,   261,   261,   417,   423,   443,   452,
     334,   334,   404,   817,   818,   819,   820,   821,   822,   823,
     824,   405,     4,     5,     6,     7,   592,   261,   833,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
     333,   333,   456,   460,   406,   861,   333,   333,   477,   479,
     110,    28,   608,   608,   609,   637,   261,   261,   645,   679,
      33,   261,   261,   681,   685,   687,   412,   688,    41,    42,
      43,   410,   261,    46,   689,   411,   690,   570,   334,   695,
     413,   874,   875,    50,    89,   712,    90,    91,    92,    93,
      94,    95,    96,   608,   261,   704,   707,   414,    97,   882,
     883,   884,   261,   608,   708,   752,   735,   608,   261,   753,
     780,   499,   709,   458,    99,   687,   893,   781,   261,   795,
     782,   334,   333,   415,   802,    54,   902,   261,   419,   873,
     908,   261,    55,   929,   261,   912,   934,   608,   139,   943,
      57,   608,   422,   951,    58,   506,   608,   441,   996,   700,
     701,   442,   930,   931,   364,   365,   366,   367,   368,   369,
     932,   371,   372,   373,   374,   375,   376,   377,   378,   791,
     261,   803,   458,   877,   261,   935,   458,   161,   163,   416,
     389,   390,   461,   473,   475,   478,   480,   481,   482,   483,
     949,   484,   485,   486,   500,   494,   495,   801,   502,   523,
     524,   805,   806,   807,   961,   525,   526,   527,   528,   965,
     529,   530,   539,   531,   532,   533,   534,   535,   536,   537,
     538,   220,   825,   540,   541,   542,   827,   548,   833,   549,
     550,   557,   559,   975,   976,   978,   558,   560,   561,   566,
     571,   299,   629,   621,   630,   581,   582,   620,   583,   584,
     585,   586,   587,   590,   622,   591,   615,   616,   627,   646,
     678,   680,   698,   713,   702,   714,   623,  1004,  1005,  1006,
    1007,  1008,  1009,  1010,  1011,   705,   706,   715,   716,   717,
     736,   737,   740,   738,   754,   749,  1014,  1015,  1016,   755,
     878,   879,     4,     5,     6,     7,   756,   759,   783,   336,
     337,   338,   339,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   340,   341,    24,   760,   784,    25,    26,
      27,    28,   342,   343,    31,    32,   785,   786,   788,   790,
      33,    34,    35,    36,    37,    38,   344,   345,    41,    42,
      43,    44,    45,    46,    47,   792,   793,   826,    48,    49,
     794,   798,   799,    50,   828,   853,   854,    51,   855,   858,
     859,   860,   864,   865,   872,   880,   881,   896,   897,   898,
     901,   907,   928,   905,   906,   909,   910,   944,   911,   923,
     924,   933,    52,    53,   940,   952,   941,   958,   835,   959,
      73,   953,   954,   962,   963,    54,   955,   968,   956,   957,
     960,   631,    55,   964,   971,   989,   990,   991,    56,   992,
      57,   973,   993,   994,    58,   346,   387,     4,     5,     6,
       7,   997,   676,   972,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   610,   611,
      24,   388,   568,    25,    26,    27,    28,    29,   612,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     0,     0,  1013,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,    56,     0,    57,     0,     0,     0,    58,
     613,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   945,   946,    24,     0,     0,    25,    26,    27,
      28,    29,   947,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,    48,    49,     0,
       0,     0,    50,     0,     0,     0,    51,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,    52,    53,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,    56,    46,    57,
       0,     0,     0,    58,   948,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,  -396,  -396,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,   139,    46,    57,     0,     0,     0,    58,
     723,     0,  -396,  -396,    50,  -396,  -396,  -396,  -396,  -396,
    -396,  -396,  -396,  -396,  -396,  -396,  -396,  -396,     0,     0,
       0,  -396,     0,     0,     0,     0,     0,     0,     0,  -396,
    -396,  -396,     0,  -396,     0,     0,  -396,  -396,  -396,  -396,
    -396,  -396,     0,  -396,  -396,  -396,    54,  -396,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,   139,
       0,    57,     0,     0,     0,    58,   977,     4,     5,     6,
       7,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,    25,    26,    27,    28,    29,    30,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     4,     5,     6,     7,     0,     0,     0,
     829,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,    52,    53,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,    56,    46,    57,     0,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,   830,   831,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,    57,     0,     0,     0,    58,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,   115,    46,   116,     0,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,   139,    46,    57,     0,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,    57,
     177,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,    57,   241,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,    57,   250,     0,     0,    58,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,   314,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,     0,    46,    57,   257,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,     0,    46,    57,     0,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,   179,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,    57,
       0,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,    57,   497,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,    57,   543,     0,     0,    58,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,     0,    46,    57,   546,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,   553,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,     0,    46,    57,   551,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     4,     5,   428,
       7,     0,     0,     0,   114,   107,   108,   109,   429,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,    57,
     554,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,    57,   177,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
     553,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,    57,   673,     0,     0,    58,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,     0,    46,    57,   675,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,     0,    46,    57,   862,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,   125,
       0,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,   127,     0,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,   129,     0,     0,     0,    58,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,     0,    46,   131,     0,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,     0,    46,    57,     0,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,   146,
       0,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,   148,     0,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,    54,    33,     0,     0,
       0,     0,     0,    55,     0,    41,    42,    43,     0,     0,
      46,    57,     0,     0,     0,   162,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     4,     5,   428,     7,     0,
       0,     0,   114,   107,   108,   109,   429,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,     0,    54,    33,     0,     0,     0,     0,     0,    55,
       0,    41,    42,    43,     0,     0,    46,   426,     0,     0,
       0,    58,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,     0,    54,    33,
       0,     0,     0,     0,     0,    55,     0,    41,    42,    43,
       0,     0,    46,   430,     0,     0,     0,    58,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,    54,    33,     0,     0,     0,     0,
       0,    55,     0,    41,    42,    43,     0,     0,    46,   573,
       0,     0,     0,    58,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     106,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    33,     0,     0,     0,     0,     0,    55,     0,    41,
      42,    43,     0,     0,    46,   596,     0,     0,     0,    58,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   205,   593,   457,    10,    15,    14,    14,    14,
     271,    26,    99,    26,   743,    22,    37,    38,   105,    14,
      22,    23,    24,    25,    26,    72,   123,    29,    30,   116,
      32,   327,    75,    35,    36,    10,    10,    75,    10,    14,
      14,    43,    14,    26,    46,    54,    55,    26,    26,    14,
      10,   107,    10,    10,    14,    57,    14,    14,    14,    75,
      43,    72,    14,     0,    43,    43,    31,    62,   111,   125,
      22,   118,    14,   111,   121,    56,    57,    10,   105,    60,
      22,    69,    70,    71,    14,    75,   118,    17,   125,   121,
      92,    93,    94,    95,    96,    97,    73,    74,   100,   103,
     102,   103,    56,    57,   120,   126,    60,   118,   105,   125,
     121,    63,   125,    10,   116,   119,   120,   122,   125,   125,
      12,    63,   712,   125,   121,   127,   855,   129,    58,   131,
     120,    14,    65,    66,   111,   125,   119,   120,   126,    26,
     119,   119,    12,   439,   146,   126,   148,   122,   122,   151,
     122,   153,   154,   105,   156,   157,    43,   159,   160,   161,
     162,   163,   122,   105,   122,   122,   119,   119,    65,    66,
     899,    14,   126,   126,   903,    58,   125,   119,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   107,   190,   191,
     192,   193,   194,   195,   196,   197,   283,   105,   494,   495,
       2,   288,    14,   218,   219,   125,   467,   294,    14,    73,
      74,   119,    93,    94,   943,   224,   225,   226,   227,   228,
      14,    14,   951,   107,   105,    27,    28,   107,    22,    22,
     107,   492,   119,   120,    10,    37,    38,   966,    14,   126,
     121,   125,    44,    45,    26,   125,   333,   111,   125,    26,
     252,   105,   254,   121,   256,   105,   709,   125,    10,   261,
     262,    43,   264,    10,   266,   119,    43,   996,    67,    63,
      63,   121,   101,   275,   276,   277,   278,   279,   280,   281,
     105,   296,   297,   285,   286,   114,   115,   116,   878,   105,
     105,   119,   119,    10,    11,    12,   121,    14,   126,   126,
     299,   303,   304,   119,   119,   307,    26,   309,   110,    29,
      30,   105,   105,   400,    31,   576,    36,   100,    35,    39,
      40,   408,   102,    43,    44,   119,   119,   414,   130,   416,
     132,   133,   105,   120,    26,   122,   539,   119,   340,   341,
     342,   343,   119,   120,   126,   147,   119,   149,   609,   126,
     105,    43,   119,     5,   104,    14,   443,    26,    17,   126,
      29,    30,    14,    14,   119,   452,   119,    36,   119,    26,
      39,    40,    29,    30,    43,   126,   637,   638,   180,    36,
     118,    26,    39,    40,    29,    30,    43,    44,    10,   119,
     118,    36,    14,   119,    39,    40,   126,    26,    43,   119,
      29,    30,    26,   112,   113,   407,   126,    36,   121,   119,
      39,    40,   125,   119,    43,    44,   126,   121,   119,    43,
     126,   125,   119,   119,   426,   126,    14,   119,   430,   126,
     126,   433,    10,   435,   126,   437,    14,   119,   119,   441,
     442,   119,   119,   704,   126,   126,   119,   120,   126,   126,
     119,   120,   119,   540,   541,    14,   458,   126,   119,   126,
     123,   119,   119,   119,   119,   126,   119,   469,   126,   126,
     126,   126,   121,   126,   119,   120,   119,   119,   565,   481,
     119,   126,   119,   126,   126,   119,   119,   126,    14,   126,
     119,   752,   753,   126,   119,   119,   120,   126,   500,   501,
     502,   126,   126,   119,   119,   121,   508,   509,   119,   511,
     512,   126,   514,   515,   516,   517,   518,   519,   520,   521,
     119,   119,   324,    26,   119,   119,    29,    30,   126,   105,
      10,   126,   126,    36,    14,   105,    39,    40,   834,    26,
      43,   837,    29,    30,   119,   125,   548,   549,   550,    36,
     119,   126,    39,    40,   121,   119,    43,   126,   125,    27,
      28,   563,   126,    26,   566,   567,    29,    30,   121,    53,
     121,   573,   125,    36,   125,    26,    39,    40,    29,    30,
      43,    44,   125,   119,   119,    36,   109,   110,    39,    40,
     126,   126,    43,    44,   596,    76,    77,    78,    79,    10,
      11,    12,   121,   690,   119,    86,    87,    88,   610,   611,
     612,   126,   119,   119,   701,   617,   119,   120,   119,   126,
     126,   119,   119,   126,   119,   627,   107,   108,   126,   925,
     126,   126,   119,   120,    80,    81,    82,    83,    84,    85,
     119,     7,     8,     9,   119,   120,    92,   126,   650,   651,
     652,   653,   654,   655,   456,   457,   119,   119,   460,   461,
     462,   125,   119,   109,   110,   119,   119,   469,   119,   126,
      14,   758,   126,   126,   119,   477,   478,   479,   215,   216,
     217,   126,   119,    91,   686,    93,    94,    95,    96,    97,
      98,    99,   119,   119,   119,   120,   119,   105,    10,   126,
     126,    13,    14,    15,    16,   120,    10,   122,   795,    13,
     118,    15,    16,   121,   120,   120,   122,   122,   120,   120,
     122,   122,   119,   725,   726,   727,   728,   729,   730,   731,
     732,   119,     3,     4,     5,     6,   119,   120,   740,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     120,   120,   122,   122,   119,   757,   120,   120,   122,   122,
      31,    32,   120,   120,   122,   122,   120,   120,   122,   122,
      41,   120,   120,   122,   122,   120,   126,   122,    49,    50,
      51,   124,   120,    54,   122,   108,   120,   120,   122,   122,
     119,   793,   794,    64,    91,   597,    93,    94,    95,    96,
      97,    98,    99,   120,   120,   122,   122,   121,   105,   811,
     812,   813,   120,   120,   122,   122,   618,   120,   120,   122,
     122,   118,    27,    28,   121,   120,   828,   122,   120,   120,
     122,   122,   120,    14,   122,   106,   835,   120,    14,   122,
     842,   120,   113,   122,   120,   847,   122,   120,   119,   122,
     121,   120,   119,   122,   125,   126,   120,   105,   122,   119,
     120,   105,   864,   865,   199,   200,   201,   202,   203,   204,
     872,   206,   207,   208,   209,   210,   211,   212,   213,   119,
     120,    27,    28,   119,   120,    27,    28,    42,    43,   121,
     222,   223,    10,    14,    14,    27,    10,   119,   119,   119,
     902,   119,   119,   126,   123,   121,   121,   709,   105,   119,
     119,   713,   714,   715,   916,   119,   119,   119,   119,   921,
     119,   119,    89,   119,   119,   119,   119,   119,   119,   119,
     119,   111,   734,   121,   121,    14,   738,   105,   940,   105,
     105,   124,   126,   945,   946,   947,   108,   119,   119,    72,
     119,   125,    14,   126,    14,   119,   119,   108,   119,   119,
     119,   119,   119,   119,   108,   119,   119,   119,   105,    14,
     119,   119,   119,    10,   119,    10,   126,   979,   980,   981,
     982,   983,   984,   985,   986,   119,   119,    10,    10,    10,
     119,   119,   119,    27,    14,    56,   998,   999,  1000,    14,
     802,   803,     3,     4,     5,     6,    14,   124,   119,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   126,   119,    29,    30,
      31,    32,    33,    34,    35,    36,   119,   119,    14,   119,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,   119,    72,   126,    59,    60,
      72,   119,   119,    64,   119,   119,   119,    68,    14,     7,
       7,     7,   105,   105,    72,    10,    10,    10,    10,   122,
      67,   119,   126,    14,    14,    14,    14,   126,   119,   119,
     119,   119,    93,    94,   119,   119,   898,    14,   125,    14,
     902,   119,   119,    14,    14,   106,   119,   126,   119,   119,
     119,   490,   113,   119,   119,   119,   119,   119,   119,   119,
     121,   940,   119,   119,   125,   126,   220,     3,     4,     5,
       6,   122,   541,   935,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   221,   430,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,   997,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,   125,
     126,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,   119,    54,   121,
      -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,   119,    54,   121,    -1,    -1,    -1,   125,
     126,    -1,    73,    74,    64,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,   106,   118,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,    -1,    -1,    -1,   125,   126,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,   119,    54,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,   119,    54,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,   119,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,    -1,    54,   121,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    67,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,    -1,    54,   121,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,    -1,    54,   121,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
     122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,    -1,    54,   121,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,    -1,    54,   121,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    49,    50,    51,    -1,    -1,
      54,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    49,    50,    51,    -1,    -1,    54,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    41,
      -1,    -1,    -1,    -1,    -1,   113,    -1,    49,    50,    51,
      -1,    -1,    54,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    49,    50,    51,    -1,    -1,    54,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,    49,
      50,    51,    -1,    -1,    54,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125
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
     163,   164,   165,   166,   167,   168,   169,   170,   172,    91,
      93,    94,    95,    96,    97,    98,    99,   105,   118,   121,
     123,   107,   125,   105,   118,   121,    10,    11,    12,    13,
      31,   155,   167,    14,    10,   119,   121,   157,   171,    10,
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
      14,   122,   171,   157,    14,   157,   157,    14,    17,    58,
     122,   171,   123,   107,   125,   119,   105,   122,   157,   171,
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
     166,   167,   167,   167,   167,   167,    14,    14,    17,   119,
     121,   119,   119,   119,   119,   119,   119,   105,   121,   122,
     124,   108,   126,   119,   121,    14,   121,   122,   157,    14,
     157,   157,   119,   122,   157,   157,   121,   157,     5,    14,
     121,   153,   157,   105,   119,   105,   119,   105,   119,   121,
     144,   105,   105,   120,   157,   157,   157,   157,   157,   157,
     157,   171,   120,   157,   157,   171,   122,    27,    28,   132,
     122,    10,   171,   163,   163,    10,    14,   122,   154,   129,
     157,   157,   141,    14,   157,    14,   157,   122,    27,   122,
      10,   119,   119,   119,   119,   119,   126,   155,    14,    22,
     125,   149,   122,   154,   121,   121,   151,   122,   171,   118,
     123,   125,   105,   157,   157,   157,   126,   157,    26,    43,
     126,    26,    43,   126,    26,    29,    30,    36,    39,    40,
      43,    44,   126,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,    89,
     121,   121,    14,   122,   171,   157,   122,   171,   105,   105,
     105,   122,   171,    14,   122,   152,   171,   124,   108,   126,
     119,   119,   157,   103,   119,   120,    72,    72,   153,   119,
     120,   119,   157,   121,   157,   157,   122,   154,   157,   157,
     171,   119,   119,   119,   119,   119,   119,   119,   119,   171,
     119,   119,   119,   155,   131,   155,   121,   157,   155,   155,
      75,   155,    75,    75,    10,    65,    66,   144,   120,   122,
      24,    25,    34,   126,   157,   119,   119,    37,    38,   126,
     108,   126,   108,   126,   155,   155,   155,   105,   157,    14,
      14,   149,    56,    57,    60,   126,   144,   122,   122,   154,
     154,    69,    70,    71,   126,   122,    14,   157,   157,   157,
      26,    30,    36,    39,    40,    43,   126,   126,   126,   126,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   158,   122,   171,   122,   152,   171,   119,   122,
     119,   122,   157,   157,   157,   122,    72,   120,   122,   122,
     120,   157,   171,   157,   157,   122,     5,    14,   119,   157,
     119,   120,   119,   144,   122,   119,   119,   122,   122,    27,
     132,   157,   155,    10,    10,    10,    10,    10,    10,    14,
     144,   157,   157,   126,   157,    26,    29,    30,    36,    39,
      40,    43,    44,   126,   157,   155,   119,   119,    27,   157,
     119,   126,    14,    58,    10,    13,    14,    15,    16,    56,
     144,   144,   122,   122,    14,    14,    14,   105,   121,   124,
     126,   126,   157,   157,   157,   157,   157,   157,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     122,   122,   122,   119,   119,   119,   119,   157,    14,   171,
     119,   119,   119,    72,    72,   120,   171,   144,   119,   119,
     131,   155,   122,    27,   132,   155,   155,   155,    10,    65,
      66,    26,    30,    43,   126,   126,   126,   157,   157,   157,
     157,   157,   157,   157,   157,   155,   126,   155,   119,    10,
      93,    94,   138,   157,   121,   125,   145,   121,   145,    14,
      22,    63,   105,   119,    14,    22,    63,   105,   119,    10,
      13,    15,    16,   119,   119,    14,   144,   144,     7,     7,
       7,   157,   122,   171,   105,   105,   126,   126,   126,   126,
     126,   126,    72,   122,   157,   157,   171,   119,   155,   155,
      10,    10,   157,   157,   157,   126,   126,   126,   126,   126,
     126,   126,   126,   157,    93,    94,    10,    10,   122,   122,
     154,    67,   129,   122,   154,    14,    14,   119,   157,    14,
      14,   119,   157,    14,    22,    63,   105,   119,    14,    22,
      63,   105,   119,   119,   119,   121,   145,   126,   126,   122,
     157,   157,   157,   119,   122,    27,   132,   126,   126,   126,
     119,   155,   145,   122,   126,    24,    25,    34,   126,   157,
     145,   122,   119,   119,   119,   119,   119,   119,    14,    14,
     119,   157,    14,    14,   119,   157,   122,   154,   126,   126,
     126,   119,   155,   138,   145,   157,   157,   126,   157,    26,
      29,    30,    36,    39,    40,    43,    44,   126,   145,   119,
     119,   119,   119,   119,   119,   145,   122,   122,    26,    30,
      43,   126,   126,   126,   157,   157,   157,   157,   157,   157,
     157,   157,   145,   155,   157,   157,   157,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126
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
     167,   168,   168,   169,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     171,   171,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172
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
       1,     3,     1,     1,     4,     3,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     5,     5,     1,     1,
       1,     1,     2,     3,     4,     5,     1,     2
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
#line 2670 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 334 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2676 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 335 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2682 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 339 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2688 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 341 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2694 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 343 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2700 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 345 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2706 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 347 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2712 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 349 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2720 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 353 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2727 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 356 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2735 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 360 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2741 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 362 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2747 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 364 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2753 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 366 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2759 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 368 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2765 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 370 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2773 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 374 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2781 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2787 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 380 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2793 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 382 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2799 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 384 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2805 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 386 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2811 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 388 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2817 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 390 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2823 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 392 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2829 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 394 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2835 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 396 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2841 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 398 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2847 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 400 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2853 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 402 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2859 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 404 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2865 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 406 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2872 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 409 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2879 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 412 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2885 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 414 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2892 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 417 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2899 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 420 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2905 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 422 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2911 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 424 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2917 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 426 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2923 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 428 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2932 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 433 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2940 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 437 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2948 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 441 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2957 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 446 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2965 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 449 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2971 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 451 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2977 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 453 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2983 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 455 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2989 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 457 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2995 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 459 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3002 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 462 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3008 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 464 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3014 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 466 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3020 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 468 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3027 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 471 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3034 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 474 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3041 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 477 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3048 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 480 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3055 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 483 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3061 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 485 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3067 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3073 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 489 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3079 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 491 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3086 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 494 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3092 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 496 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3098 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 498 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3104 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 500 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3111 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 503 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3118 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 506 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3126 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 510 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3133 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 513 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3139 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 515 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3145 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 517 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3151 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 519 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3158 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 522 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3165 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 525 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3172 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 528 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3179 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 531 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3186 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 534 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3193 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 537 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3200 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 540 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3207 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 543 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3214 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 546 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3221 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 549 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3228 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 551 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3234 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 552 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3240 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 553 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3246 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 554 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3252 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 555 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3258 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3264 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 558 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3270 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 560 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3276 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 562 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3282 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 564 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3288 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 565 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3294 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 566 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3300 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 567 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3306 "raku.tab.c"
    break;

  case 101: /* stmt: loop_stmt  */
#line 568 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3312 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST ';'  */
#line 569 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3318 "raku.tab.c"
    break;

  case 103: /* stmt: KW_NEXT ';'  */
#line 570 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3324 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST KW_IF expr ';'  */
#line 572 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3330 "raku.tab.c"
    break;

  case 105: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 574 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3336 "raku.tab.c"
    break;

  case 106: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 576 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3342 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3348 "raku.tab.c"
    break;

  case 108: /* stmt: sub_decl  */
#line 579 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3354 "raku.tab.c"
    break;

  case 109: /* stmt: class_decl  */
#line 580 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3360 "raku.tab.c"
    break;

  case 110: /* stmt: role_decl  */
#line 581 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3366 "raku.tab.c"
    break;

  case 111: /* stmt: grammar_decl  */
#line 582 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3372 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3378 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 588 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3384 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 590 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3390 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3396 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3402 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 596 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3408 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block elsif_tail  */
#line 598 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3414 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 600 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3420 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3426 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3432 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3438 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3450 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 614 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3456 "raku.tab.c"
    break;

  case 126: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 618 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3462 "raku.tab.c"
    break;

  case 127: /* while_stmt: KW_WHILE expr block  */
#line 620 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3468 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3474 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3480 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS expr block  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3486 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 630 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3492 "raku.tab.c"
    break;

  case 132: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 634 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3498 "raku.tab.c"
    break;

  case 133: /* until_stmt: KW_UNTIL expr block  */
#line 636 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3504 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3510 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 644 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3516 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 646 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3522 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 648 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3528 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 651 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3534 "raku.tab.c"
    break;

  case 139: /* loop_incr: VAR_SCALAR OP_INC  */
#line 652 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),1); }
#line 3540 "raku.tab.c"
    break;

  case 140: /* loop_incr: VAR_SCALAR OP_DEC  */
#line 653 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),0); }
#line 3546 "raku.tab.c"
    break;

  case 141: /* loop_incr: OP_INC VAR_SCALAR  */
#line 654 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 3552 "raku.tab.c"
    break;

  case 142: /* loop_incr: OP_DEC VAR_SCALAR  */
#line 655 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 3558 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 659 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3568 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 665 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3578 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 671 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3587 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 676 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3597 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 682 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3606 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 687 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3613 "raku.tab.c"
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
#line 3625 "raku.tab.c"
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
#line 3638 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 710 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3644 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 712 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3652 "raku.tab.c"
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
#line 3664 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 726 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3674 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 732 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3684 "raku.tab.c"
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
#line 3696 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 746 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3706 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 752 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3716 "raku.tab.c"
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
#line 3729 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 767 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3740 "raku.tab.c"
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
#line 3753 "raku.tab.c"
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
#line 3766 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 792 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3777 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 801 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3788 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 808 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3797 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 814 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3803 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 816 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3810 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 819 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3817 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 822 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3823 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 824 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3829 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 826 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3835 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3841 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 830 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3847 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 832 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3853 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 834 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3859 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 836 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3866 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 839 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3872 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 841 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3878 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 843 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3884 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 845 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3891 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 848 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3898 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 851 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3906 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 856 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3912 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 857 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3918 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 859 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3925 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 862 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3932 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 865 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3938 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 867 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3944 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 869 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3950 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 871 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3956 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 873 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3962 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 875 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3968 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 877 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3974 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 879 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3981 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 882 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3987 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 884 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3993 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 886 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3999 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 888 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4006 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 891 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4013 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 894 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4021 "raku.tab.c"
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
#line 4039 "raku.tab.c"
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
#line 4056 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 930 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4062 "raku.tab.c"
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
#line 4078 "raku.tab.c"
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
#line 4094 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 957 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4100 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 959 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4107 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 962 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4114 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 965 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4121 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 968 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4128 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 971 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4135 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 974 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4142 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 977 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4149 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 980 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4156 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 983 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4163 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 986 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4170 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 989 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4177 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 992 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4184 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 995 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4195 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1002 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4206 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1009 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4217 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1016 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4228 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1023 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4236 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1027 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4244 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1031 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4252 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1035 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4260 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1039 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4271 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1046 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4282 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1053 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4293 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1060 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4304 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
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
#line 4318 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 1077 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4330 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 1085 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4342 "raku.tab.c"
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
#line 4355 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1102 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4366 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1109 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4377 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
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
#line 4391 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 1126 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4403 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 1134 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4415 "raku.tab.c"
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
#line 4432 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1158 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4438 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1160 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4447 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1165 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4456 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1170 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4465 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1177 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4473 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1181 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4481 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1187 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4487 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1189 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4493 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1191 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4499 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1193 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4505 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1196 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4511 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1197 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4517 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1198 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4523 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1199 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4529 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1200 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4535 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1201 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4541 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1202 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4547 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1203 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4553 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list '}'  */
#line 1206 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4559 "raku.tab.c"
    break;

  case 260: /* block: '{' YADA '}'  */
#line 1207 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4565 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr '}'  */
#line 1209 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1211 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4577 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1213 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1215 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4589 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1217 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4595 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1219 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4602 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1222 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1224 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4614 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1226 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4620 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1228 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4626 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1230 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4633 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1233 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4640 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1236 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4648 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1240 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4655 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1243 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4662 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1246 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4669 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1249 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4675 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1251 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4683 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1255 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4690 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1258 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4697 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1261 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4704 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1264 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4711 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1267 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4718 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1270 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4724 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1272 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4730 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1274 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4736 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_LAST '}'  */
#line 1276 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4742 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1278 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4748 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1280 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4754 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1282 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4760 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1284 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4766 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1286 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4772 "raku.tab.c"
    break;

  case 293: /* closure: '{' expr '}'  */
#line 1289 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4778 "raku.tab.c"
    break;

  case 294: /* expr: VAR_SCALAR '=' expr  */
#line 1292 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4784 "raku.tab.c"
    break;

  case 295: /* expr: KW_GATHER block  */
#line 1293 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4794 "raku.tab.c"
    break;

  case 296: /* expr: tern_expr  */
#line 1298 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4800 "raku.tab.c"
    break;

  case 297: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1302 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4806 "raku.tab.c"
    break;

  case 298: /* tern_expr: cmp_expr  */
#line 1303 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4812 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1306 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4818 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1307 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4824 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1308 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4830 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1309 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4836 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1310 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4842 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1311 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4848 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1312 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4854 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1313 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4860 "raku.tab.c"
    break;

  case 307: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1314 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4866 "raku.tab.c"
    break;

  case 308: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1315 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4872 "raku.tab.c"
    break;

  case 309: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1316 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4878 "raku.tab.c"
    break;

  case 310: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1317 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4884 "raku.tab.c"
    break;

  case 311: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1318 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4890 "raku.tab.c"
    break;

  case 312: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1319 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4896 "raku.tab.c"
    break;

  case 313: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1321 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4906 "raku.tab.c"
    break;

  case 314: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1327 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4916 "raku.tab.c"
    break;

  case 315: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1333 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4926 "raku.tab.c"
    break;

  case 316: /* cmp_expr: jct_expr  */
#line 1338 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4932 "raku.tab.c"
    break;

  case 317: /* jct_expr: jct_expr '|' range_expr  */
#line 1341 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4938 "raku.tab.c"
    break;

  case 318: /* jct_expr: jct_expr '&' range_expr  */
#line 1342 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4944 "raku.tab.c"
    break;

  case 319: /* jct_expr: dor_expr  */
#line 1343 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4950 "raku.tab.c"
    break;

  case 320: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1347 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4956 "raku.tab.c"
    break;

  case 321: /* dor_expr: range_expr  */
#line 1348 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4962 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1351 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4968 "raku.tab.c"
    break;

  case 323: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1352 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4974 "raku.tab.c"
    break;

  case 324: /* range_expr: add_expr  */
#line 1353 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4980 "raku.tab.c"
    break;

  case 325: /* add_expr: add_expr '~' repl_expr  */
#line 1356 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4986 "raku.tab.c"
    break;

  case 326: /* add_expr: repl_expr  */
#line 1357 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4992 "raku.tab.c"
    break;

  case 327: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1360 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4998 "raku.tab.c"
    break;

  case 328: /* repl_expr: addsub_expr  */
#line 1361 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5004 "raku.tab.c"
    break;

  case 329: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1364 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5010 "raku.tab.c"
    break;

  case 330: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1365 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5016 "raku.tab.c"
    break;

  case 331: /* addsub_expr: mul_expr  */
#line 1366 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5022 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '*' unary_expr  */
#line 1369 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5028 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr '/' unary_expr  */
#line 1370 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5034 "raku.tab.c"
    break;

  case 334: /* mul_expr: mul_expr '%' unary_expr  */
#line 1371 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5040 "raku.tab.c"
    break;

  case 335: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1372 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5046 "raku.tab.c"
    break;

  case 336: /* mul_expr: unary_expr  */
#line 1373 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5052 "raku.tab.c"
    break;

  case 337: /* unary_expr: '-' unary_expr  */
#line 1376 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5058 "raku.tab.c"
    break;

  case 338: /* unary_expr: '!' unary_expr  */
#line 1377 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5064 "raku.tab.c"
    break;

  case 339: /* unary_expr: CARET unary_expr  */
#line 1378 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5070 "raku.tab.c"
    break;

  case 340: /* unary_expr: pow_expr  */
#line 1379 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5076 "raku.tab.c"
    break;

  case 341: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1382 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5082 "raku.tab.c"
    break;

  case 342: /* pow_expr: postfix_expr  */
#line 1383 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5088 "raku.tab.c"
    break;

  case 343: /* postfix_expr: call_expr  */
#line 1385 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5094 "raku.tab.c"
    break;

  case 344: /* call_expr: IDENT '(' arg_list ')'  */
#line 1388 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5103 "raku.tab.c"
    break;

  case 345: /* call_expr: IDENT '(' ')'  */
#line 1392 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5109 "raku.tab.c"
    break;

  case 346: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1394 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5118 "raku.tab.c"
    break;

  case 347: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1399 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5124 "raku.tab.c"
    break;

  case 348: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1401 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5134 "raku.tab.c"
    break;

  case 349: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1407 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5144 "raku.tab.c"
    break;

  case 350: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1413 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5152 "raku.tab.c"
    break;

  case 351: /* call_expr: IDENT '.' KW_NEW  */
#line 1417 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5160 "raku.tab.c"
    break;

  case 352: /* call_expr: IDENT '.' IDENT  */
#line 1421 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5169 "raku.tab.c"
    break;

  case 353: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1426 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5180 "raku.tab.c"
    break;

  case 354: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1433 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5189 "raku.tab.c"
    break;

  case 355: /* call_expr: IDENT '.' CARET IDENT  */
#line 1438 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5199 "raku.tab.c"
    break;

  case 356: /* call_expr: atom '.' CARET IDENT  */
#line 1444 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5209 "raku.tab.c"
    break;

  case 357: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1450 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5220 "raku.tab.c"
    break;

  case 358: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1457 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5231 "raku.tab.c"
    break;

  case 359: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1464 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5240 "raku.tab.c"
    break;

  case 360: /* call_expr: atom '.' IDENT  */
#line 1469 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5249 "raku.tab.c"
    break;

  case 361: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1474 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5260 "raku.tab.c"
    break;

  case 362: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1481 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5269 "raku.tab.c"
    break;

  case 363: /* call_expr: call_expr '.' IDENT  */
#line 1486 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5278 "raku.tab.c"
    break;

  case 364: /* call_expr: KW_DIE expr  */
#line 1491 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5284 "raku.tab.c"
    break;

  case 365: /* call_expr: KW_MAP closure expr  */
#line 1493 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5290 "raku.tab.c"
    break;

  case 366: /* call_expr: KW_GREP closure expr  */
#line 1495 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5296 "raku.tab.c"
    break;

  case 367: /* call_expr: KW_SORT expr  */
#line 1497 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5302 "raku.tab.c"
    break;

  case 368: /* call_expr: KW_SORT closure expr  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5308 "raku.tab.c"
    break;

  case 369: /* call_expr: atom  */
#line 1500 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5314 "raku.tab.c"
    break;

  case 370: /* arg_list: expr  */
#line 1503 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5320 "raku.tab.c"
    break;

  case 371: /* arg_list: arg_list ',' expr  */
#line 1504 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5326 "raku.tab.c"
    break;

  case 372: /* atom: LIT_INT  */
#line 1507 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5332 "raku.tab.c"
    break;

  case 373: /* atom: LIT_FLOAT  */
#line 1508 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5338 "raku.tab.c"
    break;

  case 374: /* atom: LIT_STR  */
#line 1509 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5344 "raku.tab.c"
    break;

  case 375: /* atom: WORDLIST  */
#line 1511 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5356 "raku.tab.c"
    break;

  case 376: /* atom: LIT_INTERP_STR  */
#line 1518 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5362 "raku.tab.c"
    break;

  case 377: /* atom: VAR_SCALAR  */
#line 1519 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5368 "raku.tab.c"
    break;

  case 378: /* atom: VAR_ARRAY  */
#line 1520 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5374 "raku.tab.c"
    break;

  case 379: /* atom: VAR_HASH  */
#line 1521 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5380 "raku.tab.c"
    break;

  case 380: /* atom: VAR_CAPTURE  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5388 "raku.tab.c"
    break;

  case 381: /* atom: VAR_FH  */
#line 1527 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5396 "raku.tab.c"
    break;

  case 382: /* atom: VAR_NAMED_CAPTURE  */
#line 1531 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5403 "raku.tab.c"
    break;

  case 383: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1534 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5409 "raku.tab.c"
    break;

  case 384: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1536 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5415 "raku.tab.c"
    break;

  case 385: /* atom: VAR_HASH '{' expr '}'  */
#line 1538 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5421 "raku.tab.c"
    break;

  case 386: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1540 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5427 "raku.tab.c"
    break;

  case 387: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1542 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5433 "raku.tab.c"
    break;

  case 388: /* atom: IDENT  */
#line 1543 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5439 "raku.tab.c"
    break;

  case 389: /* atom: VAR_TWIGIL  */
#line 1545 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5447 "raku.tab.c"
    break;

  case 390: /* atom: VAR_ARRAY_TWIGIL  */
#line 1549 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5455 "raku.tab.c"
    break;

  case 391: /* atom: VAR_HASH_TWIGIL  */
#line 1553 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5463 "raku.tab.c"
    break;

  case 392: /* atom: '(' ')'  */
#line 1556 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5469 "raku.tab.c"
    break;

  case 393: /* atom: '(' expr ')'  */
#line 1557 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5475 "raku.tab.c"
    break;

  case 394: /* atom: '(' expr ',' ')'  */
#line 1559 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5481 "raku.tab.c"
    break;

  case 395: /* atom: '(' expr ',' arg_list ')'  */
#line 1561 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5488 "raku.tab.c"
    break;

  case 396: /* atom: block  */
#line 1563 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5494 "raku.tab.c"
    break;

  case 397: /* atom: KW_SUB block  */
#line 1564 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5500 "raku.tab.c"
    break;


#line 5504 "raku.tab.c"

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

#line 1566 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
