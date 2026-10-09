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
#define YYSTYPE         PASCAL_YYSTYPE
/* Substitute the variable and function names.  */
#define yyparse         pascal_yyparse
#define yylex           pascal_yylex
#define yyerror         pascal_yyerror
#define yydebug         pascal_yydebug
#define yynerrs         pascal_yynerrs
#define yylval          pascal_yylval
#define yychar          pascal_yychar

/* First part of user prologue.  */
#line 6 "pascal.y"

#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
extern void pascal_pragma_flush(tree_t *root);
#define PAS_STMT_STACK 512
static int pas_stmt_line_stk[PAS_STMT_STACK];
static int pas_stmt_sp = 0;
static void pas_stmt_mark(void) { if (pas_stmt_sp < PAS_STMT_STACK) pas_stmt_line_stk[pas_stmt_sp] = -1; pas_stmt_sp++; }
static void pas_stmt_line_fill(void) { if (pas_stmt_sp > 0 && pas_stmt_sp <= PAS_STMT_STACK && pas_stmt_line_stk[pas_stmt_sp - 1] < 0) pas_stmt_line_stk[pas_stmt_sp - 1] = pascal_get_lineno(); }
static int  pas_stmt_line_pop(void) { int l = -1; if (pas_stmt_sp > 0) { pas_stmt_sp--; if (pas_stmt_sp < PAS_STMT_STACK) l = pas_stmt_line_stk[pas_stmt_sp]; } return l >= 0 ? l : pascal_get_lineno(); }
int pascal_lex_wrapped(void) { int t = pascal_yylex(); pas_stmt_line_fill(); return t; }
#define pascal_yylex pascal_lex_wrapped
void pascal_yyerror(const char *msg) { fprintf(stderr, "pascal parse error line %d: %s\n", pascal_get_lineno(), msg); }
tree_t   *pascal_prog_result = NULL;
static tree_t *pt_new(tree_e k) { return ast_node_new(k); }
static tree_t *pt_add(tree_t *p, tree_t *c) { if (p && c) ast_push(p, c); return p; }
static tree_t *pt_cat(tree_t *p, tree_t *l) { if (p && l) for (int i = 0; i < l->n; i++) ast_push(p, l->c[i]); return p; }
static tree_t *pt_retag(tree_t *n, tree_e k) { if (n) n->t = k; return n; }
static tree_t *pt_sval(tree_t *n, const char *s) { if (n) n->v.sval = ct_strdup(s); return n; }
static tree_t *pt_kw(const char *w) { return pt_sval(pt_new(TT_KEYWORD), w); }
static tree_t *pt_part(const char *w, tree_t *first) { return pt_add(pt_sval(pt_new(TT_PART), w), first); }
static tree_t *pt_decl(const char *w, tree_t *a, tree_t *b) { return pt_add(pt_add(pt_sval(pt_new(TT_DECL), w), a), b); }
static void pt_flush_pragmas(tree_t *root) { pascal_pragma_flush(root); }
typedef struct { char *name; int kind; long long iv; double dv; char *sv; } PasCEnvE;
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; } g_pascal_cenv;
static void cenv_put(const char *name, int kind, long long iv, double dv, const char *sv) {
    if (!name || (kind == 0 && g_pascal_cenv.ni >= 256) || (kind == 1 && g_pascal_cenv.nr >= 64) || (kind == 2 && g_pascal_cenv.ns >= 64)) return;
    if (g_pascal_cenv.n >= g_pascal_cenv.cap) { g_pascal_cenv.cap = g_pascal_cenv.cap ? g_pascal_cenv.cap * 2 : 64; g_pascal_cenv.e = (PasCEnvE *)ct_grow(g_pascal_cenv.e, (size_t)g_pascal_cenv.cap * sizeof(PasCEnvE)); }
    PasCEnvE *c = &g_pascal_cenv.e[g_pascal_cenv.n++]; c->name = ct_strdup(name); c->kind = kind; c->iv = iv; c->dv = dv; c->sv = sv ? ct_strdup(sv) : NULL;
    if (kind == 0) g_pascal_cenv.ni++; else if (kind == 1) g_pascal_cenv.nr++; else g_pascal_cenv.ns++;
}
static void cenv_int(const char *name, long long v) { cenv_put(name, 0, v, 0, NULL); }
static void cenv_real(const char *name, double v) { cenv_put(name, 1, 0, v, NULL); }
static void cenv_string(const char *name, const char *s) { if (s && strlen(s) == 1) cenv_int(name, (long long)(unsigned char)s[0]); else if (s) cenv_put(name, 2, 0, 0, s); }
int pascal_cenv_int(const char *name, long long *out) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 0 && !strcmp(g_pascal_cenv.e[i].name, name)) { *out = g_pascal_cenv.e[i].iv; return 1; } return 0; }
int pascal_cenv_real(const char *name, double *out) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 1 && !strcmp(g_pascal_cenv.e[i].name, name)) { *out = g_pascal_cenv.e[i].dv; return 1; } return 0; }
const char *pascal_cenv_str(const char *name) { for (int i = 0; name && i < g_pascal_cenv.n; i++) if (g_pascal_cenv.e[i].kind == 2 && !strcmp(g_pascal_cenv.e[i].name, name)) return g_pascal_cenv.e[i].sv; return NULL; }
void pascal_cenv_reset(void) { g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; }
static long long cenv_value(tree_t *n) {
    long long cv = 0;
    if (!n) return 0;
    switch (n->t) {
        case TT_PLS: return cenv_value(n->c[0]);
        case TT_MNS: return -cenv_value(n->c[0]);
        case TT_ILIT: case TT_CHRLIT: return n->v.ival;
        case TT_FLIT: return (long long)n->v.dval;
        case TT_QLIT: return (n->v.sval && strlen(n->v.sval) == 1) ? (long long)(unsigned char)n->v.sval[0] : 0;
        case TT_VAR:
            if (n->v.sval && !strcmp(n->v.sval, "true")) cv = 1;
            else if (n->v.sval && !strcmp(n->v.sval, "false")) cv = 0;
            else if (!pascal_cenv_int(n->v.sval, &cv) && n->v.sval && !strcmp(n->v.sval, "maxint")) cv = 2147483647;
            return cv;
        default: return 0;
    }
}

#line 144 "pascal.tab.c"

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

