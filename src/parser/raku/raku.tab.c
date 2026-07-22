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
  YYSYMBOL_KW_MAP = 46,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 47,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 48,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 49,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 50,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 51,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 52,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 53,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 54,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 55,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 56,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 57,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 58,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 59,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 60,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 61,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 62,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 63,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 64,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 65,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 66,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 67,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 68,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 69,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 70,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 71,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 72,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 73,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 74,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 75,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 76,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 77,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 78,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 79,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 80,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 81,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 82,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 83,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 84,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 85,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 86,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 87,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 88,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 89,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 90,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 91,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 92,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 93,                    /* OP_DOR  */
  YYSYMBOL_OP_DIV = 94,                    /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 95,                  /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 96,                 /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 97,                    /* OP_POW  */
  YYSYMBOL_98_ = 98,                       /* '='  */
  YYSYMBOL_99_ = 99,                       /* '!'  */
  YYSYMBOL_100_ = 100,                     /* '<'  */
  YYSYMBOL_101_ = 101,                     /* '>'  */
  YYSYMBOL_102_ = 102,                     /* '|'  */
  YYSYMBOL_103_ = 103,                     /* '&'  */
  YYSYMBOL_104_ = 104,                     /* '~'  */
  YYSYMBOL_105_ = 105,                     /* '+'  */
  YYSYMBOL_106_ = 106,                     /* '-'  */
  YYSYMBOL_107_ = 107,                     /* '*'  */
  YYSYMBOL_108_ = 108,                     /* '/'  */
  YYSYMBOL_109_ = 109,                     /* '%'  */
  YYSYMBOL_UMINUS = 110,                   /* UMINUS  */
  YYSYMBOL_111_ = 111,                     /* '.'  */
  YYSYMBOL_112_ = 112,                     /* ';'  */
  YYSYMBOL_113_ = 113,                     /* ','  */
  YYSYMBOL_114_ = 114,                     /* '('  */
  YYSYMBOL_115_ = 115,                     /* ')'  */
  YYSYMBOL_116_ = 116,                     /* '['  */
  YYSYMBOL_117_ = 117,                     /* ']'  */
  YYSYMBOL_118_ = 118,                     /* '{'  */
  YYSYMBOL_119_ = 119,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 120,                 /* $accept  */
  YYSYMBOL_program = 121,                  /* program  */
  YYSYMBOL_stmt_list = 122,                /* stmt_list  */
  YYSYMBOL_stmt = 123,                     /* stmt  */
  YYSYMBOL_if_stmt = 124,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 125,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 126,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 127,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 128,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 129,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 130,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 131,               /* given_stmt  */
  YYSYMBOL_when_list = 132,                /* when_list  */
  YYSYMBOL_sub_decl = 133,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 134,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 135,                 /* sub_body  */
  YYSYMBOL_method_body = 136,              /* method_body  */
  YYSYMBOL_class_decl = 137,               /* class_decl  */
  YYSYMBOL_role_decl = 138,                /* role_decl  */
  YYSYMBOL_is_clauses = 139,               /* is_clauses  */
  YYSYMBOL_class_body_list = 140,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 141,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 142,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 143,           /* named_arg_list  */
  YYSYMBOL_pair_list = 144,                /* pair_list  */
  YYSYMBOL_param_list = 145,               /* param_list  */
  YYSYMBOL_block = 146,                    /* block  */
  YYSYMBOL_closure = 147,                  /* closure  */
  YYSYMBOL_expr = 148,                     /* expr  */
  YYSYMBOL_tern_expr = 149,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 150,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 151,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 152,                 /* dor_expr  */
  YYSYMBOL_range_expr = 153,               /* range_expr  */
  YYSYMBOL_add_expr = 154,                 /* add_expr  */
  YYSYMBOL_repl_expr = 155,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 156,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 157,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 158,               /* unary_expr  */
  YYSYMBOL_pow_expr = 159,                 /* pow_expr  */
  YYSYMBOL_postfix_expr = 160,             /* postfix_expr  */
  YYSYMBOL_call_expr = 161,                /* call_expr  */
  YYSYMBOL_arg_list = 162,                 /* arg_list  */
  YYSYMBOL_atom = 163                      /* atom  */
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
#define YYLAST   3241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  120
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  350
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  893

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   353


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
       2,     2,     2,    99,     2,     2,     2,   109,   103,     2,
     114,   115,   107,   105,   113,   106,   111,   108,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   112,
     100,    98,   101,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   116,     2,   117,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   118,   102,   119,   104,     2,     2,     2,
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
      95,    96,    97,   110
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   313,   313,   324,   325,   328,   330,   332,   334,   336,
     338,   342,   345,   349,   351,   353,   355,   357,   359,   363,
     367,   369,   371,   373,   375,   377,   379,   381,   383,   385,
     387,   389,   391,   393,   395,   398,   401,   403,   406,   409,
     411,   413,   415,   417,   422,   426,   430,   435,   439,   440,
     442,   444,   446,   448,   451,   453,   455,   457,   460,   463,
     466,   469,   472,   474,   476,   478,   480,   483,   485,   487,
     489,   492,   495,   499,   502,   504,   506,   508,   511,   514,
     517,   520,   523,   526,   529,   532,   535,   538,   541,   542,
     543,   544,   545,   546,   547,   549,   551,   553,   555,   556,
     557,   558,   559,   560,   561,   564,   566,   568,   570,   572,
     574,   576,   578,   582,   584,   586,   588,   590,   592,   596,
     598,   602,   604,   606,   608,   612,   614,   618,   622,   628,
     634,   639,   645,   650,   655,   663,   674,   675,   681,   689,
     695,   701,   709,   715,   721,   730,   737,   746,   755,   764,
     771,   778,   779,   782,   785,   787,   789,   793,   794,   795,
     798,   801,   803,   805,   809,   825,   840,   841,   853,   867,
     868,   871,   874,   877,   880,   883,   886,   889,   892,   895,
     898,   901,   904,   911,   918,   925,   932,   936,   940,   944,
     948,   955,   962,   969,   976,   986,   994,  1002,  1011,  1018,
    1025,  1035,  1043,  1053,  1068,  1069,  1074,  1079,  1086,  1090,
    1096,  1098,  1100,  1102,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,  1113,  1116,  1117,  1118,  1120,  1122,  1124,  1126,  1128,
    1131,  1133,  1135,  1137,  1139,  1142,  1145,  1149,  1152,  1155,
    1158,  1160,  1164,  1167,  1170,  1173,  1176,  1179,  1181,  1183,
    1187,  1190,  1191,  1196,  1199,  1201,  1204,  1205,  1206,  1207,
    1208,  1209,  1210,  1211,  1212,  1213,  1214,  1220,  1226,  1232,
    1235,  1236,  1237,  1240,  1242,  1245,  1246,  1247,  1250,  1251,
    1254,  1255,  1258,  1259,  1260,  1263,  1264,  1265,  1266,  1267,
    1270,  1271,  1272,  1273,  1276,  1277,  1279,  1281,  1286,  1287,
    1292,  1294,  1300,  1306,  1310,  1314,  1319,  1326,  1331,  1337,
    1343,  1350,  1357,  1362,  1367,  1374,  1379,  1384,  1386,  1388,
    1390,  1392,  1394,  1397,  1398,  1401,  1402,  1403,  1404,  1412,
    1413,  1414,  1415,  1416,  1420,  1424,  1427,  1429,  1431,  1433,
    1435,  1437,  1438,  1442,  1446,  1450,  1451,  1452,  1454,  1457,
    1458
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
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_MAP", "KW_GREP",
  "KW_SORT", "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD",
  "KW_HAS", "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME",
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt",
  "given_stmt", "when_list", "sub_decl", "scalar_methcall", "sub_body",
  "method_body", "class_decl", "role_decl", "is_clauses",
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

