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
  YYSYMBOL_opt_pattern = 63,               /* opt_pattern  */
  YYSYMBOL_opt_repl = 64,                  /* opt_repl  */
  YYSYMBOL_goto_label_expr = 65,           /* goto_label_expr  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_expr0 = 67,                     /* expr0  */
  YYSYMBOL_expr2 = 68,                     /* expr2  */
  YYSYMBOL_expr3 = 69,                     /* expr3  */
  YYSYMBOL_expr4 = 70,                     /* expr4  */
  YYSYMBOL_expr5 = 71,                     /* expr5  */
  YYSYMBOL_expr6 = 72,                     /* expr6  */
  YYSYMBOL_expr7 = 73,                     /* expr7  */
  YYSYMBOL_expr8 = 74,                     /* expr8  */
  YYSYMBOL_expr9 = 75,                     /* expr9  */
  YYSYMBOL_expr10 = 76,                    /* expr10  */
  YYSYMBOL_expr11 = 77,                    /* expr11  */
  YYSYMBOL_expr12 = 78,                    /* expr12  */
  YYSYMBOL_expr13 = 79,                    /* expr13  */
  YYSYMBOL_expr14 = 80,                    /* expr14  */
  YYSYMBOL_expr15 = 81,                    /* expr15  */
  YYSYMBOL_82_2 = 82,                      /* $@2  */
  YYSYMBOL_83_3 = 83,                      /* $@3  */
  YYSYMBOL_idx_args = 84,                  /* idx_args  */
  YYSYMBOL_expr17 = 85,                    /* expr17  */
  YYSYMBOL_86_4 = 86,                      /* $@4  */
  YYSYMBOL_87_5 = 87,                      /* $@5  */
  YYSYMBOL_vlist_args = 88,                /* vlist_args  */
  YYSYMBOL_fnc_args = 89,                  /* fnc_args  */
  YYSYMBOL_goto_atom = 90,                 /* goto_atom  */
  YYSYMBOL_goto_fnc_args = 91,             /* goto_fnc_args  */
  YYSYMBOL_goto_expr = 92                  /* goto_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 5 "snobol4.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct { CODE_t *prog; tree_t **result; tree_t *ast_prog; } PP;
static void     sno4_stmt_commit_go(void*,Token,tree_t*,tree_t*,int,tree_t*,tree_t*,tree_t*,tree_t*);
static Lex     *g_lx;
static int      g_err_lineno;
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
    if (k==TT_VAR) e->v.sval=sv;
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
        {"FENCE",TT_FENCE},{"ABORT",TT_ABORT},{"BAL",TT_BAL},{NULL,TT_VAR}
    };
    for (int i = 0; m[i].n; i++) if (strcmp(s, m[i].n) == 0) return m[i].k;
    return TT_VAR;
}

#line 246 "snobol4.tab.c"

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
#define YYFINAL  70
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   300

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  237

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
       0,    69,    69,    70,    72,    72,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   102,   103,   104,   106,   107,   109,   110,   111,   114,
     115,   116,   117,   118,   119,   120,   120,   121,   123,   124,
     125,   127,   128,   130,   131,   133,   134,   136,   137,   139,
     140,   141,   143,   144,   146,   147,   149,   150,   152,   153,
     155,   156,   158,   159,   160,   162,   163,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   183,   183,   184,   184,   185,   187,
     188,   189,   190,   192,   193,   193,   194,   195,   195,   196,
     197,   198,   199,   200,   201,   203,   204,   206,   207,   208,
     209,   211,   212,   213,   214,   215,   217,   218,   219,   220,
     222,   223
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
  "opt_subject", "opt_pattern", "opt_repl", "goto_label_expr", "$@1",
  "expr0", "expr2", "expr3", "expr4", "expr5", "expr6", "expr7", "expr8",
  "expr9", "expr10", "expr11", "expr12", "expr13", "expr14", "expr15",
  "$@2", "$@3", "idx_args", "expr17", "$@4", "$@5", "vlist_args",
  "fnc_args", "goto_atom", "goto_fnc_args", "goto_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-184)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-57)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     128,  -184,   -44,  -184,  -184,  -184,  -184,  -184,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   196,    37,   147,  -184,  -184,
      22,    15,    62,    48,    -3,   127,    76,   101,   107,   110,
    -184,    90,  -184,   -18,    -5,  -184,  -184,    22,    26,  -184,
    -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,  -184,  -184,  -184,  -184,  -184,    52,     5,   115,   175,
    -184,  -184,   216,    44,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
    -184,  -184,   216,    81,   216,  -184,  -184,   216,   216,  -184,
      98,    98,     9,   216,  -184,   192,    22,    62,    62,    48,
     115,   127,    76,    76,   101,   107,   110,  -184,  -184,  -184,
    -184,  -184,   182,   216,   216,  -184,    74,    98,    98,  -184,
     194,    22,   216,  -184,  -184,    57,    10,   146,   198,   199,
      19,   200,  -184,    88,  -184,    16,   -12,   216,  -184,    67,
      41,  -184,    97,  -184,   156,    98,  -184,    98,  -184,  -184,
    -184,  -184,  -184,   201,   203,   136,  -184,    98,    98,  -184,
     197,   216,  -184,  -184,  -184,    98,  -184,    98,  -184,    98,
      98,  -184,   209,   216,  -184,   210,   211,   136,  -184,  -184,
    -184,  -184,  -184,  -184,  -184,  -184,     4,    69,    51,  -184,
    -184,   228,   229,    78,    60,  -184,  -184,  -184,  -184,    -8,
     169,   252,   136,    98,  -184,    98,  -184,  -184,  -184,    98,
    -184,    98,  -184,   254,   136,  -184,  -184,   253,   255,   273,
     274,  -184,   169,  -184,  -184,  -184,  -184
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      33,   109,     0,   111,   110,   113,   114,   112,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,     5,    18,
      38,     0,    52,    54,    32,    58,    61,    63,    65,    67,
      69,    71,    74,    76,    93,    98,   107,    38,     0,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    92,    90,    91,   106,     0,    50,    56,    76,
       1,     4,    37,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    96,   120,     0,    35,   104,   103,     0,     0,    36,
       0,     0,     0,     0,    19,     0,    38,    34,    51,    53,
      55,    57,    59,    60,    62,    64,    66,    68,    70,    72,
      73,    75,    31,   102,   102,   119,     0,     0,     0,     6,
       0,    38,     0,    48,    49,     0,     0,     0,     0,     0,
       0,     0,    20,     0,   101,     0,     0,   118,   108,     0,
       0,     7,     0,   116,     0,     0,    21,     0,    22,    45,
      39,    41,    40,     0,     0,     0,    47,     0,     0,    25,
       0,   100,    95,    97,   117,     0,     8,     0,     9,     0,
       0,    12,     0,     0,   105,     0,     0,   129,    42,    44,
     123,   124,   125,   122,   121,   130,     0,     0,     0,    26,
      99,     0,     0,     0,     0,    13,   115,    23,    24,     0,
     128,     0,     0,     0,    27,     0,    28,    10,    11,     0,
      14,     0,    15,     0,   127,    43,   131,     0,     0,     0,
       0,    46,   126,    29,    30,    16,    17
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -184,  -184,  -184,   242,  -184,   263,   202,   -42,   -92,  -184,
     -25,     2,   -55,   217,     3,   219,   117,   213,   218,   212,
      93,  -184,    32,   265,  -184,  -184,  -184,   174,  -184,  -184,
    -184,  -184,  -184,    87,  -184,  -183
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,   106,    73,   105,   187,
     144,    67,    32,    33,    68,    35,    36,    37,    38,    39,
      40,    41,    42,    69,    44,   123,   124,   145,    45,   132,
      92,   154,   126,   195,   209,   196
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      66,   130,    31,    34,   210,    93,   223,    46,   135,   136,
      48,    34,   137,   138,    88,   139,   -56,   -56,   211,   107,
     108,   157,   163,    97,    98,   -56,    78,   158,   164,    31,
      34,    89,   165,    75,    74,   149,   150,    70,   171,   107,
      72,   232,   224,    75,   173,    94,   -56,    99,    90,   140,
      91,   170,   177,   212,    75,   100,   101,   102,   178,   103,
     182,   104,   215,   185,   143,   186,   171,   125,   216,   155,
     172,   221,   133,   134,   156,   197,   198,   222,   141,   175,
     110,   213,    76,   201,   176,   202,   214,   203,   204,   152,
     219,   122,   127,   128,   102,   220,   103,    77,   129,   167,
     168,   102,    95,   103,    96,   169,    81,   153,   179,   180,
     102,   102,   103,   103,   181,    85,    86,    87,   119,   120,
     121,   227,   174,   228,   147,    82,   148,   229,    -3,   230,
      83,     1,     2,     3,     4,     5,     6,     7,     8,   190,
     191,    84,   192,   193,    78,   194,   200,    -2,    79,    80,
       1,     2,     3,     4,     5,     6,     7,     8,   206,   159,
     160,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,   117,   118,    25,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,   112,   113,    25,     1,
       2,     3,     4,     5,     6,     7,   183,    88,   184,   142,
      75,   151,   161,   162,   199,   188,   166,   189,   212,     1,
       2,     3,     4,     5,     6,     7,   205,   207,   208,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,   217,   218,    25,    65,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    43,   225,    25,   231,    71,
     233,    47,   234,    43,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
     235,   236,    43,   109,   114,   116,   131,   111,   146,   226,
     115
};

static const yytype_uint8 yycheck[] =
{
      25,    93,     0,     0,   187,    47,    14,    51,   100,   101,
       8,     8,     3,     4,    32,     6,    19,    20,    14,    74,
      75,    11,     3,    18,    19,    28,    29,    17,     9,    27,
      27,    49,    13,    28,    19,   127,   128,     0,    50,    94,
      18,   224,    50,    28,    56,    19,    49,    72,    53,    40,
      55,   143,    11,    49,    28,    11,    12,    13,    17,    15,
     152,    17,    11,   155,   106,   157,    50,    92,    17,    12,
      54,    11,    97,    98,    17,   167,   168,    17,   103,    12,
      77,    12,    20,   175,    17,   177,    17,   179,   180,   131,
      12,    89,    11,    12,    13,    17,    15,    49,    17,    11,
      12,    13,    50,    15,    52,    17,    30,   132,    11,    12,
      13,    13,    15,    15,    17,    25,    26,    27,    86,    87,
      88,   213,   147,   215,    50,    24,    52,   219,     0,   221,
      23,     3,     4,     5,     6,     7,     8,     9,    10,     3,
       4,    31,     6,     7,    29,     9,   171,     0,    21,    22,
       3,     4,     5,     6,     7,     8,     9,    10,   183,    13,
      14,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    84,    85,    51,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    79,    80,    51,     3,
       4,     5,     6,     7,     8,     9,    50,    32,    52,    17,
      28,    17,    14,    14,    17,    14,    16,    14,    49,     3,
       4,     5,     6,     7,     8,     9,    17,    17,    17,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    17,    17,    51,    52,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,     0,    14,    51,    14,    27,
      17,     8,    17,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      17,    17,    27,    76,    81,    83,    94,    78,   124,   212,
      82
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    51,    58,    59,    60,    61,
      62,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    85,    51,    62,    68,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    52,    67,    68,    71,    80,
       0,    60,    18,    64,    19,    28,    20,    49,    29,    21,
      22,    30,    24,    23,    31,    25,    26,    27,    32,    49,
      53,    55,    87,    64,    19,    50,    52,    18,    19,    67,
      11,    12,    13,    15,    17,    65,    63,    69,    69,    70,
      71,    72,    73,    73,    74,    75,    76,    77,    77,    79,
      79,    79,    68,    82,    83,    67,    89,    11,    12,    17,
      65,    63,    86,    67,    67,    65,    65,     3,     4,     6,
      40,    67,    17,    64,    67,    84,    84,    50,    52,    65,
      65,    17,    64,    67,    88,    12,    17,    11,    17,    13,
      14,    14,    14,     3,     9,    13,    16,    11,    12,    17,
      65,    50,    54,    56,    67,    12,    17,    11,    17,    11,
      12,    17,    65,    50,    52,    65,    65,    66,    14,    14,
       3,     4,     6,     7,     9,    90,    92,    65,    65,    17,
      67,    65,    65,    65,    65,    17,    67,    17,    17,    91,
      92,    14,    49,    12,    17,    11,    17,    17,    17,    12,
      17,    11,    17,    14,    50,    14,    90,    65,    65,    65,
      65,    14,    92,    17,    17,    17,    17
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    62,    62,    62,    63,    63,    64,    64,    64,    65,
      65,    65,    65,    65,    65,    66,    65,    65,    67,    67,
      67,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    78,    78,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    82,    81,    83,    81,    81,    84,
      84,    84,    84,    85,    86,    85,    85,    87,    85,    85,
      85,    85,    85,    85,    85,    88,    88,    89,    89,    89,
      89,    90,    90,    90,    90,    90,    91,    91,    91,    91,
      92,    92
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     4,     5,     6,     6,
       8,     8,     6,     7,     8,     8,    10,    10,     1,     3,
       4,     5,     5,     7,     7,     5,     6,     7,     7,     9,
       9,     3,     1,     0,     1,     0,     2,     1,     0,     3,
       3,     3,     4,     6,     4,     0,     7,     3,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     0,     5,     0,     5,     1,     3,
       2,     1,     0,     3,     0,     6,     2,     0,     5,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     2,     1,
       0,     1,     1,     1,     1,     1,     3,     2,     1,     0,
       1,     3
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
#line 69 "snobol4.y"
                                                                                                    { }
#line 1388 "snobol4.tab.c"
    break;

  case 3: /* top: %empty  */
