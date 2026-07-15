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
  YYSYMBOL_pair_list = 124,                /* pair_list  */
  YYSYMBOL_param_list = 125,               /* param_list  */
  YYSYMBOL_block = 126,                    /* block  */
  YYSYMBOL_closure = 127,                  /* closure  */
  YYSYMBOL_expr = 128,                     /* expr  */
  YYSYMBOL_cmp_expr = 129,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 130,                 /* jct_expr  */
  YYSYMBOL_range_expr = 131,               /* range_expr  */
  YYSYMBOL_add_expr = 132,                 /* add_expr  */
  YYSYMBOL_mul_expr = 133,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 134,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 135,             /* postfix_expr  */
  YYSYMBOL_call_expr = 136,                /* call_expr  */
  YYSYMBOL_arg_list = 137,                 /* arg_list  */
  YYSYMBOL_atom = 138                      /* atom  */
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
#define YYLAST   1697

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  104
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  226
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  570

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
       0,   246,   246,   257,   258,   261,   263,   265,   267,   269,
     271,   275,   279,   281,   283,   285,   287,   289,   293,   297,
     299,   301,   303,   305,   307,   309,   311,   313,   315,   317,
     320,   322,   324,   327,   329,   331,   333,   335,   337,   342,
     346,   350,   355,   359,   366,   371,   374,   377,   380,   383,
     386,   387,   388,   389,   390,   391,   393,   395,   397,   399,
     400,   401,   402,   403,   404,   405,   408,   410,   412,   416,
     420,   422,   426,   430,   434,   440,   446,   451,   456,   464,
     475,   476,   482,   490,   496,   505,   512,   523,   539,   554,
     555,   567,   581,   582,   585,   588,   591,   594,   597,   600,
     603,   606,   609,   612,   615,   618,   625,   632,   639,   646,
     650,   654,   658,   662,   669,   676,   683,   690,   700,   708,
     717,   724,   734,   744,   759,   760,   765,   770,   777,   781,
     787,   789,   791,   793,   797,   798,   799,   800,   801,   802,
     803,   804,   807,   808,   811,   814,   815,   820,   823,   824,
     825,   826,   827,   828,   829,   830,   831,   832,   833,   839,
     845,   851,   854,   855,   856,   859,   860,   861,   864,   865,
     866,   867,   870,   871,   872,   873,   874,   877,   878,   879,
     881,   883,   888,   889,   894,   896,   902,   908,   912,   919,
     924,   930,   936,   943,   950,   955,   960,   962,   964,   966,
     968,   970,   973,   974,   977,   978,   979,   980,   988,   989,
     990,   991,   992,   996,  1000,  1003,  1005,  1007,  1009,  1011,
    1013,  1014,  1018,  1022,  1026,  1027,  1028
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
  "pair_list", "param_list", "block", "closure", "expr", "cmp_expr",
  "jct_expr", "range_expr", "add_expr", "mul_expr", "unary_expr",
  "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-309)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-226)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -309,    60,   566,  -309,  -309,  -309,  -309,  -309,   145,   -45,
     -48,    18,   146,  -309,  -309,  -309,  -309,  -309,    92,   627,
     299,  1224,  1280,    58,    63,  1329,   -13,    67,  1329,   676,
    1329,   174,   178,   113,   132,    67,   116,   116,  1385,    67,
      67,  1329,   239,   267,   236,  -309,   298,   194,   194,  1329,
     259,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  1584,   235,   -42,   176,  -309,    87,   173,
    -309,  -309,  -309,   231,   355,  1329,   364,   732,  1329,   368,
    1329,  1329,    45,   781,   297,   -15,   284,   -27,  -309,    67,
    -309,   843,  -309,  -309,   138,   -56,    96,   119,   279,  1329,
     162,  1329,   209,  1329,  1329,   -41,   106,   292,  -309,  -309,
     314,  -309,   315,   293,    48,    55,  1329,  1329,  -309,  1329,
    1329,  1329,   892,  1329,  -309,   366,  -309,  -309,  -309,   312,
       7,   313,   318,  -309,  -309,   319,   316,   445,  -309,   194,
     194,   194,   194,   194,   194,   194,   194,   307,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   266,    12,   321,    29,  -309,    39,   322,   339,   323,
     329,   331,   413,   332,  -309,    46,  -309,  1329,  1329,   417,
    1329,   337,   319,    82,  -309,  1329,  1329,  -309,  1434,  -309,
    1490,  -309,   120,   121,   140,   200,  -309,  1329,   230,  -309,
    1329,   335,   336,   426,  -309,   194,   194,    -3,  -309,  -309,
    -309,   423,  1329,   424,  1329,   340,   341,   338,  -309,  -309,
    -309,    67,    -8,  -309,   344,   347,  -309,  -309,  -309,  -309,
     294,   294,   294,   294,   294,   294,   294,   294,  -309,  -309,
    -309,   294,   294,  -309,  -309,   229,   229,   173,   173,   173,
    -309,  -309,  -309,  -309,   348,   438,  -309,   953,  -309,  1329,
    1002,  -309,   371,   372,   380,  -309,  1064,  -309,  1113,  -309,
    -309,   381,   387,   370,  -309,   385,  -309,   399,  1329,   289,
     430,    53,  1539,   295,   401,  1329,  -309,  1595,  -309,  1329,
    -309,  1329,   300,  1329,   302,    67,    67,    67,   -39,    99,
    -309,    10,    67,   238,     6,   415,   398,   419,   406,    67,
      67,  -309,  -309,   488,   497,  -309,     2,     3,    81,   -17,
    1175,  -309,   416,   241,   418,   420,   242,  1329,  1329,  1329,
    -309,   245,    54,  -309,   248,   249,  -309,  -309,  -309,  -309,
    -309,   252,  -309,  1329,  1329,  1329,   253,  -309,    27,  -309,
     421,  1329,   304,   422,   414,  -309,   425,  -309,   492,  -309,
    -309,   505,   510,  -309,   511,   512,  -309,   127,    67,  1329,
      67,  -309,  -309,  -309,   427,   429,   500,  -309,  -309,  -309,
      26,    30,   272,   479,  -309,    67,   256,   257,   516,   518,
     519,  -309,  -309,   260,   261,  -309,   440,  -309,  -309,   442,
     444,   446,   448,  -309,  1329,   520,  -309,  -309,  1329,   306,
    -309,  -309,   450,   475,   483,  -309,   264,  -309,  1329,  -309,
     454,   455,   -14,    67,    67,  -309,  -309,  -309,    19,  -309,
      67,   449,  -309,  -309,    67,  -309,   457,   458,    17,    44,
     288,   461,   462,   539,  -309,    67,    67,   552,   553,   554,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,   496,   265,
    -309,  -309,  1329,  1329,  1329,   308,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,   555,   557,  -309,  -309,  -309,     5,     8,
     549,   550,   478,  1329,  -309,   561,   569,   498,  1329,  -309,
      71,    76,   506,   507,  -309,  -309,   495,  -309,  -309,  -309,
    -309,  -309,  1329,   521,  -309,  -309,   268,  -309,  -309,  -309,
      67,   276,    67,   277,   522,   525,  -309,   526,   527,   529,
    -309,   530,   602,   613,   533,  1329,  -309,   620,   621,   540,
    1329,  -309,  -309,  -309,    13,  -309,  -309,   548,  -309,    67,
    -309,    67,  -309,  -309,  -309,  -309,  -309,  -309,   556,   558,
    -309,   559,   564,   565,  -309,   567,    67,   280,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,    67,  -309
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   204,   205,   206,   208,   209,   210,
     211,   221,   220,   222,   223,   212,   213,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   207,     0,     0,     0,     0,
       3,     4,    51,    52,    59,    60,    61,    53,    54,    62,
      63,    64,    65,    58,     0,   147,   161,   164,   167,   171,
     176,   179,   180,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   211,   221,     0,
      24,     0,   225,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,     0,   226,   146,
       0,    36,     0,     0,     0,     0,     0,     0,    73,     0,
       0,     0,     3,     0,   199,    55,    57,   196,    89,     0,
       0,     0,   209,   178,   177,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,   182,     0,    23,     0,     0,     0,
       0,     0,   202,     0,    27,     0,     0,     6,     0,     7,
       0,     8,     0,     0,     0,     0,    28,     0,     0,    31,
       0,     0,     0,     0,    77,     0,     0,     0,    34,    35,
      80,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     200,     0,     0,    92,     0,     0,   124,   224,   143,   142,
     148,   149,   150,   151,   154,   155,   156,   157,   158,   159,
     160,   152,   153,   162,   163,   165,   166,   170,   168,   169,
     175,   172,   173,   174,   195,     0,    40,     0,    37,     0,
       0,   183,   215,   216,   217,    42,     0,   190,     0,   181,
     145,     0,     0,     0,    26,     0,   203,     0,     0,     0,
     206,   220,     0,     0,     0,     0,    20,     0,    21,     0,
      22,     0,     0,     0,     0,     0,     0,     0,   165,   166,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   144,    56,     0,     0,    92,     0,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,   220,   187,     0,     0,   215,   216,   217,    25,
       5,     0,     9,     0,     0,     0,     0,    13,     0,    12,
       0,     0,     0,     0,     0,    29,     0,    32,    66,    69,
      76,     0,     0,   135,     0,     0,    83,     0,     0,     0,
       0,    78,   218,   219,     0,     0,    70,    72,    90,    91,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,   123,   194,     0,     0,    39,     0,    41,    44,     0,
       0,     0,     0,   188,     0,     0,   185,   186,     0,     0,
     131,   130,     0,     0,     0,    15,     0,    16,     0,    19,
       0,     0,     0,     0,     0,   136,   137,   138,     0,    82,
       0,     0,    48,    49,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
     193,   192,    38,    43,    45,    46,    47,   128,     0,     0,
      10,    14,     0,     0,     0,     0,    30,    33,    68,    67,
      74,    75,   139,     0,     0,    81,    79,    71,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,    93,
       0,     0,     0,     0,    94,    95,     0,    84,    86,   125,
     126,   127,     0,     0,   133,   132,     0,    17,   140,   141,
       0,     0,     0,     0,     0,     0,   109,     0,     0,     0,
     110,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,    99,    96,    97,     0,   129,    11,     0,   118,     0,
     120,     0,   107,   108,   102,   105,   106,   101,     0,     0,
     111,     0,     0,     0,   112,     0,     0,     0,    18,   117,
     119,   115,   116,   104,   113,   114,   103,   122,     0,   121
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -309,  -309,   648,  -309,   228,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,   342,  -309,  -309,   333,
     384,  -308,    -2,   369,    43,  -309,   130,   258,   -22,   168,
     -43,  -309,  -309,   -75,  -309
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   137,    51,    52,    53,    54,    55,    56,    57,
      58,   304,    59,    60,    61,   222,   316,    62,   319,   334,
     283,   303,    92,   120,    93,    65,    66,    67,    68,    69,
      70,    71,    72,    94,    73
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,   107,   166,   106,   133,   134,   313,   300,   175,   386,
     387,   301,    23,   300,   314,   300,   183,   301,   300,   301,
     363,   224,   301,   300,   108,   109,   186,   301,   203,   472,
     361,   480,   413,   118,   139,   140,    79,   125,   126,   481,
     187,   414,   369,   370,   436,    64,   388,   389,   390,   154,
     155,   156,   381,   382,    80,    78,   383,   179,   485,   171,
       3,    50,   172,   225,   100,   102,   486,   177,   105,   364,
     365,   110,   112,   113,   482,   180,   381,   382,   473,   474,
     383,   124,   437,    77,   127,   522,   391,   108,    50,    50,
     527,   300,   135,   523,   315,   301,   302,   173,   528,   483,
      81,   487,   385,   204,   510,   384,    84,   512,   256,   371,
     257,   259,   556,   484,   250,   251,   252,   253,   163,   345,
     404,   167,   292,   169,   170,   294,   488,   260,   524,   435,
     245,   246,   211,   529,   182,    63,   185,   427,   261,   213,
     489,   428,   195,   185,   198,   269,   201,   202,    82,    82,
     212,    83,    83,   525,   152,   153,   103,   214,   530,   215,
     216,   104,   217,   218,   219,   217,   220,   526,   362,    50,
     511,   513,   531,   205,   206,   154,   155,   156,   188,   185,
      64,   275,   323,   298,   299,   326,   114,   154,   155,   156,
     115,   331,   189,   335,   154,   155,   156,     4,     5,     6,
       7,   190,   285,   287,   132,    86,    87,    88,    12,    13,
      14,   116,    15,    16,    17,   191,   286,   288,   119,   312,
     270,   271,   289,   273,    74,    89,   557,    75,   276,   277,
     117,   279,    31,   284,   184,   185,   290,    36,    37,    38,
      76,    82,    41,    77,    83,   394,   141,   142,   143,   144,
     145,   146,    45,   128,   157,   306,   147,   308,   196,   197,
     148,   149,   150,   151,   158,   159,   160,   130,   409,   230,
     231,   232,   233,   234,   235,   236,   237,    47,   241,   242,
     254,   129,   438,   255,    48,   439,   440,   441,   442,   192,
     193,   194,    49,   358,   359,   360,    50,   291,   490,   227,
     366,   491,   324,   492,   493,   199,   200,   376,   377,    95,
      96,    97,   131,    98,   238,   239,   240,   154,   155,   156,
     136,   341,   247,   248,   249,   135,   161,   293,   350,   227,
     352,   138,   353,   459,   354,   367,   356,   368,   185,   185,
     396,   399,   185,   465,   403,   405,   185,   406,   407,   408,
     348,   227,   412,   367,   367,   445,   446,   405,   185,   450,
     451,   464,   185,   227,   503,   185,   429,   537,   431,   162,
     400,   401,   402,   367,   367,   539,   541,   367,   164,   568,
     150,   151,   168,   444,   178,   342,   343,   410,   411,   506,
     207,   347,   348,   176,   416,   210,   355,   185,   357,   185,
     417,   418,   460,   185,   507,   185,   121,   123,   243,   244,
     208,   209,   430,   221,   223,   226,    77,   258,   227,   228,
     469,   470,   471,   262,   263,   265,   264,   267,   475,   266,
     268,   272,   477,   274,   295,   296,   297,   305,   307,   309,
     310,   311,   317,   497,   498,   318,   320,   457,     4,     5,
       6,     7,   321,   327,   328,     8,     9,    10,    11,    12,
      13,    14,   329,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   337,   338,    24,    25,    26,    27,    28,    29,
      30,   339,   336,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,   340,   344,   349,    43,    44,
     372,   373,   378,    45,   374,   504,   505,    46,   538,   375,
     540,   379,   395,   420,   397,   423,   398,   415,   419,   422,
     424,   425,   426,   432,   421,   433,   517,   434,    47,   443,
     447,   521,   448,   449,   458,    48,   452,   559,   453,   560,
     454,   462,   455,    49,   456,   535,   461,    50,   229,   463,
     466,   467,   476,   496,   567,   478,   479,   494,   495,   499,
     500,   501,   502,   514,   515,   508,   569,   509,   551,     4,
       5,     6,     7,   555,   516,   518,     8,     9,    10,    11,
      12,    13,    14,   519,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   534,   520,    24,    25,    26,    27,    28,
      29,    30,   532,   533,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,   548,   536,   542,    43,
      44,   543,   544,   545,    45,   546,   547,   549,    46,   550,
       4,     5,     6,     7,   552,   553,   554,    85,    86,    87,
      88,    12,    13,    14,   558,    15,    16,    17,     2,    47,
     468,     0,   561,   393,   562,   563,    48,   380,    89,    27,
     564,   565,     0,   566,    49,    31,   346,     0,    50,     0,
      36,    37,    38,     0,     0,    41,     0,     0,     0,     4,
       5,     6,     7,     0,     0,    45,    85,    86,    87,    88,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    27,     0,
      47,     0,     0,     0,    31,     0,     0,    48,     0,    36,
      37,    38,     0,    90,    41,    91,     0,     0,     0,    50,
       0,     0,     0,     0,    45,     4,     5,     6,     7,     0,
       0,     0,    85,    86,    87,    88,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,    47,
       0,     0,     0,    89,    27,     0,    48,     0,     0,     0,
      31,     0,   111,     0,    49,    36,    37,    38,    50,     0,
      41,     0,     0,     0,     4,     5,     6,     7,     0,     0,
      45,    85,    86,    87,    88,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    27,     0,    47,     0,     0,     0,    31,
       0,     0,    48,     0,    36,    37,    38,     0,     0,    41,
      49,   165,     0,     0,    50,     0,     0,     0,     0,    45,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    85,    86,    87,    88,    12,    13,    14,
       0,    15,    16,    17,    47,     0,     0,     0,     0,     0,
       0,    48,     0,     0,    89,    27,     0,     0,     0,    49,
     174,    31,     0,    50,     0,     0,    36,    37,    38,     0,
       0,    41,     0,     0,     0,     4,     5,     6,     7,     0,
       0,    45,    85,    86,    87,    88,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    27,     0,    47,     0,     0,     0,
      31,     0,     0,    48,     0,    36,    37,    38,     0,     0,
      41,    49,   181,     0,     0,    50,     0,     0,     0,     0,
      45,     0,     0,   136,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    85,    86,    87,    88,    12,    13,    14,
       0,    15,    16,    17,     0,    47,     0,     0,     0,     0,
       0,     0,    48,     0,    89,    27,     0,     0,     0,     0,
      49,    31,     0,     0,    50,     0,    36,    37,    38,     0,
       0,    41,     0,     0,     0,     4,     5,     6,     7,     0,
       0,    45,    85,    86,    87,    88,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    27,     0,    47,     0,     0,     0,
      31,     0,     0,    48,     0,    36,    37,    38,     0,     0,
      41,    49,   322,     0,     0,    50,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    85,    86,    87,    88,    12,    13,
      14,     0,    15,    16,    17,    47,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    89,    27,     0,     0,     0,
      49,   325,    31,     0,    50,     0,     0,    36,    37,    38,
       0,     0,    41,     0,     0,     0,     4,     5,     6,     7,
       0,     0,    45,    85,    86,    87,    88,   332,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    27,     0,    47,     0,     0,
       0,    31,     0,     0,    48,     0,    36,    37,    38,     0,
       0,    41,    49,   330,     0,     0,    50,     0,     0,     0,
       0,    45,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    85,    86,    87,    88,   332,
      13,    14,     0,    15,    16,    17,    47,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    89,    27,     0,     0,
       0,    49,   333,    31,     0,    50,     0,     0,    36,    37,
      38,     0,     0,    41,     0,     0,     0,     4,     5,     6,
       7,     0,     0,    45,    85,    86,    87,    88,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    27,     0,    47,     0,
       0,     0,    31,     0,     0,    48,     0,    36,    37,    38,
       0,     0,    41,    49,   392,     0,     0,    50,     0,     0,
       0,     0,    45,     4,     5,     6,     7,     0,     0,     0,
      85,    86,    87,    88,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,    47,     0,     0,
       0,    89,    27,     0,    48,     0,     0,     0,    31,     0,
       0,     0,    99,    36,    37,    38,    50,     0,    41,     0,
       0,     0,     4,     5,     6,     7,     0,     0,    45,    85,
      86,    87,    88,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    27,     0,    47,     0,     0,     0,    31,     0,     0,
      48,     0,    36,    37,    38,     0,     0,    41,   101,     0,
       0,     0,    50,     0,     0,     0,     0,    45,     4,     5,
       6,     7,     0,     0,     0,    85,    86,    87,    88,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,    47,     0,     0,     0,    89,    27,     0,    48,
       0,     0,     0,    31,     0,     0,     0,    49,    36,    37,
      38,    50,     0,    41,     0,     0,     0,     4,     5,     6,
       7,     0,     0,    45,    85,    86,    87,    88,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    27,     0,    47,     0,
       0,     0,    31,     0,     0,    48,     0,    36,    37,    38,
       0,     0,    41,    49,     0,     0,     0,   122,     0,     0,
       0,     0,    45,     4,     5,   280,     7,     0,     0,     0,
      85,    86,    87,    88,   281,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,    47,     0,     0,
       0,    89,    27,     0,    48,     0,     0,     0,    31,     0,
       0,     0,   278,    36,    37,    38,    50,     0,    41,     0,
       0,     0,     4,     5,   280,     7,     0,     0,    45,    85,
      86,    87,    88,   281,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    27,     0,    47,     0,     0,     0,    31,     0,     0,
      48,     0,    36,    37,    38,     0,     0,    41,   282,     0,
       0,     0,    50,     0,     0,     0,     0,    45,     4,     5,
       6,     7,     0,     0,     0,    85,    86,    87,    88,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,    47,     0,     0,     0,    89,    27,     0,    48,
       0,     0,     0,    31,     0,     0,     0,    49,    36,    37,
      38,    50,     0,    41,     0,     0,     0,     0,     0,     0,
       0,  -225,  -225,    45,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,     0,     0,  -225,  -225,     0,     0,  -225,  -225,
    -225,  -225,  -225,  -225,     0,  -225,  -225,  -225,    47,  -225,
    -225,     0,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,    50
};

