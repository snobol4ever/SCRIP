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
  YYSYMBOL_CARET = 15,                     /* CARET  */
  YYSYMBOL_VAR_CAPTURE = 16,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 17,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 18,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_MY = 19,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 20,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 21,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 22,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 23,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 24,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 25,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 26,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 27,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 28,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 29,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 30,                 /* KW_RETURN  */
  YYSYMBOL_KW_GIVEN = 31,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 32,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 33,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 34,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 35,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 36,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 37,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 38,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 39,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 40,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 41,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 42,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 43,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 44,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 45,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 46,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 47,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 48,                    /* KW_NEW  */
  YYSYMBOL_KW_GRAMMAR = 49,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 50,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 51,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 52,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 53,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 54,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 55,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 56,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 57,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 58,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 59,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 60,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 61,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 62,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 63,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 64,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 65,                   /* OP_BIND  */
  YYSYMBOL_OP_SMATCH = 66,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 67,                    /* OP_DIV  */
  YYSYMBOL_68_ = 68,                       /* '='  */
  YYSYMBOL_69_ = 69,                       /* '!'  */
  YYSYMBOL_70_ = 70,                       /* '<'  */
  YYSYMBOL_71_ = 71,                       /* '>'  */
  YYSYMBOL_72_ = 72,                       /* '|'  */
  YYSYMBOL_73_ = 73,                       /* '&'  */
  YYSYMBOL_74_ = 74,                       /* '~'  */
  YYSYMBOL_75_ = 75,                       /* '+'  */
  YYSYMBOL_76_ = 76,                       /* '-'  */
  YYSYMBOL_77_ = 77,                       /* '*'  */
  YYSYMBOL_78_ = 78,                       /* '/'  */
  YYSYMBOL_79_ = 79,                       /* '%'  */
  YYSYMBOL_UMINUS = 80,                    /* UMINUS  */
  YYSYMBOL_81_ = 81,                       /* '.'  */
  YYSYMBOL_82_ = 82,                       /* ';'  */
  YYSYMBOL_83_ = 83,                       /* ','  */
  YYSYMBOL_84_ = 84,                       /* '('  */
  YYSYMBOL_85_ = 85,                       /* ')'  */
  YYSYMBOL_86_ = 86,                       /* '['  */
  YYSYMBOL_87_ = 87,                       /* ']'  */
  YYSYMBOL_88_ = 88,                       /* '{'  */
  YYSYMBOL_89_ = 89,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 90,                  /* $accept  */
  YYSYMBOL_program = 91,                   /* program  */
  YYSYMBOL_stmt_list = 92,                 /* stmt_list  */
  YYSYMBOL_stmt = 93,                      /* stmt  */
  YYSYMBOL_if_stmt = 94,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 95,                /* while_stmt  */
  YYSYMBOL_unless_stmt = 96,               /* unless_stmt  */
  YYSYMBOL_until_stmt = 97,                /* until_stmt  */
  YYSYMBOL_repeat_stmt = 98,               /* repeat_stmt  */
  YYSYMBOL_for_stmt = 99,                  /* for_stmt  */
  YYSYMBOL_given_stmt = 100,               /* given_stmt  */
  YYSYMBOL_when_list = 101,                /* when_list  */
  YYSYMBOL_sub_decl = 102,                 /* sub_decl  */
  YYSYMBOL_class_decl = 103,               /* class_decl  */
  YYSYMBOL_class_body_list = 104,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 105,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 106,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 107,           /* named_arg_list  */
  YYSYMBOL_param_list = 108,               /* param_list  */
  YYSYMBOL_block = 109,                    /* block  */
  YYSYMBOL_closure = 110,                  /* closure  */
  YYSYMBOL_expr = 111,                     /* expr  */
  YYSYMBOL_cmp_expr = 112,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 113,                 /* jct_expr  */
  YYSYMBOL_range_expr = 114,               /* range_expr  */
  YYSYMBOL_add_expr = 115,                 /* add_expr  */
  YYSYMBOL_mul_expr = 116,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 117,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 118,             /* postfix_expr  */
  YYSYMBOL_call_expr = 119,                /* call_expr  */
  YYSYMBOL_arg_list = 120,                 /* arg_list  */
  YYSYMBOL_atom = 121                      /* atom  */
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
#define YYLAST   1065

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  90
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  170
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  434

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   323


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
       2,     2,     2,    69,     2,     2,     2,    79,    73,     2,
      84,    85,    77,    75,    83,    76,    81,    78,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    82,
      70,    68,    71,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    86,     2,    87,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    88,    72,    89,    74,     2,     2,     2,
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
      65,    66,    67,    80
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   194,   194,   205,   206,   209,   211,   213,   217,   221,
     223,   225,   227,   231,   235,   237,   239,   241,   243,   245,
     247,   249,   251,   253,   255,   257,   259,   264,   268,   275,
     280,   283,   286,   289,   292,   295,   296,   297,   298,   299,
     300,   302,   304,   305,   306,   307,   308,   309,   312,   314,
     316,   320,   324,   326,   330,   334,   338,   344,   350,   355,
     360,   368,   379,   380,   386,   394,   402,   415,   434,   435,
     438,   441,   444,   447,   450,   453,   456,   459,   465,   471,
     477,   483,   493,   501,   510,   519,   534,   535,   540,   545,
     552,   556,   562,   563,   566,   569,   572,   573,   578,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     597,   603,   609,   612,   613,   614,   617,   618,   619,   622,
     623,   624,   625,   628,   629,   630,   631,   632,   635,   636,
     637,   639,   641,   646,   647,   653,   659,   663,   670,   675,
     681,   687,   694,   701,   706,   711,   713,   715,   717,   719,
     721,   724,   725,   728,   729,   730,   731,   732,   733,   734,
     735,   739,   743,   746,   748,   750,   752,   754,   756,   757,
     761
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
  "IDENT", "CARET", "VAR_CAPTURE", "VAR_FH", "VAR_NAMED_CAPTURE", "KW_MY",
  "KW_SAY", "KW_PRINT", "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE",
  "KW_FOR", "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS",
  "KW_UNTIL", "KW_REPEAT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY",
  "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND", "OP_OR", "OP_BIND", "OP_SMATCH",
  "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['",
  "']'", "'{'", "'}'", "$accept", "program", "stmt_list", "stmt",
  "if_stmt", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "for_stmt", "given_stmt", "when_list", "sub_decl", "class_decl",
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

