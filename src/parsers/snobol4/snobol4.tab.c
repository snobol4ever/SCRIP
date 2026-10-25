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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         SNOBOL4_STYPE
/* Substitute the variable and function names.  */
#define yyparse         snobol4_parse
#define yylex           snobol4_lex
#define yyerror         snobol4_error
#define yydebug         snobol4_debug
#define yynerrs         snobol4_nerrs

/* First part of user prologue.  */
#line 1 "snobol4.y"

#include "ct_arena.h"

#line 81 "snobol4.tab.c"

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

#include "snobol4.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_IDENT = 3,                    /* T_IDENT  */
  YYSYMBOL_T_FUNCTION = 4,                 /* T_FUNCTION  */
  YYSYMBOL_T_KEYWORD = 5,                  /* T_KEYWORD  */
  YYSYMBOL_T_END = 6,                      /* T_END  */
  YYSYMBOL_T_INT = 7,                      /* T_INT  */
  YYSYMBOL_T_REAL = 8,                     /* T_REAL  */
  YYSYMBOL_T_STR = 9,                      /* T_STR  */
  YYSYMBOL_T_LABEL = 10,                   /* T_LABEL  */
  YYSYMBOL_T_GOTO_S = 11,                  /* T_GOTO_S  */
  YYSYMBOL_T_GOTO_F = 12,                  /* T_GOTO_F  */
  YYSYMBOL_T_GOTO_LPAREN = 13,             /* T_GOTO_LPAREN  */
  YYSYMBOL_T_GOTO_RPAREN = 14,             /* T_GOTO_RPAREN  */
  YYSYMBOL_T_GOTO_LANGLE = 15,             /* T_GOTO_LANGLE  */
  YYSYMBOL_T_GOTO_RANGLE = 16,             /* T_GOTO_RANGLE  */
  YYSYMBOL_T_STMT_END = 17,                /* T_STMT_END  */
  YYSYMBOL_T_2EQUAL = 18,                  /* T_2EQUAL  */
  YYSYMBOL_T_2QUEST = 19,                  /* T_2QUEST  */
  YYSYMBOL_T_2PIPE = 20,                   /* T_2PIPE  */
  YYSYMBOL_T_2PLUS = 21,                   /* T_2PLUS  */
  YYSYMBOL_T_2MINUS = 22,                  /* T_2MINUS  */
  YYSYMBOL_T_2STAR = 23,                   /* T_2STAR  */
  YYSYMBOL_T_2SLASH = 24,                  /* T_2SLASH  */
  YYSYMBOL_T_2CARET = 25,                  /* T_2CARET  */
  YYSYMBOL_T_2DOLLAR = 26,                 /* T_2DOLLAR  */
  YYSYMBOL_T_2DOT = 27,                    /* T_2DOT  */
  YYSYMBOL_T_2AMP = 28,                    /* T_2AMP  */
  YYSYMBOL_T_2AT = 29,                     /* T_2AT  */
  YYSYMBOL_T_2POUND = 30,                  /* T_2POUND  */
  YYSYMBOL_T_2PERCENT = 31,                /* T_2PERCENT  */
  YYSYMBOL_T_2TILDE = 32,                  /* T_2TILDE  */
  YYSYMBOL_T_1AT = 33,                     /* T_1AT  */
  YYSYMBOL_T_1TILDE = 34,                  /* T_1TILDE  */
  YYSYMBOL_T_1QUEST = 35,                  /* T_1QUEST  */
  YYSYMBOL_T_1AMP = 36,                    /* T_1AMP  */
  YYSYMBOL_T_1PLUS = 37,                   /* T_1PLUS  */
  YYSYMBOL_T_1MINUS = 38,                  /* T_1MINUS  */
  YYSYMBOL_T_1STAR = 39,                   /* T_1STAR  */
  YYSYMBOL_T_1DOLLAR = 40,                 /* T_1DOLLAR  */
  YYSYMBOL_T_1DOT = 41,                    /* T_1DOT  */
  YYSYMBOL_T_1BANG = 42,                   /* T_1BANG  */
  YYSYMBOL_T_1PERCENT = 43,                /* T_1PERCENT  */
  YYSYMBOL_T_1SLASH = 44,                  /* T_1SLASH  */
  YYSYMBOL_T_1POUND = 45,                  /* T_1POUND  */
  YYSYMBOL_T_1CARET = 46,                  /* T_1CARET  */
  YYSYMBOL_T_1EQUAL = 47,                  /* T_1EQUAL  */
  YYSYMBOL_T_1PIPE = 48,                   /* T_1PIPE  */
  YYSYMBOL_T_CONCAT = 49,                  /* T_CONCAT  */
  YYSYMBOL_T_COMMA = 50,                   /* T_COMMA  */
  YYSYMBOL_T_LPAREN = 51,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 52,                  /* T_RPAREN  */
  YYSYMBOL_T_LBRACK = 53,                  /* T_LBRACK  */
  YYSYMBOL_T_RBRACK = 54,                  /* T_RBRACK  */
  YYSYMBOL_T_LANGLE = 55,                  /* T_LANGLE  */
  YYSYMBOL_T_RANGLE = 56,                  /* T_RANGLE  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_top = 58,                       /* top  */
  YYSYMBOL_program = 59,                   /* program  */
  YYSYMBOL_stmt = 60,                      /* stmt  */
  YYSYMBOL_unlabeled_stmt = 61,            /* unlabeled_stmt  */
  YYSYMBOL_opt_subject = 62,               /* opt_subject  */
  YYSYMBOL_subj_amp = 63,                  /* subj_amp  */
  YYSYMBOL_qsubject = 64,                  /* qsubject  */
  YYSYMBOL_opt_pattern = 65,               /* opt_pattern  */
  YYSYMBOL_opt_repl = 66,                  /* opt_repl  */
  YYSYMBOL_goto_label_expr = 67,           /* goto_label_expr  */
  YYSYMBOL_68_1 = 68,                      /* $@1  */
  YYSYMBOL_expr0 = 69,                     /* expr0  */
  YYSYMBOL_expr1 = 70,                     /* expr1  */
  YYSYMBOL_expr2 = 71,                     /* expr2  */
  YYSYMBOL_expr3 = 72,                     /* expr3  */
  YYSYMBOL_expr4 = 73,                     /* expr4  */
  YYSYMBOL_expr5 = 74,                     /* expr5  */
  YYSYMBOL_expr6 = 75,                     /* expr6  */
  YYSYMBOL_expr7 = 76,                     /* expr7  */
  YYSYMBOL_expr8 = 77,                     /* expr8  */
  YYSYMBOL_expr9 = 78,                     /* expr9  */
  YYSYMBOL_expr10 = 79,                    /* expr10  */
  YYSYMBOL_expr11 = 80,                    /* expr11  */
  YYSYMBOL_expr12 = 81,                    /* expr12  */
  YYSYMBOL_expr13 = 82,                    /* expr13  */
  YYSYMBOL_expr14 = 83,                    /* expr14  */
  YYSYMBOL_expr15 = 84,                    /* expr15  */
  YYSYMBOL_85_2 = 85,                      /* $@2  */
  YYSYMBOL_86_3 = 86,                      /* $@3  */
  YYSYMBOL_idx_args = 87,                  /* idx_args  */
  YYSYMBOL_expr17 = 88,                    /* expr17  */
  YYSYMBOL_89_4 = 89,                      /* $@4  */
  YYSYMBOL_90_5 = 90,                      /* $@5  */
  YYSYMBOL_91_6 = 91,                      /* $@6  */
  YYSYMBOL_vlist_args = 92,                /* vlist_args  */
  YYSYMBOL_fnc_args = 93,                  /* fnc_args  */
  YYSYMBOL_goto_atom = 94,                 /* goto_atom  */
  YYSYMBOL_95_7 = 95,                      /* $@7  */
  YYSYMBOL_goto_fnc_args = 96,             /* goto_fnc_args  */
  YYSYMBOL_goto_expr = 97                  /* goto_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 8 "snobol4.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
static int sno4_goto_f_first;
#include <ctype.h>
typedef struct { CODE_t *prog; tree_t **result; tree_t *ast_prog; } PP;
static void     sno4_stmt_commit_go(void*,Token,tree_t*,tree_t*,int,tree_t*,tree_t*,tree_t*,tree_t*);
int             snobol4_lex(YYSTYPE *yylval_param);
void            snobol4_error(void *p, const char *msg);
static Lex     *g_lx;
static int      g_err_lineno;
static int sno_target_value(const tree_t *t) { return t && (t->t == TT_ILIT || t->t == TT_FLIT || t->t == TT_QLIT || t->t == TT_NAME); }
static void sno_target_refuse(void) { sno_error(snobol4_get_stmt_lineno(), "value used where name is required"); }
void sno_error_voice(int, const char *, int);
static int sno_fold_refused_line = -1;
static int sno_tree_has_call(const tree_t *t) { if (!t) return 0; if (t->t == TT_FNC) return 1; for (int k = 0; k < t->n; k++) if (sno_tree_has_call(t->c[k])) return 1; return 0; }
static void sno_fold_refuse(const tree_t *t) { extern int sno_preeval_expr(const tree_t *, const char **); extern int sno_error_quiet_on(void); const char *m = (const char *)0; int c, ln; if (sno_tree_has_call(t) || sno_error_quiet_on()) return; c = sno_preeval_expr(t, &m); ln = snobol4_get_stmt_lineno(); if (c && ln != sno_fold_refused_line) { sno_error_voice(c, m ? m : "", ln); sno_fold_refused_line = ln; } }
#define TAL_MAX 512
#define TAL_DEPTH 64
static tree_t *g_tal[TAL_MAX];
static int     g_tal_base[TAL_DEPTH];
static int     g_tal_n     = 0;
static int     g_tal_depth = 0;
static inline void   tal_open(void)      { g_tal_base[g_tal_depth++] = g_tal_n; }
static inline void   tal_push(tree_t *c) { g_tal[g_tal_n++] = c; }
static inline int    tal_count(void)     { return g_tal_n - g_tal_base[g_tal_depth-1]; }
static inline tree_t*tal_child(int i)    { return g_tal[g_tal_base[g_tal_depth-1] + i]; }
static inline void   tal_close(void)     { g_tal_n = g_tal_base[--g_tal_depth]; }
static tree_e  g_tal_kind[TAL_DEPTH];
static char   *g_tal_sval[TAL_DEPTH];
static inline void    tal_fnc_open(tree_e k, char *s) { g_tal_kind[g_tal_depth-1]=k; g_tal_sval[g_tal_depth-1]=s; }
static inline tree_t *tal_fnc_close(void) {
    int n=tal_count(); tree_e k=g_tal_kind[g_tal_depth-1]; char *sv=g_tal_sval[g_tal_depth-1];
    tree_t *e=ast_node_new(k==TT_VAR?TT_FNC:k);
    if (k==TT_VAR||k==TT_ARB||k==TT_BAL||k==TT_REM||k==TT_FAIL||k==TT_SUCCEED||k==TT_ABORT) e->v.sval=sv;
    for (int j=0;j<n;j++) expr_add_child(e,tal_child(j));
    tal_close(); return e;
}
static tree_t  *parse_expr(Lex*);
static tree_e pat_prim_kind(const char *s) {
    if (!s) return TT_VAR;
    static const struct { const char *n; tree_e k; } m[] = {
        {"ANY",TT_ANY},{"NOTANY",TT_NOTANY},{"SPAN",TT_SPAN},{"BREAK",TT_BREAK},{"BREAKX",TT_BREAKX},
        {"LEN",TT_LEN},{"POS",TT_POS},{"RPOS",TT_RPOS},{"TAB",TT_TAB},{"RTAB",TT_RTAB},
        {"ARB",TT_ARB},{"ARBNO",TT_ARBNO},{"REM",TT_REM},{"FAIL",TT_FAIL},{"SUCCEED",TT_SUCCEED},
        {"FENCE",TT_FENCE},{"FLUSH",TT_FLUSH},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR}
    };
    for (int i = 0; m[i].n; i++) if (strcmp(s, m[i].n) == 0) return m[i].k;
    return TT_VAR;
}

#line 267 "snobol4.tab.c"

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
typedef yytype_uint8 yy_state_t;

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

/* The parser invokes alloca or the compile-time arena; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include \"ct_arena.h\"
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
       && ! ((defined YYMALLOC) \
             && (defined YYFREE)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ct_alloc
#   if 0
void *ct_alloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ct_drop
#   if 0
void ct_drop(void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined SNOBOL4_STYPE_IS_TRIVIAL && SNOBOL4_STYPE_IS_TRIVIAL)))

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
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   282

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  135
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  224

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


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
      55,    56
};

#if SNOBOL4_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    82,    82,    83,    85,    85,    87,    88,    89,    90,
      91,    92,    93,    96,    97,    98,    99,   100,   101,   103,
     104,   105,   106,   107,   109,   110,   112,   113,   114,   115,
     117,   118,   120,   121,   122,   125,   126,   127,   128,   129,
     130,   131,   131,   132,   134,   135,   136,   138,   139,   141,
     142,   144,   145,   147,   148,   150,   151,   153,   154,   155,
     157,   158,   160,   161,   163,   164,   166,   167,   169,   170,
     172,   173,   174,   176,   177,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   197,   197,   198,   198,   199,   201,   202,   203,
     204,   206,   207,   207,   208,   209,   209,   210,   210,   211,
     212,   213,   214,   215,   216,   218,   219,   220,   221,   223,
     224,   225,   226,   228,   229,   230,   231,   232,   233,   233,
     235,   236,   237,   238,   240,   241
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if SNOBOL4_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_IDENT",
  "T_FUNCTION", "T_KEYWORD", "T_END", "T_INT", "T_REAL", "T_STR",
  "T_LABEL", "T_GOTO_S", "T_GOTO_F", "T_GOTO_LPAREN", "T_GOTO_RPAREN",
  "T_GOTO_LANGLE", "T_GOTO_RANGLE", "T_STMT_END", "T_2EQUAL", "T_2QUEST",
  "T_2PIPE", "T_2PLUS", "T_2MINUS", "T_2STAR", "T_2SLASH", "T_2CARET",
  "T_2DOLLAR", "T_2DOT", "T_2AMP", "T_2AT", "T_2POUND", "T_2PERCENT",
  "T_2TILDE", "T_1AT", "T_1TILDE", "T_1QUEST", "T_1AMP", "T_1PLUS",
  "T_1MINUS", "T_1STAR", "T_1DOLLAR", "T_1DOT", "T_1BANG", "T_1PERCENT",
  "T_1SLASH", "T_1POUND", "T_1CARET", "T_1EQUAL", "T_1PIPE", "T_CONCAT",
  "T_COMMA", "T_LPAREN", "T_RPAREN", "T_LBRACK", "T_RBRACK", "T_LANGLE",
  "T_RANGLE", "$accept", "top", "program", "stmt", "unlabeled_stmt",
  "opt_subject", "subj_amp", "qsubject", "opt_pattern", "opt_repl",
  "goto_label_expr", "$@1", "expr0", "expr1", "expr2", "expr3", "expr4",
  "expr5", "expr6", "expr7", "expr8", "expr9", "expr10", "expr11",
  "expr12", "expr13", "expr14", "expr15", "$@2", "$@3", "idx_args",
  "expr17", "$@4", "$@5", "$@6", "vlist_args", "fnc_args", "goto_atom",
  "$@7", "goto_fnc_args", "goto_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-177)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-55)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     124,  -177,   -33,  -177,  -177,  -177,  -177,  -177,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,    18,    45,   143,  -177,  -177,
      14,    75,    48,    76,    64,    77,    58,    79,    68,   121,
     169,   162,  -177,    22,  -177,    -3,   153,  -177,  -177,    14,
    -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,   159,    62,
     193,  -177,   186,  -177,  -177,   192,   101,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,  -177,  -177,   192,
     108,   192,  -177,  -177,   192,   192,  -177,   197,   197,    32,
     192,  -177,   203,  -177,    64,    64,  -177,  -177,    64,    77,
    -177,    64,    68,    68,  -177,   121,   169,   162,  -177,  -177,
    -177,  -177,  -177,   193,   -12,   192,   192,  -177,   163,   197,
     197,  -177,   205,  -177,   164,   192,  -177,   193,   105,    85,
     123,   209,   210,    80,   226,  -177,   192,  -177,   -20,    49,
     192,  -177,   190,    98,  -177,   192,  -177,   167,   197,  -177,
     197,  -177,  -177,  -177,  -177,  -177,   230,   231,   135,  -177,
    -177,   192,  -177,  -177,  -177,   197,  -177,   197,  -177,  -177,
    -177,   229,   249,   135,  -177,  -177,   234,  -177,  -177,  -177,
    -177,  -177,    27,  -177,   250,   252,  -177,  -177,    -6,   199,
    -177,   256,   135,  -177,  -177,   257,   135,   135,  -177,  -177,
    -177,   199,    25,  -177
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      23,   109,     0,   111,   110,   113,   114,   112,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     5,    12,
      34,    22,    20,     0,    50,    52,    21,    56,    59,    61,
      63,    65,    67,    69,    72,    74,    91,    96,   107,    34,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    90,    88,    89,   105,   104,     0,    46,
      48,    54,    74,     1,     4,    33,     0,    31,     0,    31,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    94,   122,
       0,   118,   102,   101,    45,     0,    32,     0,     0,     0,
       0,    13,     0,    27,    30,    25,    29,    26,    49,    51,
      53,    24,    57,    58,    55,    60,    62,    64,    66,    68,
      70,    71,    73,    19,    52,   100,   100,   121,     0,     0,
       0,     6,     0,   117,     0,   118,    44,    47,     0,     0,
       0,     0,     0,     0,     0,    14,    31,    99,     0,     0,
     120,   108,     0,     0,     7,   116,   106,     0,     0,    15,
       0,    16,    41,    35,    37,    36,     0,     0,     0,    43,
      28,    98,    93,    95,   119,     0,     8,     0,     9,   115,
     103,     0,     0,   133,    38,    40,   125,   126,   127,   124,
     123,   134,     0,    97,     0,     0,    17,    18,     0,   132,
     128,     0,     0,    10,    11,     0,   131,   133,    39,   135,
      42,   130,     0,   129
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -177,  -177,  -177,   245,  -177,   265,  -177,  -177,   -74,   225,
     -97,  -177,   -25,  -177,     1,   -64,   -63,     4,  -177,    69,
     187,   188,   189,    82,  -177,   110,   241,  -177,  -177,  -177,
     140,  -177,  -177,  -177,  -177,   133,  -177,    70,  -177,    63,
    -176
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,    31,    32,   113,    76,
     112,   193,   143,    69,    70,    34,    35,    71,    37,    38,
      39,    40,    41,    42,    43,    44,    72,    46,   135,   136,
     158,    47,   145,   101,    99,   144,   138,   201,   217,   208,
     209
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    33,   202,   142,    36,   116,   117,   156,   215,    33,
     148,   149,    36,   114,   115,   114,   114,   118,    48,   119,
     121,     1,     2,     3,     4,     5,     6,     7,    33,    95,
     181,    36,    75,   134,   182,   150,   151,    83,   152,   223,
     221,   211,   162,   163,   216,    73,    96,    92,    93,    94,
     106,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    79,    66,    25,
      67,   191,   153,   192,   137,   216,   212,   -54,   -54,   146,
     104,   105,   180,   176,    82,   154,    84,   120,   204,   177,
     205,   124,   114,   178,    77,    80,   170,   133,    88,   181,
      85,    86,   171,    78,    81,   183,   147,   -54,    87,   187,
     157,   157,   107,   108,   109,   188,   110,   168,   111,   139,
     140,   109,   169,   110,    -3,   141,    83,     1,     2,     3,
       4,     5,     6,     7,     8,   184,   172,   173,   196,   197,
     189,   198,   199,    -2,   200,    89,     1,     2,     3,     4,
       5,     6,     7,     8,   122,   123,   203,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,   128,   129,    25,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    90,    91,    25,     1,     2,     3,     4,     5,
       6,     7,   185,   130,   131,   132,    97,   186,    98,   102,
     109,   103,   110,   160,   165,   161,   166,   165,    95,   190,
     155,    81,   164,   174,   175,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    45,   179,    25,   194,   195,   206,   210,   212,    45,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,   207,   213,    45,   214,
     218,   220,    74,    49,   100,   125,   159,   126,   167,   127,
     222,     0,   219
};

static const yytype_int16 yycheck[] =
{
      25,     0,   178,   100,     0,    79,    80,    19,    14,     8,
     107,   108,     8,    77,    78,    79,    80,    81,    51,    82,
      84,     3,     4,     5,     6,     7,     8,     9,    27,    32,
      50,    27,    18,    96,    54,     3,     4,    49,     6,    14,
     216,    14,   139,   140,    50,     0,    49,    25,    26,    27,
      75,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    19,    50,    51,
      52,   168,    40,   170,    99,    50,    49,    19,    20,   104,
      18,    19,   156,     3,    20,   110,    28,    83,   185,     9,
     187,    87,   156,    13,    19,    19,    11,    96,    30,    50,
      21,    22,    17,    28,    28,    56,   105,    49,    29,    11,
     135,   136,    11,    12,    13,    17,    15,    12,    17,    11,
      12,    13,    17,    15,     0,    17,    49,     3,     4,     5,
       6,     7,     8,     9,    10,   160,    13,    14,     3,     4,
     165,     6,     7,     0,     9,    24,     3,     4,     5,     6,
       7,     8,     9,    10,    85,    86,   181,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    91,    92,    51,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    23,    31,    51,     3,     4,     5,     6,     7,
       8,     9,    12,    93,    94,    95,    53,    17,    55,    50,
      13,    52,    15,    50,    50,    52,    52,    50,    32,    52,
      17,    28,    17,    14,    14,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,     0,    16,    51,    14,    14,    17,    13,    49,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    17,    17,    27,    17,
      14,    14,    27,     8,    49,    88,   136,    89,   145,    90,
     217,    -1,   212
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    51,    58,    59,    60,    61,
      62,    63,    64,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    88,    51,    62,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    50,    52,    69,    70,
      71,    74,    83,     0,    60,    18,    66,    19,    28,    19,
      19,    28,    20,    49,    28,    21,    22,    29,    30,    24,
      23,    31,    25,    26,    27,    32,    49,    53,    55,    91,
      66,    90,    50,    52,    18,    19,    69,    11,    12,    13,
      15,    17,    67,    65,    72,    72,    65,    65,    72,    73,
      74,    72,    76,    76,    74,    77,    78,    79,    80,    80,
      82,    82,    82,    71,    73,    85,    86,    69,    93,    11,
      12,    17,    67,    69,    92,    89,    69,    71,    67,    67,
       3,     4,     6,    40,    69,    17,    19,    69,    87,    87,
      50,    52,    67,    67,    17,    50,    52,    92,    12,    17,
      11,    17,    13,    14,    14,    14,     3,     9,    13,    16,
      65,    50,    54,    56,    69,    12,    17,    11,    17,    69,
      52,    67,    67,    68,    14,    14,     3,     4,     6,     7,
       9,    94,    97,    69,    67,    67,    17,    17,    96,    97,
      13,    14,    49,    17,    17,    14,    50,    95,    14,    94,
      14,    97,    96,    14
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    61,    61,    61,    61,    61,    61,    62,
      62,    62,    62,    62,    63,    63,    64,    64,    64,    64,
      65,    65,    66,    66,    66,    67,    67,    67,    67,    67,
      67,    68,    67,    67,    69,    69,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    75,    75,    75,
      76,    76,    77,    77,    78,    78,    79,    79,    80,    80,
      81,    81,    81,    82,    82,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    85,    84,    86,    84,    84,    87,    87,    87,
      87,    88,    89,    88,    88,    90,    88,    91,    88,    88,
      88,    88,    88,    88,    88,    92,    92,    92,    92,    93,
      93,    93,    93,    94,    94,    94,    94,    94,    95,    94,
      96,    96,    96,    96,    97,    97
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     4,     5,     6,     6,
       8,     8,     1,     3,     4,     5,     5,     7,     7,     3,
       1,     1,     1,     0,     3,     3,     3,     3,     5,     3,
       1,     0,     2,     1,     0,     3,     3,     3,     4,     6,
       4,     0,     7,     3,     3,     2,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     0,     5,     0,     5,     1,     3,     2,     1,
       0,     3,     0,     6,     2,     0,     5,     0,     5,     1,
       1,     1,     1,     1,     1,     3,     2,     1,     0,     3,
       2,     1,     0,     1,     1,     1,     1,     1,     0,     5,
       3,     2,     1,     0,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SNOBOL4_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SNOBOL4_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (yyparse_param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SNOBOL4_error or SNOBOL4_UNDEF. */
