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

/* "%code top" blocks.  */
#line 4 "snocone_parse.y"


#line 71 "snocone_parse.tab.c"
/* Substitute the type names.  */
#define YYSTYPE         SC_STYPE
/* Substitute the variable and function names.  */
#define yyparse         sc_parse
#define yylex           sc_lex
#define yyerror         sc_error
#define yydebug         sc_debug
#define yynerrs         sc_nerrs

/* First part of user prologue.  */
#line 1 "snocone_parse.y"

#include "ct_arena.h"

#line 86 "snocone_parse.tab.c"

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

#include "snocone_parse.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_IDENT = 3,                    /* T_IDENT  */
  YYSYMBOL_T_KEYWORD = 4,                  /* T_KEYWORD  */
  YYSYMBOL_T_INT = 5,                      /* T_INT  */
  YYSYMBOL_T_REAL = 6,                     /* T_REAL  */
  YYSYMBOL_T_STR = 7,                      /* T_STR  */
  YYSYMBOL_T_CALL = 8,                     /* T_CALL  */
  YYSYMBOL_T_2PLUS = 9,                    /* T_2PLUS  */
  YYSYMBOL_T_2MINUS = 10,                  /* T_2MINUS  */
  YYSYMBOL_T_2STAR = 11,                   /* T_2STAR  */
  YYSYMBOL_T_2SLASH = 12,                  /* T_2SLASH  */
  YYSYMBOL_T_2CARET = 13,                  /* T_2CARET  */
  YYSYMBOL_T_1PLUS = 14,                   /* T_1PLUS  */
  YYSYMBOL_T_1MINUS = 15,                  /* T_1MINUS  */
  YYSYMBOL_T_2EQUAL = 16,                  /* T_2EQUAL  */
  YYSYMBOL_T_PLUS_ASSIGN = 17,             /* T_PLUS_ASSIGN  */
  YYSYMBOL_T_MINUS_ASSIGN = 18,            /* T_MINUS_ASSIGN  */
  YYSYMBOL_T_STAR_ASSIGN = 19,             /* T_STAR_ASSIGN  */
  YYSYMBOL_T_SLASH_ASSIGN = 20,            /* T_SLASH_ASSIGN  */
  YYSYMBOL_T_CARET_ASSIGN = 21,            /* T_CARET_ASSIGN  */
  YYSYMBOL_T_2QUEST = 22,                  /* T_2QUEST  */
  YYSYMBOL_T_2PIPE = 23,                   /* T_2PIPE  */
  YYSYMBOL_T_CONCAT = 24,                  /* T_CONCAT  */
  YYSYMBOL_T_LPAREN = 25,                  /* T_LPAREN  */
  YYSYMBOL_T_RPAREN = 26,                  /* T_RPAREN  */
  YYSYMBOL_T_SEMICOLON = 27,               /* T_SEMICOLON  */
  YYSYMBOL_T_COMMA = 28,                   /* T_COMMA  */
  YYSYMBOL_T_LBRACK = 29,                  /* T_LBRACK  */
  YYSYMBOL_T_RBRACK = 30,                  /* T_RBRACK  */
  YYSYMBOL_T_2DOLLAR = 31,                 /* T_2DOLLAR  */
  YYSYMBOL_T_2DOT = 32,                    /* T_2DOT  */
  YYSYMBOL_T_2AMP = 33,                    /* T_2AMP  */
  YYSYMBOL_T_2AT = 34,                     /* T_2AT  */
  YYSYMBOL_T_2POUND = 35,                  /* T_2POUND  */
  YYSYMBOL_T_2PERCENT = 36,                /* T_2PERCENT  */
  YYSYMBOL_T_2TILDE = 37,                  /* T_2TILDE  */
  YYSYMBOL_T_1STAR = 38,                   /* T_1STAR  */
  YYSYMBOL_T_1SLASH = 39,                  /* T_1SLASH  */
  YYSYMBOL_T_1PERCENT = 40,                /* T_1PERCENT  */
  YYSYMBOL_T_1AT = 41,                     /* T_1AT  */
  YYSYMBOL_T_1TILDE = 42,                  /* T_1TILDE  */
  YYSYMBOL_T_1DOLLAR = 43,                 /* T_1DOLLAR  */
  YYSYMBOL_T_1DOT = 44,                    /* T_1DOT  */
  YYSYMBOL_T_1POUND = 45,                  /* T_1POUND  */
  YYSYMBOL_T_1PIPE = 46,                   /* T_1PIPE  */
  YYSYMBOL_T_1EQUAL = 47,                  /* T_1EQUAL  */
  YYSYMBOL_T_1QUEST = 48,                  /* T_1QUEST  */
  YYSYMBOL_T_1AMP = 49,                    /* T_1AMP  */
  YYSYMBOL_T_1BANG = 50,                   /* T_1BANG  */
  YYSYMBOL_T_COLON = 51,                   /* T_COLON  */
  YYSYMBOL_T_DO = 52,                      /* T_DO  */
  YYSYMBOL_T_FOR = 53,                     /* T_FOR  */
  YYSYMBOL_T_SWITCH = 54,                  /* T_SWITCH  */
  YYSYMBOL_T_CASE = 55,                    /* T_CASE  */
  YYSYMBOL_T_DEFAULT = 56,                 /* T_DEFAULT  */
  YYSYMBOL_T_BREAK = 57,                   /* T_BREAK  */
  YYSYMBOL_T_CONTINUE = 58,                /* T_CONTINUE  */
  YYSYMBOL_T_GOTO = 59,                    /* T_GOTO  */
  YYSYMBOL_T_DEFINE = 60,                  /* T_DEFINE  */
  YYSYMBOL_T_RETURN = 61,                  /* T_RETURN  */
  YYSYMBOL_T_FRETURN = 62,                 /* T_FRETURN  */
  YYSYMBOL_T_NRETURN = 63,                 /* T_NRETURN  */
  YYSYMBOL_T_STRUCT = 64,                  /* T_STRUCT  */
  YYSYMBOL_T_UNKNOWN = 65,                 /* T_UNKNOWN  */
  YYSYMBOL_T_LBRACE = 66,                  /* T_LBRACE  */
  YYSYMBOL_T_RBRACE = 67,                  /* T_RBRACE  */
  YYSYMBOL_T_IF = 68,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 69,                    /* T_ELSE  */
  YYSYMBOL_T_WHILE = 70,                   /* T_WHILE  */
  YYSYMBOL_YYACCEPT = 71,                  /* $accept  */
  YYSYMBOL_program = 72,                   /* program  */
  YYSYMBOL_stmt_list = 73,                 /* stmt_list  */
  YYSYMBOL_stmt = 74,                      /* stmt  */
  YYSYMBOL_matched_stmt = 75,              /* matched_stmt  */
  YYSYMBOL_unmatched_stmt = 76,            /* unmatched_stmt  */
  YYSYMBOL_if_head = 77,                   /* if_head  */
  YYSYMBOL_while_head = 78,                /* while_head  */
  YYSYMBOL_do_head = 79,                   /* do_head  */
  YYSYMBOL_do_body = 80,                   /* do_body  */
  YYSYMBOL_for_lead = 81,                  /* for_lead  */
  YYSYMBOL_for_head = 82,                  /* for_head  */
  YYSYMBOL_switch_head = 83,               /* switch_head  */
  YYSYMBOL_switch_body = 84,               /* switch_body  */
  YYSYMBOL_case_clause = 85,               /* case_clause  */
  YYSYMBOL_case_or_default_label = 86,     /* case_or_default_label  */
  YYSYMBOL_opt_head_sep = 87,              /* opt_head_sep  */
  YYSYMBOL_func_head = 88,                 /* func_head  */
  YYSYMBOL_func_locals = 89,               /* func_locals  */
  YYSYMBOL_func_locals_ne = 90,            /* func_locals_ne  */
  YYSYMBOL_func_arglist = 91,              /* func_arglist  */
  YYSYMBOL_func_arglist_ne = 92,           /* func_arglist_ne  */
  YYSYMBOL_struct_field_list = 93,         /* struct_field_list  */
  YYSYMBOL_else_keyword = 94,              /* else_keyword  */
  YYSYMBOL_label_decl = 95,                /* label_decl  */
  YYSYMBOL_simple_stmt = 96,               /* simple_stmt  */
  YYSYMBOL_block_stmt = 97,                /* block_stmt  */
  YYSYMBOL_expr0 = 98,                     /* expr0  */
  YYSYMBOL_expr1 = 99,                     /* expr1  */
  YYSYMBOL_expr3 = 100,                    /* expr3  */
  YYSYMBOL_expr4 = 101,                    /* expr4  */
  YYSYMBOL_expr5 = 102,                    /* expr5  */
  YYSYMBOL_expr6 = 103,                    /* expr6  */
  YYSYMBOL_expr9 = 104,                    /* expr9  */
  YYSYMBOL_expr11 = 105,                   /* expr11  */
  YYSYMBOL_expr12 = 106,                   /* expr12  */
  YYSYMBOL_expr14 = 107,                   /* expr14  */
  YYSYMBOL_expr15 = 108,                   /* expr15  */
  YYSYMBOL_exprlist = 109,                 /* exprlist  */
  YYSYMBOL_exprlist_c = 110,               /* exprlist_c  */
  YYSYMBOL_optexpr = 111,                  /* optexpr  */
  YYSYMBOL_expr17 = 112                    /* expr17  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 32 "snocone_parse.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snocone_lex.h"
#include "../icon/icon_lex.h"
#define expr_new(k)            ast_node_new(k)
#define expr_add_child(p,c)    ast_push((p),(c))
#define TT_ASSIGN  TT_ASSIGN
#define TT_ADD     TT_ADD
#define TT_SUB     TT_SUB
#define TT_MUL     TT_MUL
#define TT_DIV     TT_DIV
#define TT_POW     TT_POW
#define TT_SEQ     TT_SEQ
#define TT_ALT     TT_ALT
#define TT_SCAN    TT_SCAN
#define TT_FNC     TT_FNC
#define TT_VAR     TT_VAR
#define TT_KEYWORD TT_KEYWORD
#define TT_QLIT    TT_QLIT
#define TT_ILIT    TT_ILIT
#define TT_FLIT    TT_FLIT
#define TT_NUL     TT_NUL
#define TT_VLIST   TT_VLIST
#define TT_IDX     TT_IDX
#define TT_INDIRECT   TT_INDIRECT
#define TT_DEFER      TT_DEFER
#define TT_NAME       TT_NAME
#define TT_CAPT_CURSOR      TT_CAPT_CURSOR
#define TT_CAPT_IMMED_ASGN  TT_CAPT_IMMED_ASGN
#define TT_CAPT_COND_ASGN   TT_CAPT_COND_ASGN
#define TT_PLS        TT_PLS
#define TT_MNS        TT_MNS
#define TT_NOT        TT_NOT
#define TT_INTERROGATE TT_INTERROGATE
#define TT_OPSYN      TT_OPSYN
#define kind       t
#define nchildren  n
#define children   c
#define sval       v.sval
#define ival       v.ival
#define dval       v.dval
int  sc_lex  (SC_STYPE *yylval, ScParseState *st);
void sc_error(ScParseState *st, const char *msg);
static void     sc_append_stmt        (ScParseState *st, tree_t *top);
static tree_t  *sc_collect_body       (ScParseState *st, int mark);
static void     sc_finalize_if_no_else_pst(ScParseState *st, struct IfHead *h);
static void     sc_finalize_if_else_pst(ScParseState *st, struct IfHead *h, int before_else);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t  *sc_int_literal        (const char *txt);
static tree_t  *sc_real_literal       (const char *txt);
static tree_t  *sc_str_literal        (const char *txt);
struct IfHead {
    tree_t *cond;
    int     mark;
    int     lineno;
};
struct WhileHead {
    tree_t *cond;
    int     mark;
};
struct DoHead {
    int     mark;
};
struct ForHead {
    tree_t *init;
    tree_t *cond;
    tree_t *step;
    int     mark;
};
struct FuncHead {
    char   *name;
    tree_t *params;
    tree_t *locals;
    char   *prev_func;
    int     mark;
};
struct CaseEntry {
    char   *case_label;
    tree_t *value;
    int     mark;
};
struct SwitchHead {
    tree_t *disc;
    char   *tmp_name;
    char   *end_label;
    char   *default_label;
    int     has_default;
    struct CaseEntry *cases;
    int     cases_count;
    int     cases_cap;
    struct SwitchHead *prev_switch;
    int     lineno;
};
static struct IfHead    *sc_if_head_new    (ScParseState *st, tree_t *cond);
static void     sc_finalize_while_pst  (ScParseState *st, struct WhileHead *h, tree_t *cond);
static void     sc_finalize_do_while_pst(ScParseState *st, struct DoHead *h, tree_t *cond);
static struct ForHead   *sc_for_head_new_pst(ScParseState *st, tree_t *init, tree_t *cond, tree_t *step, int mark);
static void     sc_finalize_for_pst   (ScParseState *st, struct ForHead *h);
static struct FuncHead *sc_func_head_new_pst(ScParseState *st, char *name, tree_t *params, tree_t *locals);
static void     sc_finalize_function_pst(ScParseState *st, struct FuncHead *h);
static void     sc_append_label_node  (ScParseState *st, const char *name);
static void     sc_loop_push           (ScParseState *st, char *cont_label, char *end_label, int is_loop);
static void     sc_loop_pop            (ScParseState *st);
static void     sc_append_break        (ScParseState *st, char *user_label);
static void     sc_append_continue     (ScParseState *st, char *user_label);
static struct SwitchHead *sc_switch_head_new(ScParseState *st, tree_t *disc);
static void     sc_switch_case_label   (ScParseState *st, tree_t *value);
static void     sc_switch_default_label(ScParseState *st);
static void     sc_finalize_switch_pst (ScParseState *st, struct SwitchHead *h);
static void     sc_emit_struct         (ScParseState *st, char *name, tree_t *fields);
static tree_t  *sc_qlit                (const char *txt);
static tree_t  *sc_list_one            (tree_e k, char *ident);
static tree_t  *sc_list_add            (tree_t *l, char *ident);

#line 351 "snocone_parse.tab.c"

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
         || (defined SC_STYPE_IS_TRIVIAL && SC_STYPE_IS_TRIVIAL)))

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
#define YYFINAL  105
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   826

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  235

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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70
};

#if SC_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   213,   213,   214,   216,   217,   219,   220,   223,   224,
     225,   227,   229,   231,   233,   235,   237,   239,   241,   243,
     245,   248,   250,   252,   254,   257,   260,   267,   272,   273,
     275,   277,   281,   284,   285,   287,   288,   291,   292,   295,
     296,   298,   302,   303,   306,   307,   311,   312,   313,   316,
     318,   322,   324,   328,   331,   333,   334,   335,   337,   338,
     339,   340,   341,   342,   343,   344,   346,   347,   349,   351,
     355,   358,   361,   364,   367,   370,   373,   375,   378,   380,
     383,   385,   388,   391,   393,   395,   398,   400,   402,   405,
     407,   410,   412,   414,   417,   419,   421,   422,   423,   424,
     425,   426,   427,   429,   431,   433,   435,   437,   439,   441,
     444,   453,   456,   458,   461,   463,   465,   468,   471,   473,
     482,   486,   490,   492,   494,   496,   498,   504
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if SC_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_IDENT", "T_KEYWORD",
  "T_INT", "T_REAL", "T_STR", "T_CALL", "T_2PLUS", "T_2MINUS", "T_2STAR",
  "T_2SLASH", "T_2CARET", "T_1PLUS", "T_1MINUS", "T_2EQUAL",
  "T_PLUS_ASSIGN", "T_MINUS_ASSIGN", "T_STAR_ASSIGN", "T_SLASH_ASSIGN",
  "T_CARET_ASSIGN", "T_2QUEST", "T_2PIPE", "T_CONCAT", "T_LPAREN",
  "T_RPAREN", "T_SEMICOLON", "T_COMMA", "T_LBRACK", "T_RBRACK",
  "T_2DOLLAR", "T_2DOT", "T_2AMP", "T_2AT", "T_2POUND", "T_2PERCENT",
  "T_2TILDE", "T_1STAR", "T_1SLASH", "T_1PERCENT", "T_1AT", "T_1TILDE",
  "T_1DOLLAR", "T_1DOT", "T_1POUND", "T_1PIPE", "T_1EQUAL", "T_1QUEST",
  "T_1AMP", "T_1BANG", "T_COLON", "T_DO", "T_FOR", "T_SWITCH", "T_CASE",
  "T_DEFAULT", "T_BREAK", "T_CONTINUE", "T_GOTO", "T_DEFINE", "T_RETURN",
  "T_FRETURN", "T_NRETURN", "T_STRUCT", "T_UNKNOWN", "T_LBRACE",
  "T_RBRACE", "T_IF", "T_ELSE", "T_WHILE", "$accept", "program",
  "stmt_list", "stmt", "matched_stmt", "unmatched_stmt", "if_head",
  "while_head", "do_head", "do_body", "for_lead", "for_head",
  "switch_head", "switch_body", "case_clause", "case_or_default_label",
  "opt_head_sep", "func_head", "func_locals", "func_locals_ne",
  "func_arglist", "func_arglist_ne", "struct_field_list", "else_keyword",
  "label_decl", "simple_stmt", "block_stmt", "expr0", "expr1", "expr3",
  "expr4", "expr5", "expr6", "expr9", "expr11", "expr12", "expr14",
  "expr15", "exprlist", "exprlist_c", "optexpr", "expr17", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-163)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-119)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     564,   -22,  -163,  -163,  -163,  -163,   632,   776,   776,   680,
    -163,   776,   776,   776,   776,   776,   776,   776,   776,   776,
     776,   776,   776,   776,  -163,  -163,    38,    21,    25,    47,
      64,   728,    42,    48,    71,   156,    51,    52,    79,   564,
    -163,  -163,  -163,   564,   564,    14,    56,   564,    16,    26,
    -163,  -163,  -163,    66,    69,    72,    70,  -163,    27,    54,
    -163,    22,  -163,    67,  -163,  -163,  -163,    73,    74,    75,
      76,  -163,  -163,  -163,    80,    31,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,   776,
      78,  -163,    81,  -163,    83,    77,  -163,    84,  -163,  -163,
      33,  -163,   224,   776,   776,  -163,  -163,  -163,    43,  -163,
    -163,   292,    49,   776,  -163,  -163,   -16,   360,  -163,   776,
     776,   776,   776,   776,   776,   776,   776,   776,   776,   776,
     776,   776,   776,   776,   776,   632,  -163,   776,   776,  -163,
    -163,    95,  -163,  -163,  -163,    30,  -163,     3,  -163,    96,
      97,  -163,   564,  -163,   428,    99,    98,   776,    85,  -163,
     -12,   564,  -163,  -163,   496,  -163,  -163,  -163,  -163,  -163,
    -163,    72,    70,  -163,    54,    54,  -163,  -163,  -163,  -163,
    -163,   107,  -163,  -163,  -163,  -163,    32,  -163,   102,    36,
    -163,  -163,   -18,   102,   102,  -163,  -163,  -163,   776,   776,
      87,  -163,  -163,   564,  -163,  -163,  -163,  -163,   125,  -163,
     136,  -163,  -163,   137,   138,  -163,  -163,  -163,   116,   117,
    -163,  -163,  -163,    10,  -163,  -163,   118,   776,   140,  -163,
    -163,   120,  -163,   102,  -163
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,   120,   121,   122,   123,   124,   114,     0,     0,   118,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       5,     6,     7,     0,     0,     0,     0,     0,     0,     0,
      20,     8,     9,     0,    75,    77,    79,    81,    82,    85,
      88,    90,    93,   109,   111,    54,   120,   113,     0,   112,
       0,    94,    95,   127,   117,     0,    96,   104,   103,    99,
     100,    98,    97,   105,   106,   107,   101,   102,   108,     0,
       0,    62,     0,    64,     0,     0,    58,     0,    59,    60,
       0,    67,     0,     0,     0,     1,     4,    21,     6,    11,
      23,     0,     0,     0,    13,    24,     0,     0,    55,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   119,   118,   118,   125,
     126,     0,    63,    65,    61,     0,    57,     0,    66,     0,
       0,    53,     0,    29,     0,     0,     0,     0,     0,    17,
       0,    33,    35,    15,     0,    68,    70,    71,    72,    73,
      74,    76,    78,    80,    83,    84,    86,    87,    89,    91,
      92,     0,   117,   115,   116,    32,     0,    46,    39,     0,
      51,    19,     0,    39,    39,    10,    22,    28,     0,     0,
       0,    38,    16,    34,    36,    14,   110,    47,     0,    40,
      42,    41,    48,     0,     0,    18,    25,    26,     0,     0,
      37,    49,    44,    39,    50,    52,     0,     0,     0,    43,
      12,     0,    45,    39,    31
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -163,  -163,   -33,   -34,   -17,   -43,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,   -13,  -163,  -162,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,    -6,  -163,    23,
      24,    28,  -163,   -87,   -85,  -163,     0,  -163,    17,   144,
     -65,  -163
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    38,    39,    40,    41,    42,    43,    44,    45,   112,
      46,    47,    48,   160,   161,   162,   210,    49,   211,   223,
     188,   189,   192,   152,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    68,    69,
      70,    64
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      67,   110,   102,    74,   115,   106,   190,    71,    72,   107,
     214,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    90,    97,   108,   109,    92,    65,
     114,   216,   217,   186,   209,   132,   128,   129,   228,   157,
     158,   174,   175,   157,   158,   176,   177,   178,    91,   215,
      94,   159,    93,   133,   134,   202,   187,   140,   207,   137,
     208,   229,   212,    89,   213,   130,   131,    95,   106,    98,
     191,   234,   183,   184,   100,    99,   103,   104,   154,   105,
     111,   113,   116,   141,   164,   119,   120,   121,   122,   123,
     124,   125,   117,   118,   127,   126,   135,   149,   150,   147,
     136,  -117,   145,   137,   138,   142,   139,   156,   143,   196,
     144,   146,   151,   165,   166,   167,   168,   169,   170,   155,
     106,   185,   193,   194,   198,   199,   209,   204,   221,    67,
     106,   182,   182,   179,   180,   195,   201,   206,   220,   222,
     224,   225,   226,   232,   227,   230,   233,   203,   171,     0,
     172,   200,   181,    75,     0,   173,     0,     0,     0,     1,
       2,     3,     4,     5,     6,     0,     0,     0,     0,   204,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,   218,   219,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,   231,    35,   101,    36,     0,    37,     1,     2,     3,
       4,     5,     6,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,    24,    25,    26,     0,
       0,    27,    28,    29,    30,    31,    32,    33,    34,     0,
      35,   148,    36,     0,    37,     1,     2,     3,     4,     5,
       6,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,     0,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,    24,    25,    26,     0,     0,    27,
      28,    29,    30,    31,    32,    33,    34,     0,    35,   153,
      36,     0,    37,     1,     2,     3,     4,     5,     6,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,     0,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,    24,    25,    26,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,     0,    35,   163,    36,     0,
      37,     1,     2,     3,     4,     5,     6,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
      24,    25,    26,     0,     0,    27,    28,    29,    30,    31,
      32,    33,    34,     0,    35,   197,    36,     0,    37,     1,
       2,     3,     4,     5,     6,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,     0,    35,   205,    36,     0,    37,     1,     2,     3,
       4,     5,     6,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,    24,    25,    26,     0,
       0,    27,    28,    29,    30,    31,    32,    33,    34,     0,
      35,     0,    36,     0,    37,    66,     2,     3,     4,     5,
       6,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
    -118,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    66,     2,     3,     4,     5,     6,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    66,     2,     3,     4,     5,     6,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    66,
       2,     3,     4,     5,     6,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23
};

static const yytype_int16 yycheck[] =
{
       6,    44,    35,     9,    47,    39,     3,     7,     8,    43,
      28,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     3,    31,    43,    44,     3,    51,
      47,   193,   194,     3,    24,    13,     9,    10,    28,    55,
      56,   128,   129,    55,    56,   130,   131,   132,    27,    67,
       3,    67,    27,    31,    32,    67,    26,    26,    26,    28,
      28,   223,    26,    25,    28,    11,    12,     3,   102,    27,
      67,   233,   137,   138,     3,    27,    25,    25,   111,     0,
      66,    25,    66,    89,   117,    16,    17,    18,    19,    20,
      21,    22,    66,    27,    24,    23,    29,   103,   104,    66,
      26,    28,    25,    28,    28,    27,    26,   113,    27,   152,
      27,    27,    69,   119,   120,   121,   122,   123,   124,    70,
     154,    26,    26,    26,    25,    27,    24,   161,     3,   135,
     164,   137,   138,   133,   134,   152,    51,    30,    51,     3,
       3,     3,    26,     3,    27,    27,    26,   160,   125,    -1,
     126,   157,   135,     9,    -1,   127,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,   203,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,   199,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,   227,    66,    67,    68,    -1,    70,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    53,    54,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    66,    67,
      68,    -1,    70,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    52,    53,    54,    -1,    -1,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      52,    53,    54,    -1,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    53,    54,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    -1,    68,    -1,    70,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    14,    15,    25,
      27,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    57,    58,    59,
      60,    61,    62,    63,    64,    66,    68,    70,    72,    73,
      74,    75,    76,    77,    78,    79,    81,    82,    83,    88,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   112,    51,     3,    98,   109,   110,
     111,   107,   107,    26,    98,   110,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,    25,
       3,    27,     3,    27,     3,     3,    27,    98,    27,    27,
       3,    67,    73,    25,    25,     0,    74,    74,    75,    75,
      76,    66,    80,    25,    75,    76,    66,    66,    27,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     9,    10,
      11,    12,    13,    31,    32,    29,    26,    28,    28,    26,
      26,    98,    27,    27,    27,    25,    27,    66,    67,    98,
      98,    69,    94,    67,    73,    70,    98,    55,    56,    67,
      84,    85,    86,    67,    73,    98,    98,    98,    98,    98,
      98,   100,   101,   102,   104,   104,   105,   105,   105,   107,
     107,   109,    98,   111,   111,    26,     3,    26,    91,    92,
       3,    67,    93,    26,    26,    75,    76,    67,    25,    27,
      98,    51,    67,    85,    74,    67,    30,    26,    28,    24,
      87,    89,    26,    28,    28,    67,    87,    87,    98,    98,
      51,     3,     3,    90,     3,     3,    26,    27,    28,    87,
      27,    98,     3,    26,    87
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    71,    72,    72,    73,    73,    74,    74,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    76,    76,    76,    76,    77,    78,    79,    80,    80,
      81,    82,    83,    84,    84,    85,    85,    86,    86,    87,
      87,    88,    89,    89,    90,    90,    91,    91,    91,    92,
      92,    93,    93,    94,    95,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    97,    97,    98,    98,
      98,    98,    98,    98,    98,    98,    99,    99,   100,   100,
     101,   101,   102,   103,   103,   103,   104,   104,   104,   105,
     105,   106,   106,   106,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     108,   108,   109,   109,   109,   110,   110,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   112
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       4,     2,     7,     2,     4,     3,     4,     3,     5,     4,
       1,     2,     4,     2,     2,     5,     5,     1,     3,     2,
       1,     9,     4,     1,     2,     1,     2,     3,     2,     0,
       1,     5,     1,     3,     1,     3,     1,     2,     2,     3,
       3,     1,     3,     1,     2,     2,     1,     3,     2,     2,
       2,     3,     2,     3,     2,     3,     3,     2,     3,     2,
       3,     3,     3,     3,     3,     1,     3,     1,     3,     1,
       3,     1,     1,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       4,     1,     1,     1,     0,     3,     3,     1,     0,     3,
       1,     1,     1,     1,     1,     3,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = SC_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == SC_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (st, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use SC_error or SC_UNDEF. */
