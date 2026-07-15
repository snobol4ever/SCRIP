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
#define YYLAST   1384

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  210
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  533

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
     377,   379,   381,   382,   383,   384,   385,   386,   387,   390,
     392,   394,   398,   402,   404,   408,   412,   416,   422,   428,
     433,   438,   446,   457,   458,   464,   472,   478,   487,   494,
     505,   521,   536,   537,   549,   563,   564,   567,   570,   573,
     576,   579,   582,   585,   588,   591,   594,   597,   600,   607,
     614,   621,   628,   632,   636,   640,   644,   651,   658,   665,
     672,   682,   690,   699,   706,   716,   726,   741,   742,   747,
     752,   759,   763,   769,   770,   771,   772,   773,   774,   775,
     776,   779,   780,   783,   786,   787,   792,   795,   796,   797,
     798,   799,   800,   801,   802,   803,   804,   805,   811,   817,
     823,   826,   827,   828,   831,   832,   833,   836,   837,   838,
     839,   842,   843,   844,   845,   846,   849,   850,   851,   853,
     855,   860,   861,   867,   873,   877,   884,   889,   895,   901,
     908,   915,   920,   925,   927,   929,   931,   933,   935,   938,
     939,   942,   943,   944,   945,   946,   947,   948,   949,   953,
     957,   960,   962,   964,   966,   968,   970,   971,   975,   979,
     983
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