#define YYPACT_NINF (-550)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-350)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -550,    56,  1342,  -550,  -550,  -550,  -550,  -550,   619,   -54,
     -51,    67,    -9,  -550,  -550,  3123,  -550,  -550,  -550,    63,
    1420,   383,  2391,  2452,  2513,  2574,  2635,     0,   -18,  2635,
    1474,   208,  2635,   148,   173,  2696,  2757,   -18,    89,    89,
    2818,   -18,   -18,  2635,   235,   265,    62,  -550,   301,   272,
     310,  3123,  3123,  -550,  1536,   273,  -550,  -550,  -550,  -550,
    -550,  -550,  -550,  -550,  -550,   509,  -550,  -550,  -550,  1214,
     552,  -550,   259,   306,   249,  -550,     2,   258,   220,   237,
    -550,  -550,   281,   262,   290,   356,   292,   338,  2635,  2635,
    2635,  2635,  2635,  2635,   473,  1597,  2635,   489,  2635,  2635,
      54,  1658,   307,   410,    38,  -550,   -18,  -550,  -550,   401,
      18,  -550,  1719,  -550,   387,   121,   158,   166,   448,   520,
     266,  1536,   211,  1536,    49,  1536,   -18,  1536,   -18,   117,
     128,   182,  -550,  -550,   411,  -550,   429,   446,   453,   443,
      97,   127,  1536,   -18,  1536,   -18,  -550,  2635,  2635,  2635,
    1780,  2635,  -550,   522,  -550,  -550,  -550,   506,   479,    25,
     518,   493,   553,  -550,  -550,  -550,   190,   557,   637,  2635,
    2635,  2635,  2635,  2635,  2635,  2635,  2635,  -550,  2635,  2635,
    2635,  2635,  2635,  2635,  2635,  2635,  -550,  3123,  3123,  3123,
    3123,  3123,  3123,  3123,  3123,  3123,  3123,  3123,   462,  3123,
    3123,  3123,  3123,  3123,  3123,  3123,  3123,  3123,  3123,  3123,
    3123,  3123,  3123,   677,   199,   276,  -550,  -550,   584,   592,
     617,   627,   638,   641,    61,  -550,   397,   640,   660,   615,
     653,   669,   770,   681,  -550,   407,  2635,   782,  2635,  -550,
    2635,   696,   190,   414,  -550,  2635,  2635,  -550,  2879,  -550,
    2940,  -550,   171,   183,   189,   236,   712,   713,   417,  2635,
    2635,  2635,  2635,  2635,  2635,  2635,  -550,  2635,   427,  2635,
    2635,  -550,  2635,   514,   408,   515,  -550,   802,  2635,  -550,
    3123,  3123,    -4,  -550,  -550,  -550,  -550,  2635,  2635,  -550,
     799,  2635,   800,  2635,   524,   788,   577,  -550,   697,  -550,
    -550,  -550,   -18,     4,  -550,    -1,   703,   704,  -550,  -550,
    -550,  1842,  -550,  -550,   683,   705,   155,   721,  2391,  2452,
    2635,  1102,  -550,   332,   710,   711,   714,   715,   716,   717,
     718,   719,   720,   722,   723,   724,   725,   726,   727,   728,
     393,   393,   393,   393,   393,   393,   742,   393,   393,   393,
     393,  -550,  -550,  -550,  -550,  -550,  -550,   729,   729,   258,
     220,   237,   237,  -550,  -550,  -550,  -550,  -550,   734,   735,
     827,  -550,  1903,  -550,  -550,  -550,  -550,  -550,  -550,  2635,
    1964,  -550,   744,   745,   752,  -550,  2025,  -550,  2086,  -550,
     737,   754,   738,  -550,  -550,   746,  -550,   747,  1536,   -43,
     787,   -29,  2147,   404,   748,  2635,  -550,  3001,  -550,  2635,
    -550,     5,  -550,  2635,  2635,  1842,   749,   750,   751,   753,
     755,   757,   762,   467,  1842,   763,   764,   477,   -18,   -14,
    3062,  -550,   -18,   -18,   159,    11,    23,  -550,    71,   776,
     582,   867,   783,   792,   -21,   765,   786,   805,   801,   -18,
     -18,   -18,  -550,  -550,   893,   907,  -550,    -6,   776,   599,
      15,   341,    12,  -550,   600,   908,  2635,  2635,  2635,   268,
      16,   105,  -550,   224,  2635,  2635,  2635,  2635,  2635,  2635,
    2635,  2635,  -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,
    -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,  3123,
    2208,  2269,  -550,   813,   603,   817,  -550,   606,  2635,  2635,
    2635,  -550,   607,   160,  -550,   612,   613,  -550,  -550,  -550,
    -550,  -550,   622,  2635,  -550,  2635,  2635,  2635,   625,  -550,
     149,  -550,   818,  1536,   487,   819,   776,   629,   821,   822,
     632,  -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,   633,
    -550,  -550,  -550,   587,  -550,  -550,  1536,   -18,  -550,  -550,
     925,  -550,   926,   927,  -550,   928,   929,  -550,   355,   776,
    2391,  2452,  1156,  -550,   390,  -550,  -550,  2635,   -18,  -550,
    -550,  -550,   828,   829,   915,  -550,  -550,  -550,  -550,     7,
      44,   588,   890,  -550,  -550,   776,   776,   639,   645,   930,
     931,   932,  -550,  -550,    94,   830,   831,   244,  2635,  2635,
    2635,  2635,  2635,  2635,  -550,  -550,  -550,  -550,   255,   267,
     270,   280,   294,   303,   305,   313,  -550,  -550,   651,  -550,
     663,   664,  -550,   836,  -550,  -550,   837,   839,   840,  -550,
    2635,   941,  -550,  -550,  1842,   844,   510,  -550,  -550,   845,
     889,   891,  -550,   667,  -550,  2635,  -550,  -550,   776,  -550,
    -550,   847,   849,   -14,  -550,   674,   604,   -18,   -18,   -18,
    -550,  -550,  -550,   133,  -550,   454,    81,  -550,   319,  -550,
     -18,   843,  -550,  -550,   -18,  -550,   284,   309,    41,    43,
     561,   851,   852,   951,  -550,  -550,   776,   776,   960,   961,
     962,  2635,  2330,   872,   873,  -550,   328,   333,   334,   343,
     344,   349,  -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,
    -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,   903,   675,
    -550,  -550,  -550,  2635,  2635,  1842,   533,  -550,  -550,  -550,
    -550,  -550,   -18,   -18,  -550,  -550,  -550,  -550,  -550,   964,
     965,  -550,  -550,  -550,  -550,  -550,  -550,    21,   912,  -550,
      22,  -550,   963,   966,   866,  2635,  -550,   968,   969,   875,
    2635,  -550,    52,   130,   881,   882,  -550,  -550,   323,  -550,
    -550,  -550,  -550,  -550,   369,   865,   685,  2635,  2635,  -550,
    -550,  -550,  -550,  -550,  -550,  2635,   900,  -550,  -550,   686,
    -550,   647,  -550,  -550,  -550,   895,   689,   904,   985,   895,
     690,   910,   933,  -550,   935,   936,   937,  -550,   939,  1010,
    1024,   940,  2635,  -550,  1025,  1026,   942,  2635,  -550,  -550,
    -550,    36,  -550,  -550,  -550,   924,   370,   373,  -550,  -550,
     943,   -18,  -550,  -550,   895,  -550,  2391,  2452,  1225,  -550,
     475,  -550,   895,  -550,  -550,  -550,  -550,  -550,  -550,   944,
     945,  -550,   946,   947,   948,  -550,   949,   895,   694,  -550,
    -550,  -550,  -550,  -550,  -550,   507,   180,  -550,   379,  -550,
    -550,  -550,  -550,  -550,  -550,  -550,  -550,  -550,   895,  -550,
    -550,  -550,  -550
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   325,   326,   327,   329,   330,   331,
     332,   342,   341,   343,   344,     0,   333,   334,   335,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   328,     0,     0,
       0,     0,     0,    89,     0,     3,     4,    90,    91,    98,
      99,   100,    92,    93,   101,     0,   102,   103,   104,    97,
       0,   253,   255,   269,   272,   274,   277,   279,   281,   284,
     289,   293,   295,   296,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   331,   332,   342,     0,   349,   292,     0,
     330,    29,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,   350,   252,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       3,     0,   320,    94,    96,   317,   166,     0,     0,     0,
       0,     0,     0,   291,   290,   345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,    85,     0,     0,
       0,     0,     0,   251,     0,   300,     0,     0,     0,     0,
       0,   305,     0,   304,   298,     0,     0,     0,     0,    24,
       0,   345,     0,     0,    32,     0,     0,     6,     0,     7,
       0,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,     0,     0,     0,
       0,    36,     0,     0,   108,     0,   120,     0,     0,   133,
       0,     0,     0,     3,   140,    39,    40,     0,     0,   136,
       0,     0,     0,     0,     0,   123,     0,   126,     0,   318,
     319,   321,     0,     0,   169,     0,     0,     0,   204,    86,
      87,     0,   346,   223,   330,   331,   332,   342,     0,     0,
       0,     0,   222,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     258,   259,   262,   263,   256,   257,     0,   260,   261,   264,
     265,   266,   267,   268,   270,   271,   273,   275,   276,   278,
     280,   282,   283,   288,   285,   286,   287,   294,   316,   313,
       0,    45,     0,    79,    80,    81,    82,    83,    42,     0,
       0,   299,   336,   337,   338,    47,     0,   308,     0,   297,
       0,     0,     0,   251,    31,     0,   324,     0,     0,     0,
     327,   341,     0,     0,     0,     0,    21,     0,    22,     0,
      23,     0,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   346,     0,
       0,   111,   346,     0,     0,   275,   276,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   346,
       0,   346,   250,    95,     0,     0,   169,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,    49,    51,    53,    56,    54,    55,    50,
      52,    62,    64,    66,    69,    67,    68,    63,    65,     0,
       0,     0,   309,     0,     0,     0,   150,     0,     0,     0,
       0,   307,     0,   341,   303,     0,     0,   336,   337,   338,
      30,     5,     0,     0,     9,     0,     0,     0,     0,    14,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    73,    72,    76,    74,    75,    71,    34,     0,
      77,    78,    37,   105,   110,   109,     0,     0,   119,   130,
       0,   132,     0,     0,   215,     0,     0,   139,     0,     0,
       0,     0,     0,   151,     0,    26,    25,     0,     0,   134,
     339,   340,     0,     0,   121,   124,   125,   167,   168,     0,
       0,     0,     0,   165,   148,     0,     0,     0,     0,     0,
       0,     0,   203,   348,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   240,   247,   248,     0,     0,
       0,     0,     0,     0,     0,     0,   254,   315,     0,   312,
       0,     0,    44,     0,    46,   149,     0,     0,     0,   306,
       0,     0,   301,   302,     0,     0,     0,   211,   210,     0,
       0,     0,    16,     0,    17,     0,    20,   142,     0,    28,
      27,   348,   348,     0,   112,     0,   113,     0,     0,     0,
     216,   217,   218,     0,   138,     0,     0,   154,     0,   152,
       0,     0,    60,    61,     0,   164,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   145,     0,     0,     0,     0,
       0,     0,     0,   336,   338,   244,     0,     0,     0,     0,
       0,     0,   225,   227,   229,   232,   230,   231,   226,   228,
     314,   311,   310,    43,    57,    58,    59,   208,     0,     0,
      11,    10,    15,     0,     0,     0,     0,   141,    35,    38,
     107,   106,   346,     0,   117,   131,   128,   129,   219,     0,
       0,   155,   156,   153,   137,   135,   122,     0,     3,   196,
       0,   199,     0,     0,     0,     0,   175,     0,     0,     0,
       0,   170,     0,     0,     0,     0,   171,   172,     0,   144,
     146,   205,   206,   207,     0,   150,     0,     0,     0,   234,
     236,   239,   237,   238,   235,     0,   348,   213,   212,     0,
      18,   114,   115,   220,   221,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,     0,     0,   187,     0,     0,
       0,     0,     0,   177,     0,     0,     0,     0,   176,   173,
     174,     0,   202,   243,   242,   149,     0,     0,   209,    12,
     348,     0,   118,   195,     0,   158,     0,     0,     0,   157,
       0,   198,     0,   184,   185,   179,   182,   183,   178,     0,
       0,   188,     0,     0,     0,   189,     0,     0,     0,   241,
     245,   246,    19,   116,   194,     0,     0,   161,     0,   159,
     197,   192,   193,   181,   190,   191,   180,   201,     0,   162,
     163,   160,   200
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -550,  -550,     1,  -550,  -424,  -549,  -550,  -550,  -550,  -550,
    -550,  -550,  -550,  -550,  -550,  -248,  -503,  -550,  -550,  -550,
     597,  -550,  -550,   543,   662,  -303,   184,   661,    -2,  -190,
    -550,   368,  -550,   384,   -15,   858,   860,   585,    -7,  -550,
    -550,  -550,   -58,  -550
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   168,    56,    57,   431,    58,    59,    60,    61,
      62,    63,   444,    64,    65,   284,   759,    66,    67,   303,
     457,    68,   462,   515,   403,   440,   107,   148,   113,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,   114,    84
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      70,     2,   459,   346,   664,   554,   437,   412,   108,   437,
     438,   130,    24,   438,   131,   437,   577,   578,   454,   438,
     122,   124,   126,   128,   129,   437,   455,   134,   136,   438,
     139,   437,   437,   143,   145,   438,   438,   226,   152,   306,
     527,   155,   269,   235,   163,   164,   437,   590,   591,    97,
     438,   592,   166,   523,   243,   762,     3,   767,   686,   270,
     590,   591,    96,   763,   592,   768,   819,    98,   231,   524,
     525,   232,   202,   203,   820,   269,   158,   109,   599,   600,
     601,   564,   100,   562,   307,   101,   218,   219,   220,   221,
     222,   223,   270,   159,   227,   563,   229,   230,   579,   687,
      55,   764,   100,   769,    55,   101,   204,   269,   537,   233,
     242,   439,   821,   593,   458,   204,   240,   744,    55,   258,
     536,   268,   456,   273,   270,   275,   685,   204,   271,   272,
     596,   602,    95,   565,   566,   615,   805,   809,   237,   765,
     294,   770,   296,   748,   824,   298,   299,   300,   298,   301,
     822,   867,   825,   766,   650,   771,   238,   597,   598,   379,
     140,   271,   272,   651,   823,    99,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   380,   332,   333,   334,   335,
     336,   337,   338,   339,   761,   141,    69,   357,   358,   277,
     826,   567,   701,   271,   272,   749,   750,   290,   280,   281,
     752,   363,   364,   365,   366,   367,   269,   147,   702,   423,
     594,   132,   133,   369,   427,   291,   370,   285,   137,   246,
     434,   146,   138,   270,   616,   153,   154,   292,   827,   640,
     278,   560,   204,   247,   390,    55,   392,   259,   393,   740,
     260,   261,   828,   396,   397,   293,   399,   262,   404,   156,
     263,   264,   842,   464,   265,    97,   248,   416,   417,   418,
     419,   420,   421,   422,   250,   435,   436,   425,   426,   405,
     249,   100,   245,   467,   101,   832,   256,    55,   251,   157,
     257,   407,   161,   406,   441,   442,   443,   409,   657,   446,
     132,   448,   271,   272,   608,   408,   282,   260,   609,   890,
     283,   410,   843,   311,   610,   312,   851,   611,   612,   626,
     274,   613,   276,   279,   504,   160,   469,   470,   471,   473,
     162,   674,   507,   266,   267,   206,   207,   295,   512,   297,
     516,   208,   187,   188,   189,   190,   286,   167,   191,   192,
     193,   874,   201,   617,   209,   210,   211,   694,   695,   880,
     411,   437,    69,   205,   283,   438,   385,   540,   474,   194,
     195,   475,   476,   705,   887,   672,   549,   491,   477,   673,
     215,   478,   479,   213,   712,   480,   481,   505,   212,   492,
     266,   267,   493,   196,   197,   892,   713,   614,   371,   714,
     372,   198,   494,   115,   116,   117,   522,   118,   757,   715,
     166,   214,   758,   532,   216,   534,   495,   535,   199,   200,
     737,   538,   539,   716,   119,   496,   178,   497,   120,   179,
     180,    95,   717,   760,   718,   498,   181,   758,   557,   182,
     183,   286,   719,   184,   185,   429,   430,   831,   753,   574,
     541,   758,   628,   631,   186,   543,   544,   789,   779,   780,
     217,   482,   790,   791,   806,   545,   546,   810,   252,   253,
     254,   547,   792,   793,   605,   606,   607,   646,   794,   351,
     352,   353,   618,   619,   620,   621,   622,   623,   624,   625,
     259,   634,   724,   260,   261,   726,   453,   224,   833,   870,
     262,   286,   871,   263,   264,   199,   200,   265,   891,   244,
     245,   178,   186,   228,   179,   180,   636,   637,   638,   679,
     245,   181,   381,   239,   182,   183,   529,   530,   184,   185,
     245,   645,   389,   285,   647,   648,   236,   245,   868,   395,
     415,   653,   312,   259,   255,   169,   260,   261,   170,   171,
     424,   286,   312,   262,   287,   172,   263,   264,   173,   174,
     265,   288,   175,   176,   665,   340,   341,   342,   343,   344,
     345,   289,   347,   348,   349,   350,   266,   267,   675,   676,
     678,   772,   302,   751,   773,   680,   774,   775,   178,   548,
     245,   179,   180,   354,   355,   356,   729,   186,   181,   552,
     245,   182,   183,   305,   879,   184,   185,   736,   688,   654,
     655,   689,   690,   691,   692,   309,   706,   707,   708,   709,
     710,   711,   553,   555,   663,   430,   558,   559,   561,   266,
     267,   177,   731,   245,   304,    69,   889,   311,   311,   428,
     432,   743,   430,   584,   585,   586,   308,   311,   727,   449,
       4,     5,     6,     7,   786,   800,   245,   314,   315,   316,
     317,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   318,   319,    24,   186,   310,    25,    26,    27,    28,
     320,   321,    31,    32,   841,   430,   313,   799,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     311,   368,   451,    45,    46,   568,   373,   569,    47,   784,
     149,   151,    48,    85,   374,    86,    87,    88,    89,    90,
      91,    92,   568,   245,   595,   603,   245,    93,   633,   245,
     245,   635,   639,    49,    50,   641,   245,   642,   643,   375,
      94,   797,   798,    95,   384,   644,    51,   312,   530,   376,
     649,   666,   568,    52,   658,   245,   245,   661,   662,    53,
     377,    54,   568,   378,   696,    55,   322,   382,   568,   808,
     697,   383,   681,   814,   245,   385,   720,    85,   818,    86,
      87,    88,    89,    90,    91,    92,   641,   245,   721,   722,
     735,    93,   312,   386,   387,   836,   837,   311,   245,   742,
     796,   361,   362,   838,   465,   388,   391,    95,   245,   245,
     835,   840,   568,   568,   844,   852,   850,   568,   394,   888,
     413,   414,   433,   445,   447,   450,   452,   460,   461,   468,
     862,   466,   483,   484,   499,   866,   485,   486,   487,   488,
     489,   490,   491,   204,   492,   493,   494,   495,   496,   497,
     498,   502,   508,   509,   875,   876,   878,   741,   500,   501,
     510,   745,   746,   747,   517,   518,   526,   519,   520,   521,
     531,   541,   542,   543,   754,   544,   580,   545,   756,   546,
       4,     5,     6,     7,   547,   550,   551,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   570,   571,    24,   283,   575,    25,    26,    27,    28,
      29,   572,    31,    32,   576,   581,   582,   587,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     583,   588,   604,    45,    46,   632,   801,   802,    47,   634,
     652,   656,    48,   659,   660,   667,   668,   669,   670,   671,
     682,   683,   684,   693,   698,   699,   700,   703,   723,   724,
     704,   725,   726,    49,    50,   728,   730,   732,   733,   738,
     734,   739,   755,   776,   777,   778,    51,   781,   782,   783,
     787,   788,   795,    52,   803,   804,   807,   811,   813,    53,
     812,    54,   815,   816,   834,    55,   573,   817,     4,     5,
       6,     7,    69,   829,   830,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   846,
     847,    24,   839,   758,    25,    26,    27,    28,    29,   848,
      31,    32,   853,   845,   859,   873,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,   860,   863,
     864,    45,    46,   869,   630,   854,    47,   855,   856,   857,
      48,   858,   861,   589,   865,   872,   881,   882,   883,   884,
     885,   886,   359,     0,   528,   360,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,    53,     0,    54,
       0,     0,     0,    55,   849,     4,     5,     6,     7,     0,
       0,     0,   110,   103,   104,   105,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,     0,     0,     0,    38,    39,
      40,     0,     0,    43,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    47,     0,     0,   110,   103,   104,   105,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,    51,    38,    39,    40,     0,     0,    43,    52,     0,
       0,     0,     0,     0,   135,     0,    54,    47,     0,     0,
      55,   472,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   110,   103,   104,   105,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,  -349,  -349,    51,   106,    28,     0,     0,
       0,     0,    52,     0,     0,     0,    33,     0,   135,     0,
      54,    38,    39,    40,    55,   677,    43,     0,     0,     0,
       0,     0,     0,     0,  -349,  -349,    47,  -349,  -349,  -349,
    -349,  -349,  -349,  -349,  -349,  -349,     0,     0,     0,  -349,
       0,     0,     0,     0,     0,     0,     0,  -349,  -349,  -349,
       0,  -349,     0,     0,  -349,  -349,  -349,  -349,  -349,  -349,
       0,  -349,  -349,  -349,    51,  -349,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,   135,     0,    54,
       0,     0,     0,    55,   877,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,    26,    27,    28,    29,    30,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,     0,     0,     0,    45,    46,
       0,     0,     0,    47,     0,     0,     0,    48,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,    49,    50,
     110,   103,   104,   105,    12,    13,    14,    15,    16,    17,
      18,    51,     0,     0,     0,     0,     0,     0,    52,     0,
       0,   106,    28,     0,    53,     0,    54,     0,     0,     0,
      55,    33,     0,     0,     0,     0,    38,    39,    40,     0,
       0,    43,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    47,     0,     0,   110,   103,   104,   105,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,    51,
      38,    39,    40,     0,     0,    43,    52,     0,     0,     0,
       0,     0,   111,     0,   112,    47,     0,     0,    55,     4,
       5,     6,     7,     0,     0,     0,   110,   103,   104,   105,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,    28,     0,
       0,     0,     0,    51,     0,     0,     0,    33,     0,     0,
      52,     0,    38,    39,    40,     0,   135,    43,    54,     0,
       0,     0,    55,     0,     0,     0,     0,    47,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   110,   103,   104,
     105,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   165,     0,     0,    55,     0,     0,     0,    47,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   110,   103,
     104,   105,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
      28,     0,     0,     0,     0,     0,    51,     0,     0,    33,
       0,     0,     0,    52,    38,    39,    40,     0,     0,    43,
       0,    54,   225,     0,     0,    55,     0,     0,     0,    47,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   110,
     103,   104,   105,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,    28,     0,     0,     0,     0,     0,    51,     0,     0,
      33,     0,     0,     0,    52,    38,    39,    40,     0,     0,
      43,     0,    54,   234,     0,     0,    55,     0,     0,     0,
      47,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     110,   103,   104,   105,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   241,     0,     0,    55,     0,     0,
       0,    47,     0,     0,   167,     4,     5,     6,     7,     0,
       0,     0,   110,   103,   104,   105,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,    28,     0,     0,     0,     0,    51,
       0,     0,     0,    33,     0,     0,    52,     0,    38,    39,
      40,     0,     0,    43,    54,     0,     0,     0,    55,     0,
       0,     0,     0,    47,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   110,   103,   104,   105,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,    28,     0,     0,     0,     0,
       0,    51,     0,     0,    33,     0,     0,     0,    52,    38,
      39,    40,     0,     0,    43,     0,    54,   463,     0,     0,
      55,     0,     0,     0,    47,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   110,   103,   104,   105,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,    28,     0,     0,     0,
       0,     0,    51,     0,     0,    33,     0,     0,     0,    52,
      38,    39,    40,     0,     0,    43,     0,    54,   503,     0,
       0,    55,     0,     0,     0,    47,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   110,   103,   104,   105,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,    28,     0,     0,
       0,     0,     0,    51,     0,     0,    33,     0,     0,     0,
      52,    38,    39,    40,     0,     0,    43,     0,    54,   506,
       0,     0,    55,     0,     0,     0,    47,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   110,   103,   104,   105,
     513,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,    28,     0,
       0,     0,     0,     0,    51,     0,     0,    33,     0,     0,
       0,    52,    38,    39,    40,     0,     0,    43,     0,    54,
     511,     0,     0,    55,     0,     0,     0,    47,     0,     0,
       4,     5,   400,     7,     0,     0,     0,   110,   103,   104,
     105,   401,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   514,     0,     0,    55,     0,     0,     0,    47,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   110,   103,
     104,   105,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
      28,     0,     0,     0,     0,     0,    51,     0,     0,    33,
       0,     0,     0,    52,    38,    39,    40,     0,     0,    43,
       0,    54,   165,     0,     0,    55,     0,     0,     0,    47,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   110,
     103,   104,   105,   513,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,    28,     0,     0,     0,     0,     0,    51,     0,     0,
      33,     0,     0,     0,    52,    38,    39,    40,     0,     0,
      43,     0,    54,   627,     0,     0,    55,     0,     0,     0,
      47,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     110,   103,   104,   105,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   629,     0,     0,    55,     0,     0,
       0,    47,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   110,   103,   104,   105,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,    28,     0,     0,     0,     0,     0,    51,
       0,     0,    33,     0,     0,     0,    52,    38,    39,    40,
       0,     0,    43,     0,    54,   785,     0,     0,    55,     0,
       0,     0,    47,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   110,   103,   104,   105,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,    28,     0,     0,     0,     0,     0,
      51,     0,     0,    33,     0,     0,     0,    52,    38,    39,
      40,     0,     0,    43,     0,   121,     0,     0,     0,    55,
       0,     0,     0,    47,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   110,   103,   104,   105,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,    28,     0,     0,     0,     0,
       0,    51,     0,     0,    33,     0,     0,     0,    52,    38,
      39,    40,     0,     0,    43,     0,   123,     0,     0,     0,
      55,     0,     0,     0,    47,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   110,   103,   104,   105,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,    28,     0,     0,     0,
       0,     0,    51,     0,     0,    33,     0,     0,     0,    52,
      38,    39,    40,     0,     0,    43,     0,   125,     0,     0,
       0,    55,     0,     0,     0,    47,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   110,   103,   104,   105,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,    28,     0,     0,
       0,     0,     0,    51,     0,     0,    33,     0,     0,     0,
      52,    38,    39,    40,     0,     0,    43,     0,   127,     0,
       0,     0,    55,     0,     0,     0,    47,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   110,   103,   104,   105,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,    28,     0,
       0,     0,     0,     0,    51,     0,     0,    33,     0,     0,
       0,    52,    38,    39,    40,     0,     0,    43,     0,    54,
       0,     0,     0,    55,     0,     0,     0,    47,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   110,   103,   104,
     105,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
     142,     0,     0,     0,    55,     0,     0,     0,    47,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   110,   103,
     104,   105,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
      28,     0,     0,     0,     0,     0,    51,     0,     0,    33,
       0,     0,     0,    52,    38,    39,    40,     0,     0,    43,
       0,   144,     0,     0,     0,    55,     0,     0,     0,    47,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   110,
     103,   104,   105,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,    28,     0,     0,     0,     0,     0,    51,     0,     0,
      33,     0,     0,     0,    52,    38,    39,    40,     0,     0,
      43,     0,    54,     0,     0,     0,   150,     0,     0,     0,
      47,     0,     0,     4,     5,   400,     7,     0,     0,     0,
     110,   103,   104,   105,   401,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   398,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   110,   103,   104,   105,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,    28,     0,     0,     0,     0,     0,    51,
       0,     0,    33,     0,     0,     0,    52,    38,    39,    40,
       0,     0,    43,     0,   402,     0,     0,     0,    55,     0,
       0,     0,    47,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   110,   103,   104,   105,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,    28,     0,     0,     0,     0,     0,
      51,     0,     0,    33,     0,     0,     0,    52,    38,    39,
      40,     0,     0,    43,     0,   533,     0,     0,     0,    55,
       0,     0,     0,    47,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   102,   103,   104,   105,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,     0,     0,     0,     0,     0,
       0,    51,     0,     0,    33,     0,     0,     0,    52,    38,
      39,    40,     0,     0,    43,     0,   556,     0,     0,     0,
      55,     0,     0,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,    55
};

