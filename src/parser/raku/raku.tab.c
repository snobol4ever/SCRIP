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

#line 248 "raku.tab.c"

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
  YYSYMBOL_KW_GIVEN = 35,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 36,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 37,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 38,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 39,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 40,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 41,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 42,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 43,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 44,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 45,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 46,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 47,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 48,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 49,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 50,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 51,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 52,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 53,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 54,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 55,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 56,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 57,                /* KW_HANDLES  */
  YYSYMBOL_OP_COLON_D = 58,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 59,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 60,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 61,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 62,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 63,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 64,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 65,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 66,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 67,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 68,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 69,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 70,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 71,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 72,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 73,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 74,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 75,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 76,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 77,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 78,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 79,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 80,                    /* OP_DIV  */
  YYSYMBOL_81_ = 81,                       /* '='  */
  YYSYMBOL_82_ = 82,                       /* '!'  */
  YYSYMBOL_83_ = 83,                       /* '<'  */
  YYSYMBOL_84_ = 84,                       /* '>'  */
  YYSYMBOL_85_ = 85,                       /* '|'  */
  YYSYMBOL_86_ = 86,                       /* '&'  */
  YYSYMBOL_87_ = 87,                       /* '~'  */
  YYSYMBOL_88_ = 88,                       /* '+'  */
  YYSYMBOL_89_ = 89,                       /* '-'  */
  YYSYMBOL_90_ = 90,                       /* '*'  */
  YYSYMBOL_91_ = 91,                       /* '/'  */
  YYSYMBOL_92_ = 92,                       /* '%'  */
  YYSYMBOL_UMINUS = 93,                    /* UMINUS  */
  YYSYMBOL_94_ = 94,                       /* '.'  */
  YYSYMBOL_95_ = 95,                       /* ';'  */
  YYSYMBOL_96_ = 96,                       /* ','  */
  YYSYMBOL_97_ = 97,                       /* '('  */
  YYSYMBOL_98_ = 98,                       /* ')'  */
  YYSYMBOL_99_ = 99,                       /* '['  */
  YYSYMBOL_100_ = 100,                     /* ']'  */
  YYSYMBOL_101_ = 101,                     /* '{'  */
  YYSYMBOL_102_ = 102,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 103,                 /* $accept  */
  YYSYMBOL_program = 104,                  /* program  */
  YYSYMBOL_stmt_list = 105,                /* stmt_list  */
  YYSYMBOL_stmt = 106,                     /* stmt  */
  YYSYMBOL_if_stmt = 107,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 108,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 109,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 110,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 111,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 112,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 113,               /* given_stmt  */
  YYSYMBOL_when_list = 114,                /* when_list  */
  YYSYMBOL_sub_decl = 115,                 /* sub_decl  */
  YYSYMBOL_class_decl = 116,               /* class_decl  */
  YYSYMBOL_role_decl = 117,                /* role_decl  */
  YYSYMBOL_is_clauses = 118,               /* is_clauses  */
  YYSYMBOL_class_body_list = 119,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 120,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 121,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 122,           /* named_arg_list  */
  YYSYMBOL_param_list = 123,               /* param_list  */
  YYSYMBOL_block = 124,                    /* block  */
  YYSYMBOL_closure = 125,                  /* closure  */
  YYSYMBOL_expr = 126,                     /* expr  */
  YYSYMBOL_cmp_expr = 127,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 128,                 /* jct_expr  */
  YYSYMBOL_range_expr = 129,               /* range_expr  */
  YYSYMBOL_add_expr = 130,                 /* add_expr  */
  YYSYMBOL_mul_expr = 131,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 132,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 133,             /* postfix_expr  */
  YYSYMBOL_call_expr = 134,                /* call_expr  */
  YYSYMBOL_arg_list = 135,                 /* arg_list  */
  YYSYMBOL_atom = 136                      /* atom  */
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
#define YYLAST   1587

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  542

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   336


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
       2,     2,     2,    82,     2,     2,     2,    92,    86,     2,
      97,    98,    90,    88,    96,    89,    94,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    95,
      83,    81,    84,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    99,     2,   100,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   101,    85,   102,    87,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    93
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   244,   244,   255,   256,   259,   261,   263,   265,   269,
     273,   275,   277,   279,   283,   287,   289,   291,   293,   295,
     297,   299,   301,   303,   305,   307,   309,   311,   313,   315,
     317,   319,   321,   326,   330,   334,   339,   343,   350,   355,
     358,   361,   364,   367,   370,   371,   372,   373,   374,   375,
     377,   379,   381,   383,   384,   385,   386,   387,   388,   389,
     392,   394,   396,   400,   404,   406,   410,   414,   418,   424,
     430,   435,   440,   448,   459,   460,   466,   474,   480,   489,
     496,   507,   523,   538,   539,   551,   565,   566,   569,   572,
     575,   578,   581,   584,   587,   590,   593,   596,   599,   602,
     609,   616,   623,   630,   634,   638,   642,   646,   653,   660,
     667,   674,   684,   692,   701,   708,   718,   728,   743,   744,
     749,   754,   761,   765,   771,   772,   773,   774,   775,   776,
     777,   778,   781,   782,   785,   788,   789,   794,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,   813,
     819,   825,   828,   829,   830,   833,   834,   835,   838,   839,
     840,   841,   844,   845,   846,   847,   848,   851,   852,   853,
     855,   857,   862,   863,   868,   870,   876,   882,   886,   893,
     898,   904,   910,   917,   924,   929,   934,   936,   938,   940,
     942,   944,   947,   948,   951,   952,   953,   954,   955,   956,
     957,   958,   962,   966,   969,   971,   973,   975,   977,   979,
     980,   984,   988,   992,   993,   994
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
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_GIVEN", "KW_WHEN",
  "KW_DEFAULT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL",
  "KW_REPEAT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_ROLE",
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "OP_COLON_D",
  "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND", "OP_OR", "OP_BIND",
  "OP_DOTEQ", "OP_SMATCH", "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'",
  "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'",
  "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "$accept", "program",
  "stmt_list", "stmt", "if_stmt", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "class_decl", "role_decl", "is_clauses", "class_body_list",
  "grammar_decl", "grammar_body_list", "named_arg_list", "param_list",
  "block", "closure", "expr", "cmp_expr", "jct_expr", "range_expr",
  "add_expr", "mul_expr", "unary_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-306)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-215)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -306,    44,   539,  -306,  -306,  -306,  -306,  -306,   -22,   -48,
     -51,   -15,   -18,  -306,  -306,  -306,  -306,  -306,    67,   599,
     299,  1180,  1235,    23,    28,  1282,    -6,    42,  1282,   647,
    1282,   123,   144,    65,    69,    42,   112,   112,  1337,    42,
      42,  1282,   189,   203,   194,   208,  1486,  1486,  1282,   187,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,   183,   156,   102,   159,  -306,    97,   129,  -306,
    -306,  -306,   167,   258,  1282,   292,   702,  1282,   306,  1282,
    1282,    48,   749,   181,    34,   254,   -47,  -306,    42,  -306,
     810,  -306,  -306,    50,    45,   214,   282,   200,  1282,   287,
    1282,   288,  1282,  1282,   -16,   105,   289,  -306,  -306,   290,
    -306,   293,   283,   -43,    16,  1282,  1282,  -306,  1282,  1282,
    1282,   857,  1282,  -306,   340,  -306,  -306,  -306,   291,    14,
     294,   296,  -306,  -306,   298,   300,   423,  -306,  1486,  1486,
    1486,  1486,  1486,  1486,  1486,  1486,   227,  1486,  1486,  1486,
    1486,  1486,  1486,  1486,  1486,  1486,  1486,  1486,  1486,  1486,
     136,    10,   305,    55,  -306,   196,   301,   307,   302,   308,
     297,   375,   309,  -306,   218,  -306,  1282,  1282,   393,  1282,
     313,   298,   228,  -306,  1282,  1282,  -306,  1384,  1282,    92,
      93,   109,   229,  -306,   232,  -306,   312,   314,   401,  -306,
    1486,  1486,    -7,  -306,  -306,  -306,   399,  1282,   400,  1282,
     317,   318,   315,  -306,  -306,  -306,    42,     7,  -306,   321,
     322,  -306,  -306,  -306,  -306,    83,    83,    83,    83,    83,
      83,    83,    83,  -306,  -306,  -306,    83,    83,  -306,  -306,
     152,   152,   129,   129,   129,  -306,  -306,  -306,  -306,   325,
     409,  -306,   917,  -306,  1282,   964,  -306,   343,   349,   350,
    -306,  1025,  -306,  1072,  -306,  -306,   332,   356,   348,  -306,
     364,  -306,   365,  1282,   169,   378,  1282,  -306,  1439,  -306,
    1282,  -306,  1282,  1282,    42,    42,    42,   108,   111,  -306,
      24,    42,   233,   -14,   367,   372,   391,   376,    42,    42,
    -306,  -306,   465,   467,  -306,   -24,    -5,    35,    47,  1133,
    -306,   388,   236,   390,   392,   237,  1282,  1282,  1282,  -306,
     240,    73,  -306,   241,   244,  -306,  -306,  -306,  -306,  -306,
     245,  -306,  1282,  -306,   394,  1282,   191,   395,   396,   397,
     459,  -306,  -306,   478,   481,  -306,   482,   483,  -306,    43,
      42,  1282,    42,  -306,  -306,  -306,   402,   403,   472,  -306,
    -306,  -306,    -8,    21,   192,   450,  -306,    42,   248,   249,
     488,   489,   490,  -306,  -306,   253,   256,  -306,   411,  -306,
    -306,   412,   413,   415,   418,  -306,  1282,   500,  -306,  -306,
    1282,   276,  -306,   262,  -306,  1282,  -306,   421,   422,   -12,
      42,    42,  -306,  -306,  -306,    38,  -306,    42,   416,  -306,
    -306,    42,  -306,   424,   425,    17,    33,   275,   428,   432,
     505,  -306,    42,    42,   521,   522,   523,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,   466,   263,  -306,  1282,   281,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,   524,   525,  -306,
    -306,  -306,     3,     5,   518,   519,   442,  1282,  -306,   526,
     532,   444,  1282,  -306,    49,    70,   452,   453,  -306,  -306,
     469,  -306,  -306,  -306,  -306,  -306,  1282,   461,   266,  -306,
    -306,  -306,    42,   271,    42,   272,   480,   494,  -306,   495,
     496,   499,  -306,   501,   553,   562,   502,  1282,  -306,   581,
     584,   504,  1282,  -306,  -306,  -306,     6,  -306,  -306,   506,
    -306,    42,  -306,    42,  -306,  -306,  -306,  -306,  -306,  -306,
     511,   512,  -306,   513,   527,   528,  -306,   529,    42,   277,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
      42,  -306
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   194,   195,   196,   197,   198,   199,
     200,   210,   209,   211,   212,   201,   202,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     3,
       4,    45,    46,    53,    54,    55,    47,    48,    56,    57,
      58,    59,    52,     0,   137,   151,   154,   157,   161,   166,
     169,   170,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,   199,   200,   210,     0,    20,
       0,   214,   192,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,     0,   215,   136,     0,
      30,     0,     0,     0,     0,     0,     0,    67,     0,     0,
       0,     3,     0,   189,    49,    51,   186,    83,     0,     0,
       0,   198,   168,   167,     0,     0,     0,    44,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   174,     0,     0,     0,     0,     0,
       0,     0,     0,   172,     0,    19,     0,     0,     0,     0,
       0,   192,     0,    23,     0,     0,     6,     0,     0,     0,
       0,     0,     0,    24,     0,    26,     0,     0,     0,    71,
       0,     0,     0,    28,    29,    74,     0,     0,     0,     0,
       0,     0,     0,   187,   188,   190,     0,     0,    86,     0,
       0,   118,   213,   133,   132,   138,   139,   140,   141,   144,
     145,   146,   147,   148,   149,   150,   142,   143,   152,   153,
     155,   156,   160,   158,   159,   165,   162,   163,   164,   185,
       0,    34,     0,    31,     0,     0,   173,   204,   205,   206,
      36,     0,   180,     0,   171,   135,     0,     0,     0,    22,
       0,   193,     0,     0,     0,     0,     0,    16,     0,    17,
       0,    18,     0,     0,     0,     0,     0,   155,   156,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,    50,     0,     0,    86,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,   179,
       0,   209,   177,     0,     0,   204,   205,   206,    21,     5,
       0,     7,     0,    10,     0,     0,     0,     0,     0,     0,
      60,    63,    70,     0,     0,   125,     0,     0,    77,     0,
       0,     0,     0,    72,   207,   208,     0,     0,    64,    66,
      84,    85,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,   117,   184,     0,     0,    33,     0,    35,
      38,     0,     0,     0,     0,   178,     0,     0,   175,   176,
       0,     0,    11,     0,    12,     0,    15,     0,     0,     0,
       0,     0,   126,   127,   128,     0,    76,     0,     0,    42,
      43,     0,    81,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,   183,   182,    32,
      37,    39,    40,    41,   122,     0,     0,     8,     0,     0,
      25,    27,    62,    61,    68,    69,   129,     0,     0,    75,
      73,    65,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,    87,     0,     0,     0,     0,    88,    89,
       0,    78,    80,   119,   120,   121,     0,     0,     0,    13,
     130,   131,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,   104,     0,     0,     0,     0,     0,    94,     0,
       0,     0,     0,    93,    90,    91,     0,   123,     9,     0,
     112,     0,   114,     0,   101,   102,    96,    99,   100,    95,
       0,     0,   105,     0,     0,     0,   106,     0,     0,     0,
      14,   111,   113,   109,   110,    98,   107,   108,    97,   116,
       0,   115
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -306,  -306,   616,  -306,   221,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,   323,  -306,  -306,   316,
    -305,    -2,   341,    39,  -306,   160,   231,   -19,   126,   -35,
    -306,  -306,   -72,  -306
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   136,    50,    51,    52,    53,    54,    55,    56,
      57,   293,    58,    59,    60,   217,   305,    61,   308,   323,
     292,    91,   119,    92,    64,    65,    66,    67,    68,    69,
      70,    71,    93,    72
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      62,   368,   369,   289,   165,   289,   105,   290,   106,   290,
     174,   132,   133,   289,    23,   289,   289,   290,   182,   290,
     290,   302,   351,   352,   107,   108,   363,   364,   219,   303,
     365,   454,    78,   117,   345,   413,   178,   124,   125,   455,
     206,    63,   363,   364,     3,   289,   365,   459,   446,   290,
      79,    77,   198,   404,   179,   460,    73,   405,   207,    74,
      99,   101,   170,   494,   104,   171,    80,   109,   111,   112,
     220,   495,    75,   414,   456,    76,    81,   123,   366,    82,
     126,    83,   346,   347,   499,    49,   107,   134,   353,    49,
     461,   291,   500,   367,   412,    49,   447,   448,   457,   208,
     172,   482,   199,   484,   528,   251,   496,   252,   304,   370,
     371,   372,   458,   162,   462,   176,   166,   209,   168,   169,
     102,   245,   246,   247,   248,   103,   185,   501,   463,   181,
     497,    76,   240,   241,    62,   113,   254,   192,   386,   194,
     186,   196,   197,    49,   498,   183,   184,   483,   485,   373,
     249,   502,   255,   250,   210,   211,   114,   212,   213,   214,
     212,   215,   115,   151,   152,   503,   116,    81,   149,   150,
      82,   200,   201,   276,   278,    63,   343,   138,   139,   344,
     312,   287,   288,   315,   153,   154,   155,   277,   279,   320,
     280,   324,   153,   154,   155,   153,   154,   155,   153,   154,
     155,   529,   415,   127,   281,   416,   417,   418,   419,   156,
     189,   190,   191,   118,   301,   265,   266,   128,   268,   157,
     158,   159,   130,   271,   272,   129,   274,   275,   140,   141,
     142,   143,   144,   145,   233,   234,   235,   376,   146,   153,
     154,   155,   147,   148,   149,   150,   295,   135,   297,  -214,
    -214,   137,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,
     391,   160,  -214,  -214,   331,   332,  -214,  -214,  -214,  -214,
    -214,  -214,   161,  -214,  -214,  -214,   175,  -214,  -214,   242,
     243,   244,   340,   341,   342,   464,   394,   395,   465,   348,
     466,   467,   184,   313,   256,   187,   358,   359,   225,   226,
     227,   228,   229,   230,   231,   232,   163,   236,   237,    94,
      95,    96,   330,    97,   184,   334,   264,   336,   436,   337,
     167,   338,   339,   439,   184,   282,   270,   222,   283,   349,
     222,   350,   184,   184,   378,   381,   184,   387,   385,   388,
     184,   390,   389,   222,   349,   349,   422,   423,   406,   387,
     408,   427,   184,   177,   428,   382,   383,   384,   438,   184,
     222,   477,   184,   188,   509,   421,   478,   349,   349,   511,
     513,   437,   184,   349,   393,   540,   479,   184,   120,   122,
     238,   239,   193,   195,   205,   203,   202,   216,   204,   262,
     407,   258,   218,    76,   261,   221,   222,   443,   444,   445,
     253,   257,   223,   260,   259,   449,   263,   267,   269,   451,
     284,   286,   285,   294,   296,   298,   299,   300,   306,   307,
     471,   472,   309,   310,   316,   434,     4,     5,     6,     7,
     317,   318,   325,     8,     9,    10,    11,    12,    13,    14,
     326,    15,    16,    17,    18,    19,    20,    21,    22,    23,
     327,   354,    24,    25,    26,    27,    28,    29,    30,   328,
     329,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,   333,   355,   356,    43,    44,   357,   360,
     510,   361,   512,   377,    45,   379,   399,   380,   400,   392,
     396,   401,   402,   403,   397,   398,   489,   409,   410,   411,
     420,   493,   424,   425,   426,    46,   429,   430,   431,   531,
     432,   532,    47,   433,   435,   507,   440,   441,   450,   470,
      48,   452,   453,   468,    49,   224,   539,   469,   473,   474,
     475,   476,   486,   487,   480,   481,   523,   488,   541,   492,
     490,   527,     4,     5,     6,     7,   491,   504,   505,     8,
       9,    10,    11,    12,    13,    14,   508,    15,    16,    17,
      18,    19,    20,    21,    22,    23,   506,   520,    24,    25,
      26,    27,    28,    29,    30,   514,   521,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,   515,
     516,   517,    43,    44,   518,   524,   519,   522,   525,   526,
      45,   530,     4,     5,     6,     7,   533,   534,   535,    84,
      85,    86,    87,    12,    13,    14,     2,    15,    16,    17,
     442,    46,   536,   537,   538,   375,     0,   362,    47,     0,
      88,    27,     0,     0,     0,     0,    48,    31,     0,     0,
      49,     0,    36,    37,    38,     0,     0,    41,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    84,    85,    86,
      87,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    27,
       0,    46,     0,     0,     0,    31,     0,     0,    47,     0,
      36,    37,    38,     0,    89,    41,    90,     0,     0,     0,
      49,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    84,    85,    86,    87,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     0,    88,    27,     0,    47,     0,     0,     0,
      31,     0,   110,     0,    48,    36,    37,    38,    49,     0,
      41,     0,     4,     5,     6,     7,     0,     0,     0,    84,
      85,    86,    87,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    27,     0,     0,    46,     0,     0,    31,     0,     0,
       0,    47,    36,    37,    38,     0,     0,    41,     0,    48,
     164,     0,     0,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      84,    85,    86,    87,    12,    13,    14,     0,    15,    16,
      17,    46,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    88,    27,     0,     0,     0,    48,   173,    31,     0,
      49,     0,     0,    36,    37,    38,     0,     0,    41,     0,
       4,     5,     6,     7,     0,     0,     0,    84,    85,    86,
      87,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    27,
       0,     0,    46,     0,     0,    31,     0,     0,     0,    47,
      36,    37,    38,     0,     0,    41,     0,    48,   180,     0,
       0,    49,     0,     0,     0,     0,     0,   135,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    84,    85,    86,
      87,    12,    13,    14,     0,    15,    16,    17,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,    88,    27,
       0,     0,     0,     0,    48,    31,     0,     0,    49,     0,
      36,    37,    38,     0,     0,    41,     0,     4,     5,     6,
       7,     0,     0,     0,    84,    85,    86,    87,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    27,     0,     0,    46,
       0,     0,    31,     0,     0,     0,    47,    36,    37,    38,
       0,     0,    41,     0,    48,   311,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    84,    85,    86,    87,    12,
      13,    14,     0,    15,    16,    17,    46,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    88,    27,     0,     0,
       0,    48,   314,    31,     0,    49,     0,     0,    36,    37,
      38,     0,     0,    41,     0,     4,     5,     6,     7,     0,
       0,     0,    84,    85,    86,    87,   321,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    27,     0,     0,    46,     0,     0,
      31,     0,     0,     0,    47,    36,    37,    38,     0,     0,
      41,     0,    48,   319,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    84,    85,    86,    87,   321,    13,    14,
       0,    15,    16,    17,    46,     0,     0,     0,     0,     0,
       0,    47,     0,     0,    88,    27,     0,     0,     0,    48,
     322,    31,     0,    49,     0,     0,    36,    37,    38,     0,
       0,    41,     0,     4,     5,     6,     7,     0,     0,     0,
      84,    85,    86,    87,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    27,     0,     0,    46,     0,     0,    31,     0,
       0,     0,    47,    36,    37,    38,     0,     0,    41,     0,
      48,   374,     0,     0,    49,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    84,    85,    86,    87,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,    88,    27,     0,    47,
       0,     0,     0,    31,     0,     0,     0,    98,    36,    37,
      38,    49,     0,    41,     0,     4,     5,     6,     7,     0,
       0,     0,    84,    85,    86,    87,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    27,     0,     0,    46,     0,     0,
      31,     0,     0,     0,    47,    36,    37,    38,     0,     0,
      41,     0,   100,     0,     0,     0,    49,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    84,    85,    86,
      87,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,    46,     0,     0,     0,    88,    27,
       0,    47,     0,     0,     0,    31,     0,     0,     0,    48,
      36,    37,    38,    49,     0,    41,     0,     4,     5,     6,
       7,     0,     0,     0,    84,    85,    86,    87,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    27,     0,     0,    46,
       0,     0,    31,     0,     0,     0,    47,    36,    37,    38,
       0,     0,    41,     0,    48,     0,     0,     0,   121,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    84,
      85,    86,    87,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,    46,     0,     0,     0,
      88,    27,     0,    47,     0,     0,     0,    31,     0,     0,
       0,   273,    36,    37,    38,    49,     0,    41,     0,     4,
       5,     6,     7,     0,     0,     0,   131,    85,    86,    87,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,    46,     0,     0,    31,     0,     0,     0,    47,    36,
      37,    38,     0,     0,    41,     0,   335,     0,     0,     0,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,     0,    49
};

