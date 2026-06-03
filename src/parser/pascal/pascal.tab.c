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
#line 7 "pascal.y"

#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
void pascal_yyerror(const char *msg) { fprintf(stderr, "pascal parse error line %d: %s\n", pascal_get_lineno(), msg); }
tree_t   *pascal_prog_result = NULL;
static PNodeList g_pascal_procs;
static PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)calloc(1, sizeof *l); return l; }
static void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)realloc(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
static PNodeList *pnl_concat(PNodeList *a, PNodeList *b) {
    if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a;
}
static tree_t *leaf_s(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)(s ? s : ""); return e; }
static tree_t *ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *flit(double v) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = v; return e; }
static tree_t *bin(tree_e k, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *un(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); ast_push(e, a); return e; }
static tree_t *prog_of(PNodeList *l) {
    tree_t *e = ast_node_new(TT_PROGRAM);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static tree_t *seq_of(PNodeList *l) {
    if (l && l->count == 1) return l->items[0];
    tree_t *e = ast_node_new(TT_SEQ_EXPR);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static const char *map_io(const char *fn) {
    if (fn && !strcmp(fn, "writeln")) return "__pas_writeln";
    if (fn && !strcmp(fn, "write"))   return "__pas_write";
    if (fn && !strcmp(fn, "sqr"))     return "__pas_sqr";
    return fn;
}
static int is_pas_io(const char *fn) {
    return fn && (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write"));
}
static tree_t *mk_call(const char *name, PNodeList *args) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = 0; i < args->count; i++) ast_push(e, args->items[i]);
        } else {
            for (int i = 0; i < args->count; i += 2) ast_push(e, args->items[i]);
        }
    }
    return e;
}
static void emit_proc(PNodeList *procs, tree_t *proc) {
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":lang", LANG_PASCAL));
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); }
    else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { if (i < 64) byref |= (1LL << i); pv->n = 0; }
        ast_push(vlist, pv);
    }
    vlist->v.ival = byref;
    ast_push(proc, vlist);
    ast_push(proc, body_prog);
    if (is_function) ast_push(proc, leaf_s(TT_VAR, name));
    return proc;
}
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
static int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; long long high; } g_pas_arrays[256]; static int g_pas_narray;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_narray++; } }
static tree_t *mk_ident(const char *name) {
    if (name && !strcmp(name, "true"))  return ilit(1);
    if (name && !strcmp(name, "false")) return ilit(0);
    long long cv; if (pas_const_get(name, &cv)) return ilit(cv);
    return leaf_s(TT_VAR, name);
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
static tree_t *mk_array_fill(long long high) {
    long long n = high + 1; if (n < 1) n = 1;
    size_t len = (size_t)(n * 2 - 1);
    char *buf = (char *)malloc(len + 1); size_t p = 0;
    for (long long k = 0; k < n; k++) { if (k) buf[p++] = '\001'; buf[p++] = '0'; }
    buf[p] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}

#line 192 "pascal.tab.c"

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
  YYSYMBOL_INTCONST = 59,                  /* INTCONST  */
  YYSYMBOL_REALCONST = 60,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 61,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 62,                     /* IDENT  */
  YYSYMBOL_YYACCEPT = 63,                  /* $accept  */
  YYSYMBOL_program = 64,                   /* program  */
  YYSYMBOL_file_id_list_opt = 65,          /* file_id_list_opt  */
  YYSYMBOL_block = 66,                     /* block  */
  YYSYMBOL_decl_part_list = 67,            /* decl_part_list  */
  YYSYMBOL_decl_part = 68,                 /* decl_part  */
  YYSYMBOL_label_list = 69,                /* label_list  */
  YYSYMBOL_const_decl_list = 70,           /* const_decl_list  */
  YYSYMBOL_const_decl = 71,                /* const_decl  */
  YYSYMBOL_constant = 72,                  /* constant  */
  YYSYMBOL_scalar_constant = 73,           /* scalar_constant  */
  YYSYMBOL_type_decl_list = 74,            /* type_decl_list  */
  YYSYMBOL_type_decl = 75,                 /* type_decl  */
  YYSYMBOL_type = 76,                      /* type  */
  YYSYMBOL_packed_opt = 77,                /* packed_opt  */
  YYSYMBOL_simple_type = 78,               /* simple_type  */
  YYSYMBOL_record_body = 79,               /* record_body  */
  YYSYMBOL_record_field_list = 80,         /* record_field_list  */
  YYSYMBOL_record_field = 81,              /* record_field  */
  YYSYMBOL_var_decl_list = 82,             /* var_decl_list  */
  YYSYMBOL_var_decl = 83,                  /* var_decl  */
  YYSYMBOL_procedure_decl = 84,            /* procedure_decl  */
  YYSYMBOL_parameter_list_opt = 85,        /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 86,       /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 87,            /* parameter_decl  */
  YYSYMBOL_id_list = 88,                   /* id_list  */
  YYSYMBOL_body = 89,                      /* body  */
  YYSYMBOL_statement_list = 90,            /* statement_list  */
  YYSYMBOL_statement = 91,                 /* statement  */
  YYSYMBOL_statement_no_label = 92,        /* statement_no_label  */
  YYSYMBOL_call = 93,                      /* call  */
  YYSYMBOL_call_with_args = 94,            /* call_with_args  */
  YYSYMBOL_argument_list = 95,             /* argument_list  */
  YYSYMBOL_argument = 96,                  /* argument  */
  YYSYMBOL_assignment = 97,                /* assignment  */
  YYSYMBOL_selector = 98,                  /* selector  */
  YYSYMBOL_expression_list = 99,           /* expression_list  */
  YYSYMBOL_compound_statement = 100,       /* compound_statement  */
  YYSYMBOL_goto_statement = 101,           /* goto_statement  */
  YYSYMBOL_if_statement = 102,             /* if_statement  */
  YYSYMBOL_case_statement = 103,           /* case_statement  */
  YYSYMBOL_case_list = 104,                /* case_list  */
  YYSYMBOL_case_elem = 105,                /* case_elem  */
  YYSYMBOL_constant_list = 106,            /* constant_list  */
  YYSYMBOL_while_statement = 107,          /* while_statement  */
  YYSYMBOL_repeat_statement = 108,         /* repeat_statement  */
  YYSYMBOL_for_statement = 109,            /* for_statement  */
  YYSYMBOL_with_statement = 110,           /* with_statement  */
  YYSYMBOL_selector_list = 111,            /* selector_list  */
  YYSYMBOL_expression = 112,               /* expression  */
  YYSYMBOL_simple_expression = 113,        /* simple_expression  */
  YYSYMBOL_term = 114,                     /* term  */
  YYSYMBOL_factor = 115,                   /* factor  */
  YYSYMBOL_expression_list_opt = 116       /* expression_list_opt  */
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
#define YYLAST   353

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  275

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   317


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
      55,    56,    57,    58,    59,    60,    61,    62
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   145,   145,   160,   161,   164,   167,   168,   171,   172,
     173,   174,   175,   178,   179,   182,   183,   185,   187,   187,
     187,   187,   188,   188,   188,   190,   191,   193,   195,   196,
     197,   198,   199,   200,   202,   202,   204,   205,   206,   209,
     212,   213,   216,   217,   220,   221,   223,   225,   226,   227,
     229,   233,   234,   237,   238,   241,   242,   243,   244,   247,
     248,   251,   254,   255,   258,   259,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   275,   276,   279,
     282,   283,   286,   287,   290,   293,   294,   295,   296,   299,
     300,   303,   306,   309,   310,   313,   316,   317,   320,   321,
     324,   325,   328,   331,   334,   336,   340,   343,   344,   347,
     348,   349,   350,   351,   352,   353,   354,   357,   358,   359,
     360,   361,   362,   365,   366,   367,   368,   369,   370,   373,
     374,   375,   376,   377,   378,   379,   380,   383,   384
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
  "LPARENT", "RPARENT", "DOTDOT", "INTCONST", "REALCONST", "STRINGCONST",
  "IDENT", "$accept", "program", "file_id_list_opt", "block",
  "decl_part_list", "decl_part", "label_list", "const_decl_list",
  "const_decl", "constant", "scalar_constant", "type_decl_list",
  "type_decl", "type", "packed_opt", "simple_type", "record_body",
  "record_field_list", "record_field", "var_decl_list", "var_decl",
  "procedure_decl", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "id_list", "body", "statement_list", "statement",
  "statement_no_label", "call", "call_with_args", "argument_list",
  "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "case_list", "case_elem", "constant_list", "while_statement",
  "repeat_statement", "for_statement", "with_statement", "selector_list",
  "expression", "simple_expression", "term", "factor",
  "expression_list_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-114)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-89)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      12,    -1,    70,    17,  -114,    18,   101,  -114,   -11,  -114,
      54,  -114,    32,   247,  -114,  -114,    63,    64,    72,    18,
      93,    23,    94,  -114,  -114,  -114,  -114,     8,   115,    64,
    -114,   105,    18,  -114,   140,   105,   108,   117,    23,    49,
     118,    49,    23,    49,   131,    21,     7,  -114,  -114,  -114,
    -114,  -114,   112,  -114,  -114,  -114,  -114,  -114,  -114,  -114,
    -114,   146,    94,  -114,  -114,   157,   248,  -114,     6,   217,
    -114,    71,   179,  -114,   211,    13,   158,   158,   158,    49,
      49,  -114,  -114,  -114,   182,  -114,   121,   154,   -24,   133,
    -114,  -114,   121,     4,   219,    27,   259,    30,    49,    23,
    -114,    49,   184,  -114,    49,    71,  -114,  -114,   208,   208,
    -114,  -114,  -114,  -114,   242,  -114,    18,    18,    18,     0,
    -114,   151,   256,  -114,   195,    18,   213,   216,   264,   135,
    -114,   215,    49,    49,  -114,   133,   133,   270,   284,   243,
     239,    23,    49,    49,    49,    49,    49,    49,    49,   158,
     158,   158,   158,   158,   158,   158,   158,    23,   118,   248,
    -114,    23,  -114,     3,  -114,   271,  -114,   284,  -114,    52,
     292,  -114,  -114,  -114,   272,   163,   171,     6,  -114,   244,
     316,   325,  -114,    78,   248,  -114,   277,  -114,   311,    18,
     328,   104,   284,    49,  -114,  -114,  -114,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   133,   133,   133,  -114,  -114,  -114,
    -114,  -114,  -114,   121,  -114,    33,  -114,   206,   301,    49,
    -114,    49,  -114,  -114,   273,   274,  -114,  -114,  -114,  -114,
    -114,  -114,   183,   183,   314,   333,  -114,   267,   330,    49,
      49,   284,   248,  -114,   248,    23,    23,  -114,   284,  -114,
    -114,   285,  -114,  -114,    18,    71,   336,   337,   166,   192,
    -114,  -114,  -114,  -114,   322,  -114,  -114,  -114,  -114,    23,
      23,    71,  -114,  -114,  -114
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     4,     1,     0,     0,    60,     0,     7,
       0,     3,     0,     0,    59,     2,     0,     0,     0,     0,
       0,    76,     0,     6,    12,     5,    14,     0,     0,     9,
      16,    52,    11,    45,     0,    52,     0,     0,    76,     0,
       0,     0,    76,     0,     0,    77,     0,    63,    64,    67,
      78,    66,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    10,    26,     8,     0,     0,    15,     0,     0,
      44,    35,     0,    92,     0,     0,     0,     0,     0,   138,
       0,   131,   132,   133,    88,   130,   129,     0,   109,   117,
     123,    88,   108,     0,     0,     0,     0,    76,     0,    76,
      61,     0,     0,    87,     0,    35,    25,    13,     0,     0,
      23,    24,    18,    22,     0,    19,     0,     0,     0,     0,
      54,     0,     7,    34,     0,     0,    37,     0,     0,     0,
      28,     0,     0,     0,   135,   118,   119,   137,    90,     0,
       0,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    76,     0,    99,
      91,    76,    65,     0,    81,    82,    62,    84,    86,     0,
       0,    20,    21,    17,    55,     0,     0,     0,    51,     0,
       0,     0,    29,     0,     0,    46,     0,    33,     0,    43,
       0,     0,   103,     0,   136,   134,   102,   110,   111,   112,
     113,   114,   115,   116,   122,   120,   121,   126,   127,   128,
     124,   125,   106,   107,   101,     0,    97,     0,    93,     0,
      79,     0,    85,    27,     0,     0,    53,    58,    47,    49,
      36,    38,     0,     0,     0,    39,    41,     0,     7,     0,
       0,    89,    99,    95,     0,    76,    76,    80,    83,    57,
      56,     0,    32,    31,    43,    35,     0,     0,     0,     0,
      96,   100,    98,    94,     0,    40,    42,    48,    50,    76,
      76,    35,   105,   104,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -114,  -114,  -114,  -113,  -114,  -114,  -114,  -114,   315,   -55,
     -15,  -114,   283,  -103,  -114,   -82,  -114,  -114,    92,  -114,
     317,  -114,   312,  -114,   173,    -4,  -114,    29,   -92,   251,
    -114,   -18,  -114,   132,  -114,   -21,   249,  -114,  -114,  -114,
    -114,  -114,   110,  -114,  -114,  -114,  -114,  -114,  -114,   -35,
     144,   -50,   -66,  -114
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     6,    12,    13,    23,    27,    29,    30,   127,
     115,    62,    63,   128,   129,   130,   234,   235,   236,    32,
      33,    24,    69,   119,   120,    34,    25,    46,    47,    48,
      49,    85,   163,   164,    51,    86,   137,    53,    54,    55,
      56,   215,   216,   217,    57,    58,    59,    60,    93,   138,
      88,    89,    90,   139
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,     8,   170,    50,    87,   177,    94,   166,    96,   181,
     134,   114,    99,    64,   157,   149,     1,    52,    99,    92,
      50,    52,   150,   151,    50,   116,    36,   135,   136,   133,
     100,   117,    99,    36,   118,    37,    38,    39,   242,    10,
      40,    41,    37,    38,    39,   140,    11,    40,    41,   196,
     160,   -88,    42,   219,   158,    43,   243,   178,    65,    42,
     220,     3,    43,   165,   121,   212,   167,    75,     7,   218,
       4,    95,   -88,     5,   -88,   -88,    52,    98,    52,    50,
       7,    50,    44,    15,    76,    45,   207,   208,   209,   210,
     211,   123,    45,   171,   172,    77,    78,   191,   192,   204,
     205,   206,   193,    79,   214,    80,     9,   222,    81,    82,
      83,    84,   174,   175,   176,   239,    14,   108,   109,   240,
      52,   183,    26,    50,   124,   257,    28,   125,    10,   231,
     110,   111,   112,   126,    31,   230,    52,   213,   142,    50,
      52,   186,   101,    50,   143,   144,   145,   146,   147,   148,
     251,   252,   266,   262,   263,    35,    61,   187,   241,   188,
      66,    68,   189,   102,   141,   103,   104,    73,   274,   152,
     153,   154,   102,   121,   103,   104,   269,   272,   273,    74,
      91,   155,   156,    97,   165,   237,   248,   214,   142,   261,
      10,   105,    71,    76,   143,   144,   145,   146,   147,   148,
     142,    10,   270,   179,   258,   259,   143,   144,   145,   146,
     147,   148,    79,    10,    80,   224,   107,    81,    82,    83,
      84,    10,   122,   225,    52,    52,   142,    50,    50,   108,
     109,   131,   143,   144,   145,   146,   147,   148,    98,   125,
     159,   132,   110,   111,   112,   126,   168,   173,    52,    52,
     237,    50,    50,   142,    16,    17,   244,   182,   245,   143,
     144,   145,   146,   147,   148,   180,    18,   110,   111,   185,
     113,   -22,    19,   142,   184,    20,    21,   190,    22,   143,
     144,   145,   146,   147,   148,   161,   197,   198,   199,   200,
     201,   202,   203,   142,   108,   109,   195,   223,   194,   143,
     144,   145,   146,   147,   148,   142,   227,   110,   111,   112,
     113,   143,   144,   145,   146,   147,   148,    10,   142,   255,
     193,   228,    10,   221,   143,   144,   145,   146,   147,   148,
     229,   232,   233,   238,   246,   249,   250,   253,   254,   256,
     264,   267,   268,   271,    67,   106,   265,    72,   162,    70,
     226,   247,   260,   169
};

