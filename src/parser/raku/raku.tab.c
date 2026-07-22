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

#line 305 "raku.tab.c"

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
  YYSYMBOL_for_stmt = 138,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 139,               /* given_stmt  */
  YYSYMBOL_when_list = 140,                /* when_list  */
  YYSYMBOL_sub_decl = 141,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 142,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 143,                 /* sub_body  */
  YYSYMBOL_method_body = 144,              /* method_body  */
  YYSYMBOL_class_decl = 145,               /* class_decl  */
  YYSYMBOL_role_decl = 146,                /* role_decl  */
  YYSYMBOL_is_clauses = 147,               /* is_clauses  */
  YYSYMBOL_class_body_list = 148,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 149,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 150,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 151,           /* named_arg_list  */
  YYSYMBOL_pair_list = 152,                /* pair_list  */
  YYSYMBOL_param_list = 153,               /* param_list  */
  YYSYMBOL_block = 154,                    /* block  */
  YYSYMBOL_closure = 155,                  /* closure  */
  YYSYMBOL_expr = 156,                     /* expr  */
  YYSYMBOL_tern_expr = 157,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 158,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 159,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 160,                 /* dor_expr  */
  YYSYMBOL_range_expr = 161,               /* range_expr  */
  YYSYMBOL_add_expr = 162,                 /* add_expr  */
  YYSYMBOL_repl_expr = 163,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 164,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 165,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 166,               /* unary_expr  */
  YYSYMBOL_pow_expr = 167,                 /* pow_expr  */
  YYSYMBOL_postfix_expr = 168,             /* postfix_expr  */
  YYSYMBOL_call_expr = 169,                /* call_expr  */
  YYSYMBOL_arg_list = 170,                 /* arg_list  */
  YYSYMBOL_atom = 171                      /* atom  */
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
#define YYLAST   3462

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  390
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1002

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
       0,   314,   314,   325,   326,   329,   331,   333,   335,   337,
     339,   343,   346,   350,   352,   354,   356,   358,   360,   364,
     368,   370,   372,   374,   376,   378,   380,   382,   384,   386,
     388,   390,   392,   394,   396,   399,   402,   404,   407,   410,
     412,   414,   416,   418,   423,   427,   431,   436,   440,   441,
     443,   445,   447,   449,   452,   454,   456,   458,   461,   464,
     467,   470,   473,   475,   477,   479,   481,   484,   486,   488,
     490,   493,   496,   500,   503,   505,   507,   509,   512,   515,
     518,   521,   524,   527,   530,   533,   536,   539,   542,   543,
     544,   545,   546,   547,   548,   550,   552,   554,   556,   557,
     558,   559,   560,   561,   562,   564,   566,   568,   570,   571,
     572,   573,   576,   578,   580,   582,   584,   586,   588,   590,
     594,   596,   598,   600,   602,   604,   608,   610,   614,   616,
     618,   620,   624,   626,   630,   634,   638,   644,   650,   655,
     661,   666,   671,   679,   690,   691,   697,   705,   711,   717,
     725,   731,   737,   746,   753,   762,   771,   780,   787,   794,
     795,   798,   801,   803,   805,   807,   809,   811,   813,   815,
     818,   820,   822,   824,   827,   830,   836,   837,   838,   841,
     844,   846,   848,   850,   852,   854,   856,   858,   861,   863,
     865,   867,   870,   873,   879,   895,   910,   911,   923,   937,
     938,   941,   944,   947,   950,   953,   956,   959,   962,   965,
     968,   971,   974,   981,   988,   995,  1002,  1006,  1010,  1014,
    1018,  1025,  1032,  1039,  1046,  1056,  1064,  1072,  1081,  1088,
    1095,  1105,  1113,  1123,  1138,  1139,  1144,  1149,  1156,  1160,
    1166,  1168,  1170,  1172,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1183,  1186,  1187,  1188,  1190,  1192,  1194,  1196,  1198,
    1201,  1203,  1205,  1207,  1209,  1212,  1215,  1219,  1222,  1225,
    1228,  1230,  1234,  1237,  1240,  1243,  1246,  1249,  1251,  1253,
    1255,  1257,  1259,  1261,  1263,  1265,  1269,  1272,  1273,  1278,
    1281,  1283,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1306,  1312,  1318,
    1321,  1322,  1323,  1326,  1328,  1331,  1332,  1333,  1336,  1337,
    1340,  1341,  1344,  1345,  1346,  1349,  1350,  1351,  1352,  1353,
    1356,  1357,  1358,  1359,  1362,  1363,  1365,  1367,  1372,  1373,
    1378,  1380,  1386,  1392,  1396,  1400,  1405,  1412,  1417,  1423,
    1429,  1436,  1443,  1448,  1453,  1460,  1465,  1470,  1472,  1474,
    1476,  1478,  1480,  1483,  1484,  1487,  1488,  1489,  1490,  1498,
    1499,  1500,  1501,  1502,  1506,  1510,  1513,  1515,  1517,  1519,
    1521,  1523,  1524,  1528,  1532,  1536,  1537,  1538,  1540,  1543,
    1544
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
  "for_stmt", "given_stmt", "when_list", "sub_decl", "scalar_methcall",
  "sub_body", "method_body", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "cmp_expr", "jct_expr", "dor_expr", "range_expr", "add_expr",
  "repl_expr", "addsub_expr", "mul_expr", "unary_expr", "pow_expr",
  "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-631)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-390)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -631,   105,  1489,  -631,  -631,  -631,  -631,  -631,   585,   -98,
     -56,   -58,    55,  -631,  -631,   823,  -631,  -631,  -631,   101,
    1555,   447,  2611,  2677,  2743,  2809,  2875,    -6,    30,  2875,
    1621,   223,  2875,   156,   238,  2941,  3007,    30,    30,    22,
      33,   200,   200,  3073,    30,    30,  2875,   273,   306,    66,
    -631,   314,   370,   393,   823,   823,  -631,  1687,   250,  -631,
    -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,   518,
    -631,  -631,  -631,  1354,   538,  -631,   514,   406,   231,  -631,
      92,   232,   180,   240,  -631,  -631,   327,   294,   323,   431,
     239,   335,  2875,  2875,  2875,  2875,  2875,  2875,   463,  1753,
    2875,   503,  2875,  2875,   212,  1819,   339,   369,    13,  -631,
      30,  -631,  -631,   407,     3,  -631,  1885,  -631,   229,   -35,
     -20,   184,   676,   536,   309,  1687,   495,  1687,    20,  1687,
      30,  1687,    30,   -43,   173,   218,  -631,  -631,   409,  -631,
     452,   470,   570,   483,    97,   111,  1687,    30,  1687,    30,
    -631,  -631,  2875,  2875,  -631,  2875,  2875,  -631,  2875,  2875,
    2875,  1951,  2875,  -631,   581,  -631,  -631,  -631,   564,   572,
      25,   576,   520,   611,  -631,  -631,  -631,   110,   618,   982,
    2875,  2875,  2875,  2875,  2875,  2875,  2875,  2875,  -631,  2875,
    2875,  2875,  2875,  2875,  2875,  2875,  2875,  -631,   823,   823,
     823,   823,   823,   823,   823,   823,   823,   823,   823,   823,
     823,   823,   823,   702,   823,   823,   823,   823,   823,   823,
     823,   823,   823,   823,   823,   823,   823,   823,   690,   254,
     248,  -631,  -631,   633,   662,   693,   700,   705,   726,   139,
    -631,   429,   672,   679,   741,   752,   754,   872,   767,  -631,
     433,  2875,   875,  2875,  -631,  2875,   771,   110,   544,  -631,
    2875,  2875,  -631,  3139,  -631,  3205,  -631,   186,   189,   192,
     260,   786,   787,   578,  2875,  2875,  2875,  2875,  2875,  2875,
    2875,  -631,  2875,   592,  2875,  2875,  -631,  2875,   593,   453,
     619,  -631,   884,  2875,  -631,   823,   823,     0,  -631,  -631,
    -631,  -631,  2875,  2875,  -631,   881,  2875,   882,  2875,   620,
     870,   623,  -631,   781,   782,   783,   788,   780,  -631,  -631,
    -631,    30,    -7,  -631,    21,   789,   791,  -631,  -631,  -631,
    2017,  -631,  -631,   820,   785,   120,   818,  2611,  2677,  2875,
     744,    90,   214,  -631,   321,   790,   807,   808,   811,   812,
     813,   814,   815,   816,   821,   824,   826,   827,   828,   830,
     831,   387,   387,   387,   387,   387,   387,   848,   387,   387,
     387,   387,   387,   387,   387,   387,  -631,  -631,  -631,  -631,
    -631,  -631,   840,   840,   232,   180,   240,   240,  -631,  -631,
    -631,  -631,  -631,   841,   842,   925,  -631,  2083,  -631,  -631,
    -631,  -631,  -631,  -631,  2875,  2149,  -631,   837,   847,   856,
    -631,  2215,  -631,  2281,  -631,   843,   857,   838,  -631,  -631,
     849,  -631,   850,  1687,   136,   894,    -8,  2347,   420,   851,
    2875,  -631,  3271,  -631,  2875,  -631,    26,  -631,  2875,  2875,
    2017,   855,   858,   859,   860,   861,   862,   863,   464,  2017,
     864,   865,   478,    30,   -13,  3337,  -631,    30,    30,   -37,
     -15,    36,  -631,     7,   885,   631,  1109,   890,   900,    41,
     867,   895,   868,   896,    30,    30,    30,  -631,  -631,  -631,
    -631,  -631,  -631,   975,  1006,  -631,    11,   885,   645,    39,
     376,   196,  -631,   646,  1024,  2875,  2875,  2875,   348,   -17,
     143,  -631,   162,  2875,  2875,  -631,  2875,  2875,  -631,  2875,
    2875,  2875,  2875,  2875,  2875,  2875,  2875,  -631,  -631,  -631,
    -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,
    -631,  -631,  -631,  -631,   823,  2413,  2479,  -631,   920,   649,
     921,  -631,   650,  2875,  2875,  2875,  -631,   657,   102,  -631,
     658,   664,  -631,  -631,  -631,  -631,  -631,   668,  2875,  -631,
    2875,  2875,  2875,   677,  -631,   131,  -631,   924,  1687,   529,
     926,   885,   683,   928,   929,   684,  -631,  -631,  -631,  -631,
    -631,  -631,  -631,  -631,   687,  -631,  -631,  -631,   642,  -631,
    -631,  1687,    30,  -631,  -631,  1034,  -631,  1039,  1041,  -631,
    1042,  1043,  -631,   385,   885,  2611,  2677,  1299,  -631,   353,
    -631,  -631,  2875,    30,  -631,  -631,  -631,   935,   936,  1029,
    -631,  -631,  -631,  -631,    75,    48,   681,  1001,  -631,  -631,
     885,   885,   691,   694,  1044,  1045,  1046,  -631,  -631,   159,
     937,   938,   190,  2875,  2875,  2875,  2875,  2875,  2875,  -631,
    -631,  -631,  -631,   233,   244,   247,   249,   275,   279,   281,
     285,   289,   303,   307,   308,  -631,  -631,   695,  -631,   698,
     724,  -631,   943,  -631,  -631,   944,   946,   947,  -631,  2875,
    1053,  -631,  -631,  2017,   949,   654,  -631,  -631,   950,   998,
     999,  -631,   729,  -631,  2875,  -631,  -631,   885,  -631,  -631,
     953,   954,   -13,  -631,   736,   764,    30,    30,    30,  -631,
    -631,  -631,     9,  -631,   426,   176,  -631,   316,  2875,  2875,
    2875,  2875,  2875,  2875,  2875,  2875,  -631,    30,   948,  -631,
    -631,    30,  -631,   292,   293,    23,    44,   692,   958,   960,
    1066,  -631,  -631,   885,   885,  1074,  1075,  1076,  2875,  2545,
     979,   980,  -631,   320,   324,   325,   344,   357,   401,  -631,
    -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,
    -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  1014,
     739,  -631,  -631,  -631,  2875,  2875,  2017,   703,  -631,  -631,
    -631,  -631,  -631,    30,    30,  -631,  -631,  -631,  -631,  -631,
    1077,  1079,  2875,  2875,  2875,  -631,  -631,  -631,   411,   444,
     446,   450,   454,   460,   468,   469,  -631,  -631,  -631,    40,
    1023,  -631,    42,  -631,  1078,  1080,   972,  2875,  -631,  1082,
    1083,   974,  2875,  -631,    67,   112,   981,   983,  -631,  -631,
     350,  -631,  -631,  -631,  -631,  -631,   480,   973,   740,  2875,
    2875,  -631,  -631,  -631,  -631,  -631,  -631,  2875,   985,  -631,
    -631,   746,  -631,   804,  -631,  -631,  -631,   490,   492,   498,
    -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,   984,   756,
     990,  1233,   984,   760,   986,   987,  -631,   991,  1017,  1018,
    -631,  1027,  1084,  1103,  1028,  2875,  -631,  1104,  1134,  1030,
    2875,  -631,  -631,  -631,    47,  -631,  -631,  -631,  1040,   501,
     504,  -631,  -631,  1048,    30,  -631,  -631,  -631,  -631,  -631,
     984,  -631,  2611,  2677,  1365,  -631,   380,  -631,   984,  -631,
    -631,  -631,  -631,  -631,  -631,  1051,  1052,  -631,  1055,  1056,
    1057,  -631,  1059,   984,   761,  -631,  -631,  -631,  -631,  -631,
    -631,   493,   195,  -631,   506,  2875,  2875,  2875,  2875,  2875,
    2875,  2875,  2875,  -631,  -631,  -631,  -631,  -631,  -631,  -631,
    -631,  -631,   984,  2875,  2875,  2875,  -631,  -631,  -631,   507,
     509,   510,   512,   521,   533,   534,   539,  -631,   542,   548,
     566,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,  -631,
    -631,  -631
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   365,   366,   367,   369,   370,   371,
     372,   382,   381,   383,   384,     0,   373,   374,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     368,     0,     0,     0,     0,     0,    89,     0,     3,     4,
      90,    91,    98,    99,   100,   101,    92,    93,   108,     0,
     109,   110,   111,    97,     0,   289,   291,   309,   312,   314,
     317,   319,   321,   324,   329,   333,   335,   336,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   370,   371,   372,   382,
       0,   389,   332,     0,   370,    29,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,   390,   288,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,     0,     0,   102,     0,     0,   103,     0,     0,
       0,     3,     0,   360,    94,    96,   357,   196,     0,     0,
       0,     0,     0,     0,   331,   330,   385,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,    85,     0,     0,     0,     0,     0,   287,     0,
     340,     0,     0,     0,     0,     0,   345,     0,   344,   338,
       0,     0,     0,     0,    24,     0,   385,     0,     0,    32,
       0,     0,     6,     0,     7,     0,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     0,     0,    36,     0,     0,   115,
       0,   127,     0,     0,   141,     0,     0,     0,     3,   148,
      39,    40,     0,     0,   144,     0,     0,     0,     0,     0,
     130,     0,   133,     0,     0,     0,     0,     0,   358,   359,
     361,     0,     0,   199,     0,     0,     0,   234,    86,    87,
       0,   386,   253,   370,   371,   372,   382,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   294,   295,   298,   299,   292,   293,     0,   296,   297,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   310,
     311,   313,   315,   316,   318,   320,   322,   323,   328,   325,
     326,   327,   334,   356,   353,     0,    45,     0,    79,    80,
      81,    82,    83,    42,     0,     0,   339,   376,   377,   378,
      47,     0,   348,     0,   337,     0,     0,     0,   287,    31,
       0,   364,     0,     0,     0,   367,   381,     0,     0,     0,
       0,    21,     0,    22,     0,    23,     0,   151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   386,     0,     0,   118,   386,     0,     0,
     315,   316,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   386,     0,   386,   104,   105,   106,
     107,   286,    95,     0,     0,   199,     0,     0,     0,     0,
       0,     0,   387,     0,     0,     0,     0,     0,     0,     0,
       0,   279,     0,     0,     0,   280,     0,     0,   281,     0,
       0,     0,     0,     0,     0,     0,     0,   254,    49,    51,
      53,    56,    54,    55,    50,    52,    62,    64,    66,    69,
      67,    68,    63,    65,     0,     0,     0,   349,     0,     0,
       0,   158,     0,     0,     0,     0,   347,     0,   381,   343,
       0,     0,   376,   377,   378,    30,     5,     0,     0,     9,
       0,     0,     0,     0,    14,     0,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,    73,    72,    76,
      74,    75,    71,    34,     0,    77,    78,    37,   112,   117,
     116,     0,     0,   126,   138,     0,   140,     0,     0,   245,
       0,     0,   147,     0,     0,     0,     0,     0,   159,     0,
      26,    25,     0,     0,   142,   379,   380,     0,     0,   128,
     131,   132,   197,   198,     0,     0,     0,     0,   195,   156,
       0,     0,     0,     0,     0,     0,     0,   233,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
     270,   277,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   290,   355,     0,   352,     0,
       0,    44,     0,    46,   157,     0,     0,     0,   346,     0,
       0,   341,   342,     0,     0,     0,   241,   240,     0,     0,
       0,    16,     0,    17,     0,    20,   150,     0,    28,    27,
     388,   388,     0,   119,     0,   120,     0,     0,     0,   246,
     247,   248,     0,   146,     0,     0,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,     0,    60,
      61,     0,   194,     0,     0,     0,     0,     0,     0,     0,
       0,   155,   153,     0,     0,     0,     0,     0,     0,     0,
     376,   378,   274,     0,     0,     0,     0,     0,     0,   282,
     283,   284,   285,   255,   257,   259,   262,   260,   261,   256,
     258,   354,   351,   350,    43,    57,    58,    59,   238,     0,
       0,    11,    10,    15,     0,     0,     0,     0,   149,    35,
      38,   114,   113,   386,     0,   124,   139,   136,   137,   249,
       0,     0,     0,     0,     0,   163,   164,   161,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   143,   129,     0,
       3,   226,     0,   229,     0,     0,     0,     0,   205,     0,
       0,     0,     0,   200,     0,     0,     0,     0,   201,   202,
       0,   152,   154,   235,   236,   237,     0,   158,     0,     0,
       0,   264,   266,   269,   267,   268,   265,     0,   388,   243,
     242,     0,    18,   121,   122,   250,   251,     0,     0,     0,
     165,   167,   169,   172,   170,   171,   166,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   216,     0,     0,     0,
     217,     0,     0,     0,     0,     0,   207,     0,     0,     0,
       0,   206,   203,   204,     0,   232,   273,   272,   157,     0,
       0,   239,    12,   388,     0,   125,   173,   175,   174,   225,
       0,   177,     0,     0,     0,   176,     0,   228,     0,   214,
     215,   209,   212,   213,   208,     0,     0,   218,     0,     0,
       0,   219,     0,     0,     0,   271,   275,   276,    19,   123,
     224,     0,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,   227,   222,   223,   211,   220,   221,
     210,   231,     0,     0,     0,     0,   181,   182,   179,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,   183,   185,   187,   190,   188,   189,   184,   186,   191,
     193,   192
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -631,  -631,     1,  -631,  -448,  -584,  -631,  -631,  -631,  -631,
    -631,  -631,  -631,  -631,  -631,  -631,  -259,  -630,  -631,  -631,
    -631,   680,  -631,  -631,   636,   753,  -319,   197,   810,    -2,
    -202,  -631,   526,  -631,   -18,   -10,   962,   959,   663,   -12,
    -631,  -631,  -631,   -87,  -631
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   179,    59,    60,   456,    61,    62,    63,    64,
      65,    66,    67,   469,    68,    69,   299,   821,    70,    71,
     322,   486,    72,   491,   550,   428,   465,   111,   159,   117,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   367,   112,   703,   488,   589,   483,   135,   284,
     462,   437,   241,    24,   463,   484,   134,   599,   250,   799,
     126,   128,   130,   132,   133,   100,   285,   138,   140,   258,
     143,   462,   292,   147,   149,   463,   462,   824,   595,   325,
     463,   163,   174,   175,   166,   825,   284,   103,   152,   462,
     462,   101,   462,   463,   463,   177,   463,   462,   829,   155,
     597,   463,   733,   285,   562,   153,   830,   625,   626,   102,
     261,   627,   600,   601,   800,   801,   156,   293,   612,   613,
     169,   892,    58,   260,   262,   263,   826,   326,    58,   893,
     233,   234,   235,   236,   237,   238,   219,   170,   242,   264,
     244,   245,   286,   287,   823,     3,   734,   831,   255,   650,
     104,   598,    58,   105,   257,   113,   503,   572,   485,    58,
     252,   795,   464,   273,    99,   283,   897,   288,   827,   290,
     894,   625,   626,   504,   898,   627,   689,   628,   253,   286,
     287,   154,   828,   487,   309,   690,   311,   219,   571,   832,
     313,   314,   157,   315,   316,    58,   317,   318,   319,   317,
     320,   631,   878,   833,   882,   217,   218,   614,   144,   943,
     632,   633,   895,   104,   679,   899,   105,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   896,   353,   354,   355,
     356,   357,   358,   359,   360,   448,   379,   380,   381,    73,
     452,   732,   284,   219,   305,   602,   459,   382,   383,   154,
     905,   388,   389,   390,   391,   392,   505,   900,   307,   285,
     104,   284,   306,   105,   136,   137,   246,   101,   629,   247,
     330,   901,   331,   141,   150,   151,   308,   142,   285,   558,
     506,   164,   165,   493,   404,   496,   295,   296,   919,   415,
     145,   417,   927,   418,   791,   559,   560,   507,   421,   422,
     405,   424,   300,   429,   748,   634,   635,   636,   394,   651,
     248,   395,   441,   442,   443,   444,   445,   446,   447,   915,
     749,   301,   450,   451,   219,   460,   461,   167,   652,   265,
     950,   430,   221,   222,   432,   286,   287,   434,   964,   466,
     467,   468,   806,   266,   471,   431,   473,   136,   433,   410,
     539,   435,   696,   971,   286,   287,   752,   178,   542,   271,
     168,   977,   637,   272,   547,   158,   551,   289,   171,   291,
     294,   216,   665,   157,   220,   498,   499,   500,   502,   297,
     508,   223,   987,   298,   310,   713,   312,   509,   259,   260,
     510,   511,   477,   575,   224,   225,   226,   512,   231,   759,
     513,   514,   584,   478,   515,   516,   479,   396,   480,   397,
     760,   741,   742,   761,   643,   762,    73,   275,   644,   718,
     172,   436,   719,   720,   645,   298,   462,   646,   647,   721,
     463,   648,   722,   723,   526,   711,   724,   725,   527,   712,
     528,   763,   540,   173,   529,   764,   955,   765,   530,   956,
     957,   766,   228,   819,   822,   767,   958,   820,   820,   959,
     960,   557,   531,   961,   962,   177,   532,   533,   567,   768,
     569,   227,   570,   769,   770,   301,   573,   574,   788,   576,
     197,   229,   807,   578,   579,   230,   851,   517,   667,   670,
     852,   853,   802,   592,   232,   275,   803,   119,   120,   121,
      99,   122,   277,   580,   609,   278,   279,   281,   282,   804,
     854,   904,   197,   685,   649,   820,   581,   239,   123,   726,
     454,   455,   124,   855,   841,   842,   207,   208,   209,   210,
     211,   212,   251,   640,   641,   642,   214,   215,   213,   197,
     879,   653,   654,   883,   655,   656,   963,   657,   658,   659,
     660,   661,   662,   663,   664,   214,   215,   243,   482,   973,
     582,   274,   275,   974,   275,   276,   254,   856,   300,   277,
     526,   277,   278,   279,   278,   279,   975,   870,   280,   564,
     565,   675,   676,   677,   180,   281,   282,   181,   182,   260,
     270,   406,   805,   260,   183,   414,   684,   184,   185,   686,
     687,   186,   187,   527,   189,   528,   692,   190,   191,   529,
     871,   301,   872,   530,   192,   302,   873,   193,   194,   531,
     874,   195,   196,   583,   260,   944,   875,   532,   533,   704,
     198,   199,   200,   201,   876,   877,   780,   587,   260,   673,
     202,   203,   204,   714,   715,   717,   906,   787,   304,   576,
     727,   578,   281,   282,   281,   282,   916,   582,   917,   976,
     775,   205,   206,   777,   918,   301,   526,   946,   527,   528,
     947,   529,   978,   991,   321,   992,   993,   188,   994,   328,
     530,   753,   754,   755,   756,   757,   758,   995,   693,   694,
     588,   590,   531,   532,   593,   594,   596,   197,   533,   996,
     997,   576,   848,    73,   260,   998,   420,   578,   999,   702,
     455,   619,   620,   621,  1000,   303,    89,   778,    90,    91,
      92,    93,    94,    95,    96,   582,   267,   268,   269,   323,
      97,   735,  1001,   324,   736,   737,   738,   739,   440,   861,
     331,   327,   834,    98,   393,   835,    99,   836,   837,   376,
     377,   378,   449,   330,   331,   453,   808,   809,   810,   811,
     812,   813,   814,   815,   361,   362,   363,   364,   365,   366,
     329,   368,   369,   370,   371,   372,   373,   374,   375,   330,
     330,   457,   474,   330,   332,   476,   846,     4,     5,     6,
       7,   603,   398,   604,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,   603,   260,   630,   638,   260,
     260,   672,   674,   782,   260,   110,    28,   260,   680,   678,
     681,   399,   859,   860,   260,    33,   682,   408,   683,   705,
     331,   794,   455,    41,    42,    43,   407,   565,    46,   688,
     867,   868,   869,   603,   260,   697,   700,   260,    50,   701,
     728,   603,   400,   743,   603,   260,   744,   771,   680,   401,
     772,   881,   862,   260,   402,   887,     4,     5,     6,     7,
     891,   914,   455,   106,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,   260,   403,   773,   909,   910,   786,
      54,   331,   160,   162,   110,   911,   330,    55,   793,   260,
     260,   858,   908,   139,    33,    57,   260,   409,   913,    58,
     501,   410,    41,    42,    43,   411,   603,    46,   920,   926,
     603,   603,   928,   972,   386,   387,   412,    50,   413,   416,
     419,   438,   439,   938,   458,   470,   472,   475,   942,   792,
     477,   478,   479,   796,   797,   798,   481,   480,   495,   518,
     489,    89,   490,    90,    91,    92,    93,    94,    95,    96,
     951,   952,   954,   497,   816,    97,   519,   520,   818,    54,
     521,   522,   523,   524,   525,   526,    55,   534,   494,   537,
     527,    99,   543,   528,    57,   529,   530,   531,    58,   532,
     533,   219,   544,   979,   980,   981,   982,   983,   984,   985,
     986,   545,   535,   536,   554,   553,   561,   552,   555,   556,
     566,   988,   989,   990,   576,   615,   617,   577,   578,   579,
     580,   581,   582,   585,   586,     4,     5,     6,     7,   622,
     863,   864,   333,   334,   335,   336,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   337,   338,    24,   610,
     298,    25,    26,    27,    28,   339,   340,    31,    32,   611,
     623,   616,   618,    33,    34,    35,    36,    37,    38,   341,
     342,    41,    42,    43,    44,    45,    46,    47,   639,   671,
     673,    48,    49,   691,   706,   695,    50,   698,   699,   707,
      51,   708,   709,   710,   729,   730,   731,   740,   745,   746,
     747,   750,   774,   775,   751,   776,   777,   779,   781,   783,
     784,   785,   789,   790,   817,    52,    53,   838,    73,   839,
     840,   843,   844,   845,   849,   850,   857,   865,    54,   866,
     880,   886,   884,   890,   885,    55,   888,   889,   935,   907,
     902,    56,   903,    57,   912,   929,   930,    58,   343,   820,
     931,   949,     4,     5,     6,     7,   921,   936,   939,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   605,   606,    24,   932,   933,    25,    26,
      27,    28,    29,   607,    31,    32,   934,   937,   940,   941,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,   624,   945,   948,    48,    49,
     965,   966,   669,    50,   967,   968,   969,    51,   970,   385,
     563,   384,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,    56,     0,
      57,     0,     0,     0,    58,   608,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   922,   923,    24,
       0,     0,    25,    26,    27,    28,    29,   924,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,    48,    49,     0,     0,     0,    50,     0,     0,
       0,    51,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,    52,    53,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,    56,    46,    57,     0,     0,     0,    58,   925,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,  -389,  -389,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,   139,    46,
      57,     0,     0,     0,    58,   716,     0,  -389,  -389,    50,
    -389,  -389,  -389,  -389,  -389,  -389,  -389,  -389,  -389,  -389,
    -389,  -389,  -389,     0,     0,     0,  -389,     0,     0,     0,
       0,     0,     0,     0,  -389,  -389,  -389,     0,  -389,     0,
       0,  -389,  -389,  -389,  -389,  -389,  -389,     0,  -389,  -389,
    -389,    54,  -389,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,   139,     0,    57,     0,     0,     0,
      58,   953,     4,     5,     6,     7,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,    26,
      27,    28,    29,    30,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,     0,    50,     0,     0,     0,    51,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,    52,    53,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,    56,    46,
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
      55,     0,    41,    42,    43,     0,     0,    46,    57,   176,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   240,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   249,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   256,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,   178,     0,
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
      55,     0,    41,    42,    43,     0,     0,    46,    57,   492,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   538,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   541,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,   548,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   546,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,   425,     7,     0,     0,     0,   114,   107,   108,
     109,   426,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,    57,   549,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,    57,   176,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,   548,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,   666,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
      57,   668,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,   847,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,   125,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,   127,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,   129,     0,     0,     0,    58,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
     131,     0,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,    57,     0,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,   146,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,    54,    33,     0,     0,     0,     0,     0,
      55,     0,    41,    42,    43,     0,     0,    46,   148,     0,
       0,     0,    58,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,     0,    54,
      33,     0,     0,     0,     0,     0,    55,     0,    41,    42,
      43,     0,     0,    46,    57,     0,     0,     0,   161,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     4,     5,
     425,     7,     0,     0,     0,   114,   107,   108,   109,   426,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,    54,    33,     0,     0,     0,
       0,     0,    55,     0,    41,    42,    43,     0,     0,    46,
     423,     0,     0,     0,    58,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,    54,    33,     0,     0,     0,     0,     0,    55,     0,
      41,    42,    43,     0,     0,    46,   427,     0,     0,     0,
      58,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,    54,    33,     0,
       0,     0,     0,     0,    55,     0,    41,    42,    43,     0,
       0,    46,   568,     0,     0,     0,    58,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,   591,     0,
       0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   204,    15,   588,   324,   454,    14,    14,    26,
      10,   270,    99,    26,    14,    22,    26,    10,   105,    10,
      22,    23,    24,    25,    26,   123,    43,    29,    30,   116,
      32,    10,    75,    35,    36,    14,    10,    14,    75,    14,
      14,    43,    54,    55,    46,    22,    26,   105,    26,    10,
      10,   107,    10,    14,    14,    57,    14,    10,    14,    26,
      75,    14,    14,    43,    72,    43,    22,    56,    57,   125,
     105,    60,    65,    66,    65,    66,    43,   120,    37,    38,
      14,    14,   125,   120,   119,   105,    63,    62,   125,    22,
      92,    93,    94,    95,    96,    97,   111,    31,   100,   119,
     102,   103,   119,   120,   734,     0,    58,    63,   105,   126,
     118,    75,   125,   121,   116,    14,    26,   436,   125,   125,
     107,   705,   122,   125,   121,   127,    14,   129,   105,   131,
      63,    56,    57,    43,    22,    60,     5,   126,   125,   119,
     120,   119,   119,   122,   146,    14,   148,   111,   122,   105,
     152,   153,   119,   155,   156,   125,   158,   159,   160,   161,
     162,   122,   122,   119,   122,    73,    74,   126,    12,   122,
     489,   490,   105,   118,    72,    63,   121,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   119,   189,   190,   191,
     192,   193,   194,   195,   196,   282,   214,   215,   216,     2,
     287,   126,    26,   111,   107,   464,   293,   217,   218,   119,
     840,   223,   224,   225,   226,   227,   126,   105,   107,    43,
     118,    26,   125,   121,    27,    28,    14,   107,   487,    17,
     120,   119,   122,    10,    37,    38,   125,    14,    43,   103,
      26,    44,    45,   330,   105,   125,    73,    74,   878,   251,
      12,   253,   882,   255,   702,   119,   120,    43,   260,   261,
     121,   263,   119,   265,   105,    69,    70,    71,    14,   126,
      58,    17,   274,   275,   276,   277,   278,   279,   280,   863,
     121,   119,   284,   285,   111,   295,   296,    14,   126,   105,
     920,   105,   112,   113,   105,   119,   120,   105,   928,   298,
     302,   303,   126,   119,   306,   119,   308,   110,   119,   119,
     397,   119,   571,   943,   119,   120,   126,    67,   405,    10,
      14,   126,   126,    14,   411,   125,   413,   130,    14,   132,
     133,   100,   534,   119,   102,   337,   338,   339,   340,   121,
     126,   101,   972,   125,   147,   604,   149,    26,   119,   120,
      29,    30,   119,   440,   114,   115,   116,    36,   119,   126,
      39,    40,   449,   119,    43,    44,   119,   119,   119,   121,
     126,   630,   631,   126,    26,   126,   179,    29,    30,    26,
      10,   121,    29,    30,    36,   125,    10,    39,    40,    36,
      14,    43,    39,    40,   119,    10,    43,    44,   119,    14,
     119,   126,   404,    10,   119,   126,    26,   126,   119,    29,
      30,   126,   118,   121,   121,   126,    36,   125,   125,    39,
      40,   423,   119,    43,    44,   427,   119,   119,   430,   126,
     432,   104,   434,   126,   126,   119,   438,   439,   697,   119,
     119,   118,   126,   119,   119,    14,   126,   126,   535,   536,
     126,   126,    26,   455,   119,    29,    30,    10,    11,    12,
     121,    14,    36,   119,   466,    39,    40,   119,   120,    43,
     126,   121,   119,   560,   126,   125,   119,    14,    31,   126,
      27,    28,    35,   126,   743,   744,    80,    81,    82,    83,
      84,    85,   123,   495,   496,   497,   109,   110,    92,   119,
     819,   503,   504,   822,   506,   507,   126,   509,   510,   511,
     512,   513,   514,   515,   516,   109,   110,    14,   321,    26,
     119,    26,    29,    30,    29,    30,   119,   126,   119,    36,
     119,    36,    39,    40,    39,    40,    43,   126,    43,   119,
     120,   543,   544,   545,    26,   119,   120,    29,    30,   120,
      14,   122,   126,   120,    36,   122,   558,    39,    40,   561,
     562,    43,    44,   119,    26,   119,   568,    29,    30,   119,
     126,   119,   126,   119,    36,   105,   126,    39,    40,   119,
     126,    43,    44,   119,   120,   904,   126,   119,   119,   591,
      76,    77,    78,    79,   126,   126,   683,   119,   120,   119,
      86,    87,    88,   605,   606,   607,   126,   694,   125,   119,
     612,   119,   119,   120,   119,   120,   126,   119,   126,   126,
     119,   107,   108,   119,   126,   119,   119,   126,   119,   119,
     126,   119,   126,   126,    53,   126,   126,   119,   126,   119,
     119,   643,   644,   645,   646,   647,   648,   126,   119,   120,
     453,   454,   119,   119,   457,   458,   459,   119,   119,   126,
     126,   119,   749,   466,   120,   126,   122,   119,   126,    27,
      28,   474,   475,   476,   126,   105,    91,   679,    93,    94,
      95,    96,    97,    98,    99,   119,    10,    11,    12,   125,
     105,    10,   126,   121,    13,    14,    15,    16,   120,   786,
     122,   125,    10,   118,    14,    13,   121,    15,    16,     7,
       8,     9,   120,   120,   122,   122,   718,   719,   720,   721,
     722,   723,   724,   725,   198,   199,   200,   201,   202,   203,
     119,   205,   206,   207,   208,   209,   210,   211,   212,   120,
     120,   122,   122,   120,   126,   122,   748,     3,     4,     5,
       6,   120,   119,   122,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   120,   120,   122,   122,   120,
     120,   122,   122,   119,   120,    31,    32,   120,   120,   122,
     122,   119,   784,   785,   120,    41,   122,   108,   120,   592,
     122,    27,    28,    49,    50,    51,   124,   120,    54,   122,
     802,   803,   804,   120,   120,   122,   122,   120,    64,   122,
     613,   120,   119,   122,   120,   120,   122,   122,   120,   119,
     122,   820,   119,   120,   119,   827,     3,     4,     5,     6,
     832,    27,    28,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   120,   119,   122,   849,   850,   120,
     106,   122,    42,    43,    31,   857,   120,   113,   122,   120,
     120,   122,   122,   119,    41,   121,   120,   126,   122,   125,
     126,   119,    49,    50,    51,   121,   120,    54,   122,   881,
     120,   120,   122,   122,   221,   222,    14,    64,   121,    14,
     119,   105,   105,   895,    10,    14,    14,    27,   900,   702,
     119,   119,   119,   706,   707,   708,   126,   119,   123,   119,
     121,    91,   121,    93,    94,    95,    96,    97,    98,    99,
     922,   923,   924,   105,   727,   105,   119,   119,   731,   106,
     119,   119,   119,   119,   119,   119,   113,    89,   118,    14,
     119,   121,   105,   119,   121,   119,   119,   119,   125,   119,
     119,   111,   105,   955,   956,   957,   958,   959,   960,   961,
     962,   105,   121,   121,   126,   108,    72,   124,   119,   119,
     119,   973,   974,   975,   119,   108,   108,   119,   119,   119,
     119,   119,   119,   119,   119,     3,     4,     5,     6,    14,
     793,   794,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   119,
     125,    29,    30,    31,    32,    33,    34,    35,    36,   119,
      14,   126,   126,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    14,   119,
     119,    59,    60,   119,    10,   119,    64,   119,   119,    10,
      68,    10,    10,    10,   119,   119,    27,    56,    14,    14,
      14,   124,   119,   119,   126,   119,   119,    14,   119,   119,
      72,    72,   119,   119,   126,    93,    94,   119,   881,   119,
      14,     7,     7,     7,   105,   105,    72,    10,   106,    10,
      67,   119,    14,   119,    14,   113,    14,    14,    14,   126,
     119,   119,   119,   121,   119,   119,   119,   125,   126,   125,
     119,   914,     3,     4,     5,     6,   126,    14,    14,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   119,   119,    29,    30,
      31,    32,    33,    34,    35,    36,   119,   119,    14,   119,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,   485,   126,   119,    59,    60,
     119,   119,   536,    64,   119,   119,   119,    68,   119,   220,
     427,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      51,    -1,   119,    54,   121,    -1,    -1,    -1,   125,   126,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    40,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,   119,    54,
     121,    -1,    -1,    -1,   125,   126,    -1,    73,    74,    64,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,    -1,   104,    -1,
      -1,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,   106,   118,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,
     125,   126,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    41,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    49,    50,    51,    -1,   119,    54,
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
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    41,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      49,    50,    51,    -1,    -1,    54,   121,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    -1,
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
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125
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
     131,   133,   134,   135,   136,   137,   138,   139,   141,   142,
     145,   146,   149,   154,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   171,    91,
      93,    94,    95,    96,    97,    98,    99,   105,   118,   121,
     123,   107,   125,   105,   118,   121,    10,    11,    12,    13,
      31,   154,   166,    14,    10,   119,   121,   156,   170,    10,
      11,    12,    14,    31,    35,   121,   156,   121,   156,   121,
     156,   121,   156,   156,   162,    14,   154,   154,   156,   119,
     156,    10,    14,   156,    12,    12,   121,   156,   121,   156,
     154,   154,    26,    43,   119,    26,    43,   119,   125,   155,
     155,   125,   155,   156,   154,   154,   156,    14,    14,    14,
      31,    14,    10,    10,   166,   166,   122,   156,    67,   129,
      26,    29,    30,    36,    39,    40,    43,    44,   119,    26,
      29,    30,    36,    39,    40,    43,    44,   119,    76,    77,
      78,    79,    86,    87,    88,   107,   108,    80,    81,    82,
      83,    84,    85,    92,   109,   110,   100,    73,    74,   111,
     102,   112,   113,   101,   114,   115,   116,   104,   118,   118,
      14,   119,   119,   156,   156,   156,   156,   156,   156,    14,
     122,   170,   156,    14,   156,   156,    14,    17,    58,   122,
     170,   123,   107,   125,   119,   105,   122,   156,   170,   119,
     120,   105,   119,   105,   119,   105,   119,    10,    11,    12,
      14,    10,    14,   156,    26,    29,    30,    36,    39,    40,
      43,   119,   120,   156,    26,    43,   119,   120,   156,   154,
     156,   154,    75,   120,   154,    73,    74,   121,   125,   143,
     119,   119,   105,   105,   125,   107,   125,   107,   125,   156,
     154,   156,   154,   156,   156,   156,   156,   156,   156,   156,
     156,    53,   147,   125,   121,    14,    62,   125,   119,   119,
     120,   122,   126,    10,    11,    12,    13,    24,    25,    33,
      34,    47,    48,   126,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   159,   159,   159,   159,   159,   159,   157,   159,   159,
     159,   159,   159,   159,   159,   159,     7,     8,     9,   161,
     161,   161,   162,   162,   163,   164,   165,   165,   166,   166,
     166,   166,   166,    14,    14,    17,   119,   121,   119,   119,
     119,   119,   119,   119,   105,   121,   122,   124,   108,   126,
     119,   121,    14,   121,   122,   156,    14,   156,   156,   119,
     122,   156,   156,   121,   156,     5,    14,   121,   152,   156,
     105,   119,   105,   119,   105,   119,   121,   143,   105,   105,
     120,   156,   156,   156,   156,   156,   156,   156,   170,   120,
     156,   156,   170,   122,    27,    28,   132,   122,    10,   170,
     162,   162,    10,    14,   122,   153,   129,   156,   156,   140,
      14,   156,    14,   156,   122,    27,   122,   119,   119,   119,
     119,   126,   154,    14,    22,   125,   148,   122,   153,   121,
     121,   150,   122,   170,   118,   123,   125,   105,   156,   156,
     156,   126,   156,    26,    43,   126,    26,    43,   126,    26,
      29,    30,    36,    39,    40,    43,    44,   126,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,    89,   121,   121,    14,   122,   170,
     156,   122,   170,   105,   105,   105,   122,   170,    14,   122,
     151,   170,   124,   108,   126,   119,   119,   156,   103,   119,
     120,    72,    72,   152,   119,   120,   119,   156,   121,   156,
     156,   122,   153,   156,   156,   170,   119,   119,   119,   119,
     119,   119,   119,   119,   170,   119,   119,   119,   154,   131,
     154,   121,   156,   154,   154,    75,   154,    75,    75,    10,
      65,    66,   143,   120,   122,    24,    25,    34,   126,   156,
     119,   119,    37,    38,   126,   108,   126,   108,   126,   154,
     154,   154,    14,    14,   148,    56,    57,    60,   126,   143,
     122,   122,   153,   153,    69,    70,    71,   126,   122,    14,
     156,   156,   156,    26,    30,    36,    39,    40,    43,   126,
     126,   126,   126,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   157,   122,   170,   122,   151,
     170,   119,   122,   119,   122,   156,   156,   156,   122,    72,
     120,   122,   122,   120,   156,   170,   156,   156,   122,     5,
      14,   119,   156,   119,   120,   119,   143,   122,   119,   119,
     122,   122,    27,   132,   156,   154,    10,    10,    10,    10,
      10,    10,    14,   143,   156,   156,   126,   156,    26,    29,
      30,    36,    39,    40,    43,    44,   126,   156,   154,   119,
     119,    27,   126,    14,    58,    10,    13,    14,    15,    16,
      56,   143,   143,   122,   122,    14,    14,    14,   105,   121,
     124,   126,   126,   156,   156,   156,   156,   156,   156,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   122,   122,   122,   119,   119,   119,   119,   156,    14,
     170,   119,   119,   119,    72,    72,   120,   170,   143,   119,
     119,   131,   154,   122,    27,   132,   154,   154,   154,    10,
      65,    66,    26,    30,    43,   126,   126,   126,   156,   156,
     156,   156,   156,   156,   156,   156,   154,   126,   154,   121,
     125,   144,   121,   144,    14,    22,    63,   105,   119,    14,
      22,    63,   105,   119,    10,    13,    15,    16,   119,   119,
      14,   143,   143,     7,     7,     7,   156,   122,   170,   105,
     105,   126,   126,   126,   126,   126,   126,    72,   122,   156,
     156,   170,   119,   154,   154,    10,    10,   156,   156,   156,
     126,   126,   126,   126,   126,   126,   126,   126,   122,   153,
      67,   129,   122,   153,    14,    14,   119,   156,    14,    14,
     119,   156,    14,    22,    63,   105,   119,    14,    22,    63,
     105,   119,   119,   119,   121,   144,   126,   126,   122,   156,
     156,   156,   119,   122,    27,   132,   126,   126,   126,   144,
     122,   126,    24,    25,    34,   126,   156,   144,   122,   119,
     119,   119,   119,   119,   119,    14,    14,   119,   156,    14,
      14,   119,   156,   122,   153,   126,   126,   126,   119,   154,
     144,   156,   156,   126,   156,    26,    29,    30,    36,    39,
      40,    43,    44,   126,   144,   119,   119,   119,   119,   119,
     119,   144,   122,    26,    30,    43,   126,   126,   126,   156,
     156,   156,   156,   156,   156,   156,   156,   144,   156,   156,
     156,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126
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
     134,   134,   135,   135,   136,   137,   138,   138,   138,   138,
     138,   138,   139,   139,   140,   140,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   146,   147,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   149,   150,   150,   150,   150,   151,   151,
     152,   152,   152,   152,   153,   153,   153,   153,   153,   153,
     153,   153,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   155,   156,   156,   156,
     157,   157,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     159,   159,   159,   160,   160,   161,   161,   161,   162,   162,
     163,   163,   164,   164,   164,   165,   165,   165,   165,   165,
     166,   166,   166,   166,   167,   167,   168,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   170,   170,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171
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
       3,     5,     5,     3,     2,     2,     7,     7,     5,     7,
       5,     3,     5,     7,     0,     4,     6,     5,     3,     7,
       6,     4,     7,     6,     7,     6,     5,     6,     5,     3,
       4,     5,     4,     5,     5,     6,     6,     6,     6,     6,
       6,     6,     6,     7,     7,     7,     3,     3,     4,     5,
       4,     5,     5,     6,     6,     6,     6,     6,     6,     6,
       6,     7,     7,     7,     6,     5,     0,     3,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     6,     6,     6,     5,     5,     6,     6,
       7,     7,     7,     7,     7,     6,     4,     7,     6,     4,
       8,     7,     5,     5,     0,     4,     4,     4,     3,     5,
       3,     3,     5,     5,     1,     2,     3,     3,     3,     4,
       5,     5,     3,     3,     4,     6,     6,     6,     6,     6,
       6,     6,     6,     5,     7,     7,     7,     7,     7,     7,
       5,     9,     8,     8,     6,     9,     9,     5,     5,     4,
       4,     4,     6,     6,     6,     6,     3,     3,     2,     1,
       5,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       2,     2,     2,     1,     3,     1,     1,     4,     3,     4,
       3,     6,     6,     5,     3,     3,     6,     5,     4,     4,
       6,     6,     5,     3,     6,     5,     3,     2,     3,     3,
       2,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     5,
       5,     1,     1,     1,     1,     2,     3,     4,     5,     1,
       2
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
#line 315 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2612 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 325 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2618 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 326 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2624 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 330 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2630 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 332 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2636 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 334 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2642 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 336 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2648 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 338 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2654 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 340 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2662 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 344 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2669 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 347 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2677 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 351 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2683 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 353 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2689 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 355 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2695 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 357 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2701 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 359 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2707 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 361 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2715 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 365 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2723 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 369 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2729 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 371 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2735 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 373 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2741 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 375 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2747 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 377 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2753 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 379 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2759 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 381 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2765 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 383 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2771 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 385 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2777 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 387 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2783 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 389 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2789 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 391 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2795 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 393 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2801 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 395 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2807 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 397 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2814 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 400 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2821 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 403 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2827 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2834 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 408 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2841 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 411 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2847 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 413 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2853 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 415 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2859 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 417 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2865 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 419 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2874 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 424 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2882 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 428 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2890 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 432 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2899 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 437 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2907 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 440 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2913 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 442 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2919 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 444 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2925 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 446 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2931 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 448 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2937 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 450 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2944 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 453 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2950 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 455 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2956 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 457 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2962 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2969 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 462 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2976 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 465 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2983 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 468 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2990 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 471 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2997 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3003 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 476 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3009 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 478 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3015 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 480 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3021 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 482 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3028 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 485 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3034 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3040 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 489 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3046 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 491 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3053 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 494 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3060 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 497 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3068 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 501 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3075 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 504 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3081 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 506 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3087 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 508 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3093 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 510 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3100 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 513 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3107 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 516 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3114 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 519 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3121 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 522 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3128 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 525 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3135 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 528 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3142 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 531 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3149 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 534 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3156 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 537 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3163 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 540 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3170 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 542 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3176 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 543 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3182 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 544 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3188 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 545 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3194 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 546 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3200 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 547 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3206 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 549 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3212 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 551 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3218 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 553 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3224 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 555 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3230 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3236 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 557 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3242 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 558 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3248 "raku.tab.c"
    break;

  case 101: /* stmt: loop_stmt  */