static const yytype_int16 yycheck[] =
{
       2,     0,   305,   193,   553,   429,    10,   255,    15,    10,
      14,    26,    26,    14,    14,    10,    37,    38,    14,    14,
      22,    23,    24,    25,    26,    10,    22,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    95,    40,    14,
      69,    43,    26,   101,    51,    52,    10,    53,    54,   100,
      14,    57,    54,    96,   112,    14,     0,    14,    14,    43,
      53,    54,   116,    22,    57,    22,    14,   118,    14,   112,
     113,    17,    70,    71,    22,    26,    14,    14,    66,    67,
      68,    10,   111,    72,    59,   114,    88,    89,    90,    91,
      92,    93,    43,    31,    96,    72,    98,    99,   119,    55,
     118,    60,   111,    60,   118,   114,   104,    26,   411,    55,
     112,   115,    60,   119,   115,   104,    98,   666,   118,   121,
     115,   123,   118,   125,    43,   127,   119,   104,   112,   113,
     115,   119,   114,    62,    63,   119,   115,   115,   100,    98,
     142,    98,   144,    10,    14,   147,   148,   149,   150,   151,
      98,   115,    22,   112,     5,   112,   118,   460,   461,    98,
      12,   112,   113,    14,   112,    98,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   114,   178,   179,   180,   181,
     182,   183,   184,   185,   687,    12,     2,   202,   203,    72,
      60,   439,    98,   112,   113,    62,    63,   100,    70,    71,
     119,   208,   209,   210,   211,   212,    26,   118,   114,   267,
     458,    27,    28,    14,   272,   118,    17,   112,    10,    98,
     278,    37,    14,    43,   119,    41,    42,   100,    98,    69,
     113,    72,   104,   112,   236,   118,   238,    26,   240,   663,
      29,    30,   112,   245,   246,   118,   248,    36,   250,    14,
      39,    40,   801,   311,    43,   100,    98,   259,   260,   261,
     262,   263,   264,   265,    98,   280,   281,   269,   270,    98,
     112,   111,   113,   118,   114,   778,    10,   118,   112,    14,
      14,    98,    10,   112,   283,   287,   288,    98,   536,   291,
     106,   293,   112,   113,    26,   112,   114,    29,    30,   119,
     118,   112,   805,   113,    36,   115,   809,    39,    40,   499,
     126,    43,   128,   129,   372,    14,   318,   319,   320,   321,
      10,   569,   380,   112,   113,   105,   106,   143,   386,   145,
     388,    94,    73,    74,    75,    76,   112,    64,    79,    80,
      81,   844,    93,   119,   107,   108,   109,   595,   596,   852,
     114,    10,   168,    95,   118,    14,   112,   415,    26,   100,
     101,    29,    30,   119,   867,    10,   424,   112,    36,    14,
      14,    39,    40,   111,   119,    43,    44,   379,    97,   112,
     112,   113,   112,    77,    78,   888,   119,   119,   112,   119,
     114,    85,   112,    10,    11,    12,   398,    14,   114,   119,
     402,   111,   118,   405,   112,   407,   112,   409,   102,   103,
     658,   413,   414,   119,    31,   112,    26,   112,    35,    29,
      30,   114,   119,   114,   119,   112,    36,   118,   430,    39,
      40,   112,   119,    43,    44,    27,    28,   114,   119,   441,
     112,   118,   500,   501,   112,   112,   112,   119,   696,   697,
     112,   119,   119,   119,   757,   112,   112,   760,    10,    11,
      12,   112,   119,   119,   466,   467,   468,   525,   119,     7,
       8,     9,   474,   475,   476,   477,   478,   479,   480,   481,
      26,   112,   112,    29,    30,   112,   302,    14,   119,   119,
      36,   112,   119,    39,    40,   102,   103,    43,   119,   112,
     113,    26,   112,    14,    29,    30,   508,   509,   510,   119,
     113,    36,   115,   112,    39,    40,   112,   113,    43,    44,
     113,   523,   115,   112,   526,   527,   116,   113,   831,   115,
     113,   533,   115,    26,    14,    26,    29,    30,    29,    30,
     113,   112,   115,    36,    98,    36,    39,    40,    39,    40,
      43,    98,    43,    44,   556,   187,   188,   189,   190,   191,
     192,   118,   194,   195,   196,   197,   112,   113,   570,   571,
     572,    10,    50,   119,    13,   577,    15,    16,    26,   112,
     113,    29,    30,   199,   200,   201,   644,   112,    36,   112,
     113,    39,    40,   114,   119,    43,    44,   655,    10,   112,
     113,    13,    14,    15,    16,   112,   608,   609,   610,   611,
     612,   613,   428,   429,    27,    28,   432,   433,   434,   112,
     113,   112,   112,   113,   118,   441,   119,   113,   113,   115,
     115,    27,    28,   449,   450,   451,   118,   113,   640,   115,
       3,     4,     5,     6,   702,   112,   113,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   112,   112,    29,    30,    31,    32,
      33,    34,    35,    36,    27,    28,   119,   735,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     113,    14,   115,    56,    57,   113,   112,   115,    61,   701,
      39,    40,    65,    84,   112,    86,    87,    88,    89,    90,
      91,    92,   113,   113,   115,   115,   113,    98,   115,   113,
     113,   115,   115,    86,    87,   113,   113,   115,   115,   112,
     111,   733,   734,   114,   119,   113,    99,   115,   113,   112,
     115,   557,   113,   106,   115,   113,   113,   115,   115,   112,
     112,   114,   113,   112,   115,   118,   119,   117,   113,   758,
     115,   101,   578,   765,   113,   112,   115,    84,   770,    86,
      87,    88,    89,    90,    91,    92,   113,   113,   115,   115,
     113,    98,   115,   114,    14,   787,   788,   113,   113,   115,
     115,   206,   207,   795,   111,   114,    14,   114,   113,   113,
     115,   115,   113,   113,   115,   115,   808,   113,   112,   115,
      98,    98,    10,    14,    14,    27,   119,   114,   114,    98,
     822,   116,   112,   112,    82,   827,   112,   112,   112,   112,
     112,   112,   112,   104,   112,   112,   112,   112,   112,   112,
     112,    14,    98,    98,   846,   847,   848,   663,   114,   114,
      98,   667,   668,   669,   117,   101,    69,   119,   112,   112,
     112,   112,   112,   112,   680,   112,   101,   112,   684,   112,
       3,     4,     5,     6,   112,   112,   112,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   118,   112,    29,    30,    31,    32,
      33,    34,    35,    36,   112,   119,   101,    14,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
     119,    14,    14,    56,    57,   112,   742,   743,    61,   112,
     112,   112,    65,   112,   112,    10,    10,    10,    10,    10,
     112,   112,    27,    53,    14,    14,    14,   117,   112,   112,
     119,   112,   112,    86,    87,    14,   112,   112,    69,   112,
      69,   112,   119,   112,   112,    14,    99,     7,     7,     7,
      98,    98,    69,   106,    10,    10,    64,    14,   112,   112,
      14,   114,    14,    14,   119,   118,   119,   112,     3,     4,
       5,     6,   808,   112,   112,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   112,   118,    29,    30,    31,    32,    33,    34,
      35,    36,   112,   119,    14,   841,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    14,    14,
      14,    56,    57,   119,   501,   112,    61,   112,   112,   112,
      65,   112,   112,   456,   112,   112,   112,   112,   112,   112,
     112,   112,   204,    -1,   402,   205,    -1,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,    -1,   118,   119,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    61,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    99,    46,    47,    48,    -1,    -1,    51,   106,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    61,    -1,    -1,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    40,    99,    31,    32,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    41,    -1,   112,    -1,
     114,    46,    47,    48,   118,   119,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    61,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      -1,    97,    -1,    -1,   100,   101,   102,   103,   104,   105,
      -1,   107,   108,   109,    99,   111,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,    -1,   118,   119,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    -1,    -1,    56,    57,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    86,    87,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    99,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    31,    32,    -1,   112,    -1,   114,    -1,    -1,    -1,
     118,    41,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    61,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    99,
      46,    47,    48,    -1,    -1,    51,   106,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,    61,    -1,    -1,   118,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    41,    -1,    -1,
     106,    -1,    46,    47,    48,    -1,   112,    51,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    61,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,    -1,
      -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,    -1,
     114,   115,    -1,    -1,   118,    -1,    -1,    -1,    61,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,
      -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,
      -1,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,    61,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,
      51,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,
      61,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,
      -1,    51,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,    61,    -1,    -1,    64,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    41,    -1,    -1,   106,    -1,    46,    47,
      48,    -1,    -1,    51,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    41,    -1,    -1,    -1,   106,    46,
      47,    48,    -1,    -1,    51,    -1,   114,   115,    -1,    -1,
     118,    -1,    -1,    -1,    61,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    99,    -1,    -1,    41,    -1,    -1,    -1,   106,
      46,    47,    48,    -1,    -1,    51,    -1,   114,   115,    -1,
      -1,   118,    -1,    -1,    -1,    61,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    41,    -1,    -1,    -1,
     106,    46,    47,    48,    -1,    -1,    51,    -1,   114,   115,
      -1,    -1,   118,    -1,    -1,    -1,    61,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    99,    -1,    -1,    41,    -1,    -1,
      -1,   106,    46,    47,    48,    -1,    -1,    51,    -1,   114,
     115,    -1,    -1,   118,    -1,    -1,    -1,    61,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,    -1,
      -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,    -1,
     114,   115,    -1,    -1,   118,    -1,    -1,    -1,    61,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,
      -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,
      -1,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,    61,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,
      51,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,
      61,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,
      -1,    51,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,    61,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    41,    -1,    -1,    -1,   106,    46,    47,    48,
      -1,    -1,    51,    -1,   114,   115,    -1,    -1,   118,    -1,
      -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      99,    -1,    -1,    41,    -1,    -1,    -1,   106,    46,    47,
      48,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    41,    -1,    -1,    -1,   106,    46,
      47,    48,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    61,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    99,    -1,    -1,    41,    -1,    -1,    -1,   106,
      46,    47,    48,    -1,    -1,    51,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    61,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    41,    -1,    -1,    -1,
     106,    46,    47,    48,    -1,    -1,    51,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    61,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    99,    -1,    -1,    41,    -1,    -1,
      -1,   106,    46,    47,    48,    -1,    -1,    51,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    61,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,    -1,
      -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    61,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    41,
      -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,    51,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    61,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,    -1,
      51,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      61,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    41,    -1,    -1,    -1,   106,    46,    47,    48,    -1,
      -1,    51,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    61,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    -1,    41,    -1,    -1,    -1,   106,    46,    47,    48,
      -1,    -1,    51,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      99,    -1,    -1,    41,    -1,    -1,    -1,   106,    46,    47,
      48,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    41,    -1,    -1,    -1,   106,    46,
      47,    48,    -1,    -1,    51,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   118
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   121,   122,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    56,    57,    61,    65,    86,
      87,    99,   106,   112,   114,   118,   123,   124,   126,   127,
     128,   129,   130,   131,   133,   134,   137,   138,   141,   146,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   163,    84,    86,    87,    88,    89,
      90,    91,    92,    98,   111,   114,   116,   100,   118,    98,
     111,   114,    10,    11,    12,    13,    31,   146,   158,    14,
      10,   112,   114,   148,   162,    10,    11,    12,    14,    31,
      35,   114,   148,   114,   148,   114,   148,   114,   148,   148,
     154,    14,   146,   146,   148,   112,   148,    10,    14,   148,
      12,    12,   114,   148,   114,   148,   146,   118,   147,   147,
     118,   147,   148,   146,   146,   148,    14,    14,    14,    31,
      14,    10,    10,   158,   158,   115,   148,    64,   122,    26,
      29,    30,    36,    39,    40,    43,    44,   112,    26,    29,
      30,    36,    39,    40,    43,    44,   112,    73,    74,    75,
      76,    79,    80,    81,   100,   101,    77,    78,    85,   102,
     103,    93,    70,    71,   104,    95,   105,   106,    94,   107,
     108,   109,    97,   111,   111,    14,   112,   112,   148,   148,
     148,   148,   148,   148,    14,   115,   162,   148,    14,   148,
     148,    14,    17,    55,   115,   162,   116,   100,   118,   112,
      98,   115,   148,   162,   112,   113,    98,   112,    98,   112,
      98,   112,    10,    11,    12,    14,    10,    14,   148,    26,
      29,    30,    36,    39,    40,    43,   112,   113,   148,    26,
      43,   112,   113,   148,   146,   148,   146,    72,   113,   146,
      70,    71,   114,   118,   135,   112,   112,    98,    98,   118,
     100,   118,   100,   118,   148,   146,   148,   146,   148,   148,
     148,   148,    50,   139,   118,   114,    14,    59,   118,   112,
     112,   113,   115,   119,    10,    11,    12,    13,    24,    25,
      33,    34,   119,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     151,   151,   151,   151,   151,   151,   149,   151,   151,   151,
     151,     7,     8,     9,   153,   153,   153,   154,   154,   155,
     156,   157,   157,   158,   158,   158,   158,   158,    14,    14,
      17,   112,   114,   112,   112,   112,   112,   112,   112,    98,
     114,   115,   117,   101,   119,   112,   114,    14,   114,   115,
     148,    14,   148,   148,   112,   115,   148,   148,   114,   148,
       5,    14,   114,   144,   148,    98,   112,    98,   112,    98,
     112,   114,   135,    98,    98,   113,   148,   148,   148,   148,
     148,   148,   148,   162,   113,   148,   148,   162,   115,    27,
      28,   125,   115,    10,   162,   154,   154,    10,    14,   115,
     145,   122,   148,   148,   132,    14,   148,    14,   148,   115,
      27,   115,   119,   146,    14,    22,   118,   140,   115,   145,
     114,   114,   142,   115,   162,   111,   116,   118,    98,   148,
     148,   148,   119,   148,    26,    29,    30,    36,    39,    40,
      43,    44,   119,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,    82,
     114,   114,    14,   115,   162,   148,   115,   162,    98,    98,
      98,   115,   162,    14,   115,   143,   162,   117,   101,   119,
     112,   112,   148,    96,   112,   113,    69,    69,   144,   112,
     113,   112,   148,   114,   148,   148,   115,   145,   148,   148,
     162,   112,   112,   112,   112,   112,   112,   112,   112,   162,
     112,   112,   112,   146,   124,   146,   114,   148,   146,   146,
      72,   146,    72,    72,    10,    62,    63,   135,   113,   115,
      24,    25,    34,   119,   148,   112,   112,    37,    38,   119,
     101,   119,   101,   119,   146,   146,   146,    14,    14,   140,
      53,    54,    57,   119,   135,   115,   115,   145,   145,    66,
      67,    68,   119,   115,    14,   148,   148,   148,    26,    30,
      36,    39,    40,    43,   119,   119,   119,   119,   148,   148,
     148,   148,   148,   148,   148,   148,   149,   115,   162,   115,
     143,   162,   112,   115,   112,   115,   148,   148,   148,   115,
      69,   113,   115,   115,   113,   148,   162,   148,   148,   115,
       5,    14,   112,   148,   112,   113,   112,   135,   115,   112,
     112,   115,   115,    27,   125,   148,   146,    10,    10,    10,
      10,    10,    10,    14,   135,   148,   148,   119,   148,   119,
     148,   146,   112,   112,    27,   119,    14,    55,    10,    13,
      14,    15,    16,    53,   135,   135,   115,   115,    14,    14,
      14,    98,   114,   117,   119,   119,   148,   148,   148,   148,
     148,   148,   119,   119,   119,   119,   119,   119,   119,   119,
     115,   115,   115,   112,   112,   112,   112,   148,    14,   162,
     112,   112,   112,    69,    69,   113,   162,   135,   112,   112,
     124,   146,   115,    27,   125,   146,   146,   146,    10,    62,
      63,   119,   119,   119,   146,   119,   146,   114,   118,   136,
     114,   136,    14,    22,    60,    98,   112,    14,    22,    60,
      98,   112,    10,    13,    15,    16,   112,   112,    14,   135,
     135,     7,     7,     7,   148,   115,   162,    98,    98,   119,
     119,   119,   119,   119,   119,    69,   115,   148,   148,   162,
     112,   146,   146,    10,    10,   115,   145,    64,   122,   115,
     145,    14,    14,   112,   148,    14,    14,   112,   148,    14,
      22,    60,    98,   112,    14,    22,    60,    98,   112,   112,
     112,   114,   136,   119,   119,   115,   148,   148,   148,   112,
     115,    27,   125,   136,   115,   119,    24,    25,    34,   119,
     148,   136,   115,   112,   112,   112,   112,   112,   112,    14,
      14,   112,   148,    14,    14,   112,   148,   115,   145,   119,
     119,   119,   112,   146,   136,   148,   148,   119,   148,   119,
     136,   112,   112,   112,   112,   112,   112,   136,   115,   119,
     119,   119,   136
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   120,   121,   122,   122,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   124,   124,   124,   124,   124,
     124,   124,   124,   125,   125,   125,   125,   125,   125,   126,
     126,   127,   127,   127,   127,   128,   128,   129,   130,   130,
     130,   130,   130,   130,   131,   131,   132,   132,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   134,
     134,   135,   135,   135,   135,   135,   135,   136,   136,   136,
     136,   136,   136,   136,   137,   138,   139,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   141,   142,   142,   142,   142,   143,   143,
     144,   144,   144,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     147,   148,   148,   148,   149,   149,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     151,   151,   151,   152,   152,   153,   153,   153,   154,   154,
     155,   155,   156,   156,   156,   157,   157,   157,   157,   157,
     158,   158,   158,   158,   159,   159,   160,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   162,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163
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
       1,     1,     1,     1,     1,     5,     7,     7,     3,     5,
       5,     4,     6,     3,     5,     5,     7,     4,     6,     5,
       3,     5,     7,     3,     5,     5,     3,     2,     7,     7,
       5,     7,     5,     3,     5,     7,     0,     4,     6,     5,
       3,     7,     6,     4,     7,     6,     7,     6,     5,     6,
       5,     3,     4,     5,     4,     5,     5,     3,     3,     4,
       5,     4,     5,     5,     6,     5,     0,     3,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     6,     6,     6,     5,     5,     6,     6,
       7,     7,     7,     7,     7,     6,     4,     7,     6,     4,
       8,     7,     5,     5,     0,     4,     4,     4,     3,     5,
       3,     3,     5,     5,     1,     2,     3,     3,     3,     4,
       5,     5,     3,     3,     4,     6,     6,     6,     6,     6,
       6,     6,     6,     5,     7,     7,     7,     7,     7,     7,
       5,     9,     8,     8,     6,     9,     9,     5,     5,     4,
       3,     3,     2,     1,     5,     1,     3,     3,     3,     3,
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
#line 314 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2513 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 324 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2519 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 325 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2525 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 329 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2531 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 331 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2537 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 333 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2543 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 335 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2549 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 337 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2555 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 339 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2563 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 343 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2570 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 346 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2578 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 350 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2584 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 352 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2590 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 354 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2596 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 356 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2602 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 358 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2608 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 360 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2616 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 364 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2624 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 368 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2630 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 370 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2636 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 372 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2642 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 374 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2648 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 376 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2654 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 378 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2660 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 380 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2666 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 382 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2672 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 384 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2678 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 386 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2684 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 388 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2690 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 390 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2696 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 392 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2702 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 394 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2708 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 396 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2715 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2722 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2728 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 404 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2735 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 407 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2742 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 410 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2748 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 412 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2754 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 414 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2760 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 416 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2766 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 418 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2775 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 423 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2783 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 427 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2791 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 431 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2800 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 436 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2808 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 439 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2814 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 441 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2820 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 443 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2826 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 445 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2832 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 447 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2838 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 449 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2845 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 452 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2851 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 454 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2857 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 456 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2863 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 458 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2870 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 461 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2877 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 464 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2884 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 467 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2891 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 470 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2898 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 473 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2904 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 475 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2910 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 477 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2916 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 479 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2922 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 481 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2929 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 484 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2935 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 486 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2941 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 488 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2947 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 490 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 2954 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 493 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 2961 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 496 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 2969 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 500 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 2976 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 503 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 2982 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 505 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 2988 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 507 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 2994 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 509 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3001 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 512 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3008 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 515 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3015 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 518 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3022 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 521 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3029 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 524 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3036 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 527 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3043 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 530 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3050 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 533 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3057 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 536 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3064 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 539 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3071 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 541 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3077 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 542 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3083 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 543 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3089 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 544 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3095 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 545 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3101 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 546 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3107 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 548 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3113 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 550 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3119 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 552 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3125 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 554 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3131 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 555 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3137 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3143 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 557 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3149 "raku.tab.c"
    break;

  case 101: /* stmt: sub_decl  */
#line 558 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3155 "raku.tab.c"
    break;

  case 102: /* stmt: class_decl  */
#line 559 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3161 "raku.tab.c"
    break;

  case 103: /* stmt: role_decl  */
#line 560 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3167 "raku.tab.c"
    break;

  case 104: /* stmt: grammar_decl  */
#line 561 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3173 "raku.tab.c"
    break;

  case 105: /* if_stmt: KW_IF '(' expr ')' block  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3179 "raku.tab.c"
    break;

  case 106: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3185 "raku.tab.c"
    break;

  case 107: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3191 "raku.tab.c"
    break;

  case 108: /* if_stmt: KW_IF expr block  */
#line 571 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3197 "raku.tab.c"
    break;

  case 109: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 573 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3203 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 575 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3209 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF expr block elsif_tail  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3215 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3221 "raku.tab.c"
    break;

  case 113: /* elsif_tail: KW_ELSIF expr block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3227 "raku.tab.c"
    break;

  case 114: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3233 "raku.tab.c"
    break;

  case 115: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 587 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3239 "raku.tab.c"
    break;

  case 116: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3245 "raku.tab.c"
    break;

  case 117: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 591 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3251 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 593 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3257 "raku.tab.c"
    break;

  case 119: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 597 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3263 "raku.tab.c"
    break;

  case 120: /* while_stmt: KW_WHILE expr block  */
#line 599 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3269 "raku.tab.c"
    break;

  case 121: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 603 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3275 "raku.tab.c"
    break;

  case 122: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3281 "raku.tab.c"
    break;

  case 123: /* unless_stmt: KW_UNLESS expr block  */
#line 607 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3287 "raku.tab.c"
    break;

  case 124: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 609 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3293 "raku.tab.c"
    break;

  case 125: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 613 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3299 "raku.tab.c"
    break;

  case 126: /* until_stmt: KW_UNTIL expr block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3305 "raku.tab.c"
    break;

  case 127: /* repeat_stmt: KW_REPEAT block  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3311 "raku.tab.c"
    break;

  case 128: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 623 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3321 "raku.tab.c"
    break;

  case 129: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 629 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3331 "raku.tab.c"
    break;

  case 130: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 635 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3340 "raku.tab.c"
    break;

  case 131: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 640 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3350 "raku.tab.c"
    break;

  case 132: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 646 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3359 "raku.tab.c"
    break;

  case 133: /* for_stmt: KW_FOR expr block  */
#line 651 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3366 "raku.tab.c"
    break;

  case 134: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 656 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3378 "raku.tab.c"
    break;

  case 135: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 664 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3391 "raku.tab.c"
    break;

  case 136: /* when_list: %empty  */
#line 674 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3397 "raku.tab.c"
    break;

  case 137: /* when_list: when_list KW_WHEN expr block  */
#line 676 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3405 "raku.tab.c"
    break;

  case 138: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 682 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3417 "raku.tab.c"
    break;

  case 139: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 690 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3427 "raku.tab.c"
    break;

  case 140: /* sub_decl: KW_SUB IDENT sub_body  */
#line 696 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3437 "raku.tab.c"
    break;

  case 141: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 702 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3449 "raku.tab.c"
    break;

  case 142: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 710 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3459 "raku.tab.c"
    break;

  case 143: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 716 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3469 "raku.tab.c"
    break;

  case 144: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 722 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3482 "raku.tab.c"
    break;

  case 145: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 731 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3493 "raku.tab.c"
    break;

  case 146: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 738 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3506 "raku.tab.c"
    break;

  case 147: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 747 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3519 "raku.tab.c"
    break;

  case 148: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 756 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3530 "raku.tab.c"
    break;

  case 149: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 765 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3541 "raku.tab.c"
    break;

  case 150: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 772 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3550 "raku.tab.c"
    break;

  case 151: /* sub_body: '{' stmt_list '}'  */
#line 778 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3556 "raku.tab.c"
    break;

  case 152: /* sub_body: '{' stmt_list expr '}'  */
#line 780 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3563 "raku.tab.c"
    break;

  case 153: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 783 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3570 "raku.tab.c"
    break;

  case 154: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 786 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3576 "raku.tab.c"
    break;

  case 155: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 788 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3582 "raku.tab.c"
    break;

  case 156: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 790 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3588 "raku.tab.c"
    break;

  case 157: /* method_body: '{' stmt_list '}'  */
#line 793 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3594 "raku.tab.c"
    break;

  case 158: /* method_body: '{' YADA '}'  */
#line 794 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3600 "raku.tab.c"
    break;

  case 159: /* method_body: '{' stmt_list expr '}'  */
#line 796 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3607 "raku.tab.c"
    break;

  case 160: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 799 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3614 "raku.tab.c"
    break;

  case 161: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 802 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3620 "raku.tab.c"
    break;

  case 162: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 804 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3626 "raku.tab.c"
    break;

  case 163: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 806 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3632 "raku.tab.c"
    break;

  case 164: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 810 "raku.y"
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
#line 3650 "raku.tab.c"
    break;

  case 165: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 826 "raku.y"
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
#line 3667 "raku.tab.c"
    break;

  case 166: /* is_clauses: %empty  */
#line 840 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3673 "raku.tab.c"
    break;

  case 167: /* is_clauses: is_clauses IDENT IDENT  */
#line 842 "raku.y"
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
#line 3689 "raku.tab.c"
    break;

  case 168: /* is_clauses: is_clauses TESTOP IDENT  */
#line 854 "raku.y"
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
#line 3705 "raku.tab.c"
    break;

  case 169: /* class_body_list: %empty  */
#line 867 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3711 "raku.tab.c"
    break;

  case 170: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 869 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3718 "raku.tab.c"
    break;

  case 171: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 872 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3725 "raku.tab.c"
    break;

  case 172: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 875 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3732 "raku.tab.c"
    break;

  case 173: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 878 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3739 "raku.tab.c"
    break;

  case 174: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 881 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3746 "raku.tab.c"
    break;

  case 175: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 884 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3753 "raku.tab.c"
    break;

  case 176: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 887 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3760 "raku.tab.c"
    break;

  case 177: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 890 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3767 "raku.tab.c"
    break;

  case 178: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 893 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3774 "raku.tab.c"
    break;

  case 179: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 896 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3781 "raku.tab.c"
    break;

  case 180: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 899 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3788 "raku.tab.c"
    break;

  case 181: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 902 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3795 "raku.tab.c"
    break;

  case 182: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 905 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3806 "raku.tab.c"
    break;

  case 183: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 912 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3817 "raku.tab.c"
    break;

  case 184: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 919 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3828 "raku.tab.c"
    break;

  case 185: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 926 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3839 "raku.tab.c"
    break;

  case 186: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 933 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3847 "raku.tab.c"
    break;

  case 187: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 937 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3855 "raku.tab.c"
    break;

  case 188: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 941 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3863 "raku.tab.c"
    break;

  case 189: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 945 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3871 "raku.tab.c"
    break;

  case 190: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 949 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3882 "raku.tab.c"
    break;

  case 191: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 956 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3893 "raku.tab.c"
    break;

  case 192: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 963 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3904 "raku.tab.c"
    break;

  case 193: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 970 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3915 "raku.tab.c"
    break;

  case 194: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 977 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3929 "raku.tab.c"
    break;

  case 195: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 987 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3941 "raku.tab.c"
    break;

  case 196: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 995 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3953 "raku.tab.c"
    break;

  case 197: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1003 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3966 "raku.tab.c"
    break;

  case 198: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1012 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3977 "raku.tab.c"
    break;

  case 199: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1019 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3988 "raku.tab.c"
    break;

  case 200: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 1026 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4002 "raku.tab.c"
    break;

  case 201: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 1036 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4014 "raku.tab.c"
    break;

  case 202: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 1044 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4026 "raku.tab.c"
    break;

  case 203: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1054 "raku.y"
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
#line 4043 "raku.tab.c"
    break;

  case 204: /* grammar_body_list: %empty  */
#line 1068 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4049 "raku.tab.c"
    break;

  case 205: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1070 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4058 "raku.tab.c"
    break;

  case 206: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1075 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4067 "raku.tab.c"
    break;

  case 207: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1080 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4076 "raku.tab.c"
    break;

  case 208: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1087 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4084 "raku.tab.c"
    break;

  case 209: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1091 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4092 "raku.tab.c"
    break;

  case 210: /* pair_list: IDENT OP_FATARROW expr  */
#line 1097 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4098 "raku.tab.c"
    break;

  case 211: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1099 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4104 "raku.tab.c"
    break;

  case 212: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1101 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4110 "raku.tab.c"
    break;

  case 213: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1103 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4116 "raku.tab.c"
    break;

  case 214: /* param_list: VAR_SCALAR  */
#line 1106 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4122 "raku.tab.c"
    break;

  case 215: /* param_list: IDENT VAR_SCALAR  */
#line 1107 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4128 "raku.tab.c"
    break;

  case 216: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1108 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4134 "raku.tab.c"
    break;

  case 217: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1109 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4140 "raku.tab.c"
    break;

  case 218: /* param_list: param_list ',' VAR_SCALAR  */
#line 1110 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4146 "raku.tab.c"
    break;

  case 219: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1111 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4152 "raku.tab.c"
    break;

  case 220: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1112 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4158 "raku.tab.c"
    break;

  case 221: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1113 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4164 "raku.tab.c"
    break;

  case 222: /* block: '{' stmt_list '}'  */
#line 1116 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4170 "raku.tab.c"
    break;

  case 223: /* block: '{' YADA '}'  */
#line 1117 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4176 "raku.tab.c"
    break;

  case 224: /* block: '{' stmt_list expr '}'  */
#line 1119 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4182 "raku.tab.c"
    break;

  case 225: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1121 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4188 "raku.tab.c"
    break;

  case 226: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4194 "raku.tab.c"
    break;

  case 227: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1125 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4200 "raku.tab.c"
    break;

  case 228: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1127 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4206 "raku.tab.c"
    break;

  case 229: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1129 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4213 "raku.tab.c"
    break;

  case 230: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1132 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4219 "raku.tab.c"
    break;

  case 231: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1134 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4225 "raku.tab.c"
    break;

  case 232: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1136 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4231 "raku.tab.c"
    break;

  case 233: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1138 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4237 "raku.tab.c"
    break;

  case 234: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1140 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4244 "raku.tab.c"
    break;

  case 235: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1143 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4251 "raku.tab.c"
    break;

  case 236: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1146 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4259 "raku.tab.c"
    break;

  case 237: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1150 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4266 "raku.tab.c"
    break;

  case 238: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1153 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4273 "raku.tab.c"
    break;

  case 239: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1156 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4280 "raku.tab.c"
    break;

  case 240: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1159 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4286 "raku.tab.c"
    break;

  case 241: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1161 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4294 "raku.tab.c"
    break;

  case 242: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1165 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4301 "raku.tab.c"
    break;

  case 243: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1168 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4308 "raku.tab.c"
    break;

  case 244: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1171 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4315 "raku.tab.c"
    break;

  case 245: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1174 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4322 "raku.tab.c"
    break;

  case 246: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1177 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4329 "raku.tab.c"
    break;

  case 247: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1180 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4335 "raku.tab.c"
    break;

  case 248: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1182 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4341 "raku.tab.c"
    break;

  case 249: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1184 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4347 "raku.tab.c"
    break;

  case 250: /* closure: '{' expr '}'  */
#line 1187 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4353 "raku.tab.c"
    break;

  case 251: /* expr: VAR_SCALAR '=' expr  */
#line 1190 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4359 "raku.tab.c"
    break;

  case 252: /* expr: KW_GATHER block  */
#line 1191 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4369 "raku.tab.c"
    break;

  case 253: /* expr: tern_expr  */
#line 1196 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4375 "raku.tab.c"
    break;

  case 254: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1200 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4381 "raku.tab.c"
    break;

  case 255: /* tern_expr: cmp_expr  */
#line 1201 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4387 "raku.tab.c"
    break;

  case 256: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1204 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4393 "raku.tab.c"
    break;

  case 257: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1205 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4399 "raku.tab.c"
    break;

  case 258: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1206 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4405 "raku.tab.c"
    break;

  case 259: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1207 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4411 "raku.tab.c"
    break;

  case 260: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1208 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4417 "raku.tab.c"
    break;

  case 261: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1209 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4423 "raku.tab.c"
    break;

  case 262: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1210 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4429 "raku.tab.c"
    break;

  case 263: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1211 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4435 "raku.tab.c"
    break;

  case 264: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1212 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4441 "raku.tab.c"
    break;

  case 265: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1213 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4447 "raku.tab.c"
    break;

  case 266: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1215 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4457 "raku.tab.c"
    break;

  case 267: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1221 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4467 "raku.tab.c"
    break;

  case 268: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1227 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4477 "raku.tab.c"
    break;

  case 269: /* cmp_expr: jct_expr  */
#line 1232 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4483 "raku.tab.c"
    break;

  case 270: /* jct_expr: jct_expr '|' range_expr  */
#line 1235 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4489 "raku.tab.c"
    break;

  case 271: /* jct_expr: jct_expr '&' range_expr  */
#line 1236 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4495 "raku.tab.c"
    break;

  case 272: /* jct_expr: dor_expr  */
#line 1237 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4501 "raku.tab.c"
    break;

  case 273: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1241 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4507 "raku.tab.c"
    break;

  case 274: /* dor_expr: range_expr  */
#line 1242 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4513 "raku.tab.c"
    break;

  case 275: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1245 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4519 "raku.tab.c"
    break;

  case 276: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1246 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4525 "raku.tab.c"
    break;

  case 277: /* range_expr: add_expr  */
#line 1247 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4531 "raku.tab.c"
    break;

  case 278: /* add_expr: add_expr '~' repl_expr  */
#line 1250 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4537 "raku.tab.c"
    break;

  case 279: /* add_expr: repl_expr  */
#line 1251 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4543 "raku.tab.c"
    break;

  case 280: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1254 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4549 "raku.tab.c"
    break;

  case 281: /* repl_expr: addsub_expr  */
#line 1255 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4555 "raku.tab.c"
    break;

  case 282: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1258 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4561 "raku.tab.c"
    break;

  case 283: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1259 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4567 "raku.tab.c"
    break;

  case 284: /* addsub_expr: mul_expr  */
#line 1260 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4573 "raku.tab.c"
    break;

  case 285: /* mul_expr: mul_expr '*' unary_expr  */
#line 1263 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4579 "raku.tab.c"
    break;

  case 286: /* mul_expr: mul_expr '/' unary_expr  */
#line 1264 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4585 "raku.tab.c"
    break;

  case 287: /* mul_expr: mul_expr '%' unary_expr  */
#line 1265 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4591 "raku.tab.c"
    break;

  case 288: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1266 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4597 "raku.tab.c"
    break;

  case 289: /* mul_expr: unary_expr  */
#line 1267 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4603 "raku.tab.c"
    break;

  case 290: /* unary_expr: '-' unary_expr  */
#line 1270 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4609 "raku.tab.c"
    break;

  case 291: /* unary_expr: '!' unary_expr  */
#line 1271 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4615 "raku.tab.c"
    break;

  case 292: /* unary_expr: CARET unary_expr  */
#line 1272 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 4621 "raku.tab.c"
    break;

  case 293: /* unary_expr: pow_expr  */
#line 1273 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4627 "raku.tab.c"
    break;

  case 294: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1276 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4633 "raku.tab.c"
    break;

  case 295: /* pow_expr: postfix_expr  */
#line 1277 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4639 "raku.tab.c"
    break;

  case 296: /* postfix_expr: call_expr  */
#line 1279 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4645 "raku.tab.c"
    break;

  case 297: /* call_expr: IDENT '(' arg_list ')'  */
#line 1282 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4654 "raku.tab.c"
    break;

  case 298: /* call_expr: IDENT '(' ')'  */
#line 1286 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4660 "raku.tab.c"
    break;

  case 299: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1288 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4669 "raku.tab.c"
    break;

  case 300: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1293 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4675 "raku.tab.c"
    break;

  case 301: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1295 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4685 "raku.tab.c"
    break;

  case 302: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1301 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4695 "raku.tab.c"
    break;

  case 303: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1307 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4703 "raku.tab.c"
    break;

  case 304: /* call_expr: IDENT '.' KW_NEW  */
#line 1311 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4711 "raku.tab.c"
    break;

  case 305: /* call_expr: IDENT '.' IDENT  */
#line 1315 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4720 "raku.tab.c"
    break;

  case 306: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1320 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4731 "raku.tab.c"
    break;

  case 307: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1327 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4740 "raku.tab.c"
    break;

  case 308: /* call_expr: IDENT '.' CARET IDENT  */
#line 1332 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4750 "raku.tab.c"
    break;

  case 309: /* call_expr: atom '.' CARET IDENT  */
#line 1338 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4760 "raku.tab.c"
    break;

  case 310: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1344 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4771 "raku.tab.c"
    break;

  case 311: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1351 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4782 "raku.tab.c"
    break;

  case 312: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1358 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4791 "raku.tab.c"
    break;

  case 313: /* call_expr: atom '.' IDENT  */
#line 1363 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4800 "raku.tab.c"
    break;

  case 314: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1368 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4811 "raku.tab.c"
    break;

  case 315: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1375 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4820 "raku.tab.c"
    break;

  case 316: /* call_expr: call_expr '.' IDENT  */
#line 1380 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4829 "raku.tab.c"
    break;

  case 317: /* call_expr: KW_DIE expr  */
#line 1385 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 4835 "raku.tab.c"
    break;

  case 318: /* call_expr: KW_MAP closure expr  */
#line 1387 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4841 "raku.tab.c"
    break;

  case 319: /* call_expr: KW_GREP closure expr  */
#line 1389 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4847 "raku.tab.c"
    break;

  case 320: /* call_expr: KW_SORT expr  */
#line 1391 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4853 "raku.tab.c"
    break;

  case 321: /* call_expr: KW_SORT closure expr  */
#line 1393 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4859 "raku.tab.c"
    break;

  case 322: /* call_expr: atom  */
#line 1394 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4865 "raku.tab.c"
    break;

  case 323: /* arg_list: expr  */
#line 1397 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4871 "raku.tab.c"
    break;

  case 324: /* arg_list: arg_list ',' expr  */
#line 1398 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4877 "raku.tab.c"
    break;

  case 325: /* atom: LIT_INT  */
#line 1401 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4883 "raku.tab.c"
    break;

  case 326: /* atom: LIT_FLOAT  */
#line 1402 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4889 "raku.tab.c"
    break;

  case 327: /* atom: LIT_STR  */
#line 1403 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4895 "raku.tab.c"
    break;

  case 328: /* atom: WORDLIST  */
#line 1405 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4907 "raku.tab.c"
    break;

  case 329: /* atom: LIT_INTERP_STR  */
#line 1412 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4913 "raku.tab.c"
    break;

  case 330: /* atom: VAR_SCALAR  */
#line 1413 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4919 "raku.tab.c"
    break;

  case 331: /* atom: VAR_ARRAY  */
#line 1414 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4925 "raku.tab.c"
    break;

  case 332: /* atom: VAR_HASH  */
#line 1415 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4931 "raku.tab.c"
    break;

  case 333: /* atom: VAR_CAPTURE  */
#line 1417 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4939 "raku.tab.c"
    break;

  case 334: /* atom: VAR_FH  */
#line 1421 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4947 "raku.tab.c"
    break;

  case 335: /* atom: VAR_NAMED_CAPTURE  */
#line 1425 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4954 "raku.tab.c"
    break;

  case 336: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1428 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4960 "raku.tab.c"
    break;

  case 337: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1430 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4966 "raku.tab.c"
    break;

  case 338: /* atom: VAR_HASH '{' expr '}'  */
#line 1432 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4972 "raku.tab.c"
    break;

  case 339: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1434 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4978 "raku.tab.c"
    break;

  case 340: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1436 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4984 "raku.tab.c"
    break;

  case 341: /* atom: IDENT  */
#line 1437 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4990 "raku.tab.c"
    break;

  case 342: /* atom: VAR_TWIGIL  */
#line 1439 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4998 "raku.tab.c"
    break;

  case 343: /* atom: VAR_ARRAY_TWIGIL  */
#line 1443 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5006 "raku.tab.c"
    break;

  case 344: /* atom: VAR_HASH_TWIGIL  */
#line 1447 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5014 "raku.tab.c"
    break;

  case 345: /* atom: '(' ')'  */
#line 1450 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5020 "raku.tab.c"
    break;

  case 346: /* atom: '(' expr ')'  */
#line 1451 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5026 "raku.tab.c"
    break;

  case 347: /* atom: '(' expr ',' ')'  */
#line 1453 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5032 "raku.tab.c"
    break;

  case 348: /* atom: '(' expr ',' arg_list ')'  */
#line 1455 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5039 "raku.tab.c"
    break;

  case 349: /* atom: block  */
#line 1457 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5045 "raku.tab.c"
    break;

  case 350: /* atom: KW_SUB block  */
#line 1458 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5051 "raku.tab.c"
    break;


#line 5055 "raku.tab.c"

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

#line 1460 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