static const yytype_int16 yycheck[] =
{
      21,     5,   105,    21,    39,     5,    41,    99,    43,   122,
      76,    66,     5,     5,    10,    39,     4,    38,     5,    40,
      38,    42,    46,    47,    42,    19,     3,    77,    78,    16,
      23,    25,     5,     3,    28,    12,    13,    14,     5,    50,
      17,    18,    12,    13,    14,    80,    57,    17,    18,   141,
      23,    30,    29,    50,    50,    32,    23,    57,    50,    29,
      57,    62,    32,    98,    68,   157,   101,    38,    62,   161,
       0,    42,    51,    56,    53,    54,    97,    56,    99,    97,
      62,    99,    59,    51,    35,    62,   152,   153,   154,   155,
     156,    20,    62,   108,   109,    46,    47,   132,   133,   149,
     150,   151,    50,    54,   159,    56,     5,    55,    59,    60,
      61,    62,   116,   117,   118,    11,    62,    46,    47,    15,
     141,   125,    59,   141,    53,   238,    62,    56,    50,   184,
      59,    60,    61,    62,    62,    57,   157,   158,    34,   157,
     161,     6,    30,   161,    40,    41,    42,    43,    44,    45,
     232,   233,   255,   245,   246,    62,    62,    22,   193,    24,
      45,    56,    27,    51,    10,    53,    54,    59,   271,    36,
      37,    38,    51,   177,    53,    54,    10,   269,   270,    62,
      62,    48,    49,    52,   219,   189,   221,   242,    34,   244,
      50,    45,    52,    35,    40,    41,    42,    43,    44,    45,
      34,    50,    10,    52,   239,   240,    40,    41,    42,    43,
      44,    45,    54,    50,    56,    52,    59,    59,    60,    61,
      62,    50,     5,    52,   245,   246,    34,   245,   246,    46,
      47,    52,    40,    41,    42,    43,    44,    45,    56,    56,
      21,    30,    59,    60,    61,    62,    62,     5,   269,   270,
     254,   269,   270,    34,     7,     8,    50,    62,    52,    40,
      41,    42,    43,    44,    45,     9,    19,    59,    60,     5,
      62,    58,    25,    34,    58,    28,    29,    62,    31,    40,
      41,    42,    43,    44,    45,    26,   142,   143,   144,   145,
     146,   147,   148,    34,    46,    47,    57,     5,    55,    40,
      41,    42,    43,    44,    45,    34,    62,    59,    60,    61,
      62,    40,    41,    42,    43,    44,    45,    50,    34,    52,
      50,     5,    50,    52,    40,    41,    42,    43,    44,    45,
       5,    54,    21,     5,    33,    62,    62,    23,     5,     9,
      55,     5,     5,    21,    29,    62,   254,    35,    97,    32,
     177,   219,   242,   104
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    64,    62,     0,    56,    65,    62,    88,     5,
      50,    57,    66,    67,    62,    51,     7,     8,    19,    25,
      28,    29,    31,    68,    84,    89,    59,    69,    62,    70,
      71,    62,    82,    83,    88,    62,     3,    12,    13,    14,
      17,    18,    29,    32,    59,    62,    90,    91,    92,    93,
      94,    97,    98,   100,   101,   102,   103,   107,   108,   109,
     110,    62,    74,    75,     5,    50,    45,    71,    56,    85,
      83,    52,    85,    59,    62,    90,    35,    46,    47,    54,
      56,    59,    60,    61,    62,    94,    98,   112,   113,   114,
     115,    62,    98,   111,   112,    90,   112,    52,    56,     5,
      23,    30,    51,    53,    54,    45,    75,    59,    46,    47,
      59,    60,    61,    62,    72,    73,    19,    25,    28,    86,
      87,    88,     5,    20,    53,    56,    62,    72,    76,    77,
      78,    52,    30,    16,   115,   114,   114,    99,   112,   116,
     112,    10,    34,    40,    41,    42,    43,    44,    45,    39,
      46,    47,    36,    37,    38,    48,    49,    10,    50,    21,
      23,    26,    92,    95,    96,   112,    91,   112,    62,    99,
      76,    73,    73,     5,    88,    88,    88,     5,    57,    52,
       9,    66,    62,    88,    58,     5,     6,    22,    24,    27,
      62,   112,   112,    50,    55,    57,    91,   113,   113,   113,
     113,   113,   113,   113,   114,   114,   114,   115,   115,   115,
     115,   115,    91,    98,    72,   104,   105,   106,    91,    50,
      57,    52,    55,     5,    52,    52,    87,    62,     5,     5,
      57,    72,    54,    21,    79,    80,    81,    88,     5,    11,
      15,   112,     5,    23,    50,    52,    33,    96,   112,    62,
      62,    78,    78,    23,     5,    52,     9,    66,   112,   112,
     105,    72,    91,    91,    55,    81,    76,     5,     5,    10,
      10,    21,    91,    91,    76
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    67,    67,    68,    68,
      68,    68,    68,    69,    69,    70,    70,    71,    72,    72,
      72,    72,    73,    73,    73,    74,    74,    75,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    78,    78,    79,
      80,    80,    81,    81,    82,    82,    83,    84,    84,    84,
      84,    85,    85,    86,    86,    87,    87,    87,    87,    88,
      88,    89,    90,    90,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    94,
      95,    95,    96,    96,    97,    98,    98,    98,    98,    99,
      99,   100,   101,   102,   102,   103,   104,   104,   105,   105,
     106,   106,   107,   108,   109,   109,   110,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   114,   114,   114,   114,   114,   114,   115,
     115,   115,   115,   115,   115,   115,   115,   116,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     3,     0,     2,     2,     0,     3,     2,
       2,     2,     1,     3,     1,     2,     1,     4,     1,     1,
       2,     2,     1,     1,     1,     2,     1,     4,     1,     2,
       7,     4,     4,     2,     1,     0,     3,     1,     3,     1,
       3,     1,     3,     0,     2,     1,     4,     6,     8,     6,
       8,     3,     0,     3,     1,     2,     4,     4,     3,     3,
       1,     3,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     4,
       3,     1,     1,     3,     3,     4,     3,     2,     1,     3,
       1,     3,     2,     4,     6,     5,     3,     1,     3,     0,
       3,     1,     4,     4,     8,     8,     4,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     3,     2,     3,     1,     0
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
#line 146 "pascal.y"
        { tree_t *body = (yyvsp[-1].node);
          if (g_pas_narray > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_narray; i++) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_fill(g_pas_arrays[i].high)));
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          tree_t *mainp = mk_proc("main", NULL, body, 0); emit_proc(&g_pascal_procs, mainp);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
#line 1519 "pascal.tab.c"
    break;

  case 5: /* block: decl_part_list body  */
