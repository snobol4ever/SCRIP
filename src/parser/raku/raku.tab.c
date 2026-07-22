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
  YYSYMBOL_OP_AND = 82,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 83,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 84,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 85,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 86,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 87,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 88,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 89,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 90,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 91,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 92,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 93,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 94,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 95,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 96,                    /* OP_DOR  */
  YYSYMBOL_OP_DIV = 97,                    /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 98,                  /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 99,                 /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 100,                   /* OP_POW  */
  YYSYMBOL_101_ = 101,                     /* '='  */
  YYSYMBOL_102_ = 102,                     /* '!'  */
  YYSYMBOL_103_ = 103,                     /* '<'  */
  YYSYMBOL_104_ = 104,                     /* '>'  */
  YYSYMBOL_105_ = 105,                     /* '|'  */
  YYSYMBOL_106_ = 106,                     /* '&'  */
  YYSYMBOL_107_ = 107,                     /* '~'  */
  YYSYMBOL_108_ = 108,                     /* '+'  */
  YYSYMBOL_109_ = 109,                     /* '-'  */
  YYSYMBOL_110_ = 110,                     /* '*'  */
  YYSYMBOL_111_ = 111,                     /* '/'  */
  YYSYMBOL_112_ = 112,                     /* '%'  */
  YYSYMBOL_UMINUS = 113,                   /* UMINUS  */
  YYSYMBOL_114_ = 114,                     /* '.'  */
  YYSYMBOL_115_ = 115,                     /* ';'  */
  YYSYMBOL_116_ = 116,                     /* ','  */
  YYSYMBOL_117_ = 117,                     /* '('  */
  YYSYMBOL_118_ = 118,                     /* ')'  */
  YYSYMBOL_119_ = 119,                     /* '['  */
  YYSYMBOL_120_ = 120,                     /* ']'  */
  YYSYMBOL_121_ = 121,                     /* '{'  */
  YYSYMBOL_122_ = 122,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 123,                 /* $accept  */
  YYSYMBOL_program = 124,                  /* program  */
  YYSYMBOL_stmt_list = 125,                /* stmt_list  */
  YYSYMBOL_stmt = 126,                     /* stmt  */
  YYSYMBOL_if_stmt = 127,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 128,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 129,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 130,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 131,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 132,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 133,                /* loop_stmt  */
  YYSYMBOL_for_stmt = 134,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 135,               /* given_stmt  */
  YYSYMBOL_when_list = 136,                /* when_list  */
  YYSYMBOL_sub_decl = 137,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 138,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 139,                 /* sub_body  */
  YYSYMBOL_method_body = 140,              /* method_body  */
  YYSYMBOL_class_decl = 141,               /* class_decl  */
  YYSYMBOL_role_decl = 142,                /* role_decl  */
  YYSYMBOL_is_clauses = 143,               /* is_clauses  */
  YYSYMBOL_class_body_list = 144,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 145,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 146,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 147,           /* named_arg_list  */
  YYSYMBOL_pair_list = 148,                /* pair_list  */
  YYSYMBOL_param_list = 149,               /* param_list  */
  YYSYMBOL_block = 150,                    /* block  */
  YYSYMBOL_closure = 151,                  /* closure  */
  YYSYMBOL_expr = 152,                     /* expr  */
  YYSYMBOL_tern_expr = 153,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 154,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 155,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 156,                 /* dor_expr  */
  YYSYMBOL_range_expr = 157,               /* range_expr  */
  YYSYMBOL_add_expr = 158,                 /* add_expr  */
  YYSYMBOL_repl_expr = 159,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 160,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 161,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 162,               /* unary_expr  */
  YYSYMBOL_pow_expr = 163,                 /* pow_expr  */
  YYSYMBOL_postfix_expr = 164,             /* postfix_expr  */
  YYSYMBOL_call_expr = 165,                /* call_expr  */
  YYSYMBOL_arg_list = 166,                 /* arg_list  */
  YYSYMBOL_atom = 167                      /* atom  */
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
#define YYLAST   3338

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  123
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  386
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  994

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   356


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
       2,     2,     2,   102,     2,     2,     2,   112,   106,     2,
     117,   118,   110,   108,   116,   109,   114,   111,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   115,
     103,   101,   104,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   119,     2,   120,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,   105,   122,   107,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   113
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
    1294,  1295,  1296,  1302,  1308,  1314,  1317,  1318,  1319,  1322,
    1324,  1327,  1328,  1329,  1332,  1333,  1336,  1337,  1340,  1341,
    1342,  1345,  1346,  1347,  1348,  1349,  1352,  1353,  1354,  1355,
    1358,  1359,  1361,  1363,  1368,  1369,  1374,  1376,  1382,  1388,
    1392,  1396,  1401,  1408,  1413,  1419,  1425,  1432,  1439,  1444,
    1449,  1456,  1461,  1466,  1468,  1470,  1472,  1474,  1476,  1479,
    1480,  1483,  1484,  1485,  1486,  1494,  1495,  1496,  1497,  1498,
    1502,  1506,  1509,  1511,  1513,  1515,  1517,  1519,  1520,  1524,
    1528,  1532,  1533,  1534,  1536,  1539,  1540
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
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND",
  "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX",
  "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['",
  "']'", "'{'", "'}'", "$accept", "program", "stmt_list", "stmt",
  "if_stmt", "elsif_tail", "while_stmt", "unless_stmt", "until_stmt",
  "repeat_stmt", "loop_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "scalar_methcall", "sub_body", "method_body", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "pair_list", "param_list",
  "block", "closure", "expr", "tern_expr", "cmp_expr", "jct_expr",
  "dor_expr", "range_expr", "add_expr", "repl_expr", "addsub_expr",
  "mul_expr", "unary_expr", "pow_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-623)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-386)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -623,    26,  1444,  -623,  -623,  -623,  -623,  -623,   557,  -106,
     -45,   -49,   440,  -623,  -623,  3217,  -623,  -623,  -623,    71,
    1525,   372,  2497,  2561,  2617,  2681,  2737,    -8,    76,  2737,
    1581,    67,  2737,   125,   291,  2801,  2857,    76,    76,    46,
      56,    87,    87,  2921,    76,    76,  2737,   129,   211,    66,
    -623,   222,   317,   358,  3217,  3217,  -623,  1650,   329,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,   375,
    -623,  -623,  -623,   822,   483,  -623,   463,   444,   262,  -623,
      10,   293,    65,   326,  -623,  -623,   321,   298,   318,   437,
     355,   377,  2737,  2737,  2737,  2737,  2737,  2737,   545,  1706,
    2737,   551,  2737,  2737,   192,  1771,   439,   520,   -43,  -623,
      76,  -623,  -623,   473,    35,  -623,  1827,  -623,   172,   131,
     224,   251,   430,   604,    95,  1650,   428,  1650,    16,  1650,
      76,  1650,    76,   178,   271,    78,  -623,  -623,   538,  -623,
     553,   575,   636,   618,    13,   135,  1650,    76,  1650,    76,
    -623,  -623,  2737,  2737,  -623,  2737,  2737,  -623,  2737,  2737,
    2737,  1892,  2737,  -623,   697,  -623,  -623,  -623,   632,   645,
      59,   659,   668,   676,  -623,  -623,  -623,   230,   644,   964,
    2737,  2737,  2737,  2737,  2737,  2737,  2737,  2737,  -623,  2737,
    2737,  2737,  2737,  2737,  2737,  2737,  2737,  -623,  3217,  3217,
    3217,  3217,  3217,  3217,  3217,  3217,  3217,  3217,  3217,   608,
    3217,  3217,  3217,  3217,  3217,  3217,  3217,  3217,  3217,  3217,
    3217,  3217,  3217,  3217,   793,   735,   528,  -623,  -623,   718,
     744,   755,   773,   796,   801,   160,  -623,   651,   788,   753,
     723,   806,   766,   877,   814,  -623,   654,  2737,   921,  2737,
    -623,  2737,   824,   230,   655,  -623,  2737,  2737,  -623,  2977,
    -623,  3041,  -623,   315,   324,   348,   258,   839,   840,   679,
    2737,  2737,  2737,  2737,  2737,  2737,  2737,  -623,  2737,   682,
    2737,  2737,  -623,  2737,   683,   433,   686,  -623,   932,  2737,
    -623,  3217,  3217,    -3,  -623,  -623,  -623,  -623,  2737,  2737,
    -623,   929,  2737,   930,  2737,   687,   926,   690,  -623,   841,
     842,   843,   846,   832,  -623,  -623,  -623,    76,    24,  -623,
       0,   838,   845,  -623,  -623,  -623,  1957,  -623,  -623,   695,
     847,   141,   870,  2497,  2561,  2737,   558,    85,   200,  -623,
      27,   857,   858,   876,   886,   887,   888,   889,   905,   906,
     907,   910,   911,   912,   914,   915,   916,   374,   374,   374,
     374,   374,   374,   948,   374,   374,   374,   374,  -623,  -623,
    -623,  -623,  -623,  -623,   885,   885,   293,    65,   326,   326,
    -623,  -623,  -623,  -623,  -623,   917,   918,  1022,  -623,  2013,
    -623,  -623,  -623,  -623,  -623,  -623,  2737,  2078,  -623,   936,
     937,   938,  -623,  2134,  -623,  2199,  -623,   920,   939,   919,
    -623,  -623,   927,  -623,   931,  1650,   294,   972,   114,  2255,
     422,   933,  2737,  -623,  3097,  -623,  2737,  -623,     2,  -623,
    2737,  2737,  1957,   934,   935,   940,   941,   942,   943,   944,
     467,  1957,   945,   946,   513,    76,    14,  3161,  -623,    76,
      76,   214,   -60,   -57,  -623,     9,   924,   702,  1084,   947,
     949,    50,   959,   950,   961,   952,    76,    76,    76,  -623,
    -623,  -623,  -623,  -623,  -623,  1037,  1038,  -623,     8,   924,
     703,    15,   474,   171,  -623,   708,  1053,  2737,  2737,  2737,
     194,    53,    81,  -623,   204,  2737,  2737,  -623,  2737,  2737,
    -623,  2737,  2737,  2737,  2737,  2737,  2737,  2737,  2737,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,  3217,  2320,  2376,  -623,
     953,   709,   954,  -623,   712,  2737,  2737,  2737,  -623,   716,
     203,  -623,   725,   726,  -623,  -623,  -623,  -623,  -623,   730,
    2737,  -623,  2737,  2737,  2737,   733,  -623,   253,  -623,   955,
    1650,   566,   956,   924,   734,   960,   962,   737,  -623,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,   738,  -623,  -623,  -623,
     694,  -623,  -623,  1650,    76,  -623,  -623,  1066,  -623,  1068,
    1072,  -623,  1073,  1074,  -623,   506,   924,  2497,  2561,   714,
    -623,   311,  -623,  -623,  2737,    76,  -623,  -623,  -623,   976,
     977,  1085,  -623,  -623,  -623,  -623,   107,   112,   250,  1055,
    -623,  -623,   924,   924,   742,   747,  1079,  1107,  1108,  -623,
    -623,   184,  1003,  1002,   221,  2737,  2737,  2737,  2737,  2737,
    2737,  -623,  -623,  -623,  -623,   234,   275,   312,   328,   337,
     347,   350,   351,   361,   367,   392,   393,  -623,  -623,   748,
    -623,   751,   759,  -623,  1025,  -623,  -623,  1026,  1027,  1030,
    -623,  2737,  1132,  -623,  -623,  1957,  1032,   661,  -623,  -623,
    1034,  1078,  1081,  -623,   760,  -623,  2737,  -623,  -623,   924,
    -623,  -623,  1036,  1039,    14,  -623,   763,   783,    76,    76,
      76,  -623,  -623,  -623,    41,  -623,   331,   196,  -623,   395,
    2737,  2737,  2737,  2737,  2737,  2737,  2737,  2737,  -623,    76,
    1033,  -623,  -623,    76,  -623,   484,   509,    23,    40,   741,
    1041,  1042,  1144,  -623,  -623,   924,   924,  1152,  1153,  1154,
    2737,  2441,  1061,  1062,  -623,   396,   406,   414,   438,   464,
     469,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,
    -623,  1092,   764,  -623,  -623,  -623,  2737,  2737,  1957,   699,
    -623,  -623,  -623,  -623,  -623,    76,    76,  -623,  -623,  -623,
    -623,  -623,  1155,  1156,  2737,  2737,  2737,  -623,  -623,  -623,
     470,   472,   478,   488,   489,   491,   499,   539,  -623,  -623,
    -623,    21,  1100,  -623,    22,  -623,  1157,  1158,  1054,  2737,
    -623,  1161,  1162,  1063,  2737,  -623,    47,   213,  1064,  1065,
    -623,  -623,   510,  -623,  -623,  -623,  -623,  -623,   540,  1046,
     768,  2737,  2737,  -623,  -623,  -623,  -623,  -623,  -623,  2737,
    1067,  -623,  -623,   771,  -623,   810,  -623,  -623,  -623,   541,
     542,   544,  -623,  -623,  -623,  -623,  -623,  -623,  -623,  -623,
    1049,   776,  1059,  1204,  1049,   791,  1069,  1070,  -623,  1075,
    1076,  1077,  -623,  1080,  1163,  1169,  1082,  2737,  -623,  1173,
    1174,  1083,  2737,  -623,  -623,  -623,    29,  -623,  -623,  -623,
    1089,   550,   555,  -623,  -623,  1087,    76,  -623,  -623,  -623,
    -623,  -623,  1049,  -623,  2497,  2561,  1324,  -623,   333,  -623,
    1049,  -623,  -623,  -623,  -623,  -623,  -623,  1088,  1097,  -623,
    1098,  1116,  1117,  -623,  1126,  1049,   799,  -623,  -623,  -623,
    -623,  -623,  -623,   359,   265,  -623,   579,  2737,  2737,  2737,
    2737,  2737,  2737,  2737,  2737,  -623,  -623,  -623,  -623,  -623,
    -623,  -623,  -623,  -623,  1049,  2737,  2737,  2737,  -623,  -623,
    -623,   580,   581,   582,   583,   584,   585,   601,   620,  -623,
     621,   625,   626,  -623,  -623,  -623,  -623,  -623,  -623,  -623,
    -623,  -623,  -623,  -623
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   361,   362,   363,   365,   366,   367,
     368,   378,   377,   379,   380,     0,   369,   370,   371,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,     0,     0,     0,    89,     0,     3,     4,
      90,    91,    98,    99,   100,   101,    92,    93,   108,     0,
     109,   110,   111,    97,     0,   289,   291,   305,   308,   310,
     313,   315,   317,   320,   325,   329,   331,   332,   358,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,   367,   368,   378,
       0,   385,   328,     0,   366,    29,     0,   359,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,   386,   288,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,     0,     0,   102,     0,     0,   103,     0,     0,
       0,     3,     0,   356,    94,    96,   353,   196,     0,     0,
       0,     0,     0,     0,   327,   326,   381,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,    85,     0,
       0,     0,     0,     0,   287,     0,   336,     0,     0,     0,
       0,     0,   341,     0,   340,   334,     0,     0,     0,     0,
      24,     0,   381,     0,     0,    32,     0,     0,     6,     0,
       7,     0,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    36,     0,     0,   115,     0,   127,     0,     0,
     141,     0,     0,     0,     3,   148,    39,    40,     0,     0,
     144,     0,     0,     0,     0,     0,   130,     0,   133,     0,
       0,     0,     0,     0,   354,   355,   357,     0,     0,   199,
       0,     0,     0,   234,    86,    87,     0,   382,   253,   366,
     367,   368,   378,     0,     0,     0,     0,     0,     0,   252,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,   295,   298,
     299,   292,   293,     0,   296,   297,   300,   301,   302,   303,
     304,   306,   307,   309,   311,   312,   314,   316,   318,   319,
     324,   321,   322,   323,   330,   352,   349,     0,    45,     0,
      79,    80,    81,    82,    83,    42,     0,     0,   335,   372,
     373,   374,    47,     0,   344,     0,   333,     0,     0,     0,
     287,    31,     0,   360,     0,     0,     0,   363,   377,     0,
       0,     0,     0,    21,     0,    22,     0,    23,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   382,     0,     0,   118,   382,
       0,     0,   311,   312,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   382,     0,   382,   104,
     105,   106,   107,   286,    95,     0,     0,   199,     0,     0,
       0,     0,     0,     0,   383,     0,     0,     0,     0,     0,
       0,     0,     0,   279,     0,     0,     0,   280,     0,     0,
     281,     0,     0,     0,     0,     0,     0,     0,     0,   254,
      49,    51,    53,    56,    54,    55,    50,    52,    62,    64,
      66,    69,    67,    68,    63,    65,     0,     0,     0,   345,
       0,     0,     0,   158,     0,     0,     0,     0,   343,     0,
     377,   339,     0,     0,   372,   373,   374,    30,     5,     0,
       0,     9,     0,     0,     0,     0,    14,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    70,    73,
      72,    76,    74,    75,    71,    34,     0,    77,    78,    37,
     112,   117,   116,     0,     0,   126,   138,     0,   140,     0,
       0,   245,     0,     0,   147,     0,     0,     0,     0,     0,
     159,     0,    26,    25,     0,     0,   142,   375,   376,     0,
       0,   128,   131,   132,   197,   198,     0,     0,     0,     0,
     195,   156,     0,     0,     0,     0,     0,     0,     0,   233,
     384,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   263,   270,   277,   278,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   290,   351,     0,
     348,     0,     0,    44,     0,    46,   157,     0,     0,     0,
     342,     0,     0,   337,   338,     0,     0,     0,   241,   240,
       0,     0,     0,    16,     0,    17,     0,    20,   150,     0,
      28,    27,   384,   384,     0,   119,     0,   120,     0,     0,
       0,   246,   247,   248,     0,   146,     0,     0,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   160,     0,
       0,    60,    61,     0,   194,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   153,     0,     0,     0,     0,     0,
       0,     0,   372,   374,   274,     0,     0,     0,     0,     0,
       0,   282,   283,   284,   285,   255,   257,   259,   262,   260,
     261,   256,   258,   350,   347,   346,    43,    57,    58,    59,
     238,     0,     0,    11,    10,    15,     0,     0,     0,     0,
     149,    35,    38,   114,   113,   382,     0,   124,   139,   136,
     137,   249,     0,     0,     0,     0,     0,   163,   164,   161,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   143,
     129,     0,     3,   226,     0,   229,     0,     0,     0,     0,
     205,     0,     0,     0,     0,   200,     0,     0,     0,     0,
     201,   202,     0,   152,   154,   235,   236,   237,     0,   158,
       0,     0,     0,   264,   266,   269,   267,   268,   265,     0,
     384,   243,   242,     0,    18,   121,   122,   250,   251,     0,
       0,     0,   165,   167,   169,   172,   170,   171,   166,   168,
       0,     0,     0,     0,     0,     0,     0,     0,   216,     0,
       0,     0,   217,     0,     0,     0,     0,     0,   207,     0,
       0,     0,     0,   206,   203,   204,     0,   232,   273,   272,
     157,     0,     0,   239,    12,   384,     0,   125,   173,   175,
     174,   225,     0,   177,     0,     0,     0,   176,     0,   228,
       0,   214,   215,   209,   212,   213,   208,     0,     0,   218,
       0,     0,     0,   219,     0,     0,     0,   271,   275,   276,
      19,   123,   224,     0,     0,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,   227,   222,   223,   211,
     220,   221,   210,   231,     0,     0,     0,     0,   181,   182,
     179,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   183,   185,   187,   190,   188,   189,   184,
     186,   191,   193,   192
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -623,  -623,     1,  -623,  -443,  -575,  -623,  -623,  -623,  -623,
    -623,  -623,  -623,  -623,  -623,  -623,  -258,  -622,  -623,  -623,
    -623,   717,  -623,  -623,   672,   770,  -316,   174,   881,    -2,
    -202,  -623,   485,  -623,   549,    -9,   981,   988,   720,    -6,
    -623,  -623,  -623,     3,  -623
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   179,    59,    60,   448,    61,    62,    63,    64,
      65,    66,    67,   461,    68,    69,   295,   813,    70,    71,
     318,   478,    72,   483,   542,   420,   457,   111,   159,   117,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   363,   581,   480,   695,   135,   454,   429,   112,
     454,   455,   454,   100,   455,   589,   455,   134,   590,   591,
     126,   128,   130,   132,   133,   454,     3,   138,   140,   455,
     143,   454,   454,   147,   149,   455,   455,   816,   475,   454,
      24,   163,   280,   455,   166,   817,   476,   215,   174,   175,
     215,   791,   103,   501,   821,   177,   502,   503,   101,   281,
     248,   884,   822,   504,   617,   618,   505,   506,   619,   885,
     507,   508,   152,   321,   592,   593,   102,   141,   249,   280,
     169,   142,   155,   213,   214,   113,   818,   604,   605,   153,
     229,   230,   231,   232,   233,   234,   281,   170,   238,   156,
     240,   241,   237,   823,   815,   267,   792,   793,   246,   268,
     886,   495,   564,    58,   253,   456,   301,   215,   479,   254,
     563,   322,   787,   269,   819,   279,   725,   284,   496,   286,
     620,   282,   283,   623,   302,    58,   251,   144,   820,   870,
     874,   824,   197,   167,   305,   477,   307,   935,   887,   509,
     309,   310,    99,   311,   312,   825,   313,   314,   315,   313,
     316,   154,   888,   617,   618,   624,   625,   619,   282,   283,
     726,   157,   606,   217,   218,   642,    73,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   554,   349,   350,   351,
     352,   353,   354,   355,   356,   293,   296,    58,   594,   294,
     154,   136,   137,   643,   374,   375,   242,   497,   158,   243,
     897,   150,   151,   380,   381,   382,   383,   384,   164,   165,
     635,   621,   280,   271,   636,   168,   498,   889,   104,   724,
     637,   105,   257,   638,   639,   890,   171,   640,   303,   281,
     626,   627,   628,   499,   101,   407,   258,   409,   911,   410,
     244,   783,   919,   288,   413,   414,   304,   416,   681,   421,
     727,   396,   488,   728,   729,   730,   731,   682,   433,   434,
     435,   436,   437,   438,   439,   671,   891,   397,   442,   443,
     907,   440,   452,   453,   136,   740,   444,   255,   256,   587,
     942,   280,   451,   629,   289,   458,   459,   460,   956,    58,
     463,   741,   465,   145,   285,   688,   287,   290,   281,   277,
     278,   282,   283,   963,   892,   157,   641,   104,   798,   297,
     105,   306,   500,   308,   657,   259,   644,   172,   893,   485,
     256,   490,   491,   492,   494,    58,   402,   710,   705,   260,
     711,   712,   979,   744,   291,   292,   326,   713,   327,   469,
     714,   715,   261,    73,   716,   717,   751,   794,   212,   947,
     271,   795,   948,   949,   733,   734,   262,   273,   173,   950,
     274,   275,   951,   952,   796,   428,   953,   954,   215,   294,
     282,   283,   119,   120,   121,   965,   122,   969,   271,   966,
     470,   216,   531,   550,   532,   273,   178,   752,   274,   275,
     534,   180,   967,   123,   181,   182,   539,   124,   543,   551,
     552,   183,   224,   549,   184,   185,   422,   177,   186,   187,
     559,   223,   561,   219,   562,   424,   197,   471,   565,   566,
     423,   780,   225,   718,   753,   567,   220,   221,   222,   425,
     263,   264,   265,   472,   576,   584,   277,   278,   197,   426,
     754,   226,   518,   797,   270,   955,   601,   271,   272,   755,
     446,   447,   519,   427,   273,   520,   521,   274,   275,   756,
     227,   276,   757,   758,   277,   278,   522,   833,   834,   210,
     211,   968,   523,   759,   454,   632,   633,   634,   455,   760,
     188,   474,   228,   645,   646,   871,   647,   648,   875,   649,
     650,   651,   652,   653,   654,   655,   656,   524,   525,   189,
     297,   568,   190,   191,   761,   762,   703,   799,   843,   192,
     704,   570,   193,   194,   207,   208,   195,   196,   844,   571,
     659,   662,   209,   667,   668,   669,   845,   556,   557,   198,
     199,   200,   201,   277,   278,   202,   203,   204,   676,   210,
     211,   678,   679,   572,   104,   677,    99,   105,   684,   235,
     846,     4,     5,     6,     7,   239,   205,   206,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,   573,
     936,   696,   575,   256,   574,   518,   847,   519,   250,   110,
      28,   848,   862,   520,   863,   706,   707,   709,   197,    33,
     864,   811,   719,   521,   522,   812,   523,    41,    42,    43,
     865,   866,    46,   867,   524,   368,   369,   370,   266,   580,
     582,   868,    50,   585,   586,   588,   814,   896,   579,   256,
     812,   812,    73,   745,   746,   747,   748,   749,   750,   247,
     611,   612,   613,   388,    89,   389,    90,    91,    92,    93,
      94,    95,    96,   296,   525,   665,   568,   570,    97,   574,
      54,   869,   898,   908,   909,   767,   910,    55,   297,   770,
     769,    98,   938,   139,    99,    57,   298,   939,   772,    58,
     493,   685,   686,   357,   358,   359,   360,   361,   362,   779,
     364,   365,   366,   367,   297,   518,   519,   520,   521,   522,
     523,   970,   983,   984,   985,   986,   987,   988,   800,   801,
     802,   803,   804,   805,   806,   807,   524,     4,     5,     6,
       7,   694,   447,   989,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,   525,   568,   299,   838,   300,
     570,   574,   990,   991,   840,   110,    28,   992,   993,   386,
     317,   826,   387,   319,   827,    33,   828,   829,   697,   371,
     372,   373,   320,    41,    42,    43,   328,   256,    46,   398,
     256,   256,   406,   412,   851,   852,   774,   256,    50,   720,
     323,   853,    89,   324,    90,    91,    92,    93,    94,    95,
      96,   325,   859,   860,   861,   432,    97,   327,   441,   326,
     327,   445,   326,   326,   449,   466,   326,   385,   468,   486,
     786,   447,    99,   873,   854,   256,    54,   879,   595,   595,
     596,   622,   883,    55,   256,   256,   630,   664,   256,   139,
     666,    57,   256,   390,   670,    58,   708,   906,   447,   901,
     902,   672,   256,   673,   674,   401,   675,   903,   327,   557,
     595,   680,   689,   256,   256,   692,   693,   400,   595,   391,
     735,  -385,  -385,   595,   256,   736,   763,   672,   784,   764,
     392,   918,   788,   789,   790,   256,   778,   765,   327,   326,
     256,   785,   850,   403,   256,   930,   900,   256,   393,   905,
     934,   404,   595,   808,   912,  -385,  -385,   810,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,  -385,   595,   399,   920,
    -385,   394,   943,   944,   946,   595,   395,   964,  -385,  -385,
    -385,   402,  -385,   160,   162,  -385,  -385,  -385,  -385,  -385,
    -385,   405,  -385,  -385,  -385,   408,  -385,   378,   379,   411,
     430,   431,   450,   462,   464,   971,   972,   973,   974,   975,
     976,   977,   978,   467,   473,   481,   469,   470,   471,   855,
     856,   472,   482,   980,   981,   982,   487,     4,     5,     6,
       7,   489,   510,   511,   329,   330,   331,   332,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   333,   334,
      24,   512,   215,    25,    26,    27,    28,   335,   336,    31,
      32,   513,   514,   515,   516,    33,    34,    35,    36,    37,
      38,   337,   338,    41,    42,    43,    44,    45,    46,    47,
     517,   518,   519,    48,    49,   520,   521,   522,    50,   523,
     524,   525,    51,   526,   527,   528,   529,   535,   536,   537,
     544,   546,   547,   545,   553,   294,   548,    73,   558,   568,
     569,   614,   615,    52,    53,   570,   571,   572,   573,   574,
     577,   578,   602,   607,   603,   609,    54,   631,   663,   665,
     683,   687,   608,    55,   610,   690,   698,   691,   699,    56,
     941,    57,   700,   701,   702,    58,   339,     4,     5,     6,
       7,   721,   722,   737,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   597,   598,
      24,   732,   723,    25,    26,    27,    28,    29,   599,    31,
      32,   738,   739,   742,   743,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
     766,   767,   768,    48,    49,   769,   771,   773,    50,   775,
     776,   781,    51,   777,   782,   809,   830,   831,   832,   835,
     836,   837,   841,   842,   849,   857,   858,   872,   899,   878,
     812,   876,   877,    52,    53,   880,   881,   927,   882,   894,
     895,   913,   904,   928,   921,   922,    54,   931,   932,   555,
     923,   924,   925,    55,   616,   926,   376,   929,   933,    56,
     661,    57,   940,   957,   377,    58,   600,     4,     5,     6,
       7,   937,   958,   959,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   914,   915,
      24,   960,   961,    25,    26,    27,    28,    29,   916,    31,
      32,   962,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,    56,
       0,    57,     0,     0,     0,    58,   917,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,   139,
       0,    57,     0,     0,     0,    58,   945,     4,     5,     6,
       7,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,    25,    26,    27,    28,    29,    30,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,    52,    53,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,   110,    28,     0,    56,
       0,    57,     0,     0,     0,    58,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,     0,     0,    46,
       0,     0,     0,     0,     4,     5,     6,     7,     0,    50,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
       0,     0,    33,     0,     0,     0,     0,    54,     0,     0,
      41,    42,    43,     0,    55,    46,     0,     0,     0,     0,
     115,     0,   116,     0,     0,    50,    58,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,    54,     0,     0,     0,     0,     0,     0,
      55,    33,     0,     0,     0,     0,   139,     0,    57,    41,
      42,    43,    58,     0,    46,     0,     0,     0,     0,     4,
       5,     6,     7,     0,    50,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    54,     0,     0,    41,    42,    43,     0,    55,
      46,     0,     0,     0,     0,     0,     0,    57,   176,     0,
      50,    58,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,    54,     0,
       0,     0,    33,     0,     0,    55,     0,     0,     0,     0,
      41,    42,    43,    57,   236,    46,     0,    58,     0,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,    57,   245,
       0,    50,    58,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,    54,
       0,     0,     0,    33,     0,     0,    55,     0,     0,     0,
       0,    41,    42,    43,    57,   252,    46,     0,    58,     0,
       0,     0,     0,     0,     0,     0,    50,     0,     0,   178,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,    54,     0,     0,     0,    33,     0,
       0,    55,     0,     0,     0,     0,    41,    42,    43,    57,
       0,    46,     0,    58,     0,     0,     4,     5,     6,     7,
       0,    50,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,     0,    54,
       0,     0,    41,    42,    43,     0,    55,    46,     0,     0,
       0,     0,     0,     0,    57,   484,     0,    50,    58,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,    54,     0,     0,     0,    33,
       0,     0,    55,     0,     0,     0,     0,    41,    42,    43,
      57,   530,    46,     0,    58,     0,     0,     4,     5,     6,
       7,     0,    50,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
      54,     0,     0,    41,    42,    43,     0,    55,    46,     0,
       0,     0,     0,     0,     0,    57,   533,     0,    50,    58,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,   540,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,    54,     0,     0,     0,
      33,     0,     0,    55,     0,     0,     0,     0,    41,    42,
      43,    57,   538,    46,     0,    58,     0,     0,     4,     5,
     417,     7,     0,    50,     0,   114,   107,   108,   109,   418,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,     0,     0,    33,     0,     0,     0,
       0,    54,     0,     0,    41,    42,    43,     0,    55,    46,
       0,     0,     0,     0,     0,     0,    57,   541,     0,    50,
      58,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,    54,     0,     0,
       0,    33,     0,     0,    55,     0,     0,     0,     0,    41,
      42,    43,    57,   176,    46,     0,    58,     0,     0,     4,
       5,     6,     7,     0,    50,     0,   114,   107,   108,   109,
     540,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    54,     0,     0,    41,    42,    43,     0,    55,
      46,     0,     0,     0,     0,     0,     0,    57,   658,     0,
      50,    58,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,    54,     0,
       0,     0,    33,     0,     0,    55,     0,     0,     0,     0,
      41,    42,    43,    57,   660,    46,     0,    58,     0,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,    57,   839,
       0,    50,    58,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,   125,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,   127,     0,
       0,    50,    58,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,   129,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,   131,     0,
       0,    50,    58,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,    57,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,   146,     0,
       0,    50,    58,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,   148,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,    57,     0,
       0,    50,   161,     0,     4,     5,   417,     7,     0,     0,
       0,   114,   107,   108,   109,   418,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,   415,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,   419,     0,
       0,    50,    58,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,     0,   560,    46,     0,     0,    58,     0,
       4,     5,     6,     7,     0,    50,     0,   106,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,     0,
       0,     0,     0,    54,     0,     0,    41,    42,    43,     0,
      55,    46,     0,     0,     0,     0,     0,     0,   583,     0,
       0,    50,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   204,   446,   320,   580,    14,    10,   266,    15,
      10,    14,    10,   119,    14,    75,    14,    26,    75,    10,
      22,    23,    24,    25,    26,    10,     0,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    14,    14,    10,
      26,    43,    26,    14,    46,    22,    22,   107,    54,    55,
     107,    10,   101,    26,    14,    57,    29,    30,   103,    43,
     103,    14,    22,    36,    56,    57,    39,    40,    60,    22,
      43,    44,    26,    14,    65,    66,   121,    10,   121,    26,
      14,    14,    26,    73,    74,    14,    63,    37,    38,    43,
      92,    93,    94,    95,    96,    97,    43,    31,   100,    43,
     102,   103,    99,    63,   726,    10,    65,    66,   105,    14,
      63,    26,   428,   121,   116,   118,   103,   107,   118,   116,
     118,    62,   697,   125,   101,   127,    14,   129,    43,   131,
     122,   115,   116,   118,   121,   121,   101,    12,   115,   118,
     118,   101,   115,    14,   146,   121,   148,   118,   101,   122,
     152,   153,   117,   155,   156,   115,   158,   159,   160,   161,
     162,   115,   115,    56,    57,   481,   482,    60,   115,   116,
      58,   115,   122,   108,   109,   122,     2,   179,   180,   181,
     182,   183,   184,   185,   186,   187,    72,   189,   190,   191,
     192,   193,   194,   195,   196,   117,   115,   121,   456,   121,
     115,    27,    28,   122,   213,   214,    14,   122,   121,    17,
     832,    37,    38,   219,   220,   221,   222,   223,    44,    45,
      26,   479,    26,    29,    30,    14,    26,    14,   114,   122,
      36,   117,   101,    39,    40,    22,    14,    43,   103,    43,
      69,    70,    71,    43,   103,   247,   115,   249,   870,   251,
      58,   694,   874,    75,   256,   257,   121,   259,     5,   261,
      10,   101,   121,    13,    14,    15,    16,    14,   270,   271,
     272,   273,   274,   275,   276,    72,    63,   117,   280,   281,
     855,   278,   291,   292,   110,   101,   283,   115,   116,    75,
     912,    26,   289,   122,   116,   294,   298,   299,   920,   121,
     302,   117,   304,    12,   130,   563,   132,   133,    43,   115,
     116,   115,   116,   935,   101,   115,   122,   114,   122,   115,
     117,   147,   122,   149,   526,   101,   122,    10,   115,   326,
     116,   333,   334,   335,   336,   121,   115,    26,   596,   115,
      29,    30,   964,   122,    73,    74,   116,    36,   118,   115,
      39,    40,   101,   179,    43,    44,   122,    26,    96,    26,
      29,    30,    29,    30,   622,   623,   115,    36,    10,    36,
      39,    40,    39,    40,    43,   117,    43,    44,   107,   121,
     115,   116,    10,    11,    12,    26,    14,   122,    29,    30,
     115,    98,   389,    99,   396,    36,    67,   122,    39,    40,
     397,    26,    43,    31,    29,    30,   403,    35,   405,   115,
     116,    36,   114,   415,    39,    40,   101,   419,    43,    44,
     422,   100,   424,    97,   426,   101,   115,   115,   430,   431,
     115,   689,   114,   122,   122,   432,   110,   111,   112,   115,
      10,    11,    12,   115,   441,   447,   115,   116,   115,   101,
     122,    14,   115,   122,    26,   122,   458,    29,    30,   122,
      27,    28,   115,   115,    36,   115,   115,    39,    40,   122,
     115,    43,   122,   122,   115,   116,   115,   735,   736,   105,
     106,   122,   115,   122,    10,   487,   488,   489,    14,   122,
     115,   317,   115,   495,   496,   811,   498,   499,   814,   501,
     502,   503,   504,   505,   506,   507,   508,   115,   115,    26,
     115,   115,    29,    30,   122,   122,    10,   122,   122,    36,
      14,   115,    39,    40,    80,    81,    43,    44,   122,   115,
     527,   528,    88,   535,   536,   537,   122,   115,   116,    76,
      77,    78,    79,   115,   116,    82,    83,    84,   550,   105,
     106,   553,   554,   115,   114,   552,   117,   117,   560,    14,
     122,     3,     4,     5,     6,    14,   103,   104,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   115,
     896,   583,   115,   116,   115,   115,   122,   115,   115,    31,
      32,   122,   122,   115,   122,   597,   598,   599,   115,    41,
     122,   117,   604,   115,   115,   121,   115,    49,    50,    51,
     122,   122,    54,   122,   115,     7,     8,     9,    14,   445,
     446,   122,    64,   449,   450,   451,   117,   117,   115,   116,
     121,   121,   458,   635,   636,   637,   638,   639,   640,   119,
     466,   467,   468,   115,    87,   117,    89,    90,    91,    92,
      93,    94,    95,   115,   115,   115,   115,   115,   101,   115,
     102,   122,   122,   122,   122,   115,   122,   109,   115,   671,
     115,   114,   122,   115,   117,   117,   101,   122,   675,   121,
     122,   115,   116,   198,   199,   200,   201,   202,   203,   686,
     205,   206,   207,   208,   115,   115,   115,   115,   115,   115,
     115,   122,   122,   122,   122,   122,   122,   122,   710,   711,
     712,   713,   714,   715,   716,   717,   115,     3,     4,     5,
       6,    27,    28,   122,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   115,   115,   101,   740,   121,
     115,   115,   122,   122,   741,    31,    32,   122,   122,    14,
      53,    10,    17,   121,    13,    41,    15,    16,   584,   210,
     211,   212,   117,    49,    50,    51,   122,   116,    54,   118,
     116,   116,   118,   118,   776,   777,   115,   116,    64,   605,
     121,   778,    87,   115,    89,    90,    91,    92,    93,    94,
      95,   115,   794,   795,   796,   116,   101,   118,   116,   116,
     118,   118,   116,   116,   118,   118,   116,    14,   118,   114,
      27,    28,   117,   812,   115,   116,   102,   819,   116,   116,
     118,   118,   824,   109,   116,   116,   118,   118,   116,   115,
     118,   117,   116,   115,   118,   121,   122,    27,    28,   841,
     842,   116,   116,   118,   118,   122,   116,   849,   118,   116,
     116,   118,   118,   116,   116,   118,   118,   104,   116,   115,
     118,    39,    40,   116,   116,   118,   118,   116,   694,   118,
     115,   873,   698,   699,   700,   116,   116,   118,   118,   116,
     116,   118,   118,   117,   116,   887,   118,   116,   115,   118,
     892,    14,   116,   719,   118,    73,    74,   723,    76,    77,
      78,    79,    80,    81,    82,    83,    84,   116,   120,   118,
      88,   115,   914,   915,   916,   116,   115,   118,    96,    97,
      98,   115,   100,    42,    43,   103,   104,   105,   106,   107,
     108,   117,   110,   111,   112,    14,   114,   217,   218,   115,
     101,   101,    10,    14,    14,   947,   948,   949,   950,   951,
     952,   953,   954,    27,   122,   117,   115,   115,   115,   785,
     786,   115,   117,   965,   966,   967,   119,     3,     4,     5,
       6,   101,   115,   115,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   115,   107,    29,    30,    31,    32,    33,    34,    35,
      36,   115,   115,   115,   115,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
     115,   115,   115,    59,    60,   115,   115,   115,    64,   115,
     115,   115,    68,    85,   117,   117,    14,   101,   101,   101,
     120,   122,   115,   104,    72,   121,   115,   873,   115,   115,
     115,    14,    14,    89,    90,   115,   115,   115,   115,   115,
     115,   115,   115,   104,   115,   104,   102,    14,   115,   115,
     115,   115,   122,   109,   122,   115,    10,   115,    10,   115,
     906,   117,    10,    10,    10,   121,   122,     3,     4,     5,
       6,   115,   115,    14,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    56,    27,    29,    30,    31,    32,    33,    34,    35,
      36,    14,    14,   120,   122,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
     115,   115,   115,    59,    60,   115,    14,   115,    64,   115,
      72,   115,    68,    72,   115,   122,   115,   115,    14,     7,
       7,     7,   101,   101,    72,    10,    10,    67,   122,   115,
     121,    14,    14,    89,    90,    14,    14,    14,   115,   115,
     115,   122,   115,    14,   115,   115,   102,    14,    14,   419,
     115,   115,   115,   109,   477,   115,   215,   115,   115,   115,
     528,   117,   115,   115,   216,   121,   122,     3,     4,     5,
       6,   122,   115,   115,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   115,   115,    29,    30,    31,    32,    33,    34,    35,
      36,   115,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,   121,   122,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,   121,   122,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    89,    90,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   102,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    31,    32,    -1,   115,
      -1,   117,    -1,    -1,    -1,   121,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,    54,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    64,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      49,    50,    51,    -1,   109,    54,    -1,    -1,    -1,    -1,
     115,    -1,   117,    -1,    -1,    64,   121,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,   102,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    41,    -1,    -1,    -1,    -1,   115,    -1,   117,    49,
      50,    51,   121,    -1,    54,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    64,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,   109,
      54,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      64,   121,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    41,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      49,    50,    51,   117,   118,    54,    -1,   121,    -1,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    64,   121,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    41,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    49,    50,    51,   117,   118,    54,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    41,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    49,    50,    51,   117,
      -1,    54,    -1,   121,    -1,    -1,     3,     4,     5,     6,
      -1,    64,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    49,    50,    51,    -1,   109,    54,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    64,   121,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    41,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    49,    50,    51,
     117,   118,    54,    -1,   121,    -1,    -1,     3,     4,     5,
       6,    -1,    64,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    49,    50,    51,    -1,   109,    54,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    64,   121,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      41,    -1,    -1,   109,    -1,    -1,    -1,    -1,    49,    50,
      51,   117,   118,    54,    -1,   121,    -1,    -1,     3,     4,
       5,     6,    -1,    64,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    49,    50,    51,    -1,   109,    54,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    64,
     121,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    41,    -1,    -1,   109,    -1,    -1,    -1,    -1,    49,
      50,    51,   117,   118,    54,    -1,   121,    -1,    -1,     3,
       4,     5,     6,    -1,    64,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,   109,
      54,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      64,   121,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    41,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      49,    50,    51,   117,   118,    54,    -1,   121,    -1,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    41,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      49,    50,    51,    -1,   117,    54,    -1,    -1,   121,    -1,
       3,     4,     5,     6,    -1,    64,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    49,    50,    51,    -1,
     109,    54,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    64,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   124,   125,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      64,    68,    89,    90,   102,   109,   115,   117,   121,   126,
     127,   129,   130,   131,   132,   133,   134,   135,   137,   138,
     141,   142,   145,   150,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   167,    87,
      89,    90,    91,    92,    93,    94,    95,   101,   114,   117,
     119,   103,   121,   101,   114,   117,    10,    11,    12,    13,
      31,   150,   162,    14,    10,   115,   117,   152,   166,    10,
      11,    12,    14,    31,    35,   117,   152,   117,   152,   117,
     152,   117,   152,   152,   158,    14,   150,   150,   152,   115,
     152,    10,    14,   152,    12,    12,   117,   152,   117,   152,
     150,   150,    26,    43,   115,    26,    43,   115,   121,   151,
     151,   121,   151,   152,   150,   150,   152,    14,    14,    14,
      31,    14,    10,    10,   162,   162,   118,   152,    67,   125,
      26,    29,    30,    36,    39,    40,    43,    44,   115,    26,
      29,    30,    36,    39,    40,    43,    44,   115,    76,    77,
      78,    79,    82,    83,    84,   103,   104,    80,    81,    88,
     105,   106,    96,    73,    74,   107,    98,   108,   109,    97,
     110,   111,   112,   100,   114,   114,    14,   115,   115,   152,
     152,   152,   152,   152,   152,    14,   118,   166,   152,    14,
     152,   152,    14,    17,    58,   118,   166,   119,   103,   121,
     115,   101,   118,   152,   166,   115,   116,   101,   115,   101,
     115,   101,   115,    10,    11,    12,    14,    10,    14,   152,
      26,    29,    30,    36,    39,    40,    43,   115,   116,   152,
      26,    43,   115,   116,   152,   150,   152,   150,    75,   116,
     150,    73,    74,   117,   121,   139,   115,   115,   101,   101,
     121,   103,   121,   103,   121,   152,   150,   152,   150,   152,
     152,   152,   152,   152,   152,   152,   152,    53,   143,   121,
     117,    14,    62,   121,   115,   115,   116,   118,   122,    10,
      11,    12,    13,    24,    25,    33,    34,    47,    48,   122,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   155,   155,   155,
     155,   155,   155,   153,   155,   155,   155,   155,     7,     8,
       9,   157,   157,   157,   158,   158,   159,   160,   161,   161,
     162,   162,   162,   162,   162,    14,    14,    17,   115,   117,
     115,   115,   115,   115,   115,   115,   101,   117,   118,   120,
     104,   122,   115,   117,    14,   117,   118,   152,    14,   152,
     152,   115,   118,   152,   152,   117,   152,     5,    14,   117,
     148,   152,   101,   115,   101,   115,   101,   115,   117,   139,
     101,   101,   116,   152,   152,   152,   152,   152,   152,   152,
     166,   116,   152,   152,   166,   118,    27,    28,   128,   118,
      10,   166,   158,   158,    10,    14,   118,   149,   125,   152,
     152,   136,    14,   152,    14,   152,   118,    27,   118,   115,
     115,   115,   115,   122,   150,    14,    22,   121,   144,   118,
     149,   117,   117,   146,   118,   166,   114,   119,   121,   101,
     152,   152,   152,   122,   152,    26,    43,   122,    26,    43,
     122,    26,    29,    30,    36,    39,    40,    43,    44,   122,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,    85,   117,   117,    14,
     118,   166,   152,   118,   166,   101,   101,   101,   118,   166,
      14,   118,   147,   166,   120,   104,   122,   115,   115,   152,
      99,   115,   116,    72,    72,   148,   115,   116,   115,   152,
     117,   152,   152,   118,   149,   152,   152,   166,   115,   115,
     115,   115,   115,   115,   115,   115,   166,   115,   115,   115,
     150,   127,   150,   117,   152,   150,   150,    75,   150,    75,
      75,    10,    65,    66,   139,   116,   118,    24,    25,    34,
     122,   152,   115,   115,    37,    38,   122,   104,   122,   104,
     122,   150,   150,   150,    14,    14,   144,    56,    57,    60,
     122,   139,   118,   118,   149,   149,    69,    70,    71,   122,
     118,    14,   152,   152,   152,    26,    30,    36,    39,    40,
      43,   122,   122,   122,   122,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   153,   118,   166,
     118,   147,   166,   115,   118,   115,   118,   152,   152,   152,
     118,    72,   116,   118,   118,   116,   152,   166,   152,   152,
     118,     5,    14,   115,   152,   115,   116,   115,   139,   118,
     115,   115,   118,   118,    27,   128,   152,   150,    10,    10,
      10,    10,    10,    10,    14,   139,   152,   152,   122,   152,
      26,    29,    30,    36,    39,    40,    43,    44,   122,   152,
     150,   115,   115,    27,   122,    14,    58,    10,    13,    14,
      15,    16,    56,   139,   139,   118,   118,    14,    14,    14,
     101,   117,   120,   122,   122,   152,   152,   152,   152,   152,
     152,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   118,   118,   118,   115,   115,   115,   115,
     152,    14,   166,   115,   115,   115,    72,    72,   116,   166,
     139,   115,   115,   127,   150,   118,    27,   128,   150,   150,
     150,    10,    65,    66,    26,    30,    43,   122,   122,   122,
     152,   152,   152,   152,   152,   152,   152,   152,   150,   122,
     150,   117,   121,   140,   117,   140,    14,    22,    63,   101,
     115,    14,    22,    63,   101,   115,    10,    13,    15,    16,
     115,   115,    14,   139,   139,     7,     7,     7,   152,   118,
     166,   101,   101,   122,   122,   122,   122,   122,   122,    72,
     118,   152,   152,   166,   115,   150,   150,    10,    10,   152,
     152,   152,   122,   122,   122,   122,   122,   122,   122,   122,
     118,   149,    67,   125,   118,   149,    14,    14,   115,   152,
      14,    14,   115,   152,    14,    22,    63,   101,   115,    14,
      22,    63,   101,   115,   115,   115,   117,   140,   122,   122,
     118,   152,   152,   152,   115,   118,    27,   128,   122,   122,
     122,   140,   118,   122,    24,    25,    34,   122,   152,   140,
     118,   115,   115,   115,   115,   115,   115,    14,    14,   115,
     152,    14,    14,   115,   152,   118,   149,   122,   122,   122,
     115,   150,   140,   152,   152,   122,   152,    26,    29,    30,
      36,    39,    40,    43,    44,   122,   140,   115,   115,   115,
     115,   115,   115,   140,   118,    26,    30,    43,   122,   122,
     122,   152,   152,   152,   152,   152,   152,   152,   152,   140,
     152,   152,   152,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   123,   124,   125,   125,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   127,   127,   127,   127,   127,   127,   127,   127,
     128,   128,   128,   128,   128,   128,   129,   129,   130,   130,
     130,   130,   131,   131,   132,   133,   134,   134,   134,   134,
     134,   134,   135,   135,   136,   136,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   138,   138,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   141,   142,   143,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   146,   146,   146,   146,   147,   147,
     148,   148,   148,   148,   149,   149,   149,   149,   149,   149,
     149,   149,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   151,   152,   152,   152,
     153,   153,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   155,   155,   155,   156,
     156,   157,   157,   157,   158,   158,   159,   159,   160,   160,
     160,   161,   161,   161,   161,   161,   162,   162,   162,   162,
     163,   163,   164,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   166,
     166,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167
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
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     2,     2,     2,     1,
       3,     1,     1,     4,     3,     4,     3,     6,     6,     5,
       3,     3,     6,     5,     4,     4,     6,     6,     5,     3,
       6,     5,     3,     2,     3,     3,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     4,     5,     5,     1,     1,     1,
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
#line 315 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2575 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 325 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2581 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 326 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2587 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 330 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2593 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 332 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2599 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 334 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2605 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 336 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2611 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 338 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2617 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 340 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2625 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 344 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2632 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 347 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2640 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 351 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2646 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 353 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2652 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 355 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2658 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 357 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2664 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 359 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2670 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 361 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2678 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 365 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2686 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 369 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2692 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 371 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2698 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 373 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2704 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 375 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2710 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 377 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2716 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 379 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2722 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 381 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2728 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 383 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2734 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 385 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2740 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 387 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2746 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 389 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2752 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 391 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2758 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 393 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2764 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 395 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2770 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 397 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2777 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 400 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2784 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 403 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2790 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2797 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 408 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2804 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 411 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2810 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 413 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2816 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 415 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2822 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 417 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2828 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 419 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2837 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 424 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2845 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 428 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2853 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 432 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2862 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 437 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2870 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 440 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2876 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 442 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2882 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 444 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2888 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 446 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2894 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 448 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2900 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 450 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2907 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 453 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2913 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 455 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2919 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 457 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2925 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2932 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 462 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2939 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 465 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2946 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 468 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2953 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 471 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2960 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2966 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 476 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2972 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 478 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2978 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 480 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2984 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 482 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2991 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 485 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2997 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3003 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 489 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3009 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 491 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3016 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 494 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3023 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 497 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3031 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 501 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3038 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 504 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3044 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 506 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3050 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 508 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3056 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 510 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3063 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 513 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3070 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 516 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3077 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 519 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3084 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 522 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3091 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 525 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3098 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 528 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3105 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 531 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3112 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 534 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3119 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 537 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3126 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 540 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3133 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 542 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3139 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 543 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3145 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 544 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3151 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 545 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3157 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 546 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3163 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 547 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3169 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 549 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3175 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 551 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3181 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 553 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3187 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 555 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3193 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3199 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 557 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3205 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 558 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3211 "raku.tab.c"
    break;

  case 101: /* stmt: loop_stmt  */