#line 559 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3254 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST ';'  */
#line 560 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3260 "raku.tab.c"
    break;

  case 103: /* stmt: KW_NEXT ';'  */
#line 561 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3266 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST KW_IF expr ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3272 "raku.tab.c"
    break;

  case 105: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3278 "raku.tab.c"
    break;

  case 106: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3284 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3290 "raku.tab.c"
    break;

  case 108: /* stmt: sub_decl  */
#line 570 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3296 "raku.tab.c"
    break;

  case 109: /* stmt: class_decl  */
#line 571 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3302 "raku.tab.c"
    break;

  case 110: /* stmt: role_decl  */
#line 572 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3308 "raku.tab.c"
    break;

  case 111: /* stmt: grammar_decl  */
#line 573 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3314 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3320 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3326 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3332 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3338 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3344 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 587 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3350 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block elsif_tail  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3356 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 591 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3362 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block  */
#line 595 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3368 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 597 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3374 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3380 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 601 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3386 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 603 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3392 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3398 "raku.tab.c"
    break;

  case 126: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 609 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3404 "raku.tab.c"
    break;

  case 127: /* while_stmt: KW_WHILE expr block  */
#line 611 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3410 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3416 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3422 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS expr block  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3428 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3434 "raku.tab.c"
    break;

  case 132: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3440 "raku.tab.c"
    break;

  case 133: /* until_stmt: KW_UNTIL expr block  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3446 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3452 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 635 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3458 "raku.tab.c"
    break;

  case 136: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 639 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3468 "raku.tab.c"
    break;

  case 137: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 645 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3478 "raku.tab.c"
    break;

  case 138: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 651 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3487 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 656 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3497 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 662 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3506 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr block  */