#line 164 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1525 "pascal.tab.c"
    break;

  case 17: /* const_decl: IDENT EQOP constant SEMICOLON  */
#line 185 "pascal.y"
                                          { pas_const_add((yyvsp[-3].str), (yyvsp[-1].ival)); }
#line 1531 "pascal.tab.c"
    break;

  case 18: /* constant: STRINGCONST  */
#line 187 "pascal.y"
                { (yyval.ival) = 0; }
#line 1537 "pascal.tab.c"
    break;

  case 19: /* constant: scalar_constant  */
#line 187 "pascal.y"
                                              { (yyval.ival) = (yyvsp[0].ival); }
#line 1543 "pascal.tab.c"
    break;

  case 20: /* constant: PLUS scalar_constant  */
#line 187 "pascal.y"
                                                                                  { (yyval.ival) = (yyvsp[0].ival); }
#line 1549 "pascal.tab.c"
    break;

  case 21: /* constant: MINUS scalar_constant  */
#line 187 "pascal.y"
                                                                                                                       { (yyval.ival) = -(yyvsp[0].ival); }
#line 1555 "pascal.tab.c"
    break;

  case 22: /* scalar_constant: IDENT  */
#line 188 "pascal.y"
                       { long long cv = 0; pas_const_get((yyvsp[0].str), &cv); (yyval.ival) = cv; }