#line 559 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3217 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST ';'  */
#line 560 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3223 "raku.tab.c"
    break;

  case 103: /* stmt: KW_NEXT ';'  */
#line 561 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3229 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST KW_IF expr ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3235 "raku.tab.c"
    break;

  case 105: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3241 "raku.tab.c"
    break;

  case 106: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3247 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3253 "raku.tab.c"
    break;

  case 108: /* stmt: sub_decl  */
#line 570 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3259 "raku.tab.c"
    break;

  case 109: /* stmt: class_decl  */
#line 571 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3265 "raku.tab.c"
    break;

  case 110: /* stmt: role_decl  */
#line 572 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3271 "raku.tab.c"
    break;

  case 111: /* stmt: grammar_decl  */
#line 573 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3277 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3283 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3289 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3295 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3301 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3307 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 587 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3313 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block elsif_tail  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3319 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 591 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3325 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block  */
#line 595 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3331 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 597 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3337 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3343 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 601 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3349 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 603 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3355 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3361 "raku.tab.c"
    break;

  case 126: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 609 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3367 "raku.tab.c"
    break;

  case 127: /* while_stmt: KW_WHILE expr block  */
#line 611 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3373 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3379 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3385 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS expr block  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3391 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3397 "raku.tab.c"
    break;

  case 132: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3403 "raku.tab.c"
    break;

  case 133: /* until_stmt: KW_UNTIL expr block  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3409 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3415 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 635 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3421 "raku.tab.c"
    break;

  case 136: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 639 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3431 "raku.tab.c"
    break;

  case 137: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 645 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3441 "raku.tab.c"
    break;

  case 138: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 651 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3450 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 656 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3460 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 662 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3469 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr block  */