#define YYERRCODE SC_UNDEF


/* Enable debugging if requested.  */
#if SC_DEBUG

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
                  Kind, Value, st); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ScParseState *st)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (st);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ScParseState *st)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, st);
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
                 int yyrule, ScParseState *st)
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
                       &yyvsp[(yyi + 1) - (yynrhs)], st);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, st); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !SC_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !SC_DEBUG */


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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, ScParseState *st)
{
  YY_USE (yyvaluep);
  YY_USE (st);
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
yyparse (ScParseState *st)
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

  yychar = SC_EMPTY; /* Cause a token to be read.  */

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
  if (yychar == SC_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, st);
    }

  if (yychar <= SC_EOF)
    {
      yychar = SC_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == SC_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = SC_UNDEF;
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
  yychar = SC_EMPTY;
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
  case 10: /* matched_stmt: if_head matched_stmt else_keyword matched_stmt  */
#line 226 "snocone_parse.y"
                                        { sc_finalize_if_else_pst(st, (yyvsp[-3].ifhead), (yyvsp[-1].markv)); }
#line 1602 "snocone_parse.tab.c"
    break;

  case 11: /* matched_stmt: while_head matched_stmt  */
#line 228 "snocone_parse.y"
                                        { sc_finalize_while_pst(st, (yyvsp[-1].whilehead), (yyvsp[-1].whilehead)->cond); }
#line 1608 "snocone_parse.tab.c"
    break;

  case 12: /* matched_stmt: do_head do_body T_WHILE T_LPAREN expr0 T_RPAREN T_SEMICOLON  */
#line 230 "snocone_parse.y"
                                        { sc_finalize_do_while_pst(st, (yyvsp[-6].dohead), (yyvsp[-2].expr)); }
#line 1614 "snocone_parse.tab.c"
    break;

  case 13: /* matched_stmt: for_head matched_stmt  */
#line 232 "snocone_parse.y"
                                        { sc_finalize_for_pst(st, (yyvsp[-1].forhead)); }
#line 1620 "snocone_parse.tab.c"
    break;

  case 14: /* matched_stmt: func_head T_LBRACE stmt_list T_RBRACE  */
#line 234 "snocone_parse.y"
                                        { sc_finalize_function_pst(st, (yyvsp[-3].funchead)); }
#line 1626 "snocone_parse.tab.c"
    break;

  case 15: /* matched_stmt: func_head T_LBRACE T_RBRACE  */
#line 236 "snocone_parse.y"
                                        { sc_finalize_function_pst(st, (yyvsp[-2].funchead)); }
#line 1632 "snocone_parse.tab.c"
    break;

  case 16: /* matched_stmt: switch_head T_LBRACE switch_body T_RBRACE  */
#line 238 "snocone_parse.y"
                                        { sc_finalize_switch_pst(st, (yyvsp[-3].switchhead)); }
#line 1638 "snocone_parse.tab.c"
    break;

  case 17: /* matched_stmt: switch_head T_LBRACE T_RBRACE  */
#line 240 "snocone_parse.y"
                                        { sc_finalize_switch_pst(st, (yyvsp[-2].switchhead)); }
#line 1644 "snocone_parse.tab.c"
    break;

  case 18: /* matched_stmt: T_STRUCT T_IDENT T_LBRACE struct_field_list T_RBRACE  */
#line 242 "snocone_parse.y"
                                        { sc_emit_struct(st, (yyvsp[-3].str), (yyvsp[-1].expr)); ct_drop((yyvsp[-3].str)); }
#line 1650 "snocone_parse.tab.c"
    break;

  case 19: /* matched_stmt: T_STRUCT T_IDENT T_LBRACE T_RBRACE  */
#line 244 "snocone_parse.y"
                                        { sc_emit_struct(st, (yyvsp[-2].str), ast_node_new(TT_FIELDS)); ct_drop((yyvsp[-2].str)); }
#line 1656 "snocone_parse.tab.c"
    break;

  case 21: /* unmatched_stmt: if_head stmt  */
#line 249 "snocone_parse.y"
                                        { sc_finalize_if_no_else_pst(st, (yyvsp[-1].ifhead)); }
#line 1662 "snocone_parse.tab.c"
    break;

  case 22: /* unmatched_stmt: if_head matched_stmt else_keyword unmatched_stmt  */
#line 251 "snocone_parse.y"
                                        { sc_finalize_if_else_pst(st, (yyvsp[-3].ifhead), (yyvsp[-1].markv)); }
#line 1668 "snocone_parse.tab.c"
    break;

  case 23: /* unmatched_stmt: while_head unmatched_stmt  */
#line 253 "snocone_parse.y"
                                        { sc_finalize_while_pst(st, (yyvsp[-1].whilehead), (yyvsp[-1].whilehead)->cond); }
#line 1674 "snocone_parse.tab.c"
    break;

  case 24: /* unmatched_stmt: for_head unmatched_stmt  */
#line 255 "snocone_parse.y"
                                        { sc_finalize_for_pst(st, (yyvsp[-1].forhead)); }
#line 1680 "snocone_parse.tab.c"
    break;

  case 25: /* if_head: T_IF T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 258 "snocone_parse.y"
                                        { (yyval.ifhead) = sc_if_head_new(st, (yyvsp[-2].expr)); }
#line 1686 "snocone_parse.tab.c"
    break;

  case 26: /* while_head: T_WHILE T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 261 "snocone_parse.y"
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          struct WhileHead *wh = ct_zalloc(1, sizeof *wh);
                                          wh->cond        = (yyvsp[-2].expr);
                                          wh->mark        = st->block->n;
                                          (yyval.whilehead) = wh; }
#line 1696 "snocone_parse.tab.c"
    break;

  case 27: /* do_head: T_DO  */
#line 267 "snocone_parse.y"
                                    { sc_loop_push(st, NULL, NULL, 1);
                                      struct DoHead *dh = ct_zalloc(1, sizeof *dh);
                                      dh->mark = st->block->n;
                                      (yyval.dohead) = dh; }
#line 1705 "snocone_parse.tab.c"
    break;

  case 30: /* for_lead: T_FOR  */
#line 275 "snocone_parse.y"
                                     { }
#line 1711 "snocone_parse.tab.c"
    break;

  case 31: /* for_head: for_lead T_LPAREN expr0 T_SEMICOLON expr0 T_SEMICOLON expr0 T_RPAREN opt_head_sep  */
#line 278 "snocone_parse.y"
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          (yyval.forhead) = sc_for_head_new_pst(st, (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), st->block->n); }
#line 1718 "snocone_parse.tab.c"
    break;

  case 32: /* switch_head: T_SWITCH T_LPAREN expr0 T_RPAREN  */