#line 70 "snobol4.y"
                                                                                        { }
#line 1394 "snobol4.tab.c"
    break;

  case 6: /* stmt: T_LABEL opt_subject opt_repl T_STMT_END  */
#line 74 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-3].tok),(yyvsp[-2].expr),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1400 "snobol4.tab.c"
    break;

  case 7: /* stmt: T_LABEL opt_subject opt_repl goto_label_expr T_STMT_END  */
#line 75 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-4].tok),(yyvsp[-3].expr),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1406 "snobol4.tab.c"
    break;

  case 8: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 76 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-5].tok),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1412 "snobol4.tab.c"
    break;

  case 9: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 77 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,(yyvsp[-5].tok),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1418 "snobol4.tab.c"
    break;

  case 10: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 78 "snobol4.y"
                                                                                                         { sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1424 "snobol4.tab.c"
    break;

  case 11: /* stmt: T_LABEL opt_subject opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 79 "snobol4.y"
                                                                                                         { sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1430 "snobol4.tab.c"
    break;

  case 12: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl T_STMT_END  */
#line 80 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,(yyvsp[-5].tok),expr_binary(TT_SCAN,(yyvsp[-4].expr),(yyvsp[-2].expr)),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1436 "snobol4.tab.c"
    break;

  case 13: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl goto_label_expr T_STMT_END  */
#line 81 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,(yyvsp[-6].tok),expr_binary(TT_SCAN,(yyvsp[-5].expr),(yyvsp[-3].expr)),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1442 "snobol4.tab.c"
    break;

  case 14: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 82 "snobol4.y"
                                                                                             { sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),expr_binary(TT_SCAN,(yyvsp[-6].expr),(yyvsp[-4].expr)),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1448 "snobol4.tab.c"
    break;

  case 15: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 83 "snobol4.y"
                                                                                             { sno4_stmt_commit_go(yyparse_param,(yyvsp[-7].tok),expr_binary(TT_SCAN,(yyvsp[-6].expr),(yyvsp[-4].expr)),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1454 "snobol4.tab.c"
    break;

  case 16: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 84 "snobol4.y"
                                                                                                                      { sno4_stmt_commit_go(yyparse_param,(yyvsp[-9].tok),expr_binary(TT_SCAN,(yyvsp[-8].expr),(yyvsp[-6].expr)),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1460 "snobol4.tab.c"
    break;

  case 17: /* stmt: T_LABEL expr2 T_2QUEST opt_pattern opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 85 "snobol4.y"
                                                                                                                      { sno4_stmt_commit_go(yyparse_param,(yyvsp[-9].tok),expr_binary(TT_SCAN,(yyvsp[-8].expr),(yyvsp[-6].expr)),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1466 "snobol4.tab.c"
    break;

  case 19: /* unlabeled_stmt: opt_subject opt_repl T_STMT_END  */
#line 89 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-2].expr),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1472 "snobol4.tab.c"
    break;

  case 20: /* unlabeled_stmt: opt_subject opt_repl goto_label_expr T_STMT_END  */
#line 90 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-3].expr),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1478 "snobol4.tab.c"
    break;

  case 21: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 91 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1484 "snobol4.tab.c"
    break;

  case 22: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 92 "snobol4.y"
                                                                                           { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-4].expr),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1490 "snobol4.tab.c"
    break;

  case 23: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 93 "snobol4.y"
                                                                                                 { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1496 "snobol4.tab.c"
    break;

  case 24: /* unlabeled_stmt: opt_subject opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 94 "snobol4.y"
                                                                                                 { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),(yyvsp[-6].expr),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1502 "snobol4.tab.c"
    break;

  case 25: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl T_STMT_END  */
#line 95 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-4].expr),(yyvsp[-2].expr)),NULL,((yyvsp[-1].expr)!=NULL),(yyvsp[-1].expr),NULL,NULL,NULL); }
#line 1508 "snobol4.tab.c"
    break;

  case 26: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl goto_label_expr T_STMT_END  */
#line 96 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-5].expr),(yyvsp[-3].expr)),NULL,((yyvsp[-2].expr)!=NULL),(yyvsp[-2].expr),(yyvsp[-1].expr),NULL,NULL); }
#line 1514 "snobol4.tab.c"
    break;

  case 27: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl T_GOTO_S goto_label_expr T_STMT_END  */
#line 97 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-6].expr),(yyvsp[-4].expr)),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,(yyvsp[-1].expr),NULL); }
#line 1520 "snobol4.tab.c"
    break;

  case 28: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl T_GOTO_F goto_label_expr T_STMT_END  */
#line 98 "snobol4.y"
                                                                                         { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-6].expr),(yyvsp[-4].expr)),NULL,((yyvsp[-3].expr)!=NULL),(yyvsp[-3].expr),NULL,NULL,(yyvsp[-1].expr)); }
#line 1526 "snobol4.tab.c"
    break;

  case 29: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl T_GOTO_S goto_label_expr T_GOTO_F goto_label_expr T_STMT_END  */
#line 99 "snobol4.y"
                                                                                                              { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-8].expr),(yyvsp[-6].expr)),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1532 "snobol4.tab.c"
    break;

  case 30: /* unlabeled_stmt: expr2 T_2QUEST opt_pattern opt_repl T_GOTO_F goto_label_expr T_GOTO_S goto_label_expr T_STMT_END  */