#include "pascal.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_GOTOSY = 3,                     /* GOTOSY  */
  YYSYMBOL_PROGRAMSY = 4,                  /* PROGRAMSY  */
  YYSYMBOL_SEMICOLON = 5,                  /* SEMICOLON  */
  YYSYMBOL_ARRAYSY = 6,                    /* ARRAYSY  */
  YYSYMBOL_LABELSY = 7,                    /* LABELSY  */
  YYSYMBOL_CONSTSY = 8,                    /* CONSTSY  */
  YYSYMBOL_FORWARDSY = 9,                  /* FORWARDSY  */
  YYSYMBOL_DOSY = 10,                      /* DOSY  */
  YYSYMBOL_DOWNTOSY = 11,                  /* DOWNTOSY  */
  YYSYMBOL_FORSY = 12,                     /* FORSY  */
  YYSYMBOL_REPEATSY = 13,                  /* REPEATSY  */
  YYSYMBOL_WHILESY = 14,                   /* WHILESY  */
  YYSYMBOL_TOSY = 15,                      /* TOSY  */
  YYSYMBOL_UNTILSY = 16,                   /* UNTILSY  */
  YYSYMBOL_WITHSY = 17,                    /* WITHSY  */
  YYSYMBOL_CASESY = 18,                    /* CASESY  */
  YYSYMBOL_PROCEDURESY = 19,               /* PROCEDURESY  */
  YYSYMBOL_PACKEDSY = 20,                  /* PACKEDSY  */
  YYSYMBOL_OFSY = 21,                      /* OFSY  */
  YYSYMBOL_FILESY = 22,                    /* FILESY  */
  YYSYMBOL_ENDSY = 23,                     /* ENDSY  */
  YYSYMBOL_SETSY = 24,                     /* SETSY  */
  YYSYMBOL_VARSY = 25,                     /* VARSY  */
  YYSYMBOL_THENSY = 26,                    /* THENSY  */
  YYSYMBOL_RECORDSY = 27,                  /* RECORDSY  */
  YYSYMBOL_FUNCTIONSY = 28,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 29,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 30,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 31,                    /* TYPESY  */
  YYSYMBOL_IFSY = 32,                      /* IFSY  */
  YYSYMBOL_ELSESY = 33,                    /* ELSESY  */
  YYSYMBOL_INOP = 34,                      /* INOP  */
  YYSYMBOL_NOTSY = 35,                     /* NOTSY  */
  YYSYMBOL_IDIV = 36,                      /* IDIV  */
  YYSYMBOL_IMOD = 37,                      /* IMOD  */
  YYSYMBOL_ANDOP = 38,                     /* ANDOP  */
  YYSYMBOL_OROP = 39,                      /* OROP  */
  YYSYMBOL_LTOP = 40,                      /* LTOP  */
  YYSYMBOL_LEOP = 41,                      /* LEOP  */
  YYSYMBOL_GTOP = 42,                      /* GTOP  */
  YYSYMBOL_GEOP = 43,                      /* GEOP  */
  YYSYMBOL_NEOP = 44,                      /* NEOP  */
  YYSYMBOL_EQOP = 45,                      /* EQOP  */
  YYSYMBOL_PLUS = 46,                      /* PLUS  */
  YYSYMBOL_MINUS = 47,                     /* MINUS  */
  YYSYMBOL_MUL = 48,                       /* MUL  */
  YYSYMBOL_RDIV = 49,                      /* RDIV  */
  YYSYMBOL_COMMA = 50,                     /* COMMA  */
  YYSYMBOL_PERIOD = 51,                    /* PERIOD  */
  YYSYMBOL_COLON = 52,                     /* COLON  */
  YYSYMBOL_ARROW = 53,                     /* ARROW  */
  YYSYMBOL_LBRACK = 54,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 55,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 56,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 57,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 58,                    /* DOTDOT  */
  YYSYMBOL_ATSIGN = 59,                    /* ATSIGN  */
  YYSYMBOL_INTCONST = 60,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 61,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 62,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 63,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 64,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_program = 66,                   /* program  */
  YYSYMBOL_file_id_list_opt = 67,          /* file_id_list_opt  */
  YYSYMBOL_block = 68,                     /* block  */
  YYSYMBOL_decl_part_list = 69,            /* decl_part_list  */
  YYSYMBOL_decl_part = 70,                 /* decl_part  */
  YYSYMBOL_label_list = 71,                /* label_list  */
  YYSYMBOL_const_decl_list = 72,           /* const_decl_list  */
  YYSYMBOL_const_decl = 73,                /* const_decl  */
  YYSYMBOL_constant = 74,                  /* constant  */
  YYSYMBOL_scalar_constant = 75,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 76,            /* type_decl_list  */
  YYSYMBOL_type_decl = 77,                 /* type_decl  */
  YYSYMBOL_type = 78,                      /* type  */
  YYSYMBOL_79_1 = 79,                      /* $@1  */
  YYSYMBOL_80_2 = 80,                      /* $@2  */
  YYSYMBOL_81_3 = 81,                      /* $@3  */
  YYSYMBOL_packed_opt = 82,                /* packed_opt  */
  YYSYMBOL_simple_type = 83,               /* simple_type  */
  YYSYMBOL_record_body = 84,               /* record_body  */
  YYSYMBOL_record_field_list = 85,         /* record_field_list  */
  YYSYMBOL_record_field = 86,              /* record_field  */
  YYSYMBOL_87_4 = 87,                      /* $@4  */
  YYSYMBOL_case_arm_mark = 88,             /* case_arm_mark  */
  YYSYMBOL_case_open = 89,                 /* case_open  */
  YYSYMBOL_record_case_opt = 90,           /* record_case_opt  */
  YYSYMBOL_record_case_list = 91,          /* record_case_list  */
  YYSYMBOL_record_case_arm = 92,           /* record_case_arm  */
  YYSYMBOL_var_decl_list = 93,             /* var_decl_list  */
  YYSYMBOL_var_decl = 94,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 95,            /* procedure_decl  */
  YYSYMBOL_96_5 = 96,                      /* $@5  */
  YYSYMBOL_97_6 = 97,                      /* $@6  */
  YYSYMBOL_98_7 = 98,                      /* $@7  */
  YYSYMBOL_pv_mark = 99,                   /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 100,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 101,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 102,           /* parameter_decl  */
  YYSYMBOL_pf_params = 103,                /* pf_params  */
  YYSYMBOL_pf_sections = 104,              /* pf_sections  */
  YYSYMBOL_pf_section = 105,               /* pf_section  */
  YYSYMBOL_id_list = 106,                  /* id_list  */
  YYSYMBOL_body = 107,                     /* body  */
  YYSYMBOL_statement_list = 108,           /* statement_list  */
  YYSYMBOL_109_8 = 109,                    /* $@8  */
  YYSYMBOL_110_9 = 110,                    /* $@9  */
  YYSYMBOL_body_stmt = 111,                /* body_stmt  */
  YYSYMBOL_112_10 = 112,                   /* $@10  */
  YYSYMBOL_statement = 113,                /* statement  */
  YYSYMBOL_statement_no_label = 114,       /* statement_no_label  */
  YYSYMBOL_call = 115,                     /* call  */
  YYSYMBOL_call_with_args = 116,           /* call_with_args  */
  YYSYMBOL_argument_list = 117,            /* argument_list  */
  YYSYMBOL_argument = 118,                 /* argument  */
  YYSYMBOL_assignment = 119,               /* assignment  */
  YYSYMBOL_selector = 120,                 /* selector  */
  YYSYMBOL_expression_list = 121,          /* expression_list  */
  YYSYMBOL_compound_statement = 122,       /* compound_statement  */
  YYSYMBOL_goto_statement = 123,           /* goto_statement  */
  YYSYMBOL_if_statement = 124,             /* if_statement  */
  YYSYMBOL_case_statement = 125,           /* case_statement  */
  YYSYMBOL_126_11 = 126,                   /* $@11  */
  YYSYMBOL_case_list = 127,                /* case_list  */
  YYSYMBOL_case_elem = 128,                /* case_elem  */
  YYSYMBOL_constant_list = 129,            /* constant_list  */
  YYSYMBOL_while_statement = 130,          /* while_statement  */
  YYSYMBOL_repeat_statement = 131,         /* repeat_statement  */
  YYSYMBOL_for_statement = 132,            /* for_statement  */
  YYSYMBOL_with_statement = 133,           /* with_statement  */
  YYSYMBOL_with_open = 134,                /* with_open  */
  YYSYMBOL_expression = 135,               /* expression  */
  YYSYMBOL_simple_expression = 136,        /* simple_expression  */
  YYSYMBOL_term = 137,                     /* term  */
  YYSYMBOL_factor = 138,                   /* factor  */
  YYSYMBOL_set_member_list = 139,          /* set_member_list  */
  YYSYMBOL_set_member = 140                /* set_member  */
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
         || (defined PASCAL_YYSTYPE_IS_TRIVIAL && PASCAL_YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   461

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  76
/* YYNRULES -- Number of rules.  */
#define YYNRULES  185
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  372

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,    91,    92,    95,    98,    99,   102,   103,
     104,   105,   106,   109,   110,   113,   114,   117,   118,   119,
     120,   121,   122,   125,   126,   127,   130,   131,   132,   133,
     134,   137,   138,   141,   144,   145,   146,   146,   146,   147,
     148,   148,   149,   150,   151,   154,   155,   158,   159,   160,
     161,   162,   165,   168,   169,   172,   172,   173,   176,   179,
     182,   183,   184,   187,   188,   191,   192,   195,   196,   199,
     202,   203,   204,   204,   205,   205,   206,   206,   209,   212,
     213,   216,   217,   220,   221,   222,   223,   226,   227,   230,
     231,   234,   235,   236,   237,   240,   241,   244,   247,   247,
     248,   248,   251,   251,   254,   255,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   271,   272,   275,
     278,   279,   282,   283,   284,   287,   290,   291,   292,   293,
     296,   297,   300,   303,   306,   307,   310,   310,   313,   314,
     317,   318,   321,   322,   325,   328,   331,   332,   335,   338,
     339,   346,   347,   348,   349,   350,   351,   352,   353,   356,
     357,   358,   359,   360,   361,   364,   365,   366,   367,   368,
     369,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   385,   386,   389,   390
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if PASCAL_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "GOTOSY", "PROGRAMSY",
  "SEMICOLON", "ARRAYSY", "LABELSY", "CONSTSY", "FORWARDSY", "DOSY",
  "DOWNTOSY", "FORSY", "REPEATSY", "WHILESY", "TOSY", "UNTILSY", "WITHSY",
  "CASESY", "PROCEDURESY", "PACKEDSY", "OFSY", "FILESY", "ENDSY", "SETSY",
  "VARSY", "THENSY", "RECORDSY", "FUNCTIONSY", "BEGINSY", "BECOMES",
  "TYPESY", "IFSY", "ELSESY", "INOP", "NOTSY", "IDIV", "IMOD", "ANDOP",
  "OROP", "LTOP", "LEOP", "GTOP", "GEOP", "NEOP", "EQOP", "PLUS", "MINUS",
  "MUL", "RDIV", "COMMA", "PERIOD", "COLON", "ARROW", "LBRACK", "RBRACK",
  "LPARENT", "RPARENT", "DOTDOT", "ATSIGN", "INTCONST", "CHARCODE",
  "REALCONST", "STRINGCONST", "IDENT", "$accept", "program",
  "file_id_list_opt", "block", "decl_part_list", "decl_part", "label_list",
  "const_decl_list", "const_decl", "constant", "scalar_constant",
  "type_decl_list", "type_decl", "type", "$@1", "$@2", "$@3", "packed_opt",
  "simple_type", "record_body", "record_field_list", "record_field", "$@4",
  "case_arm_mark", "case_open", "record_case_opt", "record_case_list",
  "record_case_arm", "var_decl_list", "var_decl", "procedure_decl", "$@5",
  "$@6", "$@7", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "pf_params", "pf_sections", "pf_section", "id_list",
  "body", "statement_list", "$@8", "$@9", "body_stmt", "$@10", "statement",
  "statement_no_label", "call", "call_with_args", "argument_list",
  "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@11", "case_list", "case_elem", "constant_list", "while_statement",
  "repeat_statement", "for_statement", "with_statement", "with_open",
  "expression", "simple_expression", "term", "factor", "set_member_list",
  "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-243)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-130)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      37,    32,    78,     0,  -243,    45,   105,  -243,    -8,  -243,
      61,  -243,    91,   131,  -243,  -243,    77,    84,    94,    45,
     101,  -243,   102,  -243,  -243,  -243,  -243,    14,   134,    84,
    -243,  -243,    45,  -243,   -19,  -243,    25,    26,   138,   102,
    -243,  -243,   142,   272,  -243,   164,  -243,   108,    20,  -243,
    -243,   166,   130,  -243,   213,   171,   213,  -243,   213,   186,
     168,  -243,  -243,  -243,  -243,  -243,   188,  -243,  -243,  -243,
    -243,  -243,  -243,  -243,  -243,   108,  -243,  -243,   290,   351,
    -243,   238,   242,   258,  -243,   261,  -243,    35,   273,  -243,
     356,   356,   218,    45,   227,  -243,   243,   244,   251,   284,
     234,  -243,  -243,   248,    26,  -243,   266,    63,    70,    70,
      70,   190,   213,    70,  -243,  -243,  -243,  -243,   257,  -243,
      93,   133,    73,   148,  -243,  -243,    93,    17,   250,    29,
     303,    33,   213,   213,   267,  -243,   213,   309,  -243,   310,
    -243,  -243,   325,  -243,  -243,  -243,  -243,  -243,   274,    45,
     278,    13,  -243,   143,   352,  -243,    50,   327,   327,   327,
    -243,   311,   347,   348,  -243,  -243,   306,  -243,   213,   213,
    -243,   148,   148,  -243,   263,   -18,  -243,   283,  -243,  -243,
     213,   213,   213,   213,   213,   213,   213,    70,    70,    70,
      70,    70,    70,    70,    70,  -243,   171,  -243,  -243,  -243,
    -243,    56,  -243,   315,   358,  -243,    72,   358,  -243,  -243,
    -243,   338,   149,   338,    35,  -243,   307,   390,  -243,  -243,
    -243,  -243,  -243,   316,   108,   316,    45,   391,   392,   189,
     358,   213,   213,  -243,  -243,  -243,    26,    73,    73,    73,
      73,    73,    73,    73,   148,   148,   148,  -243,  -243,  -243,
    -243,  -243,  -243,    93,   327,   388,   213,  -243,   213,   213,
    -243,    47,  -243,   359,   370,  -243,  -243,  -243,   419,    86,
    -243,  -243,   402,    69,  -243,   212,  -243,   417,   213,   213,
     358,  -243,  -243,  -243,    30,  -243,   231,  -243,  -243,   341,
     358,   363,    45,   364,    16,  -243,   260,  -243,   365,  -243,
     316,   409,  -243,    45,   367,  -243,  -243,   427,  -243,   147,
     172,   327,  -243,   327,  -243,  -243,   213,   338,   289,   338,
      47,  -243,   369,  -243,   379,  -243,  -243,     5,   108,  -243,
     430,  -243,  -243,  -243,  -243,  -243,   358,  -243,   372,   385,
    -243,  -243,   418,  -243,  -243,   374,  -243,  -243,  -243,  -243,
    -243,   376,   108,   108,   327,   420,  -243,  -243,  -243,   437,
    -243,   314,  -243,   327,   387,   327,  -243,  -243,   437,    45,
     389,  -243
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    96,     0,     7,
       0,     3,     0,     0,    95,     2,     0,     0,     0,     0,
       0,   100,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    78,    11,    68,     0,    78,     0,   116,     0,    10,
      32,     8,     0,     0,    15,    80,    67,    46,    80,    98,
      97,     0,     0,   100,     0,     0,     0,   100,     0,     0,
     117,   101,   104,   107,   118,   106,     0,   108,   109,   110,
     111,   112,   113,   114,   115,    46,    31,    13,     0,     0,
      27,     0,     0,     0,    26,     0,    23,     0,     0,    45,
       0,     0,     0,     0,     0,    28,     0,    48,     0,     0,
       0,    34,    76,     0,   116,   133,     0,     0,     0,     0,
       0,     0,     0,     0,   173,   176,   174,   175,   129,   172,
     171,     0,   151,   159,   165,   129,   150,     0,     0,     0,
       0,   116,     0,     0,     0,   128,     0,     0,    30,     0,
      29,    24,     0,    25,    21,    17,    20,    22,     0,     0,
       0,     0,    82,     0,    72,    35,     0,     0,     0,     0,
      69,     0,    43,     0,    40,     7,     0,    99,     0,     0,
     178,   160,   161,   180,   184,     0,   182,     0,   179,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,   136,   132,   102,
     105,     0,   121,   122,   125,   127,     0,   131,    33,    18,
      19,    88,     0,    88,     0,    79,     0,     0,     7,    47,
      51,    50,    49,     0,    46,     0,    57,     0,     0,     0,
     145,     0,     0,   181,   177,   144,   116,   152,   153,   154,
     155,   156,   157,   158,   164,   162,   163,   168,   169,   170,
     166,   167,   148,   149,   141,   134,     0,   119,     0,     0,
     126,     0,    83,     0,     0,    81,    86,    70,     0,     0,
      44,    42,     0,    62,    54,     0,    77,    74,     0,     0,
     185,   183,   103,   143,     0,   139,     0,   102,   120,   123,
     130,     0,     0,     0,     0,    90,     0,    85,     0,    73,
       0,     0,    41,    57,     0,    52,    55,     0,     7,     0,
       0,   141,   137,     0,   102,   135,     0,    88,     0,    88,
       0,    87,     0,    84,     0,    36,    53,     0,    46,    71,
       0,   102,   102,   138,   142,   140,   124,    93,     0,     0,
      89,    91,     0,    37,    59,     0,    56,    75,   147,   146,
      92,     0,    46,    46,    66,     0,    94,    39,    38,    61,
      64,     0,    59,    66,     0,    66,    63,    58,    60,    57,
       0,    65
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -243,  -243,  -243,  -159,  -243,  -243,  -243,  -243,   415,   -43,
     208,  -243,   406,   -73,  -243,  -243,  -243,  -243,  -212,    79,
    -243,   144,  -243,  -243,    87,  -243,    85,    88,  -243,   421,
    -243,  -243,  -243,  -243,   422,   404,  -243,   240,  -196,  -243,
     135,    -4,  -243,    27,  -243,  -243,  -179,  -243,   -96,   328,
    -243,   -27,  -243,   200,  -243,   -33,  -243,  -243,  -243,  -243,
    -243,  -243,  -243,   150,  -242,  -243,  -243,  -243,  -243,  -243,
     -51,   224,   -86,   -99,  -243,   226
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,    98,
      86,    39,    40,    99,   343,   353,   226,   100,   101,   272,
     273,   274,   328,   369,   354,   305,   359,   360,    32,    33,
      24,   218,   308,   165,    45,    88,   151,   152,   262,   294,
     295,   275,    25,    36,   104,    37,   235,   236,    61,    62,
      63,   119,   201,   202,    65,   120,   206,    67,    68,    69,
      70,   254,   284,   285,   361,    71,    72,    73,    74,   127,
     174,   122,   123,   124,   175,   176
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      85,     8,   137,   121,    66,   128,   227,   130,   167,   170,
      64,   269,   286,   271,   178,    34,   252,   264,   214,    41,
     255,   320,   126,   171,   172,   102,   344,   195,    34,    51,
      49,    10,   232,    47,    49,   311,    51,   233,    52,    53,
      54,     1,    10,    55,    56,    52,    53,    54,    50,    11,
      55,    56,   198,   312,   148,    57,     5,   345,    58,   268,
     149,   177,    57,   150,    42,    58,   291,   196,    49,   286,
     215,    66,   292,   321,   303,   293,    87,    64,     4,   169,
     107,   203,   204,   153,   129,   207,    59,   304,   324,   156,
      60,   247,   248,   249,   250,   251,     3,    60,    66,     7,
      10,   244,   245,   246,    64,   108,   256,   219,   315,     7,
       9,     7,   187,   257,   220,   221,   222,   229,   230,   188,
     189,   337,   259,   339,   111,    14,   112,   260,    89,   113,
     114,   115,   116,   117,   118,   335,   300,    26,    16,    17,
     282,   301,    15,   179,   134,   212,   135,   136,    28,   330,
      18,   270,   348,   349,    90,    91,    19,   331,    31,    20,
      21,    92,    22,   253,    93,    35,    38,   180,    80,    94,
      95,    96,    97,   181,   182,   183,   184,   185,   186,    43,
     280,   180,   332,    75,   190,   191,   192,   181,   182,   183,
     184,   185,   186,    10,   106,   216,   193,   194,  -129,    10,
     278,   263,    77,    66,   279,   203,   180,   289,   290,    64,
     153,   283,   181,   182,   183,   184,   185,   186,   133,  -129,
      87,  -129,  -129,   180,   132,   108,   105,   309,   310,   181,
     182,   183,   184,   185,   186,   125,   109,   110,   131,   134,
     161,   135,   136,   144,   111,   173,   112,   145,   108,   113,
     114,   115,   116,   117,   118,   346,   162,   296,   163,   109,
     110,   164,    10,   146,   306,   336,   147,   111,   283,   112,
     334,   197,   113,   114,   115,   116,   117,   118,   154,   357,
     358,   313,   155,   314,   180,   157,   141,   143,   318,   160,
     181,   182,   183,   184,   185,   186,   168,   180,   141,   143,
     166,   158,   -26,   181,   182,   183,   184,   185,   186,   159,
      10,   283,   322,   132,   208,   209,   296,   180,    78,    79,
     283,   231,   283,   181,   182,   183,   184,   185,   186,   199,
     210,   205,    80,    81,    82,    83,    84,   180,   211,    10,
     234,   338,   213,   181,   182,   183,   184,   185,   186,   180,
      80,   138,   139,   140,    84,   181,   182,   183,   184,   185,
     186,   217,    90,    91,   313,   223,   364,   258,   224,   225,
     228,   266,    93,    90,    91,   180,    80,    94,    95,    96,
      97,   181,   182,   183,   184,   185,   186,    80,   138,    95,
     140,    84,   180,   316,   261,   267,   276,   277,   181,   182,
     183,   184,   185,   186,   237,   238,   239,   240,   241,   242,
     243,    80,   138,   142,   140,    84,    80,   138,    95,   140,
      84,   287,   298,   297,   299,   302,   307,   317,   319,   323,
     325,   327,   329,   341,   342,   347,   350,   351,   355,   352,
     356,   362,   363,   367,    44,    76,   371,   326,   370,   365,
     368,   366,   103,    46,   265,   340,   288,    48,   281,   200,
       0,   333
};