#line 282 "snocone_parse.y"
                                        { (yyval.switchhead) = sc_switch_head_new(st, (yyvsp[-1].expr)); }
#line 1724 "snocone_parse.tab.c"
    break;

  case 37: /* case_or_default_label: T_CASE expr0 T_COLON  */
#line 291 "snocone_parse.y"
                                        { sc_switch_case_label(st, (yyvsp[-1].expr)); }
#line 1730 "snocone_parse.tab.c"
    break;

  case 38: /* case_or_default_label: T_DEFAULT T_COLON  */
#line 292 "snocone_parse.y"
                                        { sc_switch_default_label(st); }
#line 1736 "snocone_parse.tab.c"
    break;

  case 41: /* func_head: T_DEFINE T_IDENT T_LPAREN func_arglist func_locals  */
#line 299 "snocone_parse.y"
                                        { (yyval.funchead) = sc_func_head_new_pst(st, (yyvsp[-3].str), (yyvsp[-1].expr), (yyvsp[0].expr)); ct_drop((yyvsp[-3].str)); }
#line 1742 "snocone_parse.tab.c"
    break;

  case 42: /* func_locals: opt_head_sep  */
#line 302 "snocone_parse.y"
                                                        { (yyval.expr) = ast_node_new(TT_LOCALS); }