#line 667 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3476 "raku.tab.c"
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
#line 3488 "raku.tab.c"
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
#line 3501 "raku.tab.c"
    break;

  case 144: /* when_list: %empty  */
#line 690 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3507 "raku.tab.c"
    break;

  case 145: /* when_list: when_list KW_WHEN expr block  */
#line 692 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3515 "raku.tab.c"
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
#line 3527 "raku.tab.c"
    break;

  case 147: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 706 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3537 "raku.tab.c"
    break;

  case 148: /* sub_decl: KW_SUB IDENT sub_body  */
#line 712 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3547 "raku.tab.c"
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
#line 3559 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 726 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3569 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 732 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3579 "raku.tab.c"
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
#line 3592 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 747 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3603 "raku.tab.c"
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
#line 3616 "raku.tab.c"
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
#line 3629 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 772 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3640 "raku.tab.c"
    break;

  case 157: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 781 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3651 "raku.tab.c"
    break;

  case 158: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 788 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3660 "raku.tab.c"
    break;

  case 159: /* sub_body: '{' stmt_list '}'  */
#line 794 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3666 "raku.tab.c"
    break;

  case 160: /* sub_body: '{' stmt_list expr '}'  */
#line 796 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3673 "raku.tab.c"
    break;

  case 161: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 799 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3680 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 802 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3686 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 804 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3692 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 806 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3698 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 808 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3704 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3710 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 812 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3716 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3722 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 816 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3729 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 819 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3735 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 821 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3741 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 823 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3747 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 825 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3754 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 828 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3761 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 831 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3769 "raku.tab.c"
    break;

  case 176: /* method_body: '{' stmt_list '}'  */
