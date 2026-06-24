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
  YYSYMBOL_is_clauses = 106,               /* is_clauses  */
  YYSYMBOL_class_body_list = 107,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 108,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 109,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 110,           /* named_arg_list  */
  YYSYMBOL_param_list = 111,               /* param_list  */
  YYSYMBOL_block = 112,                    /* block  */
  YYSYMBOL_closure = 113,                  /* closure  */
  YYSYMBOL_expr = 114,                     /* expr  */
  YYSYMBOL_cmp_expr = 115,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 116,                 /* jct_expr  */
  YYSYMBOL_range_expr = 117,               /* range_expr  */
  YYSYMBOL_add_expr = 118,                 /* add_expr  */
  YYSYMBOL_mul_expr = 119,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 120,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 121,             /* postfix_expr  */
  YYSYMBOL_call_expr = 122,                /* call_expr  */
  YYSYMBOL_arg_list = 123,                 /* arg_list  */
  YYSYMBOL_atom = 124                      /* atom  */
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
#define YYLAST   1102

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  177
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  442

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
       0,   196,   196,   207,   208,   211,   213,   215,   219,   223,
     225,   227,   229,   233,   237,   239,   241,   243,   245,   247,
     249,   251,   253,   255,   257,   259,   261,   266,   270,   277,
     282,   285,   288,   291,   294,   297,   298,   299,   300,   301,
     302,   304,   306,   307,   308,   309,   310,   311,   314,   316,
     318,   322,   326,   328,   332,   336,   340,   346,   352,   357,
     362,   370,   381,   382,   388,   396,   404,   420,   421,   432,
     433,   436,   439,   442,   445,   448,   451,   454,   457,   460,
     463,   466,   469,   476,   483,   490,   497,   507,   515,   524,
     533,   548,   549,   554,   559,   566,   570,   576,   577,   580,
     583,   586,   587,   592,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   611,   617,   623,   626,   627,
     628,   631,   632,   633,   636,   637,   638,   639,   642,   643,
     644,   645,   646,   649,   650,   651,   653,   655,   660,   661,
     667,   673,   677,   684,   689,   695,   701,   708,   715,   720,
     725,   727,   729,   731,   733,   735,   738,   739,   742,   743,
     744,   745,   746,   747,   748,   749,   753,   757,   760,   762,
     764,   766,   768,   770,   771,   775,   779,   783
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
  "class_decl", "is_clauses", "class_body_list", "grammar_decl",
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