#line 1561 "pascal.tab.c"
    break;

  case 23: /* scalar_constant: INTCONST  */
#line 188 "pascal.y"
                                                                                         { (yyval.ival) = (yyvsp[0].ival); }
#line 1567 "pascal.tab.c"
    break;

  case 24: /* scalar_constant: REALCONST  */
#line 188 "pascal.y"
                                                                                                                  { (yyval.ival) = (long long)(yyvsp[0].dval); }
#line 1573 "pascal.tab.c"
    break;

  case 28: /* type: simple_type  */
#line 195 "pascal.y"
                { (yyval.ival) = -1; }
#line 1579 "pascal.tab.c"
    break;

  case 29: /* type: ARROW IDENT  */
#line 196 "pascal.y"
                  { (yyval.ival) = -1; }
#line 1585 "pascal.tab.c"
    break;

  case 30: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type  */
#line 197 "pascal.y"
                                                             { (yyval.ival) = (yyvsp[-3].ival); }
#line 1591 "pascal.tab.c"
    break;

  case 31: /* type: packed_opt RECORDSY record_body ENDSY  */
#line 198 "pascal.y"
                                            { (yyval.ival) = -1; }
#line 1597 "pascal.tab.c"
    break;

  case 32: /* type: packed_opt SETSY OFSY simple_type  */