#define YYPACT_NINF (-214)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -214,    13,   658,  -214,  -214,  -214,  -214,  -214,   -32,   -56,
     -45,   -26,    12,  -214,  -214,  -214,   147,   751,   797,   -46,
     -27,   843,    73,    14,   843,   705,   843,    80,   114,    55,
      78,    14,    49,    49,   176,    14,   843,   157,   161,   981,
     981,   843,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,
    -214,  -214,  -214,    96,   121,   262,  -214,    53,    87,  -214,
    -214,  -214,   110,   843,   181,   843,   197,   843,   843,    30,
     274,   194,   215,   229,   120,   232,   160,   -33,  -214,   843,
     183,   843,   199,   843,   843,   -42,    66,   166,  -214,  -214,
     207,  -214,   225,   223,   -30,   -29,   843,   843,  -214,   843,
     843,   843,   843,  -214,   269,  -214,     3,   228,  -214,  -214,
    -214,   241,  -214,   981,   981,   981,   981,   981,   981,   981,
     981,   136,   981,   981,   981,   981,   981,   981,   981,   981,
     981,   981,   981,   981,   981,   204,   235,    33,   240,   258,
     242,   248,   252,   323,   254,  -214,  -214,   -21,   843,   889,
     843,    43,    54,    56,   843,   843,   325,   843,   -16,  -214,
     129,  -214,   259,   261,   337,  -214,   981,   981,    -9,   409,
    -214,  -214,  -214,   334,   843,   338,   843,   266,   268,   265,
    -214,  -214,  -214,    14,   341,  -214,  -214,  -214,   182,   182,
     182,   182,   182,   182,   182,   182,  -214,  -214,  -214,   182,
     182,  -214,  -214,    94,    94,    87,    87,    87,  -214,  -214,
    -214,  -214,   273,   342,  -214,   843,   456,   292,   293,   294,
    -214,   503,  -214,   550,   843,  -214,   281,   843,   158,   282,
     843,  -214,   935,  -214,   843,  -214,  -214,   278,   295,   279,
     843,   843,    14,    14,    14,   -23,    59,  -214,    14,   138,
    -214,    -6,   296,   280,   299,   283,    14,    14,  -214,  -214,
     285,   -18,    62,   597,  -214,   289,   297,   141,   843,   843,
     843,  -214,   144,    35,  -214,   148,   151,  -214,  -214,   152,
    -214,   843,  -214,   298,   843,   211,   301,  -214,  -214,  -214,
     290,   291,   351,  -214,  -214,   367,   368,  -214,   371,    14,
     843,    14,  -214,  -214,  -214,   302,   304,   364,  -214,  -214,
       6,   163,  -214,   374,   375,   376,  -214,  -214,   164,   168,
    -214,  -214,   309,   311,   312,   313,  -214,   843,   382,  -214,
    -214,   843,   213,  -214,   178,  -214,   843,  -214,   315,   317,
     -15,    14,    14,  -214,  -214,    14,   314,  -214,  -214,    14,
       1,   318,   320,   -12,    -8,   150,   394,   399,   400,  -214,
    -214,  -214,  -214,  -214,  -214,  -214,   356,   186,  -214,   843,
     221,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,  -214,
    -214,    -5,    -1,   396,   843,  -214,   397,   843,  -214,     4,
       7,  -214,  -214,  -214,   843,   335,   187,  -214,    14,   190,
      14,   191,   350,   359,   360,   370,   410,   843,  -214,   419,
     843,  -214,  -214,  -214,   373,  -214,    14,  -214,    14,  -214,
    -214,  -214,  -214,   381,   383,   389,   393,  -214,  -214,  -214,
    -214,  -214,  -214,  -214
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   153,   154,   155,   156,   157,   158,
     159,   169,   168,   160,   161,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,    36,    37,    42,    43,    44,    38,    39,
      45,    46,    47,     0,    98,   112,   115,   118,   122,   127,
     130,   131,   150,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,   158,   159,   169,     0,
       0,     0,     0,     0,     0,     0,   118,     0,     3,    97,
       0,    24,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,   148,    40,   145,     0,     0,   157,   129,
     128,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   133,   151,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    18,
       0,    20,     0,     0,     0,    59,     0,     0,     0,     0,
      22,    23,    62,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   149,     0,     0,    68,    86,   170,    99,   100,
     101,   102,   105,   106,   107,   108,   109,   110,   111,   103,
     104,   113,   114,   116,   117,   121,   119,   120,   126,   123,
     124,   125,   144,     0,    25,     0,     0,   163,   164,   165,
      27,     0,   139,     0,     0,   132,     0,     0,     0,     0,
       0,    15,     0,    16,     0,    17,    96,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,    92,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,    95,    41,
       0,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,   138,     0,   168,   136,     0,     0,   152,     5,     0,
       6,     0,     9,     0,     0,     0,     0,   163,   164,   165,
       0,     0,    48,    51,    58,     0,     0,    65,     0,     0,
       0,     0,    60,   166,   167,     0,     0,    52,    54,    68,
       0,     0,    66,     0,     0,     0,    85,   143,     0,     0,
      26,    29,     0,     0,     0,     0,   137,     0,     0,   134,
     135,     0,     0,    10,     0,    11,     0,    14,     0,     0,
       0,     0,     0,    93,    64,     0,     0,    33,    34,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
     141,    28,    30,    31,    32,    90,     0,     0,     7,     0,
       0,    19,    21,    50,    49,    56,    57,    63,    61,    53,
      67,     0,     0,     0,     0,    70,     0,     0,    69,     0,
       0,    87,    88,    89,     0,     0,     0,    12,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    72,     0,
       0,    71,    91,     8,     0,    82,     0,    84,     0,    78,
      74,    77,    73,     0,     0,     0,     0,    13,    81,    83,
      80,    76,    79,    75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -214,  -214,   369,  -214,   116,  -214,  -214,  -214,  -214,  -214,
    -214,  -214,  -214,  -214,   155,  -214,  -214,   214,  -123,     0,
     272,    -2,  -214,    83,   185,   -17,    79,   -28,  -214,  -214,
    -213,  -214
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    42,    43,    44,    45,    46,    47,    48,
      49,   251,    50,    51,   261,    52,   262,   275,   249,    89,
     100,   146,    54,    55,    56,    57,    58,    59,    60,    61,
     147,    62
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,   247,   383,   267,    86,   247,   386,    19,   272,   247,
     276,   109,   110,     3,   164,    80,    82,   184,   406,    85,
     351,   409,    90,    92,    93,    66,   300,   301,   310,   311,
      65,    98,   103,   295,   105,   104,    63,   156,    83,   111,
     173,   175,    68,    67,   142,   143,    88,   310,   311,    64,
     319,   128,   129,   130,   352,   157,   384,    84,   174,   176,
     387,   136,   224,   138,   225,   140,   141,   240,   332,   187,
     385,   312,   407,    88,   388,   410,   248,   158,   144,   160,
     398,   162,   163,   302,   400,   165,   408,    87,   327,   411,
     380,   185,    94,    69,   177,   178,    70,   179,   180,   181,
     182,   215,    88,   208,   209,   210,   211,   126,   127,   203,
     204,   230,   313,   314,   315,   296,    69,   216,   367,    70,
     166,   167,   232,   370,   234,   231,    95,   128,   129,   130,
     151,   152,   153,   128,   129,   130,   233,    99,   235,    96,
     128,   129,   130,   196,   197,   198,   226,   228,   229,   245,
     246,   316,   236,   237,   131,   239,   396,    71,    72,    73,
     389,    74,    97,   390,   132,   133,   134,    53,   128,   129,
     130,   106,   253,   353,   255,   107,   354,   355,   112,     4,
       5,     6,     7,   259,   113,   114,    75,    76,    77,    78,
      12,   135,    13,    14,    15,   137,   188,   189,   190,   191,
     192,   193,   194,   195,    23,   199,   200,   205,   206,   207,
      27,   139,   241,   265,   187,    32,    33,    34,   212,   213,
      36,   298,   277,   299,   224,   279,   322,   224,   283,   326,
     285,   328,   286,   329,   224,   331,   330,   187,   290,   291,
     280,   281,   292,   293,   294,    39,   155,   328,   297,   359,
     168,   224,    40,   360,   124,   125,   307,   308,   399,   401,
      41,   369,   148,   187,    99,   159,   323,   324,   325,   224,
     224,   395,   414,   298,   298,   416,   418,     4,     5,     6,
       7,   161,   334,   149,    75,    76,    77,    78,    12,   170,
      13,    14,    15,   335,   336,   368,   224,   150,   345,   344,
     154,   346,    23,   397,   224,   101,   102,   171,    27,   201,
     202,   172,   183,    32,    33,    34,   186,   214,    36,   115,
     116,   117,   118,   119,   120,   365,   187,   217,   121,   218,
     220,   219,   122,   123,   124,   125,   221,   222,   223,   238,
     374,   375,   376,    39,   242,   377,   243,   244,   252,   379,
      40,   256,   254,   257,   258,   260,   264,   263,    41,   145,
     268,   269,   270,   278,   282,   287,   288,   303,   289,   304,
     305,   320,   306,   309,   340,   338,   339,   341,   342,   321,
     333,   343,   403,   337,   347,   405,   348,   349,   356,   357,
     358,   361,   412,   362,   363,   364,   366,   371,   415,   372,
     417,   391,   381,   378,   382,   424,   392,   393,   426,   394,
     402,   404,     4,     5,     6,     7,   428,   413,   429,     8,
       9,    10,    11,    12,   423,    13,    14,    15,    16,    17,
      18,    19,   419,   425,    20,    21,    22,    23,    24,    25,
      26,   420,   421,    27,    28,    29,    30,    31,    32,    33,
      34,    35,   422,    36,    37,   427,   373,   169,    38,     4,
       5,     6,     7,   430,   350,   431,    75,    76,    77,    78,
      12,   432,    13,    14,    15,   433,     0,   318,    39,     0,
       0,     0,     0,     0,    23,    40,     0,     0,     0,     0,
      27,     0,     0,    41,     0,    32,    33,    34,   250,     0,
      36,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    75,    76,    77,    78,    12,     0,    13,
      14,    15,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    23,    40,     0,     0,     0,     0,    27,     0,     0,
      41,   266,    32,    33,    34,     0,     0,    36,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      75,    76,    77,    78,   273,     0,    13,    14,    15,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    23,    40,
       0,     0,     0,     0,    27,     0,     0,    41,   271,    32,
      33,    34,     0,     0,    36,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    75,    76,    77,
      78,   273,     0,    13,    14,    15,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    23,    40,     0,     0,     0,
       0,    27,     0,     0,    41,   274,    32,    33,    34,     0,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,    39,     0,     8,     9,
      10,    11,    12,    40,    13,    14,    15,    16,    17,    18,
      19,    41,   317,    20,    21,    22,    23,    24,    25,    26,
       0,     0,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,    37,     0,     0,     0,    38,     4,     5,
       6,     7,     0,     0,     0,    75,    76,    77,    78,    12,
       0,    13,    14,    15,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    23,    40,     0,     0,     0,     0,    27,
       0,     0,    41,     0,    32,    33,    34,     0,     0,    36,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    75,    76,    77,    78,    12,     0,    13,    14,    15,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    23,
       0,    40,     0,     0,     0,    27,     0,    91,     0,    41,
      32,    33,    34,     0,     0,    36,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    75,    76,    77,
      78,    12,     0,    13,    14,    15,     0,     0,     0,     0,
      39,     0,     0,     0,     0,    23,     0,    40,     0,     0,
       0,    27,     0,     0,     0,    79,    32,    33,    34,     0,
       0,    36,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    75,    76,    77,    78,    12,     0,    13,
      14,    15,     0,     0,     0,     0,    39,     0,     0,     0,
       0,    23,     0,    40,     0,     0,     0,    27,     0,     0,
       0,    81,    32,    33,    34,     0,     0,    36,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    75,
      76,    77,    78,    12,     0,    13,    14,    15,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    23,     0,    40,
       0,     0,     0,    27,     0,     0,     0,    41,    32,    33,
      34,     0,     0,    36,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    75,    76,    77,    78,    12,
       0,    13,    14,    15,     0,     0,     0,     0,    39,     0,
       0,     0,     0,    23,     0,    40,     0,     0,     0,    27,
       0,     0,     0,   227,    32,    33,    34,     0,     0,    36,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   108,    76,    77,    78,    12,     0,    13,    14,    15,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,    40,     0,     0,     0,    27,     0,     0,     0,   284,
      32,    33,    34,     0,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,    41
};

