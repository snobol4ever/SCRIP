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
  YYSYMBOL_VAR_CAPTURE = 15,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 16,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 17,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_MY = 18,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 19,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 20,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 21,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 22,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 23,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 24,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 25,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 26,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 27,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 28,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 29,                 /* KW_RETURN  */
  YYSYMBOL_KW_GIVEN = 30,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 31,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 32,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 33,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 34,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 35,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 36,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 37,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 38,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 39,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 40,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 41,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 42,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 43,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 44,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 45,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 46,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 47,                    /* KW_NEW  */
  YYSYMBOL_KW_GRAMMAR = 48,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 49,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 50,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 51,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 52,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 53,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 54,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 55,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 56,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 57,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 58,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 59,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 60,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 61,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 62,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 63,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 64,                   /* OP_BIND  */
  YYSYMBOL_OP_SMATCH = 65,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 66,                    /* OP_DIV  */
  YYSYMBOL_67_ = 67,                       /* '='  */
  YYSYMBOL_68_ = 68,                       /* '!'  */
  YYSYMBOL_69_ = 69,                       /* '<'  */
  YYSYMBOL_70_ = 70,                       /* '>'  */
  YYSYMBOL_71_ = 71,                       /* '|'  */
  YYSYMBOL_72_ = 72,                       /* '&'  */
  YYSYMBOL_73_ = 73,                       /* '~'  */
  YYSYMBOL_74_ = 74,                       /* '+'  */
  YYSYMBOL_75_ = 75,                       /* '-'  */
  YYSYMBOL_76_ = 76,                       /* '*'  */
  YYSYMBOL_77_ = 77,                       /* '/'  */
  YYSYMBOL_78_ = 78,                       /* '%'  */
  YYSYMBOL_UMINUS = 79,                    /* UMINUS  */
  YYSYMBOL_80_ = 80,                       /* '.'  */
  YYSYMBOL_81_ = 81,                       /* ';'  */
  YYSYMBOL_82_ = 82,                       /* ','  */
  YYSYMBOL_83_ = 83,                       /* '('  */
  YYSYMBOL_84_ = 84,                       /* ')'  */
  YYSYMBOL_85_ = 85,                       /* '['  */
  YYSYMBOL_86_ = 86,                       /* ']'  */
  YYSYMBOL_87_ = 87,                       /* '{'  */
  YYSYMBOL_88_ = 88,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 89,                  /* $accept  */
  YYSYMBOL_program = 90,                   /* program  */
  YYSYMBOL_stmt_list = 91,                 /* stmt_list  */
  YYSYMBOL_stmt = 92,                      /* stmt  */
  YYSYMBOL_if_stmt = 93,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 94,                /* while_stmt  */
  YYSYMBOL_unless_stmt = 95,               /* unless_stmt  */
  YYSYMBOL_until_stmt = 96,                /* until_stmt  */
  YYSYMBOL_repeat_stmt = 97,               /* repeat_stmt  */
  YYSYMBOL_for_stmt = 98,                  /* for_stmt  */
  YYSYMBOL_given_stmt = 99,                /* given_stmt  */
  YYSYMBOL_when_list = 100,                /* when_list  */
  YYSYMBOL_sub_decl = 101,                 /* sub_decl  */
  YYSYMBOL_class_decl = 102,               /* class_decl  */
  YYSYMBOL_class_body_list = 103,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 104,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 105,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 106,           /* named_arg_list  */
  YYSYMBOL_param_list = 107,               /* param_list  */
  YYSYMBOL_block = 108,                    /* block  */
  YYSYMBOL_closure = 109,                  /* closure  */
  YYSYMBOL_expr = 110,                     /* expr  */
  YYSYMBOL_cmp_expr = 111,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 112,                 /* jct_expr  */
  YYSYMBOL_range_expr = 113,               /* range_expr  */
  YYSYMBOL_add_expr = 114,                 /* add_expr  */
  YYSYMBOL_mul_expr = 115,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 116,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 117,             /* postfix_expr  */
  YYSYMBOL_call_expr = 118,                /* call_expr  */
  YYSYMBOL_arg_list = 119,                 /* arg_list  */
  YYSYMBOL_atom = 120                      /* atom  */
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
#define YYLAST   1026

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  89
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  168
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  430

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   322


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
       2,     2,     2,    68,     2,     2,     2,    78,    72,     2,
      83,    84,    76,    74,    82,    75,    80,    77,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    81,
      69,    67,    70,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    85,     2,    86,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    87,    71,    88,    73,     2,     2,     2,
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
      65,    66,    79
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   193,   193,   204,   205,   208,   210,   212,   216,   220,
     222,   224,   226,   230,   234,   236,   238,   240,   242,   244,
     246,   248,   250,   252,   254,   256,   258,   263,   267,   274,
     279,   282,   285,   288,   291,   294,   295,   296,   297,   298,
     299,   301,   303,   304,   305,   306,   307,   308,   311,   313,
     315,   319,   323,   325,   329,   333,   337,   343,   349,   354,
     359,   367,   378,   379,   385,   393,   401,   414,   433,   434,
     437,   440,   443,   446,   449,   452,   455,   458,   464,   470,
     476,   482,   492,   500,   509,   518,   533,   534,   539,   544,
     551,   555,   561,   562,   565,   568,   571,   572,   577,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     596,   602,   608,   611,   612,   613,   616,   617,   618,   621,
     622,   623,   624,   627,   628,   629,   630,   631,   634,   635,
     636,   638,   640,   645,   646,   652,   658,   662,   669,   674,
     681,   688,   693,   698,   700,   702,   704,   706,   708,   711,
     712,   715,   716,   717,   718,   719,   720,   721,   722,   726,
     730,   733,   735,   737,   739,   741,   743,   744,   748
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
  "IDENT", "VAR_CAPTURE", "VAR_FH", "VAR_NAMED_CAPTURE", "KW_MY", "KW_SAY",
  "KW_PRINT", "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR",
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_GIVEN", "KW_WHEN",
  "KW_DEFAULT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL",
  "KW_REPEAT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_GRAMMAR",
  "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW", "OP_RANGE",
  "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ",
  "OP_SNE", "OP_AND", "OP_OR", "OP_BIND", "OP_SMATCH", "OP_DIV", "'='",
  "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'",
  "'}'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt",
  "given_stmt", "when_list", "sub_decl", "class_decl", "class_body_list",
  "grammar_decl", "grammar_body_list", "named_arg_list", "param_list",
  "block", "closure", "expr", "cmp_expr", "jct_expr", "range_expr",
  "add_expr", "mul_expr", "unary_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-211)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -211,    14,   667,  -211,  -211,  -211,  -211,  -211,    41,   -21,
     -39,   -10,   -47,  -211,  -211,  -211,   220,   759,   805,   -45,
      13,   851,    58,    15,   851,   713,   851,   102,   106,    37,
      61,    15,    66,    66,   174,    15,   851,   110,   201,   125,
     125,   851,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,    67,   143,   259,  -211,    59,   126,  -211,
    -211,  -211,    96,   851,   206,   851,   219,   851,   851,    -6,
     271,   202,   205,   226,   187,   229,   222,   -32,  -211,   851,
     245,   851,   252,   851,   851,   -38,   121,   239,  -211,  -211,
     253,  -211,   254,   240,   -29,   -25,   851,   851,  -211,   851,
     851,   851,   851,  -211,   290,  -211,    -1,   255,  -211,  -211,
    -211,   256,  -211,   125,   125,   125,   125,   125,   125,   125,
     125,   251,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   329,   263,   -24,   261,   278,
     262,   268,   269,   270,  -211,  -211,     9,   851,   897,   851,
      34,    42,    76,   851,   851,   337,   851,    85,  -211,    88,
    -211,   280,   283,   358,  -211,   125,   125,    -9,   405,  -211,
    -211,  -211,   355,   851,   356,   851,   287,   288,   285,  -211,
    -211,  -211,    15,   360,  -211,  -211,  -211,    90,    90,    90,
      90,    90,    90,    90,    90,  -211,  -211,  -211,    90,    90,
    -211,  -211,   177,   177,   126,   126,   126,  -211,  -211,  -211,
    -211,   292,  -211,   851,   451,   309,   310,   312,  -211,   498,
     545,   851,  -211,   299,   851,   -13,   301,   851,  -211,   943,
    -211,   851,  -211,  -211,   297,   314,   298,   851,   851,    15,
      15,    15,    52,   173,  -211,    15,    99,  -211,    -5,   315,
     302,   317,   303,    15,    15,  -211,  -211,   305,   -17,     2,
     592,   308,   316,   134,   851,   851,   851,  -211,   139,    36,
    -211,   142,   186,  -211,  -211,   189,  -211,   851,  -211,   318,
     851,   128,   319,  -211,  -211,  -211,   311,   321,   371,  -211,
    -211,   388,   392,  -211,   393,    15,   851,    15,  -211,  -211,
    -211,   325,   326,   391,  -211,  -211,     7,   146,  -211,   413,
     414,   422,  -211,  -211,   207,   208,  -211,  -211,   366,   369,
     370,   377,  -211,   851,   423,  -211,  -211,   851,   156,  -211,
     217,  -211,   851,  -211,   378,   379,    -3,    15,    15,  -211,
    -211,    15,   364,  -211,  -211,    15,     1,   386,   387,   -11,
      -7,    32,   464,   465,   467,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,   424,   218,  -211,   851,   162,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,    -8,    -4,   461,
     851,  -211,   463,   851,  -211,     6,    11,  -211,  -211,  -211,
     851,   398,   221,  -211,    15,   224,    15,   241,   400,   401,
     402,   404,   472,   851,  -211,   473,   851,  -211,  -211,  -211,
     411,  -211,    15,  -211,    15,  -211,  -211,  -211,  -211,   415,
     416,   417,   418,  -211,  -211,  -211,  -211,  -211,  -211,  -211
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   151,   152,   153,   154,   155,   156,
     157,   167,   166,   158,   159,   160,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,    36,    37,    42,    43,    44,    38,    39,
      45,    46,    47,     0,    98,   112,   115,   118,   122,   127,
     130,   131,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   156,   157,   167,     0,
       0,     0,     0,     0,     0,     0,   118,     0,     3,    97,
       0,    24,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,   146,    40,   143,     0,     0,   155,   129,
     128,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      20,     0,     0,     0,    59,     0,     0,     0,     0,    22,
      23,    62,     0,     0,     0,     0,     0,     0,     0,   144,
     145,   147,     0,     0,    68,    86,   168,    99,   100,   101,
     102,   105,   106,   107,   108,   109,   110,   111,   103,   104,
     113,   114,   116,   117,   121,   119,   120,   126,   123,   124,
     125,   142,    25,     0,     0,   161,   162,   163,    27,     0,
       0,     0,   132,     0,     0,     0,     0,     0,    15,     0,
      16,     0,    17,    96,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,    92,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,    95,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,     0,   166,
     136,     0,     0,   150,     5,     0,     6,     0,     9,     0,
       0,     0,     0,   161,   162,   163,     0,     0,    48,    51,
      58,     0,     0,    65,     0,     0,     0,     0,    60,   164,
     165,     0,     0,    52,    54,    68,     0,     0,    66,     0,
       0,     0,    85,   141,     0,     0,    26,    29,     0,     0,
       0,     0,   137,     0,     0,   134,   135,     0,     0,    10,
       0,    11,     0,    14,     0,     0,     0,     0,     0,    93,
      64,     0,     0,    33,    34,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   140,   139,    28,    30,    31,
      32,    90,     0,     0,     7,     0,     0,    19,    21,    50,
      49,    56,    57,    63,    61,    53,    67,     0,     0,     0,
       0,    70,     0,     0,    69,     0,     0,    87,    88,    89,
       0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    72,     0,     0,    71,    91,     8,
       0,    82,     0,    84,     0,    78,    74,    77,    73,     0,
       0,     0,     0,    13,    81,    83,    80,    76,    79,    75
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -211,  -211,   407,  -211,   164,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,   200,  -211,  -211,   247,  -122,     0,
     246,    -2,  -211,   243,   188,   -16,   137,   -28,  -211,  -211,
    -210,  -211
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    42,    43,    44,    45,    46,    47,    48,
      49,   248,    50,    51,   258,    52,   259,   271,   246,    89,
     100,   145,    54,    55,    56,    57,    58,    59,    60,    61,
     146,    62
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,   244,   244,   379,   263,    86,   244,   382,   142,   268,
     272,   109,   110,   183,     3,    80,    82,   163,    19,    85,
     402,   347,    90,    92,    93,   405,   296,   297,   306,   307,
      66,    98,   103,    69,   105,   104,    70,   155,    83,   111,
     172,   143,   385,   213,   174,   386,   306,   307,    67,    88,
     315,   309,   310,   311,   348,   156,   380,    68,   173,   214,
     383,   136,   175,   138,    65,   140,   141,   328,   276,   277,
     381,   308,    87,   403,   384,   245,   394,   157,   406,   159,
     396,   161,   162,   298,    88,   164,   184,   404,   323,   376,
     312,   221,   407,   222,   176,   177,    84,   178,   179,   180,
     181,   227,    88,   207,   208,   209,   210,   291,    63,   229,
     202,   203,   126,   127,    94,   228,    69,   363,    95,    70,
      96,    64,   366,   230,   106,   128,   129,   130,     4,     5,
       6,     7,   128,   129,   130,   108,    76,    77,    78,    12,
      13,    14,    15,   231,    97,   223,   225,   226,   112,   242,
     243,   233,   234,    99,   236,   392,   349,   232,    27,   350,
     351,   124,   125,    32,    33,    34,    53,   237,    36,   186,
     238,   250,   186,   252,   165,   166,   135,     4,     5,     6,
       7,   294,   256,   295,    75,    76,    77,    78,    12,    13,
      14,    15,   131,    39,   128,   129,   130,   150,   151,   152,
      40,    23,   132,   133,   134,   113,   114,    27,    41,   331,
     332,   261,    32,    33,    34,   107,   221,    36,   318,   273,
     137,   221,   275,   322,   324,   279,   325,   281,   292,   282,
      71,    72,    73,   139,    74,   286,   287,   364,   221,   288,
     289,   290,    39,   393,   221,   293,   128,   129,   130,    40,
     128,   129,   130,   303,   304,   395,   397,    41,   195,   196,
     197,    99,   319,   320,   321,   204,   205,   206,   221,   147,
     326,   327,   148,   186,     4,     5,     6,     7,   330,   101,
     102,    75,    76,    77,    78,    12,    13,    14,    15,   324,
     221,   355,   356,   149,   341,   340,   153,   342,    23,   365,
     221,   186,   391,   221,    27,   410,   294,   154,   412,    32,
      33,    34,   200,   201,    36,   115,   116,   117,   118,   119,
     120,   361,   167,   294,   121,   414,   158,   171,   122,   123,
     124,   125,   182,   160,   169,   170,   370,   371,   372,    39,
     186,   373,   185,   211,   212,   375,    40,   215,   216,   218,
     217,   235,   219,   220,    41,   144,   187,   188,   189,   190,
     191,   192,   193,   194,   239,   198,   199,   240,   241,   249,
     251,   253,   254,   255,   257,   260,   264,   265,   399,   266,
     274,   401,   278,   283,   284,   299,   285,   301,   408,   316,
     300,   302,   305,   336,   411,   334,   413,   317,   337,   329,
     333,   420,   338,   339,   422,   335,   343,   344,     4,     5,
       6,     7,   424,   345,   425,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,   352,   353,    20,
      21,    22,    23,    24,    25,    26,   354,   362,    27,    28,
      29,    30,    31,    32,    33,    34,    35,   357,    36,    37,
     358,   359,   374,    38,     4,     5,     6,     7,   360,   367,
     368,    75,    76,    77,    78,    12,    13,    14,    15,   377,
     378,   387,   388,    39,   389,   398,   390,   400,    23,   409,
      40,   415,   416,   417,    27,   418,   419,   421,    41,    32,
      33,    34,   423,   247,    36,   168,   426,   427,   428,   429,
     369,     4,     5,     6,     7,   346,     0,   314,    75,    76,
      77,    78,    12,    13,    14,    15,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    23,    40,     0,     0,     0,
       0,    27,     0,     0,    41,   262,    32,    33,    34,     0,
       0,    36,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    75,    76,    77,    78,   269,
      13,    14,    15,     0,     0,     0,    39,     0,     0,     0,
       0,     0,    23,    40,     0,     0,     0,     0,    27,     0,
       0,    41,   267,    32,    33,    34,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    75,    76,    77,    78,   269,    13,    14,    15,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    23,
      40,     0,     0,     0,     0,    27,     0,     0,    41,   270,
      32,    33,    34,     0,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,     0,    40,     0,     0,
       4,     5,     6,     7,     0,    41,   313,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,     0,     0,    38,     4,     5,     6,     7,
       0,     0,     0,    75,    76,    77,    78,    12,    13,    14,
      15,     0,     0,     0,     0,    39,     0,     0,     0,     0,
      23,     0,    40,     0,     0,     0,    27,     0,     0,     0,
      41,    32,    33,    34,     0,     0,    36,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    75,
      76,    77,    78,    12,    13,    14,    15,     0,     0,     0,
       0,    39,     0,     0,     0,     0,    23,     0,    40,     0,
       0,     0,    27,     0,    91,     0,    41,    32,    33,    34,
       0,     0,    36,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    75,    76,    77,    78,    12,
      13,    14,    15,     0,     0,     0,     0,    39,     0,     0,
       0,     0,    23,     0,    40,     0,     0,     0,    27,     0,
       0,     0,    79,    32,    33,    34,     0,     0,    36,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    75,    76,    77,    78,    12,    13,    14,    15,     0,
       0,     0,     0,    39,     0,     0,     0,     0,    23,     0,
      40,     0,     0,     0,    27,     0,     0,     0,    81,    32,
      33,    34,     0,     0,    36,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    75,    76,    77,
      78,    12,    13,    14,    15,     0,     0,     0,     0,    39,
       0,     0,     0,     0,    23,     0,    40,     0,     0,     0,
      27,     0,     0,     0,    41,    32,    33,    34,     0,     0,
      36,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    75,    76,    77,    78,    12,    13,    14,
      15,     0,     0,     0,     0,    39,     0,     0,     0,     0,
      23,     0,    40,     0,     0,     0,    27,     0,     0,     0,
     224,    32,    33,    34,     0,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,   280
};

