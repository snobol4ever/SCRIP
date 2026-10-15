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
  YYSYMBOL_T_IMPORT = 65,                  /* T_IMPORT  */
  YYSYMBOL_T_UNKNOWN = 66,                 /* T_UNKNOWN  */
  YYSYMBOL_T_LBRACE = 67,                  /* T_LBRACE  */
  YYSYMBOL_T_RBRACE = 68,                  /* T_RBRACE  */
  YYSYMBOL_T_IF = 69,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 70,                    /* T_ELSE  */
  YYSYMBOL_T_WHILE = 71,                   /* T_WHILE  */
  YYSYMBOL_YYACCEPT = 72,                  /* $accept  */
  YYSYMBOL_program = 73,                   /* program  */
  YYSYMBOL_stmt_list = 74,                 /* stmt_list  */
  YYSYMBOL_stmt = 75,                      /* stmt  */
  YYSYMBOL_matched_stmt = 76,              /* matched_stmt  */
  YYSYMBOL_unmatched_stmt = 77,            /* unmatched_stmt  */
  YYSYMBOL_if_head = 78,                   /* if_head  */
  YYSYMBOL_while_head = 79,                /* while_head  */
  YYSYMBOL_do_head = 80,                   /* do_head  */
  YYSYMBOL_do_body = 81,                   /* do_body  */
  YYSYMBOL_for_lead = 82,                  /* for_lead  */
  YYSYMBOL_for_head = 83,                  /* for_head  */
  YYSYMBOL_switch_head = 84,               /* switch_head  */
  YYSYMBOL_switch_body = 85,               /* switch_body  */
  YYSYMBOL_case_clause = 86,               /* case_clause  */
  YYSYMBOL_case_or_default_label = 87,     /* case_or_default_label  */
  YYSYMBOL_opt_head_sep = 88,              /* opt_head_sep  */
  YYSYMBOL_func_head = 89,                 /* func_head  */
  YYSYMBOL_func_locals = 90,               /* func_locals  */
  YYSYMBOL_func_locals_ne = 91,            /* func_locals_ne  */
  YYSYMBOL_func_arglist = 92,              /* func_arglist  */
  YYSYMBOL_func_arglist_ne = 93,           /* func_arglist_ne  */
  YYSYMBOL_struct_field_list = 94,         /* struct_field_list  */
  YYSYMBOL_else_keyword = 95,              /* else_keyword  */
  YYSYMBOL_label_decl = 96,                /* label_decl  */
  YYSYMBOL_simple_stmt = 97,               /* simple_stmt  */
  YYSYMBOL_block_stmt = 98,                /* block_stmt  */
  YYSYMBOL_expr0 = 99,                     /* expr0  */
  YYSYMBOL_expr1 = 100,                    /* expr1  */
  YYSYMBOL_expr2 = 101,                    /* expr2  */
  YYSYMBOL_expr3 = 102,                    /* expr3  */
  YYSYMBOL_expr4 = 103,                    /* expr4  */
  YYSYMBOL_expr5 = 104,                    /* expr5  */
  YYSYMBOL_expr6 = 105,                    /* expr6  */
  YYSYMBOL_expr7 = 106,                    /* expr7  */
  YYSYMBOL_expr8 = 107,                    /* expr8  */
  YYSYMBOL_expr9 = 108,                    /* expr9  */
  YYSYMBOL_expr10 = 109,                   /* expr10  */
  YYSYMBOL_expr11 = 110,                   /* expr11  */
  YYSYMBOL_expr12 = 111,                   /* expr12  */
  YYSYMBOL_expr13 = 112,                   /* expr13  */
  YYSYMBOL_expr14 = 113,                   /* expr14  */
  YYSYMBOL_expr15 = 114,                   /* expr15  */
  YYSYMBOL_exprlist = 115,                 /* exprlist  */
  YYSYMBOL_exprlist_c = 116,               /* exprlist_c  */
  YYSYMBOL_optexpr = 117,                  /* optexpr  */
  YYSYMBOL_expr17 = 118                    /* expr17  */
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
static void     sc_emit_import         (ScParseState *st, char *name);
static tree_t  *sc_qlit                (const char *txt);
static tree_t  *sc_list_one            (tree_e k, char *ident);
static tree_t  *sc_list_add            (tree_t *l, char *ident);

#line 358 "snocone_parse.tab.c"

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
#define YYFINAL  112
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   796

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  253

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   326


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
      65,    66,    67,    68,    69,    70,    71
};