#line 836 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3775 "raku.tab.c"
    break;

  case 177: /* method_body: '{' YADA '}'  */
#line 837 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3781 "raku.tab.c"
    break;

  case 178: /* method_body: '{' stmt_list expr '}'  */
#line 839 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3788 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 842 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3795 "raku.tab.c"
    break;

  case 180: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 845 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3801 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 847 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3807 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 849 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3813 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 851 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3819 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3825 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 855 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3831 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 857 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3837 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 859 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3844 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 862 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3850 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 864 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3856 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 866 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3862 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 868 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3869 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 871 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3876 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 874 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3884 "raku.tab.c"
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
#line 3902 "raku.tab.c"
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
#line 3919 "raku.tab.c"
    break;

  case 196: /* is_clauses: %empty  */
#line 910 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3925 "raku.tab.c"
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
#line 3941 "raku.tab.c"
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
#line 3957 "raku.tab.c"
    break;

  case 199: /* class_body_list: %empty  */
#line 937 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3963 "raku.tab.c"
    break;

  case 200: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 939 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3970 "raku.tab.c"
    break;

  case 201: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 942 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3977 "raku.tab.c"
    break;

  case 202: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 945 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3984 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 948 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3991 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 951 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3998 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 954 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4005 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 957 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4012 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 960 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4019 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 963 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4026 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 966 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4033 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 969 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4040 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 972 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4047 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 975 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4058 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 982 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4069 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 989 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4080 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 996 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4091 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1003 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4099 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1007 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4107 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1011 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4115 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1015 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4123 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1019 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4134 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1026 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4145 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1033 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4156 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1040 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4167 "raku.tab.c"
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
#line 4181 "raku.tab.c"
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
#line 4193 "raku.tab.c"
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
#line 4205 "raku.tab.c"
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
#line 4218 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1082 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4229 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1089 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4240 "raku.tab.c"
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
#line 4254 "raku.tab.c"
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
#line 4266 "raku.tab.c"
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
#line 4278 "raku.tab.c"
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
#line 4295 "raku.tab.c"
    break;

  case 234: /* grammar_body_list: %empty  */
