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

#line 232 "raku.tab.c"

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
  YYSYMBOL_KW_MY = 21,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 22,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 23,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 24,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 25,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 26,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 27,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 28,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 29,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 30,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 31,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 32,                 /* KW_RETURN  */
  YYSYMBOL_KW_GIVEN = 33,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 34,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 35,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 36,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 37,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 38,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 39,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 40,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 41,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 42,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 43,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 44,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 45,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 46,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 47,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 48,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 49,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 50,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 51,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 52,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 53,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 54,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 55,                /* KW_HANDLES  */
  YYSYMBOL_OP_COLON_D = 56,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 57,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 58,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 59,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 60,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 61,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 62,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 63,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 64,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 65,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 66,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 67,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 68,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 69,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 70,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 71,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 72,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 73,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 74,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 75,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 76,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 77,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 78,                    /* OP_DIV  */
  YYSYMBOL_79_ = 79,                       /* '='  */
  YYSYMBOL_80_ = 80,                       /* '!'  */
  YYSYMBOL_81_ = 81,                       /* '<'  */
  YYSYMBOL_82_ = 82,                       /* '>'  */
  YYSYMBOL_83_ = 83,                       /* '|'  */
  YYSYMBOL_84_ = 84,                       /* '&'  */
  YYSYMBOL_85_ = 85,                       /* '~'  */
  YYSYMBOL_86_ = 86,                       /* '+'  */
  YYSYMBOL_87_ = 87,                       /* '-'  */
  YYSYMBOL_88_ = 88,                       /* '*'  */
  YYSYMBOL_89_ = 89,                       /* '/'  */
  YYSYMBOL_90_ = 90,                       /* '%'  */
  YYSYMBOL_UMINUS = 91,                    /* UMINUS  */
  YYSYMBOL_92_ = 92,                       /* '.'  */
  YYSYMBOL_93_ = 93,                       /* ';'  */
  YYSYMBOL_94_ = 94,                       /* ','  */
  YYSYMBOL_95_ = 95,                       /* '('  */
  YYSYMBOL_96_ = 96,                       /* ')'  */
  YYSYMBOL_97_ = 97,                       /* '['  */
  YYSYMBOL_98_ = 98,                       /* ']'  */
  YYSYMBOL_99_ = 99,                       /* '{'  */
  YYSYMBOL_100_ = 100,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 101,                 /* $accept  */
  YYSYMBOL_program = 102,                  /* program  */
  YYSYMBOL_stmt_list = 103,                /* stmt_list  */
  YYSYMBOL_stmt = 104,                     /* stmt  */
  YYSYMBOL_if_stmt = 105,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 106,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 107,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 108,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 109,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 110,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 111,               /* given_stmt  */
  YYSYMBOL_when_list = 112,                /* when_list  */
  YYSYMBOL_sub_decl = 113,                 /* sub_decl  */
  YYSYMBOL_class_decl = 114,               /* class_decl  */
  YYSYMBOL_role_decl = 115,                /* role_decl  */
  YYSYMBOL_is_clauses = 116,               /* is_clauses  */
  YYSYMBOL_class_body_list = 117,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 118,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 119,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 120,           /* named_arg_list  */
  YYSYMBOL_param_list = 121,               /* param_list  */
  YYSYMBOL_block = 122,                    /* block  */
  YYSYMBOL_closure = 123,                  /* closure  */
  YYSYMBOL_expr = 124,                     /* expr  */
  YYSYMBOL_cmp_expr = 125,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 126,                 /* jct_expr  */
  YYSYMBOL_range_expr = 127,               /* range_expr  */
  YYSYMBOL_add_expr = 128,                 /* add_expr  */
  YYSYMBOL_mul_expr = 129,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 130,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 131,             /* postfix_expr  */
  YYSYMBOL_call_expr = 132,                /* call_expr  */
  YYSYMBOL_arg_list = 133,                 /* arg_list  */
  YYSYMBOL_atom = 134                      /* atom  */
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
#define YYLAST   1191

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  199
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  503

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   334


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
       2,     2,     2,    80,     2,     2,     2,    90,    84,     2,
      95,    96,    88,    86,    94,    87,    92,    89,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    93,
      81,    79,    82,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    97,     2,    98,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    99,    83,   100,    85,     2,     2,     2,
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
      75,    76,    77,    78,    91
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   226,   226,   237,   238,   241,   243,   245,   247,   251,
     255,   257,   259,   261,   265,   269,   271,   273,   275,   277,
     279,   281,   283,   285,   287,   289,   291,   293,   298,   302,
     306,   311,   315,   322,   327,   330,   333,   336,   339,   342,
     343,   344,   345,   346,   347,   349,   351,   352,   353,   354,
     355,   356,   357,   360,   362,   364,   368,   372,   374,   378,
     382,   386,   392,   398,   403,   408,   416,   427,   428,   434,
     442,   448,   457,   464,   475,   491,   506,   507,   521,   522,
     525,   528,   531,   534,   537,   540,   543,   546,   549,   552,
     555,   558,   565,   572,   576,   580,   584,   588,   595,   602,
     612,   620,   629,   636,   646,   656,   671,   672,   677,   682,
     689,   693,   699,   700,   701,   702,   703,   704,   705,   706,
     709,   710,   713,   716,   717,   722,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   741,   747,   753,
     756,   757,   758,   761,   762,   763,   766,   767,   768,   769,
     772,   773,   774,   775,   776,   779,   780,   781,   783,   785,
     790,   791,   797,   803,   807,   814,   819,   825,   831,   838,
     845,   850,   855,   857,   859,   861,   863,   865,   868,   869,
     872,   873,   874,   875,   876,   877,   878,   879,   883,   887,
     890,   892,   894,   896,   898,   900,   901,   905,   909,   913
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
  "VAR_FH", "VAR_NAMED_CAPTURE", "KW_MY", "KW_SAY", "KW_PRINT", "KW_IF",
  "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR", "KW_SUB", "KW_GATHER",
  "KW_TAKE", "KW_RETURN", "KW_GIVEN", "KW_WHEN", "KW_DEFAULT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_MAP", "KW_GREP",
  "KW_SORT", "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD",
  "KW_HAS", "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME",
  "KW_HANDLES", "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR",
  "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW", "OP_RANGE",
  "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ",
  "OP_SNE", "OP_AND", "OP_OR", "OP_BIND", "OP_DOTEQ", "OP_SMATCH",
  "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['",
  "']'", "'{'", "'}'", "$accept", "program", "stmt_list", "stmt",
  "if_stmt", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "for_stmt", "given_stmt", "when_list", "sub_decl", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "param_list", "block", "closure",
  "expr", "cmp_expr", "jct_expr", "range_expr", "add_expr", "mul_expr",
  "unary_expr", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-280)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -280,    52,   850,  -280,  -280,  -280,  -280,  -280,    97,    -4,
     -51,    30,    59,  -280,  -280,  -280,  -280,  -280,    71,   908,
     955,    31,   103,  1002,   179,   124,  1002,   262,  1002,   218,
     230,   150,   156,   124,   154,   154,   472,   124,  1002,   256,
     265,   255,   271,   117,   117,  1002,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,   197,    76,
     245,  -280,   122,   144,  -280,  -280,  -280,   202,   292,  1002,
     310,  1002,   339,  1002,  1002,    20,   519,   -39,   277,   293,
     191,   301,   284,   -34,  -280,  1002,   296,  1002,   297,  1002,
    1002,   -55,   131,   305,   346,  -280,   328,  -280,   338,   313,
     -24,   -10,  1002,  1002,  -280,  1002,  1002,  1002,  1002,  -280,
     391,  -280,  -280,   340,    15,   341,  -280,  -280,  -280,   345,
    -280,   117,   117,   117,   117,   117,   117,   117,   117,   220,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   157,    -7,   344,   -18,   348,   361,   349,
     351,   353,   437,   358,  -280,  -280,   175,  1002,  -280,  1049,
    1002,    46,    62,    98,  1002,  1002,   440,  1002,   205,  -280,
     206,  -280,   360,   363,   447,  -280,   117,   117,     0,   362,
     355,  -280,  -280,  -280,   446,  1002,   449,  1002,   365,   368,
     366,  -280,  -280,  -280,   124,    -8,  -280,   370,   372,  -280,
    -280,    95,    95,    95,    95,    95,    95,    95,    95,  -280,
    -280,  -280,    95,    95,  -280,  -280,   152,   152,   144,   144,
     144,  -280,  -280,  -280,  -280,   374,   457,  -280,   575,  -280,
    1002,   622,   393,   394,   400,  -280,   669,  -280,   716,  1002,
    -280,   387,  1002,    90,   388,  1002,  -280,  1096,  -280,  1002,
    -280,  -280,   395,   407,   396,  1002,  1002,   124,   124,   124,
     134,   170,  -280,     7,   124,   224,  -280,  -280,    38,   412,
     397,   413,   398,   124,   124,  -280,  -280,   485,  -280,   -13,
       2,    41,     6,   763,  -280,   408,   225,   410,   411,   229,
    1002,  1002,  1002,  -280,   237,   -36,  -280,   238,   241,  -280,
    -280,   242,  -280,  1002,  -280,   414,  1002,   112,   416,  -280,
    -280,  -280,   404,   409,   481,  -280,  -280,   500,   501,  -280,
     502,   506,  -280,   138,   124,  1002,   124,  -280,  -280,  -280,
     424,   426,   495,  -280,  -280,    -3,    12,   273,   473,  -280,
     124,   249,   250,   512,   513,   514,  -280,  -280,   254,   258,
    -280,   443,  -280,  -280,   448,   450,   451,   452,  -280,  1002,
     526,  -280,  -280,  1002,   121,  -280,   309,  -280,  1002,  -280,
     453,   454,   -15,   124,   124,  -280,  -280,  -280,    43,  -280,
     124,   442,  -280,  -280,   124,  -280,   455,   456,    87,    88,
     233,   460,   461,   534,  -280,   124,   124,   549,   550,   551,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,   503,   314,
    -280,  1002,   165,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
     553,   554,  -280,  -280,  -280,     8,     9,   555,   475,  1002,
    -280,   556,   479,  1002,  -280,    89,   120,   480,   482,  -280,
    -280,   487,  -280,  -280,  -280,  -280,  -280,  1002,   483,   315,
    -280,  -280,  -280,   124,   319,   124,   322,   484,  -280,   490,
     491,  -280,   499,   560,   504,  1002,  -280,   582,   505,  1002,
    -280,  -280,  -280,    28,  -280,  -280,   507,  -280,   124,  -280,
     124,  -280,  -280,  -280,  -280,   508,  -280,   509,   510,  -280,
     511,   124,   323,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
    -280,   124,  -280
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   180,   181,   182,   183,   184,   185,
     186,   196,   195,   197,   198,   187,   188,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,    40,    41,    46,
      47,    48,    42,    43,    49,    50,    51,    52,     0,   125,
     139,   142,   145,   149,   154,   157,   158,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   184,   185,   186,   196,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     3,   124,     0,    25,     0,     0,
       0,     0,     0,     0,    60,     0,     0,     0,     0,   175,
      44,   172,    76,     0,     0,     0,   184,   156,   155,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   160,   178,     0,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,    21,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    23,    24,    67,     0,     0,     0,     0,     0,     0,
       0,   173,   174,   176,     0,     0,    78,     0,     0,   106,
     199,   126,   127,   128,   129,   132,   133,   134,   135,   136,
     137,   138,   130,   131,   140,   141,   143,   144,   148,   146,
     147,   153,   150,   151,   152,   171,     0,    29,     0,    26,
       0,     0,   190,   191,   192,    31,     0,   166,     0,     0,
     159,     0,     0,     0,     0,     0,    16,     0,    17,     0,
      18,   123,     0,     0,     0,     0,     0,     0,     0,     0,
     143,   144,   112,     0,     0,     0,   121,   120,     0,     0,
       0,     0,     0,     0,     0,   122,    45,     0,    78,     0,
       0,     0,     0,     0,   167,     0,     0,     0,     0,     0,
       0,     0,     0,   165,     0,   195,   163,     0,     0,   179,
       5,     0,     7,     0,    10,     0,     0,     0,     0,   190,
     191,   192,     0,     0,    53,    56,    63,     0,     0,   113,
       0,     0,    70,     0,     0,     0,     0,    65,   193,   194,
       0,     0,    57,    59,    77,     0,     0,     0,     0,    75,
       0,     0,     0,     0,     0,     0,   105,   170,     0,     0,
      28,     0,    30,    33,     0,     0,     0,     0,   164,     0,
       0,   161,   162,     0,     0,    11,     0,    12,     0,    15,
       0,     0,     0,     0,     0,   114,   115,   116,     0,    69,
       0,     0,    37,    38,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
     169,   168,    27,    32,    34,    35,    36,   110,     0,     0,
       8,     0,     0,    20,    22,    55,    54,    61,    62,   117,
       0,     0,    68,    66,    58,     0,     0,     0,     0,     0,
      84,     0,     0,     0,    79,     0,     0,     0,     0,    80,
      81,     0,    71,    73,   107,   108,   109,     0,     0,     0,
      13,   118,   119,     0,     0,     0,     0,     0,    93,     0,
       0,    94,     0,     0,     0,     0,    86,     0,     0,     0,
      85,    82,    83,     0,   111,     9,     0,   100,     0,   102,
       0,    92,    88,    91,    87,     0,    95,     0,     0,    96,
       0,     0,     0,    14,    99,   101,    98,    90,    97,    89,
     104,     0,   103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -280,  -280,   515,  -280,   235,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,   330,  -280,  -280,   327,
    -279,   -33,   274,     5,  -280,   302,    78,   -20,   126,   -23,
    -280,  -280,  -223,  -280
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    46,    47,    48,    49,    50,    51,    52,
      53,   268,    54,    55,    56,   195,   279,    57,   282,   297,
     265,    95,   106,   155,    59,    60,    61,    62,    63,    64,
      65,    66,   156,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     104,   341,   342,    92,   110,   286,   277,    58,   289,    21,
     262,   174,   262,   294,   263,   298,   263,   319,   262,   262,
     117,   118,   263,   263,    86,    88,   386,   359,    91,   197,
      72,    96,    98,    99,   151,   336,   337,   152,   262,   338,
     157,   109,   263,   111,    94,   336,   337,   166,    73,   338,
     119,   262,     3,   419,   158,   263,    75,   184,   175,    76,
     349,   230,   387,   320,   321,   167,   343,   344,   345,   198,
     153,   186,   325,   326,   145,   185,   147,   231,   149,   150,
     364,    77,    78,    79,    94,    80,   227,   339,   228,   187,
     168,   278,   170,    71,   172,   173,   264,   385,   340,   420,
     421,   427,   431,   463,   453,   455,   346,   188,   189,    74,
     190,   191,   192,   193,   216,   217,   221,   222,   223,   224,
       4,     5,     6,     7,   491,   245,    89,   116,    82,    83,
      84,    12,    13,    14,   467,    15,    16,    17,   327,   246,
     409,   247,   428,   432,   464,   412,   454,   456,   377,   121,
     122,    75,   378,    29,    76,   248,   260,   261,    34,    35,
      36,   276,   241,    38,   243,   244,   429,   433,   465,   251,
     252,   225,   254,    68,   226,   468,    69,   249,   132,   133,
     430,   434,   466,   302,   303,    58,   134,   135,   449,    70,
     270,   250,   272,    93,   492,   176,   177,    43,    90,   469,
     317,   161,   162,   163,    44,   367,   368,   136,   137,   138,
     214,   215,    45,   470,   410,   239,   136,   137,   138,   136,
     137,   138,   139,    94,   314,   315,   316,   209,   210,   211,
     100,   322,   140,   141,   142,   287,   318,   136,   137,   138,
     332,   333,   101,   435,   299,   102,   436,   301,   437,   438,
     305,   103,   307,   105,   308,   136,   137,   138,   450,   239,
     312,   313,   218,   219,   220,     4,     5,     6,     7,   239,
     112,   240,    81,    82,    83,    84,    12,    13,    14,   113,
      15,    16,    17,   388,   114,   115,   389,   390,   391,   392,
     120,   379,    25,   381,   143,   355,   356,   357,    29,   255,
     256,   200,   200,    34,    35,    36,   144,   394,    38,   107,
     108,   366,   123,   124,   125,   126,   127,   128,   323,   239,
     324,   351,   129,   239,   146,   354,   130,   131,   132,   133,
     380,   239,   360,   358,   361,   239,   363,   362,   200,   416,
     417,   418,    43,   323,   323,   395,   396,   422,   360,    44,
     400,   424,   239,   148,   401,    97,   159,    45,     4,     5,
       6,     7,   442,   443,   407,     8,     9,    10,    11,    12,
      13,    14,   160,    15,    16,    17,    18,    19,    20,    21,
     164,   165,    22,    23,    24,    25,    26,    27,    28,   169,
     171,    29,    30,    31,    32,    33,    34,    35,    36,    37,
     178,    38,    39,   411,   179,   200,    40,    41,   239,   239,
     448,   476,   183,   323,    42,   478,   323,   323,   480,   501,
     477,   181,   479,   201,   202,   203,   204,   205,   206,   207,
     208,   182,   212,   213,   459,    43,   194,   229,   462,   196,
     199,   200,    44,   233,   235,   494,   232,   495,   236,   234,
      45,   237,   474,   238,   253,   267,   257,   259,   500,   258,
     269,   273,   266,   271,   274,   280,   275,   281,   502,   283,
     487,   284,   290,   291,   490,     4,     5,     6,     7,   292,
     300,   304,    81,    82,    83,    84,    12,    13,    14,   310,
      15,    16,    17,   309,   328,   330,   311,   329,   331,   334,
     370,   350,    25,   352,   353,   371,   372,   365,    29,   369,
     373,   374,   375,    34,    35,    36,   376,   382,    38,   383,
     384,   393,     4,     5,     6,     7,   397,   398,   399,    81,
      82,    83,    84,    12,    13,    14,   402,    15,    16,    17,
     408,   403,   423,   404,   405,   406,   413,   414,   441,    25,
     425,   426,    43,   439,   440,    29,   444,   445,   446,    44,
      34,    35,    36,   451,   452,    38,   447,    45,   458,   457,
     460,   105,   461,   471,   485,   472,   475,   481,     4,     5,
       6,     7,   473,   482,   483,    81,    82,    83,    84,    12,
      13,    14,   484,    15,    16,    17,   488,   486,   489,    43,
     493,   496,   497,   498,   499,    25,    44,   415,   335,   180,
     348,    29,     0,     0,    45,   154,    34,    35,    36,     0,
       0,    38,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    43,     0,     0,    29,     0,
       0,     0,    44,    34,    35,    36,     0,     0,    38,     0,
      45,   285,     4,     5,     6,     7,     0,     0,     0,    81,
      82,    83,    84,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,    43,     0,     0,    29,     0,     0,     0,    44,
      34,    35,    36,     0,     0,    38,     0,    45,   288,     4,
       5,     6,     7,     0,     0,     0,    81,    82,    83,    84,
     295,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,    43,
       0,     0,    29,     0,     0,     0,    44,    34,    35,    36,
       0,     0,    38,     0,    45,   293,     4,     5,     6,     7,
       0,     0,     0,    81,    82,    83,    84,   295,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,    43,     0,     0,    29,
       0,     0,     0,    44,    34,    35,    36,     0,     0,    38,
       0,    45,   296,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     4,     5,     6,     7,     0,    45,   347,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    22,    23,    24,
      25,    26,    27,    28,     0,     0,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,    38,    39,     0,     0,
       0,    40,    41,     0,     0,     0,     0,     0,     0,    42,
       0,     4,     5,     6,     7,     0,     0,     0,    81,    82,
      83,    84,    12,    13,    14,     0,    15,    16,    17,     0,
      43,     0,     0,     0,     0,     0,     0,    44,    25,     0,
       0,     0,     0,     0,    29,    45,     0,     0,     0,    34,
      35,    36,     0,     0,    38,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    81,    82,    83,    84,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,    43,     0,
       0,    29,     0,     0,     0,    44,    34,    35,    36,     0,
       0,    38,     0,    85,     0,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    43,     0,     0,    29,     0,
       0,     0,    44,    34,    35,    36,     0,     0,    38,     0,
      87,     0,     4,     5,     6,     7,     0,     0,     0,    81,
      82,    83,    84,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,    43,     0,     0,    29,     0,     0,     0,    44,
      34,    35,    36,     0,     0,    38,     0,    45,     0,     4,
       5,     6,     7,     0,     0,     0,    81,    82,    83,    84,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,    43,
       0,     0,    29,     0,     0,     0,    44,    34,    35,    36,
       0,     0,    38,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,    44,     0,     0,     0,     0,     0,     0,
       0,   306
};