#line 667 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3513 "raku.tab.c"
    break;

  case 142: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 672 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3525 "raku.tab.c"
    break;

  case 143: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 680 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3538 "raku.tab.c"
    break;

  case 144: /* when_list: %empty  */
#line 690 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3544 "raku.tab.c"
    break;

  case 145: /* when_list: when_list KW_WHEN expr block  */
#line 692 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3552 "raku.tab.c"
    break;

  case 146: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 698 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3564 "raku.tab.c"
    break;

  case 147: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 706 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3574 "raku.tab.c"
    break;

  case 148: /* sub_decl: KW_SUB IDENT sub_body  */
#line 712 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3584 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 718 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3596 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 726 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3606 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 732 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3616 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 738 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3629 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 747 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3640 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 754 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3653 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 763 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3666 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 772 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3677 "raku.tab.c"
    break;

  case 157: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 781 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3688 "raku.tab.c"
    break;

  case 158: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 788 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3697 "raku.tab.c"
    break;

  case 159: /* sub_body: '{' stmt_list '}'  */
#line 794 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3703 "raku.tab.c"
    break;

  case 160: /* sub_body: '{' stmt_list expr '}'  */
#line 796 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3710 "raku.tab.c"
    break;

  case 161: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 799 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3717 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 802 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3723 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 804 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3729 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 806 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3735 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3741 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3747 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 812 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3753 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3759 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 816 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3766 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 819 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3772 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 821 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3778 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 823 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3784 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 825 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3791 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 828 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3798 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 831 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3806 "raku.tab.c"
    break;

  case 176: /* method_body: '{' stmt_list '}'  */
