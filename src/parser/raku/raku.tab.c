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
  YYSYMBOL_97_ = 97,                       /* '='  */
  YYSYMBOL_98_ = 98,                       /* '!'  */
  YYSYMBOL_99_ = 99,                       /* '<'  */
  YYSYMBOL_100_ = 100,                     /* '>'  */
  YYSYMBOL_101_ = 101,                     /* '|'  */
  YYSYMBOL_102_ = 102,                     /* '&'  */
  YYSYMBOL_103_ = 103,                     /* '~'  */
  YYSYMBOL_104_ = 104,                     /* '+'  */
  YYSYMBOL_105_ = 105,                     /* '-'  */
  YYSYMBOL_106_ = 106,                     /* '*'  */
  YYSYMBOL_107_ = 107,                     /* '/'  */
  YYSYMBOL_108_ = 108,                     /* '%'  */
  YYSYMBOL_UMINUS = 109,                   /* UMINUS  */
  YYSYMBOL_110_ = 110,                     /* '.'  */
  YYSYMBOL_111_ = 111,                     /* ';'  */
  YYSYMBOL_112_ = 112,                     /* ','  */
  YYSYMBOL_113_ = 113,                     /* '('  */
  YYSYMBOL_114_ = 114,                     /* ')'  */
  YYSYMBOL_115_ = 115,                     /* '['  */
  YYSYMBOL_116_ = 116,                     /* ']'  */
  YYSYMBOL_117_ = 117,                     /* '{'  */
  YYSYMBOL_118_ = 118,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 119,                 /* $accept  */
  YYSYMBOL_program = 120,                  /* program  */
  YYSYMBOL_stmt_list = 121,                /* stmt_list  */
  YYSYMBOL_stmt = 122,                     /* stmt  */
  YYSYMBOL_if_stmt = 123,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 124,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 125,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 126,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 127,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 128,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 129,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 130,               /* given_stmt  */
  YYSYMBOL_when_list = 131,                /* when_list  */
  YYSYMBOL_sub_decl = 132,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 133,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 134,                 /* sub_body  */
  YYSYMBOL_method_body = 135,              /* method_body  */
  YYSYMBOL_class_decl = 136,               /* class_decl  */
  YYSYMBOL_role_decl = 137,                /* role_decl  */
  YYSYMBOL_is_clauses = 138,               /* is_clauses  */
  YYSYMBOL_class_body_list = 139,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 140,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 141,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 142,           /* named_arg_list  */
  YYSYMBOL_pair_list = 143,                /* pair_list  */
  YYSYMBOL_param_list = 144,               /* param_list  */
  YYSYMBOL_block = 145,                    /* block  */
  YYSYMBOL_closure = 146,                  /* closure  */
  YYSYMBOL_expr = 147,                     /* expr  */
  YYSYMBOL_tern_expr = 148,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 149,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 150,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 151,                 /* dor_expr  */
  YYSYMBOL_range_expr = 152,               /* range_expr  */
  YYSYMBOL_add_expr = 153,                 /* add_expr  */
  YYSYMBOL_repl_expr = 154,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 155,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 156,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 157,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 158,             /* postfix_expr  */
  YYSYMBOL_call_expr = 159,                /* call_expr  */
  YYSYMBOL_arg_list = 160,                 /* arg_list  */
  YYSYMBOL_atom = 161                      /* atom  */
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
#define YYLAST   3237

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  119
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  334
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  848

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   352


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
       2,     2,     2,    98,     2,     2,     2,   108,   102,     2,
     113,   114,   106,   104,   112,   105,   110,   107,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   111,
      99,    97,   100,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   115,     2,   116,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   117,   101,   118,   103,     2,     2,     2,
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
      95,    96,   109
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   311,   311,   322,   323,   326,   328,   330,   332,   334,
     336,   340,   343,   347,   349,   351,   353,   355,   357,   361,
     365,   367,   369,   371,   373,   375,   377,   379,   381,   383,
     385,   387,   389,   391,   393,   396,   399,   401,   404,   407,
     409,   411,   413,   415,   420,   424,   428,   433,   437,   438,
     440,   442,   444,   446,   449,   451,   453,   455,   458,   461,
     464,   467,   470,   472,   474,   476,   478,   481,   483,   485,
     487,   490,   493,   497,   500,   502,   504,   506,   509,   512,
     515,   518,   521,   524,   527,   530,   533,   536,   539,   540,
     541,   542,   543,   544,   545,   547,   549,   551,   553,   554,
     555,   556,   557,   558,   559,   562,   564,   566,   568,   570,
     572,   574,   576,   580,   582,   584,   586,   588,   590,   594,
     596,   600,   602,   604,   606,   610,   612,   616,   620,   626,
     632,   637,   643,   648,   653,   661,   672,   673,   679,   687,
     693,   699,   707,   713,   719,   728,   735,   744,   753,   762,
     769,   776,   777,   780,   783,   785,   787,   791,   792,   793,
     796,   799,   801,   803,   807,   823,   838,   839,   851,   865,
     866,   869,   872,   875,   878,   881,   884,   887,   890,   893,
     896,   899,   902,   909,   916,   923,   930,   934,   938,   942,
     946,   953,   960,   967,   974,   984,   992,  1000,  1009,  1016,
    1023,  1033,  1041,  1051,  1066,  1067,  1072,  1077,  1084,  1088,
    1094,  1096,  1098,  1100,  1104,  1105,  1106,  1107,  1108,  1109,
    1110,  1111,  1114,  1115,  1116,  1118,  1120,  1122,  1126,  1129,
    1132,  1135,  1138,  1141,  1143,  1145,  1149,  1152,  1153,  1158,
    1161,  1163,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,
    1174,  1175,  1176,  1182,  1188,  1194,  1197,  1198,  1199,  1202,
    1204,  1207,  1208,  1209,  1212,  1213,  1216,  1217,  1220,  1221,
    1222,  1225,  1226,  1227,  1228,  1229,  1232,  1233,  1234,  1235,
    1237,  1239,  1244,  1245,  1250,  1252,  1258,  1264,  1268,  1272,
    1277,  1284,  1289,  1295,  1301,  1308,  1315,  1320,  1325,  1332,
    1337,  1342,  1344,  1346,  1348,  1350,  1352,  1355,  1356,  1359,
    1360,  1361,  1362,  1370,  1371,  1372,  1373,  1374,  1378,  1382,
    1385,  1387,  1389,  1391,  1393,  1395,  1396,  1400,  1404,  1408,
    1409,  1410,  1412,  1415,  1416
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
  "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX", "'='", "'!'", "'<'", "'>'",
  "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'",
  "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "$accept",
  "program", "stmt_list", "stmt", "if_stmt", "elsif_tail", "while_stmt",
  "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt", "given_stmt",
  "when_list", "sub_decl", "scalar_methcall", "sub_body", "method_body",
  "class_decl", "role_decl", "is_clauses", "class_body_list",
  "grammar_decl", "grammar_body_list", "named_arg_list", "pair_list",
  "param_list", "block", "closure", "expr", "tern_expr", "cmp_expr",
  "jct_expr", "dor_expr", "range_expr", "add_expr", "repl_expr",
  "addsub_expr", "mul_expr", "unary_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-649)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-334)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -649,    54,  1321,  -649,  -649,  -649,  -649,  -649,   540,   -48,
     -41,   -24,   189,  -649,  -649,  3120,  -649,  -649,  -649,    78,
    1436,   318,  2400,  2460,  2520,  2580,  2640,    -3,   -15,  2640,
    1489,     2,  2640,    70,   152,  2700,  2760,   -15,     0,     0,
    2820,   -15,   -15,  2640,   173,   211,   182,  -649,   224,   205,
     240,  3120,  3120,  -649,  1557,   230,  -649,  -649,  -649,  -649,
    -649,  -649,  -649,  -649,  -649,   462,  -649,  -649,  -649,  3109,
     578,  -649,   315,   375,   231,  -649,   118,   209,   301,   237,
    -649,  -649,   254,   260,   378,   305,   343,  2640,  2640,  2640,
    2640,  2640,  2640,   384,  1617,  2640,   443,  2640,  2640,   197,
    1677,   313,   333,   -16,  -649,   -15,  -649,  -649,   356,   120,
    -649,  1737,  -649,   307,   113,   170,   177,   435,   457,   144,
    1557,   429,  1557,    51,  1557,   -15,  1557,   -15,   -43,   160,
     110,  -649,  -649,   364,  -649,   396,   381,   385,   369,    86,
      92,  1557,   -15,  1557,   -15,  -649,  2640,  2640,  2640,  1797,
    2640,  -649,   469,  -649,  -649,  -649,   437,   447,    11,   449,
     460,   487,  -649,  -649,  -649,   159,   484,   804,  2640,  2640,
    2640,  2640,  2640,  2640,  2640,  2640,  -649,  2640,  2640,  2640,
    2640,  2640,  2640,  2640,  2640,  -649,  3120,  3120,  3120,  3120,
    3120,  3120,  3120,  3120,  3120,  3120,  3120,   526,  3120,  3120,
    3120,  3120,  3120,  3120,  3120,  3120,  3120,  3120,  3120,  3120,
    3120,   592,   304,   269,  -649,  -649,   498,   505,   509,   512,
     541,   556,   140,  -649,   337,   560,   548,   561,   580,   574,
     680,   605,  -649,   352,  2640,   709,  2640,  -649,  2640,   617,
     159,   367,  -649,  2640,  2640,  -649,  2880,  -649,  2940,  -649,
     179,   198,   201,   252,   639,   640,   403,  2640,  2640,  2640,
    2640,  2640,  2640,  2640,  -649,  2640,   430,  2640,  2640,  -649,
    2640,   438,   466,   439,  -649,   728,  2640,  -649,  3120,  3120,
      -4,  -649,  -649,  -649,  -649,  2640,  2640,  -649,   735,  2640,
     736,  2640,   451,   715,   473,  -649,   633,  -649,  -649,  -649,
     -15,     4,  -649,    -1,   641,   642,  -649,  -649,  -649,  1860,
    -649,  -649,   612,   637,   105,   656,  2400,  2460,  2640,   564,
    -649,   312,   646,   647,   648,   651,   652,   653,   654,   655,
     657,   658,   659,   660,   661,   662,   663,   665,   388,   388,
     388,   388,   388,   388,   685,   388,   388,   388,   388,  -649,
    -649,  -649,  -649,  -649,  -649,   674,   674,   209,   301,   237,
     237,  -649,  -649,  -649,  -649,   666,   668,   764,  -649,  1920,
    -649,  -649,  -649,  -649,  -649,  -649,  2640,  1980,  -649,   686,
     687,   688,  -649,  2040,  -649,  2100,  -649,   670,   682,   669,
    -649,  -649,   677,  -649,   678,  1557,   166,   721,    -5,  2160,
     392,   681,  2640,  -649,  3000,  -649,  2640,  -649,     5,  -649,
    2640,  2640,  1860,   683,   684,   691,   692,   700,   701,   702,
     412,  1860,   720,   730,   417,   -15,     6,  3060,  -649,   -15,
     -15,   -33,   -35,    34,  -649,    37,   676,   474,   920,   731,
     732,    43,   696,   673,   697,   714,   -15,   -15,   -15,  -649,
    -649,   784,   792,  -649,     9,   676,   477,    21,   363,    -7,
    -649,   478,   830,  2640,  2640,  2640,   267,    14,   161,  -649,
     202,  -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,
    -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,  3120,  2220,
    2280,  -649,   746,   485,   747,  -649,   489,  2640,  2640,  2640,
    -649,   521,    84,  -649,   522,   527,  -649,  -649,  -649,  -649,
    -649,   528,  2640,  -649,  2640,  2640,  2640,   531,  -649,   287,
    -649,   748,  1557,   434,   751,   676,   532,   752,   755,   537,
    -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,   558,  -649,
    -649,  -649,   472,  -649,  -649,  1557,   -15,  -649,  -649,   857,
    -649,   858,   860,  -649,   861,   862,  -649,   389,   676,  2400,
    2460,  1152,  -649,   332,  -649,  -649,  2640,   -15,  -649,  -649,
    -649,   762,   763,   848,  -649,  -649,  -649,  -649,    18,    42,
     397,   823,  -649,  -649,   676,   676,   559,   566,   863,   864,
     865,  -649,  -649,   167,   765,   766,   215,  -649,  -649,  -649,
    -649,  -649,  -649,   571,  -649,   572,   576,  -649,   769,  -649,
    -649,   771,   772,   774,  -649,  2640,   872,  -649,  -649,  1860,
     776,   543,  -649,  -649,   777,   820,   824,  -649,   583,  -649,
    2640,  -649,  -649,   676,  -649,  -649,   781,   783,     6,  -649,
     593,   699,   -15,   -15,   -15,  -649,  -649,  -649,    68,  -649,
     444,    22,  -649,   228,  -649,   -15,   778,  -649,  -649,   -15,
    -649,   316,   322,   138,   194,   371,   787,   788,   881,  -649,
    -649,   676,   676,   893,   894,   896,  2640,  2340,   807,   808,
    -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,   837,
     594,  -649,  -649,  -649,  2640,  2640,  1860,   619,  -649,  -649,
    -649,  -649,  -649,   -15,   -15,  -649,  -649,  -649,  -649,  -649,
     897,   898,  -649,  -649,  -649,  -649,  -649,  -649,    35,   846,
    -649,    36,  -649,   899,   900,   800,  2640,  -649,   902,   904,
     801,  2640,  -649,   225,   226,   809,   816,  -649,  -649,   327,
    -649,  -649,  -649,  -649,  -649,   229,   810,   598,  2640,  2640,
    2640,   818,  -649,  -649,   599,  -649,   705,  -649,  -649,  -649,
     802,   602,   829,  1036,   802,   603,   847,   849,  -649,   867,
     868,   869,  -649,   871,   934,   943,   873,  2640,  -649,   945,
     959,   875,  2640,  -649,  -649,  -649,    41,  -649,  -649,  -649,
     856,   239,   248,  -649,  -649,   876,   -15,  -649,  -649,   802,
    -649,  2400,  2460,  1205,  -649,   398,  -649,   802,  -649,  -649,
    -649,  -649,  -649,  -649,   877,   878,  -649,   879,   880,   882,
    -649,   883,   802,   607,  -649,  -649,  -649,  -649,  -649,  -649,
     482,    72,  -649,   249,  -649,  -649,  -649,  -649,  -649,  -649,
    -649,  -649,  -649,   802,  -649,  -649,  -649,  -649
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   309,   310,   311,   313,   314,   315,
     316,   326,   325,   327,   328,     0,   317,   318,   319,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,    89,     0,     3,     4,    90,    91,    98,
      99,   100,    92,    93,   101,     0,   102,   103,   104,    97,
       0,   239,   241,   255,   258,   260,   263,   265,   267,   270,
     275,   279,   280,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   314,   315,   316,   326,     0,   333,   278,     0,   314,
      29,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   334,   238,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   127,     0,     0,     0,     3,
       0,   304,    94,    96,   301,   166,     0,     0,     0,     0,
       0,     0,   277,   276,   329,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,    85,     0,     0,     0,     0,
       0,   237,     0,   284,     0,     0,     0,     0,     0,   289,
       0,   288,   282,     0,     0,     0,     0,    24,     0,   329,
       0,     0,    32,     0,     0,     6,     0,     7,     0,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,     0,    36,
       0,     0,   108,     0,   120,     0,     0,   133,     0,     0,
       0,     3,   140,    39,    40,     0,     0,   136,     0,     0,
       0,     0,     0,   123,     0,   126,     0,   302,   303,   305,
       0,     0,   169,     0,     0,     0,   204,    86,    87,     0,
     330,   223,   314,   315,   316,   326,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     248,   249,   242,   243,     0,   246,   247,   250,   251,   252,
     253,   254,   256,   257,   259,   261,   262,   264,   266,   268,
     269,   274,   271,   272,   273,   300,   297,     0,    45,     0,
      79,    80,    81,    82,    83,    42,     0,     0,   283,   320,
     321,   322,    47,     0,   292,     0,   281,     0,     0,     0,
     237,    31,     0,   308,     0,     0,     0,   311,   325,     0,
       0,     0,     0,    21,     0,    22,     0,    23,     0,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   330,     0,     0,   111,   330,
       0,     0,   261,   262,   214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,   330,   236,
      95,     0,     0,   169,     0,     0,     0,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   224,    49,    51,    53,    56,    54,    55,    50,    52,
      62,    64,    66,    69,    67,    68,    63,    65,     0,     0,
       0,   293,     0,     0,     0,   150,     0,     0,     0,     0,
     291,     0,   325,   287,     0,     0,   320,   321,   322,    30,
       5,     0,     0,     9,     0,     0,     0,     0,    14,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      70,    73,    72,    76,    74,    75,    71,    34,     0,    77,
      78,    37,   105,   110,   109,     0,     0,   119,   130,     0,
     132,     0,     0,   215,     0,     0,   139,     0,     0,     0,
       0,     0,   151,     0,    26,    25,     0,     0,   134,   323,
     324,     0,     0,   121,   124,   125,   167,   168,     0,     0,
       0,     0,   165,   148,     0,     0,     0,     0,     0,     0,
       0,   203,   332,     0,     0,     0,     0,   225,   226,   233,
     234,   240,   299,     0,   296,     0,     0,    44,     0,    46,
     149,     0,     0,     0,   290,     0,     0,   285,   286,     0,
       0,     0,   211,   210,     0,     0,     0,    16,     0,    17,
       0,    20,   142,     0,    28,    27,   332,   332,     0,   112,
       0,   113,     0,     0,     0,   216,   217,   218,     0,   138,
       0,     0,   154,     0,   152,     0,     0,    60,    61,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     145,     0,     0,     0,     0,     0,     0,     0,   320,   322,
     230,   298,   295,   294,    43,    57,    58,    59,   208,     0,
       0,    11,    10,    15,     0,     0,     0,     0,   141,    35,
      38,   107,   106,   330,     0,   117,   131,   128,   129,   219,
       0,     0,   155,   156,   153,   137,   135,   122,     0,     3,
     196,     0,   199,     0,     0,     0,     0,   175,     0,     0,
       0,     0,   170,     0,     0,     0,     0,   171,   172,     0,
     144,   146,   205,   206,   207,     0,   150,     0,     0,     0,
       0,   332,   213,   212,     0,    18,   114,   115,   220,   221,
       0,     0,     0,     0,     0,     0,     0,     0,   186,     0,
       0,     0,   187,     0,     0,     0,     0,     0,   177,     0,
       0,     0,     0,   176,   173,   174,     0,   202,   229,   228,
     149,     0,     0,   209,    12,   332,     0,   118,   195,     0,
     158,     0,     0,     0,   157,     0,   198,     0,   184,   185,
     179,   182,   183,   178,     0,     0,   188,     0,     0,     0,
     189,     0,     0,     0,   227,   231,   232,    19,   116,   194,
       0,     0,   161,     0,   159,   197,   192,   193,   181,   190,
     191,   180,   201,     0,   162,   163,   160,   200
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -649,  -649,     1,  -649,  -418,  -537,  -649,  -649,  -649,  -649,
    -649,  -649,  -649,  -649,  -649,  -250,  -648,  -649,  -649,  -649,
     530,  -649,  -649,   502,   596,  -301,   101,   695,    -2,  -188,
    -649,   470,  -649,   338,    -9,   793,   794,   535,    -8,  -649,
    -649,   -58,  -649
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   167,    56,    57,   428,    58,    59,    60,    61,
      62,    63,   441,    64,    65,   282,   720,    66,    67,   301,
     454,    68,   459,   504,   400,   437,   106,   147,   112,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   113,    83
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      70,     2,   456,   409,   344,   639,   434,   107,   543,   434,
     435,   130,   136,   435,   722,   434,   137,   129,   451,   435,
     121,   123,   125,   127,   128,   304,   452,   133,   135,   275,
     138,   434,    24,   142,   144,   435,   224,   551,   151,   549,
     267,   154,   233,   162,   163,   434,   434,   553,   267,   435,
     435,   434,   165,   241,     3,   435,   661,   268,    96,   588,
     589,   590,   579,   580,   516,   268,   581,    95,   203,   276,
     305,   579,   580,    98,    55,   581,    97,   267,   709,   243,
     566,   567,   139,   235,    55,   216,   217,   218,   219,   220,
     221,   787,   108,   225,   268,   227,   228,   662,   267,   554,
     555,   236,    55,    69,   705,    99,   552,   526,   100,   240,
     436,   591,   798,   455,    55,   268,   806,   146,   256,   525,
     266,   453,   271,    55,   273,   269,   270,   582,   131,   132,
     710,   711,   598,   269,   270,   585,   660,   203,   145,   292,
     713,   294,   152,   153,   296,   297,   298,   296,   299,   760,
     764,   829,   723,   615,   254,   822,   586,   587,   255,   835,
     724,   568,   269,   270,   140,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   842,   330,   331,   332,   333,   334,
     335,   336,   337,   269,   270,   288,   556,   155,   201,   202,
     845,   290,   355,   356,    99,   847,   157,   100,   725,   361,
     362,   363,   364,   289,    96,   583,   131,   420,   728,   291,
     244,   229,   424,   158,   230,   160,   729,   238,   431,   797,
     701,   203,   464,   280,   245,   156,   272,   281,   274,   277,
     278,   279,   387,    94,   389,   726,   390,   376,   159,   774,
     779,   393,   394,   293,   396,   295,   401,   775,   780,   727,
     161,   461,   231,   377,   730,   413,   414,   415,   416,   417,
     418,   419,   512,   203,   676,   422,   423,   246,    69,   432,
     433,   309,   283,   310,   248,   632,   402,   513,   514,   599,
     677,   247,   438,   439,   440,   776,   781,   443,   249,   445,
     403,   731,   625,   257,   166,   404,   258,   259,   406,    99,
     601,   626,   100,   260,   204,   732,   261,   262,   649,   405,
     263,   493,   407,   284,   466,   467,   468,   470,   366,   496,
     600,   367,   777,   782,   200,   501,   382,   505,   114,   115,
     116,   207,   117,   680,   669,   670,   778,   783,   177,   284,
     609,   178,   179,   208,   209,   210,   714,   788,   180,   118,
     685,   181,   182,   119,   529,   183,   184,   825,   177,   687,
     284,   178,   179,   538,   211,   408,   826,   846,   180,   281,
     212,   181,   182,   434,   494,   183,   184,   435,   264,   265,
     368,   733,   369,   698,   734,   597,   735,   736,   186,   187,
     188,   189,   213,   511,   190,   191,   192,   165,   222,   647,
     521,   450,   523,   648,   524,   205,   206,   663,   527,   528,
     664,   665,   666,   667,   193,   194,   214,   761,   242,   243,
     765,   740,   741,   185,   177,   546,    94,   178,   179,   718,
     471,   603,   606,   719,   180,   721,   563,   181,   182,   719,
     786,   183,   184,   185,   719,   250,   251,   252,   234,   243,
     654,   378,   195,   196,   215,   257,   621,   226,   258,   259,
     197,   594,   595,   596,   243,   260,   386,   237,   261,   262,
     257,   253,   263,   258,   259,   283,   198,   199,   285,   243,
     260,   392,   286,   261,   262,   823,   287,   263,   168,   198,
     199,   169,   170,   426,   427,   611,   612,   613,   171,   638,
     427,   172,   173,   518,   519,   174,   175,   284,   257,   185,
     620,   258,   259,   622,   623,   412,   834,   310,   260,   300,
     628,   261,   262,   537,   243,   263,   542,   544,   541,   243,
     547,   548,   550,   349,   350,   351,   352,   353,   354,    69,
     264,   265,   421,   640,   310,   629,   630,   573,   574,   575,
     309,   309,   425,   429,   302,   264,   265,   650,   651,   653,
     303,   690,   712,   309,   655,   446,   306,     4,     5,     6,
       7,   307,   697,   176,   109,   102,   103,   104,    12,    13,
      14,    15,    16,    17,    18,   309,   557,   448,   558,   557,
     243,   584,   592,   264,   265,   105,    28,   243,   308,   608,
     844,   243,   311,   610,   177,    33,   365,   178,   179,   370,
      38,    39,    40,   688,   180,    43,   371,   181,   182,   747,
     372,   183,   184,   373,    84,    47,    85,    86,    87,    88,
      89,    90,    91,   243,   616,   614,   617,    92,   754,   243,
     619,   618,   310,   519,   557,   624,   633,   641,   380,   243,
      93,   636,   374,    94,   692,   243,   338,   339,   340,   341,
     342,   343,    51,   345,   346,   347,   348,   375,   656,    52,
     243,   557,   637,   671,   745,   134,   379,    54,   557,   381,
     672,    55,   469,   243,   616,   681,   682,   383,   243,   185,
     683,   382,   752,   753,   384,   696,    84,   310,    85,    86,
      87,    88,    89,    90,    91,   309,   243,   703,   751,    92,
     243,   243,   790,   795,   557,   557,   799,   807,   385,   557,
     763,   843,   462,   388,   769,    94,   704,   427,   391,   773,
     755,   243,   796,   427,   148,   150,   410,   411,   430,   702,
     359,   360,   447,   706,   707,   708,   791,   792,   793,   442,
     444,   449,   463,   465,   457,   458,   715,   472,   473,   474,
     717,   805,   475,   476,   477,   478,   479,   488,   480,   481,
     482,   483,   484,   485,   486,   817,   487,   203,   491,   489,
     821,   490,   507,   497,   498,   499,   506,   508,   509,   510,
     515,   570,   520,   281,   530,   531,   569,   571,   576,   830,
     831,   833,   532,   533,   756,   757,   577,     4,     5,     6,
       7,   534,   535,   536,   312,   313,   314,   315,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   316,   317,
      24,   539,   572,    25,    26,    27,    28,   318,   319,    31,
      32,   540,   564,   565,   593,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,   607,   609,   627,
      45,    46,   631,   634,    69,    47,   635,   642,   643,    48,
     644,   645,   646,   657,   658,   659,   668,   673,   674,   675,
     684,   678,   685,   686,   679,   687,   689,   691,   693,   694,
      49,    50,   699,   695,   700,   739,   716,   828,   737,   738,
     742,   743,    51,   744,   748,   749,   750,   758,   759,    52,
     762,   768,   772,   766,   767,    53,   770,    54,   771,   719,
     784,    55,   320,     4,     5,     6,     7,   785,   789,   794,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   559,   560,    24,   800,   814,    25,
      26,    27,    28,    29,   561,    31,    32,   815,   808,   818,
     809,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,   819,   824,     0,    45,    46,   810,   811,
     812,    47,   813,   578,   816,    48,   820,   827,   836,   837,
     838,   839,   605,   840,   841,   517,   357,     0,   358,     0,
       0,     0,     0,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,    53,     0,    54,     0,     0,     0,    55,   562,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     801,   802,    24,     0,     0,    25,    26,    27,    28,    29,
     803,    31,    32,     0,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,    53,     0,    54,
       0,     0,     0,    55,   804,     4,     5,     6,     7,     0,
       0,     0,   109,   102,   103,   104,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,    28,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,     0,     0,     0,    38,    39,
      40,     0,     0,    43,     0,     0,     0,     0,     4,     5,
       6,     7,     0,    47,     0,   109,   102,   103,   104,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,    28,     0,     0,
       0,     0,     0,     0,     0,     0,    33,     0,     0,     0,
      51,    38,    39,    40,     0,     0,    43,    52,     0,     0,
       0,     0,     0,   134,     0,    54,    47,     0,     0,    55,
     652,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,   134,     0,    54,     0,
       0,     0,    55,   832,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     0,
       0,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,     0,     0,     0,    45,    46,     0,
       0,     0,    47,     0,     0,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,    53,     0,    54,     0,     0,     0,    55,     4,
       5,     6,     7,     0,     0,     0,   109,   102,   103,   104,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,    28,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,    38,    39,    40,     0,     0,    43,     0,     0,
       0,     0,     4,     5,     6,     7,     0,    47,     0,   109,
     102,   103,   104,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,    28,     0,     0,     0,     0,     0,     0,     0,     0,
      33,     0,     0,     0,    51,    38,    39,    40,     0,     0,
      43,    52,     0,     0,     0,     0,     0,   110,     0,   111,
      47,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   109,   102,   103,
     104,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,   105,    28,
       0,     0,     0,     0,    52,     0,     0,     0,    33,     0,
     134,     0,    54,    38,    39,    40,    55,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
       4,     5,     6,     7,     0,     0,     0,   109,   102,   103,
     104,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   164,     0,     0,    55,     0,     0,     0,    47,     0,
       4,     5,     6,     7,     0,     0,     0,   109,   102,   103,
     104,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   223,     0,     0,    55,     0,     0,     0,    47,     0,
       4,     5,     6,     7,     0,     0,     0,   109,   102,   103,
     104,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   232,     0,     0,    55,     0,     0,     0,    47,     0,
       4,     5,     6,     7,     0,     0,     0,   109,   102,   103,
     104,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,    28,
       0,     0,     0,     0,     0,    51,     0,     0,    33,     0,
       0,     0,    52,    38,    39,    40,     0,     0,    43,     0,
      54,   239,     0,     0,    55,     0,     0,     0,    47,     0,
       0,   166,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,    51,     0,     0,     0,     0,
       0,    33,    52,     0,     0,     0,    38,    39,    40,     0,
      54,    43,     0,     0,    55,     0,     0,     0,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   460,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   492,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   495,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,   502,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   500,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,   397,     7,     0,     0,     0,
     109,   102,   103,   104,   398,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   503,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   164,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,   502,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   602,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   604,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,   746,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   120,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   122,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   124,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   126,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   141,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   143,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,    54,     0,     0,     0,   149,     0,     0,
       0,    47,     0,     4,     5,   397,     7,     0,     0,     0,
     109,   102,   103,   104,   398,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   395,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   399,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     109,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    28,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   522,     0,     0,     0,    55,     0,     0,
       0,    47,     0,     4,     5,     6,     7,     0,     0,     0,
     101,   102,   103,   104,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,  -333,  -333,
       0,   105,     0,     0,     0,     0,     0,     0,    51,     0,
       0,    33,     0,     0,     0,    52,    38,    39,    40,     0,
       0,    43,     0,   545,     0,     0,     0,    55,     0,  -333,
    -333,    47,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,     0,     0,     0,  -333,     0,     0,     0,     0,     0,
       0,     0,  -333,  -333,  -333,     0,     0,     0,  -333,  -333,
    -333,  -333,  -333,  -333,     0,  -333,  -333,  -333,    51,  -333,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,    55
};