#line 1138 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4301 "raku.tab.c"
    break;

  case 235: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1140 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4310 "raku.tab.c"
    break;

  case 236: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1145 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4319 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1150 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4328 "raku.tab.c"
    break;

  case 238: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1157 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4336 "raku.tab.c"
    break;

  case 239: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1161 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4344 "raku.tab.c"
    break;

  case 240: /* pair_list: IDENT OP_FATARROW expr  */
#line 1167 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4350 "raku.tab.c"
    break;

  case 241: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1169 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4356 "raku.tab.c"
    break;

  case 242: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1171 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4362 "raku.tab.c"
    break;

  case 243: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1173 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4368 "raku.tab.c"
    break;

  case 244: /* param_list: VAR_SCALAR  */
#line 1176 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4374 "raku.tab.c"
    break;

  case 245: /* param_list: IDENT VAR_SCALAR  */
#line 1177 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4380 "raku.tab.c"
    break;

  case 246: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1178 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4386 "raku.tab.c"
    break;

  case 247: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1179 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4392 "raku.tab.c"
    break;

  case 248: /* param_list: param_list ',' VAR_SCALAR  */
#line 1180 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4398 "raku.tab.c"
    break;

  case 249: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1181 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4404 "raku.tab.c"
    break;

  case 250: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1182 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4410 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1183 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4416 "raku.tab.c"
    break;

  case 252: /* block: '{' stmt_list '}'  */