#define YYPACT_NINF (-281)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -281,    16,   691,  -281,  -281,  -281,  -281,  -281,    49,   -68,
      14,   -27,    70,  -281,  -281,  -281,  -281,  -281,   197,   786,
     832,   -11,    34,   878,    52,    41,   878,   740,   878,   139,
     154,    85,   107,    41,   126,   126,   176,    41,   878,   196,
     218,  1016,  1016,   878,  -281,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,   172,   -13,   294,  -281,   -29,
      61,  -281,  -281,  -281,   141,   878,   246,   878,   250,   878,
     878,     5,   274,   205,   212,   239,   127,   241,   193,    26,
    -281,   878,   207,   878,   228,   878,   878,   -48,    36,   227,
    -281,  -281,   230,  -281,   234,   229,    31,    46,   878,   878,
    -281,   878,   878,   878,   878,  -281,   276,  -281,  -281,   232,
    -281,  -281,  -281,   236,  -281,  1016,  1016,  1016,  1016,  1016,
    1016,  1016,  1016,   136,  1016,  1016,  1016,  1016,  1016,  1016,
    1016,  1016,  1016,  1016,  1016,  1016,  1016,   156,   242,   -47,
     238,   252,   237,   245,   244,   317,   247,  -281,  -281,     3,
     878,   924,   878,   -30,   -25,   -20,   878,   878,   318,   878,
     128,  -281,   140,  -281,   248,   249,   324,  -281,  1016,  1016,
      -7,   418,  -281,  -281,  -281,   323,   878,   328,   878,   251,
     256,   255,  -281,  -281,  -281,    41,   -12,  -281,  -281,   -14,
     -14,   -14,   -14,   -14,   -14,   -14,   -14,  -281,  -281,  -281,
     -14,   -14,  -281,  -281,   158,   158,    61,    61,    61,  -281,
    -281,  -281,  -281,   261,   335,  -281,   878,   472,   280,   281,
     289,  -281,   519,  -281,   566,   878,  -281,   279,   878,    99,
     286,   878,  -281,   970,  -281,   878,  -281,  -281,   275,   292,
     282,   878,   878,    41,    41,    41,    51,    57,  -281,    41,
     143,  -281,   -21,   298,   283,   299,   284,    41,    41,  -281,
    -281,   362,  -281,     4,   613,  -281,   293,   295,   161,   878,
     878,   878,  -281,   168,    -9,  -281,   174,   178,  -281,  -281,
     185,  -281,   878,  -281,   296,   878,   114,   297,  -281,  -281,
    -281,   291,   300,   357,  -281,  -281,   373,   375,  -281,   376,
      41,   878,    41,  -281,  -281,  -281,   304,   305,   365,  -281,
    -281,   -19,   378,   379,   381,  -281,  -281,   186,   189,  -281,
    -281,   312,   313,   314,   315,  -281,   878,   386,  -281,  -281,
     878,   153,  -281,   210,  -281,   878,  -281,   319,   320,   -17,
      41,    41,  -281,  -281,    41,   310,  -281,  -281,    41,    -6,
     187,  -281,   395,   398,   399,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,   353,   211,  -281,   878,   157,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,   325,   326,    -8,     1,
     162,   329,   331,  -281,  -281,  -281,   878,   332,   216,  -281,
      -5,     2,   396,   878,  -281,   404,   878,  -281,    21,    24,
     336,   342,  -281,  -281,  -281,  -281,   351,    41,   220,    41,
     221,   359,   360,   368,   369,   405,   878,  -281,   449,   878,
    -281,  -281,  -281,  -281,  -281,    41,  -281,    41,  -281,  -281,
    -281,  -281,   382,   383,   384,   387,  -281,  -281,  -281,  -281,
    -281,  -281
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   158,   159,   160,   161,   162,   163,
     164,   174,   173,   175,   176,   165,   166,   167,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,    36,    37,    42,    43,    44,
      38,    39,    45,    46,    47,     0,   103,   117,   120,   123,
     127,   132,   135,   136,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,   163,   164,
     174,     0,     0,     0,     0,     0,     0,     0,   123,     0,
       3,   102,     0,    24,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,   153,    40,   150,    67,     0,
     162,   134,   133,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,   156,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,    20,     0,     0,     0,    59,     0,     0,
       0,     0,    22,    23,    62,     0,     0,     0,     0,     0,
       0,     0,   151,   152,   154,     0,     0,    91,   177,   104,
     105,   106,   107,   110,   111,   112,   113,   114,   115,   116,
     108,   109,   118,   119,   121,   122,   126,   124,   125,   131,
     128,   129,   130,   149,     0,    25,     0,     0,   168,   169,
     170,    27,     0,   144,     0,     0,   137,     0,     0,     0,
       0,     0,    15,     0,    16,     0,    17,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,    97,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,   100,
      41,     0,    69,     0,     0,   145,     0,     0,     0,     0,
       0,     0,   143,     0,   173,   141,     0,     0,   157,     5,
       0,     6,     0,     9,     0,     0,     0,     0,   168,   169,
     170,     0,     0,    48,    51,    58,     0,     0,    65,     0,
       0,     0,     0,    60,   171,   172,     0,     0,    52,    54,
      68,     0,     0,     0,     0,    90,   148,     0,     0,    26,
      29,     0,     0,     0,     0,   142,     0,     0,   139,   140,
       0,     0,    10,     0,    11,     0,    14,     0,     0,     0,
       0,     0,    98,    64,     0,     0,    33,    34,     0,     0,
       0,    66,     0,     0,     0,   147,   146,    28,    30,    31,
      32,    95,     0,     0,     7,     0,     0,    19,    21,    50,
      49,    56,    57,    63,    61,    53,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,     0,     0,     0,    12,
       0,     0,     0,     0,    75,     0,     0,    70,     0,     0,
       0,     0,    71,    72,    96,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,     0,     0,
      76,    73,    74,    13,    87,     0,    89,     0,    83,    79,
      82,    78,     0,     0,     0,     0,    86,    88,    85,    81,
      84,    80
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -281,  -281,   380,  -281,   133,  -281,  -281,  -281,  -281,  -281,
    -281,  -281,  -281,  -281,  -281,  -281,  -281,  -281,   209,  -280,
       0,   169,    -2,  -281,    43,    94,   -22,   120,   -10,  -281,
    -281,  -213,  -281
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    44,    45,    46,    47,    48,    49,    50,
      51,   252,    52,    53,   186,   311,    54,   263,   276,   250,
      91,   102,   148,    56,    57,    58,    59,    60,    61,    62,
      63,   149,    64
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,    88,   261,   248,   268,   248,   392,    21,   376,   273,
     166,   277,   248,   301,   302,   395,     3,    82,    84,   144,
      67,    87,   145,   216,    92,    94,    95,   128,   129,   349,
     350,   111,   112,   100,   105,   415,   107,   106,   418,   217,
     231,   113,    90,    70,   377,   233,   326,   130,   131,   132,
     235,   318,   115,   116,   232,   146,   312,   313,   314,   234,
     126,   127,   393,   138,   236,   140,    89,   142,   143,   331,
     303,   396,   351,    90,    71,    85,   394,    72,   262,   160,
     249,   162,   407,   164,   165,   397,    68,   167,   225,   409,
     226,   416,   168,   169,   419,   315,   179,   180,   158,   181,
     182,   183,   184,   175,    69,   417,   204,   205,   420,   296,
     408,   410,   130,   131,   132,   297,   159,   363,   177,    65,
      86,   176,   366,   209,   210,   211,   212,   130,   131,   132,
     133,    90,    66,   130,   131,   132,   178,   153,   154,   155,
     134,   135,   136,   197,   198,   199,   246,   247,   227,   229,
     230,    96,   388,    71,   237,   238,    72,   240,   189,   190,
     191,   192,   193,   194,   195,   196,    97,   200,   201,    55,
     213,    98,   398,   214,   254,   399,   256,   400,   401,     4,
       5,     6,     7,   281,   282,   260,    77,    78,    79,    80,
      12,    13,    14,    99,    15,    16,    17,   378,   334,   335,
     379,   380,   381,   382,   103,   104,    25,    73,    74,    75,
     108,    76,    29,   241,   266,   188,   101,    34,    35,    36,
     202,   203,    38,   278,   137,   242,   280,   188,   299,   284,
     300,   286,   109,   287,   130,   131,   132,   364,   225,   291,
     292,   389,   225,   293,   294,   295,   225,    41,   321,   298,
     206,   207,   208,   225,    42,   325,   114,   308,   309,   327,
     139,   328,    43,   225,   141,   329,   101,   322,   323,   324,
     330,   327,   188,   355,   225,   150,   356,     4,     5,     6,
       7,   157,   151,   333,    77,    78,    79,    80,    12,    13,
      14,   161,    15,    16,    17,   365,   225,   188,   387,   344,
     343,   225,   345,   406,    25,   299,   299,   425,   427,   152,
      29,   156,   163,   170,   172,    34,    35,    36,   173,   174,
      38,   185,   187,   188,   361,   219,   215,   218,   220,   221,
     222,   223,   239,   224,   245,   243,   244,   253,   257,   370,
     371,   372,   255,   258,   373,    41,   259,   264,   375,   265,
     269,   270,    42,   117,   118,   119,   120,   121,   122,   271,
      43,   147,   123,   279,   288,   289,   124,   125,   126,   127,
     283,   304,   306,   290,   305,   307,   310,   319,   337,   320,
     332,   336,   339,   340,   404,   341,   342,   338,   346,   347,
     348,   412,   352,   353,   414,   354,   357,   358,   359,   360,
     362,   374,   383,   367,   368,   384,   385,   424,   386,   426,
     411,   390,   391,   402,   433,   403,   405,   435,   413,   432,
     421,     4,     5,     6,     7,   436,   422,   437,     8,     9,
      10,    11,    12,    13,    14,   423,    15,    16,    17,    18,
      19,    20,    21,   428,   429,    22,    23,    24,    25,    26,
      27,    28,   430,   431,    29,    30,    31,    32,    33,    34,
      35,    36,    37,   434,    38,    39,   438,   439,   440,    40,
     171,   441,   369,   317,     0,     4,     5,     6,     7,     0,
       0,     0,    77,    78,    79,    80,    12,    13,    14,    41,
      15,    16,    17,     0,     0,     0,    42,     0,     0,     0,
       0,     0,    25,     0,    43,     0,     0,     0,    29,   251,
       0,     0,     0,    34,    35,    36,     0,     0,    38,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    77,
      78,    79,    80,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,    41,     0,     0,     0,     0,     0,    25,
      42,     0,     0,     0,     0,    29,     0,     0,    43,   267,
      34,    35,    36,     0,     0,    38,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    77,    78,    79,    80,
     274,    13,    14,     0,    15,    16,    17,     0,     0,     0,
      41,     0,     0,     0,     0,     0,    25,    42,     0,     0,
       0,     0,    29,     0,     0,    43,   272,    34,    35,    36,
       0,     0,    38,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    77,    78,    79,    80,   274,    13,    14,
       0,    15,    16,    17,     0,     0,     0,    41,     0,     0,
       0,     0,     0,    25,    42,     0,     0,     0,     0,    29,
       0,     0,    43,   275,    34,    35,    36,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     4,     5,     6,     7,     0,    43,
     316,     8,     9,    10,    11,    12,    13,    14,     0,    15,
      16,    17,    18,    19,    20,    21,     0,     0,    22,    23,
      24,    25,    26,    27,    28,     0,     0,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     0,    38,    39,     0,
       0,     0,    40,     4,     5,     6,     7,     0,     0,     0,
      77,    78,    79,    80,    12,    13,    14,     0,    15,    16,
      17,     0,    41,     0,     0,     0,     0,     0,     0,    42,
      25,     0,     0,     0,     0,     0,    29,    43,     0,     0,
       0,    34,    35,    36,     0,     0,    38,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    77,    78,    79,    80,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,    41,     0,     0,     0,     0,    25,     0,    42,     0,
       0,     0,    29,     0,    93,     0,    43,    34,    35,    36,
       0,     0,    38,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    77,    78,    79,    80,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,    41,     0,     0,
       0,     0,    25,     0,    42,     0,     0,     0,    29,     0,
       0,     0,    81,    34,    35,    36,     0,     0,    38,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    77,    78,
      79,    80,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,    41,     0,     0,     0,     0,    25,     0,
      42,     0,     0,     0,    29,     0,     0,     0,    83,    34,
      35,    36,     0,     0,    38,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    77,    78,    79,    80,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,    41,
       0,     0,     0,     0,    25,     0,    42,     0,     0,     0,
      29,     0,     0,     0,    43,    34,    35,    36,     0,     0,
      38,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      77,    78,    79,    80,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,    41,     0,     0,     0,     0,
      25,     0,    42,     0,     0,     0,    29,     0,     0,     0,
     228,    34,    35,    36,     0,     0,    38,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   110,    78,    79,    80,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,    29,     0,     0,     0,   285,    34,    35,    36,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,    43
};