#line 100 "snobol4.y"
                                                                                                              { sno4_stmt_commit_go(yyparse_param,((Token){NULL,0,0,0}),expr_binary(TT_SCAN,(yyvsp[-8].expr),(yyvsp[-6].expr)),NULL,((yyvsp[-5].expr)!=NULL),(yyvsp[-5].expr),NULL,(yyvsp[-1].expr),(yyvsp[-3].expr)); }
#line 1538 "snobol4.tab.c"
    break;

  case 31: /* opt_subject: expr14 T_CONCAT expr2  */
#line 102 "snobol4.y"
                                                                                                   { (yyval.expr)=expr_binary(TT_SCAN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1544 "snobol4.tab.c"
    break;

  case 32: /* opt_subject: expr5  */
#line 103 "snobol4.y"
                                                                                                    { (yyval.expr)=(yyvsp[0].expr); }
#line 1550 "snobol4.tab.c"
    break;

  case 33: /* opt_subject: %empty  */
#line 104 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1556 "snobol4.tab.c"
    break;

  case 34: /* opt_pattern: expr3  */
#line 106 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1562 "snobol4.tab.c"
    break;

  case 35: /* opt_pattern: %empty  */
#line 107 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1568 "snobol4.tab.c"
    break;

  case 36: /* opt_repl: T_2EQUAL expr0  */
#line 109 "snobol4.y"
                                                                                              { (yyval.expr)=(yyvsp[0].expr); }
#line 1574 "snobol4.tab.c"
    break;

  case 37: /* opt_repl: T_2EQUAL  */
#line 110 "snobol4.y"
                                                                                               { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=strdup("");(yyval.expr)=e; }
#line 1580 "snobol4.tab.c"
    break;

  case 38: /* opt_repl: %empty  */
#line 111 "snobol4.y"
                                                                                       { (yyval.expr)=NULL; }
#line 1586 "snobol4.tab.c"
    break;

  case 39: /* goto_label_expr: T_GOTO_LPAREN T_IDENT T_GOTO_RPAREN  */
#line 114 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1592 "snobol4.tab.c"
    break;

  case 40: /* goto_label_expr: T_GOTO_LPAREN T_END T_GOTO_RPAREN  */
#line 115 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1598 "snobol4.tab.c"
    break;

  case 41: /* goto_label_expr: T_GOTO_LPAREN T_FUNCTION T_GOTO_RPAREN  */
#line 116 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1604 "snobol4.tab.c"
    break;

  case 42: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_IDENT T_GOTO_RPAREN  */
#line 117 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);char buf[512];snprintf(buf,sizeof buf,"$%s",(yyvsp[-1].tok).sval);e->v.sval=strdup(buf);(yyval.expr)=e; }
#line 1610 "snobol4.tab.c"
    break;

  case 43: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_GOTO_LPAREN goto_expr T_GOTO_RPAREN T_GOTO_RPAREN  */
#line 118 "snobol4.y"
                                                                                            { (yyval.expr)=(yyvsp[-2].expr); }
#line 1616 "snobol4.tab.c"
    break;

  case 44: /* goto_label_expr: T_GOTO_LPAREN T_1DOLLAR T_STR T_GOTO_RPAREN  */
#line 119 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_QLIT);e->v.sval=strdup((yyvsp[-1].tok).sval);(yyval.expr)=e; }
#line 1622 "snobol4.tab.c"
    break;

  case 45: /* $@1: %empty  */
#line 120 "snobol4.y"
                                                 { tree_e _k=pat_prim_kind((yyvsp[-1].tok).sval); tal_open(); tal_fnc_open(_k,(char*)(yyvsp[-1].tok).sval); }
#line 1628 "snobol4.tab.c"
    break;

  case 46: /* goto_label_expr: T_GOTO_LPAREN T_IDENT T_GOTO_LPAREN $@1 goto_fnc_args T_GOTO_RPAREN T_GOTO_RPAREN  */
#line 120 "snobol4.y"
                                                                                                                                                                              { (yyval.expr)=tal_fnc_close(); }
#line 1634 "snobol4.tab.c"
    break;

  case 47: /* goto_label_expr: T_GOTO_LANGLE expr0 T_GOTO_RANGLE  */
#line 121 "snobol4.y"
                                                                                             { tree_t*e=ast_node_new(TT_GOTO_DIRECT);expr_add_child(e,(yyvsp[-1].expr));(yyval.expr)=e; }
#line 1640 "snobol4.tab.c"
    break;

  case 48: /* expr0: expr2 T_2EQUAL expr0  */
#line 123 "snobol4.y"
                                                                                              { (yyval.expr)=expr_binary(TT_ASSIGN,          (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1646 "snobol4.tab.c"
    break;

  case 49: /* expr0: expr2 T_2QUEST expr0  */
#line 124 "snobol4.y"
                                                                                                   { (yyval.expr)=expr_binary(TT_SCAN,            (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1652 "snobol4.tab.c"
    break;

  case 50: /* expr0: expr2  */
#line 125 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1658 "snobol4.tab.c"
    break;

  case 51: /* expr2: expr2 T_2AMP expr3  */
#line 127 "snobol4.y"
                                                                                             { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=strdup("&"); (yyval.expr)=_e; }
#line 1664 "snobol4.tab.c"
    break;

  case 52: /* expr2: expr3  */
#line 128 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1670 "snobol4.tab.c"
    break;

  case 53: /* expr3: expr3 T_2PIPE expr4  */
#line 130 "snobol4.y"
                                                                                            { tree_t*a=ast_node_new(TT_ALT);expr_add_child(a,(yyvsp[-2].expr));expr_add_child(a,(yyvsp[0].expr));(yyval.expr)=a; }
#line 1676 "snobol4.tab.c"
    break;

  case 54: /* expr3: expr4  */
#line 131 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1682 "snobol4.tab.c"
    break;

  case 55: /* expr4: expr4 T_CONCAT expr5  */
#line 133 "snobol4.y"
                                                                                                            { tree_t*s=ast_node_new(TT_SEQ);expr_add_child(s,(yyvsp[-2].expr));expr_add_child(s,(yyvsp[0].expr));(yyval.expr)=s; }
#line 1688 "snobol4.tab.c"
    break;

  case 56: /* expr4: expr5  */
#line 134 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1694 "snobol4.tab.c"
    break;

  case 57: /* expr5: expr5 T_2AT expr6  */
#line 136 "snobol4.y"
                                                                                              { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=strdup("@"); (yyval.expr)=_e; }
#line 1700 "snobol4.tab.c"
    break;

  case 58: /* expr5: expr6  */
#line 137 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1706 "snobol4.tab.c"
    break;

  case 59: /* expr6: expr6 T_2PLUS expr7  */
#line 139 "snobol4.y"
                                                                                               { (yyval.expr)=expr_binary(TT_ADD,             (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1712 "snobol4.tab.c"
    break;

  case 60: /* expr6: expr6 T_2MINUS expr7  */
#line 140 "snobol4.y"
                                                                                             { (yyval.expr)=expr_binary(TT_SUB,             (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1718 "snobol4.tab.c"
    break;

  case 61: /* expr6: expr7  */
#line 141 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1724 "snobol4.tab.c"
    break;

  case 62: /* expr7: expr7 T_2POUND expr8  */
#line 143 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=strdup("#"); (yyval.expr)=_e; }
#line 1730 "snobol4.tab.c"
    break;

  case 63: /* expr7: expr8  */
#line 144 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1736 "snobol4.tab.c"
    break;

  case 64: /* expr8: expr8 T_2SLASH expr9  */
#line 146 "snobol4.y"
                                                                                                { (yyval.expr)=expr_binary(TT_DIV,             (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1742 "snobol4.tab.c"
    break;

  case 65: /* expr8: expr9  */
#line 147 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1748 "snobol4.tab.c"
    break;

  case 66: /* expr9: expr9 T_2STAR expr10  */
#line 149 "snobol4.y"
                                                                                         { (yyval.expr)=expr_binary(TT_MUL,             (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1754 "snobol4.tab.c"
    break;

  case 67: /* expr9: expr10  */
#line 150 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1760 "snobol4.tab.c"
    break;

  case 68: /* expr10: expr10 T_2PERCENT expr11  */
#line 152 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=strdup("%"); (yyval.expr)=_e; }
#line 1766 "snobol4.tab.c"
    break;

  case 69: /* expr10: expr11  */
#line 153 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1772 "snobol4.tab.c"
    break;

  case 70: /* expr11: expr12 T_2CARET expr11  */
#line 155 "snobol4.y"
                                                                                          { (yyval.expr)=expr_binary(TT_POW,             (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1778 "snobol4.tab.c"
    break;

  case 71: /* expr11: expr12  */
#line 156 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1784 "snobol4.tab.c"
    break;

  case 72: /* expr12: expr12 T_2DOLLAR expr13  */
#line 158 "snobol4.y"
                                                                                         { (yyval.expr)=expr_binary(TT_CAPT_IMMED_ASGN,(yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1790 "snobol4.tab.c"
    break;

  case 73: /* expr12: expr12 T_2DOT expr13  */
#line 159 "snobol4.y"
                                                                                           { (yyval.expr)=expr_binary(TT_CAPT_COND_ASGN, (yyvsp[-2].expr),(yyvsp[0].expr)); }
#line 1796 "snobol4.tab.c"
    break;

  case 74: /* expr12: expr13  */
#line 160 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1802 "snobol4.tab.c"
    break;

  case 75: /* expr13: expr14 T_2TILDE expr13  */
#line 162 "snobol4.y"
                                                                                                   { tree_t*_e=expr_binary(TT_OPSYN,(yyvsp[-2].expr),(yyvsp[0].expr)); _e->v.sval=strdup("~"); (yyval.expr)=_e; }
#line 1808 "snobol4.tab.c"
    break;

  case 76: /* expr13: expr14  */
#line 163 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1814 "snobol4.tab.c"
    break;

  case 77: /* expr14: T_1AT expr14  */
#line 165 "snobol4.y"
                                                                                           { (yyval.expr)=expr_unary(TT_CAPT_CURSOR,     (yyvsp[0].expr)); }
#line 1820 "snobol4.tab.c"
    break;

  case 78: /* expr14: T_1TILDE expr14  */
#line 166 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_NOT,             (yyvsp[0].expr)); }
#line 1826 "snobol4.tab.c"
    break;

  case 79: /* expr14: T_1QUEST expr14  */
#line 167 "snobol4.y"
                                                                                        { (yyval.expr)=expr_unary(TT_INTERROGATE,     (yyvsp[0].expr)); }
#line 1832 "snobol4.tab.c"
    break;

  case 80: /* expr14: T_1AMP expr14  */
#line 168 "snobol4.y"
                                                                                          { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("&"); (yyval.expr)=_e; }
#line 1838 "snobol4.tab.c"
    break;

  case 81: /* expr14: T_1PLUS expr14  */
#line 169 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_PLS,             (yyvsp[0].expr)); }
#line 1844 "snobol4.tab.c"
    break;

  case 82: /* expr14: T_1MINUS expr14  */
#line 170 "snobol4.y"
                                                                                                { (yyval.expr)=expr_unary(TT_MNS,             (yyvsp[0].expr)); }
#line 1850 "snobol4.tab.c"
    break;

  case 83: /* expr14: T_1STAR expr14  */
#line 171 "snobol4.y"
                                                                                            { (yyval.expr)=expr_unary(TT_DEFER,           (yyvsp[0].expr)); }
#line 1856 "snobol4.tab.c"
    break;

  case 84: /* expr14: T_1DOLLAR expr14  */
#line 172 "snobol4.y"
                                                                                           { (yyval.expr)=expr_unary(TT_INDIRECT,        (yyvsp[0].expr)); }
#line 1862 "snobol4.tab.c"
    break;

  case 85: /* expr14: T_1DOT expr14  */
#line 173 "snobol4.y"
                                                                                             { (yyval.expr)=expr_unary(TT_NAME,            (yyvsp[0].expr)); }
#line 1868 "snobol4.tab.c"
    break;

  case 86: /* expr14: T_1BANG expr14  */
#line 174 "snobol4.y"
                                                                                         { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("!"); (yyval.expr)=_e; }
#line 1874 "snobol4.tab.c"
    break;

  case 87: /* expr14: T_1PERCENT expr14  */
#line 175 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("%"); (yyval.expr)=_e; }
#line 1880 "snobol4.tab.c"
    break;

  case 88: /* expr14: T_1SLASH expr14  */
#line 176 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("/"); (yyval.expr)=_e; }
#line 1886 "snobol4.tab.c"
    break;

  case 89: /* expr14: T_1POUND expr14  */
#line 177 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("#"); (yyval.expr)=_e; }
#line 1892 "snobol4.tab.c"
    break;

  case 90: /* expr14: T_1EQUAL expr14  */
#line 178 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("="); (yyval.expr)=_e; }
#line 1898 "snobol4.tab.c"
    break;

  case 91: /* expr14: T_1PIPE expr14  */
#line 179 "snobol4.y"
                                                                                        { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("|"); (yyval.expr)=_e; }
#line 1904 "snobol4.tab.c"
    break;

  case 92: /* expr14: T_1CARET expr14  */
#line 180 "snobol4.y"
                                                                                                { tree_t*_e=expr_unary(TT_OPSYN,(yyvsp[0].expr)); _e->v.sval=strdup("^"); (yyval.expr)=_e; }
#line 1910 "snobol4.tab.c"
    break;

  case 93: /* expr14: expr15  */
#line 181 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1916 "snobol4.tab.c"
    break;

  case 94: /* $@2: %empty  */
#line 183 "snobol4.y"
                             { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1922 "snobol4.tab.c"
    break;

  case 95: /* expr15: expr15 T_LBRACK $@2 idx_args T_RBRACK  */
#line 183 "snobol4.y"
                                                                              { int _n=tal_count(); tree_t*_i=ast_node_new(TT_IDX); for(int _j=0;_j<_n;_j++) expr_add_child(_i,tal_child(_j)); tal_close(); (yyval.expr)=_i; }
#line 1928 "snobol4.tab.c"
    break;

  case 96: /* $@3: %empty  */
#line 184 "snobol4.y"
                             { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1934 "snobol4.tab.c"
    break;

  case 97: /* expr15: expr15 T_LANGLE $@3 idx_args T_RANGLE  */
#line 184 "snobol4.y"
                                                                              { int _n=tal_count(); tree_t*_i=ast_node_new(TT_IDX); for(int _j=0;_j<_n;_j++) expr_add_child(_i,tal_child(_j)); tal_close(); (yyval.expr)=_i; }
#line 1940 "snobol4.tab.c"
    break;

  case 98: /* expr15: expr17  */
#line 185 "snobol4.y"
                                                                                                   { (yyval.expr)=(yyvsp[0].expr); }
#line 1946 "snobol4.tab.c"
    break;

  case 99: /* idx_args: idx_args T_COMMA expr0  */
#line 187 "snobol4.y"
                                                                                                  { tal_push((yyvsp[0].expr)); }
#line 1952 "snobol4.tab.c"
    break;

  case 100: /* idx_args: idx_args T_COMMA  */
#line 188 "snobol4.y"
                                                                                                  { tal_push(ast_node_new(TT_NUL)); }
#line 1958 "snobol4.tab.c"
    break;

  case 101: /* idx_args: expr0  */
#line 189 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 1964 "snobol4.tab.c"
    break;

  case 103: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 192 "snobol4.y"
                                                                                                { (yyval.expr)=(yyvsp[-1].expr); }
#line 1970 "snobol4.tab.c"
    break;

  case 104: /* $@4: %empty  */
#line 193 "snobol4.y"
                                    { tal_open(); tal_push((yyvsp[-1].expr)); }
#line 1976 "snobol4.tab.c"
    break;

  case 105: /* expr17: T_LPAREN expr0 T_COMMA $@4 vlist_args T_RPAREN  */
#line 193 "snobol4.y"
                                                                                      { int _n=tal_count(); tree_t*_a=ast_node_new(TT_VLIST); for(int _j=0;_j<_n;_j++) expr_add_child(_a,tal_child(_j)); tal_close(); (yyval.expr)=_a; }
#line 1982 "snobol4.tab.c"
    break;

  case 106: /* expr17: T_LPAREN T_RPAREN  */
#line 194 "snobol4.y"
                                                                                                { (yyval.expr)=ast_node_new(TT_NUL); }
#line 1988 "snobol4.tab.c"
    break;

  case 107: /* $@5: %empty  */
#line 195 "snobol4.y"
                                 { tree_e _k=pat_prim_kind((yyvsp[-1].tok).sval); tal_open(); tal_fnc_open(_k,(char*)(yyvsp[-1].tok).sval); }
#line 1994 "snobol4.tab.c"
    break;

  case 108: /* expr17: T_FUNCTION T_LPAREN $@5 fnc_args T_RPAREN  */
#line 195 "snobol4.y"
                                                                                                                                      { (yyval.expr)=tal_fnc_close(); }
#line 2000 "snobol4.tab.c"
    break;

  case 109: /* expr17: T_IDENT  */
#line 196 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_VAR);e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2006 "snobol4.tab.c"
    break;

  case 110: /* expr17: T_END  */
#line 197 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_VAR);    e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2012 "snobol4.tab.c"
    break;

  case 111: /* expr17: T_KEYWORD  */
#line 198 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_KEYWORD);e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2018 "snobol4.tab.c"
    break;

  case 112: /* expr17: T_STR  */
#line 199 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_QLIT);   e->v.sval=(char*)(yyvsp[0].tok).sval;(yyval.expr)=e; }
#line 2024 "snobol4.tab.c"
    break;

  case 113: /* expr17: T_INT  */
#line 200 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_ILIT);   e->v.ival=(yyvsp[0].tok).ival;(yyval.expr)=e; }
#line 2030 "snobol4.tab.c"
    break;

  case 114: /* expr17: T_REAL  */
#line 201 "snobol4.y"
                                                                                                  { tree_t*e=ast_node_new(TT_FLIT);   e->v.dval=(yyvsp[0].tok).dval;(yyval.expr)=e; }
#line 2036 "snobol4.tab.c"
    break;

  case 115: /* vlist_args: vlist_args T_COMMA expr0  */
#line 203 "snobol4.y"
                                                                                                 { tal_push((yyvsp[0].expr)); }
#line 2042 "snobol4.tab.c"
    break;

  case 116: /* vlist_args: expr0  */
#line 204 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 2048 "snobol4.tab.c"
    break;

  case 117: /* fnc_args: fnc_args T_COMMA expr0  */
#line 206 "snobol4.y"
                                                                                                 { if (tal_count()==0) tal_push(ast_node_new(TT_NUL)); tal_push((yyvsp[0].expr)); }
#line 2054 "snobol4.tab.c"
    break;

  case 118: /* fnc_args: fnc_args T_COMMA  */
#line 207 "snobol4.y"
                                                                                                  { if (tal_count()==0) tal_push(ast_node_new(TT_NUL)); tal_push(ast_node_new(TT_NUL)); }
#line 2060 "snobol4.tab.c"
    break;

  case 119: /* fnc_args: expr0  */
#line 208 "snobol4.y"
                                                                                                   { tal_push((yyvsp[0].expr)); }
#line 2066 "snobol4.tab.c"
    break;

  case 121: /* goto_atom: T_STR  */
#line 211 "snobol4.y"
                      { tree_t*e=ast_node_new(TT_QLIT); e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2072 "snobol4.tab.c"
    break;

  case 122: /* goto_atom: T_INT  */
#line 212 "snobol4.y"
                      { tree_t*e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].tok).ival; (yyval.expr)=e; }
#line 2078 "snobol4.tab.c"
    break;

  case 123: /* goto_atom: T_IDENT  */
#line 213 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2084 "snobol4.tab.c"
    break;

  case 124: /* goto_atom: T_FUNCTION  */
#line 214 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2090 "snobol4.tab.c"
    break;

  case 125: /* goto_atom: T_END  */
#line 215 "snobol4.y"
                       { tree_t*e=ast_node_new(TT_VAR);  e->v.sval=(char*)(yyvsp[0].tok).sval; (yyval.expr)=e; }
#line 2096 "snobol4.tab.c"
    break;

  case 126: /* goto_fnc_args: goto_fnc_args T_COMMA goto_expr  */
#line 217 "snobol4.y"
                                                                                                { tal_push((yyvsp[0].expr)); }
#line 2102 "snobol4.tab.c"
    break;

  case 127: /* goto_fnc_args: goto_fnc_args T_COMMA  */
#line 218 "snobol4.y"
                                                                                                { tal_push(ast_node_new(TT_NUL)); }
#line 2108 "snobol4.tab.c"
    break;

  case 128: /* goto_fnc_args: goto_expr  */
#line 219 "snobol4.y"
                                                                                                { tal_push((yyvsp[0].expr)); }
#line 2114 "snobol4.tab.c"
    break;

  case 129: /* goto_fnc_args: %empty  */
#line 220 "snobol4.y"
                                                                                                { }
#line 2120 "snobol4.tab.c"
    break;

  case 130: /* goto_expr: goto_atom  */
#line 222 "snobol4.y"
                                                                                                  { (yyval.expr)=(yyvsp[0].expr); }
#line 2126 "snobol4.tab.c"
    break;

  case 131: /* goto_expr: goto_expr T_CONCAT goto_atom  */
#line 223 "snobol4.y"
                                                                                                  { tree_t*s=ast_node_new(TT_SEQ);expr_add_child(s,(yyvsp[-2].expr));expr_add_child(s,(yyvsp[0].expr));(yyval.expr)=s; }
#line 2132 "snobol4.tab.c"
    break;


#line 2136 "snobol4.tab.c"

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

#line 225 "snobol4.y"

int snobol4_lex(YYSTYPE *yylval_param, void *yyparse_param) {
    (void)yyparse_param; Token t=lex_next(g_lx); yylval_param->tok=t; if (t.lineno) g_err_lineno=t.lineno;
    if (getenv("SNO_TOK_TRACE"))
        fprintf(stderr,"[TOK %d sval=%s ival=%ld]\n",t.kind,t.sval?t.sval:"",t.ival);
    return t.kind;
}
void snobol4_error(void *p,const char *msg){(void)p;sno_error(g_err_lineno,"parse error: %s",msg);}
static void sno4_stmt_commit_go(void *param,Token lbl,tree_t *subj,tree_t *pat,int has_eq,tree_t *repl,tree_t *gu,tree_t *gs,tree_t *gf){
    PP *pp=(PP*)param;
    STMT_t *s=stmt_new();
    s->lineno = lbl.lineno ? lbl.lineno : snobol4_get_stmt_lineno();
    { extern int snobol4_get_nofail_mode(void); s->nofail = snobol4_get_nofail_mode(); }
    s->stno = ++pp->prog->nstmts;
    if(lbl.sval){s->label=strdup(lbl.sval);s->is_end=lbl.ival||(strcmp(lbl.sval,"END")==0);
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
    CODE_t *prog=calloc(1,sizeof*prog);PP p={prog,NULL,NULL};g_lx=lx;snobol4_parse(&p);
    return prog->head?prog->head->subject:NULL;
}
CODE_t *parse_program_tokens(Lex *stream){
    CODE_t *prog=calloc(1,sizeof*prog);PP p={prog,NULL,NULL};g_lx=stream;snobol4_parse(&p);return prog;
}
CODE_t *parse_program_tokens_ast(Lex *stream, tree_t **ast_out){
    CODE_t *prog=calloc(1,sizeof*prog);
    tree_t *ast=calloc(1,sizeof*ast); ast->t=TT_PROGRAM;
    PP p={prog,NULL,ast};g_lx=stream;snobol4_parse(&p);
    *ast_out=ast;
    return prog;
}
CODE_t *parse_program(LineArray *lines){(void)lines;return calloc(1,sizeof(CODE_t));}
tree_t *parse_expr_from_str(const char *src){
    if(!src||!*src) return NULL;Lex lx={0};lex_open_str(&lx,src,(int)strlen(src),0);return parse_expr(&lx);
}
tree_t *parse_expr_pat_from_str(const char *src) {
    if (!src || !*src) return NULL;
    int slen = (int)strlen(src);
    char *buf = malloc(slen + 2);
    if (!buf) return NULL;
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str(&lx, buf, slen + 1, 0);
    CODE_t *prog = calloc(1, sizeof(CODE_t));
    PP p = {prog, NULL, NULL};
    g_lx = &lx;
    snobol4_parse(&p);
    free(buf);
    if (p.ast_prog && p.ast_prog->n > 0) {
        const tree_t *s = p.ast_prog->c[0];
        if (s) {
            tree_t *pat = stmt_attr_expr(stmt_attr_find(s, ":pat"));
            if (pat) { free(prog); return pat; }
            return stmt_attr_expr(stmt_attr_find(s, ":subj"));
        }
    }
    if (!prog->head) { free(prog); return NULL; }
    STMT_t *s = prog->head;
    tree_t *res = s->pattern ? s->pattern : s->subject;
    free(prog);
    return res;
}
CODE_t *sno_parse_string(const char *src) {
    if (!src) return calloc(1, sizeof(CODE_t));
    int slen = (int)strlen(src);
    char *buf = malloc(slen + 2);
    if (!buf) return calloc(1, sizeof(CODE_t));
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str_initial(&lx, buf, slen + 1, 0);
    CODE_t *prog = calloc(1, sizeof(CODE_t));
    PP p = {prog, NULL, NULL};
    g_lx = &lx;
    snobol4_parse(&p);
    free(buf);
    return prog;
}
tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out) {
    if (!src) { if (code_out) *code_out = calloc(1, sizeof(CODE_t)); return NULL; }
    int slen = (int)strlen(src);
    char *buf = malloc(slen + 2);
    if (!buf) { if (code_out) *code_out = calloc(1, sizeof(CODE_t)); return NULL; }
    memcpy(buf, src, slen);
    buf[slen]   = '\n';
    buf[slen+1] = '\0';
    Lex lx = {0};
    lex_open_str_initial(&lx, buf, slen + 1, 0);
    CODE_t *prog = calloc(1, sizeof(CODE_t));
    tree_t  *ast  = calloc(1, sizeof(tree_t)); ast->t = TT_PROGRAM;
    PP p = {prog, NULL, ast};
    g_lx = &lx;
    snobol4_parse(&p);
    ast = p.ast_prog;
    free(buf);
    if (code_out) *code_out = prog; else free(prog);
    return (ast && ast->n > 0) ? ast : NULL;
}