#line 1186 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4422 "raku.tab.c"
    break;

  case 253: /* block: '{' YADA '}'  */
#line 1187 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4428 "raku.tab.c"
    break;

  case 254: /* block: '{' stmt_list expr '}'  */
#line 1189 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4434 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1191 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4440 "raku.tab.c"
    break;

  case 256: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1193 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4446 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1195 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4452 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1197 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4458 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1199 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4465 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1202 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4471 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1204 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4477 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1206 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4483 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1208 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4489 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1210 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4496 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1213 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4503 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1216 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4511 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1220 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4518 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1223 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4525 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1226 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4532 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1229 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4538 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1231 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4546 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1235 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4553 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1238 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4560 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1241 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4567 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1244 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4574 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1247 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4581 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1250 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4587 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1252 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4593 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1254 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4599 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_LAST '}'  */
#line 1256 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4605 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1258 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4611 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1260 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4617 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1262 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4623 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1264 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4629 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1266 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4635 "raku.tab.c"
    break;

  case 286: /* closure: '{' expr '}'  */
#line 1269 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4641 "raku.tab.c"
    break;

  case 287: /* expr: VAR_SCALAR '=' expr  */
#line 1272 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4647 "raku.tab.c"
    break;

  case 288: /* expr: KW_GATHER block  */