#line 199 "pascal.y"
                                        { (yyval.ival) = -1; }
#line 1603 "pascal.tab.c"
    break;

  case 33: /* type: packed_opt FILESY  */
#line 200 "pascal.y"
                        { (yyval.ival) = -1; }
#line 1609 "pascal.tab.c"
    break;

  case 36: /* simple_type: LPARENT id_list RPARENT  */
#line 204 "pascal.y"
                            { (yyval.ival) = -1; }
#line 1615 "pascal.tab.c"
    break;

  case 37: /* simple_type: IDENT  */
#line 205 "pascal.y"
            { (yyval.ival) = -1; }
#line 1621 "pascal.tab.c"
    break;

  case 38: /* simple_type: constant DOTDOT constant  */
#line 206 "pascal.y"
                               { (yyval.ival) = (yyvsp[0].ival); }
#line 1627 "pascal.tab.c"
    break;

  case 46: /* var_decl: id_list COLON type SEMICOLON  */
#line 223 "pascal.y"
                                       { if ((yyvsp[-1].ival) >= 0 && (yyvsp[-3].list)) for (int i = 0; i < (yyvsp[-3].list)->count; i++) { tree_t *id = (yyvsp[-3].list)->items[i]; if (id && id->v.sval) pas_array_add(id->v.sval, (yyvsp[-1].ival)); } }