#line 836 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3812 "raku.tab.c"
    break;

  case 177: /* method_body: '{' YADA '}'  */
#line 837 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3818 "raku.tab.c"
    break;

  case 178: /* method_body: '{' stmt_list expr '}'  */
#line 839 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3825 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 842 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3832 "raku.tab.c"
    break;

  case 180: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 845 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3838 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 847 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3844 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 849 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3850 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3856 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3862 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 855 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3868 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3874 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 859 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3881 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 862 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3887 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 864 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3893 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 866 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3899 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 868 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3906 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 871 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3913 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 874 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3921 "raku.tab.c"
    break;

  case 194: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 880 "raku.y"
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
#line 3939 "raku.tab.c"
    break;

  case 195: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 896 "raku.y"
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
#line 3956 "raku.tab.c"
    break;

  case 196: /* is_clauses: %empty  */
#line 910 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3962 "raku.tab.c"
    break;

  case 197: /* is_clauses: is_clauses IDENT IDENT  */
#line 912 "raku.y"
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
#line 3978 "raku.tab.c"
    break;

  case 198: /* is_clauses: is_clauses TESTOP IDENT  */
#line 924 "raku.y"
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
#line 3994 "raku.tab.c"
    break;

  case 199: /* class_body_list: %empty  */
#line 937 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4000 "raku.tab.c"
    break;

  case 200: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 939 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4007 "raku.tab.c"
    break;

  case 201: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 942 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4014 "raku.tab.c"
    break;

  case 202: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 945 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4021 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 948 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4028 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 951 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4035 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 954 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4042 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 957 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4049 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 960 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4056 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 963 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4063 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 966 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4070 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 969 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4077 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 972 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4084 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 975 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4095 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 982 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4106 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 989 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4117 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 996 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4128 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1003 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4136 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1007 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4144 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1011 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4152 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1015 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4160 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1019 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4171 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1026 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4182 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1033 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4193 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1040 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4204 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 1047 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4218 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 1057 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4230 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 1065 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4242 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1073 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4255 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1082 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4266 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1089 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4277 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 1096 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4291 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 1106 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4303 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 1114 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4315 "raku.tab.c"
    break;

  case 233: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1124 "raku.y"
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
#line 4332 "raku.tab.c"
    break;

  case 234: /* grammar_body_list: %empty  */