#line 1273 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4657 "raku.tab.c"
    break;

  case 289: /* expr: tern_expr  */
#line 1278 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4663 "raku.tab.c"
    break;

  case 290: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1282 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4669 "raku.tab.c"
    break;

  case 291: /* tern_expr: cmp_expr  */
#line 1283 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4675 "raku.tab.c"
    break;

  case 292: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1286 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4681 "raku.tab.c"
    break;

  case 293: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1287 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4687 "raku.tab.c"
    break;

  case 294: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1288 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4693 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1289 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4699 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1290 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4705 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1291 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4711 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1292 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4717 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1293 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4723 "raku.tab.c"
    break;

  case 300: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1294 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4729 "raku.tab.c"
    break;

  case 301: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1295 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4735 "raku.tab.c"
    break;

  case 302: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1297 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4745 "raku.tab.c"
    break;

  case 303: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1303 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4755 "raku.tab.c"
    break;

  case 304: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1309 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4765 "raku.tab.c"
    break;

  case 305: /* cmp_expr: jct_expr  */
#line 1314 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4771 "raku.tab.c"
    break;

  case 306: /* jct_expr: jct_expr '|' range_expr  */
#line 1317 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4777 "raku.tab.c"
    break;

  case 307: /* jct_expr: jct_expr '&' range_expr  */