static const yytype_int16 yycheck[] =
{
       2,     0,   303,   253,   192,   542,    10,    15,   426,    10,
      14,    14,    10,    14,   662,    10,    14,    26,    14,    14,
      22,    23,    24,    25,    26,    14,    22,    29,    30,    72,
      32,    10,    26,    35,    36,    14,    94,    72,    40,    72,
      26,    43,   100,    51,    52,    10,    10,    10,    26,    14,
      14,    10,    54,   111,     0,    14,    14,    43,    99,    66,
      67,    68,    53,    54,    69,    43,    57,   115,   103,   112,
      59,    53,    54,    97,   117,    57,   117,    26,    10,   112,
      37,    38,    12,    99,   117,    87,    88,    89,    90,    91,
      92,   739,    14,    95,    43,    97,    98,    55,    26,    62,
      63,   117,   117,     2,   641,   110,    72,   408,   113,   111,
     114,   118,   760,   114,   117,    43,   764,   117,   120,   114,
     122,   117,   124,   117,   126,   111,   112,   118,    27,    28,
      62,    63,   118,   111,   112,   114,   118,   103,    37,   141,
     118,   143,    41,    42,   146,   147,   148,   149,   150,   114,
     114,   799,    14,    69,    10,   114,   457,   458,    14,   807,
      22,   118,   111,   112,    12,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   822,   177,   178,   179,   180,   181,
     182,   183,   184,   111,   112,    99,   436,    14,    70,    71,
     118,    99,   201,   202,   110,   843,    14,   113,    60,   207,
     208,   209,   210,   117,    99,   455,   105,   265,    14,   117,
      97,    14,   270,    31,    17,    10,    22,    97,   276,   756,
     638,   103,   117,   113,   111,    14,   125,   117,   127,   128,
      70,    71,   234,   113,   236,    97,   238,    97,    14,    14,
      14,   243,   244,   142,   246,   144,   248,    22,    22,   111,
      10,   309,    55,   113,    60,   257,   258,   259,   260,   261,
     262,   263,    96,   103,    97,   267,   268,    97,   167,   278,
     279,   112,   111,   114,    97,   525,    97,   111,   112,   118,
     113,   111,   281,   285,   286,    60,    60,   289,   111,   291,
     111,    97,     5,    26,    64,    97,    29,    30,    97,   110,
     488,    14,   113,    36,    95,   111,    39,    40,   558,   111,
      43,   369,   111,   111,   316,   317,   318,   319,    14,   377,
     118,    17,    97,    97,    93,   383,   111,   385,    10,    11,
      12,    94,    14,   118,   584,   585,   111,   111,    26,   111,
     111,    29,    30,   106,   107,   108,   118,   118,    36,    31,
     111,    39,    40,    35,   412,    43,    44,   118,    26,   111,
     111,    29,    30,   421,   110,   113,   118,   118,    36,   117,
     110,    39,    40,    10,   376,    43,    44,    14,   111,   112,
     111,    10,   113,   633,    13,   118,    15,    16,    73,    74,
      75,    76,    14,   395,    79,    80,    81,   399,    14,    10,
     402,   300,   404,    14,   406,   104,   105,    10,   410,   411,
      13,    14,    15,    16,    99,   100,   111,   718,   111,   112,
     721,   671,   672,   111,    26,   427,   113,    29,    30,   113,
     118,   489,   490,   117,    36,   113,   438,    39,    40,   117,
     113,    43,    44,   111,   117,    10,    11,    12,   115,   112,
     118,   114,    77,    78,   111,    26,   514,    14,    29,    30,
      85,   463,   464,   465,   112,    36,   114,   111,    39,    40,
      26,    14,    43,    29,    30,   111,   101,   102,    97,   112,
      36,   114,    97,    39,    40,   786,   117,    43,    26,   101,
     102,    29,    30,    27,    28,   497,   498,   499,    36,    27,
      28,    39,    40,   111,   112,    43,    44,   111,    26,   111,
     512,    29,    30,   515,   516,   112,   118,   114,    36,    50,
     522,    39,    40,   111,   112,    43,   425,   426,   111,   112,
     429,   430,   431,     7,     8,     9,   198,   199,   200,   438,
     111,   112,   112,   545,   114,   111,   112,   446,   447,   448,
     112,   112,   114,   114,   117,   111,   112,   559,   560,   561,
     113,   619,   118,   112,   566,   114,   117,     3,     4,     5,
       6,   111,   630,   111,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   112,   112,   114,   114,   112,
     112,   114,   114,   111,   112,    31,    32,   112,   111,   114,
     118,   112,   118,   114,    26,    41,    14,    29,    30,   111,
      46,    47,    48,   615,    36,    51,   111,    39,    40,   677,
     111,    43,    44,   111,    84,    61,    86,    87,    88,    89,
      90,    91,    92,   112,   112,   114,   114,    97,   696,   112,
     112,   114,   114,   112,   112,   114,   114,   546,   100,   112,
     110,   114,   111,   113,   111,   112,   186,   187,   188,   189,
     190,   191,    98,   193,   194,   195,   196,   111,   567,   105,
     112,   112,   114,   114,   676,   111,   116,   113,   112,   118,
     114,   117,   118,   112,   112,   114,   114,   113,   112,   111,
     114,   111,   694,   695,    14,   112,    84,   114,    86,    87,
      88,    89,    90,    91,    92,   112,   112,   114,   114,    97,
     112,   112,   114,   114,   112,   112,   114,   114,   113,   112,
     719,   114,   110,    14,   726,   113,    27,    28,   111,   731,
     111,   112,    27,    28,    39,    40,    97,    97,    10,   638,
     205,   206,    27,   642,   643,   644,   748,   749,   750,    14,
      14,   118,   115,    97,   113,   113,   655,   111,   111,   111,
     659,   763,   111,   111,   111,   111,   111,    82,   111,   111,
     111,   111,   111,   111,   111,   777,   111,   103,    14,   113,
     782,   113,   100,    97,    97,    97,   116,   118,   111,   111,
      69,   118,   111,   117,   111,   111,   100,   100,    14,   801,
     802,   803,   111,   111,   703,   704,    14,     3,     4,     5,
       6,   111,   111,   111,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   111,   118,    29,    30,    31,    32,    33,    34,    35,
      36,   111,   111,   111,    14,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,   111,   111,   111,
      56,    57,   111,   111,   763,    61,   111,    10,    10,    65,
      10,    10,    10,   111,   111,    27,    53,    14,    14,    14,
     111,   116,   111,   111,   118,   111,    14,   111,   111,    69,
      86,    87,   111,    69,   111,    14,   118,   796,   111,   111,
       7,     7,    98,     7,    97,    97,    69,    10,    10,   105,
      64,   111,   111,    14,    14,   111,    14,   113,    14,   117,
     111,   117,   118,     3,     4,     5,     6,   111,   118,   111,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   118,    14,    29,
      30,    31,    32,    33,    34,    35,    36,    14,   111,    14,
     111,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    14,   118,    -1,    56,    57,   111,   111,
     111,    61,   111,   453,   111,    65,   111,   111,   111,   111,
     111,   111,   490,   111,   111,   399,   203,    -1,   204,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,    -1,   117,   118,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    -1,
      -1,    -1,    56,    57,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,    -1,   117,   118,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    -1,    -1,    51,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    61,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      98,    46,    47,    48,    -1,    -1,    51,   105,    -1,    -1,
      -1,    -1,    -1,   111,    -1,   113,    61,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
      -1,    -1,   117,   118,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    -1,    -1,    -1,    56,    57,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,    -1,   117,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    -1,    -1,    51,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    61,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    98,    46,    47,    48,    -1,    -1,
      51,   105,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
      61,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    31,    32,
      -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    41,    -1,
     111,    -1,   113,    46,    47,    48,   117,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    41,    -1,
      -1,    -1,   105,    46,    47,    48,    -1,    -1,    51,    -1,
     113,   114,    -1,    -1,   117,    -1,    -1,    -1,    61,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    41,    -1,
      -1,    -1,   105,    46,    47,    48,    -1,    -1,    51,    -1,
     113,   114,    -1,    -1,   117,    -1,    -1,    -1,    61,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    41,    -1,
      -1,    -1,   105,    46,    47,    48,    -1,    -1,    51,    -1,
     113,   114,    -1,    -1,   117,    -1,    -1,    -1,    61,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    41,    -1,
      -1,    -1,   105,    46,    47,    48,    -1,    -1,    51,    -1,
     113,   114,    -1,    -1,   117,    -1,    -1,    -1,    61,    -1,
      -1,    64,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    41,   105,    -1,    -1,    -1,    46,    47,    48,    -1,
     113,    51,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,   114,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    61,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    41,    -1,    -1,    -1,   105,    46,    47,    48,    -1,
      -1,    51,    -1,   113,    -1,    -1,    -1,   117,    -1,    70,
      71,    61,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    -1,    -1,    -1,    99,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,    98,   110,
      -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    -1,   117
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   120,   121,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    56,    57,    61,    65,    86,
      87,    98,   105,   111,   113,   117,   122,   123,   125,   126,
     127,   128,   129,   130,   132,   133,   136,   137,   140,   145,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   161,    84,    86,    87,    88,    89,    90,
      91,    92,    97,   110,   113,   115,    99,   117,    97,   110,
     113,    10,    11,    12,    13,    31,   145,   157,    14,    10,
     111,   113,   147,   160,    10,    11,    12,    14,    31,    35,
     113,   147,   113,   147,   113,   147,   113,   147,   147,   153,
      14,   145,   145,   147,   111,   147,    10,    14,   147,    12,
      12,   113,   147,   113,   147,   145,   117,   146,   146,   117,
     146,   147,   145,   145,   147,    14,    14,    14,    31,    14,
      10,    10,   157,   157,   114,   147,    64,   121,    26,    29,
      30,    36,    39,    40,    43,    44,   111,    26,    29,    30,
      36,    39,    40,    43,    44,   111,    73,    74,    75,    76,
      79,    80,    81,    99,   100,    77,    78,    85,   101,   102,
      93,    70,    71,   103,    95,   104,   105,    94,   106,   107,
     108,   110,   110,    14,   111,   111,   147,   147,   147,   147,
     147,   147,    14,   114,   160,   147,    14,   147,   147,    14,
      17,    55,   114,   160,   115,    99,   117,   111,    97,   114,
     147,   160,   111,   112,    97,   111,    97,   111,    97,   111,
      10,    11,    12,    14,    10,    14,   147,    26,    29,    30,
      36,    39,    40,    43,   111,   112,   147,    26,    43,   111,
     112,   147,   145,   147,   145,    72,   112,   145,    70,    71,
     113,   117,   134,   111,   111,    97,    97,   117,    99,   117,
      99,   117,   147,   145,   147,   145,   147,   147,   147,   147,
      50,   138,   117,   113,    14,    59,   117,   111,   111,   112,
     114,   118,    10,    11,    12,    13,    24,    25,    33,    34,
     118,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   150,   150,
     150,   150,   150,   150,   148,   150,   150,   150,   150,     7,
       8,     9,   152,   152,   152,   153,   153,   154,   155,   156,
     156,   157,   157,   157,   157,    14,    14,    17,   111,   113,
     111,   111,   111,   111,   111,   111,    97,   113,   114,   116,
     100,   118,   111,   113,    14,   113,   114,   147,    14,   147,
     147,   111,   114,   147,   147,   113,   147,     5,    14,   113,
     143,   147,    97,   111,    97,   111,    97,   111,   113,   134,
      97,    97,   112,   147,   147,   147,   147,   147,   147,   147,
     160,   112,   147,   147,   160,   114,    27,    28,   124,   114,
      10,   160,   153,   153,    10,    14,   114,   144,   121,   147,
     147,   131,    14,   147,    14,   147,   114,    27,   114,   118,
     145,    14,    22,   117,   139,   114,   144,   113,   113,   141,
     114,   160,   110,   115,   117,    97,   147,   147,   147,   118,
     147,   118,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,    82,   113,
     113,    14,   114,   160,   147,   114,   160,    97,    97,    97,
     114,   160,    14,   114,   142,   160,   116,   100,   118,   111,
     111,   147,    96,   111,   112,    69,    69,   143,   111,   112,
     111,   147,   113,   147,   147,   114,   144,   147,   147,   160,
     111,   111,   111,   111,   111,   111,   111,   111,   160,   111,
     111,   111,   145,   123,   145,   113,   147,   145,   145,    72,
     145,    72,    72,    10,    62,    63,   134,   112,   114,    24,
      25,    34,   118,   147,   111,   111,    37,    38,   118,   100,
     118,   100,   118,   145,   145,   145,    14,    14,   139,    53,
      54,    57,   118,   134,   114,   114,   144,   144,    66,    67,
      68,   118,   114,    14,   147,   147,   147,   118,   118,   118,
     118,   148,   114,   160,   114,   142,   160,   111,   114,   111,
     114,   147,   147,   147,   114,    69,   112,   114,   114,   112,
     147,   160,   147,   147,   114,     5,    14,   111,   147,   111,
     112,   111,   134,   114,   111,   111,   114,   114,    27,   124,
     147,   145,    10,    10,    10,    10,    10,    10,    14,   134,
     147,   147,   118,   147,   118,   147,   145,   111,   111,    27,
     118,    14,    55,    10,    13,    14,    15,    16,    53,   134,
     134,   114,   114,    14,    14,    14,    97,   113,   116,   118,
     118,   114,   114,   114,   111,   111,   111,   111,   147,    14,
     160,   111,   111,   111,    69,    69,   112,   160,   134,   111,
     111,   123,   145,   114,    27,   124,   145,   145,   145,    10,
      62,    63,   118,   118,   118,   145,   118,   145,   113,   117,
     135,   113,   135,    14,    22,    60,    97,   111,    14,    22,
      60,    97,   111,    10,    13,    15,    16,   111,   111,    14,
     134,   134,     7,     7,     7,   147,   114,   160,    97,    97,
      69,   114,   147,   147,   160,   111,   145,   145,    10,    10,
     114,   144,    64,   121,   114,   144,    14,    14,   111,   147,
      14,    14,   111,   147,    14,    22,    60,    97,   111,    14,
      22,    60,    97,   111,   111,   111,   113,   135,   118,   118,
     114,   147,   147,   147,   111,   114,    27,   124,   135,   114,
     118,    24,    25,    34,   118,   147,   135,   114,   111,   111,
     111,   111,   111,   111,    14,    14,   111,   147,    14,    14,
     111,   147,   114,   144,   118,   118,   118,   111,   145,   135,
     147,   147,   118,   147,   118,   135,   111,   111,   111,   111,
     111,   111,   135,   114,   118,   118,   118,   135
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   119,   120,   121,   121,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   123,   123,   123,   123,   123,
     123,   123,   123,   124,   124,   124,   124,   124,   124,   125,
     125,   126,   126,   126,   126,   127,   127,   128,   129,   129,
     129,   129,   129,   129,   130,   130,   131,   131,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   133,
     133,   134,   134,   134,   134,   134,   134,   135,   135,   135,
     135,   135,   135,   135,   136,   137,   138,   138,   138,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   140,   141,   141,   141,   141,   142,   142,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   146,   147,   147,   147,
     148,   148,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   150,   150,   150,   151,
     151,   152,   152,   152,   153,   153,   154,   154,   155,   155,
     155,   156,   156,   156,   156,   156,   157,   157,   157,   157,
     158,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161
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
       5,     5,     3,     3,     4,     5,     5,     9,     8,     8,
       6,     9,     9,     5,     5,     4,     3,     3,     2,     1,
       5,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     2,     2,     2,     1,
       1,     4,     3,     4,     3,     6,     6,     5,     3,     3,
       6,     5,     4,     4,     6,     6,     5,     3,     6,     5,
       3,     2,     3,     3,     2,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     5,     5,     1,     1,     1,     1,     2,
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
#line 312 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2488 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 322 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2494 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 323 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2500 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 327 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2506 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 329 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2512 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 331 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2518 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 333 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2524 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 335 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2530 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 337 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2538 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 341 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2545 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 344 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2553 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 348 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2559 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 350 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2565 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 352 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2571 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2577 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 356 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2583 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 358 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2591 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 362 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2599 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 366 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2605 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 368 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2611 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 370 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2617 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 372 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2623 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 374 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2629 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 376 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2635 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 378 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2641 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 380 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2647 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 382 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2653 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 384 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2659 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 386 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2665 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 388 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2671 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 390 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2677 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 392 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2683 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 394 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2690 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 397 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2697 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 400 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2703 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2710 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2717 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 408 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2723 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 410 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2729 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 412 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2735 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 414 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2741 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 416 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2750 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 421 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2758 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 425 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2766 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 429 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2775 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 434 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2783 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 437 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2789 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 439 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2795 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 441 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2801 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 443 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2807 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 445 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2813 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 447 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2820 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 450 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2826 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 452 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2832 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 454 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2838 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 456 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2845 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2852 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 462 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2859 "raku.tab.c"
    break;

  case 60: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 465 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2866 "raku.tab.c"
    break;

  case 61: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 468 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2873 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_IF expr ';'  */
#line 471 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2879 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_UNLESS expr ';'  */
#line 473 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2885 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_WHILE expr ';'  */
#line 475 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2891 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNTIL expr ';'  */
#line 477 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2897 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_FOR expr ';'  */
#line 479 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2904 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_WITH expr ';'  */
#line 482 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 2910 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_WITHOUT expr ';'  */
#line 484 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 2916 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_GIVEN expr ';'  */
#line 486 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 2922 "raku.tab.c"
    break;

  case 70: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 488 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 2929 "raku.tab.c"
    break;

  case 71: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 491 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 2936 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 494 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 2944 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 498 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 2951 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 501 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 2957 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 503 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 2963 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 505 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 2969 "raku.tab.c"
    break;

  case 77: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 507 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 2976 "raku.tab.c"
    break;

  case 78: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 510 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 2983 "raku.tab.c"
    break;

  case 79: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 513 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 2990 "raku.tab.c"
    break;

  case 80: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 516 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 2997 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 519 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3004 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 522 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3011 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 525 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3018 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 528 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3025 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 531 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3032 "raku.tab.c"
    break;

  case 86: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 534 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3039 "raku.tab.c"
    break;

  case 87: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 537 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3046 "raku.tab.c"
    break;

  case 88: /* stmt: expr ';'  */
#line 539 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3052 "raku.tab.c"
    break;

  case 89: /* stmt: ';'  */
#line 540 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3058 "raku.tab.c"
    break;

  case 90: /* stmt: if_stmt  */
#line 541 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3064 "raku.tab.c"
    break;

  case 91: /* stmt: while_stmt  */
#line 542 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3070 "raku.tab.c"
    break;

  case 92: /* stmt: for_stmt  */
#line 543 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3076 "raku.tab.c"
    break;

  case 93: /* stmt: given_stmt  */
#line 544 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3082 "raku.tab.c"
    break;

  case 94: /* stmt: KW_TRY block  */
#line 546 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3088 "raku.tab.c"
    break;

  case 95: /* stmt: KW_TRY block KW_CATCH block  */
#line 548 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3094 "raku.tab.c"
    break;

  case 96: /* stmt: KW_CATCH block  */
#line 550 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3100 "raku.tab.c"
    break;

  case 97: /* stmt: block  */
#line 552 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3106 "raku.tab.c"
    break;

  case 98: /* stmt: unless_stmt  */
#line 553 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3112 "raku.tab.c"
    break;

  case 99: /* stmt: until_stmt  */
#line 554 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3118 "raku.tab.c"
    break;

  case 100: /* stmt: repeat_stmt  */
#line 555 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3124 "raku.tab.c"
    break;

  case 101: /* stmt: sub_decl  */
#line 556 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3130 "raku.tab.c"
    break;

  case 102: /* stmt: class_decl  */
#line 557 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3136 "raku.tab.c"
    break;

  case 103: /* stmt: role_decl  */
#line 558 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3142 "raku.tab.c"
    break;

  case 104: /* stmt: grammar_decl  */
#line 559 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3148 "raku.tab.c"
    break;

  case 105: /* if_stmt: KW_IF '(' expr ')' block  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3154 "raku.tab.c"
    break;

  case 106: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 565 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3160 "raku.tab.c"
    break;

  case 107: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3166 "raku.tab.c"
    break;

  case 108: /* if_stmt: KW_IF expr block  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3172 "raku.tab.c"
    break;

  case 109: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 571 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3178 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 573 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3184 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF expr block elsif_tail  */
#line 575 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3190 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3196 "raku.tab.c"
    break;

  case 113: /* elsif_tail: KW_ELSIF expr block  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3202 "raku.tab.c"
    break;

  case 114: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3208 "raku.tab.c"
    break;

  case 115: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3214 "raku.tab.c"
    break;

  case 116: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 587 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3220 "raku.tab.c"
    break;

  case 117: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3226 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 591 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3232 "raku.tab.c"
    break;

  case 119: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 595 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3238 "raku.tab.c"
    break;

  case 120: /* while_stmt: KW_WHILE expr block  */
#line 597 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3244 "raku.tab.c"
    break;

  case 121: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 601 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3250 "raku.tab.c"
    break;

  case 122: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 603 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3256 "raku.tab.c"
    break;

  case 123: /* unless_stmt: KW_UNLESS expr block  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3262 "raku.tab.c"
    break;

  case 124: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 607 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3268 "raku.tab.c"
    break;

  case 125: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 611 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3274 "raku.tab.c"
    break;

  case 126: /* until_stmt: KW_UNTIL expr block  */
#line 613 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3280 "raku.tab.c"
    break;

  case 127: /* repeat_stmt: KW_REPEAT block  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3286 "raku.tab.c"
    break;

  case 128: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 621 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3296 "raku.tab.c"
    break;

  case 129: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 627 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3306 "raku.tab.c"
    break;

  case 130: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 633 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3315 "raku.tab.c"
    break;

  case 131: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 638 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3325 "raku.tab.c"
    break;

  case 132: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 644 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3334 "raku.tab.c"
    break;

  case 133: /* for_stmt: KW_FOR expr block  */
#line 649 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3341 "raku.tab.c"
    break;

  case 134: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 654 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3353 "raku.tab.c"
    break;

  case 135: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 662 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3366 "raku.tab.c"
    break;

  case 136: /* when_list: %empty  */
#line 672 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3372 "raku.tab.c"
    break;

  case 137: /* when_list: when_list KW_WHEN expr block  */
#line 674 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3380 "raku.tab.c"
    break;

  case 138: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 680 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3392 "raku.tab.c"
    break;

  case 139: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 688 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3402 "raku.tab.c"
    break;

  case 140: /* sub_decl: KW_SUB IDENT sub_body  */
#line 694 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3412 "raku.tab.c"
    break;

  case 141: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 700 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3424 "raku.tab.c"
    break;

  case 142: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 708 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3434 "raku.tab.c"
    break;

  case 143: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 714 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 144: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 720 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3457 "raku.tab.c"
    break;

  case 145: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 729 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3468 "raku.tab.c"
    break;

  case 146: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 736 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3481 "raku.tab.c"
    break;

  case 147: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 745 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3494 "raku.tab.c"
    break;

  case 148: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 754 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3505 "raku.tab.c"
    break;

  case 149: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 763 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3516 "raku.tab.c"
    break;

  case 150: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 770 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3525 "raku.tab.c"
    break;

  case 151: /* sub_body: '{' stmt_list '}'  */
#line 776 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3531 "raku.tab.c"
    break;

  case 152: /* sub_body: '{' stmt_list expr '}'  */
#line 778 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3538 "raku.tab.c"
    break;

  case 153: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 781 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3545 "raku.tab.c"
    break;

  case 154: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 784 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3551 "raku.tab.c"
    break;

  case 155: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 786 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3557 "raku.tab.c"
    break;

  case 156: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 788 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3563 "raku.tab.c"
    break;

  case 157: /* method_body: '{' stmt_list '}'  */
#line 791 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3569 "raku.tab.c"
    break;

  case 158: /* method_body: '{' YADA '}'  */
#line 792 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3575 "raku.tab.c"
    break;

  case 159: /* method_body: '{' stmt_list expr '}'  */
#line 794 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3582 "raku.tab.c"
    break;

  case 160: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 797 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3589 "raku.tab.c"
    break;

  case 161: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 800 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3595 "raku.tab.c"
    break;

  case 162: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 802 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3601 "raku.tab.c"
    break;

  case 163: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 804 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3607 "raku.tab.c"
    break;

  case 164: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 808 "raku.y"
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
#line 3625 "raku.tab.c"
    break;

  case 165: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 824 "raku.y"
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
#line 3642 "raku.tab.c"
    break;

  case 166: /* is_clauses: %empty  */
#line 838 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3648 "raku.tab.c"
    break;

  case 167: /* is_clauses: is_clauses IDENT IDENT  */
#line 840 "raku.y"
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
#line 3664 "raku.tab.c"
    break;

  case 168: /* is_clauses: is_clauses TESTOP IDENT  */
#line 852 "raku.y"
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
#line 3680 "raku.tab.c"
    break;

  case 169: /* class_body_list: %empty  */
#line 865 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3686 "raku.tab.c"
    break;

  case 170: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 867 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3693 "raku.tab.c"
    break;

  case 171: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 870 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3700 "raku.tab.c"
    break;

  case 172: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 873 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3707 "raku.tab.c"
    break;

  case 173: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 876 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3714 "raku.tab.c"
    break;

  case 174: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 879 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3721 "raku.tab.c"
    break;

  case 175: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 882 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3728 "raku.tab.c"
    break;

  case 176: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 885 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3735 "raku.tab.c"
    break;

  case 177: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 888 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3742 "raku.tab.c"
    break;

  case 178: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 891 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3749 "raku.tab.c"
    break;

  case 179: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 894 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3756 "raku.tab.c"
    break;

  case 180: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 897 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3763 "raku.tab.c"
    break;

  case 181: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 900 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3770 "raku.tab.c"
    break;

  case 182: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 903 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3781 "raku.tab.c"
    break;

  case 183: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 910 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3792 "raku.tab.c"
    break;

  case 184: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 917 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3803 "raku.tab.c"
    break;

  case 185: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 924 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3814 "raku.tab.c"
    break;

  case 186: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 931 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3822 "raku.tab.c"
    break;

  case 187: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 935 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3830 "raku.tab.c"
    break;

  case 188: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 939 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3838 "raku.tab.c"
    break;

  case 189: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 943 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3846 "raku.tab.c"
    break;

  case 190: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 947 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3857 "raku.tab.c"
    break;

  case 191: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 954 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3868 "raku.tab.c"
    break;

  case 192: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 961 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3879 "raku.tab.c"
    break;

  case 193: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 968 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3890 "raku.tab.c"
    break;

  case 194: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 975 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3904 "raku.tab.c"
    break;

  case 195: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 985 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3916 "raku.tab.c"
    break;

  case 196: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 993 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3928 "raku.tab.c"
    break;

  case 197: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1001 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3941 "raku.tab.c"
    break;

  case 198: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1010 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3952 "raku.tab.c"
    break;

  case 199: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1017 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3963 "raku.tab.c"
    break;

  case 200: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 1024 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3977 "raku.tab.c"
    break;

  case 201: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 1034 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3989 "raku.tab.c"
    break;

  case 202: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 1042 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4001 "raku.tab.c"
    break;

  case 203: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1052 "raku.y"
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
#line 4018 "raku.tab.c"
    break;

  case 204: /* grammar_body_list: %empty  */
#line 1066 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4024 "raku.tab.c"
    break;

  case 205: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1068 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4033 "raku.tab.c"
    break;

  case 206: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1073 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4042 "raku.tab.c"
    break;

  case 207: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1078 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4051 "raku.tab.c"
    break;

  case 208: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1085 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4059 "raku.tab.c"
    break;

  case 209: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1089 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4067 "raku.tab.c"
    break;

  case 210: /* pair_list: IDENT OP_FATARROW expr  */
#line 1095 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4073 "raku.tab.c"
    break;

  case 211: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1097 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4079 "raku.tab.c"
    break;

  case 212: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1099 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4085 "raku.tab.c"
    break;

  case 213: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1101 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4091 "raku.tab.c"
    break;

  case 214: /* param_list: VAR_SCALAR  */
#line 1104 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4097 "raku.tab.c"
    break;

  case 215: /* param_list: IDENT VAR_SCALAR  */
#line 1105 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4103 "raku.tab.c"
    break;

  case 216: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1106 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4109 "raku.tab.c"
    break;

  case 217: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1107 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4115 "raku.tab.c"
    break;

  case 218: /* param_list: param_list ',' VAR_SCALAR  */
#line 1108 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4121 "raku.tab.c"
    break;

  case 219: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1109 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4127 "raku.tab.c"
    break;

  case 220: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1110 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4133 "raku.tab.c"
    break;

  case 221: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1111 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4139 "raku.tab.c"
    break;

  case 222: /* block: '{' stmt_list '}'  */
#line 1114 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4145 "raku.tab.c"
    break;

  case 223: /* block: '{' YADA '}'  */
#line 1115 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4151 "raku.tab.c"
    break;

  case 224: /* block: '{' stmt_list expr '}'  */
#line 1117 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4157 "raku.tab.c"
    break;

  case 225: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1119 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4163 "raku.tab.c"
    break;

  case 226: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1121 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4169 "raku.tab.c"
    break;

  case 227: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1123 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4177 "raku.tab.c"
    break;

  case 228: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1127 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4184 "raku.tab.c"
    break;

  case 229: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1130 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4191 "raku.tab.c"
    break;

  case 230: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1133 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4198 "raku.tab.c"
    break;

  case 231: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1136 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4205 "raku.tab.c"
    break;

  case 232: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1139 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4212 "raku.tab.c"
    break;

  case 233: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1142 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4218 "raku.tab.c"
    break;

  case 234: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1144 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4224 "raku.tab.c"
    break;

  case 235: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1146 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4230 "raku.tab.c"
    break;

  case 236: /* closure: '{' expr '}'  */
#line 1149 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4236 "raku.tab.c"
    break;

  case 237: /* expr: VAR_SCALAR '=' expr  */
#line 1152 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4242 "raku.tab.c"
    break;

  case 238: /* expr: KW_GATHER block  */
#line 1153 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4252 "raku.tab.c"
    break;

  case 239: /* expr: tern_expr  */
#line 1158 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4258 "raku.tab.c"
    break;

  case 240: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1162 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4264 "raku.tab.c"
    break;

  case 241: /* tern_expr: cmp_expr  */
#line 1163 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4270 "raku.tab.c"
    break;

  case 242: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1166 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4276 "raku.tab.c"
    break;

  case 243: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1167 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4282 "raku.tab.c"
    break;

  case 244: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1168 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4288 "raku.tab.c"
    break;

  case 245: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1169 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4294 "raku.tab.c"
    break;

  case 246: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1170 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4300 "raku.tab.c"
    break;

  case 247: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1171 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4306 "raku.tab.c"
    break;

  case 248: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1172 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4312 "raku.tab.c"
    break;

  case 249: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1173 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4318 "raku.tab.c"
    break;

  case 250: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1174 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4324 "raku.tab.c"
    break;

  case 251: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1175 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4330 "raku.tab.c"
    break;

  case 252: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1177 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4340 "raku.tab.c"
    break;

  case 253: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1183 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4350 "raku.tab.c"
    break;

  case 254: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1189 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4360 "raku.tab.c"
    break;

  case 255: /* cmp_expr: jct_expr  */
#line 1194 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4366 "raku.tab.c"
    break;

  case 256: /* jct_expr: jct_expr '|' range_expr  */
#line 1197 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4372 "raku.tab.c"
    break;

  case 257: /* jct_expr: jct_expr '&' range_expr  */
#line 1198 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4378 "raku.tab.c"
    break;

  case 258: /* jct_expr: dor_expr  */
#line 1199 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4384 "raku.tab.c"
    break;

  case 259: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1203 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4390 "raku.tab.c"
    break;

  case 260: /* dor_expr: range_expr  */
#line 1204 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4396 "raku.tab.c"
    break;

  case 261: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1207 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4402 "raku.tab.c"
    break;

  case 262: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1208 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4408 "raku.tab.c"
    break;

  case 263: /* range_expr: add_expr  */
#line 1209 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4414 "raku.tab.c"
    break;

  case 264: /* add_expr: add_expr '~' repl_expr  */
#line 1212 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4420 "raku.tab.c"
    break;

  case 265: /* add_expr: repl_expr  */
#line 1213 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4426 "raku.tab.c"
    break;

  case 266: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1216 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4432 "raku.tab.c"
    break;

  case 267: /* repl_expr: addsub_expr  */
#line 1217 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4438 "raku.tab.c"
    break;

  case 268: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1220 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4444 "raku.tab.c"
    break;

  case 269: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1221 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4450 "raku.tab.c"
    break;

  case 270: /* addsub_expr: mul_expr  */
#line 1222 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4456 "raku.tab.c"
    break;

  case 271: /* mul_expr: mul_expr '*' unary_expr  */
#line 1225 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4462 "raku.tab.c"
    break;

  case 272: /* mul_expr: mul_expr '/' unary_expr  */
#line 1226 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4468 "raku.tab.c"
    break;

  case 273: /* mul_expr: mul_expr '%' unary_expr  */
#line 1227 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4474 "raku.tab.c"
    break;

  case 274: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1228 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4480 "raku.tab.c"
    break;

  case 275: /* mul_expr: unary_expr  */
#line 1229 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4486 "raku.tab.c"
    break;

  case 276: /* unary_expr: '-' unary_expr  */
#line 1232 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4492 "raku.tab.c"
    break;

  case 277: /* unary_expr: '!' unary_expr  */
#line 1233 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4498 "raku.tab.c"
    break;

  case 278: /* unary_expr: CARET unary_expr  */
#line 1234 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 4504 "raku.tab.c"
    break;

  case 279: /* unary_expr: postfix_expr  */
#line 1235 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4510 "raku.tab.c"
    break;

  case 280: /* postfix_expr: call_expr  */
#line 1237 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4516 "raku.tab.c"
    break;

  case 281: /* call_expr: IDENT '(' arg_list ')'  */
#line 1240 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4525 "raku.tab.c"
    break;

  case 282: /* call_expr: IDENT '(' ')'  */
#line 1244 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4531 "raku.tab.c"
    break;

  case 283: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1246 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4540 "raku.tab.c"
    break;

  case 284: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1251 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4546 "raku.tab.c"
    break;

  case 285: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1253 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4556 "raku.tab.c"
    break;

  case 286: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1259 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4566 "raku.tab.c"
    break;

  case 287: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1265 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4574 "raku.tab.c"
    break;

  case 288: /* call_expr: IDENT '.' KW_NEW  */
#line 1269 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4582 "raku.tab.c"
    break;

  case 289: /* call_expr: IDENT '.' IDENT  */
#line 1273 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4591 "raku.tab.c"
    break;

  case 290: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1278 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4602 "raku.tab.c"
    break;

  case 291: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1285 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4611 "raku.tab.c"
    break;

  case 292: /* call_expr: IDENT '.' CARET IDENT  */
#line 1290 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4621 "raku.tab.c"
    break;

  case 293: /* call_expr: atom '.' CARET IDENT  */
#line 1296 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4631 "raku.tab.c"
    break;

  case 294: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1302 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4642 "raku.tab.c"
    break;

  case 295: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1309 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4653 "raku.tab.c"
    break;

  case 296: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1316 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4662 "raku.tab.c"
    break;

  case 297: /* call_expr: atom '.' IDENT  */
#line 1321 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4671 "raku.tab.c"
    break;

  case 298: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1326 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4682 "raku.tab.c"
    break;

  case 299: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1333 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4691 "raku.tab.c"
    break;

  case 300: /* call_expr: call_expr '.' IDENT  */
#line 1338 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4700 "raku.tab.c"
    break;

  case 301: /* call_expr: KW_DIE expr  */
#line 1343 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 4706 "raku.tab.c"
    break;

  case 302: /* call_expr: KW_MAP closure expr  */
#line 1345 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4712 "raku.tab.c"
    break;

  case 303: /* call_expr: KW_GREP closure expr  */
#line 1347 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4718 "raku.tab.c"
    break;

  case 304: /* call_expr: KW_SORT expr  */
#line 1349 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4724 "raku.tab.c"
    break;

  case 305: /* call_expr: KW_SORT closure expr  */
#line 1351 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4730 "raku.tab.c"
    break;

  case 306: /* call_expr: atom  */
#line 1352 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4736 "raku.tab.c"
    break;

  case 307: /* arg_list: expr  */
#line 1355 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4742 "raku.tab.c"
    break;

  case 308: /* arg_list: arg_list ',' expr  */
#line 1356 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4748 "raku.tab.c"
    break;

  case 309: /* atom: LIT_INT  */
#line 1359 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4754 "raku.tab.c"
    break;

  case 310: /* atom: LIT_FLOAT  */
#line 1360 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4760 "raku.tab.c"
    break;

  case 311: /* atom: LIT_STR  */
#line 1361 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4766 "raku.tab.c"
    break;

  case 312: /* atom: WORDLIST  */
#line 1363 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4778 "raku.tab.c"
    break;

  case 313: /* atom: LIT_INTERP_STR  */
#line 1370 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4784 "raku.tab.c"
    break;

  case 314: /* atom: VAR_SCALAR  */
#line 1371 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4790 "raku.tab.c"
    break;

  case 315: /* atom: VAR_ARRAY  */
#line 1372 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4796 "raku.tab.c"
    break;

  case 316: /* atom: VAR_HASH  */
#line 1373 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4802 "raku.tab.c"
    break;

  case 317: /* atom: VAR_CAPTURE  */
#line 1375 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4810 "raku.tab.c"
    break;

  case 318: /* atom: VAR_FH  */
#line 1379 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4818 "raku.tab.c"
    break;

  case 319: /* atom: VAR_NAMED_CAPTURE  */
#line 1383 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4825 "raku.tab.c"
    break;

  case 320: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1386 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4831 "raku.tab.c"
    break;

  case 321: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1388 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4837 "raku.tab.c"
    break;

  case 322: /* atom: VAR_HASH '{' expr '}'  */
#line 1390 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4843 "raku.tab.c"
    break;

  case 323: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1392 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4849 "raku.tab.c"
    break;

  case 324: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1394 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4855 "raku.tab.c"
    break;

  case 325: /* atom: IDENT  */
#line 1395 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4861 "raku.tab.c"
    break;

  case 326: /* atom: VAR_TWIGIL  */
#line 1397 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4869 "raku.tab.c"
    break;

  case 327: /* atom: VAR_ARRAY_TWIGIL  */
#line 1401 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4877 "raku.tab.c"
    break;

  case 328: /* atom: VAR_HASH_TWIGIL  */
#line 1405 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4885 "raku.tab.c"
    break;

  case 329: /* atom: '(' ')'  */
#line 1408 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 4891 "raku.tab.c"
    break;

  case 330: /* atom: '(' expr ')'  */
#line 1409 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 4897 "raku.tab.c"
    break;

  case 331: /* atom: '(' expr ',' ')'  */
#line 1411 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 4903 "raku.tab.c"
    break;

  case 332: /* atom: '(' expr ',' arg_list ')'  */
#line 1413 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 4910 "raku.tab.c"
    break;

  case 333: /* atom: block  */
#line 1415 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4916 "raku.tab.c"
    break;

  case 334: /* atom: KW_SUB block  */
#line 1416 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4922 "raku.tab.c"
    break;


#line 4926 "raku.tab.c"

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

#line 1418 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