#define YYPACT_NINF (-297)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -297,    52,   894,  -297,  -297,  -297,  -297,  -297,    72,   -36,
     -32,    36,   106,  -297,  -297,  -297,  -297,  -297,    75,   954,
     254,  1050,  1097,    70,   108,  1145,   113,   111,  1145,  1002,
    1145,   195,   212,   131,   140,   111,   152,   152,   120,   111,
     111,  1145,   230,   277,   265,   290,  1287,  1287,  1145,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,
    -297,   225,    30,   209,  -297,   103,   143,  -297,  -297,  -297,
     221,   315,  1145,   326,  1145,   329,  1145,  1145,    33,   496,
     250,   272,   273,   -18,  -297,  -297,   543,  -297,    49,    -3,
     288,   292,   231,  1145,   281,  1145,   282,  1145,  1145,   -39,
     159,   283,   321,  -297,   293,  -297,   294,   289,    13,    54,
    1145,  1145,  -297,  1145,  1145,  1145,  1145,  -297,   336,  -297,
    -297,  -297,   297,    14,   307,  -297,  -297,  -297,   311,  -297,
    1287,  1287,  1287,  1287,  1287,  1287,  1287,  1287,   243,  1287,
    1287,  1287,  1287,  1287,  1287,  1287,  1287,  1287,  1287,  1287,
    1287,  1287,   197,   -23,   322,   -22,   318,   347,   330,   338,
     339,   423,   342,  -297,   -12,  -297,  1145,  1145,   427,  1145,
     348,   311,    89,  -297,  1145,  1145,  -297,  1192,  1145,    -1,
      27,    62,    97,  -297,   160,  -297,   346,   349,   435,  -297,
    1287,  1287,     3,   344,   381,  -297,  -297,  -297,   434,  1145,
     436,  1145,   351,   353,   350,  -297,  -297,  -297,   111,   -11,
    -297,   357,   358,  -297,  -297,    76,    76,    76,    76,    76,
      76,    76,    76,  -297,  -297,  -297,    76,    76,  -297,  -297,
     210,   210,   143,   143,   143,  -297,  -297,  -297,  -297,   359,
     443,  -297,   599,  -297,  1145,   646,   378,   379,   380,  -297,
     702,  -297,   749,  -297,  -297,   362,   382,   363,  -297,   369,
    -297,   373,  1145,   124,   376,  1145,  -297,  1240,  -297,  1145,
    -297,  1145,  1145,   111,   111,   111,   109,   142,  -297,     2,
     111,   173,  -297,  -297,    -5,   389,   372,   391,   374,   111,
     111,  -297,  -297,   463,   465,  -297,   -14,     6,    34,    -7,
     805,  -297,   385,   177,   386,   387,   207,  1145,  1145,  1145,
    -297,   211,   -26,  -297,   220,   223,  -297,  -297,  -297,  -297,
    -297,   228,  -297,  1145,  -297,   390,  1145,   190,   392,   388,
     393,   461,  -297,  -297,   479,   480,  -297,   482,   484,  -297,
     146,   111,  1145,   111,  -297,  -297,  -297,   400,   402,   476,
    -297,  -297,  -297,    -9,    24,   247,   454,  -297,   111,   229,
     232,   491,   499,   503,  -297,  -297,   248,   252,  -297,   424,
    -297,  -297,   425,   426,   428,   429,  -297,  1145,   504,  -297,
    -297,  1145,   215,  -297,   253,  -297,  1145,  -297,   430,   431,
      -2,   111,   111,  -297,  -297,  -297,     8,  -297,   111,   420,
    -297,  -297,   111,  -297,   432,   433,    -8,   127,   274,   437,
     438,   513,  -297,   111,   111,   524,   528,   529,  -297,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,   472,   256,  -297,  1145,
     275,  -297,  -297,  -297,  -297,  -297,  -297,  -297,   532,   533,
    -297,  -297,  -297,     9,    11,   531,   536,   456,  1145,  -297,
     538,   546,   469,  1145,  -297,   132,   137,   470,   471,  -297,
    -297,   441,  -297,  -297,  -297,  -297,  -297,  1145,   473,   262,
    -297,  -297,  -297,   111,   263,   111,   266,   474,   475,  -297,
     477,   478,   481,  -297,   485,   553,   557,   487,  1145,  -297,
     560,   563,   488,  1145,  -297,  -297,  -297,    44,  -297,  -297,
     489,  -297,   111,  -297,   111,  -297,  -297,  -297,  -297,  -297,
    -297,   494,   495,  -297,   497,   500,   501,  -297,   502,   111,
     270,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,
    -297,   111,  -297
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   191,   192,   193,   194,   195,   196,
     197,   207,   206,   208,   209,   198,   199,   200,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
      45,    46,    52,    53,    54,    47,    48,    55,    56,    57,
      58,     0,   136,   150,   153,   156,   160,   165,   168,   169,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   195,   196,   197,   207,    20,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     156,     0,     3,   135,     0,    30,     0,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,   186,    49,    51,
     183,    82,     0,     0,     0,   195,   167,   166,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,    19,     0,     0,     0,     0,
       0,   189,     0,    23,     0,     0,     6,     0,     0,     0,
       0,     0,     0,    24,     0,    26,     0,     0,     0,    70,
       0,     0,     0,     0,     0,    28,    29,    73,     0,     0,
       0,     0,     0,     0,     0,   184,   185,   187,     0,     0,
      85,     0,     0,   117,   210,   137,   138,   139,   140,   143,
     144,   145,   146,   147,   148,   149,   141,   142,   151,   152,
     154,   155,   159,   157,   158,   164,   161,   162,   163,   182,
       0,    34,     0,    31,     0,     0,   201,   202,   203,    36,
       0,   177,     0,   170,   134,     0,     0,     0,    22,     0,
     190,     0,     0,     0,     0,     0,    16,     0,    17,     0,
      18,     0,     0,     0,     0,     0,   154,   155,   123,     0,
       0,     0,   132,   131,     0,     0,     0,     0,     0,     0,
       0,   133,    50,     0,     0,    85,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,     0,     0,     0,     0,
     176,     0,   206,   174,     0,     0,   201,   202,   203,    21,
       5,     0,     7,     0,    10,     0,     0,     0,     0,     0,
       0,    59,    62,    69,     0,     0,   124,     0,     0,    76,
       0,     0,     0,     0,    71,   204,   205,     0,     0,    63,
      65,    83,    84,     0,     0,     0,     0,    81,     0,     0,
       0,     0,     0,     0,   116,   181,     0,     0,    33,     0,
      35,    38,     0,     0,     0,     0,   175,     0,     0,   172,
     173,     0,     0,    11,     0,    12,     0,    15,     0,     0,
       0,     0,     0,   125,   126,   127,     0,    75,     0,     0,
      42,    43,     0,    80,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,   180,   179,
      32,    37,    39,    40,    41,   121,     0,     0,     8,     0,
       0,    25,    27,    61,    60,    67,    68,   128,     0,     0,
      74,    72,    64,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,    86,     0,     0,     0,     0,    87,
      88,     0,    77,    79,   118,   119,   120,     0,     0,     0,
      13,   129,   130,     0,     0,     0,     0,     0,     0,   102,
       0,     0,     0,   103,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,    92,    89,    90,     0,   122,     9,
       0,   111,     0,   113,     0,   100,   101,    95,    98,    99,
      94,     0,     0,   104,     0,     0,     0,   105,     0,     0,
       0,    14,   110,   112,   108,   109,    97,   106,   107,    96,
     115,     0,   114
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -297,  -297,   498,  -297,   189,  -297,  -297,  -297,  -297,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,   303,  -297,  -297,   299,
    -296,   -35,   178,     5,  -297,   202,   233,   -15,   155,   -38,
    -297,  -297,   -64,  -297
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    49,    50,    51,    52,    53,    54,    55,
      56,   284,    57,    58,    59,   209,   296,    60,   299,   314,
     281,   103,   114,    87,    62,    63,    64,    65,    66,    67,
      68,    69,    88,    70
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     112,   359,   360,   293,   118,   119,   445,    61,   126,   127,
     100,   294,   336,   278,   446,   164,   278,   279,   437,   278,
     279,   278,   172,   279,    23,   279,    94,    96,   211,   188,
      99,   342,   343,   104,   106,   107,   354,   355,   404,   377,
     356,   354,   355,   117,   278,   356,   120,   160,   279,   447,
     161,    75,     3,   128,   278,   361,   362,   363,   279,   244,
     337,   338,   102,    74,   189,   168,   438,   439,    78,    76,
     212,    79,   241,   448,   242,   245,   405,   154,   175,   156,
     265,   158,   159,   169,   174,   162,   253,   449,   357,    80,
     295,   171,   176,   403,   266,   364,   198,   344,   182,   102,
     184,   280,   186,   187,   358,   130,   131,   473,   267,   475,
     235,   236,   237,   238,   199,   202,   203,    77,   204,   205,
     206,   207,   268,     4,     5,     6,     7,   101,   230,   231,
      81,    82,    83,    84,    12,    13,    14,   200,    15,    16,
      17,   450,   519,   269,   173,   174,   485,   474,   476,   451,
      71,   490,    27,    72,   486,   201,   395,   270,    31,   491,
     396,   141,   142,    36,    37,    38,    73,    97,    41,   143,
     144,   254,   255,   292,   257,   276,   277,   334,   303,   260,
     261,   306,   263,   264,   452,   174,   311,   259,   315,   487,
     145,   146,   147,   271,   492,   214,   145,   146,   147,    61,
      78,   520,    46,    79,   286,    98,   288,   108,   453,    47,
     335,   239,   102,   488,   240,   115,   116,    48,   493,   322,
     323,   113,   454,   148,   109,   190,   191,   489,   110,   145,
     146,   147,   494,   149,   150,   151,   367,   111,   331,   332,
     333,   179,   180,   181,   121,   339,   145,   146,   147,   304,
     223,   224,   225,   113,   349,   350,   272,   406,   214,   382,
     407,   408,   409,   410,    89,    90,    91,   321,    92,   340,
     325,   341,   327,   174,   328,   369,   329,   330,   132,   133,
     134,   135,   136,   137,   455,   385,   386,   456,   138,   457,
     458,   122,   139,   140,   141,   142,   123,   145,   146,   147,
     232,   233,   234,   174,   124,   372,   397,   174,   399,   376,
     428,   174,   373,   374,   375,   152,   378,   427,   379,   174,
     129,   380,   430,   412,   381,   340,   214,   413,   340,   153,
     414,   384,   215,   216,   217,   218,   219,   220,   221,   222,
     155,   226,   227,   157,   378,   165,   418,   398,   174,   429,
     419,   214,   174,   166,   468,   434,   435,   436,   174,   340,
     500,   502,   340,   440,   504,   469,   340,   442,   531,   177,
     470,   174,   167,   178,   228,   229,   183,   185,   462,   463,
     192,   193,   425,   208,     4,     5,     6,     7,   195,   196,
     197,     8,     9,    10,    11,    12,    13,    14,   210,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   213,   214,
      24,    25,    26,    27,    28,    29,    30,   243,   246,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,   247,   248,   249,    43,    44,   250,   251,   501,   252,
     503,   256,    45,   258,   273,   275,   282,   274,   285,   289,
     287,   290,   291,   480,   297,   298,   300,   301,   484,   307,
     308,   309,   316,    46,   319,   318,   317,   522,   320,   523,
      47,   324,   498,   345,   346,   347,   348,   351,    48,   352,
     368,   370,   371,   283,   530,   383,   388,   387,   390,   391,
     392,   389,   393,   514,   394,   400,   532,   401,   518,     4,
       5,     6,     7,   402,   411,   415,    81,    82,    83,    84,
      12,    13,    14,   416,    15,    16,    17,   417,   426,   420,
     421,   422,   441,   423,   424,   431,   432,   461,    27,   443,
     444,   464,   459,   460,    31,   465,   466,   467,   497,    36,
      37,    38,   471,   472,    41,   477,     4,     5,     6,     7,
     478,   479,   481,    81,    82,    83,    84,    12,    13,    14,
     482,    15,    16,    17,   483,   495,   496,   511,   499,   505,
     506,   512,   507,   508,   515,    27,   509,   516,    46,   433,
     510,    31,   513,   517,   521,    47,    36,    37,    38,   524,
     525,    41,   526,    48,   163,   527,   528,   529,   353,   366,
     194,     0,     4,     5,     6,     7,     0,     0,     0,    81,
      82,    83,    84,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,    46,     0,     0,     0,     0,
       0,    27,    47,     0,     0,     0,     0,    31,     0,     0,
      48,   170,    36,    37,    38,     0,     0,    41,     0,     4,
       5,     6,     7,     0,     0,     0,    81,    82,    83,    84,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,    46,     0,     0,    31,     0,     0,     0,    47,    36,
      37,    38,     0,     0,    41,     0,    48,   302,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     0,    27,    47,     0,     0,     0,     0,
      31,     0,     0,    48,   305,    36,    37,    38,     0,     0,
      41,     0,     4,     5,     6,     7,     0,     0,     0,    81,
      82,    83,    84,   312,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,    46,     0,     0,    31,     0,     0,
       0,    47,    36,    37,    38,     0,     0,    41,     0,    48,
     310,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    81,    82,    83,    84,   312,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,    46,     0,     0,     0,     0,     0,    27,    47,     0,
       0,     0,     0,    31,     0,     0,    48,   313,    36,    37,
      38,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,     0,     0,
       0,     0,     0,     0,    47,     0,     0,     4,     5,     6,
       7,     0,    48,   365,     8,     9,    10,    11,    12,    13,
      14,     0,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,     0,    24,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,     0,     0,     0,    43,    44,     0,
       0,     0,     0,     0,     0,    45,     0,     4,     5,     6,
       7,     0,     0,     0,    81,    82,    83,    84,    12,    13,
      14,     0,    15,    16,    17,     0,    46,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    27,     0,     0,     0,
       0,    48,    31,     0,     0,     0,     0,    36,    37,    38,
       0,     0,    41,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,    46,     0,     0,     0,
      31,     0,     0,    47,     0,    36,    37,    38,     0,    85,
      41,    86,     0,     4,     5,     6,     7,     0,     0,     0,
      81,    82,    83,    84,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    46,     0,     0,     0,    31,     0,
       0,    47,     0,    36,    37,    38,     0,   105,    41,    48,
       4,     5,     6,     7,     0,     0,     0,    81,    82,    83,
      84,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,    46,     0,     0,    31,     0,     0,     0,    47,
      36,    37,    38,     0,     0,    41,     0,    93,     4,     5,
       6,     7,     0,     0,     0,    81,    82,    83,    84,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    46,
       0,     0,     0,    31,     0,     0,    47,     0,    36,    37,
      38,     0,     0,    41,    95,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    46,     0,     0,
      31,     0,     0,     0,    47,    36,    37,    38,     0,     0,
      41,     0,    48,     4,     5,     6,     7,     0,     0,     0,
      81,    82,    83,    84,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    46,     0,     0,     0,    31,     0,
       0,    47,     0,    36,    37,    38,     0,     0,    41,   262,
       4,     5,     6,     7,     0,     0,     0,   125,    82,    83,
      84,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,    31,     0,     0,     0,    47,
      36,    37,    38,     0,     0,    41,     0,   326,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     0,     0,     0,     0,    47,     0,     0,     0,
       0,     0,     0,     0,    48
};