#line 1633 "pascal.tab.c"
    break;

  case 47: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 225 "pascal.y"
                                                                       { }
#line 1639 "pascal.tab.c"
    break;

  case 48: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON  */
#line 226 "pascal.y"
                                                                                    { }
#line 1645 "pascal.tab.c"
    break;

  case 49: /* procedure_decl: PROCEDURESY IDENT parameter_list_opt SEMICOLON block SEMICOLON  */
#line 228 "pascal.y"
        { emit_proc(&g_pascal_procs, mk_proc((yyvsp[-4].str), (yyvsp[-3].list), (yyvsp[-1].node), 0)); }
#line 1651 "pascal.tab.c"
    break;

  case 50: /* procedure_decl: FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON block SEMICOLON  */
#line 230 "pascal.y"
        { emit_proc(&g_pascal_procs, mk_proc((yyvsp[-6].str), (yyvsp[-5].list), (yyvsp[-1].node), 1)); }
#line 1657 "pascal.tab.c"
    break;

  case 51: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 233 "pascal.y"
                                        { (yyval.list) = (yyvsp[-1].list); }
#line 1663 "pascal.tab.c"
    break;

  case 52: /* parameter_list_opt: %empty  */
#line 234 "pascal.y"
      { (yyval.list) = pnl_new(); }
#line 1669 "pascal.tab.c"
    break;

  case 53: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 237 "pascal.y"
                                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1675 "pascal.tab.c"
    break;

  case 54: /* parameter_decl_list: parameter_decl  */
#line 238 "pascal.y"
                     { (yyval.list) = (yyvsp[0].list); }
#line 1681 "pascal.tab.c"
    break;

  case 55: /* parameter_decl: PROCEDURESY id_list  */
#line 241 "pascal.y"
                        { (yyval.list) = (yyvsp[0].list); }
#line 1687 "pascal.tab.c"
    break;

  case 56: /* parameter_decl: FUNCTIONSY id_list COLON IDENT  */
#line 242 "pascal.y"
                                     { (yyval.list) = (yyvsp[-2].list); }
#line 1693 "pascal.tab.c"
    break;

  case 57: /* parameter_decl: VARSY id_list COLON IDENT  */
#line 243 "pascal.y"
                                { for (int i = 0; i < (yyvsp[-2].list)->count; i++) if ((yyvsp[-2].list)->items[i]) ast_push((yyvsp[-2].list)->items[i], ast_node_new(TT_SUCCEED)); (yyval.list) = (yyvsp[-2].list); }
#line 1699 "pascal.tab.c"
    break;

  case 58: /* parameter_decl: id_list COLON IDENT  */
#line 244 "pascal.y"
                          { (yyval.list) = (yyvsp[-2].list); }
#line 1705 "pascal.tab.c"
    break;

  case 59: /* id_list: id_list COMMA IDENT  */
#line 247 "pascal.y"
                        { pnl_push((yyvsp[-2].list), leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = (yyvsp[-2].list); }
#line 1711 "pascal.tab.c"
    break;

  case 60: /* id_list: IDENT  */
#line 248 "pascal.y"
            { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, (yyvsp[0].str))); (yyval.list) = l; }
#line 1717 "pascal.tab.c"
    break;

  case 61: /* body: BEGINSY statement_list ENDSY  */
#line 251 "pascal.y"
                                 { (yyval.node) = prog_of((yyvsp[-1].list)); }
#line 1723 "pascal.tab.c"
    break;

  case 62: /* statement_list: statement_list SEMICOLON statement  */