static const yytype_int16 yycheck[] =
{
      33,   280,   281,    23,    37,   228,    14,     2,   231,    24,
      10,    66,    10,   236,    14,   238,    14,    10,    10,    10,
      43,    44,    14,    14,    19,    20,    14,    63,    23,    14,
      81,    26,    27,    28,    14,    48,    49,    17,    10,    52,
      79,    36,    14,    38,    99,    48,    49,    81,    99,    52,
      45,    10,     0,    10,    93,    14,    92,    81,    91,    95,
     283,    79,    50,    56,    57,    99,    60,    61,    62,    54,
      50,    81,    34,    35,    69,    99,    71,    95,    73,    74,
     303,    10,    11,    12,    99,    14,    93,   100,    95,    99,
      85,    99,    87,    97,    89,    90,    96,   100,    96,    56,
      57,    14,    14,    14,    96,    96,   100,   102,   103,    79,
     105,   106,   107,   108,   134,   135,   139,   140,   141,   142,
       3,     4,     5,     6,    96,    79,    95,    10,    11,    12,
      13,    14,    15,    16,    14,    18,    19,    20,   100,    93,
     363,    79,    55,    55,    55,   368,   425,   426,    10,    73,
      74,    92,    14,    36,    95,    93,   176,   177,    41,    42,
      43,   194,   157,    46,   159,   160,    79,    79,    79,   164,
     165,    14,   167,    76,    17,    55,    79,    79,    83,    84,
      93,    93,    93,    93,    94,   180,    64,    65,   411,    92,
     185,    93,   187,    14,   473,    64,    65,    80,    95,    79,
      66,    10,    11,    12,    87,    93,    94,    85,    86,    87,
     132,   133,    95,    93,    93,    94,    85,    86,    87,    85,
      86,    87,    78,    99,   257,   258,   259,     7,     8,     9,
      12,   264,    88,    89,    90,   230,    66,    85,    86,    87,
     273,   274,    12,    10,   239,    95,    13,   242,    15,    16,
     245,    95,   247,    99,   249,    85,    86,    87,    93,    94,
     255,   256,   136,   137,   138,     3,     4,     5,     6,    94,
      14,    96,    10,    11,    12,    13,    14,    15,    16,    14,
      18,    19,    20,    10,    29,    14,    13,    14,    15,    16,
      93,   324,    30,   326,    92,   290,   291,   292,    36,    94,
      94,    96,    96,    41,    42,    43,    14,   340,    46,    35,
      36,   306,    67,    68,    69,    70,    71,    72,    94,    94,
      96,    96,    77,    94,    14,    96,    81,    82,    83,    84,
     325,    94,    94,    96,    96,    94,    94,    96,    96,   372,
     373,   374,    80,    94,    94,    96,    96,   380,    94,    87,
      96,   384,    94,    14,    96,    93,    79,    95,     3,     4,
       5,     6,   395,   396,   359,    10,    11,    12,    13,    14,
      15,    16,    79,    18,    19,    20,    21,    22,    23,    24,
      79,    97,    27,    28,    29,    30,    31,    32,    33,    93,
      93,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      95,    46,    47,    94,    58,    96,    51,    52,    94,    94,
      96,    96,    99,    94,    59,    96,    94,    94,    96,    96,
     453,    93,   455,   121,   122,   123,   124,   125,   126,   127,
     128,    93,   130,   131,   429,    80,    45,    93,   433,    99,
      99,    96,    87,    82,    93,   478,    98,   480,    95,   100,
      95,    14,   447,    95,    14,   100,    96,    10,   491,    96,
      14,    96,   100,    14,    96,    95,   100,    95,   501,    95,
     465,    14,    79,    79,   469,     3,     4,     5,     6,    79,
      93,    93,    10,    11,    12,    13,    14,    15,    16,    82,
      18,    19,    20,    98,    82,    82,   100,   100,   100,    14,
      96,    93,    30,    93,    93,    96,    25,    93,    36,    93,
      10,    10,    10,    41,    42,    43,    10,    93,    46,    93,
      25,    48,     3,     4,     5,     6,    14,    14,    14,    10,
      11,    12,    13,    14,    15,    16,    93,    18,    19,    20,
      14,    93,   100,    93,    93,    93,    93,    93,    14,    30,
      95,    95,    80,    93,    93,    36,     7,     7,     7,    87,
      41,    42,    43,    10,    10,    46,    63,    95,    93,    14,
      14,    99,    93,    93,    14,    93,    93,    93,     3,     4,
       5,     6,    95,    93,    93,    10,    11,    12,    13,    14,
      15,    16,    93,    18,    19,    20,    14,    93,    93,    80,
      93,    93,    93,    93,    93,    30,    87,   372,   278,    94,
     283,    36,    -1,    -1,    95,    96,    41,    42,    43,    -1,
      -1,    46,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    80,    -1,    -1,    36,    -1,
      -1,    -1,    87,    41,    42,    43,    -1,    -1,    46,    -1,
      95,    96,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    80,    -1,    -1,    36,    -1,    -1,    -1,    87,
      41,    42,    43,    -1,    -1,    46,    -1,    95,    96,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    80,
      -1,    -1,    36,    -1,    -1,    -1,    87,    41,    42,    43,
      -1,    -1,    46,    -1,    95,    96,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    80,    -1,    -1,    36,
      -1,    -1,    -1,    87,    41,    42,    43,    -1,    -1,    46,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,     4,     5,     6,    -1,    95,    96,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    27,    28,    29,
      30,    31,    32,    33,    -1,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    47,    -1,    -1,
      -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    30,    -1,
      -1,    -1,    -1,    -1,    36,    95,    -1,    -1,    -1,    41,
      42,    43,    -1,    -1,    46,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    80,    -1,
      -1,    36,    -1,    -1,    -1,    87,    41,    42,    43,    -1,
      -1,    46,    -1,    95,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    80,    -1,    -1,    36,    -1,
      -1,    -1,    87,    41,    42,    43,    -1,    -1,    46,    -1,
      95,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    80,    -1,    -1,    36,    -1,    -1,    -1,    87,
      41,    42,    43,    -1,    -1,    46,    -1,    95,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    80,
      -1,    -1,    36,    -1,    -1,    -1,    87,    41,    42,    43,
      -1,    -1,    46,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   102,   103,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    27,    28,    29,    30,    31,    32,    33,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      51,    52,    59,    80,    87,    95,   104,   105,   106,   107,
     108,   109,   110,   111,   113,   114,   115,   118,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   134,    76,    79,
      92,    97,    81,    99,    79,    92,    95,    10,    11,    12,
      14,    10,    11,    12,    13,    95,   124,    95,   124,    95,
      95,   124,   128,    14,    99,   122,   124,    93,   124,   124,
      12,    12,    95,    95,   122,    99,   123,   123,   123,   124,
     122,   124,    14,    14,    29,    14,    10,   130,   130,   124,
      93,    73,    74,    67,    68,    69,    70,    71,    72,    77,
      81,    82,    83,    84,    64,    65,    85,    86,    87,    78,
      88,    89,    90,    92,    14,   124,    14,   124,    14,   124,
     124,    14,    17,    50,    96,   124,   133,    79,    93,    79,
      79,    10,    11,    12,    79,    97,    81,    99,   124,    93,
     124,    93,   124,   124,    66,   122,    64,    65,    95,    58,
     103,    93,    93,    99,    81,    99,    81,    99,   124,   124,
     124,   124,   124,   124,    45,   116,    99,    14,    54,    99,
      96,   126,   126,   126,   126,   126,   126,   126,   126,     7,
       8,     9,   126,   126,   127,   127,   128,   128,   129,   129,
     129,   130,   130,   130,   130,    14,    17,    93,    95,    93,
      79,    95,    98,    82,   100,    93,    95,    14,    95,    94,
      96,   124,    95,   124,   124,    79,    93,    79,    93,    79,
      93,   124,   124,    14,   124,    94,    94,    96,    96,    10,
     128,   128,    10,    14,    96,   121,   100,   100,   112,    14,
     124,    14,   124,    96,    96,   100,   122,    14,    99,   117,
      95,    95,   119,    95,    14,    96,   133,   124,    96,   133,
      79,    79,    79,    96,   133,    14,    96,   120,   133,   124,
      93,   124,    93,    94,    93,   124,    95,   124,   124,    98,
      82,   100,   124,   124,   122,   122,   122,    66,    66,    10,
      56,    57,   122,    94,    96,    34,    35,   100,    82,   100,
      82,   100,   122,   122,    14,   117,    48,    49,    52,   100,
      96,   121,   121,    60,    61,    62,   100,    96,   120,   133,
      93,    96,    93,    93,    96,   124,   124,   124,    96,    63,
      94,    96,    96,    94,   133,    93,   124,    93,    94,    93,
      96,    96,    25,    10,    10,    10,    10,    10,    14,   122,
     124,   122,    93,    93,    25,   100,    14,    50,    10,    13,
      14,    15,    16,    48,   122,    96,    96,    14,    14,    14,
      96,    96,    93,    93,    93,    93,    93,   124,    14,   133,
      93,    94,   133,    93,    93,   105,   122,   122,   122,    10,
      56,    57,   122,   100,   122,    95,    95,    14,    55,    79,
      93,    14,    55,    79,    93,    10,    13,    15,    16,    93,
      93,    14,   122,   122,     7,     7,     7,    63,    96,   133,
      93,    10,    10,    96,   121,    96,   121,    14,    93,   124,
      14,    93,   124,    14,    55,    79,    93,    14,    55,    79,
      93,    93,    93,    95,   124,    93,    96,   122,    96,   122,
      96,    93,    93,    93,    93,    14,    93,   124,    14,    93,
     124,    96,   121,    93,   122,   122,    93,    93,    93,    93,
     122,    96,   122
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   101,   102,   103,   103,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   105,   105,   105,   106,   107,   107,   108,
     109,   110,   110,   110,   110,   111,   111,   112,   112,   113,
     113,   113,   113,   113,   114,   115,   116,   116,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   118,   119,   119,   119,   119,
     120,   120,   121,   121,   121,   121,   121,   121,   121,   121,
     122,   122,   123,   124,   124,   124,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     126,   126,   126,   127,   127,   127,   128,   128,   128,   128,
     129,   129,   129,   129,   129,   130,   130,   130,   131,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   133,   133,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     7,     9,
       5,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       7,     3,     7,     3,     3,     2,     4,     7,     6,     4,
       6,     4,     7,     6,     7,     7,     7,     6,     6,     2,
       1,     1,     1,     1,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     5,     7,     7,     5,     5,     7,     5,
       2,     7,     7,     5,     3,     5,     7,     0,     4,     6,
       5,     7,     6,     7,     6,     5,     0,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       6,     7,     6,     8,     7,     5,     0,     4,     4,     4,
       3,     5,     1,     2,     3,     3,     3,     4,     5,     5,
       3,     3,     3,     3,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     3,     3,     1,     2,     2,     1,     1,     4,
       3,     6,     6,     5,     6,     5,     4,     4,     6,     6,
       5,     3,     2,     3,     3,     2,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     5,     5,     1,     1,     1,     1,     3
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
#line 227 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1826 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 237 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1832 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 238 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1838 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 242 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1844 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 244 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1850 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 246 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1856 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 248 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1864 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 252 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1872 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 256 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1878 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 258 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1884 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 260 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1890 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 262 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1898 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 266 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1906 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 270 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1912 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 272 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1918 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 274 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1924 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 276 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1930 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY expr ';'  */
#line 278 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1936 "raku.tab.c"
    break;

  case 20: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 280 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1942 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT expr ';'  */
#line 282 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1948 "raku.tab.c"
    break;

  case 22: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 284 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1954 "raku.tab.c"
    break;

  case 23: /* stmt: KW_TAKE expr ';'  */
#line 286 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1960 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN expr ';'  */
#line 288 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1966 "raku.tab.c"
    break;

  case 25: /* stmt: KW_RETURN ';'  */
#line 290 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1972 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 292 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1978 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 294 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 1987 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 299 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 1995 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 303 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2003 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 307 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2012 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 312 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2020 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 316 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2031 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 323 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2040 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 328 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2047 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 331 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2054 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 334 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2061 "raku.tab.c"
    break;

  case 37: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 337 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2068 "raku.tab.c"
    break;

  case 38: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 340 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2075 "raku.tab.c"
    break;

  case 39: /* stmt: expr ';'  */
#line 342 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2081 "raku.tab.c"
    break;

  case 40: /* stmt: if_stmt  */
#line 343 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2087 "raku.tab.c"
    break;

  case 41: /* stmt: while_stmt  */
#line 344 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2093 "raku.tab.c"
    break;

  case 42: /* stmt: for_stmt  */
#line 345 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2099 "raku.tab.c"
    break;

  case 43: /* stmt: given_stmt  */
#line 346 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2105 "raku.tab.c"
    break;

  case 44: /* stmt: KW_TRY block  */
#line 348 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2111 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TRY block KW_CATCH block  */
#line 350 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2117 "raku.tab.c"
    break;

  case 46: /* stmt: unless_stmt  */
#line 351 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2123 "raku.tab.c"
    break;

  case 47: /* stmt: until_stmt  */
#line 352 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2129 "raku.tab.c"
    break;

  case 48: /* stmt: repeat_stmt  */
#line 353 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2135 "raku.tab.c"
    break;

  case 49: /* stmt: sub_decl  */
#line 354 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2141 "raku.tab.c"
    break;

  case 50: /* stmt: class_decl  */
#line 355 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2147 "raku.tab.c"
    break;

  case 51: /* stmt: role_decl  */
#line 356 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2153 "raku.tab.c"
    break;

  case 52: /* stmt: grammar_decl  */
#line 357 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2159 "raku.tab.c"
    break;

  case 53: /* if_stmt: KW_IF '(' expr ')' block  */
#line 361 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2165 "raku.tab.c"
    break;

  case 54: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 363 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2171 "raku.tab.c"
    break;

  case 55: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 365 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2177 "raku.tab.c"
    break;

  case 56: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 369 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2183 "raku.tab.c"
    break;

  case 57: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 373 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2189 "raku.tab.c"
    break;

  case 58: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 375 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2195 "raku.tab.c"
    break;

  case 59: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 379 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2201 "raku.tab.c"
    break;

  case 60: /* repeat_stmt: KW_REPEAT block  */
#line 383 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2207 "raku.tab.c"
    break;

  case 61: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 387 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2217 "raku.tab.c"
    break;

  case 62: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 393 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2227 "raku.tab.c"
    break;

  case 63: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 399 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2236 "raku.tab.c"
    break;

  case 64: /* for_stmt: KW_FOR expr block  */
#line 404 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2243 "raku.tab.c"
    break;

  case 65: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 409 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2255 "raku.tab.c"
    break;

  case 66: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 417 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2268 "raku.tab.c"
    break;

  case 67: /* when_list: %empty  */
#line 427 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2274 "raku.tab.c"
    break;

  case 68: /* when_list: when_list KW_WHEN expr block  */
#line 429 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2282 "raku.tab.c"
    break;

  case 69: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 435 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2294 "raku.tab.c"
    break;

  case 70: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 443 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2304 "raku.tab.c"
    break;

  case 71: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 449 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2317 "raku.tab.c"
    break;

  case 72: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 458 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2328 "raku.tab.c"
    break;

  case 73: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 465 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2341 "raku.tab.c"
    break;

  case 74: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 476 "raku.y"
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
#line 2359 "raku.tab.c"
    break;

  case 75: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 492 "raku.y"
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
#line 2376 "raku.tab.c"
    break;

  case 76: /* is_clauses: %empty  */
#line 506 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2382 "raku.tab.c"
    break;

  case 77: /* is_clauses: is_clauses IDENT IDENT  */
#line 508 "raku.y"
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
#line 2398 "raku.tab.c"
    break;

  case 78: /* class_body_list: %empty  */
#line 521 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2404 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 523 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2411 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 526 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2418 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 529 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2425 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 532 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2432 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 535 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2439 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 538 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2446 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 541 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2453 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 544 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2460 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2467 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 550 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2474 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 553 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2481 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 556 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2488 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 559 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2499 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 566 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2510 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 573 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2518 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 577 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2526 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 581 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2534 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 585 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2542 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 589 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2553 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 596 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2564 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 603 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2578 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 613 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2590 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 621 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2603 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 630 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2614 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 637 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2628 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 647 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2640 "raku.tab.c"
    break;

  case 105: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 657 "raku.y"
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
#line 2657 "raku.tab.c"
    break;

  case 106: /* grammar_body_list: %empty  */
#line 671 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2663 "raku.tab.c"
    break;

  case 107: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 673 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2672 "raku.tab.c"
    break;

  case 108: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 678 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2681 "raku.tab.c"
    break;

  case 109: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 683 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2690 "raku.tab.c"
    break;

  case 110: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 690 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2698 "raku.tab.c"
    break;

  case 111: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 694 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2706 "raku.tab.c"
    break;

  case 112: /* param_list: VAR_SCALAR  */
#line 699 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2712 "raku.tab.c"
    break;

  case 113: /* param_list: IDENT VAR_SCALAR  */
#line 700 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2718 "raku.tab.c"
    break;

  case 114: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 701 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2724 "raku.tab.c"
    break;

  case 115: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 702 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2730 "raku.tab.c"
    break;

  case 116: /* param_list: param_list ',' VAR_SCALAR  */
#line 703 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2736 "raku.tab.c"
    break;

  case 117: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 704 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2742 "raku.tab.c"
    break;

  case 118: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 705 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2748 "raku.tab.c"
    break;

  case 119: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 706 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2754 "raku.tab.c"
    break;

  case 120: /* block: '{' stmt_list '}'  */
#line 709 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2760 "raku.tab.c"
    break;

  case 121: /* block: '{' YADA '}'  */
#line 710 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2766 "raku.tab.c"
    break;

  case 122: /* closure: '{' expr '}'  */
#line 713 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2772 "raku.tab.c"
    break;

  case 123: /* expr: VAR_SCALAR '=' expr  */
#line 716 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2778 "raku.tab.c"
    break;

  case 124: /* expr: KW_GATHER block  */
#line 717 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2788 "raku.tab.c"
    break;

  case 125: /* expr: cmp_expr  */
#line 722 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2794 "raku.tab.c"
    break;

  case 126: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 725 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2800 "raku.tab.c"
    break;

  case 127: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 726 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2806 "raku.tab.c"
    break;

  case 128: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 727 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2812 "raku.tab.c"
    break;

  case 129: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 728 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2818 "raku.tab.c"
    break;

  case 130: /* cmp_expr: jct_expr '<' jct_expr  */
#line 729 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2824 "raku.tab.c"
    break;

  case 131: /* cmp_expr: jct_expr '>' jct_expr  */
#line 730 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2830 "raku.tab.c"
    break;

  case 132: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 731 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2836 "raku.tab.c"
    break;

  case 133: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 732 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2842 "raku.tab.c"
    break;

  case 134: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 733 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2848 "raku.tab.c"
    break;

  case 135: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 734 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2854 "raku.tab.c"
    break;

  case 136: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 736 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2864 "raku.tab.c"
    break;

  case 137: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 742 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2874 "raku.tab.c"
    break;

  case 138: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 748 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2884 "raku.tab.c"
    break;

  case 139: /* cmp_expr: jct_expr  */
#line 753 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2890 "raku.tab.c"
    break;

  case 140: /* jct_expr: jct_expr '|' range_expr  */
#line 756 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2896 "raku.tab.c"
    break;

  case 141: /* jct_expr: jct_expr '&' range_expr  */
#line 757 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2902 "raku.tab.c"
    break;

  case 142: /* jct_expr: range_expr  */
#line 758 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2908 "raku.tab.c"
    break;

  case 143: /* range_expr: add_expr OP_RANGE add_expr  */
#line 761 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2914 "raku.tab.c"
    break;

  case 144: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 762 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2920 "raku.tab.c"
    break;

  case 145: /* range_expr: add_expr  */
#line 763 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2926 "raku.tab.c"
    break;

  case 146: /* add_expr: add_expr '+' mul_expr  */
#line 766 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2932 "raku.tab.c"
    break;

  case 147: /* add_expr: add_expr '-' mul_expr  */
#line 767 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2938 "raku.tab.c"
    break;

  case 148: /* add_expr: add_expr '~' mul_expr  */
#line 768 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2944 "raku.tab.c"
    break;

  case 149: /* add_expr: mul_expr  */
#line 769 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2950 "raku.tab.c"
    break;

  case 150: /* mul_expr: mul_expr '*' unary_expr  */
#line 772 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2956 "raku.tab.c"
    break;

  case 151: /* mul_expr: mul_expr '/' unary_expr  */
#line 773 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2962 "raku.tab.c"
    break;

  case 152: /* mul_expr: mul_expr '%' unary_expr  */
#line 774 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2968 "raku.tab.c"
    break;

  case 153: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 775 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2974 "raku.tab.c"
    break;

  case 154: /* mul_expr: unary_expr  */
#line 776 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2980 "raku.tab.c"
    break;

  case 155: /* unary_expr: '-' unary_expr  */
#line 779 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2986 "raku.tab.c"
    break;

  case 156: /* unary_expr: '!' unary_expr  */
#line 780 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2992 "raku.tab.c"
    break;

  case 157: /* unary_expr: postfix_expr  */
#line 781 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2998 "raku.tab.c"
    break;

  case 158: /* postfix_expr: call_expr  */
#line 783 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3004 "raku.tab.c"
    break;

  case 159: /* call_expr: IDENT '(' arg_list ')'  */
#line 786 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3013 "raku.tab.c"
    break;

  case 160: /* call_expr: IDENT '(' ')'  */
#line 790 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3019 "raku.tab.c"
    break;

  case 161: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 792 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3029 "raku.tab.c"
    break;

  case 162: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 798 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3039 "raku.tab.c"
    break;

  case 163: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 804 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3047 "raku.tab.c"
    break;

  case 164: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 808 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3058 "raku.tab.c"
    break;

  case 165: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 815 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3067 "raku.tab.c"
    break;

  case 166: /* call_expr: IDENT '.' CARET IDENT  */
#line 820 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3077 "raku.tab.c"
    break;

  case 167: /* call_expr: atom '.' CARET IDENT  */
#line 826 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3087 "raku.tab.c"
    break;

  case 168: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 832 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3098 "raku.tab.c"
    break;

  case 169: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 839 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3109 "raku.tab.c"
    break;

  case 170: /* call_expr: atom '.' IDENT '(' ')'  */
#line 846 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3118 "raku.tab.c"
    break;

  case 171: /* call_expr: atom '.' IDENT  */
#line 851 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3127 "raku.tab.c"
    break;

  case 172: /* call_expr: KW_DIE expr  */
#line 856 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3133 "raku.tab.c"
    break;

  case 173: /* call_expr: KW_MAP closure expr  */
#line 858 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3139 "raku.tab.c"
    break;

  case 174: /* call_expr: KW_GREP closure expr  */
#line 860 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3145 "raku.tab.c"
    break;

  case 175: /* call_expr: KW_SORT expr  */
#line 862 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3151 "raku.tab.c"
    break;

  case 176: /* call_expr: KW_SORT closure expr  */
#line 864 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3157 "raku.tab.c"
    break;

  case 177: /* call_expr: atom  */
#line 865 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3163 "raku.tab.c"
    break;

  case 178: /* arg_list: expr  */
#line 868 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3169 "raku.tab.c"
    break;

  case 179: /* arg_list: arg_list ',' expr  */
#line 869 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3175 "raku.tab.c"
    break;

  case 180: /* atom: LIT_INT  */
#line 872 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3181 "raku.tab.c"
    break;

  case 181: /* atom: LIT_FLOAT  */
#line 873 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3187 "raku.tab.c"
    break;

  case 182: /* atom: LIT_STR  */
#line 874 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3193 "raku.tab.c"
    break;

  case 183: /* atom: LIT_INTERP_STR  */
#line 875 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3199 "raku.tab.c"
    break;

  case 184: /* atom: VAR_SCALAR  */
#line 876 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3205 "raku.tab.c"
    break;

  case 185: /* atom: VAR_ARRAY  */
#line 877 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3211 "raku.tab.c"
    break;

  case 186: /* atom: VAR_HASH  */
#line 878 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3217 "raku.tab.c"
    break;

  case 187: /* atom: VAR_CAPTURE  */
#line 880 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3225 "raku.tab.c"
    break;

  case 188: /* atom: VAR_FH  */
#line 884 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3233 "raku.tab.c"
    break;

  case 189: /* atom: VAR_NAMED_CAPTURE  */
#line 888 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3240 "raku.tab.c"
    break;

  case 190: /* atom: VAR_ARRAY '[' expr ']'  */
#line 891 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3246 "raku.tab.c"
    break;

  case 191: /* atom: VAR_HASH '<' IDENT '>'  */
#line 893 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3252 "raku.tab.c"
    break;

  case 192: /* atom: VAR_HASH '{' expr '}'  */
#line 895 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3258 "raku.tab.c"
    break;

  case 193: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 897 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3264 "raku.tab.c"
    break;

  case 194: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 899 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3270 "raku.tab.c"
    break;

  case 195: /* atom: IDENT  */
#line 900 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3276 "raku.tab.c"
    break;

  case 196: /* atom: VAR_TWIGIL  */
#line 902 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3284 "raku.tab.c"
    break;

  case 197: /* atom: VAR_ARRAY_TWIGIL  */
#line 906 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3292 "raku.tab.c"
    break;

  case 198: /* atom: VAR_HASH_TWIGIL  */
#line 910 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3300 "raku.tab.c"
    break;

  case 199: /* atom: '(' expr ')'  */
#line 913 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3306 "raku.tab.c"
    break;


#line 3310 "raku.tab.c"

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

#line 915 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