static const yytype_int16 yycheck[] =
{
      43,     5,    75,    54,    37,    56,   165,    58,   104,   108,
      37,   223,   254,   225,   113,    19,   195,   213,     5,     5,
     199,     5,    55,   109,   110,     5,    21,    10,    32,     3,
       5,    50,    50,    52,     5,     5,     3,    55,    12,    13,
      14,     4,    50,    17,    18,    12,    13,    14,    23,    57,
      17,    18,    23,    23,    19,    29,    56,    52,    32,   218,
      25,   112,    29,    28,    50,    32,    19,    50,     5,   311,
      57,   104,    25,    57,     5,    28,    56,   104,     0,    16,
      53,   132,   133,    87,    57,   136,    60,    18,   300,    93,
      64,   190,   191,   192,   193,   194,    64,    64,   131,    64,
      50,   187,   188,   189,   131,    35,    50,    57,   287,    64,
       5,    64,    39,    57,   157,   158,   159,   168,   169,    46,
      47,   317,    50,   319,    54,    64,    56,    55,    20,    59,
      60,    61,    62,    63,    64,   314,    50,    60,     7,     8,
     236,    55,    51,    10,    51,   149,    53,    54,    64,   308,
      19,   224,   331,   332,    46,    47,    25,    10,    64,    28,
      29,    53,    31,   196,    56,    64,    64,    34,    60,    61,
      62,    63,    64,    40,    41,    42,    43,    44,    45,    45,
     231,    34,    10,    45,    36,    37,    38,    40,    41,    42,
      43,    44,    45,    50,    64,    52,    48,    49,    30,    50,
      11,    52,    60,   236,    15,   256,    34,   258,   259,   236,
     214,   254,    40,    41,    42,    43,    44,    45,    30,    51,
      56,    53,    54,    34,    56,    35,    60,   278,   279,    40,
      41,    42,    43,    44,    45,    64,    46,    47,    52,    51,
       6,    53,    54,     5,    54,    55,    56,     5,    35,    59,
      60,    61,    62,    63,    64,   328,    22,   261,    24,    46,
      47,    27,    50,     5,    52,   316,     5,    54,   311,    56,
     313,    21,    59,    60,    61,    62,    63,    64,     5,   352,
     353,    50,    64,    52,    34,    58,    78,    79,   292,     5,
      40,    41,    42,    43,    44,    45,    30,    34,    90,    91,
      52,    58,    58,    40,    41,    42,    43,    44,    45,    58,
      50,   354,    52,    56,     5,     5,   320,    34,    46,    47,
     363,    58,   365,    40,    41,    42,    43,    44,    45,    26,
       5,    64,    60,    61,    62,    63,    64,    34,    64,    50,
      57,    52,    64,    40,    41,    42,    43,    44,    45,    34,
      60,    61,    62,    63,    64,    40,    41,    42,    43,    44,
      45,     9,    46,    47,    50,    54,    52,    52,    21,    21,
      64,    64,    56,    46,    47,    34,    60,    61,    62,    63,
      64,    40,    41,    42,    43,    44,    45,    60,    61,    62,
      63,    64,    34,    52,    56,     5,     5,     5,    40,    41,
      42,    43,    44,    45,   180,   181,   182,   183,   184,   185,
     186,    60,    61,    62,    63,    64,    60,    61,    62,    63,
      64,    33,    52,    64,     5,    23,     9,    64,    64,    64,
      21,    64,     5,    64,    55,     5,    64,    52,    64,    21,
      64,    21,     5,    56,    29,    39,    57,   303,   369,   362,
     365,   363,    48,    32,   214,   320,   256,    35,   232,   131,
      -1,   311
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    66,    64,     0,    56,    67,    64,   106,     5,
      50,    57,    68,    69,    64,    51,     7,     8,    19,    25,
      28,    29,    31,    70,    95,   107,    60,    71,    64,    72,
      73,    64,    93,    94,   106,    64,   108,   110,    64,    76,
      77,     5,    50,    45,    73,    99,    94,    52,    99,     5,
      23,     3,    12,    13,    14,    17,    18,    29,    32,    60,
      64,   113,   114,   115,   116,   119,   120,   122,   123,   124,
     125,   130,   131,   132,   133,    45,    77,    60,    46,    47,
      60,    61,    62,    63,    64,    74,    75,    56,   100,    20,
      46,    47,    53,    56,    61,    62,    63,    64,    74,    78,
      82,    83,     5,   100,   109,    60,    64,   108,    35,    46,
      47,    54,    56,    59,    60,    61,    62,    63,    64,   116,
     120,   135,   136,   137,   138,    64,   120,   134,   135,   108,
     135,    52,    56,    30,    51,    53,    54,    78,    61,    62,
      63,    75,    62,    75,     5,     5,     5,     5,    19,    25,
      28,   101,   102,   106,     5,    64,   106,    58,    58,    58,
       5,     6,    22,    24,    27,    98,    52,   113,    30,    16,
     138,   137,   137,    55,   135,   139,   140,   135,   138,    10,
      34,    40,    41,    42,    43,    44,    45,    39,    46,    47,
      36,    37,    38,    48,    49,    10,    50,    21,    23,    26,
     114,   117,   118,   135,   135,    64,   121,   135,     5,     5,
       5,    64,   106,    64,     5,    57,    52,     9,    96,    57,
      74,    74,    74,    54,    21,    21,    81,    68,    64,   135,
     135,    58,    50,    55,    57,   111,   112,   136,   136,   136,
     136,   136,   136,   136,   137,   137,   137,   138,   138,   138,
     138,   138,   111,   120,   126,   111,    50,    57,    52,    50,
      55,    56,   103,    52,   103,   102,    64,     5,    68,    83,
      78,    83,    84,    85,    86,   106,     5,     5,    11,    15,
     135,   140,   113,    74,   127,   128,   129,    33,   118,   135,
     135,    19,    25,    28,   104,   105,   106,    64,    52,     5,
      50,    55,    23,     5,    18,    90,    52,     9,    97,   135,
     135,     5,    23,    50,    52,   111,    52,    64,   106,    64,
       5,    57,    52,    64,    83,    21,    86,    64,    87,     5,
      68,    10,    10,   128,    74,   111,   135,   103,    52,   103,
     105,    64,    55,    79,    21,    52,    78,     5,   111,   111,
      64,    52,    21,    80,    89,    64,    64,    78,    78,    91,
      92,   129,    21,     5,    52,    89,    92,    56,    91,    88,
      84,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    67,    68,    69,    69,    70,    70,
      70,    70,    70,    71,    71,    72,    72,    73,    73,    73,
      73,    73,    73,    74,    74,    74,    75,    75,    75,    75,
      75,    76,    76,    77,    78,    78,    79,    80,    78,    78,
      81,    78,    78,    78,    78,    82,    82,    83,    83,    83,
      83,    83,    84,    85,    85,    87,    86,    86,    88,    89,
      90,    90,    90,    91,    91,    92,    92,    93,    93,    94,
      95,    95,    96,    95,    97,    95,    98,    95,    99,   100,
     100,   101,   101,   102,   102,   102,   102,   103,   103,   104,
     104,   105,   105,   105,   105,   106,   106,   107,   109,   108,
     110,   108,   112,   111,   113,   113,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   115,   115,   116,
     117,   117,   118,   118,   118,   119,   120,   120,   120,   120,
     121,   121,   122,   123,   124,   124,   126,   125,   127,   127,
     128,   128,   129,   129,   130,   131,   132,   132,   133,   134,
     134,   135,   135,   135,   135,   135,   135,   135,   135,   136,
     136,   136,   136,   136,   136,   137,   137,   137,   137,   137,
     137,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   139,   139,   140,   140
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     5,     5,
       4,     4,     4,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     1,     4,     1,     2,     0,     0,     9,     9,
       0,     5,     4,     2,     4,     1,     0,     3,     1,     3,
       3,     3,     2,     3,     1,     0,     4,     0,     0,     0,
       7,     5,     0,     3,     1,     6,     0,     2,     1,     4,
       7,     9,     0,     8,     0,    10,     0,     7,     0,     3,
       0,     3,     1,     3,     5,     4,     3,     3,     0,     3,
       1,     3,     4,     3,     5,     3,     1,     3,     0,     4,
       0,     2,     0,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     4,
       3,     1,     1,     3,     5,     3,     4,     3,     2,     1,
       3,     1,     3,     2,     4,     6,     0,     6,     3,     1,
       3,     0,     3,     1,     4,     4,     8,     8,     4,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     3,     2,     2,
       2,     3,     1,     3,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = PASCAL_YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == PASCAL_YYEMPTY)                                        \
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
   Use PASCAL_YYerror or PASCAL_YYUNDEF. */