#define YYERRCODE SNOBOL4_UNDEF


/* Enable debugging if requested.  */
#if SNOBOL4_DEBUG

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
                  Kind, Value, yyparse_param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *yyparse_param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yyparse_param);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *yyparse_param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, yyparse_param);
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
                 int yyrule, void *yyparse_param)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], yyparse_param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, yyparse_param); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SNOBOL4_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SNOBOL4_DEBUG */


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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *yyparse_param)
{
  YY_USE (yyvaluep);
  YY_USE (yyparse_param);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *yyparse_param)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

  yychar = SNOBOL4_EMPTY; /* Cause a token to be read.  */

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
  if (yychar == SNOBOL4_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval);
    }

  if (yychar <= SNOBOL4_EOF)
    {
      yychar = SNOBOL4_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SNOBOL4_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SNOBOL4_UNDEF;
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
  yychar = SNOBOL4_EMPTY;
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
  case 2: /* top: program  */
#line 82 "snobol4.y"
                                                                                                    { }
#line 1405 "snobol4.tab.c"
    break;

  case 3: /* top: %empty  */
#line 83 "snobol4.y"
                                                                                        { }
#line 1411 "snobol4.tab.c"
    break;

  case 6: /* stmt: T_LABEL opt_subject opt_repl T_STMT_END  */
#line 87 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-3].tok),(yyvsp[-2].expr),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1417 "snobol4.tab.c"
    break;

  case 7: /* stmt: T_LABEL opt_subject opt_repl goto_label_expr T_STMT_END  */
#line 88 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-4].tok),(yyvsp[-3].expr),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1423 "snobol4.tab.c"
    break;

  case 8: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 89 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-5].tok),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1429 "snobol4.tab.c"
    break;

  case 9: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 90 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-5].tok),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1435 "snobol4.tab.c"
    break;

  case 10: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 91 "snobol4.y"
                                                                                                         { sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1441 "snobol4.tab.c"
    break;

  case 11: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 92 "snobol4.y"
                                                                                                         { sno4_goto_f_first=1; sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1447 "snobol4.tab.c"
    break;

  case 13: /* unlabeled_stmt: opt_subject opt_repl T_STMT_END  */
#line 96 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-2].expr),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1453 "snobol4.tab.c"
    break;

  case 14: /* unlabeled_stmt: opt_subject opt_repl goto_label_expr T_STMT_END  */
#line 97 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-3].expr),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1459 "snobol4.tab.c"
    break;

  case 15: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 98 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1465 "snobol4.tab.c"
    break;

  case 16: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 99 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1471 "snobol4.tab.c"
    break;

  case 17: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 100 "snobol4.y"
                                                                                                 { sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1477 "snobol4.tab.c"
    break;

  case 18: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 101 "snobol4.y"
                                                                                                 { sno4_goto_f_first=1; sno4_stmt_commit_go(yyparse_param,((Token){0,0,0,0}),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1483 "snobol4.tab.c"
    break;

  case 19: /* opt_subject: expr14 T_CONCAT expr2  */
#line 103 "snobol4.y"
                                                                                                   { (yyval.expr)=expr_binary(TT_SCAN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1489 "snobol4.tab.c"
    break;

  case 20: /* opt_subject: qsubject  */
#line 104 "snobol4.y"
                                                                                                    { (yyval.expr)=(yyvsp[0].expr); }
#line 1495 "snobol4.tab.c"
    break;

  case 21: /* opt_subject: expr5  */
#line 105 "snobol4.y"
                                                                                                    { (yyval.expr)=(yyvsp[0].expr); }
#line 1501 "snobol4.tab.c"
    break;

  case 22: /* opt_subject: subj_amp  */
#line 106 "snobol4.y"
                                                                                                    { (yyval.expr)=(yyvsp[0].expr); }
#line 1507 "snobol4.tab.c"
    break;

  case 23: /* opt_subject: %empty  */
#line 107 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1513 "snobol4.tab.c"
    break;

  case 24: /* subj_amp: expr5 T_2AMP expr3  */
#line 109 "snobol4.y"
                                                                                                    { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("&"); (yyval.expr)=_e; }
#line 1519 "snobol4.tab.c"
    break;

  case 25: /* subj_amp: subj_amp T_2AMP expr3  */
#line 110 "snobol4.y"
                                                                                                    { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("&"); (yyval.expr)=_e; }
#line 1525 "snobol4.tab.c"
    break;

  case 26: /* qsubject: expr2 T_2QUEST opt_pattern  */
#line 112 "snobol4.y"
                                                                                                    { (yyval.expr)=expr_binary(TT_SCAN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1531 "snobol4.tab.c"
    break;

  case 27: /* qsubject: subj_amp T_2QUEST opt_pattern  */
#line 113 "snobol4.y"
                                                                                                    { (yyval.expr)=expr_binary(TT_SCAN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1537 "snobol4.tab.c"
    break;

  case 28: /* qsubject: expr14 T_CONCAT expr4 T_2QUEST opt_pattern  */
#line 114 "snobol4.y"
                                                                                                    { tree_t*s=ast_node_new(TT_SEQ);expr_add_child(s,(yyvsp[-4].expr));expr_add_child(s,(yyvsp[-2].expr));(yyval.expr)=expr_binary(TT_SCAN,s,(yyvsp[0].expr)); }
#line 1543 "snobol4.tab.c"
    break;

  case 29: /* qsubject: qsubject T_2QUEST opt_pattern  */
#line 115 "snobol4.y"
                                                                                                    { (yyval.expr)=expr_binary(TT_SCAN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1549 "snobol4.tab.c"
    break;

  case 30: /* opt_pattern: expr3  */
#line 117 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1555 "snobol4.tab.c"
    break;

  case 31: /* opt_pattern: %empty  */
#line 118 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1561 "snobol4.tab.c"
    break;

  case 32: /* opt_repl: T_2EQUAL expr0  */
#line 120 "snobol4.y"
                                                                                              { (yyval.expr)=(yyvsp[0].expr); }
#line 1567 "snobol4.tab.c"
    break;

  case 33: /* opt_repl: T_2EQUAL  */
#line 121 "snobol4.y"
                                                                                               { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup("");(yyval.expr)=e; }
#line 1573 "snobol4.tab.c"
    break;

  case 34: /* opt_repl: %empty  */
#line 122 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1579 "snobol4.tab.c"
    break;

  case 35: /* goto_label_expr: T_GOTO_LPAREN T_IDENT T_GOTO_RPAREN  */
#line 125 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1585 "snobol4.tab.c"
    break;

  case 36: /* goto_label_expr: T_GOTO_LPAREN T_END T_GOTO_RPAREN  */
#line 126 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1591 "snobol4.tab.c"
    break;

  case 37: /* goto_label_expr: T_GOTO_LPAREN T_FUNCTION T_GOTO_RPAREN  */
#line 127 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1597 "snobol4.tab.c"
    break;

  case 38: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_IDENT T_GOTO_RPAREN  */
#line 128 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);char buf[512];snprintf(buf,sizeof buf,"$%s",(yyvsp[-1].tok).sval);e->v.sval=ct_strdup(buf);(yyval.expr)=e; }
#line 1603 "snobol4.tab.c"
    break;

  case 39: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_GOTO_LPAREN goto_expr T_GOTO_RPAREN T_GOTO_RPAREN  */
#line 129 "snobol4.y"
                                                                                            { (yyval.expr)=(yyvsp[-2].expr); }
#line 1609 "snobol4.tab.c"
    break;

  case 40: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_STR T_GOTO_RPAREN  */
#line 130 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1615 "snobol4.tab.c"
    break;

  case 41: /* $@1: %empty  */
#line 131 "snobol4.y"
                                                 { tree_e _k=pat_prim_kind((yyvsp[-1].tok).sval); tal_open(); tal_fnc_open(_k,(char*)(yyvsp[-1].tok).sval); }
#line 1621 "snobol4.tab.c"
    break;

  case 42: /* goto_label_expr: T_GOTO_LPAREN T_IDENT T_GOTO_LPAREN $@1 goto_fnc_args T_GOTO_RPAREN T_GOTO_RPAREN  */
#line 131 "snobol4.y"
                                                                                                                                                                              { (yyval.expr)=tal_fnc_close(); }
#line 1627 "snobol4.tab.c"
    break;

  case 43: /* goto_label_expr: T_GOTO_LANGLE expr0 T_GOTO_RANGLE  */
#line 132 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_GOTO_DIRECT);expr_add_child(e,(yyvsp[-1].expr));(yyval.expr)=e; }
#line 1633 "snobol4.tab.c"
    break;

  case 44: /* expr0: expr1 T_2EQUAL expr0  */
#line 134 "snobol4.y"
                                                                                              { if (sno_target_value((yyvsp[-2].expr))) sno_target_refuse(); (yyval.expr)=expr_binary(TT_ASSIGN,          (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1639 "snobol4.tab.c"
    break;

  case 45: /* expr0: expr1 T_2EQUAL  */
#line 135 "snobol4.y"
                                                                                                   { if (sno_target_value((yyvsp[-1].expr))) sno_target_refuse(); tree_t*e=ast_node_new(TT_QLIT);e->v.sval=ct_strdup("");(yyval.expr)=expr_binary(TT_ASSIGN,(yyvsp[-1].expr),e); }
#line 1645 "snobol4.tab.c"
    break;

  case 46: /* expr0: expr1  */
#line 136 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1651 "snobol4.tab.c"
    break;

  case 47: /* expr1: expr1 T_2QUEST expr2  */
#line 138 "snobol4.y"
                                                                                                   { (yyval.expr)=expr_binary(TT_SCAN,            (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1657 "snobol4.tab.c"
    break;

  case 48: /* expr1: expr2  */
#line 139 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1663 "snobol4.tab.c"
    break;

  case 49: /* expr2: expr2 T_2AMP expr3  */
#line 141 "snobol4.y"
                                                                                             { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("&"); (yyval.expr)=_e; }
#line 1669 "snobol4.tab.c"
    break;

  case 50: /* expr2: expr3  */
#line 142 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1675 "snobol4.tab.c"
    break;

  case 51: /* expr3: expr3 T_2PIPE expr4  */
#line 144 "snobol4.y"
                                                                                            { tree_t*a=ast_node_new(TT_ALT);expr_add_child(a,(yyvsp[-2].expr));expr_add_child(a,(yyvsp[0].expr));(yyval.expr)=a; }
#line 1681 "snobol4.tab.c"
    break;

  case 52: /* expr3: expr4  */
#line 145 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1687 "snobol4.tab.c"
    break;

  case 53: /* expr4: expr4 T_CONCAT expr5  */
#line 147 "snobol4.y"
                                                                                                            { tree_t*s=ast_node_new(TT_SEQ);expr_add_child(s,(yyvsp[-2].expr));expr_add_child(s,(yyvsp[0].expr));(yyval.expr)=s; }
#line 1693 "snobol4.tab.c"
    break;

  case 54: /* expr4: expr5  */
#line 148 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1699 "snobol4.tab.c"
    break;

  case 55: /* expr5: expr6 T_2AT expr5  */
#line 150 "snobol4.y"
                                                                                              { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("@"); (yyval.expr)=_e; }
#line 1705 "snobol4.tab.c"
    break;

  case 56: /* expr5: expr6  */
#line 151 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1711 "snobol4.tab.c"
    break;

  case 57: /* expr6: expr6 T_2PLUS expr7  */
#line 153 "snobol4.y"
                                                                                               { (yyval.expr)=expr_binary(TT_ADD,             (yyvsp[-2].expr),(yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1717 "snobol4.tab.c"
    break;

  case 58: /* expr6: expr6 T_2MINUS expr7  */
#line 154 "snobol4.y"
                                                                                             { (yyval.expr)=expr_binary(TT_SUB,             (yyvsp[-2].expr),(yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1723 "snobol4.tab.c"
    break;

  case 59: /* expr6: expr7  */
#line 155 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1729 "snobol4.tab.c"
    break;

  case 60: /* expr7: expr7 T_2POUND expr8  */
#line 157 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("#"); (yyval.expr)=_e; }
#line 1735 "snobol4.tab.c"
    break;

  case 61: /* expr7: expr8  */
#line 158 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1741 "snobol4.tab.c"
    break;

  case 62: /* expr8: expr8 T_2SLASH expr9  */
#line 160 "snobol4.y"
                                                                                                { (yyval.expr)=expr_binary(TT_DIV,             (yyvsp[-2].expr),(yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1747 "snobol4.tab.c"
    break;

  case 63: /* expr8: expr9  */
#line 161 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1753 "snobol4.tab.c"
    break;

  case 64: /* expr9: expr9 T_2STAR expr10  */
#line 163 "snobol4.y"
                                                                                         { (yyval.expr)=expr_binary(TT_MUL,             (yyvsp[-2].expr),(yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1759 "snobol4.tab.c"
    break;

  case 65: /* expr9: expr10  */
#line 164 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1765 "snobol4.tab.c"
    break;

  case 66: /* expr10: expr10 T_2PERCENT expr11  */
#line 166 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("%"); (yyval.expr)=_e; }
#line 1771 "snobol4.tab.c"
    break;

  case 67: /* expr10: expr11  */
#line 167 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1777 "snobol4.tab.c"
    break;

  case 68: /* expr11: expr12 T_2CARET expr11  */
#line 169 "snobol4.y"
                                                                                          { (yyval.expr)=expr_binary(TT_POW,             (yyvsp[-2].expr),(yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1783 "snobol4.tab.c"
    break;

  case 69: /* expr11: expr12  */
#line 170 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1789 "snobol4.tab.c"
    break;

  case 70: /* expr12: expr12 T_2DOLLAR expr13  */
#line 172 "snobol4.y"
                                                                                         { if (sno_target_value((yyvsp[0].expr))) sno_target_refuse(); (yyval.expr)=expr_binary(TT_CAPT_IMMED_ASGN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1795 "snobol4.tab.c"
    break;

  case 71: /* expr12: expr12 T_2DOT expr13  */
#line 173 "snobol4.y"
                                                                                           { if (sno_target_value((yyvsp[0].expr))) sno_target_refuse(); (yyval.expr)=expr_binary(TT_CAPT_COND_ASGN, (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1801 "snobol4.tab.c"
    break;

  case 72: /* expr12: expr13  */
#line 174 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1807 "snobol4.tab.c"
    break;

  case 73: /* expr13: expr14 T_2TILDE expr13  */
#line 176 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=ct_strdup("~"); (yyval.expr)=_e; }
#line 1813 "snobol4.tab.c"
    break;

  case 74: /* expr13: expr14  */
#line 177 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1819 "snobol4.tab.c"
    break;

  case 75: /* expr14: T_1AT expr14  */
#line 179 "snobol4.y"
                                                                                           { if (sno_target_value((yyvsp[0].expr))) sno_target_refuse(); (yyval.expr)=expr_unary(TT_CAPT_CURSOR,     (yyvsp[0].expr)); }
#line 1825 "snobol4.tab.c"
    break;

  case 76: /* expr14: T_1TILDE expr14  */
#line 180 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_NOT,             (yyvsp[0].expr)); }
#line 1831 "snobol4.tab.c"
    break;

  case 77: /* expr14: T_1QUEST expr14  */
#line 181 "snobol4.y"
                                                                                        { (yyval.expr)=expr_unary(TT_INTERROGATE,     (yyvsp[0].expr)); }
#line 1837 "snobol4.tab.c"
    break;

  case 78: /* expr14: T_1AMP expr14  */
#line 182 "snobol4.y"
                                                                                          { if (sno_target_value((yyvsp[0].expr))) sno_target_refuse(); tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("&"); (yyval.expr)=_e; }
#line 1843 "snobol4.tab.c"
    break;

  case 79: /* expr14: T_1PLUS expr14  */
#line 183 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_PLS,             (yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1849 "snobol4.tab.c"
    break;

  case 80: /* expr14: T_1MINUS expr14  */
#line 184 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_MNS,             (yyvsp[0].expr)); sno_fold_refuse((yyval.expr)); }
#line 1855 "snobol4.tab.c"
    break;

  case 81: /* expr14: T_1STAR expr14  */
#line 185 "snobol4.y"
                                                                                            { (yyval.expr)=expr_unary(TT_DEFER,           (yyvsp[0].expr)); }
#line 1861 "snobol4.tab.c"
    break;

  case 82: /* expr14: T_1DOLLAR expr14  */
#line 186 "snobol4.y"
                                                                                           { (yyval.expr)=expr_unary(TT_INDIRECT,        (yyvsp[0].expr)); }
#line 1867 "snobol4.tab.c"
    break;

  case 83: /* expr14: T_1DOT expr14  */
#line 187 "snobol4.y"
                                                                                             { if ((yyvsp[0].expr) && ((yyvsp[0].expr)->t==TT_ILIT || (yyvsp[0].expr)->t==TT_FLIT || (yyvsp[0].expr)->t==TT_QLIT)) sno_error(g_err_lineno,"value used where name is required"); (yyval.expr)=expr_unary(TT_NAME, (yyvsp[0].expr)); }
#line 1873 "snobol4.tab.c"
    break;

  case 84: /* expr14: T_1BANG expr14  */
#line 188 "snobol4.y"
                                                                                         { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("!"); (yyval.expr)=_e; }
#line 1879 "snobol4.tab.c"
    break;

  case 85: /* expr14: T_1PERCENT expr14  */
#line 189 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("%"); (yyval.expr)=_e; }
#line 1885 "snobol4.tab.c"
    break;

  case 86: /* expr14: T_1SLASH expr14  */
#line 190 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("/"); (yyval.expr)=_e; }
#line 1891 "snobol4.tab.c"
    break;

  case 87: /* expr14: T_1POUND expr14  */
#line 191 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("#"); (yyval.expr)=_e; }
#line 1897 "snobol4.tab.c"
    break;

  case 88: /* expr14: T_1EQUAL expr14  */
#line 192 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("="); (yyval.expr)=_e; }
#line 1903 "snobol4.tab.c"
    break;

  case 89: /* expr14: T_1PIPE expr14  */
#line 193 "snobol4.y"
                                                                                        { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("|"); (yyval.expr)=_e; }
#line 1909 "snobol4.tab.c"
    break;

  case 90: /* expr14: T_1CARET expr14  */
#line 194 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=ct_strdup("^"); (yyval.expr)=_e; }
#line 1915 "snobol4.tab.c"
    break;

  case 91: /* expr14: expr15  */
#line 195 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1921 "snobol4.tab.c"
    break;

  case 92: /* $@2: %empty  */
#line 197 "snobol4.y"
                             { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1927 "snobol4.tab.c"
    break;

  case 93: /* expr15: expr15 T_LBRACK $@2 idx_args T_RBRACK  */
#line 197 "snobol4.y"
                                                                              { int _n=tal_count(); tree_t*_i=ast_node_new(TT_IDX); for(int _j=0;_j<_n;_j++) expr_add_child(_i,tal_child(_j)); tal_close(); (yyval.expr)=_i; }
#line 1933 "snobol4.tab.c"
    break;

  case 94: /* $@3: %empty  */
#line 198 "snobol4.y"
                             { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1939 "snobol4.tab.c"
    break;

  case 95: /* expr15: expr15 T_LANGLE $@3 idx_args T_RANGLE  */
#line 198 "snobol4.y"
                                                                              { int _n=tal_count(); tree_t*_i=ast_node_new(TT_IDX); for(int _j=0;_j<_n;_j++) expr_add_child(_i,tal_child(_j)); tal_close(); (yyval.expr)=_i; }
#line 1945 "snobol4.tab.c"
    break;

  case 96: /* expr15: expr17  */
#line 199 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1951 "snobol4.tab.c"
    break;

  case 97: /* idx_args: idx_args T_COMMA expr0  */
#line 201 "snobol4.y"
                                                                                                  { if (tal_count()==1) tal_push(ast_node_new(TT_NUL)); tal_push((yyvsp[0].expr)); }
#line 1957 "snobol4.tab.c"
    break;

  case 98: /* idx_args: idx_args T_COMMA  */
#line 202 "snobol4.y"
                                                                                                  { if (tal_count()==1) tal_push(ast_node_new(TT_NUL)); tal_push(ast_node_new(TT_NUL)); }
#line 1963 "snobol4.tab.c"
    break;

  case 99: /* idx_args: expr0  */
#line 203 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 1969 "snobol4.tab.c"
    break;

  case 101: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 206 "snobol4.y"
                                                                                                { (yyval.expr)=(yyvsp[-1].expr); }
#line 1975 "snobol4.tab.c"
    break;

  case 102: /* $@4: %empty  */
#line 207 "snobol4.y"
                                    { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1981 "snobol4.tab.c"
    break;

  case 103: /* expr17: T_LPAREN expr0 T_COMMA $@4 vlist_args T_RPAREN  */
#line 207 "snobol4.y"
                                                                                      { int _n=tal_count(); tree_t*_a=ast_node_new(TT_VLIST); for(int _j=0;_j<_n;_j++) expr_add_child(_a,tal_child(_j)); tal_close(); (yyval.expr)=_a; }
#line 1987 "snobol4.tab.c"
    break;

  case 104: /* expr17: T_LPAREN T_RPAREN  */
#line 208 "snobol4.y"
                                                                                                { (yyval.expr)=ast_node_new(TT_NUL); }
#line 1993 "snobol4.tab.c"
    break;

  case 105: /* $@5: %empty  */
#line 209 "snobol4.y"
                              { tal_open(); tal_push(ast_node_new(TT_NUL)); }
#line 1999 "snobol4.tab.c"
    break;

  case 106: /* expr17: T_LPAREN T_COMMA $@5 vlist_args T_RPAREN  */
#line 209 "snobol4.y"
                                                                                                  { int _n=tal_count(); tree_t*_a=ast_node_new(TT_VLIST); for(int _j=0;_j<_n;_j++) expr_add_child(_a,tal_child(_j)); tal_close(); (yyval.expr)=_a; }
#line 2005 "snobol4.tab.c"
    break;

  case 107: /* $@6: %empty  */
#line 210 "snobol4.y"
                                 { tree_e _k=pat_prim_kind((yyvsp[-1].tok).sval); tal_open(); tal_fnc_open(_k,(char*)(yyvsp[-1].tok).sval); }
#line 2011 "snobol4.tab.c"
    break;

  case 108: /* expr17: T_FUNCTION T_LPAREN $@6 fnc_args T_RPAREN  */
#line 210 "snobol4.y"
                                                                                                                                      { (yyval.expr)=tal_fnc_close(); }
#line 2017 "snobol4.tab.c"
    break;

  case 109: /* expr17: T_IDENT  */
#line 211 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_VAR);e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2023 "snobol4.tab.c"
    break;

  case 110: /* expr17: T_END  */
#line 212 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_VAR);    e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2029 "snobol4.tab.c"
    break;

  case 111: /* expr17: T_KEYWORD  */
#line 213 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_KEYWORD);e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2035 "snobol4.tab.c"
    break;

  case 112: /* expr17: T_STR  */
#line 214 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_QLIT);   e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2041 "snobol4.tab.c"
    break;

  case 113: /* expr17: T_INT  */
#line 215 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_ILIT);   e->v.ival=(yyvsp[0].tok).ival;(yyval.expr)=e; }
#line 2047 "snobol4.tab.c"
    break;

  case 114: /* expr17: T_REAL  */
#line 216 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_FLIT);   e->v.dval=(yyvsp[0].tok).dval;(yyval.expr)=e; }
#line 2053 "snobol4.tab.c"
    break;

  case 115: /* vlist_args: vlist_args T_COMMA expr0  */
#line 218 "snobol4.y"
                                                                                                 { tal_push((yyvsp[0].expr)); }
#line 2059 "snobol4.tab.c"
    break;

  case 116: /* vlist_args: vlist_args T_COMMA  */
#line 219 "snobol4.y"
                                                                                                  { tal_push(ast_node_new(TT_NUL)); }
#line 2065 "snobol4.tab.c"
    break;

  case 117: /* vlist_args: expr0  */
#line 220 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 2071 "snobol4.tab.c"
    break;

  case 118: /* vlist_args: %empty  */
#line 221 "snobol4.y"
                                                                                                   { tal_push(ast_node_new(TT_NUL)); }
#line 2077 "snobol4.tab.c"
    break;

  case 119: /* fnc_args: fnc_args T_COMMA expr0  */
#line 223 "snobol4.y"
                                                                                                 { if (tal_count()==0) tal_push(ast_node_new(TT_NUL)); tal_push((yyvsp[0].expr)); }
#line 2083 "snobol4.tab.c"
    break;

  case 120: /* fnc_args: fnc_args T_COMMA  */
#line 224 "snobol4.y"
                                                                                                  { if (tal_count()==0) tal_push(ast_node_new(TT_NUL)); tal_push(ast_node_new(TT_NUL)); }
#line 2089 "snobol4.tab.c"
    break;

  case 121: /* fnc_args: expr0  */
#line 225 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 2095 "snobol4.tab.c"
    break;

  case 123: /* goto_atom: T_STR  */
#line 228 "snobol4.y"
                      { tree_t*e=ast_node_new(TT_QLIT); e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2101 "snobol4.tab.c"
    break;

  case 124: /* goto_atom: T_INT  */
#line 229 "snobol4.y"
                      { tree_t*e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].tok).ival; (yyval.expr)=e; }
#line 2107 "snobol4.tab.c"
    break;

  case 125: /* goto_atom: T_IDENT  */
#line 230 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2113 "snobol4.tab.c"
    break;

  case 126: /* goto_atom: T_FUNCTION  */
#line 231 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2119 "snobol4.tab.c"
    break;

  case 127: /* goto_atom: T_END  */
#line 232 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2125 "snobol4.tab.c"
    break;

  case 128: /* $@7: %empty  */
#line 233 "snobol4.y"
                                   { tree_e _k=pat_prim_kind((yyvsp[-1].tok).sval); tal_open(); tal_fnc_open(_k,(char*)(yyvsp[-1].tok).sval); }
#line 2131 "snobol4.tab.c"
    break;

  case 129: /* goto_atom: T_IDENT T_GOTO_LPAREN $@7 goto_fnc_args T_GOTO_RPAREN  */
#line 233 "snobol4.y"
                                                                                                                                                  { (yyval.expr)=tal_fnc_close(); }
#line 2137 "snobol4.tab.c"
    break;

  case 130: /* goto_fnc_args: goto_fnc_args T_COMMA goto_expr  */
#line 235 "snobol4.y"
                                                                                                { tal_push((yyvsp[0].expr)); }
#line 2143 "snobol4.tab.c"
    break;

  case 131: /* goto_fnc_args: goto_fnc_args T_COMMA  */
#line 236 "snobol4.y"
                                                                                                { tal_push(ast_node_new(TT_NUL)); }
#line 2149 "snobol4.tab.c"
    break;

  case 132: /* goto_fnc_args: goto_expr  */
#line 237 "snobol4.y"
                                                                                                { tal_push((yyvsp[0].expr)); }
#line 2155 "snobol4.tab.c"
    break;

  case 133: /* goto_fnc_args: %empty  */
#line 238 "snobol4.y"
                                                                                                { }
#line 2161 "snobol4.tab.c"
    break;

  case 134: /* goto_expr: goto_atom  */
#line 240 "snobol4.y"
                                                                                                  { (yyval.expr)=(yyvsp[0].expr); }
#line 2167 "snobol4.tab.c"
    break;

  case 135: /* goto_expr: goto_expr T_CONCAT goto_atom  */
#line 241 "snobol4.y"
                                                                                                  { tree_t*s=ast_node_new(TT_SEQ);expr_add_child(s,(yyvsp[-2].expr));expr_add_child(s,(yyvsp[0].expr));(yyval.expr)=s; }
#line 2173 "snobol4.tab.c"
    break;


#line 2177 "snobol4.tab.c"

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
  yytoken = yychar == SNOBOL4_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (yyparse_param, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SNOBOL4_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SNOBOL4_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, yyparse_param);
          yychar = SNOBOL4_EMPTY;
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yyparse_param);
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
  yyerror (yyparse_param, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != SNOBOL4_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, yyparse_param);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yyparse_param);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 243 "snobol4.y"

int snobol4_lex(YYSTYPE *yylval_param) {
    Token t=lex_next(g_lx); yylval_param->tok=t; if (t.lineno) g_err_lineno=t.lineno;
    if (getenv("SNO_TOK_TRACE"))
        fprintf(stderr,"[TOK %d sval=%s ival=%ld]\n",t.kind,t.sval?t.sval:"",t.ival);
    return t.kind;
}
void snobol4_error(void *p,const char *msg){(void)p;sno_error(g_err_lineno,"parse error: %s",msg);}
static void sno4_stmt_commit_go(void *param,Token lbl,tree_t *subj,tree_t *pat,int has_eq,tree_t *repl,tree_t *gu,tree_t *gs,tree_t *gf){
    PP *pp=(PP*)param;
    STMT_t *s=stmt_new();
    { const tree_t *s0=subj; while(s0&&(s0->t==TT_SCAN||s0->t==TT_SEQ)&&s0->n>0&&s0->c[0]) s0=s0->c[0]; if(has_eq&&sno_target_value(s0)) sno_target_refuse(); }
    s->goto_f_first=sno4_goto_f_first; sno4_goto_f_first=0;
    s->lineno = lbl.lineno ? lbl.lineno : snobol4_get_stmt_lineno();
    { extern int snobol4_get_stmt_lline(void); extern const char *snobol4_get_stmt_file(void); extern const char *snobol4_get_stmt_kwfile(void);
      s->lline = snobol4_get_stmt_lline(); s->file = ct_strdup(snobol4_get_stmt_file()); if (snobol4_get_stmt_kwfile()) s->kwfile = ct_strdup(snobol4_get_stmt_kwfile()); }
    { extern int snobol4_get_nofail_mode(void); s->nofail = snobol4_get_nofail_mode(); }
    s->stno = ++pp->prog->nstmts;
    if(lbl.sval){s->label=ct_strdup(lbl.sval);s->is_end=lbl.ival||(strcmp(lbl.sval,"END")==0);
        for(STMT_t *p=pp->prog->head;p;p=p->next) if(p->label&&!strcmp(p->label,lbl.sval)){sno_error(s->lineno,"duplicate label '%s'",lbl.sval);break;}}
    s->subject=subj; s->pattern=pat;
    if(has_eq){s->has_eq=1;s->replacement=repl;}
    if(gu) s->goto_u_expr=gu;
    if(gs) s->goto_s_expr=gs;
    if(gf) s->goto_f_expr=gf;
    if(!pp->prog->head) pp->prog->head=pp->prog->tail=s; else{pp->prog->tail->next=s;pp->prog->tail=s;}
    if (pp->ast_prog) {
        tree_t *anode = stmt_to_ast(s);
        ast_push(pp->ast_prog, anode);
    }
}
static tree_t *parse_expr(Lex *lx){
    CODE_t *prog=ct_zalloc(1,sizeof*prog);PP p={prog,NULL,NULL};g_lx=lx;snobol4_parse(&p);
    return prog->head?prog->head->subject:NULL;
}
CODE_t *parse_program_tokens(Lex *stream){
    CODE_t *prog=ct_zalloc(1,sizeof*prog);PP p={prog,NULL,NULL};g_lx=stream;snobol4_parse(&p);return prog;
}
CODE_t *parse_program_tokens_ast(Lex *stream, tree_t **ast_out){
    CODE_t *prog=ct_zalloc(1,sizeof*prog);
    tree_t *ast=ct_zalloc(1,sizeof*ast); ast->t=TT_PROGRAM;
    PP p={prog,NULL,ast};g_lx=stream;snobol4_parse(&p);
    *ast_out=ast;
    return prog;
}
CODE_t *parse_program(LineArray *lines){(void)lines;return ct_zalloc(1,sizeof(CODE_t));}
tree_t *parse_expr_from_str(const char *src){
    if(!src||!*src) return NULL;Lex lx={0};lex_open_str(&lx,src,(int)strlen(src),0);tree_t *r=parse_expr(&lx);flex_lex_destroy(&lx);return r;
}
static void sno4_stmt_shells_drop(CODE_t *prog){
    for(STMT_t *s=prog->head,*n;s;s=n){n=s->next;ct_drop(s->file);ct_drop(s->kwfile);ct_drop(s->label);ct_drop(s);}
    ct_drop(prog);
}
tree_t *parse_expr_pat_from_str(const char *src) {
    if (!src || !*src) return NULL;
    int slen = (int)strlen(src);
    char *buf = ct_alloc(slen + 2);
    if (!buf) return NULL;
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str(&lx, buf, slen + 1, 0);
    CODE_t *prog = ct_zalloc(1, sizeof(CODE_t));
    PP p = {prog, NULL, NULL};
    g_lx = &lx;
    snobol4_parse(&p);
    flex_lex_destroy(&lx);
    ct_drop(buf);
    if (p.ast_prog && p.ast_prog->n > 0) {
        const tree_t *s = p.ast_prog->c[0];
        if (s) {
            tree_t *pat = stmt_attr_expr(stmt_attr_find(s, ":pat"));
            if (pat) { ct_drop(prog); return pat; }
            return stmt_attr_expr(stmt_attr_find(s, ":subj"));
        }
    }
    if (!prog->head) { ct_drop(prog); return NULL; }
    STMT_t *s = prog->head;
    tree_t *res = s->pattern ? s->pattern : s->subject;
    sno4_stmt_shells_drop(prog);
    return res;
}
CODE_t *sno_parse_string(const char *src) {
    if (!src) return ct_zalloc(1, sizeof(CODE_t));
    int slen = (int)strlen(src);
    char *buf = ct_alloc(slen + 2);
    if (!buf) return ct_zalloc(1, sizeof(CODE_t));
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str_initial(&lx, buf, slen + 1, 0);
    CODE_t *prog = ct_zalloc(1, sizeof(CODE_t));
    PP p = {prog, NULL, NULL};
    g_lx = &lx;
    snobol4_parse(&p);
    flex_lex_destroy(&lx);
    ct_drop(buf);
    return prog;
}
tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out) {
    if (!src) { if (code_out) *code_out = ct_zalloc(1, sizeof(CODE_t)); return NULL; }
    int slen = (int)strlen(src);
    char *buf = ct_alloc(slen + 2);
    if (!buf) { if (code_out) *code_out = ct_zalloc(1, sizeof(CODE_t)); return NULL; }
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str_initial(&lx, buf, slen + 1, 0);
    CODE_t *prog = ct_zalloc(1, sizeof(CODE_t));
    tree_t  *ast  = ct_zalloc(1, sizeof(tree_t)); ast->t = TT_PROGRAM;
    PP p = {prog, NULL, ast};
    g_lx = &lx;
    snobol4_parse(&p);
    flex_lex_destroy(&lx);
    ast = p.ast_prog;
    ct_drop(buf);
    if (code_out) *code_out = prog; else ct_drop(prog);
    return (ast && ast->n > 0) ? ast : NULL;
}