static const yytype_int16 yycheck[] =
{
       2,    23,    14,    10,   217,    10,    14,    24,    14,   222,
      58,   224,    10,    34,    35,    14,     0,    19,    20,    14,
      88,    23,    17,    70,    26,    27,    28,    56,    57,    48,
      49,    41,    42,    33,    36,    14,    38,    37,    14,    86,
      70,    43,    90,    70,    50,    70,    55,    76,    77,    78,
      70,   264,    65,    66,    84,    50,    52,    53,    54,    84,
      74,    75,    70,    65,    84,    67,    14,    69,    70,   282,
      91,    70,    91,    90,    83,    86,    84,    86,    90,    81,
      87,    83,    87,    85,    86,    84,    72,    87,    85,    87,
      87,    70,    56,    57,    70,    91,    98,    99,    72,   101,
     102,   103,   104,    72,    90,    84,   128,   129,    84,    58,
     390,   391,    76,    77,    78,    58,    90,   330,    72,    70,
      86,    90,   335,   133,   134,   135,   136,    76,    77,    78,
      69,    90,    83,    76,    77,    78,    90,    10,    11,    12,
      79,    80,    81,     7,     8,     9,   168,   169,   150,   151,
     152,    12,   365,    83,   156,   157,    86,   159,   115,   116,
     117,   118,   119,   120,   121,   122,    12,   124,   125,   171,
      14,    86,    10,    17,   176,    13,   178,    15,    16,     3,
       4,     5,     6,    84,    85,   185,    10,    11,    12,    13,
      14,    15,    16,    86,    18,    19,    20,    10,    84,    85,
      13,    14,    15,    16,    35,    36,    30,    10,    11,    12,
      14,    14,    36,    85,   216,    87,    90,    41,    42,    43,
     126,   127,    46,   225,    83,    85,   228,    87,    85,   231,
      87,   233,    14,   235,    76,    77,    78,    84,    85,   241,
     242,    84,    85,   243,   244,   245,    85,    71,    87,   249,
     130,   131,   132,    85,    78,    87,    84,   257,   258,    85,
      14,    87,    86,    85,    14,    87,    90,   269,   270,   271,
      85,    85,    87,    87,    85,    70,    87,     3,     4,     5,
       6,    88,    70,   285,    10,    11,    12,    13,    14,    15,
      16,    84,    18,    19,    20,    85,    85,    87,    87,   301,
     300,    85,   302,    87,    30,    85,    85,    87,    87,    70,
      36,    70,    84,    86,    84,    41,    42,    43,    84,    90,
      46,    45,    90,    87,   326,    73,    84,    89,    91,    84,
      86,    14,    14,    86,    10,    87,    87,    14,    87,   339,
     340,   341,    14,    87,   344,    71,    91,    86,   348,    14,
      70,    70,    78,    59,    60,    61,    62,    63,    64,    70,
      86,    87,    68,    84,    89,    73,    72,    73,    74,    75,
      84,    73,    73,    91,    91,    91,    14,    84,    87,    84,
      84,    84,    25,    10,   386,    10,    10,    87,    84,    84,
      25,   393,    14,    14,   396,    14,    84,    84,    84,    84,
      14,    91,     7,    84,    84,     7,     7,   407,    55,   409,
      14,    86,    86,    84,   416,    84,    84,   419,    14,    14,
      84,     3,     4,     5,     6,   425,    84,   427,    10,    11,
      12,    13,    14,    15,    16,    84,    18,    19,    20,    21,
      22,    23,    24,    84,    84,    27,    28,    29,    30,    31,
      32,    33,    84,    84,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    14,    46,    47,    84,    84,    84,    51,
      90,    84,   339,   264,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    71,
      18,    19,    20,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    86,    -1,    -1,    -1,    36,    91,
      -1,    -1,    -1,    41,    42,    43,    -1,    -1,    46,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    30,
      78,    -1,    -1,    -1,    -1,    36,    -1,    -1,    86,    87,
      41,    42,    43,    -1,    -1,    46,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    30,    78,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    86,    87,    41,    42,    43,
      -1,    -1,    46,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    30,    78,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    86,    87,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,     3,     4,     5,     6,    -1,    86,
      87,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    27,    28,
      29,    30,    31,    32,    33,    -1,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    47,    -1,
      -1,    -1,    51,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      30,    -1,    -1,    -1,    -1,    -1,    36,    86,    -1,    -1,
      -1,    41,    42,    43,    -1,    -1,    46,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    30,    -1,    78,    -1,
      -1,    -1,    36,    -1,    84,    -1,    86,    41,    42,    43,
      -1,    -1,    46,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    30,    -1,    78,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    86,    41,    42,    43,    -1,    -1,    46,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    30,    -1,
      78,    -1,    -1,    -1,    36,    -1,    -1,    -1,    86,    41,
      42,    43,    -1,    -1,    46,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    30,    -1,    78,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    86,    41,    42,    43,    -1,    -1,
      46,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      30,    -1,    78,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      86,    41,    42,    43,    -1,    -1,    46,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    86,    41,    42,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86
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
     101,   102,   104,   105,   108,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   124,    70,    83,    88,    72,    90,
      70,    83,    86,    10,    11,    12,    14,    10,    11,    12,
      13,    86,   114,    86,   114,    86,    86,   114,   118,    14,
      90,   112,   114,    84,   114,   114,    12,    12,    86,    86,
     112,    90,   113,   113,   113,   114,   112,   114,    14,    14,
      10,   120,   120,   114,    84,    65,    66,    59,    60,    61,
      62,    63,    64,    68,    72,    73,    74,    75,    56,    57,
      76,    77,    78,    69,    79,    80,    81,    83,   114,    14,
     114,    14,   114,   114,    14,    17,    50,    87,   114,   123,
      70,    70,    70,    10,    11,    12,    70,    88,    72,    90,
     114,    84,   114,    84,   114,   114,    58,   112,    56,    57,
      86,    94,    84,    84,    90,    72,    90,    72,    90,   114,
     114,   114,   114,   114,   114,    45,   106,    90,    87,   116,
     116,   116,   116,   116,   116,   116,   116,     7,     8,     9,
     116,   116,   117,   117,   118,   118,   119,   119,   119,   120,
     120,   120,   120,    14,    17,    84,    70,    86,    89,    73,
      91,    84,    86,    14,    86,    85,    87,   114,    86,   114,
     114,    70,    84,    70,    84,    70,    84,   114,   114,    14,
     114,    85,    85,    87,    87,    10,   118,   118,    10,    87,
     111,    91,   103,    14,   114,    14,   114,    87,    87,    91,
     112,    14,    90,   109,    86,    14,   114,    87,   123,    70,
      70,    70,    87,   123,    14,    87,   110,   123,   114,    84,
     114,    84,    85,    84,   114,    86,   114,   114,    89,    73,
      91,   114,   114,   112,   112,   112,    58,    58,   112,    85,
      87,    34,    35,    91,    73,    91,    73,    91,   112,   112,
      14,   107,    52,    53,    54,    91,    87,   110,   123,    84,
      84,    87,   114,   114,   114,    87,    55,    85,    87,    87,
      85,   123,    84,   114,    84,    85,    84,    87,    87,    25,
      10,    10,    10,   112,   114,   112,    84,    84,    25,    48,
      49,    91,    14,    14,    14,    87,    87,    84,    84,    84,
      84,   114,    14,   123,    84,    85,   123,    84,    84,    96,
     112,   112,   112,   112,    91,   112,    14,    50,    10,    13,
      14,    15,    16,     7,     7,     7,    55,    87,   123,    84,
      86,    86,    14,    70,    84,    14,    70,    84,    10,    13,
      15,    16,    84,    84,   114,    84,    87,    87,   111,    87,
     111,    14,   114,    14,   114,    14,    70,    84,    14,    70,
      84,    84,    84,    84,   112,    87,   112,    87,    84,    84,
      84,    84,    14,   114,    14,   114,   112,   112,    84,    84,
      84,    84
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
     102,   102,   103,   103,   104,   104,   105,   106,   106,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     108,   109,   109,   109,   109,   110,   110,   111,   111,   112,
     113,   114,   114,   114,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   116,   116,
     116,   117,   117,   117,   118,   118,   118,   118,   119,   119,
     119,   119,   119,   120,   120,   120,   121,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   123,   123,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124
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
       5,     7,     0,     4,     6,     5,     6,     0,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     6,     7,     7,     7,     6,     7,     6,
       5,     0,     4,     4,     4,     3,     5,     1,     3,     3,
       3,     3,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     3,     3,
       3,     3,     1,     2,     2,     1,     1,     4,     3,     6,
       6,     5,     6,     5,     4,     4,     6,     6,     5,     3,
       2,     3,     3,     2,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     5,     5,     1,     1,     1,     1,     3
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
#line 197 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1748 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 207 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1754 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 208 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1760 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 212 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1766 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 214 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1772 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 216 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1780 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 220 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1788 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 224 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1794 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 226 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1800 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 228 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1806 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 230 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1814 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 234 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1822 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 238 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1828 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 240 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1834 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 242 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1840 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 244 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1846 "raku.tab.c"
    break;

  case 18: /* stmt: KW_SAY expr ';'  */
#line 246 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1852 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 248 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1858 "raku.tab.c"
    break;

  case 20: /* stmt: KW_PRINT expr ';'  */
#line 250 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1864 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 252 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1870 "raku.tab.c"
    break;

  case 22: /* stmt: KW_TAKE expr ';'  */
#line 254 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1876 "raku.tab.c"
    break;

  case 23: /* stmt: KW_RETURN expr ';'  */
#line 256 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1882 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN ';'  */
#line 258 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1888 "raku.tab.c"
    break;

  case 25: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 260 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1894 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 262 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1903 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 267 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1911 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 271 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 1922 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 278 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 1931 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 283 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1938 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 286 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1945 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 289 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1952 "raku.tab.c"
    break;

  case 33: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 292 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1959 "raku.tab.c"
    break;

  case 34: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 295 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1966 "raku.tab.c"
    break;

  case 35: /* stmt: expr ';'  */
#line 297 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1972 "raku.tab.c"
    break;

  case 36: /* stmt: if_stmt  */
#line 298 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1978 "raku.tab.c"
    break;

  case 37: /* stmt: while_stmt  */
#line 299 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1984 "raku.tab.c"
    break;

  case 38: /* stmt: for_stmt  */
#line 300 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1990 "raku.tab.c"
    break;

  case 39: /* stmt: given_stmt  */
#line 301 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1996 "raku.tab.c"
    break;

  case 40: /* stmt: KW_TRY block  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2002 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TRY block KW_CATCH block  */
#line 305 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2008 "raku.tab.c"
    break;

  case 42: /* stmt: unless_stmt  */
#line 306 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2014 "raku.tab.c"
    break;

  case 43: /* stmt: until_stmt  */
#line 307 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2020 "raku.tab.c"
    break;

  case 44: /* stmt: repeat_stmt  */
#line 308 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2026 "raku.tab.c"
    break;

  case 45: /* stmt: sub_decl  */
#line 309 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2032 "raku.tab.c"
    break;

  case 46: /* stmt: class_decl  */
#line 310 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2038 "raku.tab.c"
    break;

  case 47: /* stmt: grammar_decl  */
#line 311 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2044 "raku.tab.c"
    break;

  case 48: /* if_stmt: KW_IF '(' expr ')' block  */
#line 315 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2050 "raku.tab.c"
    break;

  case 49: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 317 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2056 "raku.tab.c"
    break;

  case 50: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 319 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2062 "raku.tab.c"
    break;

  case 51: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 323 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2068 "raku.tab.c"
    break;

  case 52: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 327 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2074 "raku.tab.c"
    break;

  case 53: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 329 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2080 "raku.tab.c"
    break;

  case 54: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 333 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2086 "raku.tab.c"
    break;

  case 55: /* repeat_stmt: KW_REPEAT block  */
#line 337 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2092 "raku.tab.c"
    break;

  case 56: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 341 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2102 "raku.tab.c"
    break;

  case 57: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 347 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2112 "raku.tab.c"
    break;

  case 58: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 353 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2121 "raku.tab.c"
    break;

  case 59: /* for_stmt: KW_FOR expr block  */
#line 358 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2128 "raku.tab.c"
    break;

  case 60: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 363 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2140 "raku.tab.c"
    break;

  case 61: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 371 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2153 "raku.tab.c"
    break;

  case 62: /* when_list: %empty  */
#line 381 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2159 "raku.tab.c"
    break;

  case 63: /* when_list: when_list KW_WHEN expr block  */
#line 383 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2167 "raku.tab.c"
    break;

  case 64: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 389 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2179 "raku.tab.c"
    break;

  case 65: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 397 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2189 "raku.tab.c"
    break;

  case 66: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 405 "raku.y"
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
#line 2207 "raku.tab.c"
    break;

  case 67: /* is_clauses: %empty  */
#line 420 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2213 "raku.tab.c"
    break;

  case 68: /* is_clauses: is_clauses IDENT IDENT  */
#line 422 "raku.y"
        {
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is") && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 1); memcpy(m, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 2); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; memcpy(m + l1 + 1, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 2226 "raku.tab.c"
    break;

  case 69: /* class_body_list: %empty  */
#line 432 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2232 "raku.tab.c"
    break;

  case 70: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 434 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2239 "raku.tab.c"
    break;

  case 71: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 437 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2246 "raku.tab.c"
    break;

  case 72: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 440 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2253 "raku.tab.c"
    break;

  case 73: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 443 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2260 "raku.tab.c"
    break;

  case 74: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 446 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2267 "raku.tab.c"
    break;

  case 75: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 449 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2274 "raku.tab.c"
    break;

  case 76: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 452 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2281 "raku.tab.c"
    break;

  case 77: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 455 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2288 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 458 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2295 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 461 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2302 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 464 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2309 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 467 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2316 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 470 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2327 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 477 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2338 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 484 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2349 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 491 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2360 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 498 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2374 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 508 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2386 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 516 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2399 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 525 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2410 "raku.tab.c"
    break;

  case 90: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 534 "raku.y"
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
#line 2427 "raku.tab.c"
    break;

  case 91: /* grammar_body_list: %empty  */
#line 548 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2433 "raku.tab.c"
    break;

  case 92: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 550 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2442 "raku.tab.c"
    break;

  case 93: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 555 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2451 "raku.tab.c"
    break;

  case 94: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 560 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2460 "raku.tab.c"
    break;

  case 95: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 567 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2468 "raku.tab.c"
    break;

  case 96: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 571 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2476 "raku.tab.c"
    break;

  case 97: /* param_list: VAR_SCALAR  */
#line 576 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2482 "raku.tab.c"
    break;

  case 98: /* param_list: param_list ',' VAR_SCALAR  */
#line 577 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2488 "raku.tab.c"
    break;

  case 99: /* block: '{' stmt_list '}'  */
#line 580 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2494 "raku.tab.c"
    break;

  case 100: /* closure: '{' expr '}'  */
#line 583 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2500 "raku.tab.c"
    break;

  case 101: /* expr: VAR_SCALAR '=' expr  */
#line 586 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2506 "raku.tab.c"
    break;

  case 102: /* expr: KW_GATHER block  */
#line 587 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2516 "raku.tab.c"
    break;

  case 103: /* expr: cmp_expr  */
#line 592 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2522 "raku.tab.c"
    break;

  case 104: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 595 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2528 "raku.tab.c"
    break;

  case 105: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 596 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2534 "raku.tab.c"
    break;

  case 106: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 597 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2540 "raku.tab.c"
    break;

  case 107: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 598 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2546 "raku.tab.c"
    break;

  case 108: /* cmp_expr: jct_expr '<' jct_expr  */
#line 599 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2552 "raku.tab.c"
    break;

  case 109: /* cmp_expr: jct_expr '>' jct_expr  */
#line 600 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2558 "raku.tab.c"
    break;

  case 110: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 601 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2564 "raku.tab.c"
    break;

  case 111: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 602 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2570 "raku.tab.c"
    break;

  case 112: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 603 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2576 "raku.tab.c"
    break;

  case 113: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 604 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2582 "raku.tab.c"
    break;

  case 114: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 606 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2592 "raku.tab.c"
    break;

  case 115: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 612 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2602 "raku.tab.c"
    break;

  case 116: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 618 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2612 "raku.tab.c"
    break;

  case 117: /* cmp_expr: jct_expr  */
#line 623 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2618 "raku.tab.c"
    break;

  case 118: /* jct_expr: jct_expr '|' range_expr  */
#line 626 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2624 "raku.tab.c"
    break;

  case 119: /* jct_expr: jct_expr '&' range_expr  */
#line 627 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2630 "raku.tab.c"
    break;

  case 120: /* jct_expr: range_expr  */
#line 628 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2636 "raku.tab.c"
    break;

  case 121: /* range_expr: add_expr OP_RANGE add_expr  */
#line 631 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2642 "raku.tab.c"
    break;

  case 122: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 632 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2648 "raku.tab.c"
    break;

  case 123: /* range_expr: add_expr  */
#line 633 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2654 "raku.tab.c"
    break;

  case 124: /* add_expr: add_expr '+' mul_expr  */
#line 636 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2660 "raku.tab.c"
    break;

  case 125: /* add_expr: add_expr '-' mul_expr  */
#line 637 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2666 "raku.tab.c"
    break;

  case 126: /* add_expr: add_expr '~' mul_expr  */
#line 638 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2672 "raku.tab.c"
    break;

  case 127: /* add_expr: mul_expr  */
#line 639 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2678 "raku.tab.c"
    break;

  case 128: /* mul_expr: mul_expr '*' unary_expr  */
#line 642 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2684 "raku.tab.c"
    break;

  case 129: /* mul_expr: mul_expr '/' unary_expr  */
#line 643 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2690 "raku.tab.c"
    break;

  case 130: /* mul_expr: mul_expr '%' unary_expr  */
#line 644 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2696 "raku.tab.c"
    break;

  case 131: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 645 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2702 "raku.tab.c"
    break;

  case 132: /* mul_expr: unary_expr  */
#line 646 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2708 "raku.tab.c"
    break;

  case 133: /* unary_expr: '-' unary_expr  */
#line 649 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2714 "raku.tab.c"
    break;

  case 134: /* unary_expr: '!' unary_expr  */
#line 650 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2720 "raku.tab.c"
    break;

  case 135: /* unary_expr: postfix_expr  */
#line 651 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2726 "raku.tab.c"
    break;

  case 136: /* postfix_expr: call_expr  */
#line 653 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2732 "raku.tab.c"
    break;

  case 137: /* call_expr: IDENT '(' arg_list ')'  */
#line 656 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2741 "raku.tab.c"
    break;

  case 138: /* call_expr: IDENT '(' ')'  */
#line 660 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2747 "raku.tab.c"
    break;

  case 139: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 662 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2757 "raku.tab.c"
    break;

  case 140: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 668 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2767 "raku.tab.c"
    break;

  case 141: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 674 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2775 "raku.tab.c"
    break;

  case 142: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 678 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2786 "raku.tab.c"
    break;

  case 143: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 685 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2795 "raku.tab.c"
    break;

  case 144: /* call_expr: IDENT '.' CARET IDENT  */
#line 690 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2805 "raku.tab.c"
    break;

  case 145: /* call_expr: atom '.' CARET IDENT  */
#line 696 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2815 "raku.tab.c"
    break;

  case 146: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 702 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2826 "raku.tab.c"
    break;

  case 147: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 709 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2837 "raku.tab.c"
    break;

  case 148: /* call_expr: atom '.' IDENT '(' ')'  */
#line 716 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2846 "raku.tab.c"
    break;

  case 149: /* call_expr: atom '.' IDENT  */
#line 721 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2855 "raku.tab.c"
    break;

  case 150: /* call_expr: KW_DIE expr  */
#line 726 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 2861 "raku.tab.c"
    break;

  case 151: /* call_expr: KW_MAP closure expr  */
#line 728 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2867 "raku.tab.c"
    break;

  case 152: /* call_expr: KW_GREP closure expr  */
#line 730 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2873 "raku.tab.c"
    break;

  case 153: /* call_expr: KW_SORT expr  */
#line 732 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2879 "raku.tab.c"
    break;

  case 154: /* call_expr: KW_SORT closure expr  */
#line 734 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2885 "raku.tab.c"
    break;

  case 155: /* call_expr: atom  */
#line 735 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2891 "raku.tab.c"
    break;

  case 156: /* arg_list: expr  */
#line 738 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2897 "raku.tab.c"
    break;

  case 157: /* arg_list: arg_list ',' expr  */
#line 739 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2903 "raku.tab.c"
    break;

  case 158: /* atom: LIT_INT  */
#line 742 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2909 "raku.tab.c"
    break;

  case 159: /* atom: LIT_FLOAT  */
#line 743 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2915 "raku.tab.c"
    break;

  case 160: /* atom: LIT_STR  */
#line 744 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 2921 "raku.tab.c"
    break;

  case 161: /* atom: LIT_INTERP_STR  */
#line 745 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2927 "raku.tab.c"
    break;

  case 162: /* atom: VAR_SCALAR  */
#line 746 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2933 "raku.tab.c"
    break;

  case 163: /* atom: VAR_ARRAY  */
#line 747 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2939 "raku.tab.c"
    break;

  case 164: /* atom: VAR_HASH  */
#line 748 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2945 "raku.tab.c"
    break;

  case 165: /* atom: VAR_CAPTURE  */
#line 750 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2953 "raku.tab.c"
    break;

  case 166: /* atom: VAR_FH  */
#line 754 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2961 "raku.tab.c"
    break;

  case 167: /* atom: VAR_NAMED_CAPTURE  */
#line 758 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 2968 "raku.tab.c"
    break;

  case 168: /* atom: VAR_ARRAY '[' expr ']'  */
#line 761 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2974 "raku.tab.c"
    break;

  case 169: /* atom: VAR_HASH '<' IDENT '>'  */
#line 763 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2980 "raku.tab.c"
    break;

  case 170: /* atom: VAR_HASH '{' expr '}'  */
#line 765 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2986 "raku.tab.c"
    break;

  case 171: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 767 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2992 "raku.tab.c"
    break;

  case 172: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 769 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2998 "raku.tab.c"
    break;

  case 173: /* atom: IDENT  */
#line 770 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3004 "raku.tab.c"
    break;

  case 174: /* atom: VAR_TWIGIL  */
#line 772 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3012 "raku.tab.c"
    break;

  case 175: /* atom: VAR_ARRAY_TWIGIL  */
#line 776 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3020 "raku.tab.c"
    break;

  case 176: /* atom: VAR_HASH_TWIGIL  */
#line 780 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3028 "raku.tab.c"
    break;

  case 177: /* atom: '(' expr ')'  */
#line 783 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3034 "raku.tab.c"
    break;


#line 3038 "raku.tab.c"

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

#line 785 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