static const yytype_int16 yycheck[] =
{
       2,   306,   307,    10,    76,    10,    25,    14,    14,    14,
      82,    46,    47,    10,    26,    10,    10,    14,    90,    14,
      14,    14,    36,    37,    26,    27,    50,    51,    14,    22,
      54,    14,    83,    35,    10,    14,    83,    39,    40,    22,
      83,     2,    50,    51,     0,    10,    54,    14,    10,    14,
     101,    99,    68,    10,   101,    22,    78,    14,   101,    81,
      21,    22,    14,    14,    25,    17,    81,    28,    29,    30,
      56,    22,    94,    52,    57,    97,    94,    38,   102,    97,
      41,    14,    58,    59,    14,   101,    88,    48,   102,   101,
      57,    98,    22,    98,   102,   101,    58,    59,    81,    83,
      52,    98,   104,    98,    98,    95,    57,    97,   101,    62,
      63,    64,    95,    74,    81,    81,    77,   101,    79,    80,
      97,   156,   157,   158,   159,    97,    81,    57,    95,    90,
      81,    97,   151,   152,   136,    12,    81,    98,    65,   100,
      95,   102,   103,   101,    95,    95,    96,   452,   453,   102,
      14,    81,    97,    17,   115,   116,    12,   118,   119,   120,
     121,   122,    97,    66,    67,    95,    97,    94,    85,    86,
      97,    66,    67,    81,    81,   136,    68,    75,    76,    68,
     252,   200,   201,   255,    87,    88,    89,    95,    95,   261,
      81,   263,    87,    88,    89,    87,    88,    89,    87,    88,
      89,   506,    10,    14,    95,    13,    14,    15,    16,    80,
      10,    11,    12,   101,   216,   176,   177,    14,   179,    90,
      91,    92,    14,   184,   185,    31,   187,   188,    69,    70,
      71,    72,    73,    74,     7,     8,     9,   309,    79,    87,
      88,    89,    83,    84,    85,    86,   207,    60,   209,    66,
      67,    95,    69,    70,    71,    72,    73,    74,    75,    76,
     332,    94,    79,    80,    95,    96,    83,    84,    85,    86,
      87,    88,    14,    90,    91,    92,    95,    94,    95,   153,
     154,   155,   284,   285,   286,    10,    95,    96,    13,   291,
      15,    16,    96,   254,    98,    81,   298,   299,   138,   139,
     140,   141,   142,   143,   144,   145,    14,   147,   148,    10,
      11,    12,   273,    14,    96,   276,    98,   278,   390,   280,
      14,   282,   283,   395,    96,    96,    98,    98,    96,    96,
      98,    98,    96,    96,    98,    98,    96,    96,    98,    98,
      96,    96,    98,    98,    96,    96,    98,    98,   350,    96,
     352,    98,    96,    99,    98,   316,   317,   318,    96,    96,
      98,    98,    96,    81,    98,   367,   438,    96,    96,    98,
      98,    95,    96,    96,   335,    98,    95,    96,    37,    38,
     149,   150,    95,    95,   101,    95,    97,    47,    95,    14,
     351,    84,   101,    97,    97,   101,    98,   399,   400,   401,
      95,   100,   102,    95,   102,   407,    97,    14,    95,   411,
      98,    10,    98,    14,    14,    98,    98,   102,    97,    97,
     422,   423,    97,    14,    81,   386,     3,     4,     5,     6,
      81,    81,   100,    10,    11,    12,    13,    14,    15,    16,
      84,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     102,    84,    29,    30,    31,    32,    33,    34,    35,    95,
      95,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    95,   102,    84,    53,    54,   102,    14,
     482,    14,   484,    95,    61,    95,    27,    95,    10,    95,
      95,    10,    10,    10,    98,    98,   457,    95,    95,    27,
      50,   462,    14,    14,    14,    82,    95,    95,    95,   511,
      95,   513,    89,    95,    14,   476,    95,    95,   102,    14,
      97,    97,    97,    95,   101,   102,   528,    95,     7,     7,
       7,    65,    14,    14,    10,    10,   497,    95,   540,    95,
      14,   502,     3,     4,     5,     6,    14,    95,    95,    10,
      11,    12,    13,    14,    15,    16,    95,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    97,    14,    29,    30,
      31,    32,    33,    34,    35,    95,    14,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    95,
      95,    95,    53,    54,    95,    14,    95,    95,    14,    95,
      61,    95,     3,     4,     5,     6,    95,    95,    95,    10,
      11,    12,    13,    14,    15,    16,     0,    18,    19,    20,
     399,    82,    95,    95,    95,   309,    -1,   304,    89,    -1,
      31,    32,    -1,    -1,    -1,    -1,    97,    38,    -1,    -1,
     101,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    82,    -1,    -1,    -1,    38,    -1,    -1,    89,    -1,
      43,    44,    45,    -1,    95,    48,    97,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    31,    32,    -1,    89,    -1,    -1,    -1,
      38,    -1,    95,    -1,    97,    43,    44,    45,   101,    -1,
      48,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    82,    -1,    -1,    38,    -1,    -1,
      -1,    89,    43,    44,    45,    -1,    -1,    48,    -1,    97,
      98,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    82,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    31,    32,    -1,    -1,    -1,    97,    98,    38,    -1,
     101,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    82,    -1,    -1,    38,    -1,    -1,    -1,    89,
      43,    44,    45,    -1,    -1,    48,    -1,    97,    98,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    31,    32,
      -1,    -1,    -1,    -1,    97,    38,    -1,    -1,   101,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    82,
      -1,    -1,    38,    -1,    -1,    -1,    89,    43,    44,    45,
      -1,    -1,    48,    -1,    97,    98,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    31,    32,    -1,    -1,
      -1,    97,    98,    38,    -1,   101,    -1,    -1,    43,    44,
      45,    -1,    -1,    48,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    82,    -1,    -1,
      38,    -1,    -1,    -1,    89,    43,    44,    45,    -1,    -1,
      48,    -1,    97,    98,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    31,    32,    -1,    -1,    -1,    97,
      98,    38,    -1,   101,    -1,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    82,    -1,    -1,    38,    -1,
      -1,    -1,    89,    43,    44,    45,    -1,    -1,    48,    -1,
      97,    98,    -1,    -1,   101,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    31,    32,    -1,    89,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    97,    43,    44,
      45,   101,    -1,    48,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    82,    -1,    -1,
      38,    -1,    -1,    -1,    89,    43,    44,    45,    -1,    -1,
      48,    -1,    97,    -1,    -1,    -1,   101,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    31,    32,
      -1,    89,    -1,    -1,    -1,    38,    -1,    -1,    -1,    97,
      43,    44,    45,   101,    -1,    48,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    82,
      -1,    -1,    38,    -1,    -1,    -1,    89,    43,    44,    45,
      -1,    -1,    48,    -1,    97,    -1,    -1,    -1,   101,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      31,    32,    -1,    89,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    97,    43,    44,    45,   101,    -1,    48,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    82,    -1,    -1,    38,    -1,    -1,    -1,    89,    43,
      44,    45,    -1,    -1,    48,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,   101
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   104,   105,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    53,    54,    61,    82,    89,    97,   101,
     106,   107,   108,   109,   110,   111,   112,   113,   115,   116,
     117,   120,   124,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   136,    78,    81,    94,    97,    99,    83,   101,
      81,    94,    97,    14,    10,    11,    12,    13,    31,    95,
      97,   124,   126,   135,    10,    11,    12,    14,    97,   126,
      97,   126,    97,    97,   126,   130,    14,   124,   124,   126,
      95,   126,   126,    12,    12,    97,    97,   124,   101,   125,
     125,   101,   125,   126,   124,   124,   126,    14,    14,    31,
      14,    10,   132,   132,   126,    60,   105,    95,    75,    76,
      69,    70,    71,    72,    73,    74,    79,    83,    84,    85,
      86,    66,    67,    87,    88,    89,    80,    90,    91,    92,
      94,    14,   126,    14,    98,   135,   126,    14,   126,   126,
      14,    17,    52,    98,   135,    95,    81,    99,    83,   101,
      98,   126,   135,    95,    96,    81,    95,    81,    81,    10,
      11,    12,   126,    95,   126,    95,   126,   126,    68,   124,
      66,    67,    97,    95,    95,   101,    83,   101,    83,   101,
     126,   126,   126,   126,   126,   126,    47,   118,   101,    14,
      56,   101,    98,   102,   102,   128,   128,   128,   128,   128,
     128,   128,   128,     7,     8,     9,   128,   128,   129,   129,
     130,   130,   131,   131,   131,   132,   132,   132,   132,    14,
      17,    95,    97,    95,    81,    97,    98,   100,    84,   102,
      95,    97,    14,    97,    98,   126,   126,    14,   126,    95,
      98,   126,   126,    97,   126,   126,    81,    95,    81,    95,
      81,    95,    96,    96,    98,    98,    10,   130,   130,    10,
      14,    98,   123,   114,    14,   126,    14,   126,    98,    98,
     102,   124,    14,    22,   101,   119,    97,    97,   121,    97,
      14,    98,   135,   126,    98,   135,    81,    81,    81,    98,
     135,    14,    98,   122,   135,   100,    84,   102,    95,    95,
     126,    95,    96,    95,   126,    97,   126,   126,   126,   126,
     124,   124,   124,    68,    68,    10,    58,    59,   124,    96,
      98,    36,    37,   102,    84,   102,    84,   102,   124,   124,
      14,    14,   119,    50,    51,    54,   102,    98,   123,   123,
      62,    63,    64,   102,    98,   122,   135,    95,    98,    95,
      95,    98,   126,   126,   126,    98,    65,    96,    98,    98,
      96,   135,    95,   126,    95,    96,    95,    98,    98,    27,
      10,    10,    10,    10,    10,    14,   124,   126,   124,    95,
      95,    27,   102,    14,    52,    10,    13,    14,    15,    16,
      50,   124,    98,    98,    14,    14,    14,    98,    98,    95,
      95,    95,    95,    95,   126,    14,   135,    95,    96,   135,
      95,    95,   107,   124,   124,   124,    10,    58,    59,   124,
     102,   124,    97,    97,    14,    22,    57,    81,    95,    14,
      22,    57,    81,    95,    10,    13,    15,    16,    95,    95,
      14,   124,   124,     7,     7,     7,    65,    98,   135,    95,
      10,    10,    98,   123,    98,   123,    14,    14,    95,   126,
      14,    14,    95,   126,    14,    22,    57,    81,    95,    14,
      22,    57,    81,    95,    95,    95,    97,   126,    95,    98,
     124,    98,   124,    98,    95,    95,    95,    95,    95,    95,
      14,    14,    95,   126,    14,    14,    95,   126,    98,   123,
      95,   124,   124,    95,    95,    95,    95,    95,    95,   124,
      98,   124
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   103,   104,   105,   105,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   107,   107,   108,   109,   109,   110,   111,   112,   112,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   116,   117,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   120,   121,   121,
     121,   121,   122,   122,   123,   123,   123,   123,   123,   123,
     123,   123,   124,   124,   125,   126,   126,   126,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   128,   129,   129,   129,   130,   130,
     130,   130,   131,   131,   131,   131,   131,   132,   132,   132,
     133,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   135,   135,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     7,     9,
       5,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       2,     5,     4,     3,     3,     7,     3,     7,     3,     3,
       2,     4,     7,     6,     4,     6,     4,     7,     6,     7,
       7,     7,     6,     6,     2,     1,     1,     1,     1,     2,
       4,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     7,     7,     5,     5,     7,     5,     2,     7,     7,
       5,     3,     5,     7,     0,     4,     6,     5,     7,     6,
       7,     6,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       6,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       7,     7,     6,     7,     6,     8,     7,     5,     0,     4,
       4,     4,     3,     5,     1,     2,     3,     3,     3,     4,
       5,     5,     3,     3,     3,     3,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     3,     3,     1,     2,     2,     1,
       1,     4,     3,     4,     3,     6,     6,     5,     6,     5,
       4,     4,     6,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     5,     5,     1,
       1,     1,     1,     3,     1,     2
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
#line 245 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1941 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 255 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1947 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 256 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1953 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 260 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1959 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 262 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1965 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 264 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1971 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 266 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1979 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 270 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1987 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 274 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1993 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 276 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1999 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 278 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2005 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 280 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2013 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 284 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2021 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 288 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2027 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 290 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2033 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 292 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2039 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 294 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2045 "raku.tab.c"
    break;

  case 19: /* stmt: KW_USE IDENT ';'  */
#line 296 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2051 "raku.tab.c"
    break;

  case 20: /* stmt: TESTOP ';'  */
#line 298 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2057 "raku.tab.c"
    break;

  case 21: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 300 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2063 "raku.tab.c"
    break;

  case 22: /* stmt: TESTOP '(' ')' ';'  */
#line 302 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2069 "raku.tab.c"
    break;

  case 23: /* stmt: TESTOP arg_list ';'  */
#line 304 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2075 "raku.tab.c"
    break;

  case 24: /* stmt: KW_SAY expr ';'  */
#line 306 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2081 "raku.tab.c"
    break;

  case 25: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 308 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2087 "raku.tab.c"
    break;

  case 26: /* stmt: KW_PRINT expr ';'  */
#line 310 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2093 "raku.tab.c"
    break;

  case 27: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 312 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2099 "raku.tab.c"
    break;

  case 28: /* stmt: KW_TAKE expr ';'  */
#line 314 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2105 "raku.tab.c"
    break;

  case 29: /* stmt: KW_RETURN expr ';'  */
#line 316 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2111 "raku.tab.c"
    break;

  case 30: /* stmt: KW_RETURN ';'  */
#line 318 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2117 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 320 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2123 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 322 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2132 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 327 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2140 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 331 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2148 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 335 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2157 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 340 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2165 "raku.tab.c"
    break;

  case 37: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 344 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2176 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 351 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2185 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 356 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2192 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 359 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2199 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 362 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2206 "raku.tab.c"
    break;

  case 42: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 365 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2213 "raku.tab.c"
    break;

  case 43: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 368 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2220 "raku.tab.c"
    break;

  case 44: /* stmt: expr ';'  */
#line 370 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2226 "raku.tab.c"
    break;

  case 45: /* stmt: if_stmt  */
#line 371 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2232 "raku.tab.c"
    break;

  case 46: /* stmt: while_stmt  */
#line 372 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2238 "raku.tab.c"
    break;

  case 47: /* stmt: for_stmt  */
#line 373 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2244 "raku.tab.c"
    break;

  case 48: /* stmt: given_stmt  */
#line 374 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2250 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TRY block  */
#line 376 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2256 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TRY block KW_CATCH block  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2262 "raku.tab.c"
    break;

  case 51: /* stmt: KW_CATCH block  */
#line 380 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2268 "raku.tab.c"
    break;

  case 52: /* stmt: block  */
#line 382 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2274 "raku.tab.c"
    break;

  case 53: /* stmt: unless_stmt  */
#line 383 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2280 "raku.tab.c"
    break;

  case 54: /* stmt: until_stmt  */
#line 384 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2286 "raku.tab.c"
    break;

  case 55: /* stmt: repeat_stmt  */
#line 385 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2292 "raku.tab.c"
    break;

  case 56: /* stmt: sub_decl  */
#line 386 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2298 "raku.tab.c"
    break;

  case 57: /* stmt: class_decl  */
#line 387 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2304 "raku.tab.c"
    break;

  case 58: /* stmt: role_decl  */
#line 388 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2310 "raku.tab.c"
    break;

  case 59: /* stmt: grammar_decl  */
#line 389 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2316 "raku.tab.c"
    break;

  case 60: /* if_stmt: KW_IF '(' expr ')' block  */
#line 393 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2322 "raku.tab.c"
    break;

  case 61: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 395 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2328 "raku.tab.c"
    break;

  case 62: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 397 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2334 "raku.tab.c"
    break;

  case 63: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 401 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2340 "raku.tab.c"
    break;

  case 64: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 405 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2346 "raku.tab.c"
    break;

  case 65: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 407 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2352 "raku.tab.c"
    break;

  case 66: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 411 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2358 "raku.tab.c"
    break;

  case 67: /* repeat_stmt: KW_REPEAT block  */
#line 415 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2364 "raku.tab.c"
    break;

  case 68: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 419 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2374 "raku.tab.c"
    break;

  case 69: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 425 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2384 "raku.tab.c"
    break;

  case 70: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 431 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2393 "raku.tab.c"
    break;

  case 71: /* for_stmt: KW_FOR expr block  */
#line 436 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2400 "raku.tab.c"
    break;

  case 72: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 441 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2412 "raku.tab.c"
    break;

  case 73: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 449 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2425 "raku.tab.c"
    break;

  case 74: /* when_list: %empty  */
#line 459 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2431 "raku.tab.c"
    break;

  case 75: /* when_list: when_list KW_WHEN expr block  */
#line 461 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2439 "raku.tab.c"
    break;

  case 76: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 467 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2451 "raku.tab.c"
    break;

  case 77: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 475 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2461 "raku.tab.c"
    break;

  case 78: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 481 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2474 "raku.tab.c"
    break;

  case 79: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 490 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2485 "raku.tab.c"
    break;

  case 80: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 497 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2498 "raku.tab.c"
    break;

  case 81: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 508 "raku.y"
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
#line 2516 "raku.tab.c"
    break;

  case 82: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 524 "raku.y"
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
#line 2533 "raku.tab.c"
    break;

  case 83: /* is_clauses: %empty  */
#line 538 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2539 "raku.tab.c"
    break;

  case 84: /* is_clauses: is_clauses IDENT IDENT  */
#line 540 "raku.y"
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
#line 2555 "raku.tab.c"
    break;

  case 85: /* is_clauses: is_clauses TESTOP IDENT  */
#line 552 "raku.y"
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
#line 2571 "raku.tab.c"
    break;

  case 86: /* class_body_list: %empty  */
#line 565 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2577 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 567 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2584 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 570 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2591 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 573 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2598 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 576 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2605 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 579 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2612 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 582 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2619 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 585 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2626 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 588 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2633 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 591 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2640 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 594 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2647 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 597 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2654 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 600 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2661 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 603 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2672 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 610 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2683 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 617 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2694 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 624 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2705 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 631 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2713 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 635 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2721 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 639 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2729 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 643 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2737 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 647 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2748 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 654 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2759 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 661 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2770 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 668 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2781 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 675 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2795 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 685 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2807 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 693 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2820 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 702 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2831 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 709 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2845 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 719 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2857 "raku.tab.c"
    break;

  case 117: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 729 "raku.y"
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
#line 2874 "raku.tab.c"
    break;

  case 118: /* grammar_body_list: %empty  */
#line 743 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2880 "raku.tab.c"
    break;

  case 119: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 745 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2889 "raku.tab.c"
    break;

  case 120: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 750 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2898 "raku.tab.c"
    break;

  case 121: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 755 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2907 "raku.tab.c"
    break;

  case 122: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 762 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2915 "raku.tab.c"
    break;

  case 123: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 766 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2923 "raku.tab.c"
    break;

  case 124: /* param_list: VAR_SCALAR  */
#line 771 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2929 "raku.tab.c"
    break;

  case 125: /* param_list: IDENT VAR_SCALAR  */
#line 772 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2935 "raku.tab.c"
    break;

  case 126: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 773 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2941 "raku.tab.c"
    break;

  case 127: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 774 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2947 "raku.tab.c"
    break;

  case 128: /* param_list: param_list ',' VAR_SCALAR  */
#line 775 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2953 "raku.tab.c"
    break;

  case 129: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 776 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2959 "raku.tab.c"
    break;

  case 130: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 777 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2965 "raku.tab.c"
    break;

  case 131: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 778 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2971 "raku.tab.c"
    break;

  case 132: /* block: '{' stmt_list '}'  */
#line 781 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2977 "raku.tab.c"
    break;

  case 133: /* block: '{' YADA '}'  */
#line 782 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2983 "raku.tab.c"
    break;

  case 134: /* closure: '{' expr '}'  */
#line 785 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2989 "raku.tab.c"
    break;

  case 135: /* expr: VAR_SCALAR '=' expr  */
#line 788 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2995 "raku.tab.c"
    break;

  case 136: /* expr: KW_GATHER block  */
#line 789 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3005 "raku.tab.c"
    break;

  case 137: /* expr: cmp_expr  */
#line 794 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3011 "raku.tab.c"
    break;

  case 138: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 797 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3017 "raku.tab.c"
    break;

  case 139: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 798 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3023 "raku.tab.c"
    break;

  case 140: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 799 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3029 "raku.tab.c"
    break;

  case 141: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 800 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3035 "raku.tab.c"
    break;

  case 142: /* cmp_expr: jct_expr '<' jct_expr  */
#line 801 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3041 "raku.tab.c"
    break;

  case 143: /* cmp_expr: jct_expr '>' jct_expr  */
#line 802 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3047 "raku.tab.c"
    break;

  case 144: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 803 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3053 "raku.tab.c"
    break;

  case 145: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 804 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3059 "raku.tab.c"
    break;

  case 146: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 805 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3065 "raku.tab.c"
    break;

  case 147: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 806 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3071 "raku.tab.c"
    break;

  case 148: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 808 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3081 "raku.tab.c"
    break;

  case 149: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 814 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3091 "raku.tab.c"
    break;

  case 150: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 820 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3101 "raku.tab.c"
    break;

  case 151: /* cmp_expr: jct_expr  */
#line 825 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3107 "raku.tab.c"
    break;

  case 152: /* jct_expr: jct_expr '|' range_expr  */
#line 828 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3113 "raku.tab.c"
    break;

  case 153: /* jct_expr: jct_expr '&' range_expr  */
#line 829 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3119 "raku.tab.c"
    break;

  case 154: /* jct_expr: range_expr  */
#line 830 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3125 "raku.tab.c"
    break;

  case 155: /* range_expr: add_expr OP_RANGE add_expr  */
#line 833 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3131 "raku.tab.c"
    break;

  case 156: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 834 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3137 "raku.tab.c"
    break;

  case 157: /* range_expr: add_expr  */
#line 835 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3143 "raku.tab.c"
    break;

  case 158: /* add_expr: add_expr '+' mul_expr  */
#line 838 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3149 "raku.tab.c"
    break;

  case 159: /* add_expr: add_expr '-' mul_expr  */
#line 839 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3155 "raku.tab.c"
    break;

  case 160: /* add_expr: add_expr '~' mul_expr  */
#line 840 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3161 "raku.tab.c"
    break;

  case 161: /* add_expr: mul_expr  */
#line 841 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 3167 "raku.tab.c"
    break;

  case 162: /* mul_expr: mul_expr '*' unary_expr  */
#line 844 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3173 "raku.tab.c"
    break;

  case 163: /* mul_expr: mul_expr '/' unary_expr  */
#line 845 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3179 "raku.tab.c"
    break;

  case 164: /* mul_expr: mul_expr '%' unary_expr  */
#line 846 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3185 "raku.tab.c"
    break;

  case 165: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 847 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3191 "raku.tab.c"
    break;

  case 166: /* mul_expr: unary_expr  */
#line 848 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3197 "raku.tab.c"
    break;

  case 167: /* unary_expr: '-' unary_expr  */
#line 851 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3203 "raku.tab.c"
    break;

  case 168: /* unary_expr: '!' unary_expr  */
#line 852 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3209 "raku.tab.c"
    break;

  case 169: /* unary_expr: postfix_expr  */
#line 853 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3215 "raku.tab.c"
    break;

  case 170: /* postfix_expr: call_expr  */
#line 855 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3221 "raku.tab.c"
    break;

  case 171: /* call_expr: IDENT '(' arg_list ')'  */
#line 858 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3230 "raku.tab.c"
    break;

  case 172: /* call_expr: IDENT '(' ')'  */
#line 862 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3236 "raku.tab.c"
    break;

  case 173: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 864 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3245 "raku.tab.c"
    break;

  case 174: /* call_expr: VAR_SCALAR '(' ')'  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3251 "raku.tab.c"
    break;

  case 175: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 871 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3261 "raku.tab.c"
    break;

  case 176: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 877 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3271 "raku.tab.c"
    break;

  case 177: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 883 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3279 "raku.tab.c"
    break;

  case 178: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 887 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3290 "raku.tab.c"
    break;

  case 179: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 894 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3299 "raku.tab.c"
    break;

  case 180: /* call_expr: IDENT '.' CARET IDENT  */
#line 899 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3309 "raku.tab.c"
    break;

  case 181: /* call_expr: atom '.' CARET IDENT  */
#line 905 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3319 "raku.tab.c"
    break;

  case 182: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 911 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3330 "raku.tab.c"
    break;

  case 183: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 918 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3341 "raku.tab.c"
    break;

  case 184: /* call_expr: atom '.' IDENT '(' ')'  */
#line 925 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3350 "raku.tab.c"
    break;

  case 185: /* call_expr: atom '.' IDENT  */
#line 930 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3359 "raku.tab.c"
    break;

  case 186: /* call_expr: KW_DIE expr  */
#line 935 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3365 "raku.tab.c"
    break;

  case 187: /* call_expr: KW_MAP closure expr  */
#line 937 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3371 "raku.tab.c"
    break;

  case 188: /* call_expr: KW_GREP closure expr  */
#line 939 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3377 "raku.tab.c"
    break;

  case 189: /* call_expr: KW_SORT expr  */
#line 941 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3383 "raku.tab.c"
    break;

  case 190: /* call_expr: KW_SORT closure expr  */
#line 943 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3389 "raku.tab.c"
    break;

  case 191: /* call_expr: atom  */
#line 944 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3395 "raku.tab.c"
    break;

  case 192: /* arg_list: expr  */
#line 947 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3401 "raku.tab.c"
    break;

  case 193: /* arg_list: arg_list ',' expr  */
#line 948 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3407 "raku.tab.c"
    break;

  case 194: /* atom: LIT_INT  */
#line 951 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3413 "raku.tab.c"
    break;

  case 195: /* atom: LIT_FLOAT  */
#line 952 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3419 "raku.tab.c"
    break;

  case 196: /* atom: LIT_STR  */
#line 953 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3425 "raku.tab.c"
    break;

  case 197: /* atom: LIT_INTERP_STR  */
#line 954 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3431 "raku.tab.c"
    break;

  case 198: /* atom: VAR_SCALAR  */
#line 955 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3437 "raku.tab.c"
    break;

  case 199: /* atom: VAR_ARRAY  */
#line 956 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3443 "raku.tab.c"
    break;

  case 200: /* atom: VAR_HASH  */
#line 957 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3449 "raku.tab.c"
    break;

  case 201: /* atom: VAR_CAPTURE  */
#line 959 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3457 "raku.tab.c"
    break;

  case 202: /* atom: VAR_FH  */
#line 963 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3465 "raku.tab.c"
    break;

  case 203: /* atom: VAR_NAMED_CAPTURE  */
#line 967 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3472 "raku.tab.c"
    break;

  case 204: /* atom: VAR_ARRAY '[' expr ']'  */
#line 970 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3478 "raku.tab.c"
    break;

  case 205: /* atom: VAR_HASH '<' IDENT '>'  */
#line 972 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3484 "raku.tab.c"
    break;

  case 206: /* atom: VAR_HASH '{' expr '}'  */
#line 974 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3490 "raku.tab.c"
    break;

  case 207: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 976 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3496 "raku.tab.c"
    break;

  case 208: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 978 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3502 "raku.tab.c"
    break;

  case 209: /* atom: IDENT  */
#line 979 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3508 "raku.tab.c"
    break;

  case 210: /* atom: VAR_TWIGIL  */
#line 981 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3516 "raku.tab.c"
    break;

  case 211: /* atom: VAR_ARRAY_TWIGIL  */
#line 985 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3524 "raku.tab.c"
    break;

  case 212: /* atom: VAR_HASH_TWIGIL  */
#line 989 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3532 "raku.tab.c"
    break;

  case 213: /* atom: '(' expr ')'  */
#line 992 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3538 "raku.tab.c"
    break;

  case 214: /* atom: block  */
#line 993 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3544 "raku.tab.c"
    break;

  case 215: /* atom: KW_SUB block  */
#line 994 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3550 "raku.tab.c"
    break;


#line 3554 "raku.tab.c"

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

#line 996 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