static const yytype_int16 yycheck[] =
{
       2,    10,    10,    14,   214,    21,    10,    14,    14,   219,
     220,    39,    40,    14,     0,    17,    18,    55,    21,    21,
      14,    14,    24,    25,    26,    14,    31,    32,    45,    46,
      69,    31,    34,    80,    36,    35,    83,    69,    83,    41,
      69,    47,    10,    67,    69,    13,    45,    46,    87,    87,
     260,    49,    50,    51,    47,    87,    67,    67,    87,    83,
      67,    63,    87,    65,    85,    67,    68,   277,    81,    82,
      81,    88,    14,    67,    81,    84,    84,    79,    67,    81,
      84,    83,    84,    88,    87,    85,    87,    81,    52,    88,
      88,    82,    81,    84,    96,    97,    83,    99,   100,   101,
     102,    67,    87,   131,   132,   133,   134,    55,    67,    67,
     126,   127,    53,    54,    12,    81,    80,   327,    12,    83,
      83,    80,   332,    81,    14,    73,    74,    75,     3,     4,
       5,     6,    73,    74,    75,    10,    11,    12,    13,    14,
      15,    16,    17,    67,    83,   147,   148,   149,    81,   165,
     166,   153,   154,    87,   156,   365,    10,    81,    33,    13,
      14,    71,    72,    38,    39,    40,   168,    82,    43,    84,
      82,   173,    84,   175,    53,    54,    80,     3,     4,     5,
       6,    82,   182,    84,    10,    11,    12,    13,    14,    15,
      16,    17,    66,    68,    73,    74,    75,    10,    11,    12,
      75,    27,    76,    77,    78,    62,    63,    33,    83,    81,
      82,   213,    38,    39,    40,    14,    82,    43,    84,   221,
      14,    82,   224,    84,    82,   227,    84,   229,    55,   231,
      10,    11,    12,    14,    14,   237,   238,    81,    82,   239,
     240,   241,    68,    81,    82,   245,    73,    74,    75,    75,
      73,    74,    75,   253,   254,   377,   378,    83,     7,     8,
       9,    87,   264,   265,   266,   128,   129,   130,    82,    67,
      84,    82,    67,    84,     3,     4,     5,     6,   280,    33,
      34,    10,    11,    12,    13,    14,    15,    16,    17,    82,
      82,    84,    84,    67,   296,   295,    67,   297,    27,    82,
      82,    84,    84,    82,    33,    84,    82,    85,    84,    38,
      39,    40,   124,   125,    43,    56,    57,    58,    59,    60,
      61,   323,    83,    82,    65,    84,    81,    87,    69,    70,
      71,    72,    42,    81,    81,    81,   336,   337,   338,    68,
      84,   341,    87,    14,    81,   345,    75,    86,    70,    81,
      88,    14,    83,    83,    83,    84,   113,   114,   115,   116,
     117,   118,   119,   120,    84,   122,   123,    84,    10,    14,
      14,    84,    84,    88,    14,    83,    67,    67,   380,    67,
      81,   383,    81,    86,    70,    70,    88,    70,   390,    81,
      88,    88,    87,    22,   394,    84,   396,    81,    10,    81,
      81,   403,    10,    10,   406,    84,    81,    81,     3,     4,
       5,     6,   412,    22,   414,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    14,    14,    24,
      25,    26,    27,    28,    29,    30,    14,    14,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    81,    43,    44,
      81,    81,    88,    48,     3,     4,     5,     6,    81,    81,
      81,    10,    11,    12,    13,    14,    15,    16,    17,    83,
      83,     7,     7,    68,     7,    14,    52,    14,    27,    81,
      75,    81,    81,    81,    33,    81,    14,    14,    83,    38,
      39,    40,    81,    88,    43,    88,    81,    81,    81,    81,
     336,     3,     4,     5,     6,   305,    -1,   260,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    27,    75,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    83,    84,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    27,    75,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    83,    84,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    27,
      75,    -1,    -1,    -1,    -1,    33,    -1,    -1,    83,    84,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
       3,     4,     5,     6,    -1,    83,    84,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      27,    -1,    75,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      83,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    27,    -1,    75,    -1,
      -1,    -1,    33,    -1,    81,    -1,    83,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    27,    -1,    75,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    83,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    27,    -1,
      75,    -1,    -1,    -1,    33,    -1,    -1,    -1,    83,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    27,    -1,    75,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    83,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      27,    -1,    75,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      83,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    90,    91,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      24,    25,    26,    27,    28,    29,    30,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    48,    68,
      75,    83,    92,    93,    94,    95,    96,    97,    98,    99,
     101,   102,   104,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   120,    67,    80,    85,    69,    87,    67,    80,
      83,    10,    11,    12,    14,    10,    11,    12,    13,    83,
     110,    83,   110,    83,    83,   110,   114,    14,    87,   108,
     110,    81,   110,   110,    12,    12,    83,    83,   108,    87,
     109,   109,   109,   110,   108,   110,    14,    14,    10,   116,
     116,   110,    81,    62,    63,    56,    57,    58,    59,    60,
      61,    65,    69,    70,    71,    72,    53,    54,    73,    74,
      75,    66,    76,    77,    78,    80,   110,    14,   110,    14,
     110,   110,    14,    47,    84,   110,   119,    67,    67,    67,
      10,    11,    12,    67,    85,    69,    87,   110,    81,   110,
      81,   110,   110,    55,   108,    53,    54,    83,    91,    81,
      81,    87,    69,    87,    69,    87,   110,   110,   110,   110,
     110,   110,    42,    14,    87,    87,    84,   112,   112,   112,
     112,   112,   112,   112,   112,     7,     8,     9,   112,   112,
     113,   113,   114,   114,   115,   115,   115,   116,   116,   116,
     116,    14,    81,    67,    83,    86,    70,    88,    81,    83,
      83,    82,    84,   110,    83,   110,   110,    67,    81,    67,
      81,    67,    81,   110,   110,    14,   110,    82,    82,    84,
      84,    10,   114,   114,    10,    84,   107,    88,   100,    14,
     110,    14,   110,    84,    84,    88,   108,    14,   103,   105,
      83,   110,    84,   119,    67,    67,    67,    84,   119,    14,
      84,   106,   119,   110,    81,   110,    81,    82,    81,   110,
      83,   110,   110,    86,    70,    88,   110,   110,   108,   108,
     108,    55,    55,   108,    82,    84,    31,    32,    88,    70,
      88,    70,    88,   108,   108,    87,    45,    46,    88,    49,
      50,    51,    88,    84,   106,   119,    81,    81,    84,   110,
     110,   110,    84,    52,    82,    84,    84,    82,   119,    81,
     110,    81,    82,    81,    84,    84,    22,    10,    10,    10,
     108,   110,   108,    81,    81,    22,   103,    14,    47,    10,
      13,    14,    14,    14,    14,    84,    84,    81,    81,    81,
      81,   110,    14,   119,    81,    82,   119,    81,    81,    93,
     108,   108,   108,   108,    88,   108,    88,    83,    83,    14,
      67,    81,    14,    67,    81,    10,    13,     7,     7,     7,
      52,    84,   119,    81,    84,   107,    84,   107,    14,   110,
      14,   110,    14,    67,    81,    14,    67,    81,   110,    81,
      84,   108,    84,   108,    84,    81,    81,    81,    81,    14,
     110,    14,   110,    81,   108,   108,    81,    81,    81,    81
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    89,    90,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    93,    93,
      93,    94,    95,    95,    96,    97,    98,    98,    98,    98,
      99,    99,   100,   100,   101,   101,   102,   102,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   104,   105,   105,   105,   105,
     106,   106,   107,   107,   108,   109,   110,   110,   110,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   112,   112,   112,   113,   113,   113,   114,
     114,   114,   114,   115,   115,   115,   115,   115,   116,   116,
     116,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   119,
     119,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120
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
       1,     1,     4,     3,     6,     6,     5,     6,     5,     6,
       6,     5,     3,     2,     3,     3,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     5,     5,     1,     1,     3
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
#line 194 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1719 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 204 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1725 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 205 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1731 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 209 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1737 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 211 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1743 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 213 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1751 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 217 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1759 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 221 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1765 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 223 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1771 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 225 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1777 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 227 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1785 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 231 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1793 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 235 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1799 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 237 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1805 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 239 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1811 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 241 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1817 "raku.tab.c"
    break;

  case 18: /* stmt: KW_SAY expr ';'  */
#line 243 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1823 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 245 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1829 "raku.tab.c"
    break;

  case 20: /* stmt: KW_PRINT expr ';'  */
#line 247 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1835 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 249 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1841 "raku.tab.c"
    break;

  case 22: /* stmt: KW_TAKE expr ';'  */
#line 251 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1847 "raku.tab.c"
    break;

  case 23: /* stmt: KW_RETURN expr ';'  */
#line 253 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1853 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN ';'  */
#line 255 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1859 "raku.tab.c"
    break;

  case 25: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 257 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1865 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 259 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1874 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 264 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1882 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 268 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 1893 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 275 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 1902 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 280 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1909 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 283 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1916 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 286 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1923 "raku.tab.c"
    break;

  case 33: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 289 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1930 "raku.tab.c"
    break;

  case 34: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 292 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1937 "raku.tab.c"
    break;

  case 35: /* stmt: expr ';'  */
#line 294 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1943 "raku.tab.c"
    break;

  case 36: /* stmt: if_stmt  */
#line 295 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1949 "raku.tab.c"
    break;

  case 37: /* stmt: while_stmt  */
#line 296 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1955 "raku.tab.c"
    break;

  case 38: /* stmt: for_stmt  */
#line 297 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1961 "raku.tab.c"
    break;

  case 39: /* stmt: given_stmt  */
#line 298 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1967 "raku.tab.c"
    break;

  case 40: /* stmt: KW_TRY block  */
#line 300 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1973 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TRY block KW_CATCH block  */
#line 302 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1979 "raku.tab.c"
    break;

  case 42: /* stmt: unless_stmt  */
#line 303 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1985 "raku.tab.c"
    break;

  case 43: /* stmt: until_stmt  */
#line 304 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1991 "raku.tab.c"
    break;

  case 44: /* stmt: repeat_stmt  */
#line 305 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1997 "raku.tab.c"
    break;

  case 45: /* stmt: sub_decl  */
#line 306 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2003 "raku.tab.c"
    break;

  case 46: /* stmt: class_decl  */
#line 307 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2009 "raku.tab.c"
    break;

  case 47: /* stmt: grammar_decl  */
#line 308 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2015 "raku.tab.c"
    break;

  case 48: /* if_stmt: KW_IF '(' expr ')' block  */
#line 312 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2021 "raku.tab.c"
    break;

  case 49: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 314 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2027 "raku.tab.c"
    break;

  case 50: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 316 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2033 "raku.tab.c"
    break;

  case 51: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 320 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2039 "raku.tab.c"
    break;

  case 52: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 324 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2045 "raku.tab.c"
    break;

  case 53: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 326 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2051 "raku.tab.c"
    break;

  case 54: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 330 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2057 "raku.tab.c"
    break;

  case 55: /* repeat_stmt: KW_REPEAT block  */
#line 334 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2063 "raku.tab.c"
    break;

  case 56: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 338 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2073 "raku.tab.c"
    break;

  case 57: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 344 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2083 "raku.tab.c"
    break;

  case 58: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 350 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2092 "raku.tab.c"
    break;

  case 59: /* for_stmt: KW_FOR expr block  */
#line 355 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2099 "raku.tab.c"
    break;

  case 60: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 360 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2111 "raku.tab.c"
    break;

  case 61: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 368 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2124 "raku.tab.c"
    break;

  case 62: /* when_list: %empty  */
#line 378 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2130 "raku.tab.c"
    break;

  case 63: /* when_list: when_list KW_WHEN expr block  */
#line 380 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2138 "raku.tab.c"
    break;

  case 64: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 386 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2150 "raku.tab.c"
    break;

  case 65: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 394 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2160 "raku.tab.c"
    break;

  case 66: /* class_decl: KW_CLASS IDENT '{' class_body_list '}'  */
#line 402 "raku.y"
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
#line 2177 "raku.tab.c"
    break;

  case 67: /* class_decl: KW_CLASS IDENT IDENT IDENT '{' class_body_list '}'  */
#line 415 "raku.y"
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
#line 2198 "raku.tab.c"
    break;

  case 68: /* class_body_list: %empty  */
#line 433 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2204 "raku.tab.c"
    break;

  case 69: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 435 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2211 "raku.tab.c"
    break;

  case 70: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 438 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2218 "raku.tab.c"
    break;

  case 71: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 441 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2225 "raku.tab.c"
    break;

  case 72: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 444 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2232 "raku.tab.c"
    break;

  case 73: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 447 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2239 "raku.tab.c"
    break;

  case 74: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 450 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2246 "raku.tab.c"
    break;

  case 75: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 453 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2253 "raku.tab.c"
    break;

  case 76: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 456 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2260 "raku.tab.c"
    break;

  case 77: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 459 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2270 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 465 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2280 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 471 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2290 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 477 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2300 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 483 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2314 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 493 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2326 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 501 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2339 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 510 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2350 "raku.tab.c"
    break;

  case 85: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 519 "raku.y"
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
#line 2367 "raku.tab.c"
    break;

  case 86: /* grammar_body_list: %empty  */
#line 533 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2373 "raku.tab.c"
    break;

  case 87: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 535 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2382 "raku.tab.c"
    break;

  case 88: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 540 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2391 "raku.tab.c"
    break;

  case 89: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 545 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2400 "raku.tab.c"
    break;

  case 90: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 552 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2408 "raku.tab.c"
    break;

  case 91: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 556 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2416 "raku.tab.c"
    break;

  case 92: /* param_list: VAR_SCALAR  */
#line 561 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2422 "raku.tab.c"
    break;

  case 93: /* param_list: param_list ',' VAR_SCALAR  */
#line 562 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2428 "raku.tab.c"
    break;

  case 94: /* block: '{' stmt_list '}'  */
#line 565 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2434 "raku.tab.c"
    break;

  case 95: /* closure: '{' expr '}'  */
#line 568 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2440 "raku.tab.c"
    break;

  case 96: /* expr: VAR_SCALAR '=' expr  */
#line 571 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2446 "raku.tab.c"
    break;

  case 97: /* expr: KW_GATHER block  */
#line 572 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2456 "raku.tab.c"
    break;

  case 98: /* expr: cmp_expr  */
#line 577 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2462 "raku.tab.c"
    break;

  case 99: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 580 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2468 "raku.tab.c"
    break;

  case 100: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 581 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2474 "raku.tab.c"
    break;

  case 101: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 582 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2480 "raku.tab.c"
    break;

  case 102: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 583 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2486 "raku.tab.c"
    break;

  case 103: /* cmp_expr: jct_expr '<' jct_expr  */
#line 584 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2492 "raku.tab.c"
    break;

  case 104: /* cmp_expr: jct_expr '>' jct_expr  */
#line 585 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2498 "raku.tab.c"
    break;

  case 105: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 586 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2504 "raku.tab.c"
    break;

  case 106: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 587 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2510 "raku.tab.c"
    break;

  case 107: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 588 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2516 "raku.tab.c"
    break;

  case 108: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 589 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2522 "raku.tab.c"
    break;

  case 109: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 591 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2532 "raku.tab.c"
    break;

  case 110: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 597 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2542 "raku.tab.c"
    break;

  case 111: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 603 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2552 "raku.tab.c"
    break;

  case 112: /* cmp_expr: jct_expr  */
#line 608 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2558 "raku.tab.c"
    break;

  case 113: /* jct_expr: jct_expr '|' range_expr  */
#line 611 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2564 "raku.tab.c"
    break;

  case 114: /* jct_expr: jct_expr '&' range_expr  */
#line 612 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2570 "raku.tab.c"
    break;

  case 115: /* jct_expr: range_expr  */
#line 613 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2576 "raku.tab.c"
    break;

  case 116: /* range_expr: add_expr OP_RANGE add_expr  */
#line 616 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2582 "raku.tab.c"
    break;

  case 117: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 617 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2588 "raku.tab.c"
    break;

  case 118: /* range_expr: add_expr  */
#line 618 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2594 "raku.tab.c"
    break;

  case 119: /* add_expr: add_expr '+' mul_expr  */
#line 621 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2600 "raku.tab.c"
    break;

  case 120: /* add_expr: add_expr '-' mul_expr  */
#line 622 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2606 "raku.tab.c"
    break;

  case 121: /* add_expr: add_expr '~' mul_expr  */
#line 623 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2612 "raku.tab.c"
    break;

  case 122: /* add_expr: mul_expr  */
#line 624 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2618 "raku.tab.c"
    break;

  case 123: /* mul_expr: mul_expr '*' unary_expr  */
#line 627 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2624 "raku.tab.c"
    break;

  case 124: /* mul_expr: mul_expr '/' unary_expr  */
#line 628 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2630 "raku.tab.c"
    break;

  case 125: /* mul_expr: mul_expr '%' unary_expr  */
#line 629 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2636 "raku.tab.c"
    break;

  case 126: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 630 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2642 "raku.tab.c"
    break;

  case 127: /* mul_expr: unary_expr  */
#line 631 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2648 "raku.tab.c"
    break;

  case 128: /* unary_expr: '-' unary_expr  */
#line 634 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2654 "raku.tab.c"
    break;

  case 129: /* unary_expr: '!' unary_expr  */
#line 635 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2660 "raku.tab.c"
    break;

  case 130: /* unary_expr: postfix_expr  */
#line 636 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2666 "raku.tab.c"
    break;

  case 131: /* postfix_expr: call_expr  */
#line 638 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2672 "raku.tab.c"
    break;

  case 132: /* call_expr: IDENT '(' arg_list ')'  */
#line 641 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2681 "raku.tab.c"
    break;

  case 133: /* call_expr: IDENT '(' ')'  */
#line 645 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2687 "raku.tab.c"
    break;

  case 134: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 647 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2697 "raku.tab.c"
    break;

  case 135: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 653 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2707 "raku.tab.c"
    break;

  case 136: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 659 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2715 "raku.tab.c"
    break;

  case 137: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 663 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2726 "raku.tab.c"
    break;

  case 138: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 670 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2735 "raku.tab.c"
    break;

  case 139: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 675 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2746 "raku.tab.c"
    break;

  case 140: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 682 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2757 "raku.tab.c"
    break;

  case 141: /* call_expr: atom '.' IDENT '(' ')'  */
#line 689 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2766 "raku.tab.c"
    break;

  case 142: /* call_expr: atom '.' IDENT  */
#line 694 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2775 "raku.tab.c"
    break;

  case 143: /* call_expr: KW_DIE expr  */
#line 699 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 2781 "raku.tab.c"
    break;

  case 144: /* call_expr: KW_MAP closure expr  */
#line 701 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2787 "raku.tab.c"
    break;

  case 145: /* call_expr: KW_GREP closure expr  */
#line 703 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2793 "raku.tab.c"
    break;

  case 146: /* call_expr: KW_SORT expr  */
#line 705 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2799 "raku.tab.c"
    break;

  case 147: /* call_expr: KW_SORT closure expr  */
#line 707 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2805 "raku.tab.c"
    break;

  case 148: /* call_expr: atom  */
#line 708 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2811 "raku.tab.c"
    break;

  case 149: /* arg_list: expr  */
#line 711 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2817 "raku.tab.c"
    break;

  case 150: /* arg_list: arg_list ',' expr  */
#line 712 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2823 "raku.tab.c"
    break;

  case 151: /* atom: LIT_INT  */
#line 715 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2829 "raku.tab.c"
    break;

  case 152: /* atom: LIT_FLOAT  */
#line 716 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2835 "raku.tab.c"
    break;

  case 153: /* atom: LIT_STR  */
#line 717 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 2841 "raku.tab.c"
    break;

  case 154: /* atom: LIT_INTERP_STR  */
#line 718 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2847 "raku.tab.c"
    break;

  case 155: /* atom: VAR_SCALAR  */
#line 719 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2853 "raku.tab.c"
    break;

  case 156: /* atom: VAR_ARRAY  */
#line 720 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2859 "raku.tab.c"
    break;

  case 157: /* atom: VAR_HASH  */
#line 721 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2865 "raku.tab.c"
    break;

  case 158: /* atom: VAR_CAPTURE  */
#line 723 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2873 "raku.tab.c"
    break;

  case 159: /* atom: VAR_FH  */
#line 727 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2881 "raku.tab.c"
    break;

  case 160: /* atom: VAR_NAMED_CAPTURE  */
#line 731 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 2888 "raku.tab.c"
    break;

  case 161: /* atom: VAR_ARRAY '[' expr ']'  */
#line 734 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2894 "raku.tab.c"
    break;

  case 162: /* atom: VAR_HASH '<' IDENT '>'  */
#line 736 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2900 "raku.tab.c"
    break;

  case 163: /* atom: VAR_HASH '{' expr '}'  */
#line 738 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2906 "raku.tab.c"
    break;

  case 164: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 740 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2912 "raku.tab.c"
    break;

  case 165: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 742 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2918 "raku.tab.c"
    break;

  case 166: /* atom: IDENT  */
#line 743 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2924 "raku.tab.c"
    break;

  case 167: /* atom: VAR_TWIGIL  */
#line 745 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 2932 "raku.tab.c"
    break;

  case 168: /* atom: '(' expr ')'  */
#line 748 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 2938 "raku.tab.c"
    break;


#line 2942 "raku.tab.c"

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

#line 750 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