#line 1138 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4338 "raku.tab.c"
    break;

  case 235: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1140 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4347 "raku.tab.c"
    break;

  case 236: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1145 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4356 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1150 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4365 "raku.tab.c"
    break;

  case 238: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1157 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4373 "raku.tab.c"
    break;

  case 239: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1161 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4381 "raku.tab.c"
    break;

  case 240: /* pair_list: IDENT OP_FATARROW expr  */
#line 1167 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4387 "raku.tab.c"
    break;

  case 241: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1169 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4393 "raku.tab.c"
    break;

  case 242: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1171 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4399 "raku.tab.c"
    break;

  case 243: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1173 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4405 "raku.tab.c"
    break;

  case 244: /* param_list: VAR_SCALAR  */
#line 1176 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4411 "raku.tab.c"
    break;

  case 245: /* param_list: IDENT VAR_SCALAR  */
#line 1177 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4417 "raku.tab.c"
    break;

  case 246: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1178 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4423 "raku.tab.c"
    break;

  case 247: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1179 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4429 "raku.tab.c"
    break;

  case 248: /* param_list: param_list ',' VAR_SCALAR  */
#line 1180 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4435 "raku.tab.c"
    break;

  case 249: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1181 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4441 "raku.tab.c"
    break;

  case 250: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1182 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4447 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1183 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4453 "raku.tab.c"
    break;

  case 252: /* block: '{' stmt_list '}'  */
