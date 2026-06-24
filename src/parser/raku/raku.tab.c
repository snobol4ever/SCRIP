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
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
    expr_add_child(st, ast_attr_int(":lang", LANG_RAKU));
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

#line 209 "raku.tab.c"

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
  YYSYMBOL_KW_GRAMMAR = 51,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 52,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 53,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 54,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 55,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 56,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 57,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 58,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 59,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 60,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 61,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 62,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 63,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 64,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 65,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 66,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 67,                   /* OP_BIND  */
  YYSYMBOL_OP_SMATCH = 68,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 69,                    /* OP_DIV  */
  YYSYMBOL_70_ = 70,                       /* '='  */
  YYSYMBOL_71_ = 71,                       /* '!'  */
  YYSYMBOL_72_ = 72,                       /* '<'  */
  YYSYMBOL_73_ = 73,                       /* '>'  */
  YYSYMBOL_74_ = 74,                       /* '|'  */
  YYSYMBOL_75_ = 75,                       /* '&'  */
  YYSYMBOL_76_ = 76,                       /* '~'  */
  YYSYMBOL_77_ = 77,                       /* '+'  */
  YYSYMBOL_78_ = 78,                       /* '-'  */
  YYSYMBOL_79_ = 79,                       /* '*'  */
  YYSYMBOL_80_ = 80,                       /* '/'  */
  YYSYMBOL_81_ = 81,                       /* '%'  */
  YYSYMBOL_UMINUS = 82,                    /* UMINUS  */
  YYSYMBOL_83_ = 83,                       /* '.'  */
  YYSYMBOL_84_ = 84,                       /* ';'  */
  YYSYMBOL_85_ = 85,                       /* ','  */
  YYSYMBOL_86_ = 86,                       /* '('  */
  YYSYMBOL_87_ = 87,                       /* ')'  */
  YYSYMBOL_88_ = 88,                       /* '['  */
  YYSYMBOL_89_ = 89,                       /* ']'  */
  YYSYMBOL_90_ = 90,                       /* '{'  */
  YYSYMBOL_91_ = 91,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 92,                  /* $accept  */
  YYSYMBOL_program = 93,                   /* program  */
  YYSYMBOL_stmt_list = 94,                 /* stmt_list  */
  YYSYMBOL_stmt = 95,                      /* stmt  */
  YYSYMBOL_if_stmt = 96,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 97,                /* while_stmt  */
  YYSYMBOL_unless_stmt = 98,               /* unless_stmt  */
  YYSYMBOL_until_stmt = 99,                /* until_stmt  */
  YYSYMBOL_repeat_stmt = 100,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 101,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 102,               /* given_stmt  */
  YYSYMBOL_when_list = 103,                /* when_list  */
  YYSYMBOL_sub_decl = 104,                 /* sub_decl  */
  YYSYMBOL_class_decl = 105,               /* class_decl  */
  YYSYMBOL_class_body_list = 106,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 107,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 108,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 109,           /* named_arg_list  */
  YYSYMBOL_param_list = 110,               /* param_list  */
  YYSYMBOL_block = 111,                    /* block  */
  YYSYMBOL_closure = 112,                  /* closure  */
  YYSYMBOL_expr = 113,                     /* expr  */
  YYSYMBOL_cmp_expr = 114,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 115,                 /* jct_expr  */
  YYSYMBOL_range_expr = 116,               /* range_expr  */
  YYSYMBOL_add_expr = 117,                 /* add_expr  */
  YYSYMBOL_mul_expr = 118,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 119,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 120,             /* postfix_expr  */
  YYSYMBOL_call_expr = 121,                /* call_expr  */
  YYSYMBOL_arg_list = 122,                 /* arg_list  */
  YYSYMBOL_atom = 123                      /* atom  */
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
#define YYLAST   1057

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  176
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  444

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   325


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
       2,     2,     2,    71,     2,     2,     2,    81,    75,     2,
      86,    87,    79,    77,    85,    78,    83,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    84,
      72,    70,    73,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    88,     2,    89,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,    74,    91,    76,     2,     2,     2,
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
      65,    66,    67,    68,    69,    82
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   195,   195,   206,   207,   210,   212,   214,   218,   222,
     224,   226,   228,   232,   236,   238,   240,   242,   244,   246,
     248,   250,   252,   254,   256,   258,   260,   265,   269,   276,
     281,   284,   287,   290,   293,   296,   297,   298,   299,   300,
     301,   303,   305,   306,   307,   308,   309,   310,   313,   315,
     317,   321,   325,   327,   331,   335,   339,   345,   351,   356,
     361,   369,   380,   381,   387,   395,   403,   416,   435,   436,
     439,   442,   445,   448,   451,   454,   457,   460,   463,   466,
     469,   472,   479,   486,   493,   500,   510,   518,   527,   536,
     551,   552,   557,   562,   569,   573,   579,   580,   583,   586,
     589,   590,   595,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   614,   620,   626,   629,   630,   631,
     634,   635,   636,   639,   640,   641,   642,   645,   646,   647,
     648,   649,   652,   653,   654,   656,   658,   663,   664,   670,
     676,   680,   687,   692,   698,   704,   711,   718,   723,   728,
     730,   732,   734,   736,   738,   741,   742,   745,   746,   747,
     748,   749,   750,   751,   752,   756,   760,   763,   765,   767,
     769,   771,   773,   774,   778,   782,   786
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
  "KW_HAS", "KW_NEW", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND", "OP_OR", "OP_BIND",
  "OP_SMATCH", "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('",
  "')'", "'['", "']'", "'{'", "'}'", "$accept", "program", "stmt_list",
  "stmt", "if_stmt", "while_stmt", "unless_stmt", "until_stmt",
  "repeat_stmt", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "class_decl", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "param_list", "block", "closure", "expr", "cmp_expr",
  "jct_expr", "range_expr", "add_expr", "mul_expr", "unary_expr",
  "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-218)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -218,    23,   692,  -218,  -218,  -218,  -218,  -218,    28,   -56,
     -29,    21,     9,  -218,  -218,  -218,  -218,  -218,   199,   787,
     833,   -37,    29,   879,    55,    17,   879,   741,   879,   110,
     127,    65,    80,    17,    72,    72,   176,    17,   879,   170,
     206,   122,   122,   879,  -218,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,   183,   173,   295,  -218,    53,
     -22,  -218,  -218,  -218,    88,   879,   258,   879,   278,   879,
     879,    14,   275,   134,   155,   245,   187,   249,   248,   -28,
    -218,   879,   255,   879,   261,   879,   879,   -48,    67,   262,
    -218,  -218,   265,  -218,   266,   270,   -17,    87,   879,   879,
    -218,   879,   879,   879,   879,  -218,   307,  -218,   -12,   274,
    -218,  -218,  -218,   279,  -218,   122,   122,   122,   122,   122,
     122,   122,   122,   228,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   161,   281,   -35,
     282,   299,   283,   289,   291,   361,   292,  -218,  -218,   -31,
     879,   925,   879,    24,    76,    77,   879,   879,   362,   879,
     120,  -218,   129,  -218,   293,   294,   369,  -218,   122,   122,
      -7,   419,  -218,  -218,  -218,   368,   879,   370,   879,   296,
     298,   297,  -218,  -218,  -218,    17,   373,  -218,  -218,  -218,
     168,   168,   168,   168,   168,   168,   168,   168,  -218,  -218,
    -218,   168,   168,  -218,  -218,   179,   179,   -22,   -22,   -22,
    -218,  -218,  -218,  -218,   304,   377,  -218,   879,   473,   322,
     323,   324,  -218,   520,  -218,   567,   879,  -218,   311,   879,
     164,   312,   879,  -218,   971,  -218,   879,  -218,  -218,   308,
     325,   310,   879,   879,    17,    17,    17,   125,   175,  -218,
      17,   136,  -218,   -21,   326,   313,   329,   314,    17,    17,
    -218,  -218,   317,   -19,   -14,   614,  -218,   319,   327,   141,
     879,   879,   879,  -218,   144,   -33,  -218,   186,   189,  -218,
    -218,   190,  -218,   879,  -218,   328,   879,   198,   330,  -218,
    -218,  -218,   331,   332,   384,  -218,  -218,   400,   405,  -218,
     407,    17,   879,    17,  -218,  -218,  -218,   336,   337,   402,
    -218,  -218,     2,   104,  -218,   422,   430,   431,  -218,  -218,
     211,   212,  -218,  -218,   380,   383,   385,   387,  -218,   879,
     439,  -218,  -218,   879,   229,  -218,   217,  -218,   879,  -218,
     388,   389,     3,    17,    17,  -218,  -218,    17,   363,  -218,
    -218,    17,    -3,   382,   394,   -10,     1,   157,   390,   391,
     474,   475,   487,  -218,  -218,  -218,  -218,  -218,  -218,  -218,
     440,   221,  -218,   879,   239,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,  -218,    -5,    -1,   482,   879,  -218,
     484,   879,  -218,     5,     6,   415,   416,  -218,  -218,  -218,
    -218,  -218,   879,   417,   222,  -218,    17,   225,    17,   235,
     418,   420,   423,   424,   492,   879,  -218,   497,   879,  -218,
    -218,  -218,  -218,  -218,   428,  -218,    17,  -218,    17,  -218,
    -218,  -218,  -218,   429,   433,   434,   436,  -218,  -218,  -218,
    -218,  -218,  -218,  -218
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   157,   158,   159,   160,   161,   162,
     163,   173,   172,   174,   175,   164,   165,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,    36,    37,    42,    43,    44,
      38,    39,    45,    46,    47,     0,   102,   116,   119,   122,
     126,   131,   134,   135,   154,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,   163,
     173,     0,     0,     0,     0,     0,     0,     0,   122,     0,
       3,   101,     0,    24,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,   152,    40,   149,     0,     0,
     161,   133,   132,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,   155,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,    20,     0,     0,     0,    59,     0,     0,
       0,     0,    22,    23,    62,     0,     0,     0,     0,     0,
       0,     0,   150,   151,   153,     0,     0,    68,    90,   176,
     103,   104,   105,   106,   109,   110,   111,   112,   113,   114,
     115,   107,   108,   117,   118,   120,   121,   125,   123,   124,
     130,   127,   128,   129,   148,     0,    25,     0,     0,   167,
     168,   169,    27,     0,   143,     0,     0,   136,     0,     0,
       0,     0,     0,    15,     0,    16,     0,    17,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   120,   121,    96,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
      99,    41,     0,     0,     0,     0,   144,     0,     0,     0,
       0,     0,     0,   142,     0,   172,   140,     0,     0,   156,
       5,     0,     6,     0,     9,     0,     0,     0,     0,   167,
     168,   169,     0,     0,    48,    51,    58,     0,     0,    65,
       0,     0,     0,     0,    60,   170,   171,     0,     0,    52,
      54,    68,     0,     0,    66,     0,     0,     0,    89,   147,
       0,     0,    26,    29,     0,     0,     0,     0,   141,     0,
       0,   138,   139,     0,     0,    10,     0,    11,     0,    14,
       0,     0,     0,     0,     0,    97,    64,     0,     0,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   145,    28,    30,    31,    32,    94,
       0,     0,     7,     0,     0,    19,    21,    50,    49,    56,
      57,    63,    61,    53,    67,     0,     0,     0,     0,    74,
       0,     0,    69,     0,     0,     0,     0,    70,    71,    91,
      92,    93,     0,     0,     0,    12,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    76,     0,     0,    75,
      72,    73,    95,     8,     0,    86,     0,    88,     0,    82,
      78,    81,    77,     0,     0,     0,     0,    13,    85,    87,
      84,    80,    83,    79
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -218,  -218,   432,  -218,   185,  -218,  -218,  -218,  -218,  -218,
    -218,  -218,  -218,  -218,   210,  -218,  -218,   263,  -125,     0,
     290,    -2,  -218,   213,   214,   -16,   133,   -30,  -218,  -218,
    -217,  -218
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    44,    45,    46,    47,    48,    49,    50,
      51,   253,    52,    53,   263,    54,   264,   277,   251,    91,
     102,   148,    56,    57,    58,    59,    60,    61,    62,    63,
     149,    64
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,   269,   186,   249,   387,   249,   274,    88,   278,   249,
     166,   111,   112,   302,   303,   390,   353,    82,    84,   414,
     417,    87,   329,     3,    92,    94,    95,    21,   144,   312,
     313,   145,    67,   100,   105,   217,   107,   106,   315,   316,
     317,   113,    90,    68,   158,   312,   313,   133,   321,    85,
      71,   218,   354,    72,   226,   175,   227,   134,   135,   136,
     388,    69,   159,   138,   146,   140,   334,   142,   143,    89,
     304,   391,   314,   176,   389,   415,   418,   318,   187,   160,
     250,   162,   406,   164,   165,   392,   408,   167,   384,   416,
     419,    70,    71,    90,   232,    72,   179,   180,    65,   181,
     182,   183,   184,   210,   211,   212,   213,    90,   233,   128,
     129,    66,   205,   206,   355,    86,   371,   356,   357,   358,
     359,   374,    96,   168,   169,     4,     5,     6,     7,   130,
     131,   132,   110,    78,    79,    80,    12,    13,    14,    97,
      15,    16,    17,   130,   131,   132,   234,   236,   228,   230,
     231,    98,   247,   248,   238,   239,   404,   241,    29,   177,
     235,   237,   101,    34,    35,    36,    99,   393,    38,    55,
     394,   137,   395,   396,   255,   214,   257,   178,   215,     4,
       5,     6,     7,   297,   108,   261,    77,    78,    79,    80,
      12,    13,    14,    41,    15,    16,    17,   153,   154,   155,
      42,   130,   131,   132,   150,   242,    25,   189,    43,    73,
      74,    75,    29,    76,   243,   267,   189,    34,    35,    36,
     109,   300,    38,   301,   279,   151,   226,   281,   324,   226,
     285,   328,   287,   298,   288,   198,   199,   200,   115,   116,
     292,   293,   126,   127,   294,   295,   296,    41,   282,   283,
     299,   130,   131,   132,    42,   130,   131,   132,   309,   310,
     407,   409,    43,   207,   208,   209,   101,   114,   325,   326,
     327,   330,   139,   331,   226,   333,   332,   189,     4,     5,
       6,     7,   337,   338,   336,    77,    78,    79,    80,    12,
      13,    14,   141,    15,    16,    17,   330,   226,   363,   364,
     347,   346,   373,   348,   189,    25,   226,   226,   403,   424,
     300,    29,   426,   372,   226,   152,    34,    35,    36,   156,
     300,    38,   428,   405,   226,   103,   104,   369,   190,   191,
     192,   193,   194,   195,   196,   197,   157,   201,   202,   161,
     203,   204,   378,   379,   380,   163,    41,   381,   170,   172,
     173,   383,   185,    42,   117,   118,   119,   120,   121,   122,
     174,    43,   147,   123,   188,   216,   189,   124,   125,   126,
     127,   219,   220,   222,   221,   224,   240,   223,   225,   246,
     244,   245,   254,   258,   256,   259,   411,   262,   260,   413,
     265,   266,   270,   271,   272,   280,   284,   289,   290,   305,
     422,   291,   307,   322,   306,   308,   425,   311,   427,   342,
     343,   323,   335,   434,   339,   344,   436,   345,   340,   341,
     349,   350,     4,     5,     6,     7,   438,   351,   439,     8,
       9,    10,    11,    12,    13,    14,   360,    15,    16,    17,
      18,    19,    20,    21,   361,   362,    22,    23,    24,    25,
      26,    27,    28,   370,   382,    29,    30,    31,    32,    33,
      34,    35,    36,    37,   365,    38,    39,   366,   385,   367,
      40,   368,   375,   376,   397,   398,     4,     5,     6,     7,
     386,   399,   400,    77,    78,    79,    80,    12,    13,    14,
      41,    15,    16,    17,   401,   402,   410,    42,   412,   420,
     421,   423,   429,    25,   430,    43,   433,   431,   432,    29,
     252,   435,   437,   440,    34,    35,    36,   441,   442,    38,
     443,   352,   171,     4,     5,     6,     7,   377,   320,     0,
      77,    78,    79,    80,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,    41,     0,     0,     0,     0,     0,
      25,    42,     0,     0,     0,     0,    29,     0,     0,    43,
     268,    34,    35,    36,     0,     0,    38,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    77,    78,    79,
      80,   275,    13,    14,     0,    15,    16,    17,     0,     0,
       0,    41,     0,     0,     0,     0,     0,    25,    42,     0,
       0,     0,     0,    29,     0,     0,    43,   273,    34,    35,
      36,     0,     0,    38,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    77,    78,    79,    80,   275,    13,
      14,     0,    15,    16,    17,     0,     0,     0,    41,     0,
       0,     0,     0,     0,    25,    42,     0,     0,     0,     0,
      29,     0,     0,    43,   276,    34,    35,    36,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     4,     5,     6,     7,     0,
      43,   319,     8,     9,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,    20,    21,     0,     0,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,     0,     0,    40,     4,     5,     6,     7,     0,     0,
       0,    77,    78,    79,    80,    12,    13,    14,     0,    15,
      16,    17,     0,    41,     0,     0,     0,     0,     0,     0,
      42,    25,     0,     0,     0,     0,     0,    29,    43,     0,
       0,     0,    34,    35,    36,     0,     0,    38,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    77,    78,    79,
      80,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,    41,     0,     0,     0,     0,    25,     0,    42,
       0,     0,     0,    29,     0,    93,     0,    43,    34,    35,
      36,     0,     0,    38,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    77,    78,    79,    80,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,    41,     0,
       0,     0,     0,    25,     0,    42,     0,     0,     0,    29,
       0,     0,     0,    81,    34,    35,    36,     0,     0,    38,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    77,
      78,    79,    80,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,    41,     0,     0,     0,     0,    25,
       0,    42,     0,     0,     0,    29,     0,     0,     0,    83,
      34,    35,    36,     0,     0,    38,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    77,    78,    79,    80,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
      41,     0,     0,     0,     0,    25,     0,    42,     0,     0,
       0,    29,     0,     0,     0,    43,    34,    35,    36,     0,
       0,    38,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    77,    78,    79,    80,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,    41,     0,     0,     0,
       0,    25,     0,    42,     0,     0,     0,    29,     0,     0,
       0,   229,    34,    35,    36,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,     0,     0,     0,   286
};