static const yytype_int16 yycheck[] =
{
      35,   297,   298,    14,    39,    40,    14,     2,    46,    47,
      25,    22,    10,    10,    22,    79,    10,    14,    10,    10,
      14,    10,    86,    14,    26,    14,    21,    22,    14,    68,
      25,    36,    37,    28,    29,    30,    50,    51,    14,    65,
      54,    50,    51,    38,    10,    54,    41,    14,    14,    57,
      17,    83,     0,    48,    10,    62,    63,    64,    14,    81,
      58,    59,   101,    99,    99,    83,    58,    59,    94,   101,
      56,    97,    95,    81,    97,    97,    52,    72,    81,    74,
      81,    76,    77,   101,    96,    52,    98,    95,   102,    14,
     101,    86,    95,   102,    95,   102,    83,   102,    93,   101,
      95,    98,    97,    98,    98,    75,    76,    98,    81,    98,
     148,   149,   150,   151,   101,   110,   111,    81,   113,   114,
     115,   116,    95,     3,     4,     5,     6,    14,   143,   144,
      10,    11,    12,    13,    14,    15,    16,    83,    18,    19,
      20,    14,    98,    81,    95,    96,    14,   443,   444,    22,
      78,    14,    32,    81,    22,   101,    10,    95,    38,    22,
      14,    85,    86,    43,    44,    45,    94,    97,    48,    66,
      67,   166,   167,   208,   169,   190,   191,    68,   242,   174,
     175,   245,   177,   178,    57,    96,   250,    98,   252,    57,
      87,    88,    89,    96,    57,    98,    87,    88,    89,   194,
      94,   497,    82,    97,   199,    97,   201,    12,    81,    89,
      68,    14,   101,    81,    17,    37,    38,    97,    81,    95,
      96,   101,    95,    80,    12,    66,    67,    95,    97,    87,
      88,    89,    95,    90,    91,    92,   300,    97,   273,   274,
     275,    10,    11,    12,    14,   280,    87,    88,    89,   244,
       7,     8,     9,   101,   289,   290,    96,    10,    98,   323,
      13,    14,    15,    16,    10,    11,    12,   262,    14,    96,
     265,    98,   267,    96,   269,    98,   271,   272,    69,    70,
      71,    72,    73,    74,    10,    95,    96,    13,    79,    15,
      16,    14,    83,    84,    85,    86,    31,    87,    88,    89,
     145,   146,   147,    96,    14,    98,   341,    96,   343,    98,
      95,    96,   307,   308,   309,    94,    96,   381,    98,    96,
      95,    98,   386,   358,    96,    96,    98,    98,    96,    14,
      98,   326,   130,   131,   132,   133,   134,   135,   136,   137,
      14,   139,   140,    14,    96,    95,    98,   342,    96,    96,
      98,    98,    96,    81,    98,   390,   391,   392,    96,    96,
      98,    98,    96,   398,    98,   429,    96,   402,    98,    81,
      95,    96,    99,    81,   141,   142,    95,    95,   413,   414,
      97,    60,   377,    47,     3,     4,     5,     6,    95,    95,
     101,    10,    11,    12,    13,    14,    15,    16,   101,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   101,    98,
      29,    30,    31,    32,    33,    34,    35,    95,   100,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    84,   102,    95,    53,    54,    97,    14,   473,    97,
     475,    14,    61,    95,    98,    10,   102,    98,    14,    98,
      14,    98,   102,   448,    97,    97,    97,    14,   453,    81,
      81,    81,   100,    82,    95,   102,    84,   502,    95,   504,
      89,    95,   467,    84,   102,    84,   102,    14,    97,    14,
      95,    95,    95,   102,   519,    95,    98,    95,    27,    10,
      10,    98,    10,   488,    10,    95,   531,    95,   493,     3,
       4,     5,     6,    27,    50,    14,    10,    11,    12,    13,
      14,    15,    16,    14,    18,    19,    20,    14,    14,    95,
      95,    95,   102,    95,    95,    95,    95,    14,    32,    97,
      97,     7,    95,    95,    38,     7,     7,    65,    97,    43,
      44,    45,    10,    10,    48,    14,     3,     4,     5,     6,
      14,    95,    14,    10,    11,    12,    13,    14,    15,    16,
      14,    18,    19,    20,    95,    95,    95,    14,    95,    95,
      95,    14,    95,    95,    14,    32,    95,    14,    82,   390,
      95,    38,    95,    95,    95,    89,    43,    44,    45,    95,
      95,    48,    95,    97,    98,    95,    95,    95,   295,   300,
     102,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    32,    89,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      97,    98,    43,    44,    45,    -1,    -1,    48,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    82,    -1,    -1,    38,    -1,    -1,    -1,    89,    43,
      44,    45,    -1,    -1,    48,    -1,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    32,    89,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    97,    98,    43,    44,    45,    -1,    -1,
      48,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    82,    -1,    -1,    38,    -1,    -1,
      -1,    89,    43,    44,    45,    -1,    -1,    48,    -1,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    32,    89,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    97,    98,    43,    44,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,     3,     4,     5,
       6,    -1,    97,    98,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    97,    38,    -1,    -1,    -1,    -1,    43,    44,    45,
      -1,    -1,    48,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    82,    -1,    -1,    -1,
      38,    -1,    -1,    89,    -1,    43,    44,    45,    -1,    95,
      48,    97,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    82,    -1,    -1,    -1,    38,    -1,
      -1,    89,    -1,    43,    44,    45,    -1,    95,    48,    97,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    82,    -1,    -1,    38,    -1,    -1,    -1,    89,
      43,    44,    45,    -1,    -1,    48,    -1,    97,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    82,
      -1,    -1,    -1,    38,    -1,    -1,    89,    -1,    43,    44,
      45,    -1,    -1,    48,    97,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    82,    -1,    -1,
      38,    -1,    -1,    -1,    89,    43,    44,    45,    -1,    -1,
      48,    -1,    97,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    82,    -1,    -1,    -1,    38,    -1,
      -1,    89,    -1,    43,    44,    45,    -1,    -1,    48,    97,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    38,    -1,    -1,    -1,    89,
      43,    44,    45,    -1,    -1,    48,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   104,   105,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    53,    54,    61,    82,    89,    97,   106,
     107,   108,   109,   110,   111,   112,   113,   115,   116,   117,
     120,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     136,    78,    81,    94,    99,    83,   101,    81,    94,    97,
      14,    10,    11,    12,    13,    95,    97,   126,   135,    10,
      11,    12,    14,    97,   126,    97,   126,    97,    97,   126,
     130,    14,   101,   124,   126,    95,   126,   126,    12,    12,
      97,    97,   124,   101,   125,   125,   125,   126,   124,   124,
     126,    14,    14,    31,    14,    10,   132,   132,   126,    95,
      75,    76,    69,    70,    71,    72,    73,    74,    79,    83,
      84,    85,    86,    66,    67,    87,    88,    89,    80,    90,
      91,    92,    94,    14,   126,    14,   126,    14,   126,   126,
      14,    17,    52,    98,   135,    95,    81,    99,    83,   101,
      98,   126,   135,    95,    96,    81,    95,    81,    81,    10,
      11,    12,   126,    95,   126,    95,   126,   126,    68,   124,
      66,    67,    97,    60,   105,    95,    95,   101,    83,   101,
      83,   101,   126,   126,   126,   126,   126,   126,    47,   118,
     101,    14,    56,   101,    98,   128,   128,   128,   128,   128,
     128,   128,   128,     7,     8,     9,   128,   128,   129,   129,
     130,   130,   131,   131,   131,   132,   132,   132,   132,    14,
      17,    95,    97,    95,    81,    97,   100,    84,   102,    95,
      97,    14,    97,    98,   126,   126,    14,   126,    95,    98,
     126,   126,    97,   126,   126,    81,    95,    81,    95,    81,
      95,    96,    96,    98,    98,    10,   130,   130,    10,    14,
      98,   123,   102,   102,   114,    14,   126,    14,   126,    98,
      98,   102,   124,    14,    22,   101,   119,    97,    97,   121,
      97,    14,    98,   135,   126,    98,   135,    81,    81,    81,
      98,   135,    14,    98,   122,   135,   100,    84,   102,    95,
      95,   126,    95,    96,    95,   126,    97,   126,   126,   126,
     126,   124,   124,   124,    68,    68,    10,    58,    59,   124,
      96,    98,    36,    37,   102,    84,   102,    84,   102,   124,
     124,    14,    14,   119,    50,    51,    54,   102,    98,   123,
     123,    62,    63,    64,   102,    98,   122,   135,    95,    98,
      95,    95,    98,   126,   126,   126,    98,    65,    96,    98,
      98,    96,   135,    95,   126,    95,    96,    95,    98,    98,
      27,    10,    10,    10,    10,    10,    14,   124,   126,   124,
      95,    95,    27,   102,    14,    52,    10,    13,    14,    15,
      16,    50,   124,    98,    98,    14,    14,    14,    98,    98,
      95,    95,    95,    95,    95,   126,    14,   135,    95,    96,
     135,    95,    95,   107,   124,   124,   124,    10,    58,    59,
     124,   102,   124,    97,    97,    14,    22,    57,    81,    95,
      14,    22,    57,    81,    95,    10,    13,    15,    16,    95,
      95,    14,   124,   124,     7,     7,     7,    65,    98,   135,
      95,    10,    10,    98,   123,    98,   123,    14,    14,    95,
     126,    14,    14,    95,   126,    14,    22,    57,    81,    95,
      14,    22,    57,    81,    95,    95,    95,    97,   126,    95,
      98,   124,    98,   124,    98,    95,    95,    95,    95,    95,
      95,    14,    14,    95,   126,    14,    14,    95,   126,    98,
     123,    95,   124,   124,    95,    95,    95,    95,    95,    95,
     124,    98,   124
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   103,   104,   105,   105,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   107,
     107,   107,   108,   109,   109,   110,   111,   112,   112,   112,
     112,   113,   113,   114,   114,   115,   115,   115,   115,   115,
     116,   117,   118,   118,   118,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   120,   121,   121,   121,
     121,   122,   122,   123,   123,   123,   123,   123,   123,   123,
     123,   124,   124,   125,   126,   126,   126,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   128,   128,   128,   129,   129,   129,   130,   130,   130,
     130,   131,   131,   131,   131,   131,   132,   132,   132,   133,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     7,     9,
       5,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       2,     5,     4,     3,     3,     7,     3,     7,     3,     3,
       2,     4,     7,     6,     4,     6,     4,     7,     6,     7,
       7,     7,     6,     6,     2,     1,     1,     1,     1,     2,
       4,     2,     1,     1,     1,     1,     1,     1,     1,     5,
       7,     7,     5,     5,     7,     5,     2,     7,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     7,     6,     7,
       6,     5,     0,     3,     3,     0,     4,     4,     4,     5,
       5,     4,     5,     5,     6,     6,     7,     7,     6,     6,
       6,     6,     5,     5,     6,     6,     7,     7,     7,     7,
       7,     6,     7,     6,     8,     7,     5,     0,     4,     4,
       4,     3,     5,     1,     2,     3,     3,     3,     4,     5,
       5,     3,     3,     3,     3,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     2,     2,     1,     1,
       4,     3,     6,     6,     5,     6,     5,     4,     4,     6,
       6,     5,     3,     2,     3,     3,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     5,     5,     1,     1,     1,     1,
       3
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
#line 1898 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 255 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1904 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 256 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1910 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 260 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1916 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 262 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1922 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 264 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1928 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 266 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1936 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 270 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1944 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 274 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1950 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 276 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1956 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 278 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1962 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 280 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1970 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 284 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1978 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 288 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1984 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 290 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1990 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 292 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1996 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 294 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2002 "raku.tab.c"
    break;

  case 19: /* stmt: KW_USE IDENT ';'  */
#line 296 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2008 "raku.tab.c"
    break;

  case 20: /* stmt: TESTOP ';'  */
#line 298 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2014 "raku.tab.c"
    break;

  case 21: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 300 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2020 "raku.tab.c"
    break;

  case 22: /* stmt: TESTOP '(' ')' ';'  */
#line 302 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2026 "raku.tab.c"
    break;

  case 23: /* stmt: TESTOP arg_list ';'  */
#line 304 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2032 "raku.tab.c"
    break;

  case 24: /* stmt: KW_SAY expr ';'  */
#line 306 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2038 "raku.tab.c"
    break;

  case 25: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 308 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2044 "raku.tab.c"
    break;

  case 26: /* stmt: KW_PRINT expr ';'  */
#line 310 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2050 "raku.tab.c"
    break;

  case 27: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 312 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2056 "raku.tab.c"
    break;

  case 28: /* stmt: KW_TAKE expr ';'  */
#line 314 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2062 "raku.tab.c"
    break;

  case 29: /* stmt: KW_RETURN expr ';'  */
#line 316 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2068 "raku.tab.c"
    break;

  case 30: /* stmt: KW_RETURN ';'  */
#line 318 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2074 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 320 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2080 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 322 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2089 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 327 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2097 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 331 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2105 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 335 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2114 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 340 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2122 "raku.tab.c"
    break;

  case 37: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 344 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2133 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 351 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2142 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 356 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2149 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 359 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2156 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 362 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2163 "raku.tab.c"
    break;

  case 42: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 365 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2170 "raku.tab.c"
    break;

  case 43: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 368 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2177 "raku.tab.c"
    break;

  case 44: /* stmt: expr ';'  */
#line 370 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2183 "raku.tab.c"
    break;

  case 45: /* stmt: if_stmt  */
#line 371 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2189 "raku.tab.c"
    break;

  case 46: /* stmt: while_stmt  */
#line 372 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2195 "raku.tab.c"
    break;

  case 47: /* stmt: for_stmt  */
#line 373 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2201 "raku.tab.c"
    break;

  case 48: /* stmt: given_stmt  */
#line 374 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2207 "raku.tab.c"
    break;

  case 49: /* stmt: KW_TRY block  */
#line 376 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2213 "raku.tab.c"
    break;

  case 50: /* stmt: KW_TRY block KW_CATCH block  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2219 "raku.tab.c"
    break;

  case 51: /* stmt: KW_CATCH block  */
#line 380 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2225 "raku.tab.c"
    break;

  case 52: /* stmt: unless_stmt  */
#line 381 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2231 "raku.tab.c"
    break;

  case 53: /* stmt: until_stmt  */
#line 382 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2237 "raku.tab.c"
    break;

  case 54: /* stmt: repeat_stmt  */
#line 383 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2243 "raku.tab.c"
    break;

  case 55: /* stmt: sub_decl  */
#line 384 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2249 "raku.tab.c"
    break;

  case 56: /* stmt: class_decl  */
#line 385 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2255 "raku.tab.c"
    break;

  case 57: /* stmt: role_decl  */
#line 386 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2261 "raku.tab.c"
    break;

  case 58: /* stmt: grammar_decl  */
#line 387 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2267 "raku.tab.c"
    break;

  case 59: /* if_stmt: KW_IF '(' expr ')' block  */
#line 391 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2273 "raku.tab.c"
    break;

  case 60: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 393 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2279 "raku.tab.c"
    break;

  case 61: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 395 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2285 "raku.tab.c"
    break;

  case 62: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 399 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2291 "raku.tab.c"
    break;

  case 63: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 403 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2297 "raku.tab.c"
    break;

  case 64: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 405 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2303 "raku.tab.c"
    break;

  case 65: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 409 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2309 "raku.tab.c"
    break;

  case 66: /* repeat_stmt: KW_REPEAT block  */
#line 413 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2315 "raku.tab.c"
    break;

  case 67: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 417 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2325 "raku.tab.c"
    break;

  case 68: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 423 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2335 "raku.tab.c"
    break;

  case 69: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 429 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2344 "raku.tab.c"
    break;

  case 70: /* for_stmt: KW_FOR expr block  */
#line 434 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2351 "raku.tab.c"
    break;

  case 71: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 439 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2363 "raku.tab.c"
    break;

  case 72: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 447 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2376 "raku.tab.c"
    break;

  case 73: /* when_list: %empty  */
#line 457 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2382 "raku.tab.c"
    break;

  case 74: /* when_list: when_list KW_WHEN expr block  */
#line 459 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2390 "raku.tab.c"
    break;

  case 75: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 465 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2402 "raku.tab.c"
    break;

  case 76: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 473 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2412 "raku.tab.c"
    break;

  case 77: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 479 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2425 "raku.tab.c"
    break;

  case 78: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 488 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2436 "raku.tab.c"
    break;

  case 79: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 495 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2449 "raku.tab.c"
    break;

  case 80: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 506 "raku.y"
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
#line 2467 "raku.tab.c"
    break;

  case 81: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 522 "raku.y"
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
#line 2484 "raku.tab.c"
    break;

  case 82: /* is_clauses: %empty  */
#line 536 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2490 "raku.tab.c"
    break;

  case 83: /* is_clauses: is_clauses IDENT IDENT  */
#line 538 "raku.y"
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
#line 2506 "raku.tab.c"
    break;

  case 84: /* is_clauses: is_clauses TESTOP IDENT  */
#line 550 "raku.y"
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
#line 2522 "raku.tab.c"
    break;

  case 85: /* class_body_list: %empty  */
#line 563 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2528 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 565 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2535 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 568 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2542 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 571 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2549 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 574 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2556 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 577 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2563 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 580 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2570 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 583 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2577 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 586 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2584 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 589 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2591 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 592 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2598 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 595 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2605 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 598 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2612 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 601 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2623 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 608 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2634 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 615 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2645 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 622 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2656 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 629 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2664 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 633 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2672 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 637 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2680 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 641 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2688 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 645 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2699 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 652 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2710 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 659 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2721 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 666 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2732 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 673 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2746 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 683 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2758 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 691 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2771 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 700 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2782 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 707 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2796 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 717 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2808 "raku.tab.c"
    break;

  case 116: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 727 "raku.y"
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
#line 2825 "raku.tab.c"
    break;

  case 117: /* grammar_body_list: %empty  */
#line 741 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2831 "raku.tab.c"
    break;

  case 118: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 743 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2840 "raku.tab.c"
    break;

  case 119: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 748 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2849 "raku.tab.c"
    break;

  case 120: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 753 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2858 "raku.tab.c"
    break;

  case 121: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 760 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2866 "raku.tab.c"
    break;

  case 122: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 764 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2874 "raku.tab.c"
    break;

  case 123: /* param_list: VAR_SCALAR  */
#line 769 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2880 "raku.tab.c"
    break;

  case 124: /* param_list: IDENT VAR_SCALAR  */
#line 770 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2886 "raku.tab.c"
    break;

  case 125: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 771 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2892 "raku.tab.c"
    break;

  case 126: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 772 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2898 "raku.tab.c"
    break;

  case 127: /* param_list: param_list ',' VAR_SCALAR  */
#line 773 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2904 "raku.tab.c"
    break;

  case 128: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 774 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2910 "raku.tab.c"
    break;

  case 129: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 775 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2916 "raku.tab.c"
    break;

  case 130: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 776 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2922 "raku.tab.c"
    break;

  case 131: /* block: '{' stmt_list '}'  */
#line 779 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2928 "raku.tab.c"
    break;

  case 132: /* block: '{' YADA '}'  */
#line 780 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2934 "raku.tab.c"
    break;

  case 133: /* closure: '{' expr '}'  */
#line 783 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2940 "raku.tab.c"
    break;

  case 134: /* expr: VAR_SCALAR '=' expr  */
#line 786 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2946 "raku.tab.c"
    break;

  case 135: /* expr: KW_GATHER block  */
#line 787 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2956 "raku.tab.c"
    break;

  case 136: /* expr: cmp_expr  */
#line 792 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2962 "raku.tab.c"
    break;

  case 137: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 795 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2968 "raku.tab.c"
    break;

  case 138: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 796 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2974 "raku.tab.c"
    break;

  case 139: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 797 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2980 "raku.tab.c"
    break;

  case 140: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 798 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2986 "raku.tab.c"
    break;

  case 141: /* cmp_expr: jct_expr '<' jct_expr  */
#line 799 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2992 "raku.tab.c"
    break;

  case 142: /* cmp_expr: jct_expr '>' jct_expr  */
#line 800 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2998 "raku.tab.c"
    break;

  case 143: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 801 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3004 "raku.tab.c"
    break;

  case 144: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 802 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3010 "raku.tab.c"
    break;

  case 145: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 803 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3016 "raku.tab.c"
    break;

  case 146: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 804 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3022 "raku.tab.c"
    break;

  case 147: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 806 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3032 "raku.tab.c"
    break;

  case 148: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 812 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3042 "raku.tab.c"
    break;

  case 149: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 818 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3052 "raku.tab.c"
    break;

  case 150: /* cmp_expr: jct_expr  */
#line 823 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3058 "raku.tab.c"
    break;

  case 151: /* jct_expr: jct_expr '|' range_expr  */
#line 826 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3064 "raku.tab.c"
    break;

  case 152: /* jct_expr: jct_expr '&' range_expr  */
#line 827 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3070 "raku.tab.c"
    break;

  case 153: /* jct_expr: range_expr  */
#line 828 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3076 "raku.tab.c"
    break;

  case 154: /* range_expr: add_expr OP_RANGE add_expr  */
#line 831 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3082 "raku.tab.c"
    break;

  case 155: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 832 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3088 "raku.tab.c"
    break;

  case 156: /* range_expr: add_expr  */
#line 833 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3094 "raku.tab.c"
    break;

  case 157: /* add_expr: add_expr '+' mul_expr  */
#line 836 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3100 "raku.tab.c"
    break;

  case 158: /* add_expr: add_expr '-' mul_expr  */
#line 837 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3106 "raku.tab.c"
    break;

  case 159: /* add_expr: add_expr '~' mul_expr  */
#line 838 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3112 "raku.tab.c"
    break;

  case 160: /* add_expr: mul_expr  */
#line 839 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 3118 "raku.tab.c"
    break;

  case 161: /* mul_expr: mul_expr '*' unary_expr  */
#line 842 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3124 "raku.tab.c"
    break;

  case 162: /* mul_expr: mul_expr '/' unary_expr  */
#line 843 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3130 "raku.tab.c"
    break;

  case 163: /* mul_expr: mul_expr '%' unary_expr  */
#line 844 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3136 "raku.tab.c"
    break;

  case 164: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 845 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3142 "raku.tab.c"
    break;

  case 165: /* mul_expr: unary_expr  */
#line 846 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3148 "raku.tab.c"
    break;

  case 166: /* unary_expr: '-' unary_expr  */
#line 849 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3154 "raku.tab.c"
    break;

  case 167: /* unary_expr: '!' unary_expr  */
#line 850 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3160 "raku.tab.c"
    break;

  case 168: /* unary_expr: postfix_expr  */
#line 851 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3166 "raku.tab.c"
    break;

  case 169: /* postfix_expr: call_expr  */
#line 853 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3172 "raku.tab.c"
    break;

  case 170: /* call_expr: IDENT '(' arg_list ')'  */
#line 856 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3181 "raku.tab.c"
    break;

  case 171: /* call_expr: IDENT '(' ')'  */
#line 860 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3187 "raku.tab.c"
    break;

  case 172: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 862 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3197 "raku.tab.c"
    break;

  case 173: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 868 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3207 "raku.tab.c"
    break;

  case 174: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 874 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3215 "raku.tab.c"
    break;

  case 175: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 878 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3226 "raku.tab.c"
    break;

  case 176: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 885 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3235 "raku.tab.c"
    break;

  case 177: /* call_expr: IDENT '.' CARET IDENT  */
#line 890 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3245 "raku.tab.c"
    break;

  case 178: /* call_expr: atom '.' CARET IDENT  */
#line 896 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3255 "raku.tab.c"
    break;

  case 179: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 902 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3266 "raku.tab.c"
    break;

  case 180: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 909 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3277 "raku.tab.c"
    break;

  case 181: /* call_expr: atom '.' IDENT '(' ')'  */
#line 916 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3286 "raku.tab.c"
    break;

  case 182: /* call_expr: atom '.' IDENT  */
#line 921 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3295 "raku.tab.c"
    break;

  case 183: /* call_expr: KW_DIE expr  */
#line 926 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3301 "raku.tab.c"
    break;

  case 184: /* call_expr: KW_MAP closure expr  */
#line 928 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3307 "raku.tab.c"
    break;

  case 185: /* call_expr: KW_GREP closure expr  */
#line 930 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3313 "raku.tab.c"
    break;

  case 186: /* call_expr: KW_SORT expr  */
#line 932 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3319 "raku.tab.c"
    break;

  case 187: /* call_expr: KW_SORT closure expr  */
#line 934 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3325 "raku.tab.c"
    break;

  case 188: /* call_expr: atom  */
#line 935 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3331 "raku.tab.c"
    break;

  case 189: /* arg_list: expr  */
#line 938 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3337 "raku.tab.c"
    break;

  case 190: /* arg_list: arg_list ',' expr  */
#line 939 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3343 "raku.tab.c"
    break;

  case 191: /* atom: LIT_INT  */
#line 942 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3349 "raku.tab.c"
    break;

  case 192: /* atom: LIT_FLOAT  */
#line 943 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3355 "raku.tab.c"
    break;

  case 193: /* atom: LIT_STR  */
#line 944 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3361 "raku.tab.c"
    break;

  case 194: /* atom: LIT_INTERP_STR  */
#line 945 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3367 "raku.tab.c"
    break;

  case 195: /* atom: VAR_SCALAR  */
#line 946 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3373 "raku.tab.c"
    break;

  case 196: /* atom: VAR_ARRAY  */
#line 947 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3379 "raku.tab.c"
    break;

  case 197: /* atom: VAR_HASH  */
#line 948 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3385 "raku.tab.c"
    break;

  case 198: /* atom: VAR_CAPTURE  */
#line 950 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3393 "raku.tab.c"
    break;

  case 199: /* atom: VAR_FH  */
#line 954 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3401 "raku.tab.c"
    break;

  case 200: /* atom: VAR_NAMED_CAPTURE  */
#line 958 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3408 "raku.tab.c"
    break;

  case 201: /* atom: VAR_ARRAY '[' expr ']'  */
#line 961 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3414 "raku.tab.c"
    break;

  case 202: /* atom: VAR_HASH '<' IDENT '>'  */
#line 963 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3420 "raku.tab.c"
    break;

  case 203: /* atom: VAR_HASH '{' expr '}'  */
#line 965 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3426 "raku.tab.c"
    break;

  case 204: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 967 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3432 "raku.tab.c"
    break;

  case 205: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 969 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3438 "raku.tab.c"
    break;

  case 206: /* atom: IDENT  */
#line 970 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3444 "raku.tab.c"
    break;

  case 207: /* atom: VAR_TWIGIL  */
#line 972 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3452 "raku.tab.c"
    break;

  case 208: /* atom: VAR_ARRAY_TWIGIL  */
#line 976 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3460 "raku.tab.c"
    break;

  case 209: /* atom: VAR_HASH_TWIGIL  */
#line 980 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3468 "raku.tab.c"
    break;

  case 210: /* atom: '(' expr ')'  */
#line 983 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3474 "raku.tab.c"
    break;


#line 3478 "raku.tab.c"

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

#line 985 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