#line 1318 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4783 "raku.tab.c"
    break;

  case 308: /* jct_expr: dor_expr  */
#line 1319 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4789 "raku.tab.c"
    break;

  case 309: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1323 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4795 "raku.tab.c"
    break;

  case 310: /* dor_expr: range_expr  */
#line 1324 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4801 "raku.tab.c"
    break;

  case 311: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1327 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4807 "raku.tab.c"
    break;

  case 312: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1328 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4813 "raku.tab.c"
    break;

  case 313: /* range_expr: add_expr  */
#line 1329 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4819 "raku.tab.c"
    break;

  case 314: /* add_expr: add_expr '~' repl_expr  */
#line 1332 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4825 "raku.tab.c"
    break;

  case 315: /* add_expr: repl_expr  */
#line 1333 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4831 "raku.tab.c"
    break;

  case 316: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1336 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4837 "raku.tab.c"
    break;

  case 317: /* repl_expr: addsub_expr  */
#line 1337 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4843 "raku.tab.c"
    break;

  case 318: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1340 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4849 "raku.tab.c"
    break;

  case 319: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1341 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4855 "raku.tab.c"
    break;

  case 320: /* addsub_expr: mul_expr  */
#line 1342 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4861 "raku.tab.c"
    break;

  case 321: /* mul_expr: mul_expr '*' unary_expr  */
#line 1345 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4867 "raku.tab.c"
    break;

  case 322: /* mul_expr: mul_expr '/' unary_expr  */
#line 1346 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4873 "raku.tab.c"
    break;

  case 323: /* mul_expr: mul_expr '%' unary_expr  */
#line 1347 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4879 "raku.tab.c"
    break;

  case 324: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1348 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4885 "raku.tab.c"
    break;

  case 325: /* mul_expr: unary_expr  */
#line 1349 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4891 "raku.tab.c"
    break;

  case 326: /* unary_expr: '-' unary_expr  */
#line 1352 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4897 "raku.tab.c"
    break;

  case 327: /* unary_expr: '!' unary_expr  */
#line 1353 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4903 "raku.tab.c"
    break;

  case 328: /* unary_expr: CARET unary_expr  */
#line 1354 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 4909 "raku.tab.c"
    break;

  case 329: /* unary_expr: pow_expr  */
#line 1355 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4915 "raku.tab.c"
    break;

  case 330: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1358 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4921 "raku.tab.c"
    break;

  case 331: /* pow_expr: postfix_expr  */
#line 1359 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4927 "raku.tab.c"
    break;

  case 332: /* postfix_expr: call_expr  */
#line 1361 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4933 "raku.tab.c"
    break;

  case 333: /* call_expr: IDENT '(' arg_list ')'  */
#line 1364 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4942 "raku.tab.c"
    break;

  case 334: /* call_expr: IDENT '(' ')'  */
#line 1368 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4948 "raku.tab.c"
    break;

  case 335: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1370 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4957 "raku.tab.c"
    break;

  case 336: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1375 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4963 "raku.tab.c"
    break;

  case 337: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1377 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4973 "raku.tab.c"
    break;

  case 338: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1383 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4983 "raku.tab.c"
    break;

  case 339: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1389 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4991 "raku.tab.c"
    break;

  case 340: /* call_expr: IDENT '.' KW_NEW  */
#line 1393 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4999 "raku.tab.c"
    break;

  case 341: /* call_expr: IDENT '.' IDENT  */
#line 1397 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5008 "raku.tab.c"
    break;

  case 342: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1402 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5019 "raku.tab.c"
    break;

  case 343: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1409 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5028 "raku.tab.c"
    break;

  case 344: /* call_expr: IDENT '.' CARET IDENT  */
#line 1414 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5038 "raku.tab.c"
    break;

  case 345: /* call_expr: atom '.' CARET IDENT  */
#line 1420 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5048 "raku.tab.c"
    break;

  case 346: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1426 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5059 "raku.tab.c"
    break;

  case 347: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1433 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5070 "raku.tab.c"
    break;

  case 348: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1440 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5079 "raku.tab.c"
    break;

  case 349: /* call_expr: atom '.' IDENT  */
#line 1445 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5088 "raku.tab.c"
    break;

  case 350: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1450 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5099 "raku.tab.c"
    break;

  case 351: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1457 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5108 "raku.tab.c"
    break;

  case 352: /* call_expr: call_expr '.' IDENT  */
#line 1462 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5117 "raku.tab.c"
    break;

  case 353: /* call_expr: KW_DIE expr  */
#line 1467 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5123 "raku.tab.c"
    break;

  case 354: /* call_expr: KW_MAP closure expr  */
#line 1469 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5129 "raku.tab.c"
    break;

  case 355: /* call_expr: KW_GREP closure expr  */
#line 1471 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5135 "raku.tab.c"
    break;

  case 356: /* call_expr: KW_SORT expr  */
#line 1473 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5141 "raku.tab.c"
    break;

  case 357: /* call_expr: KW_SORT closure expr  */
#line 1475 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5147 "raku.tab.c"
    break;

  case 358: /* call_expr: atom  */
#line 1476 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5153 "raku.tab.c"
    break;

  case 359: /* arg_list: expr  */
#line 1479 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5159 "raku.tab.c"
    break;

  case 360: /* arg_list: arg_list ',' expr  */
#line 1480 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5165 "raku.tab.c"
    break;

  case 361: /* atom: LIT_INT  */
#line 1483 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5171 "raku.tab.c"
    break;

  case 362: /* atom: LIT_FLOAT  */
#line 1484 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5177 "raku.tab.c"
    break;

  case 363: /* atom: LIT_STR  */
#line 1485 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5183 "raku.tab.c"
    break;

  case 364: /* atom: WORDLIST  */
#line 1487 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5195 "raku.tab.c"
    break;

  case 365: /* atom: LIT_INTERP_STR  */
#line 1494 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5201 "raku.tab.c"
    break;

  case 366: /* atom: VAR_SCALAR  */
#line 1495 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5207 "raku.tab.c"
    break;

  case 367: /* atom: VAR_ARRAY  */
#line 1496 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5213 "raku.tab.c"
    break;

  case 368: /* atom: VAR_HASH  */
#line 1497 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5219 "raku.tab.c"
    break;

  case 369: /* atom: VAR_CAPTURE  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5227 "raku.tab.c"
    break;

  case 370: /* atom: VAR_FH  */
#line 1503 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5235 "raku.tab.c"
    break;

  case 371: /* atom: VAR_NAMED_CAPTURE  */
#line 1507 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5242 "raku.tab.c"
    break;

  case 372: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1510 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5248 "raku.tab.c"
    break;

  case 373: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1512 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5254 "raku.tab.c"
    break;

  case 374: /* atom: VAR_HASH '{' expr '}'  */
#line 1514 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5260 "raku.tab.c"
    break;

  case 375: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1516 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5266 "raku.tab.c"
    break;

  case 376: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1518 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5272 "raku.tab.c"
    break;

  case 377: /* atom: IDENT  */
#line 1519 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5278 "raku.tab.c"
    break;

  case 378: /* atom: VAR_TWIGIL  */
#line 1521 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5286 "raku.tab.c"
    break;

  case 379: /* atom: VAR_ARRAY_TWIGIL  */
#line 1525 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5294 "raku.tab.c"
    break;

  case 380: /* atom: VAR_HASH_TWIGIL  */
#line 1529 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5302 "raku.tab.c"
    break;

  case 381: /* atom: '(' ')'  */
#line 1532 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5308 "raku.tab.c"
    break;

  case 382: /* atom: '(' expr ')'  */
#line 1533 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5314 "raku.tab.c"
    break;

  case 383: /* atom: '(' expr ',' ')'  */
#line 1535 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5320 "raku.tab.c"
    break;

  case 384: /* atom: '(' expr ',' arg_list ')'  */
#line 1537 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5327 "raku.tab.c"
    break;

  case 385: /* atom: block  */
#line 1539 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5333 "raku.tab.c"
    break;

  case 386: /* atom: KW_SUB block  */
#line 1540 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5339 "raku.tab.c"
    break;


#line 5343 "raku.tab.c"

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

#line 1542 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