static const yytype_int16 yycheck[] =
{
       2,    10,    14,   216,    21,    10,    14,    22,   221,    10,
     223,    39,    40,     0,    56,    17,    18,    14,    14,    21,
      14,    14,    24,    25,    26,    70,    32,    33,    46,    47,
      86,    31,    34,    56,    36,    35,    68,    70,    84,    41,
      70,    70,    68,    88,    14,    15,    88,    46,    47,    81,
     263,    74,    75,    76,    48,    88,    68,    84,    88,    88,
      68,    63,    83,    65,    85,    67,    68,    83,   281,    85,
      82,    89,    68,    88,    82,    68,    85,    79,    48,    81,
      85,    83,    84,    89,    85,    85,    82,    14,    53,    82,
      89,    88,    12,    81,    96,    97,    84,    99,   100,   101,
     102,    68,    88,   131,   132,   133,   134,    54,    55,   126,
     127,    68,    50,    51,    52,    56,    81,    84,   331,    84,
      54,    55,    68,   336,    68,    82,    12,    74,    75,    76,
      10,    11,    12,    74,    75,    76,    82,    88,    82,    84,
      74,    75,    76,     7,     8,     9,   148,   149,   150,   166,
     167,    89,   154,   155,    67,   157,   369,    10,    11,    12,
      10,    14,    84,    13,    77,    78,    79,   169,    74,    75,
      76,    14,   174,    10,   176,    14,    13,    14,    82,     3,
       4,     5,     6,   183,    63,    64,    10,    11,    12,    13,
      14,    81,    16,    17,    18,    14,   113,   114,   115,   116,
     117,   118,   119,   120,    28,   122,   123,   128,   129,   130,
      34,    14,    83,   215,    85,    39,    40,    41,    14,    15,
      44,    83,   224,    85,    83,   227,    85,    83,   230,    85,
     232,    83,   234,    85,    83,    83,    85,    85,   240,   241,
      82,    83,   242,   243,   244,    69,    86,    83,   248,    85,
      84,    83,    76,    85,    72,    73,   256,   257,   381,   382,
      84,    83,    68,    85,    88,    82,   268,   269,   270,    83,
      83,    85,    85,    83,    83,    85,    85,     3,     4,     5,
       6,    82,   284,    68,    10,    11,    12,    13,    14,    82,
      16,    17,    18,    82,    83,    82,    83,    68,   300,   299,
      68,   301,    28,    82,    83,    33,    34,    82,    34,   124,
     125,    88,    43,    39,    40,    41,    88,    82,    44,    57,
      58,    59,    60,    61,    62,   327,    85,    87,    66,    71,
      82,    89,    70,    71,    72,    73,    84,    14,    84,    14,
     340,   341,   342,    69,    85,   345,    85,    10,    14,   349,
      76,    85,    14,    85,    89,    14,    14,    84,    84,    85,
      68,    68,    68,    82,    82,    87,    71,    71,    89,    89,
      71,    82,    89,    88,    23,    85,    85,    10,    10,    82,
      82,    10,   384,    82,    82,   387,    82,    23,    14,    14,
      14,    82,   394,    82,    82,    82,    14,    82,   398,    82,
     400,     7,    84,    89,    84,   407,     7,     7,   410,    53,
      14,    14,     3,     4,     5,     6,   416,    82,   418,    10,
      11,    12,    13,    14,    14,    16,    17,    18,    19,    20,
      21,    22,    82,    14,    25,    26,    27,    28,    29,    30,
      31,    82,    82,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    82,    44,    45,    82,   340,    88,    49,     3,
       4,     5,     6,    82,   309,    82,    10,    11,    12,    13,
      14,    82,    16,    17,    18,    82,    -1,   263,    69,    -1,
      -1,    -1,    -1,    -1,    28,    76,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    84,    -1,    39,    40,    41,    89,    -1,
      44,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    28,    76,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      84,    85,    39,    40,    41,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    28,    76,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    84,    85,    39,
      40,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    28,    76,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    84,    85,    39,    40,    41,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    69,    -1,    10,    11,
      12,    13,    14,    76,    16,    17,    18,    19,    20,    21,
      22,    84,    85,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    28,    76,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    84,    -1,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    28,
      -1,    76,    -1,    -1,    -1,    34,    -1,    82,    -1,    84,
      39,    40,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    28,    -1,    76,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    84,    39,    40,    41,    -1,
      -1,    44,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    28,    -1,    76,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    84,    39,    40,    41,    -1,    -1,    44,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    28,    -1,    76,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    84,    39,    40,
      41,    -1,    -1,    44,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    28,    -1,    76,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    84,    39,    40,    41,    -1,    -1,    44,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    -1,    16,    17,    18,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    34,    -1,    -1,    -1,    84,
      39,    40,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    91,    92,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      25,    26,    27,    28,    29,    30,    31,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    44,    45,    49,    69,
      76,    84,    93,    94,    95,    96,    97,    98,    99,   100,
     102,   103,   105,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   121,    68,    81,    86,    70,    88,    68,    81,
      84,    10,    11,    12,    14,    10,    11,    12,    13,    84,
     111,    84,   111,    84,    84,   111,   115,    14,    88,   109,
     111,    82,   111,   111,    12,    12,    84,    84,   109,    88,
     110,   110,   110,   111,   109,   111,    14,    14,    10,   117,
     117,   111,    82,    63,    64,    57,    58,    59,    60,    61,
      62,    66,    70,    71,    72,    73,    54,    55,    74,    75,
      76,    67,    77,    78,    79,    81,   111,    14,   111,    14,
     111,   111,    14,    15,    48,    85,   111,   120,    68,    68,
      68,    10,    11,    12,    68,    86,    70,    88,   111,    82,
     111,    82,   111,   111,    56,   109,    54,    55,    84,    92,
      82,    82,    88,    70,    88,    70,    88,   111,   111,   111,
     111,   111,   111,    43,    14,    88,    88,    85,   113,   113,
     113,   113,   113,   113,   113,   113,     7,     8,     9,   113,
     113,   114,   114,   115,   115,   116,   116,   116,   117,   117,
     117,   117,    14,    15,    82,    68,    84,    87,    71,    89,
      82,    84,    14,    84,    83,    85,   111,    84,   111,   111,
      68,    82,    68,    82,    68,    82,   111,   111,    14,   111,
      83,    83,    85,    85,    10,   115,   115,    10,    85,   108,
      89,   101,    14,   111,    14,   111,    85,    85,    89,   109,
      14,   104,   106,    84,    14,   111,    85,   120,    68,    68,
      68,    85,   120,    14,    85,   107,   120,   111,    82,   111,
      82,    83,    82,   111,    84,   111,   111,    87,    71,    89,
     111,   111,   109,   109,   109,    56,    56,   109,    83,    85,
      32,    33,    89,    71,    89,    71,    89,   109,   109,    88,
      46,    47,    89,    50,    51,    52,    89,    85,   107,   120,
      82,    82,    85,   111,   111,   111,    85,    53,    83,    85,
      85,    83,   120,    82,   111,    82,    83,    82,    85,    85,
      23,    10,    10,    10,   109,   111,   109,    82,    82,    23,
     104,    14,    48,    10,    13,    14,    14,    14,    14,    85,
      85,    82,    82,    82,    82,   111,    14,   120,    82,    83,
     120,    82,    82,    94,   109,   109,   109,   109,    89,   109,
      89,    84,    84,    14,    68,    82,    14,    68,    82,    10,
      13,     7,     7,     7,    53,    85,   120,    82,    85,   108,
      85,   108,    14,   111,    14,   111,    14,    68,    82,    14,
      68,    82,   111,    82,    85,   109,    85,   109,    85,    82,
      82,    82,    82,    14,   111,    14,   111,    82,   109,   109,
      82,    82,    82,    82
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    90,    91,    92,    92,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    94,    94,
      94,    95,    96,    96,    97,    98,    99,    99,    99,    99,
     100,   100,   101,   101,   102,   102,   103,   103,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   105,   106,   106,   106,   106,
     107,   107,   108,   108,   109,   110,   111,   111,   111,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   113,   114,   114,   114,   115,
     115,   115,   115,   116,   116,   116,   116,   116,   117,   117,
     117,   118,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   120,   120,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121
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
       4,     5,     5,     6,     6,     7,     7,     6,     6,     7,
       7,     7,     6,     7,     6,     5,     0,     4,     4,     4,
       3,     5,     1,     3,     3,     3,     3,     2,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     3,     3,     1,     2,     2,
       1,     1,     4,     3,     6,     6,     5,     6,     5,     4,
       4,     6,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     5,     5,     1,     1,
       3
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
#line 195 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1734 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 205 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1740 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 206 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1746 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 210 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1752 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 212 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1758 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 214 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1766 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 218 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1774 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 222 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1780 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 224 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1786 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 226 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1792 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 228 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1800 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 232 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1808 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 236 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1814 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 238 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1820 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 240 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1826 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 242 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1832 "raku.tab.c"
    break;

  case 18: /* stmt: KW_SAY expr ';'  */
#line 244 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1838 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 246 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1844 "raku.tab.c"
    break;

  case 20: /* stmt: KW_PRINT expr ';'  */
#line 248 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1850 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 250 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1856 "raku.tab.c"
    break;

  case 22: /* stmt: KW_TAKE expr ';'  */
#line 252 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1862 "raku.tab.c"
    break;

  case 23: /* stmt: KW_RETURN expr ';'  */
#line 254 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1868 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN ';'  */
#line 256 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1874 "raku.tab.c"
    break;

  case 25: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 258 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1880 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 260 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1889 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 265 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1897 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 269 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 1908 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 276 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 1917 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 281 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1924 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 284 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1931 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 287 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1938 "raku.tab.c"
    break;

  case 33: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 290 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1945 "raku.tab.c"
    break;

  case 34: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 293 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1952 "raku.tab.c"
    break;

  case 35: /* stmt: expr ';'  */
#line 295 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1958 "raku.tab.c"
    break;

  case 36: /* stmt: if_stmt  */
#line 296 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1964 "raku.tab.c"
    break;

  case 37: /* stmt: while_stmt  */
#line 297 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1970 "raku.tab.c"
    break;

  case 38: /* stmt: for_stmt  */
#line 298 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1976 "raku.tab.c"
    break;

  case 39: /* stmt: given_stmt  */
#line 299 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1982 "raku.tab.c"
    break;

  case 40: /* stmt: KW_TRY block  */
#line 301 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1988 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TRY block KW_CATCH block  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1994 "raku.tab.c"
    break;

  case 42: /* stmt: unless_stmt  */
#line 304 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2000 "raku.tab.c"
    break;

  case 43: /* stmt: until_stmt  */
#line 305 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2006 "raku.tab.c"
    break;

  case 44: /* stmt: repeat_stmt  */
#line 306 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2012 "raku.tab.c"
    break;

  case 45: /* stmt: sub_decl  */
#line 307 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2018 "raku.tab.c"
    break;

  case 46: /* stmt: class_decl  */
#line 308 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2024 "raku.tab.c"
    break;

  case 47: /* stmt: grammar_decl  */
#line 309 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2030 "raku.tab.c"
    break;

  case 48: /* if_stmt: KW_IF '(' expr ')' block  */
#line 313 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2036 "raku.tab.c"
    break;

  case 49: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 315 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2042 "raku.tab.c"
    break;

  case 50: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 317 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2048 "raku.tab.c"
    break;

  case 51: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 321 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2054 "raku.tab.c"
    break;

  case 52: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 325 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2060 "raku.tab.c"
    break;

  case 53: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 327 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2066 "raku.tab.c"
    break;

  case 54: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 331 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2072 "raku.tab.c"
    break;

  case 55: /* repeat_stmt: KW_REPEAT block  */
#line 335 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2078 "raku.tab.c"
    break;

  case 56: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 339 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2088 "raku.tab.c"
    break;

  case 57: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 345 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2098 "raku.tab.c"
    break;

  case 58: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 351 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2107 "raku.tab.c"
    break;

  case 59: /* for_stmt: KW_FOR expr block  */
#line 356 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2114 "raku.tab.c"
    break;

  case 60: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 361 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2126 "raku.tab.c"
    break;

  case 61: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 369 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2139 "raku.tab.c"
    break;

  case 62: /* when_list: %empty  */
#line 379 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2145 "raku.tab.c"
    break;

  case 63: /* when_list: when_list KW_WHEN expr block  */
#line 381 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2153 "raku.tab.c"
    break;

  case 64: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 387 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2165 "raku.tab.c"
    break;

  case 65: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 395 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2175 "raku.tab.c"
    break;

  case 66: /* class_decl: KW_CLASS IDENT '{' class_body_list '}'  */
#line 403 "raku.y"
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
#line 2192 "raku.tab.c"
    break;

  case 67: /* class_decl: KW_CLASS IDENT IDENT IDENT '{' class_body_list '}'  */
#line 416 "raku.y"
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
#line 2213 "raku.tab.c"
    break;

  case 68: /* class_body_list: %empty  */
#line 434 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2219 "raku.tab.c"
    break;

  case 69: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 436 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2226 "raku.tab.c"
    break;

  case 70: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 439 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2233 "raku.tab.c"
    break;

  case 71: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 442 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2240 "raku.tab.c"
    break;

  case 72: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 445 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2247 "raku.tab.c"
    break;

  case 73: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 448 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2254 "raku.tab.c"
    break;

  case 74: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 451 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2261 "raku.tab.c"
    break;

  case 75: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 454 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2268 "raku.tab.c"
    break;

  case 76: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 457 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2275 "raku.tab.c"
    break;

  case 77: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 460 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2285 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 466 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2295 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 472 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2305 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 478 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2315 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 484 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2329 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 494 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2341 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 502 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2354 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 511 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2365 "raku.tab.c"
    break;

  case 85: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 520 "raku.y"
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
#line 2382 "raku.tab.c"
    break;

  case 86: /* grammar_body_list: %empty  */
#line 534 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2388 "raku.tab.c"
    break;

  case 87: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 536 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2397 "raku.tab.c"
    break;

  case 88: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 541 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2406 "raku.tab.c"
    break;

  case 89: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 546 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2415 "raku.tab.c"
    break;

  case 90: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 553 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2423 "raku.tab.c"
    break;

  case 91: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 557 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2431 "raku.tab.c"
    break;

  case 92: /* param_list: VAR_SCALAR  */
#line 562 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2437 "raku.tab.c"
    break;

  case 93: /* param_list: param_list ',' VAR_SCALAR  */
#line 563 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2443 "raku.tab.c"
    break;

  case 94: /* block: '{' stmt_list '}'  */
#line 566 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2449 "raku.tab.c"
    break;

  case 95: /* closure: '{' expr '}'  */
#line 569 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2455 "raku.tab.c"
    break;

  case 96: /* expr: VAR_SCALAR '=' expr  */
#line 572 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2461 "raku.tab.c"
    break;

  case 97: /* expr: KW_GATHER block  */
#line 573 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2471 "raku.tab.c"
    break;

  case 98: /* expr: cmp_expr  */
#line 578 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2477 "raku.tab.c"
    break;

  case 99: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 581 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2483 "raku.tab.c"
    break;

  case 100: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 582 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2489 "raku.tab.c"
    break;

  case 101: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 583 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2495 "raku.tab.c"
    break;

  case 102: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 584 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2501 "raku.tab.c"
    break;

  case 103: /* cmp_expr: jct_expr '<' jct_expr  */
#line 585 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2507 "raku.tab.c"
    break;

  case 104: /* cmp_expr: jct_expr '>' jct_expr  */
#line 586 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2513 "raku.tab.c"
    break;

  case 105: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 587 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2519 "raku.tab.c"
    break;

  case 106: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 588 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2525 "raku.tab.c"
    break;

  case 107: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 589 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2531 "raku.tab.c"
    break;

  case 108: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 590 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2537 "raku.tab.c"
    break;

  case 109: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 592 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2547 "raku.tab.c"
    break;

  case 110: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 598 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2557 "raku.tab.c"
    break;

  case 111: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 604 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2567 "raku.tab.c"
    break;

  case 112: /* cmp_expr: jct_expr  */
#line 609 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2573 "raku.tab.c"
    break;

  case 113: /* jct_expr: jct_expr '|' range_expr  */
#line 612 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2579 "raku.tab.c"
    break;

  case 114: /* jct_expr: jct_expr '&' range_expr  */
#line 613 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2585 "raku.tab.c"
    break;

  case 115: /* jct_expr: range_expr  */
#line 614 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2591 "raku.tab.c"
    break;

  case 116: /* range_expr: add_expr OP_RANGE add_expr  */
#line 617 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2597 "raku.tab.c"
    break;

  case 117: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 618 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2603 "raku.tab.c"
    break;

  case 118: /* range_expr: add_expr  */
#line 619 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2609 "raku.tab.c"
    break;

  case 119: /* add_expr: add_expr '+' mul_expr  */
#line 622 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2615 "raku.tab.c"
    break;

  case 120: /* add_expr: add_expr '-' mul_expr  */
#line 623 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2621 "raku.tab.c"
    break;

  case 121: /* add_expr: add_expr '~' mul_expr  */
#line 624 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2627 "raku.tab.c"
    break;

  case 122: /* add_expr: mul_expr  */
#line 625 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2633 "raku.tab.c"
    break;

  case 123: /* mul_expr: mul_expr '*' unary_expr  */
#line 628 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2639 "raku.tab.c"
    break;

  case 124: /* mul_expr: mul_expr '/' unary_expr  */
#line 629 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2645 "raku.tab.c"
    break;

  case 125: /* mul_expr: mul_expr '%' unary_expr  */
#line 630 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2651 "raku.tab.c"
    break;

  case 126: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 631 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2657 "raku.tab.c"
    break;

  case 127: /* mul_expr: unary_expr  */
#line 632 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2663 "raku.tab.c"
    break;

  case 128: /* unary_expr: '-' unary_expr  */
#line 635 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2669 "raku.tab.c"
    break;

  case 129: /* unary_expr: '!' unary_expr  */
#line 636 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2675 "raku.tab.c"
    break;

  case 130: /* unary_expr: postfix_expr  */
#line 637 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2681 "raku.tab.c"
    break;

  case 131: /* postfix_expr: call_expr  */
#line 639 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2687 "raku.tab.c"
    break;

  case 132: /* call_expr: IDENT '(' arg_list ')'  */
#line 642 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2696 "raku.tab.c"
    break;

  case 133: /* call_expr: IDENT '(' ')'  */
#line 646 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2702 "raku.tab.c"
    break;

  case 134: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 648 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2712 "raku.tab.c"
    break;

  case 135: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 654 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2722 "raku.tab.c"
    break;

  case 136: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 660 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2730 "raku.tab.c"
    break;

  case 137: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 664 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2741 "raku.tab.c"
    break;

  case 138: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 671 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2750 "raku.tab.c"
    break;

  case 139: /* call_expr: IDENT '.' CARET IDENT  */
#line 676 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2760 "raku.tab.c"
    break;

  case 140: /* call_expr: atom '.' CARET IDENT  */
#line 682 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2770 "raku.tab.c"
    break;

  case 141: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 688 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2781 "raku.tab.c"
    break;

  case 142: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 695 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2792 "raku.tab.c"
    break;

  case 143: /* call_expr: atom '.' IDENT '(' ')'  */
#line 702 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2801 "raku.tab.c"
    break;

  case 144: /* call_expr: atom '.' IDENT  */
#line 707 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2810 "raku.tab.c"
    break;

  case 145: /* call_expr: KW_DIE expr  */
#line 712 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 2816 "raku.tab.c"
    break;

  case 146: /* call_expr: KW_MAP closure expr  */
#line 714 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2822 "raku.tab.c"
    break;

  case 147: /* call_expr: KW_GREP closure expr  */
#line 716 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2828 "raku.tab.c"
    break;

  case 148: /* call_expr: KW_SORT expr  */
#line 718 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2834 "raku.tab.c"
    break;

  case 149: /* call_expr: KW_SORT closure expr  */
#line 720 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2840 "raku.tab.c"
    break;

  case 150: /* call_expr: atom  */
#line 721 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2846 "raku.tab.c"
    break;

  case 151: /* arg_list: expr  */
#line 724 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2852 "raku.tab.c"
    break;

  case 152: /* arg_list: arg_list ',' expr  */
#line 725 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2858 "raku.tab.c"
    break;

  case 153: /* atom: LIT_INT  */
#line 728 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2864 "raku.tab.c"
    break;

  case 154: /* atom: LIT_FLOAT  */
#line 729 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2870 "raku.tab.c"
    break;

  case 155: /* atom: LIT_STR  */
#line 730 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 2876 "raku.tab.c"
    break;

  case 156: /* atom: LIT_INTERP_STR  */
#line 731 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2882 "raku.tab.c"
    break;

  case 157: /* atom: VAR_SCALAR  */
#line 732 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2888 "raku.tab.c"
    break;

  case 158: /* atom: VAR_ARRAY  */
#line 733 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2894 "raku.tab.c"
    break;

  case 159: /* atom: VAR_HASH  */
#line 734 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2900 "raku.tab.c"
    break;

  case 160: /* atom: VAR_CAPTURE  */
#line 736 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2908 "raku.tab.c"
    break;

  case 161: /* atom: VAR_FH  */
#line 740 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2916 "raku.tab.c"
    break;

  case 162: /* atom: VAR_NAMED_CAPTURE  */
#line 744 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 2923 "raku.tab.c"
    break;

  case 163: /* atom: VAR_ARRAY '[' expr ']'  */
#line 747 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2929 "raku.tab.c"
    break;

  case 164: /* atom: VAR_HASH '<' IDENT '>'  */
#line 749 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2935 "raku.tab.c"
    break;

  case 165: /* atom: VAR_HASH '{' expr '}'  */
#line 751 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2941 "raku.tab.c"
    break;

  case 166: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 753 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2947 "raku.tab.c"
    break;

  case 167: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 755 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2953 "raku.tab.c"
    break;

  case 168: /* atom: IDENT  */
#line 756 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2959 "raku.tab.c"
    break;

  case 169: /* atom: VAR_TWIGIL  */
#line 758 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 2967 "raku.tab.c"
    break;

  case 170: /* atom: '(' expr ')'  */
#line 761 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 2973 "raku.tab.c"
    break;


#line 2977 "raku.tab.c"

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

#line 763 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