static const yytype_int16 yycheck[] =
{
       2,    14,    77,    25,    47,    48,    14,    10,    83,   317,
     318,    14,    26,    10,    22,    10,    91,    14,    10,    14,
      10,    14,    14,    10,    26,    27,    82,    14,    69,    10,
      69,    14,     5,    35,    76,    77,    84,    39,    40,    22,
      96,    14,    36,    37,    14,     2,    63,    64,    65,    88,
      89,    90,    50,    51,   102,   100,    54,    84,    14,    14,
       0,   102,    17,    56,    21,    22,    22,    82,    25,    59,
      60,    28,    29,    30,    57,   102,    50,    51,    59,    60,
      54,    38,    52,    98,    41,    14,   103,    89,   102,   102,
      14,    10,    49,    22,   102,    14,    99,    52,    22,    82,
      82,    57,    99,   105,    99,   103,    14,    99,    96,   103,
      98,    82,    99,    96,   157,   158,   159,   160,    75,    66,
      66,    78,   197,    80,    81,   200,    82,    98,    57,   103,
     152,   153,    84,    57,    91,   137,    97,    10,    99,    84,
      96,    14,    99,    97,   101,    99,   103,   104,    95,    95,
     102,    98,    98,    82,    67,    68,    98,   102,    82,   116,
     117,    98,   119,   120,   121,   122,   123,    96,    69,   102,
     478,   479,    96,    67,    68,    88,    89,    90,    82,    97,
     137,    99,   257,   205,   206,   260,    12,    88,    89,    90,
      12,   266,    96,   268,    88,    89,    90,     3,     4,     5,
       6,    82,    82,    82,    10,    11,    12,    13,    14,    15,
      16,    98,    18,    19,    20,    96,    96,    96,   102,   221,
     177,   178,    82,   180,    79,    31,   534,    82,   185,   186,
      98,   188,    38,   190,    96,    97,    96,    43,    44,    45,
      95,    95,    48,    98,    98,   320,    70,    71,    72,    73,
      74,    75,    58,    14,    81,   212,    80,   214,    96,    97,
      84,    85,    86,    87,    91,    92,    93,    31,   343,   139,
     140,   141,   142,   143,   144,   145,   146,    83,   148,   149,
      14,    14,    10,    17,    90,    13,    14,    15,    16,    10,
      11,    12,    98,   295,   296,   297,   102,    97,    10,    99,
     302,    13,   259,    15,    16,    96,    97,   309,   310,    10,
      11,    12,    14,    14,     7,     8,     9,    88,    89,    90,
      61,   278,   154,   155,   156,   282,    95,    97,   285,    99,
     287,    96,   289,   408,   291,    97,   293,    99,    97,    97,
      99,    99,    97,   418,    99,    97,    97,    99,    99,    97,
      97,    99,    99,    97,    97,    99,    99,    97,    97,    99,
      99,    97,    97,    99,    99,    97,   368,    99,   370,    14,
     327,   328,   329,    97,    97,    99,    99,    97,    14,    99,
      86,    87,    14,   385,   100,    96,    97,   344,   345,   464,
      98,    96,    97,    96,   351,   102,    96,    97,    96,    97,
      96,    97,    96,    97,    96,    97,    37,    38,   150,   151,
      96,    96,   369,    47,   102,   102,    98,    96,    99,   103,
     422,   423,   424,   101,    85,    96,   103,    14,   430,    98,
      98,    14,   434,    96,    99,    99,    10,    14,    14,    99,
      99,   103,    98,   445,   446,    98,    98,   404,     3,     4,
       5,     6,    14,    82,    82,    10,    11,    12,    13,    14,
      15,    16,    82,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    85,   103,    29,    30,    31,    32,    33,    34,
      35,    96,   101,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    96,    66,    96,    53,    54,
      85,   103,    14,    58,    85,   462,   463,    62,   510,   103,
     512,    14,    96,    99,    96,    10,    96,    96,    96,    27,
      10,    10,    10,    96,    99,    96,   483,    27,    83,    50,
      14,   488,    14,    14,    14,    90,    96,   539,    96,   541,
      96,    66,    96,    98,    96,   502,    96,   102,   103,    66,
      96,    96,   103,    14,   556,    98,    98,    96,    96,     7,
       7,     7,    66,    14,    14,    10,   568,    10,   525,     3,
       4,     5,     6,   530,    96,    14,    10,    11,    12,    13,
      14,    15,    16,    14,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    98,    96,    29,    30,    31,    32,    33,
      34,    35,    96,    96,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    14,    96,    96,    53,
      54,    96,    96,    96,    58,    96,    96,    14,    62,    96,
       3,     4,     5,     6,    14,    14,    96,    10,    11,    12,
      13,    14,    15,    16,    96,    18,    19,    20,     0,    83,
     422,    -1,    96,   320,    96,    96,    90,   315,    31,    32,
      96,    96,    -1,    96,    98,    38,   282,    -1,   102,    -1,
      43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    58,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      83,    -1,    -1,    -1,    38,    -1,    -1,    90,    -1,    43,
      44,    45,    -1,    96,    48,    98,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    58,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    31,    32,    -1,    90,    -1,    -1,    -1,
      38,    -1,    96,    -1,    98,    43,    44,    45,   102,    -1,
      48,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      58,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    83,    -1,    -1,    -1,    38,
      -1,    -1,    90,    -1,    43,    44,    45,    -1,    -1,    48,
      98,    99,    -1,    -1,   102,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,
      99,    38,    -1,   102,    -1,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    58,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    83,    -1,    -1,    -1,
      38,    -1,    -1,    90,    -1,    43,    44,    45,    -1,    -1,
      48,    98,    99,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    61,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    31,    32,    -1,    -1,    -1,    -1,
      98,    38,    -1,    -1,   102,    -1,    43,    44,    45,    -1,
      -1,    48,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    58,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    83,    -1,    -1,    -1,
      38,    -1,    -1,    90,    -1,    43,    44,    45,    -1,    -1,
      48,    98,    99,    -1,    -1,   102,    -1,    -1,    -1,    -1,
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
      45,    -1,    -1,    48,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    58,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    83,    -1,
      -1,    -1,    38,    -1,    -1,    90,    -1,    43,    44,    45,
      -1,    -1,    48,    98,    99,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    58,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    31,    32,    -1,    90,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    98,    43,    44,    45,   102,    -1,    48,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    58,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    83,    -1,    -1,    -1,    38,    -1,    -1,
      90,    -1,    43,    44,    45,    -1,    -1,    48,    98,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    58,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    31,    32,    -1,    90,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    98,    43,    44,
      45,   102,    -1,    48,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    58,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    83,    -1,
      -1,    -1,    38,    -1,    -1,    90,    -1,    43,    44,    45,
      -1,    -1,    48,    98,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    58,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    31,    32,    -1,    90,    -1,    -1,    -1,    38,    -1,
      -1,    -1,    98,    43,    44,    45,   102,    -1,    48,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    58,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    83,    -1,    -1,    -1,    38,    -1,    -1,
      90,    -1,    43,    44,    45,    -1,    -1,    48,    98,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    58,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    31,    32,    -1,    90,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    98,    43,    44,
      45,   102,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    58,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    80,    81,    -1,    -1,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    83,    95,
      96,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,   102
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
     117,   118,   121,   126,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   138,    79,    82,    95,    98,   100,    84,
     102,    82,    95,    98,    14,    10,    11,    12,    13,    31,
      96,    98,   126,   128,   137,    10,    11,    12,    14,    98,
     128,    98,   128,    98,    98,   128,   132,    14,   126,   126,
     128,    96,   128,   128,    12,    12,    98,    98,   126,   102,
     127,   127,   102,   127,   128,   126,   126,   128,    14,    14,
      31,    14,    10,   134,   134,   128,    61,   106,    96,    76,
      77,    70,    71,    72,    73,    74,    75,    80,    84,    85,
      86,    87,    67,    68,    88,    89,    90,    81,    91,    92,
      93,    95,    14,   128,    14,    99,   137,   128,    14,   128,
     128,    14,    17,    52,    99,   137,    96,    82,   100,    84,
     102,    99,   128,   137,    96,    97,    82,    96,    82,    96,
      82,    96,    10,    11,    12,   128,    96,    97,   128,    96,
      97,   128,   128,    69,   126,    67,    68,    98,    96,    96,
     102,    84,   102,    84,   102,   128,   128,   128,   128,   128,
     128,    47,   119,   102,    14,    56,   102,    99,   103,   103,
     130,   130,   130,   130,   130,   130,   130,   130,     7,     8,
       9,   130,   130,   131,   131,   132,   132,   133,   133,   133,
     134,   134,   134,   134,    14,    17,    96,    98,    96,    82,
      98,    99,   101,    85,   103,    96,    98,    14,    98,    99,
     128,   128,    14,   128,    96,    99,   128,   128,    98,   128,
       5,    14,    98,   124,   128,    82,    96,    82,    96,    82,
      96,    97,   137,    97,   137,    99,    99,    10,   132,   132,
      10,    14,    99,   125,   115,    14,   128,    14,   128,    99,
      99,   103,   126,    14,    22,   102,   120,    98,    98,   122,
      98,    14,    99,   137,   128,    99,   137,    82,    82,    82,
      99,   137,    14,    99,   123,   137,   101,    85,   103,    96,
      96,   128,    96,    97,    66,    66,   124,    96,    97,    96,
     128,    98,   128,   128,   128,    96,   128,    96,   126,   126,
     126,    69,    69,    10,    59,    60,   126,    97,    99,    36,
      37,   103,    85,   103,    85,   103,   126,   126,    14,    14,
     120,    50,    51,    54,   103,    99,   125,   125,    63,    64,
      65,   103,    99,   123,   137,    96,    99,    96,    96,    99,
     128,   128,   128,    99,    66,    97,    99,    99,    97,   137,
     128,   128,    99,     5,    14,    96,   128,    96,    97,    96,
      99,    99,    27,    10,    10,    10,    10,    10,    14,   126,
     128,   126,    96,    96,    27,   103,    14,    52,    10,    13,
      14,    15,    16,    50,   126,    99,    99,    14,    14,    14,
      99,    99,    96,    96,    96,    96,    96,   128,    14,   137,
      96,    96,    66,    66,    97,   137,    96,    96,   108,   126,
     126,   126,    10,    59,    60,   126,   103,   126,    98,    98,
      14,    22,    57,    82,    96,    14,    22,    57,    82,    96,
      10,    13,    15,    16,    96,    96,    14,   126,   126,     7,
       7,     7,    66,    99,   128,   128,   137,    96,    10,    10,
      99,   125,    99,   125,    14,    14,    96,   128,    14,    14,
      96,   128,    14,    22,    57,    82,    96,    14,    22,    57,
      82,    96,    96,    96,    98,   128,    96,    99,   126,    99,
     126,    99,    96,    96,    96,    96,    96,    96,    14,    14,
      96,   128,    14,    14,    96,   128,    99,   125,    96,   126,
     126,    96,    96,    96,    96,    96,    96,   126,    99,   126
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
     107,   107,   107,   107,   107,   107,   108,   108,   108,   109,
     110,   110,   111,   112,   113,   113,   113,   113,   114,   114,
     115,   115,   116,   116,   116,   116,   116,   117,   118,   119,
     119,   119,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   121,   122,   122,   122,   122,   123,   123,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   125,
     125,   125,   126,   126,   127,   128,   128,   128,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   130,   130,   130,   131,   131,   131,   132,   132,
     132,   132,   133,   133,   133,   133,   133,   134,   134,   134,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   137,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     9,     5,     5,     7,     6,     6,     8,    10,     6,
       4,     4,     4,     3,     2,     5,     4,     3,     3,     5,
       7,     3,     5,     7,     3,     3,     2,     4,     7,     6,
       4,     6,     4,     7,     6,     7,     7,     7,     6,     6,
       2,     1,     1,     1,     1,     2,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     7,     7,     5,
       5,     7,     5,     2,     7,     7,     5,     3,     5,     7,
       0,     4,     6,     5,     7,     6,     7,     6,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     7,
       6,     8,     7,     5,     0,     4,     4,     4,     3,     5,
       3,     3,     5,     5,     1,     2,     3,     3,     3,     4,
       5,     5,     3,     3,     3,     3,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     3,     3,     1,     2,     2,     1,
       1,     4,     3,     4,     3,     6,     6,     5,     6,     5,
       4,     4,     6,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     5,     5,
       1,     1,     1,     1,     3,     1,     2
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
#line 247 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1974 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 257 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1980 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 258 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1986 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 262 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1992 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 264 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1998 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 266 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2004 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 268 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2010 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 270 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2016 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 272 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2024 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 276 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2032 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 280 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2038 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 282 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2044 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 284 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2050 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 286 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2056 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 288 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2062 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 290 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2070 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 294 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2078 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 298 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2084 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 300 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2090 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 302 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2096 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 304 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2102 "raku.tab.c"
    break;

  case 23: /* stmt: KW_USE IDENT ';'  */
#line 306 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2108 "raku.tab.c"
    break;

  case 24: /* stmt: TESTOP ';'  */
#line 308 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2114 "raku.tab.c"
    break;

  case 25: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 310 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2120 "raku.tab.c"
    break;

  case 26: /* stmt: TESTOP '(' ')' ';'  */
#line 312 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2126 "raku.tab.c"
    break;

  case 27: /* stmt: TESTOP arg_list ';'  */
#line 314 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2132 "raku.tab.c"
    break;

  case 28: /* stmt: KW_SAY expr ';'  */
#line 316 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2138 "raku.tab.c"
    break;

  case 29: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 318 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2145 "raku.tab.c"
    break;

  case 30: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 321 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2151 "raku.tab.c"
    break;

  case 31: /* stmt: KW_PRINT expr ';'  */
#line 323 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2157 "raku.tab.c"
    break;

  case 32: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 325 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2164 "raku.tab.c"
    break;

  case 33: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 328 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2170 "raku.tab.c"
    break;

  case 34: /* stmt: KW_TAKE expr ';'  */
#line 330 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2176 "raku.tab.c"
    break;

  case 35: /* stmt: KW_RETURN expr ';'  */
#line 332 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2182 "raku.tab.c"
    break;

  case 36: /* stmt: KW_RETURN ';'  */
#line 334 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2188 "raku.tab.c"
    break;

  case 37: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 336 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2194 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 338 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2203 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 343 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2211 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 347 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2219 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 351 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2228 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 356 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2236 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 360 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2247 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 367 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2256 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 372 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2263 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 375 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2270 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 378 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2277 "raku.tab.c"
    break;

  case 48: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 381 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2284 "raku.tab.c"
    break;

  case 49: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 384 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2291 "raku.tab.c"
    break;

  case 50: /* stmt: expr ';'  */
#line 386 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2297 "raku.tab.c"
    break;

  case 51: /* stmt: if_stmt  */
#line 387 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2303 "raku.tab.c"
    break;

  case 52: /* stmt: while_stmt  */
#line 388 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2309 "raku.tab.c"
    break;

  case 53: /* stmt: for_stmt  */
#line 389 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2315 "raku.tab.c"
    break;

  case 54: /* stmt: given_stmt  */
#line 390 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2321 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TRY block  */
#line 392 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2327 "raku.tab.c"
    break;

  case 56: /* stmt: KW_TRY block KW_CATCH block  */
#line 394 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2333 "raku.tab.c"
    break;

  case 57: /* stmt: KW_CATCH block  */
#line 396 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2339 "raku.tab.c"
    break;

  case 58: /* stmt: block  */
#line 398 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2345 "raku.tab.c"
    break;

  case 59: /* stmt: unless_stmt  */
#line 399 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2351 "raku.tab.c"
    break;

  case 60: /* stmt: until_stmt  */
#line 400 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2357 "raku.tab.c"
    break;

  case 61: /* stmt: repeat_stmt  */
#line 401 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2363 "raku.tab.c"
    break;

  case 62: /* stmt: sub_decl  */
#line 402 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2369 "raku.tab.c"
    break;

  case 63: /* stmt: class_decl  */
#line 403 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2375 "raku.tab.c"
    break;

  case 64: /* stmt: role_decl  */
#line 404 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2381 "raku.tab.c"
    break;

  case 65: /* stmt: grammar_decl  */
#line 405 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2387 "raku.tab.c"
    break;

  case 66: /* if_stmt: KW_IF '(' expr ')' block  */
#line 409 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2393 "raku.tab.c"
    break;

  case 67: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 411 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2399 "raku.tab.c"
    break;

  case 68: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 413 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2405 "raku.tab.c"
    break;

  case 69: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 417 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2411 "raku.tab.c"
    break;

  case 70: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 421 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2417 "raku.tab.c"
    break;

  case 71: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 423 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2423 "raku.tab.c"
    break;

  case 72: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 427 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2429 "raku.tab.c"
    break;

  case 73: /* repeat_stmt: KW_REPEAT block  */
#line 431 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2435 "raku.tab.c"
    break;

  case 74: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 435 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2445 "raku.tab.c"
    break;

  case 75: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 441 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2455 "raku.tab.c"
    break;

  case 76: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 447 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2464 "raku.tab.c"
    break;

  case 77: /* for_stmt: KW_FOR expr block  */
#line 452 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2471 "raku.tab.c"
    break;

  case 78: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 457 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2483 "raku.tab.c"
    break;

  case 79: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 465 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2496 "raku.tab.c"
    break;

  case 80: /* when_list: %empty  */
#line 475 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2502 "raku.tab.c"
    break;

  case 81: /* when_list: when_list KW_WHEN expr block  */
#line 477 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2510 "raku.tab.c"
    break;

  case 82: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 483 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2522 "raku.tab.c"
    break;

  case 83: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 491 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2532 "raku.tab.c"
    break;

  case 84: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 497 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2545 "raku.tab.c"
    break;

  case 85: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 506 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2556 "raku.tab.c"
    break;

  case 86: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 513 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2569 "raku.tab.c"
    break;

  case 87: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 524 "raku.y"
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
#line 2587 "raku.tab.c"
    break;

  case 88: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 540 "raku.y"
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
#line 2604 "raku.tab.c"
    break;

  case 89: /* is_clauses: %empty  */
#line 554 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2610 "raku.tab.c"
    break;

  case 90: /* is_clauses: is_clauses IDENT IDENT  */
#line 556 "raku.y"
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
#line 2626 "raku.tab.c"
    break;

  case 91: /* is_clauses: is_clauses TESTOP IDENT  */
#line 568 "raku.y"
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
#line 2642 "raku.tab.c"
    break;

  case 92: /* class_body_list: %empty  */
#line 581 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2648 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 583 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2655 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 586 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2662 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 589 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2669 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 592 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2676 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 595 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2683 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 598 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2690 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 601 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2697 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 604 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2704 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 607 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2711 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 610 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2718 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 613 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2725 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 616 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2732 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 619 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2743 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 626 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2754 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 633 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2765 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 640 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2776 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 647 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2784 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 651 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2792 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 655 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2800 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 659 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2808 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 663 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2819 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 670 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2830 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 677 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2841 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 684 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2852 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 691 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2866 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 701 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2878 "raku.tab.c"
    break;

  case 119: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 709 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2891 "raku.tab.c"
    break;

  case 120: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 718 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2902 "raku.tab.c"
    break;

  case 121: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 725 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2916 "raku.tab.c"
    break;

  case 122: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 735 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2928 "raku.tab.c"
    break;

  case 123: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 745 "raku.y"
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
#line 2945 "raku.tab.c"
    break;

  case 124: /* grammar_body_list: %empty  */
#line 759 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2951 "raku.tab.c"
    break;

  case 125: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 761 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2960 "raku.tab.c"
    break;

  case 126: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 766 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2969 "raku.tab.c"
    break;

  case 127: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 771 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2978 "raku.tab.c"
    break;

  case 128: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 778 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2986 "raku.tab.c"
    break;

  case 129: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 782 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2994 "raku.tab.c"
    break;

  case 130: /* pair_list: IDENT OP_FATARROW expr  */
#line 788 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3000 "raku.tab.c"
    break;

  case 131: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 790 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3006 "raku.tab.c"
    break;

  case 132: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 792 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3012 "raku.tab.c"
    break;

  case 133: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 794 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3018 "raku.tab.c"
    break;

  case 134: /* param_list: VAR_SCALAR  */
#line 797 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3024 "raku.tab.c"
    break;

  case 135: /* param_list: IDENT VAR_SCALAR  */
#line 798 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3030 "raku.tab.c"
    break;

  case 136: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 799 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3036 "raku.tab.c"
    break;

  case 137: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 800 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3042 "raku.tab.c"
    break;

  case 138: /* param_list: param_list ',' VAR_SCALAR  */
#line 801 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3048 "raku.tab.c"
    break;

  case 139: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 802 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3054 "raku.tab.c"
    break;

  case 140: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 803 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3060 "raku.tab.c"
    break;

  case 141: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 804 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3066 "raku.tab.c"
    break;

  case 142: /* block: '{' stmt_list '}'  */
#line 807 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3072 "raku.tab.c"
    break;

  case 143: /* block: '{' YADA '}'  */
#line 808 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3078 "raku.tab.c"
    break;

  case 144: /* closure: '{' expr '}'  */
#line 811 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3084 "raku.tab.c"
    break;

  case 145: /* expr: VAR_SCALAR '=' expr  */
#line 814 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3090 "raku.tab.c"
    break;

  case 146: /* expr: KW_GATHER block  */
#line 815 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3100 "raku.tab.c"
    break;

  case 147: /* expr: cmp_expr  */
#line 820 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3106 "raku.tab.c"
    break;

  case 148: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 823 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3112 "raku.tab.c"
    break;

  case 149: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 824 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3118 "raku.tab.c"
    break;

  case 150: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 825 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3124 "raku.tab.c"
    break;

  case 151: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 826 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3130 "raku.tab.c"
    break;

  case 152: /* cmp_expr: jct_expr '<' jct_expr  */
#line 827 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3136 "raku.tab.c"
    break;

  case 153: /* cmp_expr: jct_expr '>' jct_expr  */
#line 828 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3142 "raku.tab.c"
    break;

  case 154: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 829 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3148 "raku.tab.c"
    break;

  case 155: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 830 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3154 "raku.tab.c"
    break;

  case 156: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 831 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3160 "raku.tab.c"
    break;

  case 157: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 832 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3166 "raku.tab.c"
    break;

  case 158: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 834 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3176 "raku.tab.c"
    break;

  case 159: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 840 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3186 "raku.tab.c"
    break;

  case 160: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 846 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3196 "raku.tab.c"
    break;

  case 161: /* cmp_expr: jct_expr  */
#line 851 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3202 "raku.tab.c"
    break;

  case 162: /* jct_expr: jct_expr '|' range_expr  */
#line 854 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3208 "raku.tab.c"
    break;

  case 163: /* jct_expr: jct_expr '&' range_expr  */
#line 855 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3214 "raku.tab.c"
    break;

  case 164: /* jct_expr: range_expr  */
#line 856 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3220 "raku.tab.c"
    break;

  case 165: /* range_expr: add_expr OP_RANGE add_expr  */
#line 859 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3226 "raku.tab.c"
    break;

  case 166: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 860 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3232 "raku.tab.c"
    break;

  case 167: /* range_expr: add_expr  */
#line 861 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3238 "raku.tab.c"
    break;

  case 168: /* add_expr: add_expr '+' mul_expr  */
#line 864 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3244 "raku.tab.c"
    break;

  case 169: /* add_expr: add_expr '-' mul_expr  */
#line 865 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3250 "raku.tab.c"
    break;

  case 170: /* add_expr: add_expr '~' mul_expr  */
#line 866 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3256 "raku.tab.c"
    break;

  case 171: /* add_expr: mul_expr  */
#line 867 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 3262 "raku.tab.c"
    break;

  case 172: /* mul_expr: mul_expr '*' unary_expr  */
#line 870 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3268 "raku.tab.c"
    break;

  case 173: /* mul_expr: mul_expr '/' unary_expr  */
#line 871 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3274 "raku.tab.c"
    break;

  case 174: /* mul_expr: mul_expr '%' unary_expr  */
#line 872 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3280 "raku.tab.c"
    break;

  case 175: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 873 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3286 "raku.tab.c"
    break;

  case 176: /* mul_expr: unary_expr  */
#line 874 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3292 "raku.tab.c"
    break;

  case 177: /* unary_expr: '-' unary_expr  */
#line 877 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3298 "raku.tab.c"
    break;

  case 178: /* unary_expr: '!' unary_expr  */
#line 878 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3304 "raku.tab.c"
    break;

  case 179: /* unary_expr: postfix_expr  */
#line 879 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3310 "raku.tab.c"
    break;

  case 180: /* postfix_expr: call_expr  */
#line 881 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3316 "raku.tab.c"
    break;

  case 181: /* call_expr: IDENT '(' arg_list ')'  */
#line 884 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3325 "raku.tab.c"
    break;

  case 182: /* call_expr: IDENT '(' ')'  */
#line 888 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3331 "raku.tab.c"
    break;

  case 183: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 890 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3340 "raku.tab.c"
    break;

  case 184: /* call_expr: VAR_SCALAR '(' ')'  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3346 "raku.tab.c"
    break;

  case 185: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 897 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3356 "raku.tab.c"
    break;

  case 186: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 903 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3366 "raku.tab.c"
    break;

  case 187: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 909 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3374 "raku.tab.c"
    break;

  case 188: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 913 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3385 "raku.tab.c"
    break;

  case 189: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 920 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3394 "raku.tab.c"
    break;

  case 190: /* call_expr: IDENT '.' CARET IDENT  */
#line 925 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3404 "raku.tab.c"
    break;

  case 191: /* call_expr: atom '.' CARET IDENT  */
#line 931 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3414 "raku.tab.c"
    break;

  case 192: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 937 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3425 "raku.tab.c"
    break;

  case 193: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 944 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3436 "raku.tab.c"
    break;

  case 194: /* call_expr: atom '.' IDENT '(' ')'  */
#line 951 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3445 "raku.tab.c"
    break;

  case 195: /* call_expr: atom '.' IDENT  */
#line 956 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3454 "raku.tab.c"
    break;

  case 196: /* call_expr: KW_DIE expr  */
#line 961 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3460 "raku.tab.c"
    break;

  case 197: /* call_expr: KW_MAP closure expr  */
#line 963 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3466 "raku.tab.c"
    break;

  case 198: /* call_expr: KW_GREP closure expr  */
#line 965 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3472 "raku.tab.c"
    break;

  case 199: /* call_expr: KW_SORT expr  */
#line 967 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3478 "raku.tab.c"
    break;

  case 200: /* call_expr: KW_SORT closure expr  */
#line 969 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3484 "raku.tab.c"
    break;

  case 201: /* call_expr: atom  */
#line 970 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3490 "raku.tab.c"
    break;

  case 202: /* arg_list: expr  */
#line 973 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3496 "raku.tab.c"
    break;

  case 203: /* arg_list: arg_list ',' expr  */
#line 974 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3502 "raku.tab.c"
    break;

  case 204: /* atom: LIT_INT  */
#line 977 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3508 "raku.tab.c"
    break;

  case 205: /* atom: LIT_FLOAT  */
#line 978 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3514 "raku.tab.c"
    break;

  case 206: /* atom: LIT_STR  */
#line 979 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3520 "raku.tab.c"
    break;

  case 207: /* atom: WORDLIST  */
#line 981 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3532 "raku.tab.c"
    break;

  case 208: /* atom: LIT_INTERP_STR  */
#line 988 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3538 "raku.tab.c"
    break;

  case 209: /* atom: VAR_SCALAR  */
#line 989 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3544 "raku.tab.c"
    break;

  case 210: /* atom: VAR_ARRAY  */
#line 990 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3550 "raku.tab.c"
    break;

  case 211: /* atom: VAR_HASH  */
#line 991 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3556 "raku.tab.c"
    break;

  case 212: /* atom: VAR_CAPTURE  */
#line 993 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3564 "raku.tab.c"
    break;

  case 213: /* atom: VAR_FH  */
#line 997 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3572 "raku.tab.c"
    break;

  case 214: /* atom: VAR_NAMED_CAPTURE  */
#line 1001 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3579 "raku.tab.c"
    break;

  case 215: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1004 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3585 "raku.tab.c"
    break;

  case 216: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1006 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3591 "raku.tab.c"
    break;

  case 217: /* atom: VAR_HASH '{' expr '}'  */
#line 1008 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3597 "raku.tab.c"
    break;

  case 218: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1010 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3603 "raku.tab.c"
    break;

  case 219: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1012 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3609 "raku.tab.c"
    break;

  case 220: /* atom: IDENT  */
#line 1013 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3615 "raku.tab.c"
    break;

  case 221: /* atom: VAR_TWIGIL  */
#line 1015 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3623 "raku.tab.c"
    break;

  case 222: /* atom: VAR_ARRAY_TWIGIL  */
#line 1019 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3631 "raku.tab.c"
    break;

  case 223: /* atom: VAR_HASH_TWIGIL  */
#line 1023 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3639 "raku.tab.c"
    break;

  case 224: /* atom: '(' expr ')'  */
#line 1026 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3645 "raku.tab.c"
    break;

  case 225: /* atom: block  */
#line 1027 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3651 "raku.tab.c"
    break;

  case 226: /* atom: KW_SUB block  */
#line 1028 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3657 "raku.tab.c"
    break;


#line 3661 "raku.tab.c"

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

#line 1030 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