#line 1748 "snocone_parse.tab.c"
    break;

  case 43: /* func_locals: opt_head_sep func_locals_ne opt_head_sep  */
#line 303 "snocone_parse.y"
                                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1754 "snocone_parse.tab.c"
    break;

  case 44: /* func_locals_ne: T_IDENT  */
#line 306 "snocone_parse.y"
                                       { (yyval.expr) = sc_list_one(TT_LOCALS, (yyvsp[0].str)); }
#line 1760 "snocone_parse.tab.c"
    break;

  case 45: /* func_locals_ne: func_locals_ne T_COMMA T_IDENT  */
#line 308 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1766 "snocone_parse.tab.c"
    break;

  case 46: /* func_arglist: T_RPAREN  */
#line 311 "snocone_parse.y"
                                       { (yyval.expr) = ast_node_new(TT_PARAMS); }
#line 1772 "snocone_parse.tab.c"
    break;

  case 47: /* func_arglist: T_IDENT T_RPAREN  */
#line 312 "snocone_parse.y"
                                       { (yyval.expr) = sc_list_one(TT_PARAMS, (yyvsp[-1].str)); }
#line 1778 "snocone_parse.tab.c"
    break;

  case 48: /* func_arglist: func_arglist_ne T_RPAREN  */
#line 313 "snocone_parse.y"
                                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 1784 "snocone_parse.tab.c"
    break;

  case 49: /* func_arglist_ne: T_IDENT T_COMMA T_IDENT  */
