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
  YYSYMBOL_WORDLIST = 58,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 59,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 60,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 61,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 62,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 63,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 64,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 65,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 66,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 67,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 68,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 69,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 70,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 71,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 72,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 73,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 74,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 75,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 76,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 77,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 78,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 79,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 80,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 81,                    /* OP_DIV  */
  YYSYMBOL_82_ = 82,                       /* '='  */
  YYSYMBOL_83_ = 83,                       /* '!'  */
  YYSYMBOL_84_ = 84,                       /* '<'  */
  YYSYMBOL_85_ = 85,                       /* '>'  */
  YYSYMBOL_86_ = 86,                       /* '|'  */
  YYSYMBOL_87_ = 87,                       /* '&'  */
  YYSYMBOL_88_ = 88,                       /* '~'  */
  YYSYMBOL_89_ = 89,                       /* '+'  */
  YYSYMBOL_90_ = 90,                       /* '-'  */
  YYSYMBOL_91_ = 91,                       /* '*'  */
  YYSYMBOL_92_ = 92,                       /* '/'  */
  YYSYMBOL_93_ = 93,                       /* '%'  */
  YYSYMBOL_UMINUS = 94,                    /* UMINUS  */
  YYSYMBOL_95_ = 95,                       /* '.'  */
  YYSYMBOL_96_ = 96,                       /* ';'  */
  YYSYMBOL_97_ = 97,                       /* ','  */
  YYSYMBOL_98_ = 98,                       /* '('  */
  YYSYMBOL_99_ = 99,                       /* ')'  */
  YYSYMBOL_100_ = 100,                     /* '['  */
  YYSYMBOL_101_ = 101,                     /* ']'  */
  YYSYMBOL_102_ = 102,                     /* '{'  */
  YYSYMBOL_103_ = 103,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 104,                 /* $accept  */
  YYSYMBOL_program = 105,                  /* program  */
  YYSYMBOL_stmt_list = 106,                /* stmt_list  */
  YYSYMBOL_stmt = 107,                     /* stmt  */
  YYSYMBOL_if_stmt = 108,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 109,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 110,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 111,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 112,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 113,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 114,               /* given_stmt  */
  YYSYMBOL_when_list = 115,                /* when_list  */
  YYSYMBOL_sub_decl = 116,                 /* sub_decl  */
  YYSYMBOL_class_decl = 117,               /* class_decl  */
  YYSYMBOL_role_decl = 118,                /* role_decl  */
  YYSYMBOL_is_clauses = 119,               /* is_clauses  */
  YYSYMBOL_class_body_list = 120,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 121,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 122,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 123,           /* named_arg_list  */
  YYSYMBOL_param_list = 124,               /* param_list  */
  YYSYMBOL_block = 125,                    /* block  */
  YYSYMBOL_closure = 126,                  /* closure  */
  YYSYMBOL_expr = 127,                     /* expr  */
  YYSYMBOL_cmp_expr = 128,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 129,                 /* jct_expr  */
  YYSYMBOL_range_expr = 130,               /* range_expr  */
  YYSYMBOL_add_expr = 131,                 /* add_expr  */
  YYSYMBOL_mul_expr = 132,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 133,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 134,             /* postfix_expr  */
  YYSYMBOL_call_expr = 135,                /* call_expr  */
  YYSYMBOL_arg_list = 136,                 /* arg_list  */
  YYSYMBOL_atom = 137                      /* atom  */
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
#define YYLAST   1564

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  218
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  545

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   337


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
       2,     2,     2,    83,     2,     2,     2,    93,    87,     2,
      98,    99,    91,    89,    97,    90,    95,    92,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    96,
      84,    82,    85,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   100,     2,   101,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   102,    86,   103,    88,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    94
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   245,   245,   256,   257,   260,   262,   264,   266,   268,
     270,   274,   278,   280,   282,   284,   288,   292,   294,   296,
     298,   300,   302,   304,   306,   308,   310,   312,   314,   316,
     318,   320,   322,   324,   326,   331,   335,   339,   344,   348,
     355,   360,   363,   366,   369,   372,   375,   376,   377,   378,
     379,   380,   382,   384,   386,   388,   389,   390,   391,   392,
     393,   394,   397,   399,   401,   405,   409,   411,   415,   419,
     423,   429,   435,   440,   445,   453,   464,   465,   471,   479,
     485,   494,   501,   512,   528,   543,   544,   556,   570,   571,
     574,   577,   580,   583,   586,   589,   592,   595,   598,   601,
     604,   607,   614,   621,   628,   635,   639,   643,   647,   651,
     658,   665,   672,   679,   689,   697,   706,   713,   723,   733,
     748,   749,   754,   759,   766,   770,   776,   777,   778,   779,
     780,   781,   782,   783,   786,   787,   790,   793,   794,   799,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   811,
     812,   818,   824,   830,   833,   834,   835,   838,   839,   840,
     843,   844,   845,   846,   849,   850,   851,   852,   853,   856,
     857,   858,   860,   862,   867,   868,   873,   875,   881,   887,
     891,   898,   903,   909,   915,   922,   929,   934,   939,   941,
     943,   945,   947,   949,   952,   953,   956,   957,   958,   959,
     967,   968,   969,   970,   971,   975,   979,   982,   984,   986,
     988,   990,   992,   993,   997,  1001,  1005,  1006,  1007
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
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND",
  "OP_OR", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_DIV", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "while_stmt",
  "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt", "given_stmt",
  "when_list", "sub_decl", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "param_list", "block", "closure", "expr", "cmp_expr", "jct_expr",
  "range_expr", "add_expr", "mul_expr", "unary_expr", "postfix_expr",
  "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-305)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-218)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -305,    18,   426,  -305,  -305,  -305,  -305,  -305,    99,   -30,
     -57,    11,    33,  -305,  -305,  -305,  -305,  -305,    68,   236,
     259,  1147,  1203,     2,    64,  1252,   -13,    72,  1252,   593,
    1252,   154,   180,   133,   174,    72,   151,   151,  1308,    72,
      72,  1252,   264,   281,   286,  -305,   283,  1462,  1462,  1252,
     221,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,   302,   225,   119,   148,  -305,    65,   109,
    -305,  -305,  -305,   229,   319,  1252,   322,   642,  1252,   335,
    1252,  1252,    69,   704,   260,   -36,   267,   -31,  -305,    72,
    -305,   753,  -305,  -305,   129,   -23,    86,    90,   198,  1252,
     275,  1252,   285,  1252,  1252,   -25,    71,   294,  -305,  -305,
     300,  -305,   303,   305,    10,    39,  1252,  1252,  -305,  1252,
    1252,  1252,   815,  1252,  -305,   315,  -305,  -305,  -305,   314,
       1,   316,   313,  -305,  -305,   318,   317,   532,  -305,  1462,
    1462,  1462,  1462,  1462,  1462,  1462,  1462,   268,  1462,  1462,
    1462,  1462,  1462,  1462,  1462,  1462,  1462,  1462,  1462,  1462,
    1462,   150,   -35,   323,    75,  -305,   116,   320,   337,   324,
     329,   328,   414,   336,  -305,   160,  -305,  1252,  1252,   419,
    1252,   339,   318,   194,  -305,  1252,  1252,  -305,  1357,  -305,
    1252,  -305,   102,   103,   111,   199,  -305,   204,  -305,   344,
     354,   444,  -305,  1462,  1462,    -3,  -305,  -305,  -305,   448,
    1252,   449,  1252,   377,   378,   375,  -305,  -305,  -305,    72,
       7,  -305,   383,   384,  -305,  -305,  -305,  -305,   179,   179,
     179,   179,   179,   179,   179,   179,  -305,  -305,  -305,   179,
     179,  -305,  -305,   155,   155,   109,   109,   109,  -305,  -305,
    -305,  -305,   389,   475,  -305,   876,  -305,  1252,   925,  -305,
     408,   418,   421,  -305,   987,  -305,  1036,  -305,  -305,   403,
     422,   405,  -305,   410,  -305,   415,  1252,   247,   417,  1252,
    -305,  1413,  -305,  1252,  -305,  1252,  1252,    72,    72,    72,
      81,    87,  -305,    20,    72,   207,    -5,   425,   412,   432,
     416,    72,    72,  -305,  -305,   504,   506,  -305,   -11,     0,
      58,   -29,  1098,  -305,   427,   211,   429,   431,   216,  1252,
    1252,  1252,  -305,   217,   -17,  -305,   223,   228,  -305,  -305,
    -305,  -305,  -305,   231,  -305,  1252,  -305,   434,  1252,   288,
     436,   423,   435,   494,  -305,  -305,   523,   529,  -305,   530,
     539,  -305,   112,    72,  1252,    72,  -305,  -305,  -305,   463,
     464,   541,  -305,  -305,  -305,    15,     6,   248,   519,  -305,
      72,   232,   238,   568,   569,   570,  -305,  -305,   242,   243,
    -305,   491,  -305,  -305,   492,   493,   495,   496,  -305,  1252,
     579,  -305,  -305,  1252,   307,  -305,   251,  -305,  1252,  -305,
     499,   505,   -14,    72,    72,  -305,  -305,  -305,    31,  -305,
      72,   497,  -305,  -305,    72,  -305,   512,   516,    28,    38,
     214,   520,   521,   588,  -305,    72,    72,   611,   612,   613,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,   555,   255,
    -305,  1252,   309,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
     616,   617,  -305,  -305,  -305,     3,     9,   609,   614,   533,
    1252,  -305,   618,   619,   543,  1252,  -305,    62,    83,   544,
     546,  -305,  -305,   545,  -305,  -305,  -305,  -305,  -305,  1252,
     548,   258,  -305,  -305,  -305,    72,   261,    72,   262,   553,
     554,  -305,   563,   567,   571,  -305,   572,   650,   651,   573,
    1252,  -305,   652,   656,   575,  1252,  -305,  -305,  -305,    12,
    -305,  -305,   576,  -305,    72,  -305,    72,  -305,  -305,  -305,
    -305,  -305,  -305,   581,   582,  -305,   583,   585,   586,  -305,
     592,    72,   266,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,    72,  -305
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   196,   197,   198,   200,   201,   202,
     203,   213,   212,   214,   215,   204,   205,   206,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   199,     0,     0,     0,     0,
       3,     4,    47,    48,    55,    56,    57,    49,    50,    58,
      59,    60,    61,    54,     0,   139,   153,   156,   159,   163,
     168,   171,   172,   193,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   201,   202,   203,   213,     0,
      22,     0,   217,   194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,     0,   218,   138,
       0,    32,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     3,     0,   191,    51,    53,   188,    85,     0,
       0,     0,   201,   170,   169,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,   174,     0,    21,     0,     0,     0,
       0,     0,   194,     0,    25,     0,     0,     6,     0,     7,
       0,     8,     0,     0,     0,     0,    26,     0,    28,     0,
       0,     0,    73,     0,     0,     0,    30,    31,    76,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   192,     0,
       0,    88,     0,     0,   120,   216,   135,   134,   140,   141,
     142,   143,   146,   147,   148,   149,   150,   151,   152,   144,
     145,   154,   155,   157,   158,   162,   160,   161,   167,   164,
     165,   166,   187,     0,    36,     0,    33,     0,     0,   175,
     207,   208,   209,    38,     0,   182,     0,   173,   137,     0,
       0,     0,    24,     0,   195,     0,     0,     0,     0,     0,
      18,     0,    19,     0,    20,     0,     0,     0,     0,     0,
     157,   158,   126,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   136,    52,     0,     0,    88,     0,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   212,   179,     0,     0,   207,   208,
     209,    23,     5,     0,     9,     0,    12,     0,     0,     0,
       0,     0,     0,    62,    65,    72,     0,     0,   127,     0,
       0,    79,     0,     0,     0,     0,    74,   210,   211,     0,
       0,    66,    68,    86,    87,     0,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,   119,   186,     0,     0,
      35,     0,    37,    40,     0,     0,     0,     0,   180,     0,
       0,   177,   178,     0,     0,    13,     0,    14,     0,    17,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    78,
       0,     0,    44,    45,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
     185,   184,    34,    39,    41,    42,    43,   124,     0,     0,
      10,     0,     0,    27,    29,    64,    63,    70,    71,   131,
       0,     0,    77,    75,    67,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,    89,     0,     0,     0,
       0,    90,    91,     0,    80,    82,   121,   122,   123,     0,
       0,     0,    15,   132,   133,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,   106,     0,     0,     0,     0,
       0,    96,     0,     0,     0,     0,    95,    92,    93,     0,
     125,    11,     0,   114,     0,   116,     0,   103,   104,    98,
     101,   102,    97,     0,     0,   107,     0,     0,     0,   108,
       0,     0,     0,    16,   113,   115,   111,   112,   100,   109,
     110,    99,   118,     0,   117
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -305,  -305,   675,  -305,   282,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,   385,  -305,  -305,   381,
    -304,    -2,   372,    26,  -305,   353,   263,   -16,   134,   -44,
    -305,  -305,   -75,  -305
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   137,    51,    52,    53,    54,    55,    56,    57,
      58,   296,    59,    60,    61,   220,   308,    62,   311,   326,
     295,    92,   120,    93,    65,    66,    67,    68,    69,    70,
      71,    72,    94,    73
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,   107,   166,   133,   134,   371,   372,   292,   175,   106,
     292,   293,    23,   292,   293,   222,   183,   293,     3,   292,
     416,   305,   292,   293,   108,   109,   293,    79,    64,   306,
     348,   354,   355,   118,   373,   374,   375,   125,   126,   366,
     367,   449,   457,   368,   201,    80,   177,   100,   102,   389,
     458,   105,   462,   179,   110,   112,   113,   223,   417,   186,
     463,   254,    77,   255,   124,   366,   367,   127,   292,   368,
      78,   180,   293,   187,   376,   135,   497,    50,    82,   349,
     350,    83,    84,   171,   498,   459,   172,   108,    50,    50,
     450,   451,   369,    81,   209,   464,   294,   502,   356,   370,
     103,   163,   485,   202,   167,   503,   169,   170,   487,   307,
     460,   531,   210,   248,   249,   250,   251,   182,   415,   499,
     465,   173,   407,   211,   461,   195,   408,   197,    82,   199,
     200,    83,   152,   153,   466,    63,   243,   244,   203,   204,
     504,   212,   213,   214,   500,   215,   216,   217,   215,   218,
     346,   486,   488,   154,   155,   156,   347,   257,   501,   154,
     155,   156,   104,    64,   252,   505,   114,   253,   188,   154,
     155,   156,   190,   258,    50,   154,   155,   156,    74,   506,
     315,    75,   189,   318,   279,   281,   191,   290,   291,   323,
     157,   327,   115,   283,    76,   139,   140,    77,   280,   282,
     158,   159,   160,   268,   269,   532,   271,   284,   192,   193,
     194,   274,   275,   185,   277,   259,   278,   304,   141,   142,
     143,   144,   145,   146,   467,   184,   185,   468,   147,   469,
     470,   116,   148,   149,   150,   151,   298,   379,   300,     4,
       5,     6,     7,   154,   155,   156,    85,    86,    87,    88,
      12,    13,    14,   119,    15,    16,    17,   185,   418,   267,
     394,   419,   420,   421,   422,   150,   151,    89,    27,    95,
      96,    97,   117,    98,    31,   236,   237,   238,   128,    36,
      37,    38,   136,   316,    41,   343,   344,   345,   245,   246,
     247,   185,   351,   273,    45,   129,   285,   131,   225,   361,
     362,   286,   333,   225,   352,   337,   353,   339,   185,   340,
     381,   341,   342,   185,   185,   384,   388,   130,   439,    47,
     390,   138,   391,   442,   161,   185,    48,   392,   393,   352,
     225,   425,    90,   162,    91,   352,   164,   426,    50,   390,
     185,   430,   431,   334,   335,   385,   386,   387,   441,   168,
     225,   409,   185,   411,   480,   185,   176,   512,   352,   352,
     514,   516,   219,   352,   396,   543,   481,   178,   424,  -217,
    -217,   196,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
     410,   198,  -217,  -217,   397,   398,  -217,  -217,  -217,  -217,
    -217,  -217,   205,  -217,  -217,  -217,   206,  -217,  -217,   207,
     446,   447,   448,   440,   185,   482,   185,   208,   452,   121,
     123,    77,   454,   241,   242,   437,   221,   225,   224,   256,
     226,   260,   261,   474,   475,   263,   264,   262,   265,     4,
       5,     6,     7,   270,   266,   272,     8,     9,    10,    11,
      12,    13,    14,   287,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   288,   289,    24,    25,    26,    27,    28,
      29,    30,   297,   299,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,   301,   302,   303,    43,
      44,   309,   310,   513,    45,   515,   492,   312,    46,   313,
     319,   496,   228,   229,   230,   231,   232,   233,   234,   235,
     320,   239,   240,   321,   328,   510,   331,   329,   330,    47,
     357,   332,   534,   336,   535,   358,    48,   359,   363,   360,
     364,   402,   400,   380,    49,   382,   526,   383,    50,   542,
     395,   530,   399,   403,   401,     4,     5,     6,     7,   404,
     405,   544,     8,     9,    10,    11,    12,    13,    14,   406,
      15,    16,    17,    18,    19,    20,    21,    22,    23,   412,
     413,    24,    25,    26,    27,    28,    29,    30,   414,   423,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,   427,   428,   429,    43,    44,   432,   433,   434,
      45,   435,   436,   438,    46,   443,     4,     5,     6,     7,
     453,   444,   473,    85,    86,    87,    88,    12,    13,    14,
     455,    15,    16,    17,   456,    47,   471,   472,   476,   477,
     478,   479,    48,   489,    89,    27,   483,   484,   490,   491,
      49,    31,   493,   494,    50,   227,    36,    37,    38,   495,
     507,    41,   508,   509,   511,     4,     5,     6,     7,   517,
     518,    45,    85,    86,    87,    88,    12,    13,    14,   519,
      15,    16,    17,   520,   523,   524,   527,   521,   522,   525,
     528,   529,   533,    89,    27,     2,    47,   536,   537,   538,
      31,   539,   540,    48,   445,    36,    37,    38,   541,   111,
      41,    49,   365,   378,     0,    50,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    85,    86,    87,    88,    12,    13,
      14,     0,    15,    16,    17,    47,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    89,    27,     0,     0,     0,
      49,   165,    31,     0,    50,     0,     0,    36,    37,    38,
       0,     0,    41,     0,     0,     0,     4,     5,     6,     7,
       0,     0,    45,    85,    86,    87,    88,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    27,     0,    47,     0,     0,
       0,    31,     0,     0,    48,     0,    36,    37,    38,     0,
       0,    41,    49,   174,     0,     0,    50,     0,     0,     0,
       0,    45,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    85,    86,    87,    88,    12,
      13,    14,     0,    15,    16,    17,    47,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    89,    27,     0,     0,
       0,    49,   181,    31,     0,    50,     0,     0,    36,    37,
      38,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,     0,     0,   136,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    85,    86,    87,    88,
      12,    13,    14,     0,    15,    16,    17,     0,    47,     0,
       0,     0,     0,     0,     0,    48,     0,    89,    27,     0,
       0,     0,     0,    49,    31,     0,     0,    50,     0,    36,
      37,    38,     0,     0,    41,     0,     0,     0,     4,     5,
       6,     7,     0,     0,    45,    85,    86,    87,    88,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    27,     0,    47,
       0,     0,     0,    31,     0,     0,    48,     0,    36,    37,
      38,     0,     0,    41,    49,   314,     0,     0,    50,     0,
       0,     0,     0,    45,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    85,    86,    87,
      88,    12,    13,    14,     0,    15,    16,    17,    47,     0,
       0,     0,     0,     0,     0,    48,     0,     0,    89,    27,
       0,     0,     0,    49,   317,    31,     0,    50,     0,     0,
      36,    37,    38,     0,     0,    41,     0,     0,     0,     4,
       5,     6,     7,     0,     0,    45,    85,    86,    87,    88,
     324,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    27,     0,
      47,     0,     0,     0,    31,     0,     0,    48,     0,    36,
      37,    38,     0,     0,    41,    49,   322,     0,     0,    50,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    85,    86,
      87,    88,   324,    13,    14,     0,    15,    16,    17,    47,
       0,     0,     0,     0,     0,     0,    48,     0,     0,    89,
      27,     0,     0,     0,    49,   325,    31,     0,    50,     0,
       0,    36,    37,    38,     0,     0,    41,     0,     0,     0,
       4,     5,     6,     7,     0,     0,    45,    85,    86,    87,
      88,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    27,
       0,    47,     0,     0,     0,    31,     0,     0,    48,     0,
      36,    37,    38,     0,     0,    41,    49,   377,     0,     0,
      50,     0,     0,     0,     0,    45,     4,     5,     6,     7,
       0,     0,     0,    85,    86,    87,    88,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
      47,     0,     0,     0,    89,    27,     0,    48,     0,     0,
       0,    31,     0,     0,     0,    99,    36,    37,    38,    50,
       0,    41,     0,     0,     0,     4,     5,     6,     7,     0,
       0,    45,    85,    86,    87,    88,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    27,     0,    47,     0,     0,     0,
      31,     0,     0,    48,     0,    36,    37,    38,     0,     0,
      41,   101,     0,     0,     0,    50,     0,     0,     0,     0,
      45,     4,     5,     6,     7,     0,     0,     0,    85,    86,
      87,    88,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,    89,
      27,     0,    48,     0,     0,     0,    31,     0,     0,     0,
      49,    36,    37,    38,    50,     0,    41,     0,     0,     0,
       4,     5,     6,     7,     0,     0,    45,    85,    86,    87,
      88,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    27,
       0,    47,     0,     0,     0,    31,     0,     0,    48,     0,
      36,    37,    38,     0,     0,    41,    49,     0,     0,     0,
     122,     0,     0,     0,     0,    45,     4,     5,     6,     7,
       0,     0,     0,    85,    86,    87,    88,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
      47,     0,     0,     0,    89,    27,     0,    48,     0,     0,
       0,    31,     0,     0,     0,   276,    36,    37,    38,    50,
       0,    41,     0,     0,     0,     4,     5,     6,     7,     0,
       0,    45,   132,    86,    87,    88,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,    47,     0,     0,     0,
      31,     0,     0,    48,     0,    36,    37,    38,     0,     0,
      41,   338,     0,     0,     0,    50,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,    48,     0,     0,     0,     0,     0,     0,     0,
      49,     0,     0,     0,    50
};