#if SC_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   214,   214,   215,   217,   218,   220,   221,   224,   225,
     226,   228,   230,   232,   234,   236,   238,   240,   242,   244,
     246,   248,   251,   253,   255,   257,   260,   263,   270,   275,
     276,   278,   280,   284,   287,   288,   290,   291,   294,   295,
     298,   299,   301,   305,   306,   309,   310,   314,   315,   316,
     319,   321,   325,   327,   331,   334,   336,   337,   338,   340,
     341,   342,   343,   344,   345,   346,   347,   349,   350,   352,
     354,   358,   361,   364,   367,   370,   373,   376,   378,   381,
     383,   386,   388,   391,   393,   396,   398,   401,   403,   405,
     408,   410,   413,   415,   418,   420,   423,   425,   428,   430,
     433,   435,   437,   440,   442,   445,   447,   449,   450,   451,
     452,   453,   454,   455,   457,   459,   461,   463,   465,   467,
     469,   472,   481,   484,   486,   489,   491,   493,   496,   499,
     501,   510,   514,   518,   520,   522,   524,   526,   532
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
  "T_FRETURN", "T_NRETURN", "T_STRUCT", "T_IMPORT", "T_UNKNOWN",
  "T_LBRACE", "T_RBRACE", "T_IF", "T_ELSE", "T_WHILE", "$accept",
  "program", "stmt_list", "stmt", "matched_stmt", "unmatched_stmt",
  "if_head", "while_head", "do_head", "do_body", "for_lead", "for_head",
  "switch_head", "switch_body", "case_clause", "case_or_default_label",
  "opt_head_sep", "func_head", "func_locals", "func_locals_ne",
  "func_arglist", "func_arglist_ne", "struct_field_list", "else_keyword",
  "label_decl", "simple_stmt", "block_stmt", "expr0", "expr1", "expr2",
  "expr3", "expr4", "expr5", "expr6", "expr7", "expr8", "expr9", "expr10",
  "expr11", "expr12", "expr13", "expr14", "expr15", "exprlist",
  "exprlist_c", "optexpr", "expr17", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-130)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     533,    -1,  -126,  -126,  -126,  -126,   602,   746,   746,   650,
    -126,   746,   746,   746,   746,   746,   746,   746,   746,   746,
     746,   746,   746,   746,  -126,  -126,     7,     8,    13,    42,
      62,   698,    53,    61,   100,   112,    14,    92,    93,    26,
     533,  -126,  -126,  -126,   533,   533,    52,   105,   533,    64,
      65,  -126,  -126,  -126,   106,    75,   101,   113,   115,  -126,
       4,   102,   128,   131,   111,  -126,     2,  -126,   125,   119,
    -126,  -126,  -126,   135,   139,   140,   141,  -126,  -126,  -126,
     144,    16,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,   746,   145,  -126,   146,  -126,
     147,   142,  -126,   148,  -126,  -126,    99,   149,  -126,   188,
     746,   746,  -126,  -126,  -126,   107,  -126,  -126,   257,   108,
     746,  -126,  -126,   -25,   326,  -126,   746,   746,   746,   746,
     746,   746,   746,   746,   746,   746,   746,   746,   746,   746,
     746,   746,   746,   746,   746,   746,   746,   602,  -126,   746,
     746,  -126,  -126,   152,  -126,  -126,  -126,    20,  -126,     1,
    -126,  -126,   154,   155,  -126,   533,  -126,   395,   158,   157,
     746,   120,  -126,   -19,   533,  -126,  -126,   464,  -126,  -126,
    -126,  -126,  -126,  -126,   101,   113,   115,  -126,   102,   102,
    -126,   128,   131,   111,  -126,  -126,  -126,  -126,  -126,   156,
    -126,  -126,  -126,  -126,    80,  -126,   161,    81,  -126,  -126,
     -20,   161,   161,  -126,  -126,  -126,   746,   746,   136,  -126,
    -126,   533,  -126,  -126,  -126,  -126,   185,  -126,   186,  -126,
    -126,   187,   194,  -126,  -126,  -126,   172,   173,  -126,  -126,
    -126,    23,  -126,  -126,   174,   746,   196,  -126,  -126,   178,
    -126,   161,  -126
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,   131,   132,   133,   134,   135,   125,     0,     0,   129,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     5,     6,     7,     0,     0,     0,     0,     0,     0,
       0,    21,     8,     9,     0,    76,    78,    80,    82,    84,
      86,    89,    91,    93,    95,    97,    99,   102,   104,   120,
     122,    55,   131,   124,     0,   123,     0,   105,   106,   138,
     128,     0,   107,   115,   114,   110,   111,   109,   108,   116,
     117,   118,   112,   113,   119,     0,     0,    63,     0,    65,
       0,     0,    59,     0,    60,    61,     0,     0,    68,     0,
       0,     0,     1,     4,    22,     6,    11,    24,     0,     0,
       0,    13,    25,     0,     0,    56,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   130,   129,
     129,   136,   137,     0,    64,    66,    62,     0,    58,     0,
      20,    67,     0,     0,    54,     0,    30,     0,     0,     0,
       0,     0,    17,     0,    34,    36,    15,     0,    69,    71,
      72,    73,    74,    75,    77,    79,    81,    83,    87,    88,
      85,    90,    92,    94,    96,    98,   100,   101,   103,     0,
     128,   126,   127,    33,     0,    47,    40,     0,    52,    19,
       0,    40,    40,    10,    23,    29,     0,     0,     0,    39,
      16,    35,    37,    14,   121,    48,     0,    41,    43,    42,
      49,     0,     0,    18,    26,    27,     0,     0,    38,    50,
      45,    40,    51,    53,     0,     0,     0,    44,    12,     0,
      46,    40,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -126,  -126,   -34,   -39,   -38,   -36,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,    32,  -126,  -125,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,  -126,    -6,  -126,    74,
      76,    73,  -111,  -126,   -35,    69,    72,    77,   -32,  -126,
     -46,   138,  -126,    67,   207,   -37,  -126
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    39,    40,    41,    42,    43,    44,    45,    46,   119,
      47,    48,    49,   173,   174,   175,   228,    50,   229,   241,
     206,   207,   210,   165,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    74,    75,    76,    70
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      73,   113,   109,    80,   208,   114,   115,   116,   232,   117,
     121,    96,   122,   136,   137,   143,    98,     1,     2,     3,
       4,     5,     6,   204,   187,   103,   112,   190,     7,     8,
     170,   171,    95,   144,   145,    97,   170,   171,   138,     9,
      99,    10,   152,   172,   149,   100,   205,   227,   233,   220,
      71,   246,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,   101,    24,    25,    26,   209,
     113,    27,    28,    29,    30,    31,    32,    33,    34,    35,
     104,    36,   108,    37,   167,    38,   234,   235,   105,   153,
     177,   126,   127,   128,   129,   130,   131,   132,   196,   197,
     198,   188,   189,   106,   162,   163,   225,   230,   226,   231,
     194,   195,   201,   202,   169,   107,   247,   110,   111,   118,
     178,   179,   180,   181,   182,   183,   252,   213,   113,   214,
     120,   123,   124,   125,   133,   222,   134,   139,   113,   135,
     140,    73,   141,   200,   200,    77,    78,   142,   147,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   146,  -128,   218,   148,   159,   157,   149,   150,
     151,   219,   154,   155,   156,   158,   160,   164,   203,   168,
     211,   212,   222,   216,   217,   227,   224,   238,   239,   240,
     242,     1,     2,     3,     4,     5,     6,   243,   244,   250,
     245,   248,     7,     8,   251,   221,   184,   186,   191,   185,
     236,   237,   192,     9,   199,    10,    81,     0,   193,     0,
       0,     0,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,   249,
      24,    25,    26,     0,     0,    27,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,   161,    37,     0,    38,
       1,     2,     3,     4,     5,     6,     0,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     0,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,    24,
      25,    26,     0,     0,    27,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,   166,    37,     0,    38,     1,
       2,     3,     4,     5,     6,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,     0,     0,    27,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,   176,    37,     0,    38,     1,     2,
       3,     4,     5,     6,     0,     0,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       9,     0,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,     0,    24,    25,    26,
       0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,   215,    37,     0,    38,     1,     2,     3,
       4,     5,     6,     0,     0,     0,     0,     0,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,    24,    25,    26,     0,
       0,    27,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,   223,    37,     0,    38,     1,     2,     3,     4,
       5,     6,     0,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     9,     0,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     0,    24,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,     0,    37,     0,    38,    72,     2,     3,     4,     5,
       6,     0,     0,     0,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
    -129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    72,     2,     3,     4,     5,     6,     0,
       0,     0,     0,     0,     7,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    72,     2,     3,     4,     5,     6,     0,     0,     0,
       0,     0,     7,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     0,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    72,
       2,     3,     4,     5,     6,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23
};