static const yytype_int16 yycheck[] =
{
       2,   218,    14,    10,    14,    10,   223,    23,   225,    10,
      58,    41,    42,    34,    35,    14,    14,    19,    20,    14,
      14,    23,    55,     0,    26,    27,    28,    24,    14,    48,
      49,    17,    88,    33,    36,    70,    38,    37,    52,    53,
      54,    43,    90,    72,    72,    48,    49,    69,   265,    86,
      83,    86,    50,    86,    85,    72,    87,    79,    80,    81,
      70,    90,    90,    65,    50,    67,   283,    69,    70,    14,
      91,    70,    91,    90,    84,    70,    70,    91,    90,    81,
      87,    83,    87,    85,    86,    84,    87,    87,    91,    84,
      84,    70,    83,    90,    70,    86,    98,    99,    70,   101,
     102,   103,   104,   133,   134,   135,   136,    90,    84,    56,
      57,    83,   128,   129,    10,    86,   333,    13,    14,    15,
      16,   338,    12,    56,    57,     3,     4,     5,     6,    76,
      77,    78,    10,    11,    12,    13,    14,    15,    16,    12,
      18,    19,    20,    76,    77,    78,    70,    70,   150,   151,
     152,    86,   168,   169,   156,   157,   373,   159,    36,    72,
      84,    84,    90,    41,    42,    43,    86,    10,    46,   171,
      13,    83,    15,    16,   176,    14,   178,    90,    17,     3,
       4,     5,     6,    58,    14,   185,    10,    11,    12,    13,
      14,    15,    16,    71,    18,    19,    20,    10,    11,    12,
      78,    76,    77,    78,    70,    85,    30,    87,    86,    10,
      11,    12,    36,    14,    85,   217,    87,    41,    42,    43,
      14,    85,    46,    87,   226,    70,    85,   229,    87,    85,
     232,    87,   234,    58,   236,     7,     8,     9,    65,    66,
     242,   243,    74,    75,   244,   245,   246,    71,    84,    85,
     250,    76,    77,    78,    78,    76,    77,    78,   258,   259,
     385,   386,    86,   130,   131,   132,    90,    84,   270,   271,
     272,    85,    14,    87,    85,    85,    87,    87,     3,     4,
       5,     6,    84,    85,   286,    10,    11,    12,    13,    14,
      15,    16,    14,    18,    19,    20,    85,    85,    87,    87,
     302,   301,    85,   303,    87,    30,    85,    85,    87,    87,
      85,    36,    87,    84,    85,    70,    41,    42,    43,    70,
      85,    46,    87,    84,    85,    35,    36,   329,   115,   116,
     117,   118,   119,   120,   121,   122,    88,   124,   125,    84,
     126,   127,   342,   343,   344,    84,    71,   347,    86,    84,
      84,   351,    45,    78,    59,    60,    61,    62,    63,    64,
      90,    86,    87,    68,    90,    84,    87,    72,    73,    74,
      75,    89,    73,    84,    91,    14,    14,    86,    86,    10,
      87,    87,    14,    87,    14,    87,   388,    14,    91,   391,
      86,    14,    70,    70,    70,    84,    84,    89,    73,    73,
     402,    91,    73,    84,    91,    91,   406,    90,   408,    25,
      10,    84,    84,   415,    84,    10,   418,    10,    87,    87,
      84,    84,     3,     4,     5,     6,   426,    25,   428,    10,
      11,    12,    13,    14,    15,    16,    14,    18,    19,    20,
      21,    22,    23,    24,    14,    14,    27,    28,    29,    30,
      31,    32,    33,    14,    91,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    84,    46,    47,    84,    86,    84,
      51,    84,    84,    84,    84,    84,     3,     4,     5,     6,
      86,     7,     7,    10,    11,    12,    13,    14,    15,    16,
      71,    18,    19,    20,     7,    55,    14,    78,    14,    84,
      84,    84,    84,    30,    84,    86,    14,    84,    84,    36,
      91,    14,    84,    84,    41,    42,    43,    84,    84,    46,
      84,   311,    90,     3,     4,     5,     6,   342,   265,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      30,    78,    -1,    -1,    -1,    -1,    36,    -1,    -1,    86,
      87,    41,    42,    43,    -1,    -1,    46,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    30,    78,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    86,    87,    41,    42,
      43,    -1,    -1,    46,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    30,    78,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    86,    87,    41,    42,    43,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,     3,     4,     5,     6,    -1,
      86,    87,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    -1,    51,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    30,    -1,    -1,    -1,    -1,    -1,    36,    86,    -1,
      -1,    -1,    41,    42,    43,    -1,    -1,    46,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    30,    -1,    78,
      -1,    -1,    -1,    36,    -1,    84,    -1,    86,    41,    42,
      43,    -1,    -1,    46,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    30,    -1,    78,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    86,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    30,
      -1,    78,    -1,    -1,    -1,    36,    -1,    -1,    -1,    86,
      41,    42,    43,    -1,    -1,    46,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    30,    -1,    78,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    86,    41,    42,    43,    -1,
      -1,    46,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    30,    -1,    78,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    86,    41,    42,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    93,    94,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    27,    28,    29,    30,    31,    32,    33,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      51,    71,    78,    86,    95,    96,    97,    98,    99,   100,
     101,   102,   104,   105,   107,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   123,    70,    83,    88,    72,    90,
      70,    83,    86,    10,    11,    12,    14,    10,    11,    12,
      13,    86,   113,    86,   113,    86,    86,   113,   117,    14,
      90,   111,   113,    84,   113,   113,    12,    12,    86,    86,
     111,    90,   112,   112,   112,   113,   111,   113,    14,    14,
      10,   119,   119,   113,    84,    65,    66,    59,    60,    61,
      62,    63,    64,    68,    72,    73,    74,    75,    56,    57,
      76,    77,    78,    69,    79,    80,    81,    83,   113,    14,
     113,    14,   113,   113,    14,    17,    50,    87,   113,   122,
      70,    70,    70,    10,    11,    12,    70,    88,    72,    90,
     113,    84,   113,    84,   113,   113,    58,   111,    56,    57,
      86,    94,    84,    84,    90,    72,    90,    72,    90,   113,
     113,   113,   113,   113,   113,    45,    14,    90,    90,    87,
     115,   115,   115,   115,   115,   115,   115,   115,     7,     8,
       9,   115,   115,   116,   116,   117,   117,   118,   118,   118,
     119,   119,   119,   119,    14,    17,    84,    70,    86,    89,
      73,    91,    84,    86,    14,    86,    85,    87,   113,    86,
     113,   113,    70,    84,    70,    84,    70,    84,   113,   113,
      14,   113,    85,    85,    87,    87,    10,   117,   117,    10,
      87,   110,    91,   103,    14,   113,    14,   113,    87,    87,
      91,   111,    14,   106,   108,    86,    14,   113,    87,   122,
      70,    70,    70,    87,   122,    14,    87,   109,   122,   113,
      84,   113,    84,    85,    84,   113,    86,   113,   113,    89,
      73,    91,   113,   113,   111,   111,   111,    58,    58,   111,
      85,    87,    34,    35,    91,    73,    91,    73,    91,   111,
     111,    90,    48,    49,    91,    52,    53,    54,    91,    87,
     109,   122,    84,    84,    87,   113,   113,   113,    87,    55,
      85,    87,    87,    85,   122,    84,   113,    84,    85,    84,
      87,    87,    25,    10,    10,    10,   111,   113,   111,    84,
      84,    25,   106,    14,    50,    10,    13,    14,    15,    16,
      14,    14,    14,    87,    87,    84,    84,    84,    84,   113,
      14,   122,    84,    85,   122,    84,    84,    96,   111,   111,
     111,   111,    91,   111,    91,    86,    86,    14,    70,    84,
      14,    70,    84,    10,    13,    15,    16,    84,    84,     7,
       7,     7,    55,    87,   122,    84,    87,   110,    87,   110,
      14,   113,    14,   113,    14,    70,    84,    14,    70,    84,
      84,    84,   113,    84,    87,   111,    87,   111,    87,    84,
      84,    84,    84,    14,   113,    14,   113,    84,   111,   111,
      84,    84,    84,    84
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    92,    93,    94,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    96,    96,
      96,    97,    98,    98,    99,   100,   101,   101,   101,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   107,
     108,   108,   108,   108,   109,   109,   110,   110,   111,   112,
     113,   113,   113,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   115,   115,   115,
     116,   116,   116,   117,   117,   117,   117,   118,   118,   118,
     118,   118,   119,   119,   119,   120,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   122,   122,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     5,     7,     9,     5,
       6,     6,     8,    10,     6,     4,     4,     4,     3,     7,
       3,     7,     3,     3,     2,     4,     6,     4,     7,     6,
       7,     7,     7,     6,     6,     2,     1,     1,     1,     1,
       2,     4,     1,     1,     1,     1,     1,     1,     5,     7,
       7,     5,     5,     7,     5,     2,     7,     7,     5,     3,
       5,     7,     0,     4,     6,     5,     5,     7,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     7,     7,     7,     6,     7,     6,     5,
       0,     4,     4,     4,     3,     5,     1,     3,     3,     3,
       3,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     2,     2,     1,     1,     4,     3,     6,     6,
       5,     6,     5,     4,     4,     6,     6,     5,     3,     2,
       3,     3,     2,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       5,     5,     1,     1,     1,     1,     3
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
#line 196 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1737 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 206 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1743 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 207 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1749 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 211 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1755 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 213 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1761 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 215 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1769 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 219 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1777 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 223 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1783 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 225 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1789 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 227 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1795 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 229 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1803 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 233 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1811 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 237 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1817 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 239 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1823 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 241 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1829 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 243 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1835 "raku.tab.c"
    break;

  case 18: /* stmt: KW_SAY expr ';'  */
#line 245 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1841 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 247 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1847 "raku.tab.c"
    break;

  case 20: /* stmt: KW_PRINT expr ';'  */
#line 249 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1853 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 251 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1859 "raku.tab.c"
    break;

  case 22: /* stmt: KW_TAKE expr ';'  */
#line 253 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1865 "raku.tab.c"
    break;

  case 23: /* stmt: KW_RETURN expr ';'  */
#line 255 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1871 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN ';'  */
#line 257 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1877 "raku.tab.c"
    break;

  case 25: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 259 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1883 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 261 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1892 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 266 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1900 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 270 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 1911 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 277 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 1920 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 282 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1927 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 285 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1934 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 288 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1941 "raku.tab.c"
    break;

  case 33: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 291 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1948 "raku.tab.c"
    break;

  case 34: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 294 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1955 "raku.tab.c"
    break;

  case 35: /* stmt: expr ';'  */
#line 296 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1961 "raku.tab.c"
    break;

  case 36: /* stmt: if_stmt  */
#line 297 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1967 "raku.tab.c"
    break;

  case 37: /* stmt: while_stmt  */
#line 298 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1973 "raku.tab.c"
    break;

  case 38: /* stmt: for_stmt  */
#line 299 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1979 "raku.tab.c"
    break;

  case 39: /* stmt: given_stmt  */
#line 300 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1985 "raku.tab.c"
    break;

  case 40: /* stmt: KW_TRY block  */
#line 302 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1991 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TRY block KW_CATCH block  */
#line 304 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1997 "raku.tab.c"
    break;

  case 42: /* stmt: unless_stmt  */
#line 305 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2003 "raku.tab.c"
    break;

  case 43: /* stmt: until_stmt  */
#line 306 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2009 "raku.tab.c"
    break;

  case 44: /* stmt: repeat_stmt  */
#line 307 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2015 "raku.tab.c"
    break;

  case 45: /* stmt: sub_decl  */
#line 308 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2021 "raku.tab.c"
    break;

  case 46: /* stmt: class_decl  */
#line 309 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2027 "raku.tab.c"
    break;

  case 47: /* stmt: grammar_decl  */
#line 310 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2033 "raku.tab.c"
    break;

  case 48: /* if_stmt: KW_IF '(' expr ')' block  */
#line 314 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2039 "raku.tab.c"
    break;

  case 49: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 316 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2045 "raku.tab.c"
    break;

  case 50: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 318 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2051 "raku.tab.c"
    break;

  case 51: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 322 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2057 "raku.tab.c"
    break;

  case 52: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 326 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2063 "raku.tab.c"
    break;

  case 53: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 328 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2069 "raku.tab.c"
    break;

  case 54: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 332 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2075 "raku.tab.c"
    break;

  case 55: /* repeat_stmt: KW_REPEAT block  */
#line 336 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2081 "raku.tab.c"
    break;

  case 56: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 340 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2091 "raku.tab.c"
    break;

  case 57: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 346 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2101 "raku.tab.c"
    break;

  case 58: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 352 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2110 "raku.tab.c"
    break;

  case 59: /* for_stmt: KW_FOR expr block  */
#line 357 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2117 "raku.tab.c"
    break;

  case 60: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 362 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2129 "raku.tab.c"
    break;

  case 61: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 370 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2142 "raku.tab.c"
    break;

  case 62: /* when_list: %empty  */
#line 380 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2148 "raku.tab.c"
    break;

  case 63: /* when_list: when_list KW_WHEN expr block  */
#line 382 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2156 "raku.tab.c"
    break;

  case 64: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 388 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2168 "raku.tab.c"
    break;

  case 65: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 396 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2178 "raku.tab.c"
    break;

  case 66: /* class_decl: KW_CLASS IDENT '{' class_body_list '}'  */
#line 404 "raku.y"
        {
            const char *cname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *cd = ast_node_new(TT_CLASS_DECL);
            ast_push(cd, leaf_sval(TT_VAR, cname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(cd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = cd;
        }
#line 2195 "raku.tab.c"
    break;

  case 67: /* class_decl: KW_CLASS IDENT IDENT IDENT '{' class_body_list '}'  */
#line 417 "raku.y"
        {
            const char *cname = intern((yyvsp[-5].sval)); free((yyvsp[-5].sval));
            const char *iskw  = (yyvsp[-4].sval);
            const char *pname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *cd = ast_node_new(TT_CLASS_DECL);
            if (iskw && !strcmp(iskw, "is")) cd->v.sval = (char *)pname;
            free((void *)iskw);
            ast_push(cd, leaf_sval(TT_VAR, cname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(cd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = cd;
        }
#line 2216 "raku.tab.c"
    break;

  case 68: /* class_body_list: %empty  */
#line 435 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2222 "raku.tab.c"
    break;

  case 69: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 437 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2229 "raku.tab.c"
    break;

  case 70: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 440 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2236 "raku.tab.c"
    break;

  case 71: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 443 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2243 "raku.tab.c"
    break;

  case 72: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 446 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2250 "raku.tab.c"
    break;

  case 73: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 449 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2257 "raku.tab.c"
    break;

  case 74: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 452 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2264 "raku.tab.c"
    break;

  case 75: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 455 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2271 "raku.tab.c"
    break;

  case 76: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 458 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2278 "raku.tab.c"
    break;

  case 77: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 461 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2285 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 464 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2292 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 467 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2299 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 470 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2306 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 473 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2317 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 480 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2328 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 487 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2339 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 494 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2350 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 501 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2364 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 511 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2376 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 519 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2389 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 528 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2400 "raku.tab.c"
    break;

  case 89: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 537 "raku.y"
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
#line 2417 "raku.tab.c"
    break;

  case 90: /* grammar_body_list: %empty  */
#line 551 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2423 "raku.tab.c"
    break;

  case 91: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 553 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2432 "raku.tab.c"
    break;

  case 92: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 558 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2441 "raku.tab.c"
    break;

  case 93: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 563 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2450 "raku.tab.c"
    break;

  case 94: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 570 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2458 "raku.tab.c"
    break;

  case 95: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 574 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2466 "raku.tab.c"
    break;

  case 96: /* param_list: VAR_SCALAR  */
#line 579 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2472 "raku.tab.c"
    break;

  case 97: /* param_list: param_list ',' VAR_SCALAR  */
#line 580 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2478 "raku.tab.c"
    break;

  case 98: /* block: '{' stmt_list '}'  */
#line 583 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2484 "raku.tab.c"
    break;

  case 99: /* closure: '{' expr '}'  */
#line 586 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2490 "raku.tab.c"
    break;

  case 100: /* expr: VAR_SCALAR '=' expr  */
#line 589 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2496 "raku.tab.c"
    break;

  case 101: /* expr: KW_GATHER block  */
#line 590 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2506 "raku.tab.c"
    break;

  case 102: /* expr: cmp_expr  */
#line 595 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2512 "raku.tab.c"
    break;

  case 103: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 598 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2518 "raku.tab.c"
    break;

  case 104: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 599 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2524 "raku.tab.c"
    break;

  case 105: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 600 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2530 "raku.tab.c"
    break;

  case 106: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 601 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2536 "raku.tab.c"
    break;

  case 107: /* cmp_expr: jct_expr '<' jct_expr  */
#line 602 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2542 "raku.tab.c"
    break;

  case 108: /* cmp_expr: jct_expr '>' jct_expr  */
#line 603 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2548 "raku.tab.c"
    break;

  case 109: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 604 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2554 "raku.tab.c"
    break;

  case 110: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 605 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2560 "raku.tab.c"
    break;

  case 111: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 606 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2566 "raku.tab.c"
    break;

  case 112: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 607 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2572 "raku.tab.c"
    break;

  case 113: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 609 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2582 "raku.tab.c"
    break;

  case 114: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 615 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2592 "raku.tab.c"
    break;

  case 115: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 621 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2602 "raku.tab.c"
    break;

  case 116: /* cmp_expr: jct_expr  */
#line 626 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2608 "raku.tab.c"
    break;

  case 117: /* jct_expr: jct_expr '|' range_expr  */
#line 629 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2614 "raku.tab.c"
    break;

  case 118: /* jct_expr: jct_expr '&' range_expr  */
#line 630 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2620 "raku.tab.c"
    break;

  case 119: /* jct_expr: range_expr  */
#line 631 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2626 "raku.tab.c"
    break;

  case 120: /* range_expr: add_expr OP_RANGE add_expr  */
#line 634 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2632 "raku.tab.c"
    break;

  case 121: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 635 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2638 "raku.tab.c"
    break;

  case 122: /* range_expr: add_expr  */
#line 636 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2644 "raku.tab.c"
    break;

  case 123: /* add_expr: add_expr '+' mul_expr  */
#line 639 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2650 "raku.tab.c"
    break;

  case 124: /* add_expr: add_expr '-' mul_expr  */
#line 640 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2656 "raku.tab.c"
    break;

  case 125: /* add_expr: add_expr '~' mul_expr  */
#line 641 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2662 "raku.tab.c"
    break;

  case 126: /* add_expr: mul_expr  */
#line 642 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2668 "raku.tab.c"
    break;

  case 127: /* mul_expr: mul_expr '*' unary_expr  */
#line 645 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2674 "raku.tab.c"
    break;

  case 128: /* mul_expr: mul_expr '/' unary_expr  */
#line 646 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2680 "raku.tab.c"
    break;

  case 129: /* mul_expr: mul_expr '%' unary_expr  */
#line 647 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2686 "raku.tab.c"
    break;

  case 130: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 648 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2692 "raku.tab.c"
    break;

  case 131: /* mul_expr: unary_expr  */
#line 649 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2698 "raku.tab.c"
    break;

  case 132: /* unary_expr: '-' unary_expr  */
#line 652 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2704 "raku.tab.c"
    break;

  case 133: /* unary_expr: '!' unary_expr  */
#line 653 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2710 "raku.tab.c"
    break;

  case 134: /* unary_expr: postfix_expr  */
#line 654 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2716 "raku.tab.c"
    break;

  case 135: /* postfix_expr: call_expr  */
#line 656 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2722 "raku.tab.c"
    break;

  case 136: /* call_expr: IDENT '(' arg_list ')'  */
#line 659 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2731 "raku.tab.c"
    break;

  case 137: /* call_expr: IDENT '(' ')'  */
#line 663 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2737 "raku.tab.c"
    break;

  case 138: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 665 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2747 "raku.tab.c"
    break;

  case 139: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 671 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2757 "raku.tab.c"
    break;

  case 140: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 677 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2765 "raku.tab.c"
    break;

  case 141: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 681 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2776 "raku.tab.c"
    break;

  case 142: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 688 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2785 "raku.tab.c"
    break;

  case 143: /* call_expr: IDENT '.' CARET IDENT  */
#line 693 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2795 "raku.tab.c"
    break;

  case 144: /* call_expr: atom '.' CARET IDENT  */
#line 699 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2805 "raku.tab.c"
    break;

  case 145: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 705 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2816 "raku.tab.c"
    break;

  case 146: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 712 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2827 "raku.tab.c"
    break;

  case 147: /* call_expr: atom '.' IDENT '(' ')'  */
#line 719 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2836 "raku.tab.c"
    break;

  case 148: /* call_expr: atom '.' IDENT  */
#line 724 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2845 "raku.tab.c"
    break;

  case 149: /* call_expr: KW_DIE expr  */
#line 729 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 2851 "raku.tab.c"
    break;

  case 150: /* call_expr: KW_MAP closure expr  */
#line 731 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2857 "raku.tab.c"
    break;

  case 151: /* call_expr: KW_GREP closure expr  */
#line 733 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2863 "raku.tab.c"
    break;

  case 152: /* call_expr: KW_SORT expr  */
#line 735 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2869 "raku.tab.c"
    break;

  case 153: /* call_expr: KW_SORT closure expr  */
#line 737 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2875 "raku.tab.c"
    break;

  case 154: /* call_expr: atom  */
#line 738 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2881 "raku.tab.c"
    break;

  case 155: /* arg_list: expr  */
#line 741 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2887 "raku.tab.c"
    break;

  case 156: /* arg_list: arg_list ',' expr  */
#line 742 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2893 "raku.tab.c"
    break;

  case 157: /* atom: LIT_INT  */
#line 745 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2899 "raku.tab.c"
    break;

  case 158: /* atom: LIT_FLOAT  */
#line 746 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2905 "raku.tab.c"
    break;

  case 159: /* atom: LIT_STR  */
#line 747 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 2911 "raku.tab.c"
    break;

  case 160: /* atom: LIT_INTERP_STR  */
#line 748 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2917 "raku.tab.c"
    break;

  case 161: /* atom: VAR_SCALAR  */
#line 749 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2923 "raku.tab.c"
    break;

  case 162: /* atom: VAR_ARRAY  */
#line 750 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2929 "raku.tab.c"
    break;

  case 163: /* atom: VAR_HASH  */
#line 751 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2935 "raku.tab.c"
    break;

  case 164: /* atom: VAR_CAPTURE  */
#line 753 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2943 "raku.tab.c"
    break;

  case 165: /* atom: VAR_FH  */
#line 757 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2951 "raku.tab.c"
    break;

  case 166: /* atom: VAR_NAMED_CAPTURE  */
#line 761 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 2958 "raku.tab.c"
    break;

  case 167: /* atom: VAR_ARRAY '[' expr ']'  */
#line 764 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2964 "raku.tab.c"
    break;

  case 168: /* atom: VAR_HASH '<' IDENT '>'  */
#line 766 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2970 "raku.tab.c"
    break;

  case 169: /* atom: VAR_HASH '{' expr '}'  */
#line 768 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2976 "raku.tab.c"
    break;

  case 170: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 770 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2982 "raku.tab.c"
    break;

  case 171: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 772 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2988 "raku.tab.c"
    break;

  case 172: /* atom: IDENT  */
#line 773 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2994 "raku.tab.c"
    break;

  case 173: /* atom: VAR_TWIGIL  */
#line 775 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3002 "raku.tab.c"
    break;

  case 174: /* atom: VAR_ARRAY_TWIGIL  */
#line 779 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3010 "raku.tab.c"
    break;

  case 175: /* atom: VAR_HASH_TWIGIL  */
#line 783 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3018 "raku.tab.c"
    break;

  case 176: /* atom: '(' expr ')'  */
#line 786 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3024 "raku.tab.c"
    break;


#line 3028 "raku.tab.c"

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

#line 788 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