#define YYERRCODE PASCAL_YYUNDEF


/* Enable debugging if requested.  */
#if PASCAL_YYDEBUG

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
#else /* !PASCAL_YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !PASCAL_YYDEBUG */


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

  yychar = PASCAL_YYEMPTY; /* Cause a token to be read.  */

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
  if (yychar == PASCAL_YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= PASCAL_YYEOF)
    {
      yychar = PASCAL_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == PASCAL_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = PASCAL_YYUNDEF;
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
  yychar = PASCAL_YYEMPTY;
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
  case 2: /* program: PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD  */
#line 88 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 1558 "pascal.tab.c"
    break;

  case 3: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 91 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 1564 "pascal.tab.c"
    break;

  case 4: /* file_id_list_opt: %empty  */
#line 92 "pascal.y"
     { (yyval.node) = NULL; }
#line 1570 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 95 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1576 "pascal.tab.c"
    break;

  case 6: /* decl_part_list: decl_part_list decl_part  */
#line 98 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1582 "pascal.tab.c"
    break;

  case 7: /* decl_part_list: %empty  */
#line 99 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 1588 "pascal.tab.c"
    break;

  case 8: /* decl_part: LABELSY label_list SEMICOLON  */
#line 102 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1594 "pascal.tab.c"
    break;

  case 9: /* decl_part: CONSTSY const_decl_list  */
#line 103 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 1600 "pascal.tab.c"
    break;

  case 10: /* decl_part: TYPESY type_decl_list  */
#line 104 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1606 "pascal.tab.c"
    break;

  case 11: /* decl_part: VARSY var_decl_list  */
#line 105 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1612 "pascal.tab.c"
    break;

  case 12: /* decl_part: procedure_decl  */
#line 106 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1618 "pascal.tab.c"
    break;

  case 13: /* label_list: label_list COMMA INTCONST  */
#line 109 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1624 "pascal.tab.c"
    break;

  case 14: /* label_list: INTCONST  */
#line 110 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 1630 "pascal.tab.c"
    break;

  case 15: /* const_decl_list: const_decl_list const_decl  */
#line 113 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1636 "pascal.tab.c"
    break;

  case 16: /* const_decl_list: const_decl  */
#line 114 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 1642 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP REALCONST SEMICOLON  */
#line 117 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_real((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1648 "pascal.tab.c"
    break;

  case 18: /* const_decl: IDENT EQOP PLUS REALCONST SEMICOLON  */
#line 118 "pascal.y"
                                         { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_PLS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, (yyvsp[-1].node)->v.dval); }