#line 254 "pascal.y"
                                       { if ((yyvsp[0].node)) pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1729 "pascal.tab.c"
    break;

  case 63: /* statement_list: statement  */
#line 255 "pascal.y"
                { PNodeList *l = pnl_new(); if ((yyvsp[0].node)) pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 1735 "pascal.tab.c"
    break;

  case 64: /* statement: statement_no_label  */
#line 258 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1741 "pascal.tab.c"
    break;

  case 65: /* statement: INTCONST COLON statement_no_label  */
#line 259 "pascal.y"
                                        { (yyval.node) = (yyvsp[0].node); }
#line 1747 "pascal.tab.c"
    break;

  case 66: /* statement_no_label: assignment  */
#line 262 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1753 "pascal.tab.c"
    break;

  case 67: /* statement_no_label: call  */
#line 263 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 1759 "pascal.tab.c"
    break;

  case 68: /* statement_no_label: compound_statement  */
#line 264 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1765 "pascal.tab.c"
    break;

  case 69: /* statement_no_label: goto_statement  */
#line 265 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1771 "pascal.tab.c"
    break;

  case 70: /* statement_no_label: if_statement  */
#line 266 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1777 "pascal.tab.c"
    break;

  case 71: /* statement_no_label: case_statement  */
#line 267 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1783 "pascal.tab.c"
    break;

  case 72: /* statement_no_label: while_statement  */
#line 268 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1789 "pascal.tab.c"
    break;

  case 73: /* statement_no_label: repeat_statement  */
#line 269 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1795 "pascal.tab.c"
    break;

  case 74: /* statement_no_label: for_statement  */
#line 270 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1801 "pascal.tab.c"
    break;

  case 75: /* statement_no_label: with_statement  */
#line 271 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1807 "pascal.tab.c"
    break;

  case 76: /* statement_no_label: %empty  */
#line 272 "pascal.y"
      { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 1813 "pascal.tab.c"
    break;

  case 77: /* call: IDENT  */
#line 275 "pascal.y"
          { (yyval.node) = mk_call((yyvsp[0].str), NULL); }
#line 1819 "pascal.tab.c"
    break;

  case 78: /* call: call_with_args  */
#line 276 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1825 "pascal.tab.c"
    break;

  case 79: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 279 "pascal.y"
                                        { (yyval.node) = mk_call((yyvsp[-3].str), (yyvsp[-1].list)); }
#line 1831 "pascal.tab.c"
    break;

  case 80: /* argument_list: argument_list COMMA argument  */
#line 282 "pascal.y"
                                 { (yyval.list) = pnl_concat((yyvsp[-2].list), (yyvsp[0].list)); }
#line 1837 "pascal.tab.c"
    break;

  case 81: /* argument_list: argument  */
#line 283 "pascal.y"
               { (yyval.list) = (yyvsp[0].list); }
#line 1843 "pascal.tab.c"
    break;

  case 82: /* argument: expression  */
#line 286 "pascal.y"
               { PNodeList *_al = pnl_new(); pnl_push(_al, (yyvsp[0].node)); pnl_push(_al, ilit(-1)); (yyval.list) = _al; }
#line 1849 "pascal.tab.c"
    break;

  case 83: /* argument: expression COLON expression  */
#line 287 "pascal.y"
                                  { PNodeList *_al = pnl_new(); pnl_push(_al, (yyvsp[-2].node)); pnl_push(_al, (yyvsp[0].node)); (yyval.list) = _al; }
#line 1855 "pascal.tab.c"
    break;

  case 84: /* assignment: selector BECOMES expression  */
#line 290 "pascal.y"
                                { (yyval.node) = bin(TT_ASSIGN, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1861 "pascal.tab.c"
    break;

  case 85: /* selector: selector LBRACK expression_list RBRACK  */
#line 293 "pascal.y"
                                           { tree_t *e = ast_node_new(TT_IDX); ast_push(e, (yyvsp[-3].node)); if ((yyvsp[-1].list)) for (int i = 0; i < (yyvsp[-1].list)->count; i++) ast_push(e, (yyvsp[-1].list)->items[i]); (yyval.node) = e; }
#line 1867 "pascal.tab.c"
    break;

  case 86: /* selector: selector PERIOD IDENT  */
#line 294 "pascal.y"
                            { (yyval.node) = bin(TT_FIELD, (yyvsp[-2].node), leaf_s(TT_VAR, (yyvsp[0].str))); }
#line 1873 "pascal.tab.c"
    break;

  case 87: /* selector: selector ARROW  */
#line 295 "pascal.y"
                     { (yyval.node) = (yyvsp[-1].node); }
#line 1879 "pascal.tab.c"
    break;

  case 88: /* selector: IDENT  */
#line 296 "pascal.y"
            { (yyval.node) = mk_ident((yyvsp[0].str)); }
#line 1885 "pascal.tab.c"
    break;

  case 89: /* expression_list: expression_list COMMA expression  */
#line 299 "pascal.y"
                                     { pnl_push((yyvsp[-2].list), (yyvsp[0].node)); (yyval.list) = (yyvsp[-2].list); }
#line 1891 "pascal.tab.c"
    break;

  case 90: /* expression_list: expression  */
#line 300 "pascal.y"
                 { PNodeList *l = pnl_new(); pnl_push(l, (yyvsp[0].node)); (yyval.list) = l; }
#line 1897 "pascal.tab.c"
    break;

  case 91: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 303 "pascal.y"
                                 { (yyval.node) = seq_of((yyvsp[-1].list)); }
#line 1903 "pascal.tab.c"
    break;

  case 92: /* goto_statement: GOTOSY INTCONST  */
#line 306 "pascal.y"
                    { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 1909 "pascal.tab.c"
    break;

  case 93: /* if_statement: IFSY expression THENSY statement  */
#line 309 "pascal.y"
                                     { (yyval.node) = bin(TT_IF, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 1915 "pascal.tab.c"
    break;

  case 94: /* if_statement: IFSY expression THENSY statement ELSESY statement  */
#line 310 "pascal.y"
                                                        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond((yyvsp[-4].node))); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 1921 "pascal.tab.c"
    break;

  case 95: /* case_statement: CASESY expression OFSY case_list ENDSY  */
#line 313 "pascal.y"
                                           { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 1927 "pascal.tab.c"
    break;

  case 102: /* while_statement: WHILESY expression DOSY statement  */
#line 328 "pascal.y"
                                      { (yyval.node) = bin(TT_WHILE, pas_cond((yyvsp[-2].node)), (yyvsp[0].node)); }
#line 1933 "pascal.tab.c"
    break;

  case 103: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 331 "pascal.y"
                                               { (yyval.node) = bin(TT_REPEAT, seq_of((yyvsp[-2].list)), pas_cond((yyvsp[0].node))); }
#line 1939 "pascal.tab.c"
    break;

  case 104: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY statement  */
#line 335 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); (yyval.node) = e; }
#line 1945 "pascal.tab.c"
    break;

  case 105: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement  */
#line 337 "pascal.y"
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, (yyvsp[-6].str))); ast_push(e, (yyvsp[-4].node)); ast_push(e, (yyvsp[-2].node)); ast_push(e, (yyvsp[0].node)); e->v.ival = 1; (yyval.node) = e; }
#line 1951 "pascal.tab.c"
    break;

  case 106: /* with_statement: WITHSY selector_list DOSY statement  */
#line 340 "pascal.y"
                                        { (yyval.node) = (yyvsp[0].node); }
#line 1957 "pascal.tab.c"
    break;

  case 109: /* expression: simple_expression  */
#line 347 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1963 "pascal.tab.c"
    break;

  case 110: /* expression: expression INOP simple_expression  */
#line 348 "pascal.y"
                                        { (yyval.node) = bin(TT_FNC, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1969 "pascal.tab.c"
    break;

  case 111: /* expression: expression LTOP simple_expression  */
#line 349 "pascal.y"
                                        { (yyval.node) = bin(TT_LT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1975 "pascal.tab.c"
    break;

  case 112: /* expression: expression LEOP simple_expression  */
#line 350 "pascal.y"
                                        { (yyval.node) = bin(TT_LE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1981 "pascal.tab.c"
    break;

  case 113: /* expression: expression GTOP simple_expression  */
#line 351 "pascal.y"
                                        { (yyval.node) = bin(TT_GT, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1987 "pascal.tab.c"
    break;

  case 114: /* expression: expression GEOP simple_expression  */
#line 352 "pascal.y"
                                        { (yyval.node) = bin(TT_GE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1993 "pascal.tab.c"
    break;

  case 115: /* expression: expression NEOP simple_expression  */
#line 353 "pascal.y"
                                        { (yyval.node) = bin(TT_NE, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1999 "pascal.tab.c"
    break;

  case 116: /* expression: expression EQOP simple_expression  */
#line 354 "pascal.y"
                                        { (yyval.node) = bin(TT_EQ, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2005 "pascal.tab.c"
    break;

  case 117: /* simple_expression: term  */
#line 357 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 2011 "pascal.tab.c"
    break;

  case 118: /* simple_expression: PLUS term  */
#line 358 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2017 "pascal.tab.c"
    break;

  case 119: /* simple_expression: MINUS term  */
#line 359 "pascal.y"
                 { (yyval.node) = un(TT_MNS, (yyvsp[0].node)); }
#line 2023 "pascal.tab.c"
    break;

  case 120: /* simple_expression: simple_expression PLUS term  */
#line 360 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2029 "pascal.tab.c"
    break;

  case 121: /* simple_expression: simple_expression MINUS term  */
#line 361 "pascal.y"
                                   { (yyval.node) = bin(TT_SUB, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2035 "pascal.tab.c"
    break;

  case 122: /* simple_expression: simple_expression OROP term  */
#line 362 "pascal.y"
                                  { (yyval.node) = bin(TT_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2041 "pascal.tab.c"
    break;

  case 123: /* term: factor  */
#line 365 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2047 "pascal.tab.c"
    break;

  case 124: /* term: term MUL factor  */
#line 366 "pascal.y"
                      { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2053 "pascal.tab.c"
    break;

  case 125: /* term: term RDIV factor  */
#line 367 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2059 "pascal.tab.c"
    break;

  case 126: /* term: term IDIV factor  */
#line 368 "pascal.y"
                       { (yyval.node) = bin(TT_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2065 "pascal.tab.c"
    break;

  case 127: /* term: term IMOD factor  */
#line 369 "pascal.y"
                       { (yyval.node) = bin(TT_MOD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2071 "pascal.tab.c"
    break;

  case 128: /* term: term ANDOP factor  */
#line 370 "pascal.y"
                        { (yyval.node) = bin(TT_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2077 "pascal.tab.c"
    break;

  case 129: /* factor: selector  */
#line 373 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2083 "pascal.tab.c"
    break;

  case 130: /* factor: call_with_args  */
#line 374 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2089 "pascal.tab.c"
    break;

  case 131: /* factor: INTCONST  */
#line 375 "pascal.y"
               { (yyval.node) = ilit((yyvsp[0].ival)); }
#line 2095 "pascal.tab.c"
    break;

  case 132: /* factor: REALCONST  */
#line 376 "pascal.y"
                { (yyval.node) = flit((yyvsp[0].dval)); }
#line 2101 "pascal.tab.c"
    break;

  case 133: /* factor: STRINGCONST  */
#line 377 "pascal.y"
                  { (yyval.node) = leaf_s(TT_QLIT, (yyvsp[0].str)); }
#line 2107 "pascal.tab.c"
    break;

  case 134: /* factor: LPARENT expression RPARENT  */
#line 378 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2113 "pascal.tab.c"
    break;

  case 135: /* factor: NOTSY factor  */
#line 379 "pascal.y"
                   { (yyval.node) = un(TT_NOT, (yyvsp[0].node)); }
#line 2119 "pascal.tab.c"
    break;

  case 136: /* factor: LBRACK expression_list_opt RBRACK  */
#line 380 "pascal.y"
                                        { (yyval.node) = ast_node_new(TT_SUCCEED); }
#line 2125 "pascal.tab.c"
    break;


#line 2129 "pascal.tab.c"

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

#line 386 "pascal.y"

extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