#line 1186 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4459 "raku.tab.c"
    break;

  case 253: /* block: '{' YADA '}'  */
#line 1187 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4465 "raku.tab.c"
    break;

  case 254: /* block: '{' stmt_list expr '}'  */
#line 1189 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4471 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1191 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4477 "raku.tab.c"
    break;

  case 256: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1193 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4483 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1195 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4489 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1197 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4495 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1199 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4502 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1202 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4508 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1204 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4514 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1206 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4520 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1208 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4526 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1210 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4533 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1213 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4540 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1216 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4548 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1220 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4555 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1223 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4562 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1226 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4569 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1229 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4575 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1231 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1235 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4590 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1238 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4597 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1241 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4604 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1244 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4611 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1247 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4618 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1250 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4624 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1252 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4630 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1254 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4636 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_LAST '}'  */
#line 1256 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4642 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1258 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4648 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1260 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4654 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1262 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4660 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1264 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4666 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1266 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4672 "raku.tab.c"
    break;

  case 286: /* closure: '{' expr '}'  */
#line 1269 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4678 "raku.tab.c"
    break;

  case 287: /* expr: VAR_SCALAR '=' expr  */
#line 1272 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4684 "raku.tab.c"
    break;

  case 288: /* expr: KW_GATHER block  */
#line 1273 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4694 "raku.tab.c"
    break;

  case 289: /* expr: tern_expr  */