static const yytype_int16 yycheck[] =
{
       6,    40,    36,     9,     3,    44,    44,    45,    28,    45,
      48,     3,    48,     9,    10,    13,     3,     3,     4,     5,
       6,     7,     8,     3,   135,    31,     0,   138,    14,    15,
      55,    56,    25,    31,    32,    27,    55,    56,    34,    25,
      27,    27,    26,    68,    28,     3,    26,    24,    68,    68,
      51,    28,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     3,    52,    53,    54,    68,
     109,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      27,    67,    68,    69,   118,    71,   211,   212,    27,    95,
     124,    16,    17,    18,    19,    20,    21,    22,   144,   145,
     146,   136,   137,     3,   110,   111,    26,    26,    28,    28,
     142,   143,   149,   150,   120,     3,   241,    25,    25,    67,
     126,   127,   128,   129,   130,   131,   251,   165,   167,   165,
      25,    67,    67,    27,    33,   174,    23,    35,   177,    24,
      12,   147,    11,   149,   150,     7,     8,    36,    29,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    37,    28,   170,    26,    67,    25,    28,    28,
      26,    51,    27,    27,    27,    27,    27,    70,    26,    71,
      26,    26,   221,    25,    27,    24,    30,    51,     3,     3,
       3,     3,     4,     5,     6,     7,     8,     3,    26,     3,
      27,    27,    14,    15,    26,   173,   132,   134,   139,   133,
     216,   217,   140,    25,   147,    27,     9,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   245,
      52,    53,    54,    -1,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    71,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      53,    54,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    67,    68,    69,    -1,    71,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    52,    53,
      54,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    68,    69,    -1,    71,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    53,    54,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    -1,    71,     3,     4,     5,     6,     7,
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
      60,    61,    62,    63,    64,    65,    67,    69,    71,    73,
      74,    75,    76,    77,    78,    79,    80,    82,    83,    84,
      89,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     118,    51,     3,    99,   115,   116,   117,   113,   113,    26,
      99,   116,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,    25,     3,    27,     3,    27,
       3,     3,    27,    99,    27,    27,     3,     3,    68,    74,
      25,    25,     0,    75,    75,    76,    76,    77,    67,    81,
      25,    76,    77,    67,    67,    27,    16,    17,    18,    19,
      20,    21,    22,    33,    23,    24,     9,    10,    34,    35,
      12,    11,    36,    13,    31,    32,    37,    29,    26,    28,
      28,    26,    26,    99,    27,    27,    27,    25,    27,    67,
      27,    68,    99,    99,    70,    95,    68,    74,    71,    99,
      55,    56,    68,    85,    86,    87,    68,    74,    99,    99,
      99,    99,    99,    99,   101,   102,   103,   104,   106,   106,
     104,   107,   108,   109,   110,   110,   112,   112,   112,   115,
      99,   117,   117,    26,     3,    26,    92,    93,     3,    68,
      94,    26,    26,    76,    77,    68,    25,    27,    99,    51,
      68,    86,    75,    68,    30,    26,    28,    24,    88,    90,
      26,    28,    28,    68,    88,    88,    99,    99,    51,     3,
       3,    91,     3,     3,    26,    27,    28,    88,    27,    99,
       3,    26,    88
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    75,    75,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    77,    77,    77,    77,    78,    79,    80,    81,
      81,    82,    83,    84,    85,    85,    86,    86,    87,    87,
      88,    88,    89,    90,    90,    91,    91,    92,    92,    92,
      93,    93,    94,    94,    95,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    98,    98,    99,
      99,    99,    99,    99,    99,    99,    99,   100,   100,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   105,
     106,   106,   107,   107,   108,   108,   109,   109,   110,   110,
     111,   111,   111,   112,   112,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   115,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   118,   118,   118,   118
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       4,     2,     7,     2,     4,     3,     4,     3,     5,     4,
       3,     1,     2,     4,     2,     2,     5,     5,     1,     3,
       2,     1,     9,     4,     1,     2,     1,     2,     3,     2,
       0,     1,     5,     1,     3,     1,     3,     1,     2,     2,
       3,     3,     1,     3,     1,     2,     2,     1,     3,     2,
       2,     2,     3,     2,     3,     2,     3,     3,     2,     3,
       2,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     4,     1,     1,     1,     0,     3,     3,     1,     0,
       3,     1,     1,     1,     1,     1,     3,     3,     2
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
#line 227 "snocone_parse.y"
                                        { sc_finalize_if_else_pst(st, (yyvsp[-3].ifhead), (yyvsp[-1].markv)); }
#line 1613 "snocone_parse.tab.c"
    break;

  case 11: /* matched_stmt: while_head matched_stmt  */
#line 229 "snocone_parse.y"
                                        { sc_finalize_while_pst(st, (yyvsp[-1].whilehead), (yyvsp[-1].whilehead)->cond); }
#line 1619 "snocone_parse.tab.c"
    break;

  case 12: /* matched_stmt: do_head do_body T_WHILE T_LPAREN expr0 T_RPAREN T_SEMICOLON  */
#line 231 "snocone_parse.y"
                                        { sc_finalize_do_while_pst(st, (yyvsp[-6].dohead), (yyvsp[-2].expr)); }
#line 1625 "snocone_parse.tab.c"
    break;

  case 13: /* matched_stmt: for_head matched_stmt  */
#line 233 "snocone_parse.y"
                                        { sc_finalize_for_pst(st, (yyvsp[-1].forhead)); }
#line 1631 "snocone_parse.tab.c"
    break;

  case 14: /* matched_stmt: func_head T_LBRACE stmt_list T_RBRACE  */
#line 235 "snocone_parse.y"
                                        { sc_finalize_function_pst(st, (yyvsp[-3].funchead)); }
#line 1637 "snocone_parse.tab.c"
    break;

  case 15: /* matched_stmt: func_head T_LBRACE T_RBRACE  */
#line 237 "snocone_parse.y"
                                        { sc_finalize_function_pst(st, (yyvsp[-2].funchead)); }
#line 1643 "snocone_parse.tab.c"
    break;

  case 16: /* matched_stmt: switch_head T_LBRACE switch_body T_RBRACE  */
#line 239 "snocone_parse.y"
                                        { sc_finalize_switch_pst(st, (yyvsp[-3].switchhead)); }
#line 1649 "snocone_parse.tab.c"
    break;

  case 17: /* matched_stmt: switch_head T_LBRACE T_RBRACE  */
#line 241 "snocone_parse.y"
                                        { sc_finalize_switch_pst(st, (yyvsp[-2].switchhead)); }
#line 1655 "snocone_parse.tab.c"
    break;

  case 18: /* matched_stmt: T_STRUCT T_IDENT T_LBRACE struct_field_list T_RBRACE  */
#line 243 "snocone_parse.y"
                                        { sc_emit_struct(st, (yyvsp[-3].str), (yyvsp[-1].expr)); ct_drop((yyvsp[-3].str)); }
#line 1661 "snocone_parse.tab.c"
    break;

  case 19: /* matched_stmt: T_STRUCT T_IDENT T_LBRACE T_RBRACE  */
#line 245 "snocone_parse.y"
                                        { sc_emit_struct(st, (yyvsp[-2].str), ast_node_new(TT_FIELDS)); ct_drop((yyvsp[-2].str)); }
#line 1667 "snocone_parse.tab.c"
    break;

  case 20: /* matched_stmt: T_IMPORT T_IDENT T_SEMICOLON  */
#line 247 "snocone_parse.y"
                                        { sc_emit_import(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1673 "snocone_parse.tab.c"
    break;

  case 22: /* unmatched_stmt: if_head stmt  */
#line 252 "snocone_parse.y"
                                        { sc_finalize_if_no_else_pst(st, (yyvsp[-1].ifhead)); }
#line 1679 "snocone_parse.tab.c"
    break;

  case 23: /* unmatched_stmt: if_head matched_stmt else_keyword unmatched_stmt  */
#line 254 "snocone_parse.y"
                                        { sc_finalize_if_else_pst(st, (yyvsp[-3].ifhead), (yyvsp[-1].markv)); }
#line 1685 "snocone_parse.tab.c"
    break;

  case 24: /* unmatched_stmt: while_head unmatched_stmt  */
#line 256 "snocone_parse.y"
                                        { sc_finalize_while_pst(st, (yyvsp[-1].whilehead), (yyvsp[-1].whilehead)->cond); }
#line 1691 "snocone_parse.tab.c"
    break;

  case 25: /* unmatched_stmt: for_head unmatched_stmt  */
#line 258 "snocone_parse.y"
                                        { sc_finalize_for_pst(st, (yyvsp[-1].forhead)); }
#line 1697 "snocone_parse.tab.c"
    break;

  case 26: /* if_head: T_IF T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 261 "snocone_parse.y"
                                        { (yyval.ifhead) = sc_if_head_new(st, (yyvsp[-2].expr)); }
#line 1703 "snocone_parse.tab.c"
    break;

  case 27: /* while_head: T_WHILE T_LPAREN expr0 T_RPAREN opt_head_sep  */
#line 264 "snocone_parse.y"
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          struct WhileHead *wh = ct_zalloc(1, sizeof *wh);
                                          wh->cond        = (yyvsp[-2].expr);
                                          wh->mark        = st->block->n;
                                          (yyval.whilehead) = wh; }
#line 1713 "snocone_parse.tab.c"
    break;

  case 28: /* do_head: T_DO  */
#line 270 "snocone_parse.y"
                                    { sc_loop_push(st, NULL, NULL, 1);
                                      struct DoHead *dh = ct_zalloc(1, sizeof *dh);
                                      dh->mark = st->block->n;
                                      (yyval.dohead) = dh; }
#line 1722 "snocone_parse.tab.c"
    break;

  case 31: /* for_lead: T_FOR  */
#line 278 "snocone_parse.y"
                                     { }
#line 1728 "snocone_parse.tab.c"
    break;

  case 32: /* for_head: for_lead T_LPAREN expr0 T_SEMICOLON expr0 T_SEMICOLON expr0 T_RPAREN opt_head_sep  */
#line 281 "snocone_parse.y"
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          (yyval.forhead) = sc_for_head_new_pst(st, (yyvsp[-6].expr), (yyvsp[-4].expr), (yyvsp[-2].expr), st->block->n); }
#line 1735 "snocone_parse.tab.c"
    break;

  case 33: /* switch_head: T_SWITCH T_LPAREN expr0 T_RPAREN  */
#line 285 "snocone_parse.y"
                                        { (yyval.switchhead) = sc_switch_head_new(st, (yyvsp[-1].expr)); }
#line 1741 "snocone_parse.tab.c"
    break;

  case 38: /* case_or_default_label: T_CASE expr0 T_COLON  */
#line 294 "snocone_parse.y"
                                        { sc_switch_case_label(st, (yyvsp[-1].expr)); }
#line 1747 "snocone_parse.tab.c"
    break;

  case 39: /* case_or_default_label: T_DEFAULT T_COLON  */
#line 295 "snocone_parse.y"
                                        { sc_switch_default_label(st); }
#line 1753 "snocone_parse.tab.c"
    break;

  case 42: /* func_head: T_DEFINE T_IDENT T_LPAREN func_arglist func_locals  */
#line 302 "snocone_parse.y"
                                        { (yyval.funchead) = sc_func_head_new_pst(st, (yyvsp[-3].str), (yyvsp[-1].expr), (yyvsp[0].expr)); ct_drop((yyvsp[-3].str)); }
#line 1759 "snocone_parse.tab.c"
    break;

  case 43: /* func_locals: opt_head_sep  */
#line 305 "snocone_parse.y"
                                                        { (yyval.expr) = ast_node_new(TT_LOCALS); }
#line 1765 "snocone_parse.tab.c"
    break;

  case 44: /* func_locals: opt_head_sep func_locals_ne opt_head_sep  */
#line 306 "snocone_parse.y"
                                                        { (yyval.expr) = (yyvsp[-1].expr); }
#line 1771 "snocone_parse.tab.c"
    break;

  case 45: /* func_locals_ne: T_IDENT  */
#line 309 "snocone_parse.y"
                                       { (yyval.expr) = sc_list_one(TT_LOCALS, (yyvsp[0].str)); }
#line 1777 "snocone_parse.tab.c"
    break;

  case 46: /* func_locals_ne: func_locals_ne T_COMMA T_IDENT  */
#line 311 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1783 "snocone_parse.tab.c"
    break;

  case 47: /* func_arglist: T_RPAREN  */
#line 314 "snocone_parse.y"
                                       { (yyval.expr) = ast_node_new(TT_PARAMS); }
#line 1789 "snocone_parse.tab.c"
    break;

  case 48: /* func_arglist: T_IDENT T_RPAREN  */
#line 315 "snocone_parse.y"
                                       { (yyval.expr) = sc_list_one(TT_PARAMS, (yyvsp[-1].str)); }
#line 1795 "snocone_parse.tab.c"
    break;

  case 49: /* func_arglist: func_arglist_ne T_RPAREN  */
#line 316 "snocone_parse.y"
                                       { (yyval.expr) = (yyvsp[-1].expr); }
#line 1801 "snocone_parse.tab.c"
    break;

  case 50: /* func_arglist_ne: T_IDENT T_COMMA T_IDENT  */
#line 320 "snocone_parse.y"
                { (yyval.expr) = sc_list_add(sc_list_one(TT_PARAMS, (yyvsp[-2].str)), (yyvsp[0].str)); }
#line 1807 "snocone_parse.tab.c"
    break;

  case 51: /* func_arglist_ne: func_arglist_ne T_COMMA T_IDENT  */
#line 322 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1813 "snocone_parse.tab.c"
    break;

  case 52: /* struct_field_list: T_IDENT  */
#line 326 "snocone_parse.y"
                { (yyval.expr) = sc_list_one(TT_FIELDS, (yyvsp[0].str)); }
#line 1819 "snocone_parse.tab.c"
    break;

  case 53: /* struct_field_list: struct_field_list T_COMMA T_IDENT  */
#line 328 "snocone_parse.y"
                { (yyval.expr) = sc_list_add((yyvsp[-2].expr), (yyvsp[0].str)); }
#line 1825 "snocone_parse.tab.c"
    break;

  case 54: /* else_keyword: T_ELSE  */
#line 331 "snocone_parse.y"
                                     { (yyval.markv) = st->block->n; }
#line 1831 "snocone_parse.tab.c"
    break;

  case 55: /* label_decl: T_IDENT T_COLON  */
#line 334 "snocone_parse.y"
                                     { sc_append_label_node(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1837 "snocone_parse.tab.c"
    break;

  case 56: /* simple_stmt: expr0 T_SEMICOLON  */
#line 336 "snocone_parse.y"
                                               { sc_append_stmt(st, (yyvsp[-1].expr)); }
#line 1843 "snocone_parse.tab.c"
    break;

  case 57: /* simple_stmt: T_SEMICOLON  */
#line 337 "snocone_parse.y"
                                               {         }
#line 1849 "snocone_parse.tab.c"
    break;

  case 58: /* simple_stmt: T_RETURN expr0 T_SEMICOLON  */
#line 338 "snocone_parse.y"
                                            { tree_t *r = ast_node_new(TT_RETURN); ast_push(r, (yyvsp[-1].expr));
                                             sc_append_stmt(st, r); }
#line 1856 "snocone_parse.tab.c"
    break;

  case 59: /* simple_stmt: T_RETURN T_SEMICOLON  */
#line 340 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_RETURN)); }
#line 1862 "snocone_parse.tab.c"
    break;

  case 60: /* simple_stmt: T_FRETURN T_SEMICOLON  */
#line 341 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_PROC_FAIL)); }
#line 1868 "snocone_parse.tab.c"
    break;

  case 61: /* simple_stmt: T_NRETURN T_SEMICOLON  */
#line 342 "snocone_parse.y"
                                            { sc_append_stmt(st, ast_node_new(TT_NRETURN)); }
#line 1874 "snocone_parse.tab.c"
    break;

  case 62: /* simple_stmt: T_GOTO T_IDENT T_SEMICOLON  */
#line 343 "snocone_parse.y"
                                            { tree_t *g = ast_node_new(TT_GOTO_U); ast_push(g, sc_qlit((yyvsp[-1].str))); ct_drop((yyvsp[-1].str)); sc_append_stmt(st, g); }
#line 1880 "snocone_parse.tab.c"
    break;

  case 63: /* simple_stmt: T_BREAK T_SEMICOLON  */
#line 344 "snocone_parse.y"
                                            { sc_append_break(st, NULL); }
#line 1886 "snocone_parse.tab.c"
    break;

  case 64: /* simple_stmt: T_BREAK T_IDENT T_SEMICOLON  */
#line 345 "snocone_parse.y"
                                            { sc_append_break(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1892 "snocone_parse.tab.c"
    break;

  case 65: /* simple_stmt: T_CONTINUE T_SEMICOLON  */
#line 346 "snocone_parse.y"
                                            { sc_append_continue(st, NULL); }
#line 1898 "snocone_parse.tab.c"
    break;

  case 66: /* simple_stmt: T_CONTINUE T_IDENT T_SEMICOLON  */
#line 347 "snocone_parse.y"
                                             { sc_append_continue(st, (yyvsp[-1].str)); ct_drop((yyvsp[-1].str)); }
#line 1904 "snocone_parse.tab.c"
    break;

  case 67: /* block_stmt: T_LBRACE stmt_list T_RBRACE  */
#line 349 "snocone_parse.y"
                                               { }
#line 1910 "snocone_parse.tab.c"
    break;

  case 68: /* block_stmt: T_LBRACE T_RBRACE  */
#line 350 "snocone_parse.y"
                                               {                  }
#line 1916 "snocone_parse.tab.c"
    break;

  case 69: /* expr0: expr1 T_2EQUAL expr0  */
#line 353 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1922 "snocone_parse.tab.c"
    break;

  case 70: /* expr0: expr1 T_2EQUAL  */
#line 355 "snocone_parse.y"
                                { tree_t *empty = expr_new(TT_QLIT);
                                  empty->sval = ct_strdup("");
                                  (yyval.expr) = expr_binary(TT_ASSIGN, (yyvsp[-1].expr), empty); }
#line 1930 "snocone_parse.tab.c"
    break;

  case 71: /* expr0: expr1 T_PLUS_ASSIGN expr0  */
#line 359 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPLUS;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1937 "snocone_parse.tab.c"
    break;

  case 72: /* expr0: expr1 T_MINUS_ASSIGN expr0  */
#line 362 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGMINUS;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1944 "snocone_parse.tab.c"
    break;

  case 73: /* expr0: expr1 T_STAR_ASSIGN expr0  */
#line 365 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSTAR;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1951 "snocone_parse.tab.c"
    break;

  case 74: /* expr0: expr1 T_SLASH_ASSIGN expr0  */
#line 368 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSLASH;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1958 "snocone_parse.tab.c"
    break;

  case 75: /* expr0: expr1 T_CARET_ASSIGN expr0  */
#line 371 "snocone_parse.y"
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPOW;
                                  ast_push(a, (yyvsp[-2].expr)); ast_push(a, (yyvsp[0].expr)); (yyval.expr) = a; }
#line 1965 "snocone_parse.tab.c"
    break;

  case 76: /* expr0: expr1  */
#line 374 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1971 "snocone_parse.tab.c"
    break;

  case 77: /* expr1: expr1 T_2QUEST expr2  */
#line 377 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SCAN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1977 "snocone_parse.tab.c"
    break;

  case 78: /* expr1: expr2  */
#line 379 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1983 "snocone_parse.tab.c"
    break;

  case 79: /* expr2: expr2 T_2AMP expr3  */
#line 382 "snocone_parse.y"
                                { tree_t *e = expr_binary(TT_OPSYN, (yyvsp[-2].expr), (yyvsp[0].expr)); e->sval = ct_strdup("&"); (yyval.expr) = e; }
#line 1989 "snocone_parse.tab.c"
    break;

  case 80: /* expr2: expr3  */
#line 384 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1995 "snocone_parse.tab.c"
    break;

  case 81: /* expr3: expr3 T_2PIPE expr4  */
#line 387 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ALT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2001 "snocone_parse.tab.c"
    break;

  case 82: /* expr3: expr4  */
#line 389 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2007 "snocone_parse.tab.c"
    break;

  case 83: /* expr4: expr4 T_CONCAT expr5  */
#line 392 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2013 "snocone_parse.tab.c"
    break;

  case 84: /* expr4: expr5  */
#line 394 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2019 "snocone_parse.tab.c"
    break;

  case 85: /* expr5: expr6 T_2AT expr5  */
#line 397 "snocone_parse.y"
                                { tree_t *e = expr_binary(TT_OPSYN, (yyvsp[-2].expr), (yyvsp[0].expr)); e->sval = ct_strdup("@"); (yyval.expr) = e; }
#line 2025 "snocone_parse.tab.c"
    break;

  case 86: /* expr5: expr6  */
#line 399 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2031 "snocone_parse.tab.c"
    break;

  case 87: /* expr6: expr6 T_2PLUS expr7  */
#line 402 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2037 "snocone_parse.tab.c"
    break;

  case 88: /* expr6: expr6 T_2MINUS expr7  */
#line 404 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2043 "snocone_parse.tab.c"
    break;

  case 89: /* expr6: expr7  */
#line 406 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2049 "snocone_parse.tab.c"
    break;

  case 90: /* expr7: expr7 T_2POUND expr8  */
#line 409 "snocone_parse.y"
                                { tree_t *e = expr_binary(TT_OPSYN, (yyvsp[-2].expr), (yyvsp[0].expr)); e->sval = ct_strdup("#"); (yyval.expr) = e; }
#line 2055 "snocone_parse.tab.c"
    break;

  case 91: /* expr7: expr8  */
#line 411 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2061 "snocone_parse.tab.c"
    break;

  case 92: /* expr8: expr8 T_2SLASH expr9  */
#line 414 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2067 "snocone_parse.tab.c"
    break;

  case 93: /* expr8: expr9  */
#line 416 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2073 "snocone_parse.tab.c"
    break;

  case 94: /* expr9: expr9 T_2STAR expr10  */
#line 419 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2079 "snocone_parse.tab.c"
    break;

  case 95: /* expr9: expr10  */
#line 421 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2085 "snocone_parse.tab.c"
    break;

  case 96: /* expr10: expr10 T_2PERCENT expr11  */
#line 424 "snocone_parse.y"
                                { tree_t *e = expr_binary(TT_OPSYN, (yyvsp[-2].expr), (yyvsp[0].expr)); e->sval = ct_strdup("%"); (yyval.expr) = e; }
#line 2091 "snocone_parse.tab.c"
    break;

  case 97: /* expr10: expr11  */
#line 426 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2097 "snocone_parse.tab.c"
    break;

  case 98: /* expr11: expr12 T_2CARET expr11  */
#line 429 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_POW, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2103 "snocone_parse.tab.c"
    break;

  case 99: /* expr11: expr12  */
#line 431 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2109 "snocone_parse.tab.c"
    break;

  case 100: /* expr12: expr12 T_2DOLLAR expr13  */
#line 434 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_CAPT_IMMED_ASGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2115 "snocone_parse.tab.c"
    break;

  case 101: /* expr12: expr12 T_2DOT expr13  */
#line 436 "snocone_parse.y"
                                { (yyval.expr) = expr_binary(TT_CAPT_COND_ASGN,  (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2121 "snocone_parse.tab.c"
    break;

  case 102: /* expr12: expr13  */
#line 438 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2127 "snocone_parse.tab.c"
    break;

  case 103: /* expr13: expr14 T_2TILDE expr13  */
#line 441 "snocone_parse.y"
                                { tree_t *e = expr_binary(TT_OPSYN, (yyvsp[-2].expr), (yyvsp[0].expr)); e->sval = ct_strdup("~"); (yyval.expr) = e; }
#line 2133 "snocone_parse.tab.c"
    break;

  case 104: /* expr13: expr14  */
#line 443 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2139 "snocone_parse.tab.c"
    break;

  case 105: /* expr14: T_1PLUS expr14  */
#line 446 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_PLS, (yyvsp[0].expr)); }
#line 2145 "snocone_parse.tab.c"
    break;

  case 106: /* expr14: T_1MINUS expr14  */
#line 448 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_MNS, (yyvsp[0].expr)); }
#line 2151 "snocone_parse.tab.c"
    break;

  case 107: /* expr14: T_1STAR expr14  */
#line 449 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_DEFER,       (yyvsp[0].expr)); }
#line 2157 "snocone_parse.tab.c"
    break;

  case 108: /* expr14: T_1DOT expr14  */
#line 450 "snocone_parse.y"
                                { if ((yyvsp[0].expr) && ((yyvsp[0].expr)->t == TT_ILIT || (yyvsp[0].expr)->t == TT_FLIT || (yyvsp[0].expr)->t == TT_QLIT)) sc_error(st, "value used where name is required"); (yyval.expr) = expr_unary(TT_NAME, (yyvsp[0].expr)); }
#line 2163 "snocone_parse.tab.c"
    break;

  case 109: /* expr14: T_1DOLLAR expr14  */
#line 451 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_INDIRECT,    (yyvsp[0].expr)); }
#line 2169 "snocone_parse.tab.c"
    break;

  case 110: /* expr14: T_1AT expr14  */
#line 452 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_CAPT_CURSOR, (yyvsp[0].expr)); }
#line 2175 "snocone_parse.tab.c"
    break;

  case 111: /* expr14: T_1TILDE expr14  */
#line 453 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_NOT,         (yyvsp[0].expr)); }
#line 2181 "snocone_parse.tab.c"
    break;

  case 112: /* expr14: T_1QUEST expr14  */
#line 454 "snocone_parse.y"
                                { (yyval.expr) = expr_unary(TT_INTERROGATE, (yyvsp[0].expr)); }
#line 2187 "snocone_parse.tab.c"
    break;

  case 113: /* expr14: T_1AMP expr14  */
#line 455 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("&"); (yyval.expr) = _e; }
#line 2194 "snocone_parse.tab.c"
    break;

  case 114: /* expr14: T_1PERCENT expr14  */
#line 457 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("%"); (yyval.expr) = _e; }
#line 2201 "snocone_parse.tab.c"
    break;

  case 115: /* expr14: T_1SLASH expr14  */
#line 459 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("/"); (yyval.expr) = _e; }
#line 2208 "snocone_parse.tab.c"
    break;

  case 116: /* expr14: T_1POUND expr14  */
#line 461 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("#"); (yyval.expr) = _e; }
#line 2215 "snocone_parse.tab.c"
    break;

  case 117: /* expr14: T_1PIPE expr14  */
#line 463 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("|"); (yyval.expr) = _e; }
#line 2222 "snocone_parse.tab.c"
    break;

  case 118: /* expr14: T_1EQUAL expr14  */
#line 465 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("="); (yyval.expr) = _e; }
#line 2229 "snocone_parse.tab.c"
    break;

  case 119: /* expr14: T_1BANG expr14  */
#line 467 "snocone_parse.y"
                                { tree_t *_e = expr_unary(TT_OPSYN, (yyvsp[0].expr));
                                  _e->sval = ct_strdup("!"); (yyval.expr) = _e; }
#line 2236 "snocone_parse.tab.c"
    break;

  case 120: /* expr14: expr15  */
#line 470 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2242 "snocone_parse.tab.c"
    break;

  case 121: /* expr15: expr15 T_LBRACK exprlist T_RBRACK  */
#line 473 "snocone_parse.y"
                                { tree_t *idx = expr_new(TT_IDX);
                                  tree_t *sub = expr_new(TT_VLIST);
                                  expr_add_child(idx, (yyvsp[-3].expr));
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(sub, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  expr_add_child(idx, sub);
                                  (yyval.expr) = idx; }
#line 2255 "snocone_parse.tab.c"
    break;

  case 122: /* expr15: expr17  */
#line 482 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2261 "snocone_parse.tab.c"
    break;

  case 123: /* exprlist: exprlist_c  */
#line 485 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2267 "snocone_parse.tab.c"
    break;

  case 124: /* exprlist: expr0  */
#line 487 "snocone_parse.y"
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 2273 "snocone_parse.tab.c"
    break;

  case 125: /* exprlist: %empty  */
#line 489 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2279 "snocone_parse.tab.c"
    break;

  case 126: /* exprlist_c: exprlist_c T_COMMA optexpr  */
#line 492 "snocone_parse.y"
                                { expr_add_child((yyvsp[-2].expr), (yyvsp[0].expr)); (yyval.expr) = (yyvsp[-2].expr); }
#line 2285 "snocone_parse.tab.c"
    break;

  case 127: /* exprlist_c: optexpr T_COMMA optexpr  */
#line 494 "snocone_parse.y"
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, (yyvsp[-2].expr)); expr_add_child(l, (yyvsp[0].expr)); (yyval.expr) = l; }
#line 2291 "snocone_parse.tab.c"
    break;

  case 128: /* optexpr: expr0  */
#line 497 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2297 "snocone_parse.tab.c"
    break;

  case 129: /* optexpr: %empty  */
#line 499 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2303 "snocone_parse.tab.c"
    break;

  case 130: /* expr17: T_CALL exprlist T_RPAREN  */
#line 502 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_FNC);
                                  tree_t *a = expr_new(TT_ARGS);
                                  expr_add_child(e, sc_qlit((yyvsp[-2].str))); ct_drop((yyvsp[-2].str));
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(a, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  expr_add_child(e, a);
                                  (yyval.expr) = e; }
#line 2316 "snocone_parse.tab.c"
    break;

  case 131: /* expr17: T_IDENT  */
#line 511 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_VAR);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 2324 "snocone_parse.tab.c"
    break;

  case 132: /* expr17: T_KEYWORD  */
#line 515 "snocone_parse.y"
                                { tree_t *e = expr_new(TT_KEYWORD);
                                  e->sval = (yyvsp[0].str);
                                  (yyval.expr) = e; }
#line 2332 "snocone_parse.tab.c"
    break;

  case 133: /* expr17: T_INT  */
#line 519 "snocone_parse.y"
                                { (yyval.expr) = sc_int_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2338 "snocone_parse.tab.c"
    break;

  case 134: /* expr17: T_REAL  */
#line 521 "snocone_parse.y"
                                { (yyval.expr) = sc_real_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2344 "snocone_parse.tab.c"
    break;

  case 135: /* expr17: T_STR  */
#line 523 "snocone_parse.y"
                                { (yyval.expr) = sc_str_literal((yyvsp[0].str)); ct_drop((yyvsp[0].str)); }
#line 2350 "snocone_parse.tab.c"
    break;

  case 136: /* expr17: T_LPAREN expr0 T_RPAREN  */
#line 525 "snocone_parse.y"
                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 2356 "snocone_parse.tab.c"
    break;

  case 137: /* expr17: T_LPAREN exprlist_c T_RPAREN  */
#line 527 "snocone_parse.y"
                                { tree_t *a = expr_new(TT_VLIST);
                                  for (int i = 0; i < (yyvsp[-1].expr)->nchildren; i++)
                                      expr_add_child(a, (yyvsp[-1].expr)->children[i]);
                                  if ((yyvsp[-1].expr)->c) ct_drop((char*)(yyvsp[-1].expr)->c - sizeof(size_t)); ct_drop((yyvsp[-1].expr));
                                  (yyval.expr) = a; }
#line 2366 "snocone_parse.tab.c"
    break;

  case 138: /* expr17: T_LPAREN T_RPAREN  */
#line 533 "snocone_parse.y"
                                { (yyval.expr) = expr_new(TT_NUL); }
#line 2372 "snocone_parse.tab.c"
    break;


#line 2376 "snocone_parse.tab.c"

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

#line 535 "snocone_parse.y"

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
static void sc_emit_import(ScParseState *st, char *name) {
    tree_t *node = ast_node_new(TT_IMPORT);
    ast_push(node, sc_qlit(name));
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