#line 317 "snocone_parse.y"
                { (yyval.expr) = sc_list_add(sc_list_one(TT_PARAMS, (yyvsp[-2].str)), (yyvsp[0].str)); }
#line 1790 "snocone_parse.tab.c"
    break;

  case 50: /* func_arglist_ne: func_arglist_ne T_COMMA T_IDENT  */
#line 319 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1796 "snocone_parse.tab.c"
    break;

  case 51: /* struct_field_list: T_IDENT  */
#line 323 "snocone_parse.y"
                { (yyval.expr) = sc_list_one(TT_FIELDS, (yyvsp[0].str)); }
#line 1802 "snocone_parse.tab.c"
    break;

  case 52: /* struct_field_list: struct_field_list T_COMMA T_IDENT  */
#line 325 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1808 "snocone_parse.tab.c"
    break;

  case 53: /* else_keyword: T_ELSE  */
#line 328 "snocone_parse.y"
                                     { (yyval.markv) = st->block->n; }
#line 1814 "snocone_parse.tab.c"
    break;

  case 54: /* label_decl: T_IDENT T_COLON  */
#line 331 "snocone_parse.y"
                                     { sc_append_label_node(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1820 "snocone_parse.tab.c"
    break;

  case 55: /* simple_stmt: expr0 T_SEMICOLON  */
#line 333 "snocone_parse.y"
                                               { sc_append_stmt(st, (yyvsp[-1].expr)); }
#line 1826 "snocone_parse.tab.c"
    break;

  case 56: /* simple_stmt: T_SEMICOLON  */
#line 334 "snocone_parse.y"
                                               {         }
#line 1832 "snocone_parse.tab.c"
    break;

  case 57: /* simple_stmt: T_RETURN expr0 T_SEMICOLON  */
#line 335 "snocone_parse.y"
                                            { tree_t *r = ast_node_new(TT_RETURN); ast_push(r, (yyvsp[-1].expr));
                                             sc_append_stmt(st, r); }
#line 1839 "snocone_parse.tab.c"
    break;

  case 58: /* simple_stmt: T_RETURN T_SEMICOLON  */
#line 337 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_RETURN)); }
#line 1845 "snocone_parse.tab.c"
    break;

  case 59: /* simple_stmt: T_FRETURN T_SEMICOLON  */
#line 338 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_PROC_FAIL)); }
#line 1851 "snocone_parse.tab.c"
    break;

  case 60: /* simple_stmt: T_NRETURN T_SEMICOLON  */
#line 339 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_NRETURN)); }
#line 1857 "snocone_parse.tab.c"
    break;

  case 61: /* simple_stmt: T_GOTO T_IDENT T_SEMICOLON  */
#line 340 "snocone_parse.y"
                                            { tree_t *g = ast_node_new(TT_GOTO_U); ast_push(g, sc_qlit((yyvsp[-1].str))); ct_drop((yyvsp[-1].str)); sc_append_stmt(st, g); }
#line 1863 "snocone_parse.tab.c"
    break;

  case 62: /* simple_stmt: T_BREAK T_SEMICOLON  */
#line 341 "snocone_parse.y"
                                            { sc_append_break(st, NULL); }
#line 1869 "snocone_parse.tab.c"
    break;

  case 63: /* simple_stmt: T_BREAK T_IDENT T_SEMICOLON  */
#line 342 "snocone_parse.y"
                                            { sc_append_break(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1875 "snocone_parse.tab.c"
    break;

  case 64: /* simple_stmt: T_CONTINUE T_SEMICOLON  */
#line 343 "snocone_parse.y"
                                            { sc_append_continue(st, NULL); }
#line 1881 "snocone_parse.tab.c"
    break;

  case 65: /* simple_stmt: T_CONTINUE T_IDENT T_SEMICOLON  */
#line 344 "snocone_parse.y"
                                             { sc_append_continue(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1887 "snocone_parse.tab.c"
    break;

  case 66: /* block_stmt: T_LBRACE stmt_list T_RBRACE  */
#line 346 "snocone_parse.y"
                                               { }
#line 1893 "snocone_parse.tab.c"
    break;

  case 67: /* block_stmt: T_LBRACE T_RBRACE  */
#line 347 "snocone_parse.y"
                                               {                  }
#line 1899 "snocone_parse.tab.c"
    break;

  case 68: /* expr0: expr1 T_2EQUAL expr0  */
#line 350 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1905 "snocone_parse.tab.c"
    break;

  case 69: /* expr0: expr1 T_2EQUAL  */
#line 352 "snocone_parse.y"
                                { tree_t *empty = expr_new(TT_QLIT);
                                  empty->sval = ct_strdup("");
                                  (yyval.expr) = expr_binary(TT_ASSIGN, (yyvsp[-1].expr), empty); }
#line 1913 "snocone_parse.tab.c"
    break;

  case 70: /* expr0: expr1 T_PLUS_ASSIGN expr0  */
#line 356 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPLUS;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1920 "snocone_parse.tab.c"
    break;

  case 71: /* expr0: expr1 T_MINUS_ASSIGN expr0  */
#line 359 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGMINUS;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1927 "snocone_parse.tab.c"
    break;

  case 72: /* expr0: expr1 T_STAR_ASSIGN expr0  */
#line 362 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSTAR;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1934 "snocone_parse.tab.c"
    break;

  case 73: /* expr0: expr1 T_SLASH_ASSIGN expr0  */
#line 365 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSLASH;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1941 "snocone_parse.tab.c"
    break;

  case 74: /* expr0: expr1 T_CARET_ASSIGN expr0  */
#line 368 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPOW;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1948 "snocone_parse.tab.c"
    break;

  case 75: /* expr0: expr1  */
#line 371 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1954 "snocone_parse.tab.c"
    break;

  case 76: /* expr1: expr1 T_2QUEST expr3  */
#line 374 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SCAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1960 "snocone_parse.tab.c"
    break;

  case 77: /* expr1: expr3  */
#line 376 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1966 "snocone_parse.tab.c"
    break;

  case 78: /* expr3: expr3 T_2PIPE expr4  */
#line 379 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ALT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1972 "snocone_parse.tab.c"
    break;

  case 79: /* expr3: expr4  */
#line 381 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1978 "snocone_parse.tab.c"
    break;

  case 80: /* expr4: expr4 T_CONCAT expr5  */
#line 384 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1984 "snocone_parse.tab.c"
    break;

  case 81: /* expr4: expr5  */
#line 386 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1990 "snocone_parse.tab.c"
    break;

  case 82: /* expr5: expr6  */
#line 389 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1996 "snocone_parse.tab.c"
    break;

  case 83: /* expr6: expr6 T_2PLUS expr9  */
#line 392 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2002 "snocone_parse.tab.c"
    break;

  case 84: /* expr6: expr6 T_2MINUS expr9  */
#line 394 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2008 "snocone_parse.tab.c"
    break;

  case 85: /* expr6: expr9  */
#line 396 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2014 "snocone_parse.tab.c"
    break;

  case 86: /* expr9: expr9 T_2STAR expr11  */
#line 399 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2020 "snocone_parse.tab.c"
    break;

  case 87: /* expr9: expr9 T_2SLASH expr11  */
#line 401 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2026 "snocone_parse.tab.c"
    break;

  case 88: /* expr9: expr11  */
#line 403 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2032 "snocone_parse.tab.c"
    break;

  case 89: /* expr11: expr12 T_2CARET expr11  */
#line 406 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_POW, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2038 "snocone_parse.tab.c"
    break;

  case 90: /* expr11: expr12  */
#line 408 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2044 "snocone_parse.tab.c"
    break;

  case 91: /* expr12: expr12 T_2DOLLAR expr14  */
#line 411 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_CAPT_IMMED_ASGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2050 "snocone_parse.tab.c"
    break;

  case 92: /* expr12: expr12 T_2DOT expr14  */
#line 413 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_CAPT_COND_ASGN,  (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2056 "snocone_parse.tab.c"
    break;

  case 93: /* expr12: expr14  */
#line 415 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2062 "snocone_parse.tab.c"
    break;

  case 94: /* expr14: T_1PLUS expr14  */
#line 418 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_PLS, (yyvsp[0].expr)); }
#line 2068 "snocone_parse.tab.c"
    break;

  case 95: /* expr14: T_1MINUS expr14  */
#line 420 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_MNS, (yyvsp[0].expr)); }
#line 2074 "snocone_parse.tab.c"
    break;

  case 96: /* expr14: T_1STAR expr14  */
#line 421 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_DEFER,       (yyvsp[0].expr)); }
#line 2080 "snocone_parse.tab.c"
    break;

  case 97: /* expr14: T_1DOT expr14  */
#line 422 "snocone_parse.y"
                                { if ((yyvsp[0].expr) && ((yyvsp[0].expr)->t == TT_ILIT || (yyvsp[0].expr)->t == TT_FLIT || (yyvsp[0].expr)->t == TT_QLIT)) sc_error(st, "value used where name is required"); (yyval.expr) = expr_unary(TT_NAME, (yyvsp[0].expr)); }
#line 2086 "snocone_parse.tab.c"
    break;

  case 98: /* expr14: T_1DOLLAR expr14  */
#line 423 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_INDIRECT,    (yyvsp[0].expr)); }
#line 2092 "snocone_parse.tab.c"
    break;

  case 99: /* expr14: T_1AT expr14  */
#line 424 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_CAPT_CURSOR, (yyvsp[0].expr)); }
#line 2098 "snocone_parse.tab.c"
    break;

  case 100: /* expr14: T_1TILDE expr14  */
#line 425 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_NOT,         (yyvsp[0].expr)); }
#line 2104 "snocone_parse.tab.c"
    break;

  case 101: /* expr14: T_1QUEST expr14  */
#line 426 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_INTERROGATE, (yyvsp[0].expr)); }
#line 2110 "snocone_parse.tab.c"
    break;

  case 102: /* expr14: T_1AMP expr14  */
#line 427 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("&"); (yyval.expr) = _e; }
#line 2117 "snocone_parse.tab.c"
    break;

  case 103: /* expr14: T_1PERCENT expr14  */
#line 429 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("%"); (yyval.expr) = _e; }
#line 2124 "snocone_parse.tab.c"
    break;

  case 104: /* expr14: T_1SLASH expr14  */
#line 431 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("/"); (yyval.expr) = _e; }
#line 2131 "snocone_parse.tab.c"
    break;

  case 105: /* expr14: T_1POUND expr14  */
#line 433 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("#"); (yyval.expr) = _e; }
#line 2138 "snocone_parse.tab.c"
    break;

  case 106: /* expr14: T_1PIPE expr14  */
#line 435 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("|"); (yyval.expr) = _e; }
#line 2145 "snocone_parse.tab.c"
    break;

  case 107: /* expr14: T_1EQUAL expr14  */
#line 437 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("="); (yyval.expr) = _e; }
#line 2152 "snocone_parse.tab.c"
    break;

  case 108: /* expr14: T_1BANG expr14  */
#line 439 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("!"); (yyval.expr) = _e; }
#line 2159 "snocone_parse.tab.c"
    break;

  case 109: /* expr14: expr15  */
#line 442 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2165 "snocone_parse.tab.c"
    break;

  case 110: /* expr15: expr15 T_LBRACK exprlist T_RBRACK  */
#line 445 "snocone_parse.y"
                                { tree_t *idx = expr_new(TT_IDX);
                                  tree_t *sub = expr_new(TT_VLIST);
                                  expr_add_child(idx, (yyvsp[-3].expr));
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(sub, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  expr_add_child(idx, sub);
                                  (yyval.expr) = idx; }
#line 2178 "snocone_parse.tab.c"
    break;

  case 111: /* expr15: expr17  */
#line 454 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2184 "snocone_parse.tab.c"
    break;

  case 112: /* exprlist: exprlist_c  */
#line 457 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2190 "snocone_parse.tab.c"
    break;

  case 113: /* exprlist: expr0  */
#line 459 "snocone_parse.y"
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 2196 "snocone_parse.tab.c"
    break;

  case 114: /* exprlist: %empty  */
#line 461 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2202 "snocone_parse.tab.c"
    break;

  case 115: /* exprlist_c: exprlist_c T_COMMA optexpr  */
#line 464 "snocone_parse.y"
                                { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
#line 2208 "snocone_parse.tab.c"
    break;

  case 116: /* exprlist_c: optexpr T_COMMA optexpr  */
#line 466 "snocone_parse.y"
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, (yyvsp[-2].expr)); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 2214 "snocone_parse.tab.c"
    break;

  case 117: /* optexpr: expr0  */
#line 469 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2220 "snocone_parse.tab.c"
    break;

  case 118: /* optexpr: %empty  */
#line 471 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2226 "snocone_parse.tab.c"
    break;

  case 119: /* expr17: T_CALL exprlist T_RPAREN  */
#line 474 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_FNC);
                                  tree_t *a = expr_new(TT_ARGS);
                                  expr_add_child(e, sc_qlit((yyvsp[-2].str))); ct_drop((yyvsp[-2].str));
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(a, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  expr_add_child(e, a);
                                  (yyval.expr) = e; }
#line 2239 "snocone_parse.tab.c"
    break;

  case 120: /* expr17: T_IDENT  */
#line 483 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_VAR);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 2247 "snocone_parse.tab.c"
    break;

  case 121: /* expr17: T_KEYWORD  */
#line 487 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_KEYWORD);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 2255 "snocone_parse.tab.c"
    break;

  case 122: /* expr17: T_INT  */
#line 491 "snocone_parse.y"
                                { (yyval.expr) = sc_int_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2261 "snocone_parse.tab.c"
    break;

  case 123: /* expr17: T_REAL  */
#line 493 "snocone_parse.y"
                                { (yyval.expr) = sc_real_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2267 "snocone_parse.tab.c"
    break;

  case 124: /* expr17: T_STR  */
#line 495 "snocone_parse.y"
                                { (yyval.expr) = sc_str_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2273 "snocone_parse.tab.c"
    break;

  case 125: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 497 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 2279 "snocone_parse.tab.c"
    break;

  case 126: /* expr17: T_LPAREN exprlist_c T_RPAREN  */
#line 499 "snocone_parse.y"
                                { tree_t *a = expr_new(TT_VLIST);
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(a, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  (yyval.expr) = a; }
#line 2289 "snocone_parse.tab.c"
    break;

  case 127: /* expr17: T_LPAREN T_RPAREN  */
#line 505 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2295 "snocone_parse.tab.c"
    break;


#line 2299 "snocone_parse.tab.c"

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
  yytoken = yychar == SC_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (st, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= SC_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == SC_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, st);
          yychar = SC_EMPTY;
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, st);
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
  yyerror (st, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != SC_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, st);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, st);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 507 "snocone_parse.y"

void sc_error(ScParseState *st, const char *msg) {
    fprintf(stderr, "%s:%d: snocone parse error: %s\n",
            st->filename ? st->filename : "<stdin>",
            st->ctx ? st->ctx->line : 0,
            msg);
    st->nerrors++;
}
static void sc_append_stmt(ScParseState *st, tree_t *top) {
    if (!top) return;
    top->line = st->ctx ? st->ctx->line : 0;
    ast_push(st->block, top);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_qlit(const char *txt) {
    tree_t *q = ast_node_new(TT_QLIT);
    q->sval = ct_strdup(txt);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_list_add(tree_t *l, char *ident) {
    tree_t *v = ast_node_new(TT_VAR);
    v->sval = ident;
    ast_push(l, v);
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_list_one(tree_e k, char *ident) {
    return sc_list_add(ast_node_new(k), ident);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_int_literal(const char *txt) {
    tree_t *e = expr_new(TT_ILIT);
    e->ival = strtol(txt, NULL, 10);
    return e;
}
static tree_t *sc_real_literal(const char *txt) {
    extern int rt_gtn_real(const char *, long, double *);
    tree_t *e = expr_new(TT_FLIT);
    if (!rt_gtn_real(txt, (long)strlen(txt), &e->dval)) e->dval = strtod(txt, NULL);
    return e;
}
static tree_t *sc_str_literal(const char *txt) {
    tree_t *e = expr_new(TT_QLIT);
    e->sval = ct_strdup(txt);
    return e;
}
static struct IfHead *sc_if_head_new(ScParseState *st, tree_t *cond) {
    struct IfHead *h = ct_zalloc(1, sizeof *h);
    h->cond        = cond;
    h->mark        = st->block->n;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    return h;
}
static struct ForHead *sc_for_head_new_pst(ScParseState *st, tree_t *init, tree_t *cond, tree_t *step, int mark) {
    (void)st;
    struct ForHead *h = ct_zalloc(1, sizeof *h);
    h->init        = init;
    h->cond        = cond;
    h->step        = step;
    h->mark        = mark;
    return h;
}
static struct FuncHead *sc_func_head_new_pst(ScParseState *st, char *name, tree_t *params, tree_t *locals) {
    struct FuncHead *h  = ct_zalloc(1, sizeof *h);
    h->name             = ct_strdup(name);
    h->params           = params;
    h->locals           = locals;
    h->prev_func        = st->cur_func_name;
    h->mark             = st->block->n;
    st->cur_func_name   = h->name;
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_finalize_function_pst(ScParseState *st, struct FuncHead *h)
{
    tree_t *body  = sc_collect_body(st, h->mark);
    tree_t *def   = ast_node_new(TT_DEFINE);
    ast_push(def, sc_qlit(h->name));
    ast_push(def, h->params);
    ast_push(def, h->locals);
    ast_push(def, body);
    st->cur_func_name = h->prev_func;
    ct_drop(h->name); ct_drop(h);
    sc_append_stmt(st, def);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_append_label_node(ScParseState *st, const char *name) {
    for (uint32_t i = 0; i < st->labels.len; i++) if (!strcmp(CV_AT(st->labels, char *, i), name)) { sc_error(st, ct_fmt("duplicate label '%s'", name)); return; }
    tree_t *lab = ast_node_new(TT_LABEL);
    ast_push(lab, sc_qlit(name));
    CV_PUSH(st->labels, char *) = lab->c[0]->sval;
    sc_append_stmt(st, lab);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_collect_body(ScParseState *st, int mark)
{
    tree_t *block = ast_node_new(TT_PROGRAM);
    for (int i = mark; i < st->block->n; i++) ast_push(block, st->block->c[i]);
    st->block->n = mark;
    return block;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_finalize_if_no_else_pst(ScParseState *st, struct IfHead *h)
{
    tree_t *then_block = sc_collect_body(st, h->mark);
    tree_t *if_node    = ast_node_new(TT_IF);
    ast_push(if_node, h->cond);
    ast_push(if_node, then_block);
    sc_append_stmt(st, if_node);
    ct_drop(h);
}
static void sc_finalize_if_else_pst(ScParseState *st, struct IfHead *h, int before_else)
{
    tree_t *else_block = sc_collect_body(st, before_else);
    tree_t *then_block = sc_collect_body(st, h->mark);
    tree_t *if_node    = ast_node_new(TT_IF);
    ast_push(if_node, h->cond);
    ast_push(if_node, then_block);
    ast_push(if_node, else_block);
    sc_append_stmt(st, if_node);
    ct_drop(h);
}
static void sc_finalize_while_pst(ScParseState *st, struct WhileHead *h, tree_t *cond)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *w      = ast_node_new(TT_WHILE);
    ast_push(w, cond);
    ast_push(w, body);
    sc_loop_pop(st);
    ct_drop(h);
    sc_append_stmt(st, w);
}
static void sc_finalize_do_while_pst(ScParseState *st, struct DoHead *h, tree_t *cond)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *dw     = ast_node_new(TT_DO_WHILE);
    ast_push(dw, body);
    ast_push(dw, cond);
    sc_loop_pop(st);
    ct_drop(h);
    sc_append_stmt(st, dw);
}
static void sc_finalize_for_pst(ScParseState *st, struct ForHead *h)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *f      = ast_node_new(TT_FOR);
    ast_push(f, h->init ? h->init : ast_node_new(TT_NUL));
    ast_push(f, h->cond);
    ast_push(f, h->step);
    ast_push(f, body);
    sc_loop_pop(st);
    sc_append_stmt(st, f);
    ct_drop(h);
}
static void sc_loop_push(ScParseState *st, char *cont_label, char *end_label, int is_loop) {
    LoopFrame *f = ct_zalloc(1, sizeof *f);
    f->cont_label = cont_label;
    f->end_label  = end_label;
    f->is_loop    = is_loop;
    f->outer      = st->loop_top;
    st->loop_top  = f;
}
static void sc_loop_pop(ScParseState *st) {
    LoopFrame *f = st->loop_top;
    if (!f) return;
    st->loop_top = f->outer;
    ct_drop(f->cont_label);
    ct_drop(f->end_label);
    ct_drop(f);
}
static void sc_append_break(ScParseState *st, char *user_label) {
    if (!st->loop_top) {
        sc_error(st, user_label ? "break: no enclosing loop or switch" : "break outside of loop or switch");
        return;
    }
    tree_t *brk = ast_node_new(TT_LOOP_BREAK);
    if (user_label) {
        tree_t *q = ast_node_new(TT_QLIT); q->sval = ct_strdup(user_label);
        ast_push(brk, q);
    }
    sc_append_stmt(st, brk);
}
static void sc_append_continue(ScParseState *st, char *user_label) {
    if (!st->loop_top) {
        sc_error(st, user_label ? "continue: no enclosing loop" : "continue outside of loop");
        return;
    }
    tree_t *nxt = ast_node_new(TT_LOOP_NEXT);
    if (user_label) {
        tree_t *q = ast_node_new(TT_QLIT); q->sval = ct_strdup(user_label);
        ast_push(nxt, q);
    }
    sc_append_stmt(st, nxt);
}
static void sc_switch_cases_grow(struct SwitchHead *h) {
    if (h->cases_count >= h->cases_cap) {
        int newcap = h->cases_cap ? h->cases_cap * 2 : 4;
        h->cases = ct_grow(h->cases, newcap * sizeof *h->cases);
        h->cases_cap = newcap;
    }
}
static struct SwitchHead *sc_switch_head_new(ScParseState *st, tree_t *disc) {
    struct SwitchHead *h = ct_zalloc(1, sizeof *h);
    h->disc          = disc;
    h->lineno        = st->ctx ? st->ctx->line : 0;
    h->prev_switch   = st->cur_switch;
    h->end_label     = NULL;
    h->default_label = NULL;
    h->has_default   = 0;
    h->tmp_name      = NULL;
    sc_loop_push(st, NULL, NULL, 0);
    st->cur_switch = h;
    return h;
}
static void sc_switch_case_label(ScParseState *st, tree_t *value) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) { sc_error(st, "case label outside of switch"); (void)value; return; }
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].value       = value;
    h->cases[h->cases_count].case_label  = NULL;
    h->cases[h->cases_count].mark        = st->block->n;
    h->cases_count++;
}
static void sc_switch_default_label(ScParseState *st) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) { sc_error(st, "default label outside of switch"); return; }
    if (h->has_default) { sc_error(st, "duplicate default label in switch"); return; }
    h->has_default = 1;
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].value       = NULL;
    h->cases[h->cases_count].case_label  = NULL;
    h->cases[h->cases_count].mark        = st->block->n;
    h->cases_count++;
}
static void sc_finalize_switch_pst(ScParseState *st, struct SwitchHead *h)
{
    int nc = h->cases_count;
    tree_t **bodies = ct_zalloc((size_t)(nc > 0 ? nc : 1), sizeof *bodies);
    for (int i = nc - 1; i >= 0; i--)
        bodies[i] = sc_collect_body(st, h->cases[i].mark);
    tree_t *node = ast_node_new(TT_CASE);
    ast_push(node, h->disc);
    for (int i = 0; i < nc; i++) {
        if (h->cases[i].value)
            ast_push(node, h->cases[i].value);
        else {
            tree_t *nul = ast_node_new(TT_NUL); ast_push(node, nul);
        }
        ast_push(node, bodies[i]);
    }
    ct_drop(bodies);
    sc_loop_pop(st);
    st->cur_switch = h->prev_switch;
    for (int i = 0; i < nc; i++) ct_drop(h->cases[i].case_label);
    ct_drop(h->cases);
    ct_drop(h->end_label);
    ct_drop(h->default_label);
    ct_drop(h->tmp_name);
    ct_drop(h);
    sc_append_stmt(st, node);
}
static void sc_emit_struct(ScParseState *st, char *name, tree_t *fields) {
    tree_t *node = ast_node_new(TT_STRUCT);
    ast_push(node, sc_qlit(name));
    ast_push(node, fields);
    sc_append_stmt(st, node);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *snocone_parse_tree(const char *src, const char *filename) {
    LexCtx          ctx = {0};
    ctx.p           = src ? src : "";
    ctx.line        = 1;
    ScParseState    state = {0};
    state.ctx       = (struct LexCtx *)&ctx;
    state.block     = ast_node_new(TT_PROGRAM);
    state.filename  = filename;
    state.nerrors   = 0;
    int rc = sc_parse(&state);
    while (state.loop_top) sc_loop_pop(&state);
    if (rc != 0 || state.nerrors > 0) return NULL;
    return state.block;
}