#line 1654 "pascal.tab.c"
    break;

  case 19: /* const_decl: IDENT EQOP MINUS REALCONST SEMICOLON  */
#line 119 "pascal.y"
                                          { (yyval.node) = pt_decl("const", (yyvsp[-4].node), pt_add(pt_new(TT_MNS), (yyvsp[-1].node))); cenv_real((yyvsp[-4].node)->v.sval, -(yyvsp[-1].node)->v.dval); }
#line 1660 "pascal.tab.c"
    break;

  case 20: /* const_decl: IDENT EQOP STRINGCONST SEMICOLON  */
#line 120 "pascal.y"
                                      { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_string((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.sval); }
#line 1666 "pascal.tab.c"
    break;

  case 21: /* const_decl: IDENT EQOP CHARCODE SEMICOLON  */
#line 121 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)->v.ival); }
#line 1672 "pascal.tab.c"
    break;

  case 22: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 122 "pascal.y"
                                   { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_int((yyvsp[-3].node)->v.sval, cenv_value((yyvsp[-1].node))); }
#line 1678 "pascal.tab.c"
    break;

  case 23: /* constant: scalar_constant  */
#line 125 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1684 "pascal.tab.c"
    break;

  case 24: /* constant: PLUS scalar_constant  */
#line 126 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 1690 "pascal.tab.c"
    break;

  case 25: /* constant: MINUS scalar_constant  */
#line 127 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 1696 "pascal.tab.c"
    break;

  case 26: /* scalar_constant: IDENT  */
#line 130 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1702 "pascal.tab.c"
    break;

  case 27: /* scalar_constant: INTCONST  */
#line 131 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1708 "pascal.tab.c"
    break;

  case 28: /* scalar_constant: REALCONST  */
#line 132 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1714 "pascal.tab.c"
    break;

  case 29: /* scalar_constant: STRINGCONST  */
#line 133 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1720 "pascal.tab.c"
    break;

  case 30: /* scalar_constant: CHARCODE  */
#line 134 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1726 "pascal.tab.c"
    break;

  case 31: /* type_decl_list: type_decl_list type_decl  */
#line 137 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1732 "pascal.tab.c"
    break;

  case 32: /* type_decl_list: type_decl  */
#line 138 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 1738 "pascal.tab.c"
    break;

  case 33: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 141 "pascal.y"
                              { (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 1744 "pascal.tab.c"
    break;

  case 34: /* type: simple_type  */
#line 144 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1750 "pascal.tab.c"
    break;

  case 35: /* type: ARROW IDENT  */
#line 145 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 1756 "pascal.tab.c"
    break;

  case 36: /* $@1: %empty  */
#line 146 "pascal.y"
                                                       { }
#line 1762 "pascal.tab.c"
    break;

  case 37: /* $@2: %empty  */
#line 146 "pascal.y"
                                                           { }
#line 1768 "pascal.tab.c"
    break;

  case 38: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 146 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 1774 "pascal.tab.c"
    break;

  case 39: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 147 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1780 "pascal.tab.c"
    break;

  case 40: /* $@3: %empty  */
#line 148 "pascal.y"
                         { }
#line 1786 "pascal.tab.c"
    break;

  case 41: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 148 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 1792 "pascal.tab.c"
    break;

  case 42: /* type: packed_opt SETSY OFSY simple_type  */
#line 149 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1798 "pascal.tab.c"
    break;

  case 43: /* type: packed_opt FILESY  */
#line 150 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 1804 "pascal.tab.c"
    break;

  case 44: /* type: packed_opt FILESY OFSY type  */
#line 151 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1810 "pascal.tab.c"
    break;

  case 45: /* packed_opt: PACKEDSY  */
#line 154 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 1816 "pascal.tab.c"
    break;

  case 46: /* packed_opt: %empty  */
#line 155 "pascal.y"
     { (yyval.node) = NULL; }
#line 1822 "pascal.tab.c"
    break;

  case 47: /* simple_type: LPARENT id_list RPARENT  */
#line 158 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 1828 "pascal.tab.c"
    break;

  case 48: /* simple_type: IDENT  */
#line 159 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1834 "pascal.tab.c"
    break;

  case 49: /* simple_type: constant DOTDOT constant  */
#line 160 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 1840 "pascal.tab.c"
    break;

  case 50: /* simple_type: STRINGCONST DOTDOT constant  */
#line 161 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 1846 "pascal.tab.c"
    break;

  case 51: /* simple_type: CHARCODE DOTDOT constant  */
#line 162 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 1852 "pascal.tab.c"
    break;

  case 52: /* record_body: record_field_list record_case_opt  */
#line 165 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 1858 "pascal.tab.c"
    break;

  case 53: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 168 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1864 "pascal.tab.c"
    break;

  case 54: /* record_field_list: record_field  */
#line 169 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 1870 "pascal.tab.c"
    break;

  case 55: /* $@4: %empty  */
#line 172 "pascal.y"
                  { }
#line 1876 "pascal.tab.c"
    break;

  case 56: /* record_field: id_list COLON $@4 type  */
#line 172 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 1882 "pascal.tab.c"
    break;

  case 57: /* record_field: %empty  */
#line 173 "pascal.y"
     { (yyval.node) = NULL; }
#line 1888 "pascal.tab.c"
    break;

  case 58: /* case_arm_mark: %empty  */
#line 176 "pascal.y"
    { }
#line 1894 "pascal.tab.c"
    break;

  case 59: /* case_open: %empty  */
#line 179 "pascal.y"
    { }
#line 1900 "pascal.tab.c"
    break;

  case 60: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 182 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1906 "pascal.tab.c"
    break;

  case 61: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 183 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 1912 "pascal.tab.c"
    break;

  case 62: /* record_case_opt: %empty  */
#line 184 "pascal.y"
     { (yyval.node) = NULL; }
#line 1918 "pascal.tab.c"
    break;

  case 63: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 187 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1924 "pascal.tab.c"
    break;

  case 64: /* record_case_list: record_case_arm  */
#line 188 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 1930 "pascal.tab.c"
    break;

  case 65: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 191 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 1936 "pascal.tab.c"
    break;

  case 66: /* record_case_arm: %empty  */
#line 192 "pascal.y"
     { (yyval.node) = NULL; }
#line 1942 "pascal.tab.c"
    break;

  case 67: /* var_decl_list: var_decl_list var_decl  */
#line 195 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1948 "pascal.tab.c"
    break;

  case 68: /* var_decl_list: var_decl  */
#line 196 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 1954 "pascal.tab.c"
    break;

  case 69: /* var_decl: id_list COLON type SEMICOLON  */
#line 199 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 1960 "pascal.tab.c"
    break;

  case 70: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 202 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 1966 "pascal.tab.c"
    break;

  case 71: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 203 "pascal.y"
                                                                                           { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 1972 "pascal.tab.c"
    break;

  case 72: /* $@5: %empty  */
#line 204 "pascal.y"
                                                            { }
#line 1978 "pascal.tab.c"
    break;

  case 73: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON $@5 block SEMICOLON  */
#line 204 "pascal.y"
                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 1984 "pascal.tab.c"
    break;

  case 74: /* $@6: %empty  */
#line 205 "pascal.y"
                                                                       { }
#line 1990 "pascal.tab.c"
    break;

  case 75: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON $@6 block SEMICOLON  */
#line 205 "pascal.y"
                                                                                           { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 1996 "pascal.tab.c"
    break;

  case 76: /* $@7: %empty  */
#line 206 "pascal.y"
                                        { }
#line 2002 "pascal.tab.c"
    break;

  case 77: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 206 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2008 "pascal.tab.c"
    break;

  case 78: /* pv_mark: %empty  */
#line 209 "pascal.y"
    { }
#line 2014 "pascal.tab.c"
    break;

  case 79: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 212 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 2020 "pascal.tab.c"
    break;

  case 80: /* parameter_list_opt: %empty  */
#line 213 "pascal.y"
     { (yyval.node) = NULL; }
#line 2026 "pascal.tab.c"
    break;

  case 81: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 216 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2032 "pascal.tab.c"
    break;

  case 82: /* parameter_decl_list: parameter_decl  */
#line 217 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2038 "pascal.tab.c"
    break;

  case 83: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 220 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2044 "pascal.tab.c"
    break;

  case 84: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 221 "pascal.y"
                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2050 "pascal.tab.c"
    break;

  case 85: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 222 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2056 "pascal.tab.c"
    break;

  case 86: /* parameter_decl: id_list COLON IDENT  */
#line 223 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2062 "pascal.tab.c"
    break;

  case 87: /* pf_params: LPARENT pf_sections RPARENT  */
#line 226 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2068 "pascal.tab.c"
    break;

  case 88: /* pf_params: %empty  */
#line 227 "pascal.y"
     { (yyval.node) = NULL; }
#line 2074 "pascal.tab.c"
    break;

  case 89: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 230 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2080 "pascal.tab.c"
    break;

  case 90: /* pf_sections: pf_section  */
#line 231 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 2086 "pascal.tab.c"
    break;

  case 91: /* pf_section: id_list COLON IDENT  */
#line 234 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 2092 "pascal.tab.c"
    break;

  case 92: /* pf_section: VARSY id_list COLON IDENT  */
#line 235 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 2098 "pascal.tab.c"
    break;

  case 93: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 236 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 2104 "pascal.tab.c"
    break;

  case 94: /* pf_section: FUNCTIONSY IDENT pf_params COLON IDENT  */
#line 237 "pascal.y"
                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 2110 "pascal.tab.c"
    break;

  case 95: /* id_list: id_list COMMA IDENT  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2116 "pascal.tab.c"
    break;

  case 96: /* id_list: IDENT  */
#line 241 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2122 "pascal.tab.c"
    break;

  case 97: /* body: BEGINSY statement_list ENDSY  */
#line 244 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2128 "pascal.tab.c"
    break;

  case 98: /* $@8: %empty  */
#line 247 "pascal.y"
                             { pas_stmt_mark(); }
#line 2134 "pascal.tab.c"
    break;

  case 99: /* statement_list: statement_list SEMICOLON $@8 statement  */
#line 247 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 2140 "pascal.tab.c"
    break;

  case 100: /* $@9: %empty  */
#line 248 "pascal.y"
     { pas_stmt_mark(); }
#line 2146 "pascal.tab.c"
    break;

  case 101: /* statement_list: $@9 statement  */
#line 248 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 2152 "pascal.tab.c"
    break;

  case 102: /* $@10: %empty  */
#line 251 "pascal.y"
    { pas_stmt_mark(); }
#line 2158 "pascal.tab.c"
    break;

  case 103: /* body_stmt: $@10 statement  */
#line 251 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 2164 "pascal.tab.c"
    break;

  case 104: /* statement: statement_no_label  */
#line 254 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2170 "pascal.tab.c"
    break;

  case 105: /* statement: INTCONST COLON statement_no_label  */
#line 255 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2176 "pascal.tab.c"
    break;

  case 106: /* statement_no_label: assignment  */
#line 258 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2182 "pascal.tab.c"
    break;

  case 107: /* statement_no_label: call  */
#line 259 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2188 "pascal.tab.c"
    break;

  case 108: /* statement_no_label: compound_statement  */
#line 260 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2194 "pascal.tab.c"
    break;

  case 109: /* statement_no_label: goto_statement  */
#line 261 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2200 "pascal.tab.c"
    break;

  case 110: /* statement_no_label: if_statement  */
#line 262 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2206 "pascal.tab.c"
    break;

  case 111: /* statement_no_label: case_statement  */
#line 263 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2212 "pascal.tab.c"
    break;

  case 112: /* statement_no_label: while_statement  */
#line 264 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2218 "pascal.tab.c"
    break;

  case 113: /* statement_no_label: repeat_statement  */
#line 265 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2224 "pascal.tab.c"
    break;

  case 114: /* statement_no_label: for_statement  */
#line 266 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2230 "pascal.tab.c"
    break;

  case 115: /* statement_no_label: with_statement  */
#line 267 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2236 "pascal.tab.c"
    break;

  case 116: /* statement_no_label: %empty  */
#line 268 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 2242 "pascal.tab.c"
    break;

  case 117: /* call: IDENT  */
#line 271 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 2248 "pascal.tab.c"
    break;

  case 118: /* call: call_with_args  */
#line 272 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2254 "pascal.tab.c"
    break;

  case 119: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 275 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2260 "pascal.tab.c"
    break;

  case 120: /* argument_list: argument_list COMMA argument  */
#line 278 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2266 "pascal.tab.c"
    break;

  case 121: /* argument_list: argument  */
#line 279 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2272 "pascal.tab.c"
    break;

  case 122: /* argument: expression  */
#line 282 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2278 "pascal.tab.c"
    break;

  case 123: /* argument: expression COLON expression  */
#line 283 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2284 "pascal.tab.c"
    break;

  case 124: /* argument: expression COLON expression COLON expression  */
#line 284 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2290 "pascal.tab.c"
    break;

  case 125: /* assignment: selector BECOMES expression  */
#line 287 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2296 "pascal.tab.c"
    break;

  case 126: /* selector: selector LBRACK expression_list RBRACK  */
#line 290 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2302 "pascal.tab.c"
    break;

  case 127: /* selector: selector PERIOD IDENT  */
#line 291 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2308 "pascal.tab.c"
    break;

  case 128: /* selector: selector ARROW  */
#line 292 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 2314 "pascal.tab.c"
    break;

  case 129: /* selector: IDENT  */
#line 293 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2320 "pascal.tab.c"
    break;

  case 130: /* expression_list: expression_list COMMA expression  */
#line 296 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2326 "pascal.tab.c"
    break;

  case 131: /* expression_list: expression  */
#line 297 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2332 "pascal.tab.c"
    break;

  case 132: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 300 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2338 "pascal.tab.c"
    break;

  case 133: /* goto_statement: GOTOSY INTCONST  */
#line 303 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 2344 "pascal.tab.c"
    break;

  case 134: /* if_statement: IFSY expression THENSY body_stmt  */
#line 306 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2350 "pascal.tab.c"
    break;

  case 135: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 307 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2356 "pascal.tab.c"
    break;

  case 136: /* $@11: %empty  */
#line 310 "pascal.y"
                           { }
#line 2362 "pascal.tab.c"
    break;

  case 137: /* case_statement: CASESY expression OFSY $@11 case_list ENDSY  */
#line 310 "pascal.y"
                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2368 "pascal.tab.c"
    break;

  case 138: /* case_list: case_list SEMICOLON case_elem  */
#line 313 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2374 "pascal.tab.c"
    break;

  case 139: /* case_list: case_elem  */
#line 314 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2380 "pascal.tab.c"
    break;

  case 140: /* case_elem: constant_list COLON body_stmt  */
#line 317 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2386 "pascal.tab.c"
    break;

  case 141: /* case_elem: %empty  */
#line 318 "pascal.y"
     { (yyval.node) = NULL; }
#line 2392 "pascal.tab.c"
    break;

  case 142: /* constant_list: constant_list COMMA constant  */
#line 321 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2398 "pascal.tab.c"
    break;

  case 143: /* constant_list: constant  */
#line 322 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2404 "pascal.tab.c"
    break;

  case 144: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 325 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2410 "pascal.tab.c"
    break;

  case 145: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 328 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2416 "pascal.tab.c"
    break;

  case 146: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 331 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 2422 "pascal.tab.c"
    break;

  case 147: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 332 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 2428 "pascal.tab.c"
    break;

  case 148: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 335 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2434 "pascal.tab.c"
    break;

  case 149: /* with_open: with_open COMMA selector  */
#line 338 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2440 "pascal.tab.c"
    break;

  case 150: /* with_open: selector  */
#line 339 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2446 "pascal.tab.c"
    break;

  case 151: /* expression: simple_expression  */
#line 346 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 2452 "pascal.tab.c"
    break;

  case 152: /* expression: expression INOP simple_expression  */
#line 347 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2458 "pascal.tab.c"
    break;

  case 153: /* expression: expression LTOP simple_expression  */
#line 348 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2464 "pascal.tab.c"
    break;

  case 154: /* expression: expression LEOP simple_expression  */
#line 349 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2470 "pascal.tab.c"
    break;

  case 155: /* expression: expression GTOP simple_expression  */
#line 350 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2476 "pascal.tab.c"
    break;

  case 156: /* expression: expression GEOP simple_expression  */
#line 351 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2482 "pascal.tab.c"
    break;

  case 157: /* expression: expression NEOP simple_expression  */
#line 352 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2488 "pascal.tab.c"
    break;

  case 158: /* expression: expression EQOP simple_expression  */
#line 353 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2494 "pascal.tab.c"
    break;

  case 159: /* simple_expression: term  */
#line 356 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2500 "pascal.tab.c"
    break;

  case 160: /* simple_expression: PLUS term  */
#line 357 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2506 "pascal.tab.c"
    break;

  case 161: /* simple_expression: MINUS term  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2512 "pascal.tab.c"
    break;

  case 162: /* simple_expression: simple_expression PLUS term  */
#line 359 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2518 "pascal.tab.c"
    break;

  case 163: /* simple_expression: simple_expression MINUS term  */
#line 360 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2524 "pascal.tab.c"
    break;

  case 164: /* simple_expression: simple_expression OROP term  */
#line 361 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2530 "pascal.tab.c"
    break;

  case 165: /* term: factor  */
#line 364 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2536 "pascal.tab.c"
    break;

  case 166: /* term: term MUL factor  */
#line 365 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2542 "pascal.tab.c"
    break;

  case 167: /* term: term RDIV factor  */
#line 366 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2548 "pascal.tab.c"
    break;

  case 168: /* term: term IDIV factor  */
#line 367 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2554 "pascal.tab.c"
    break;

  case 169: /* term: term IMOD factor  */
#line 368 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2560 "pascal.tab.c"
    break;

  case 170: /* term: term ANDOP factor  */
#line 369 "pascal.y"
                       { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2566 "pascal.tab.c"
    break;

  case 171: /* factor: selector  */
#line 372 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2572 "pascal.tab.c"
    break;

  case 172: /* factor: call_with_args  */
#line 373 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2578 "pascal.tab.c"
    break;

  case 173: /* factor: INTCONST  */
#line 374 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2584 "pascal.tab.c"
    break;

  case 174: /* factor: REALCONST  */
#line 375 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2590 "pascal.tab.c"
    break;

  case 175: /* factor: STRINGCONST  */
#line 376 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2596 "pascal.tab.c"
    break;

  case 176: /* factor: CHARCODE  */
#line 377 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2602 "pascal.tab.c"
    break;

  case 177: /* factor: LPARENT expression RPARENT  */
#line 378 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2608 "pascal.tab.c"
    break;

  case 178: /* factor: NOTSY factor  */
#line 379 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 2614 "pascal.tab.c"
    break;

  case 179: /* factor: ATSIGN factor  */
#line 380 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 2620 "pascal.tab.c"
    break;

  case 180: /* factor: LBRACK RBRACK  */
#line 381 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 2626 "pascal.tab.c"
    break;

  case 181: /* factor: LBRACK set_member_list RBRACK  */
#line 382 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 2632 "pascal.tab.c"
    break;

  case 182: /* set_member_list: set_member  */
#line 385 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2638 "pascal.tab.c"
    break;

  case 183: /* set_member_list: set_member_list COMMA set_member  */
#line 386 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2644 "pascal.tab.c"
    break;

  case 184: /* set_member: expression  */
#line 389 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2650 "pascal.tab.c"
    break;

  case 185: /* set_member: expression DOTDOT expression  */
#line 390 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2656 "pascal.tab.c"
    break;


#line 2660 "pascal.tab.c"

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
  yytoken = yychar == PASCAL_YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= PASCAL_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == PASCAL_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = PASCAL_YYEMPTY;
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
  if (yychar != PASCAL_YYEMPTY)
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

#line 396 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
extern void pascal_lex_reset(void);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    pascal_lex_reset(); pascal_cenv_reset();
    pas_stmt_sp = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