#line 1278 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4700 "raku.tab.c"
    break;

  case 290: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1282 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4706 "raku.tab.c"
    break;

  case 291: /* tern_expr: cmp_expr  */
#line 1283 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4712 "raku.tab.c"
    break;

  case 292: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1286 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4718 "raku.tab.c"
    break;

  case 293: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1287 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4724 "raku.tab.c"
    break;

  case 294: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1288 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4730 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1289 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4736 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1290 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4742 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1291 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4748 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1292 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4754 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1293 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4760 "raku.tab.c"
    break;

  case 300: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1294 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4766 "raku.tab.c"
    break;

  case 301: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1295 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4772 "raku.tab.c"
    break;

  case 302: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1296 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4778 "raku.tab.c"
    break;

  case 303: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1297 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4784 "raku.tab.c"
    break;

  case 304: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1298 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4790 "raku.tab.c"
    break;

  case 305: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1299 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4796 "raku.tab.c"
    break;

  case 306: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1301 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4806 "raku.tab.c"
    break;

  case 307: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1307 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4816 "raku.tab.c"
    break;

  case 308: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1313 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4826 "raku.tab.c"
    break;

  case 309: /* cmp_expr: jct_expr  */
#line 1318 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4832 "raku.tab.c"
    break;

  case 310: /* jct_expr: jct_expr '|' range_expr  */