static const yytype_int16 yycheck[] =
{
       2,    14,    77,    47,    48,   309,   310,    10,    83,    25,
      10,    14,    26,    10,    14,    14,    91,    14,     0,    10,
      14,    14,    10,    14,    26,    27,    14,    84,     2,    22,
      10,    36,    37,    35,    63,    64,    65,    39,    40,    50,
      51,    10,    14,    54,    69,   102,    82,    21,    22,    66,
      22,    25,    14,    84,    28,    29,    30,    56,    52,    82,
      22,    96,    98,    98,    38,    50,    51,    41,    10,    54,
     100,   102,    14,    96,   103,    49,    14,   102,    95,    59,
      60,    98,    14,    14,    22,    57,    17,    89,   102,   102,
      59,    60,   103,    82,    84,    57,    99,    14,   103,    99,
      98,    75,    99,   105,    78,    22,    80,    81,    99,   102,
      82,    99,   102,   157,   158,   159,   160,    91,   103,    57,
      82,    52,    10,    84,    96,    99,    14,   101,    95,   103,
     104,    98,    67,    68,    96,   137,   152,   153,    67,    68,
      57,   102,   116,   117,    82,   119,   120,   121,   122,   123,
      69,   455,   456,    88,    89,    90,    69,    82,    96,    88,
      89,    90,    98,   137,    14,    82,    12,    17,    82,    88,
      89,    90,    82,    98,   102,    88,    89,    90,    79,    96,
     255,    82,    96,   258,    82,    82,    96,   203,   204,   264,
      81,   266,    12,    82,    95,    76,    77,    98,    96,    96,
      91,    92,    93,   177,   178,   509,   180,    96,    10,    11,
      12,   185,   186,    97,   188,    99,   190,   219,    70,    71,
      72,    73,    74,    75,    10,    96,    97,    13,    80,    15,
      16,    98,    84,    85,    86,    87,   210,   312,   212,     3,
       4,     5,     6,    88,    89,    90,    10,    11,    12,    13,
      14,    15,    16,   102,    18,    19,    20,    97,    10,    99,
     335,    13,    14,    15,    16,    86,    87,    31,    32,    10,
      11,    12,    98,    14,    38,     7,     8,     9,    14,    43,
      44,    45,    61,   257,    48,   287,   288,   289,   154,   155,
     156,    97,   294,    99,    58,    14,    97,    14,    99,   301,
     302,    97,   276,    99,    97,   279,    99,   281,    97,   283,
      99,   285,   286,    97,    97,    99,    99,    31,   393,    83,
      97,    96,    99,   398,    95,    97,    90,    99,    97,    97,
      99,    99,    96,    14,    98,    97,    14,    99,   102,    97,
      97,    99,    99,    96,    97,   319,   320,   321,    97,    14,
      99,   353,    97,   355,    99,    97,    96,    99,    97,    97,
      99,    99,    47,    97,   338,    99,   441,   100,   370,    67,
      68,    96,    70,    71,    72,    73,    74,    75,    76,    77,
     354,    96,    80,    81,    96,    97,    84,    85,    86,    87,
      88,    89,    98,    91,    92,    93,    96,    95,    96,    96,
     402,   403,   404,    96,    97,    96,    97,   102,   410,    37,
      38,    98,   414,   150,   151,   389,   102,    99,   102,    96,
     103,   101,    85,   425,   426,    96,    98,   103,    14,     3,
       4,     5,     6,    14,    98,    96,    10,    11,    12,    13,
      14,    15,    16,    99,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    99,    10,    29,    30,    31,    32,    33,
      34,    35,    14,    14,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    99,    99,   103,    53,
      54,    98,    98,   485,    58,   487,   460,    98,    62,    14,
      82,   465,   139,   140,   141,   142,   143,   144,   145,   146,
      82,   148,   149,    82,   101,   479,    96,    85,   103,    83,
      85,    96,   514,    96,   516,   103,    90,    85,    14,   103,
      14,    27,    99,    96,    98,    96,   500,    96,   102,   531,
      96,   505,    96,    10,    99,     3,     4,     5,     6,    10,
      10,   543,    10,    11,    12,    13,    14,    15,    16,    10,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    96,
      96,    29,    30,    31,    32,    33,    34,    35,    27,    50,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    14,    14,    14,    53,    54,    96,    96,    96,
      58,    96,    96,    14,    62,    96,     3,     4,     5,     6,
     103,    96,    14,    10,    11,    12,    13,    14,    15,    16,
      98,    18,    19,    20,    98,    83,    96,    96,     7,     7,
       7,    66,    90,    14,    31,    32,    10,    10,    14,    96,
      98,    38,    14,    14,   102,   103,    43,    44,    45,    96,
      96,    48,    96,    98,    96,     3,     4,     5,     6,    96,
      96,    58,    10,    11,    12,    13,    14,    15,    16,    96,
      18,    19,    20,    96,    14,    14,    14,    96,    96,    96,
      14,    96,    96,    31,    32,     0,    83,    96,    96,    96,
      38,    96,    96,    90,   402,    43,    44,    45,    96,    96,
      48,    98,   307,   312,    -1,   102,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    31,    32,    -1,    -1,    -1,
      98,    99,    38,    -1,   102,    -1,    -1,    43,    44,    45,
      -1,    -1,    48,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    58,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    83,    -1,    -1,
      -1,    38,    -1,    -1,    90,    -1,    43,    44,    45,    -1,
      -1,    48,    98,    99,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    31,    32,    -1,    -1,
      -1,    98,    99,    38,    -1,   102,    -1,    -1,    43,    44,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    61,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    31,    32,    -1,
      -1,    -1,    -1,    98,    38,    -1,    -1,   102,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    58,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    83,
      -1,    -1,    -1,    38,    -1,    -1,    90,    -1,    43,    44,
      45,    -1,    -1,    48,    98,    99,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    31,    32,
      -1,    -1,    -1,    98,    99,    38,    -1,   102,    -1,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    58,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      83,    -1,    -1,    -1,    38,    -1,    -1,    90,    -1,    43,
      44,    45,    -1,    -1,    48,    98,    99,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    31,
      32,    -1,    -1,    -1,    98,    99,    38,    -1,   102,    -1,
      -1,    43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    58,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    83,    -1,    -1,    -1,    38,    -1,    -1,    90,    -1,
      43,    44,    45,    -1,    -1,    48,    98,    99,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    58,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    31,    32,    -1,    90,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    98,    43,    44,    45,   102,
      -1,    48,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    58,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    83,    -1,    -1,    -1,
      38,    -1,    -1,    90,    -1,    43,    44,    45,    -1,    -1,
      48,    98,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      58,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    31,
      32,    -1,    90,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      98,    43,    44,    45,   102,    -1,    48,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    58,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    83,    -1,    -1,    -1,    38,    -1,    -1,    90,    -1,
      43,    44,    45,    -1,    -1,    48,    98,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    58,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    31,    32,    -1,    90,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    98,    43,    44,    45,   102,
      -1,    48,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    58,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    83,    -1,    -1,    -1,
      38,    -1,    -1,    90,    -1,    43,    44,    45,    -1,    -1,
      48,    98,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,   102
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   105,   106,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    53,    54,    58,    62,    83,    90,    98,
     102,   107,   108,   109,   110,   111,   112,   113,   114,   116,
     117,   118,   121,   125,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   137,    79,    82,    95,    98,   100,    84,
     102,    82,    95,    98,    14,    10,    11,    12,    13,    31,
      96,    98,   125,   127,   136,    10,    11,    12,    14,    98,
     127,    98,   127,    98,    98,   127,   131,    14,   125,   125,
     127,    96,   127,   127,    12,    12,    98,    98,   125,   102,
     126,   126,   102,   126,   127,   125,   125,   127,    14,    14,
      31,    14,    10,   133,   133,   127,    61,   106,    96,    76,
      77,    70,    71,    72,    73,    74,    75,    80,    84,    85,
      86,    87,    67,    68,    88,    89,    90,    81,    91,    92,
      93,    95,    14,   127,    14,    99,   136,   127,    14,   127,
     127,    14,    17,    52,    99,   136,    96,    82,   100,    84,
     102,    99,   127,   136,    96,    97,    82,    96,    82,    96,
      82,    96,    10,    11,    12,   127,    96,   127,    96,   127,
     127,    69,   125,    67,    68,    98,    96,    96,   102,    84,
     102,    84,   102,   127,   127,   127,   127,   127,   127,    47,
     119,   102,    14,    56,   102,    99,   103,   103,   129,   129,
     129,   129,   129,   129,   129,   129,     7,     8,     9,   129,
     129,   130,   130,   131,   131,   132,   132,   132,   133,   133,
     133,   133,    14,    17,    96,    98,    96,    82,    98,    99,
     101,    85,   103,    96,    98,    14,    98,    99,   127,   127,
      14,   127,    96,    99,   127,   127,    98,   127,   127,    82,
      96,    82,    96,    82,    96,    97,    97,    99,    99,    10,
     131,   131,    10,    14,    99,   124,   115,    14,   127,    14,
     127,    99,    99,   103,   125,    14,    22,   102,   120,    98,
      98,   122,    98,    14,    99,   136,   127,    99,   136,    82,
      82,    82,    99,   136,    14,    99,   123,   136,   101,    85,
     103,    96,    96,   127,    96,    97,    96,   127,    98,   127,
     127,   127,   127,   125,   125,   125,    69,    69,    10,    59,
      60,   125,    97,    99,    36,    37,   103,    85,   103,    85,
     103,   125,   125,    14,    14,   120,    50,    51,    54,   103,
      99,   124,   124,    63,    64,    65,   103,    99,   123,   136,
      96,    99,    96,    96,    99,   127,   127,   127,    99,    66,
      97,    99,    99,    97,   136,    96,   127,    96,    97,    96,
      99,    99,    27,    10,    10,    10,    10,    10,    14,   125,
     127,   125,    96,    96,    27,   103,    14,    52,    10,    13,
      14,    15,    16,    50,   125,    99,    99,    14,    14,    14,
      99,    99,    96,    96,    96,    96,    96,   127,    14,   136,
      96,    97,   136,    96,    96,   108,   125,   125,   125,    10,
      59,    60,   125,   103,   125,    98,    98,    14,    22,    57,
      82,    96,    14,    22,    57,    82,    96,    10,    13,    15,
      16,    96,    96,    14,   125,   125,     7,     7,     7,    66,
      99,   136,    96,    10,    10,    99,   124,    99,   124,    14,
      14,    96,   127,    14,    14,    96,   127,    14,    22,    57,
      82,    96,    14,    22,    57,    82,    96,    96,    96,    98,
     127,    96,    99,   125,    99,   125,    99,    96,    96,    96,
      96,    96,    96,    14,    14,    96,   127,    14,    14,    96,
     127,    99,   124,    96,   125,   125,    96,    96,    96,    96,
      96,    96,   125,    99,   125
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   104,   105,   106,   106,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   108,   109,   110,   110,   111,   112,
     113,   113,   113,   113,   114,   114,   115,   115,   116,   116,
     116,   116,   116,   117,   118,   119,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   121,
     122,   122,   122,   122,   123,   123,   124,   124,   124,   124,
     124,   124,   124,   124,   125,   125,   126,   127,   127,   127,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   129,   129,   129,   130,   130,   130,
     131,   131,   131,   131,   132,   132,   132,   132,   132,   133,
     133,   133,   134,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   136,   136,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     9,     5,     6,     6,     8,    10,     6,     4,     4,
       4,     3,     2,     5,     4,     3,     3,     7,     3,     7,
       3,     3,     2,     4,     7,     6,     4,     6,     4,     7,
       6,     7,     7,     7,     6,     6,     2,     1,     1,     1,
       1,     2,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     7,     7,     5,     5,     7,     5,     2,
       7,     7,     5,     3,     5,     7,     0,     4,     6,     5,
       7,     6,     7,     6,     5,     0,     3,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     7,     6,     8,     7,     5,
       0,     4,     4,     4,     3,     5,     1,     2,     3,     3,
       3,     4,     5,     5,     3,     3,     3,     3,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     1,     1,     4,     3,     4,     3,     6,     6,     5,
       6,     5,     4,     4,     6,     6,     5,     3,     2,     3,
       3,     2,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       5,     5,     1,     1,     1,     1,     3,     1,     2
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
#line 246 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1938 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 256 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1944 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 257 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1950 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 261 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1956 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 263 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1962 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 265 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 1968 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 267 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 1974 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 269 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1980 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 271 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1988 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 275 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1996 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 279 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2002 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 281 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2008 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 283 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2014 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 285 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2022 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 289 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2030 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 293 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2036 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 295 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2042 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 297 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2048 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 299 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2054 "raku.tab.c"
    break;

  case 21: /* stmt: KW_USE IDENT ';'  */
#line 301 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2060 "raku.tab.c"
    break;

  case 22: /* stmt: TESTOP ';'  */
#line 303 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2066 "raku.tab.c"
    break;

  case 23: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 305 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2072 "raku.tab.c"
    break;

  case 24: /* stmt: TESTOP '(' ')' ';'  */
#line 307 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2078 "raku.tab.c"
    break;

  case 25: /* stmt: TESTOP arg_list ';'  */
#line 309 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2084 "raku.tab.c"
    break;

  case 26: /* stmt: KW_SAY expr ';'  */
#line 311 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2090 "raku.tab.c"
    break;

  case 27: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 313 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2096 "raku.tab.c"
    break;

  case 28: /* stmt: KW_PRINT expr ';'  */
#line 315 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2102 "raku.tab.c"
    break;

  case 29: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 317 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2108 "raku.tab.c"
    break;

  case 30: /* stmt: KW_TAKE expr ';'  */
#line 319 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2114 "raku.tab.c"
    break;

  case 31: /* stmt: KW_RETURN expr ';'  */
#line 321 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2120 "raku.tab.c"
    break;

  case 32: /* stmt: KW_RETURN ';'  */
#line 323 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2126 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 325 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2132 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 327 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2141 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 332 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2149 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 336 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2157 "raku.tab.c"
    break;

  case 37: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 340 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2166 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 345 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2174 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 349 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2185 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 356 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2194 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 361 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2201 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 364 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2208 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 367 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2215 "raku.tab.c"
    break;

  case 44: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 370 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2222 "raku.tab.c"
    break;

  case 45: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 373 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2229 "raku.tab.c"
    break;

  case 46: /* stmt: expr ';'  */
#line 375 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2235 "raku.tab.c"
    break;

  case 47: /* stmt: if_stmt  */
#line 376 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2241 "raku.tab.c"
    break;

  case 48: /* stmt: while_stmt  */
#line 377 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2247 "raku.tab.c"
    break;

  case 49: /* stmt: for_stmt  */
#line 378 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2253 "raku.tab.c"
    break;

  case 50: /* stmt: given_stmt  */
#line 379 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2259 "raku.tab.c"
    break;

  case 51: /* stmt: KW_TRY block  */
#line 381 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2265 "raku.tab.c"
    break;

  case 52: /* stmt: KW_TRY block KW_CATCH block  */
#line 383 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2271 "raku.tab.c"
    break;

  case 53: /* stmt: KW_CATCH block  */
#line 385 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2277 "raku.tab.c"
    break;

  case 54: /* stmt: block  */
#line 387 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2283 "raku.tab.c"
    break;

  case 55: /* stmt: unless_stmt  */
#line 388 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2289 "raku.tab.c"
    break;

  case 56: /* stmt: until_stmt  */
#line 389 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2295 "raku.tab.c"
    break;

  case 57: /* stmt: repeat_stmt  */
#line 390 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2301 "raku.tab.c"
    break;

  case 58: /* stmt: sub_decl  */
#line 391 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2307 "raku.tab.c"
    break;

  case 59: /* stmt: class_decl  */
#line 392 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2313 "raku.tab.c"
    break;

  case 60: /* stmt: role_decl  */
#line 393 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2319 "raku.tab.c"
    break;

  case 61: /* stmt: grammar_decl  */
#line 394 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2325 "raku.tab.c"
    break;

  case 62: /* if_stmt: KW_IF '(' expr ')' block  */
#line 398 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2331 "raku.tab.c"
    break;

  case 63: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 400 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2337 "raku.tab.c"
    break;

  case 64: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 402 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2343 "raku.tab.c"
    break;

  case 65: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 406 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2349 "raku.tab.c"
    break;

  case 66: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 410 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2355 "raku.tab.c"
    break;

  case 67: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 412 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2361 "raku.tab.c"
    break;

  case 68: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 416 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2367 "raku.tab.c"
    break;

  case 69: /* repeat_stmt: KW_REPEAT block  */
#line 420 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2373 "raku.tab.c"
    break;

  case 70: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 424 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2383 "raku.tab.c"
    break;

  case 71: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 430 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2393 "raku.tab.c"
    break;

  case 72: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 436 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2402 "raku.tab.c"
    break;

  case 73: /* for_stmt: KW_FOR expr block  */
#line 441 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2409 "raku.tab.c"
    break;

  case 74: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 446 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2421 "raku.tab.c"
    break;

  case 75: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 454 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2434 "raku.tab.c"
    break;

  case 76: /* when_list: %empty  */
#line 464 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2440 "raku.tab.c"
    break;

  case 77: /* when_list: when_list KW_WHEN expr block  */
#line 466 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2448 "raku.tab.c"
    break;

  case 78: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 472 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2460 "raku.tab.c"
    break;

  case 79: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 480 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2470 "raku.tab.c"
    break;

  case 80: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 486 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2483 "raku.tab.c"
    break;

  case 81: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 495 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2494 "raku.tab.c"
    break;

  case 82: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 502 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2507 "raku.tab.c"
    break;

  case 83: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 513 "raku.y"
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
#line 2525 "raku.tab.c"
    break;

  case 84: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 529 "raku.y"
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
#line 2542 "raku.tab.c"
    break;

  case 85: /* is_clauses: %empty  */
#line 543 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2548 "raku.tab.c"
    break;

  case 86: /* is_clauses: is_clauses IDENT IDENT  */
#line 545 "raku.y"
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
#line 2564 "raku.tab.c"
    break;

  case 87: /* is_clauses: is_clauses TESTOP IDENT  */
#line 557 "raku.y"
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
#line 2580 "raku.tab.c"
    break;

  case 88: /* class_body_list: %empty  */
#line 570 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2586 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 572 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2593 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 575 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2600 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 578 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2607 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 581 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2614 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 584 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2621 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 587 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2628 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 590 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2635 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 593 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2642 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 596 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2649 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 599 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2656 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 602 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2663 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 605 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2670 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 608 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2681 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 615 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2692 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 622 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2703 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 629 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2714 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 636 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2722 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 640 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2730 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 644 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2738 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 648 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2746 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 652 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2757 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 659 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2768 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 666 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2779 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 673 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2790 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 680 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2804 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 690 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2816 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 698 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2829 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 707 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2840 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 714 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2854 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 724 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2866 "raku.tab.c"
    break;

  case 119: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 734 "raku.y"
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
#line 2883 "raku.tab.c"
    break;

  case 120: /* grammar_body_list: %empty  */
#line 748 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2889 "raku.tab.c"
    break;

  case 121: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 750 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2898 "raku.tab.c"
    break;

  case 122: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 755 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2907 "raku.tab.c"
    break;

  case 123: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 760 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2916 "raku.tab.c"
    break;

  case 124: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 767 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2924 "raku.tab.c"
    break;

  case 125: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 771 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2932 "raku.tab.c"
    break;

  case 126: /* param_list: VAR_SCALAR  */
#line 776 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2938 "raku.tab.c"
    break;

  case 127: /* param_list: IDENT VAR_SCALAR  */
#line 777 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2944 "raku.tab.c"
    break;

  case 128: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 778 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2950 "raku.tab.c"
    break;

  case 129: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 779 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2956 "raku.tab.c"
    break;

  case 130: /* param_list: param_list ',' VAR_SCALAR  */
#line 780 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2962 "raku.tab.c"
    break;

  case 131: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 781 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2968 "raku.tab.c"
    break;

  case 132: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 782 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2974 "raku.tab.c"
    break;

  case 133: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 783 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2980 "raku.tab.c"
    break;

  case 134: /* block: '{' stmt_list '}'  */
#line 786 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2986 "raku.tab.c"
    break;

  case 135: /* block: '{' YADA '}'  */
#line 787 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2992 "raku.tab.c"
    break;

  case 136: /* closure: '{' expr '}'  */
#line 790 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2998 "raku.tab.c"
    break;

  case 137: /* expr: VAR_SCALAR '=' expr  */
#line 793 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3004 "raku.tab.c"
    break;

  case 138: /* expr: KW_GATHER block  */
#line 794 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3014 "raku.tab.c"
    break;

  case 139: /* expr: cmp_expr  */
#line 799 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3020 "raku.tab.c"
    break;

  case 140: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 802 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3026 "raku.tab.c"
    break;

  case 141: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 803 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3032 "raku.tab.c"
    break;

  case 142: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 804 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3038 "raku.tab.c"
    break;

  case 143: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 805 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3044 "raku.tab.c"
    break;

  case 144: /* cmp_expr: jct_expr '<' jct_expr  */
#line 806 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3050 "raku.tab.c"
    break;

  case 145: /* cmp_expr: jct_expr '>' jct_expr  */
#line 807 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3056 "raku.tab.c"
    break;

  case 146: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 808 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3062 "raku.tab.c"
    break;

  case 147: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 809 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3068 "raku.tab.c"
    break;

  case 148: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 810 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3074 "raku.tab.c"
    break;

  case 149: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 811 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3080 "raku.tab.c"
    break;

  case 150: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 813 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3090 "raku.tab.c"
    break;

  case 151: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 819 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3100 "raku.tab.c"
    break;

  case 152: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 825 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3110 "raku.tab.c"
    break;

  case 153: /* cmp_expr: jct_expr  */
#line 830 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3116 "raku.tab.c"
    break;

  case 154: /* jct_expr: jct_expr '|' range_expr  */
#line 833 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3122 "raku.tab.c"
    break;

  case 155: /* jct_expr: jct_expr '&' range_expr  */
#line 834 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3128 "raku.tab.c"
    break;

  case 156: /* jct_expr: range_expr  */
#line 835 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3134 "raku.tab.c"
    break;

  case 157: /* range_expr: add_expr OP_RANGE add_expr  */
#line 838 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3140 "raku.tab.c"
    break;

  case 158: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 839 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3146 "raku.tab.c"
    break;

  case 159: /* range_expr: add_expr  */
#line 840 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3152 "raku.tab.c"
    break;

  case 160: /* add_expr: add_expr '+' mul_expr  */
#line 843 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3158 "raku.tab.c"
    break;

  case 161: /* add_expr: add_expr '-' mul_expr  */
#line 844 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3164 "raku.tab.c"
    break;

  case 162: /* add_expr: add_expr '~' mul_expr  */
#line 845 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3170 "raku.tab.c"
    break;

  case 163: /* add_expr: mul_expr  */
#line 846 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 3176 "raku.tab.c"
    break;

  case 164: /* mul_expr: mul_expr '*' unary_expr  */
#line 849 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3182 "raku.tab.c"
    break;

  case 165: /* mul_expr: mul_expr '/' unary_expr  */
#line 850 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3188 "raku.tab.c"
    break;

  case 166: /* mul_expr: mul_expr '%' unary_expr  */
#line 851 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3194 "raku.tab.c"
    break;

  case 167: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 852 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3200 "raku.tab.c"
    break;

  case 168: /* mul_expr: unary_expr  */
#line 853 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3206 "raku.tab.c"
    break;

  case 169: /* unary_expr: '-' unary_expr  */
#line 856 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3212 "raku.tab.c"
    break;

  case 170: /* unary_expr: '!' unary_expr  */
#line 857 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3218 "raku.tab.c"
    break;

  case 171: /* unary_expr: postfix_expr  */
#line 858 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3224 "raku.tab.c"
    break;

  case 172: /* postfix_expr: call_expr  */
#line 860 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3230 "raku.tab.c"
    break;

  case 173: /* call_expr: IDENT '(' arg_list ')'  */
#line 863 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3239 "raku.tab.c"
    break;

  case 174: /* call_expr: IDENT '(' ')'  */
#line 867 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3245 "raku.tab.c"
    break;

  case 175: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 869 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3254 "raku.tab.c"
    break;

  case 176: /* call_expr: VAR_SCALAR '(' ')'  */
#line 874 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3260 "raku.tab.c"
    break;

  case 177: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 876 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3270 "raku.tab.c"
    break;

  case 178: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 882 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3280 "raku.tab.c"
    break;

  case 179: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 888 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3288 "raku.tab.c"
    break;

  case 180: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 892 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3299 "raku.tab.c"
    break;

  case 181: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 899 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3308 "raku.tab.c"
    break;

  case 182: /* call_expr: IDENT '.' CARET IDENT  */
#line 904 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3318 "raku.tab.c"
    break;

  case 183: /* call_expr: atom '.' CARET IDENT  */
#line 910 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3328 "raku.tab.c"
    break;

  case 184: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 916 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3339 "raku.tab.c"
    break;

  case 185: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 923 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3350 "raku.tab.c"
    break;

  case 186: /* call_expr: atom '.' IDENT '(' ')'  */
#line 930 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3359 "raku.tab.c"
    break;

  case 187: /* call_expr: atom '.' IDENT  */
#line 935 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3368 "raku.tab.c"
    break;

  case 188: /* call_expr: KW_DIE expr  */
#line 940 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3374 "raku.tab.c"
    break;

  case 189: /* call_expr: KW_MAP closure expr  */
#line 942 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3380 "raku.tab.c"
    break;

  case 190: /* call_expr: KW_GREP closure expr  */
#line 944 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3386 "raku.tab.c"
    break;

  case 191: /* call_expr: KW_SORT expr  */
#line 946 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3392 "raku.tab.c"
    break;

  case 192: /* call_expr: KW_SORT closure expr  */
#line 948 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3398 "raku.tab.c"
    break;

  case 193: /* call_expr: atom  */
#line 949 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3404 "raku.tab.c"
    break;

  case 194: /* arg_list: expr  */
#line 952 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3410 "raku.tab.c"
    break;

  case 195: /* arg_list: arg_list ',' expr  */
#line 953 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3416 "raku.tab.c"
    break;

  case 196: /* atom: LIT_INT  */
#line 956 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3422 "raku.tab.c"
    break;

  case 197: /* atom: LIT_FLOAT  */
#line 957 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3428 "raku.tab.c"
    break;

  case 198: /* atom: LIT_STR  */
#line 958 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3434 "raku.tab.c"
    break;

  case 199: /* atom: WORDLIST  */
#line 960 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3446 "raku.tab.c"
    break;

  case 200: /* atom: LIT_INTERP_STR  */
#line 967 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3452 "raku.tab.c"
    break;

  case 201: /* atom: VAR_SCALAR  */
#line 968 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3458 "raku.tab.c"
    break;

  case 202: /* atom: VAR_ARRAY  */
#line 969 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3464 "raku.tab.c"
    break;

  case 203: /* atom: VAR_HASH  */
#line 970 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3470 "raku.tab.c"
    break;

  case 204: /* atom: VAR_CAPTURE  */
#line 972 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3478 "raku.tab.c"
    break;

  case 205: /* atom: VAR_FH  */
#line 976 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3486 "raku.tab.c"
    break;

  case 206: /* atom: VAR_NAMED_CAPTURE  */
#line 980 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3493 "raku.tab.c"
    break;

  case 207: /* atom: VAR_ARRAY '[' expr ']'  */
#line 983 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3499 "raku.tab.c"
    break;

  case 208: /* atom: VAR_HASH '<' IDENT '>'  */
#line 985 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3505 "raku.tab.c"
    break;

  case 209: /* atom: VAR_HASH '{' expr '}'  */
#line 987 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3511 "raku.tab.c"
    break;

  case 210: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 989 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3517 "raku.tab.c"
    break;

  case 211: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 991 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3523 "raku.tab.c"
    break;

  case 212: /* atom: IDENT  */
#line 992 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3529 "raku.tab.c"
    break;

  case 213: /* atom: VAR_TWIGIL  */
#line 994 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3537 "raku.tab.c"
    break;

  case 214: /* atom: VAR_ARRAY_TWIGIL  */
#line 998 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3545 "raku.tab.c"
    break;

  case 215: /* atom: VAR_HASH_TWIGIL  */
#line 1002 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3553 "raku.tab.c"
    break;

  case 216: /* atom: '(' expr ')'  */
#line 1005 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3559 "raku.tab.c"
    break;

  case 217: /* atom: block  */
#line 1006 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3565 "raku.tab.c"
    break;

  case 218: /* atom: KW_SUB block  */
#line 1007 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3571 "raku.tab.c"
    break;


#line 3575 "raku.tab.c"

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

#line 1009 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