#line 1321 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4838 "raku.tab.c"
    break;

  case 311: /* jct_expr: jct_expr '&' range_expr  */
#line 1322 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4844 "raku.tab.c"
    break;

  case 312: /* jct_expr: dor_expr  */
#line 1323 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4850 "raku.tab.c"
    break;

  case 313: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1327 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4856 "raku.tab.c"
    break;

  case 314: /* dor_expr: range_expr  */
#line 1328 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4862 "raku.tab.c"
    break;

  case 315: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1331 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4868 "raku.tab.c"
    break;

  case 316: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1332 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4874 "raku.tab.c"
    break;

  case 317: /* range_expr: add_expr  */
#line 1333 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4880 "raku.tab.c"
    break;

  case 318: /* add_expr: add_expr '~' repl_expr  */
#line 1336 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4886 "raku.tab.c"
    break;

  case 319: /* add_expr: repl_expr  */
#line 1337 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4892 "raku.tab.c"
    break;

  case 320: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1340 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4898 "raku.tab.c"
    break;

  case 321: /* repl_expr: addsub_expr  */
#line 1341 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4904 "raku.tab.c"
    break;

  case 322: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1344 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4910 "raku.tab.c"
    break;

  case 323: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1345 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4916 "raku.tab.c"
    break;

  case 324: /* addsub_expr: mul_expr  */
#line 1346 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4922 "raku.tab.c"
    break;

  case 325: /* mul_expr: mul_expr '*' unary_expr  */
#line 1349 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4928 "raku.tab.c"
    break;

  case 326: /* mul_expr: mul_expr '/' unary_expr  */
#line 1350 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4934 "raku.tab.c"
    break;

  case 327: /* mul_expr: mul_expr '%' unary_expr  */
#line 1351 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4940 "raku.tab.c"
    break;

  case 328: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1352 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4946 "raku.tab.c"
    break;

  case 329: /* mul_expr: unary_expr  */
#line 1353 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4952 "raku.tab.c"
    break;

  case 330: /* unary_expr: '-' unary_expr  */
#line 1356 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4958 "raku.tab.c"
    break;

  case 331: /* unary_expr: '!' unary_expr  */
#line 1357 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4964 "raku.tab.c"
    break;

  case 332: /* unary_expr: CARET unary_expr  */
#line 1358 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 4970 "raku.tab.c"
    break;

  case 333: /* unary_expr: pow_expr  */
#line 1359 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4976 "raku.tab.c"
    break;

  case 334: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1362 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4982 "raku.tab.c"
    break;

  case 335: /* pow_expr: postfix_expr  */
#line 1363 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4988 "raku.tab.c"
    break;

  case 336: /* postfix_expr: call_expr  */
#line 1365 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4994 "raku.tab.c"
    break;

  case 337: /* call_expr: IDENT '(' arg_list ')'  */
#line 1368 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5003 "raku.tab.c"
    break;

  case 338: /* call_expr: IDENT '(' ')'  */
#line 1372 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5009 "raku.tab.c"
    break;

  case 339: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1374 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5018 "raku.tab.c"
    break;

  case 340: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1379 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5024 "raku.tab.c"
    break;

  case 341: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1381 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5034 "raku.tab.c"
    break;

  case 342: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1387 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5044 "raku.tab.c"
    break;

  case 343: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1393 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5052 "raku.tab.c"
    break;

  case 344: /* call_expr: IDENT '.' KW_NEW  */
#line 1397 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5060 "raku.tab.c"
    break;

  case 345: /* call_expr: IDENT '.' IDENT  */
#line 1401 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5069 "raku.tab.c"
    break;

  case 346: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1406 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5080 "raku.tab.c"
    break;

  case 347: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1413 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5089 "raku.tab.c"
    break;

  case 348: /* call_expr: IDENT '.' CARET IDENT  */
#line 1418 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5099 "raku.tab.c"
    break;

  case 349: /* call_expr: atom '.' CARET IDENT  */
#line 1424 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5109 "raku.tab.c"
    break;

  case 350: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1430 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5120 "raku.tab.c"
    break;

  case 351: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1437 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5131 "raku.tab.c"
    break;

  case 352: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1444 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5140 "raku.tab.c"
    break;

  case 353: /* call_expr: atom '.' IDENT  */
#line 1449 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5149 "raku.tab.c"
    break;

  case 354: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1454 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5160 "raku.tab.c"
    break;

  case 355: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1461 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5169 "raku.tab.c"
    break;

  case 356: /* call_expr: call_expr '.' IDENT  */
#line 1466 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5178 "raku.tab.c"
    break;

  case 357: /* call_expr: KW_DIE expr  */
#line 1471 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5184 "raku.tab.c"
    break;

  case 358: /* call_expr: KW_MAP closure expr  */
#line 1473 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5190 "raku.tab.c"
    break;

  case 359: /* call_expr: KW_GREP closure expr  */
#line 1475 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5196 "raku.tab.c"
    break;

  case 360: /* call_expr: KW_SORT expr  */
#line 1477 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5202 "raku.tab.c"
    break;

  case 361: /* call_expr: KW_SORT closure expr  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5208 "raku.tab.c"
    break;

  case 362: /* call_expr: atom  */
#line 1480 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5214 "raku.tab.c"
    break;

  case 363: /* arg_list: expr  */
#line 1483 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5220 "raku.tab.c"
    break;

  case 364: /* arg_list: arg_list ',' expr  */
#line 1484 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5226 "raku.tab.c"
    break;

  case 365: /* atom: LIT_INT  */
#line 1487 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5232 "raku.tab.c"
    break;

  case 366: /* atom: LIT_FLOAT  */
#line 1488 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5238 "raku.tab.c"
    break;

  case 367: /* atom: LIT_STR  */
#line 1489 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5244 "raku.tab.c"
    break;

  case 368: /* atom: WORDLIST  */
#line 1491 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5256 "raku.tab.c"
    break;

  case 369: /* atom: LIT_INTERP_STR  */
#line 1498 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5262 "raku.tab.c"
    break;

  case 370: /* atom: VAR_SCALAR  */
#line 1499 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5268 "raku.tab.c"
    break;

  case 371: /* atom: VAR_ARRAY  */
#line 1500 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5274 "raku.tab.c"
    break;

  case 372: /* atom: VAR_HASH  */
#line 1501 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5280 "raku.tab.c"
    break;

  case 373: /* atom: VAR_CAPTURE  */
#line 1503 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5288 "raku.tab.c"
    break;

  case 374: /* atom: VAR_FH  */
#line 1507 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5296 "raku.tab.c"
    break;

  case 375: /* atom: VAR_NAMED_CAPTURE  */
#line 1511 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5303 "raku.tab.c"
    break;

  case 376: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1514 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5309 "raku.tab.c"
    break;

  case 377: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1516 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5315 "raku.tab.c"
    break;

  case 378: /* atom: VAR_HASH '{' expr '}'  */
#line 1518 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5321 "raku.tab.c"
    break;

  case 379: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1520 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5327 "raku.tab.c"
    break;

  case 380: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1522 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5333 "raku.tab.c"
    break;

  case 381: /* atom: IDENT  */
#line 1523 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5339 "raku.tab.c"
    break;

  case 382: /* atom: VAR_TWIGIL  */
#line 1525 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5347 "raku.tab.c"
    break;

  case 383: /* atom: VAR_ARRAY_TWIGIL  */
#line 1529 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5355 "raku.tab.c"
    break;

  case 384: /* atom: VAR_HASH_TWIGIL  */
#line 1533 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5363 "raku.tab.c"
    break;

  case 385: /* atom: '(' ')'  */
#line 1536 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5369 "raku.tab.c"
    break;

  case 386: /* atom: '(' expr ')'  */
#line 1537 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5375 "raku.tab.c"
    break;

  case 387: /* atom: '(' expr ',' ')'  */
#line 1539 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5381 "raku.tab.c"
    break;

  case 388: /* atom: '(' expr ',' arg_list ')'  */
#line 1541 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5388 "raku.tab.c"
    break;

  case 389: /* atom: block  */
#line 1543 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5394 "raku.tab.c"
    break;

  case 390: /* atom: KW_SUB block  */
#line 1544 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5400 "raku.tab.c"
    break;


#line 5404 "raku.tab.c"

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

#line 1546 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
