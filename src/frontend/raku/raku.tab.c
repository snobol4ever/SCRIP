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
#line 11 "raku.y"

#include "../../ast/ast.h"
#include "../snobol4/scrip_cc.h"
#include "raku.tab.h"
#include "raku_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int  raku_yylex(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int  raku_get_lineno(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void raku_yyerror(const char *msg) {
    fprintf(stderr, "raku parse error line %d: %s\n", raku_get_lineno(), msg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_call(const char *name) {
    tree_t *e = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(e, n);
    return e;
}
/* RK-BB-4a infix junctions: `l | r` ⇒ any(l,r), `l & r` ⇒ all(l,r). Built as the SAME TT_FNC
   junction-constructor node make_call() produces (c[0]=TT_VAR(flavor) dup, which the lowering's
   jskip drops), so infix and constructor forms share one lowering + collapse path. Left-assoc
   nesting (1|2|3 ⇒ any(any(1,2),3)) is flattened by the recursive junction_collapse. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_seq(ExprList *stmts) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
tree_t *raku_prog_result = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
#define RAKU_METH_MAX 256
typedef struct { char key[128]; char procname[128]; } RakuMethEntry;
static RakuMethEntry raku_meth_table[RAKU_METH_MAX];
static int           raku_meth_ntypes = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void raku_meth_register(const char *classname, const char *methname, const char *procname) {
    if (raku_meth_ntypes >= RAKU_METH_MAX) return;
    RakuMethEntry *e = &raku_meth_table[raku_meth_ntypes++];
    snprintf(e->key,      sizeof e->key,      "%s::%s", classname, methname);
    snprintf(e->procname, sizeof e->procname,  "%s",     procname);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}

#line 215 "raku.tab.c"

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
#define YYLAST   932

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  89
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  150
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  375

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
       0,   196,   196,   207,   208,   211,   213,   215,   219,   223,
     225,   227,   229,   233,   237,   239,   241,   243,   245,   247,
     249,   251,   253,   255,   257,   259,   261,   266,   269,   272,
     275,   278,   281,   282,   283,   284,   285,   286,   288,   290,
     291,   292,   293,   294,   295,   298,   300,   302,   306,   310,
     312,   316,   320,   324,   330,   336,   341,   346,   354,   365,
     366,   372,   380,   388,   403,   404,   407,   410,   420,   430,
     445,   446,   451,   456,   463,   467,   473,   474,   477,   480,
     483,   484,   489,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   508,   514,   520,   523,   524,   525,
     528,   529,   530,   533,   534,   535,   536,   539,   540,   541,
     542,   543,   546,   547,   548,   550,   552,   557,   558,   564,
     568,   575,   580,   587,   592,   597,   599,   601,   603,   605,
     607,   610,   611,   614,   615,   616,   617,   618,   619,   620,
     621,   625,   629,   632,   634,   636,   638,   640,   642,   643,
     647
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

#define YYPACT_NINF (-213)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -213,    20,   532,  -213,  -213,  -213,  -213,  -213,   -59,   -55,
     -51,  -213,   -31,  -213,  -213,  -213,   124,   624,   670,   -46,
     -26,   716,    56,    -5,   716,   578,   716,    66,    75,     8,
      16,    -5,    19,    19,   114,    -5,   716,   141,   174,   849,
     849,   716,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,    51,    -9,   115,  -213,    -6,    84,  -213,
    -213,  -213,    60,   716,   182,   716,   185,   716,     3,   259,
      92,   139,   155,   111,   161,   151,   -41,   716,   156,   716,
     162,   716,   716,   -49,    36,   159,  -213,  -213,   163,  -213,
     164,   165,   -36,   -25,   716,   716,  -213,   716,   716,   716,
     716,  -213,   196,  -213,   169,   170,  -213,  -213,  -213,   166,
    -213,   849,   849,   849,   849,   849,   849,   849,   849,   157,
     849,   849,   849,   849,   849,   849,   849,   849,   849,   849,
     849,   849,   849,   232,   168,   184,   172,   189,   173,   177,
     183,  -213,  -213,    55,   716,   762,   716,   -38,     5,     7,
     716,   716,   253,   716,    74,  -213,    85,  -213,   193,   194,
     269,  -213,   849,   849,    -8,   364,  -213,  -213,  -213,   266,
     716,   267,   716,   199,   204,   201,  -213,  -213,  -213,    -5,
    -213,  -213,  -213,    13,    13,    13,    13,    13,    13,    13,
      13,  -213,  -213,  -213,    13,    13,  -213,  -213,   117,   117,
      84,    84,    84,  -213,  -213,  -213,  -213,   207,  -213,   716,
     215,   224,   226,   410,   -11,   716,  -213,   213,   716,    34,
     214,   716,  -213,   808,  -213,   716,  -213,  -213,   210,   230,
     216,   716,   716,    -5,    -5,    -5,   -15,    39,  -213,    -5,
      99,  -213,   -22,   231,   217,   233,   218,    -5,    -5,  -213,
    -213,   -32,   -24,   457,   235,   716,   716,   716,  -213,   116,
     268,  -213,   120,  -213,  -213,   121,  -213,   716,  -213,   238,
     716,    96,   243,  -213,  -213,  -213,   241,   244,   307,  -213,
    -213,   321,   322,  -213,   323,    -5,   716,    -5,  -213,  -213,
    -213,   254,   255,   315,  -213,   324,    32,  -213,   325,   326,
     327,  -213,  -213,   126,  -213,   263,   264,   265,  -213,   716,
     333,  -213,   716,   143,  -213,   127,  -213,   716,  -213,   270,
     271,   -16,    -5,    -5,  -213,  -213,    -5,   260,  -213,  -213,
      -5,   272,   273,   275,   342,   343,   346,  -213,  -213,  -213,
    -213,  -213,   305,   130,  -213,   716,   145,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,    -3,  -213,  -213,  -213,
    -213,  -213,   716,   277,   133,  -213,    -5,   136,  -213,  -213,
     278,  -213,    -5,  -213,  -213
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   133,   134,   135,   136,   137,   138,
     139,   149,   148,   140,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,    33,    34,    39,    40,    41,    35,    36,
      42,    43,    44,     0,    82,    96,    99,   102,   106,   111,
     114,   115,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     3,    81,     0,    24,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,   128,    37,   125,     0,     0,   137,   113,   112,     0,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   117,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,    20,     0,     0,
       0,    56,     0,     0,     0,     0,    22,    23,    59,     0,
       0,     0,     0,     0,     0,     0,   126,   127,   129,     0,
      64,    70,   150,    83,    84,    85,    86,    89,    90,    91,
      92,    93,    94,    95,    87,    88,    97,    98,   100,   101,
     105,   103,   104,   110,   107,   108,   109,   124,    25,     0,
     143,   144,   145,     0,     0,     0,   116,     0,     0,     0,
       0,     0,    15,     0,    16,     0,    17,    80,     0,     0,
       0,     0,     0,     0,     0,     0,   100,   101,    76,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,    79,
      38,     0,     0,     0,     0,     0,     0,     0,   121,     0,
       0,   119,     0,   132,     5,     0,     6,     0,     9,     0,
       0,     0,     0,   143,   144,   145,     0,     0,    45,    48,
      55,     0,     0,    62,     0,     0,     0,     0,    57,   146,
     147,     0,     0,    49,    51,     0,     0,    63,     0,     0,
       0,    69,   123,     0,    26,     0,     0,     0,   120,     0,
       0,   118,     0,     0,    10,     0,    11,     0,    14,     0,
       0,     0,     0,     0,    77,    61,     0,     0,    30,    31,
       0,     0,     0,     0,     0,     0,     0,   122,    27,    28,
      29,    74,     0,     0,     7,     0,     0,    19,    21,    47,
      46,    53,    54,    60,    58,    50,     0,    66,    65,    71,
      72,    73,     0,     0,     0,    12,     0,     0,    75,     8,
       0,    68,     0,    13,    67
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,  -213,   276,  -213,    40,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,     9,     0,
     198,    -2,  -213,   197,   118,   -17,    67,   -28,  -213,  -213,
    -212,  -213
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    42,    43,    44,    45,    46,    47,    48,
      49,   242,    50,    51,   251,    52,   252,   262,   240,    87,
      98,   142,    54,    55,    56,    57,    58,    59,    60,    61,
     143,    62
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,   259,   238,   260,    84,    19,   160,   238,    63,   286,
     287,   107,   108,   295,   296,    78,    80,   139,    66,    83,
       3,    64,    88,    90,    91,   298,   299,   300,   152,   221,
      65,    96,   101,   169,   103,   102,    67,    81,    86,   109,
     281,   303,   332,   222,   171,   333,   153,   124,   125,    68,
     140,   170,    69,   111,   112,   313,   297,    82,   126,   127,
     128,   134,   172,   136,   301,   138,   288,   126,   127,   128,
      85,    86,   223,   261,   225,   154,   239,   156,    92,   158,
     159,   366,    86,   161,   122,   123,   224,    93,   226,   162,
     163,    94,   173,   174,   282,   175,   176,   177,   178,    95,
     343,   203,   204,   205,   206,   346,    97,   198,   199,   126,
     127,   128,   126,   127,   128,   266,   267,     4,     5,     6,
       7,   147,   148,   149,    74,    75,    76,    11,    12,    13,
      14,    15,   110,   364,    70,    71,    72,   215,    73,   216,
     133,    23,   217,   219,   220,   236,   237,    27,   227,   228,
     129,   230,    32,    33,    34,   104,   231,    36,   182,   144,
     130,   131,   132,    53,   191,   192,   193,   232,   244,   182,
     246,   113,   114,   115,   116,   117,   118,   316,   317,   250,
     119,   284,    39,   285,   120,   121,   122,   123,   105,    40,
     126,   127,   128,   200,   201,   202,   135,    41,   215,   137,
     308,    97,   310,   312,   311,   182,   145,   254,   215,   345,
     337,   182,   215,   263,   363,   215,   265,   370,   284,   269,
     372,   271,   146,   272,   344,   215,   365,   215,   150,   276,
     277,    99,   100,   278,   279,   280,   151,   155,   179,   283,
     196,   197,   164,   157,   166,   167,   207,   293,   294,   208,
     182,   209,   168,   305,   306,   307,   180,   181,   210,   211,
     213,   212,     4,     5,     6,     7,   214,   229,   315,    74,
      75,    76,    11,    12,    13,    14,    15,   233,   234,   235,
     243,   245,   255,   247,   326,   325,    23,   327,   248,   249,
     253,   256,    27,   257,   264,   268,   273,    32,    33,    34,
     274,   289,    36,   291,   275,   290,   292,   341,   183,   184,
     185,   186,   187,   188,   189,   190,   304,   194,   195,   314,
     309,   350,   351,   352,   318,   319,   353,    39,   320,   321,
     355,   322,   323,   324,    40,   328,   329,   330,   331,   334,
     335,   336,    41,   141,   338,   339,   340,   342,   354,   359,
     360,   347,   348,   361,   357,   356,   358,   362,   369,   373,
     368,   349,   165,     0,     0,   367,   371,     4,     5,     6,
       7,     0,   374,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
       0,     0,    38,     4,     5,     6,     7,     0,     0,     0,
      74,    75,    76,    11,    12,    13,    14,    15,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    23,     0,    40,
       0,     0,     0,    27,     0,     0,     0,    41,    32,    33,
      34,     0,   241,    36,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    74,    75,    76,
      11,    12,    13,    14,    15,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    23,    40,     0,     0,     0,     0,
      27,     0,     0,    41,   258,    32,    33,    34,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     4,     5,     6,     7,     0,
      41,   302,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,    23,
      24,    25,    26,     0,     0,    27,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,     0,     0,
      38,     4,     5,     6,     7,     0,     0,     0,    74,    75,
      76,    11,    12,    13,    14,    15,     0,     0,     0,     0,
      39,     0,     0,     0,     0,    23,     0,    40,     0,     0,
       0,    27,     0,     0,     0,    41,    32,    33,    34,     0,
       0,    36,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    74,    75,    76,    11,    12,    13,
      14,    15,     0,     0,     0,     0,    39,     0,     0,     0,
       0,    23,     0,    40,     0,     0,     0,    27,     0,    89,
       0,    41,    32,    33,    34,     0,     0,    36,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      74,    75,    76,    11,    12,    13,    14,    15,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    23,     0,    40,
       0,     0,     0,    27,     0,     0,     0,    77,    32,    33,
      34,     0,     0,    36,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    74,    75,    76,    11,
      12,    13,    14,    15,     0,     0,     0,     0,    39,     0,
       0,     0,     0,    23,     0,    40,     0,     0,     0,    27,
       0,     0,     0,    79,    32,    33,    34,     0,     0,    36,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    74,    75,    76,    11,    12,    13,    14,    15,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    23,
       0,    40,     0,     0,     0,    27,     0,     0,     0,    41,
      32,    33,    34,     0,     0,    36,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    74,    75,
      76,    11,    12,    13,    14,    15,     0,     0,     0,     0,
      39,     0,     0,     0,     0,    23,     0,    40,     0,     0,
       0,    27,     0,     0,     0,   218,    32,    33,    34,     0,
       0,    36,     4,     5,     6,     7,     0,     0,     0,   106,
      75,    76,    11,    12,    13,    14,    15,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,    27,    40,     0,     0,     0,    32,    33,    34,
       0,   270,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
       0,     0,    41
};

static const yytype_int16 yycheck[] =
{
       2,   213,    10,    14,    21,    21,    55,    10,    67,    31,
      32,    39,    40,    45,    46,    17,    18,    14,    69,    21,
       0,    80,    24,    25,    26,    49,    50,    51,    69,    67,
      85,    31,    34,    69,    36,    35,    87,    83,    87,    41,
      55,   253,    10,    81,    69,    13,    87,    53,    54,    80,
      47,    87,    83,    62,    63,   267,    88,    83,    73,    74,
      75,    63,    87,    65,    88,    67,    88,    73,    74,    75,
      14,    87,    67,    84,    67,    77,    84,    79,    12,    81,
      82,    84,    87,    83,    71,    72,    81,    12,    81,    53,
      54,    83,    94,    95,    55,    97,    98,    99,   100,    83,
     312,   129,   130,   131,   132,   317,    87,   124,   125,    73,
      74,    75,    73,    74,    75,    81,    82,     3,     4,     5,
       6,    10,    11,    12,    10,    11,    12,    13,    14,    15,
      16,    17,    81,   345,    10,    11,    12,    82,    14,    84,
      80,    27,   144,   145,   146,   162,   163,    33,   150,   151,
      66,   153,    38,    39,    40,    14,    82,    43,    84,    67,
      76,    77,    78,   165,     7,     8,     9,    82,   170,    84,
     172,    56,    57,    58,    59,    60,    61,    81,    82,   179,
      65,    82,    68,    84,    69,    70,    71,    72,    14,    75,
      73,    74,    75,   126,   127,   128,    14,    83,    82,    14,
      84,    87,    82,    82,    84,    84,    67,   209,    82,    82,
      84,    84,    82,   215,    84,    82,   218,    84,    82,   221,
      84,   223,    67,   225,    81,    82,    81,    82,    67,   231,
     232,    33,    34,   233,   234,   235,    85,    81,    42,   239,
     122,   123,    83,    81,    81,    81,    14,   247,   248,    81,
      84,    67,    87,   255,   256,   257,    87,    87,    86,    70,
      83,    88,     3,     4,     5,     6,    83,    14,   270,    10,
      11,    12,    13,    14,    15,    16,    17,    84,    84,    10,
      14,    14,    67,    84,   286,   285,    27,   287,    84,    88,
      83,    67,    33,    67,    81,    81,    86,    38,    39,    40,
      70,    70,    43,    70,    88,    88,    88,   309,   111,   112,
     113,   114,   115,   116,   117,   118,    81,   120,   121,    81,
      52,   321,   322,   323,    81,    84,   326,    68,    84,    22,
     330,    10,    10,    10,    75,    81,    81,    22,    14,    14,
      14,    14,    83,    84,    81,    81,    81,    14,    88,     7,
       7,    81,    81,     7,    81,    83,    81,    52,    81,    81,
     362,   321,    86,    -1,    -1,   356,   366,     3,     4,     5,
       6,    -1,   372,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    27,    -1,    75,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    83,    38,    39,
      40,    -1,    88,    43,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    27,    75,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    83,    84,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,     3,     4,     5,     6,    -1,
      83,    84,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    -1,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    27,    -1,    75,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    83,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    27,    -1,    75,    -1,    -1,    -1,    33,    -1,    81,
      -1,    83,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    27,    -1,    75,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    83,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    27,    -1,    75,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    83,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    27,
      -1,    75,    -1,    -1,    -1,    33,    -1,    -1,    -1,    83,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    27,    -1,    75,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    83,    38,    39,    40,    -1,
      -1,    43,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    33,    75,    -1,    -1,    -1,    38,    39,    40,
      -1,    83,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83
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
     117,   118,   120,    67,    80,    85,    69,    87,    80,    83,
      10,    11,    12,    14,    10,    11,    12,    83,   110,    83,
     110,    83,    83,   110,   114,    14,    87,   108,   110,    81,
     110,   110,    12,    12,    83,    83,   108,    87,   109,   109,
     109,   110,   108,   110,    14,    14,    10,   116,   116,   110,
      81,    62,    63,    56,    57,    58,    59,    60,    61,    65,
      69,    70,    71,    72,    53,    54,    73,    74,    75,    66,
      76,    77,    78,    80,   110,    14,   110,    14,   110,    14,
      47,    84,   110,   119,    67,    67,    67,    10,    11,    12,
      67,    85,    69,    87,   110,    81,   110,    81,   110,   110,
      55,   108,    53,    54,    83,    91,    81,    81,    87,    69,
      87,    69,    87,   110,   110,   110,   110,   110,   110,    42,
      87,    87,    84,   112,   112,   112,   112,   112,   112,   112,
     112,     7,     8,     9,   112,   112,   113,   113,   114,   114,
     115,   115,   115,   116,   116,   116,   116,    14,    81,    67,
      86,    70,    88,    83,    83,    82,    84,   110,    83,   110,
     110,    67,    81,    67,    81,    67,    81,   110,   110,    14,
     110,    82,    82,    84,    84,    10,   114,   114,    10,    84,
     107,    88,   100,    14,   110,    14,   110,    84,    84,    88,
     108,   103,   105,    83,   110,    67,    67,    67,    84,   119,
      14,    84,   106,   110,    81,   110,    81,    82,    81,   110,
      83,   110,   110,    86,    70,    88,   110,   110,   108,   108,
     108,    55,    55,   108,    82,    84,    31,    32,    88,    70,
      88,    70,    88,   108,   108,    45,    46,    88,    49,    50,
      51,    88,    84,   119,    81,   110,   110,   110,    84,    52,
      82,    84,    82,   119,    81,   110,    81,    82,    81,    84,
      84,    22,    10,    10,    10,   108,   110,   108,    81,    81,
      22,    14,    10,    13,    14,    14,    14,    84,    81,    81,
      81,   110,    14,   119,    81,    82,   119,    81,    81,    93,
     108,   108,   108,   108,    88,   108,    83,    81,    81,     7,
       7,     7,    52,    84,   119,    81,    84,   107,   110,    81,
      84,   108,    84,    81,   108
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    89,    90,    91,    91,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    93,    93,    93,    94,    95,
      95,    96,    97,    98,    98,    98,    98,    99,    99,   100,
     100,   101,   101,   102,   103,   103,   103,   103,   103,   104,
     105,   105,   105,   105,   106,   106,   107,   107,   108,   109,
     110,   110,   110,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   112,
     113,   113,   113,   114,   114,   114,   114,   115,   115,   115,
     115,   115,   116,   116,   116,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   119,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     5,     7,     9,     5,
       6,     6,     8,    10,     6,     4,     4,     4,     3,     7,
       3,     7,     3,     3,     2,     4,     6,     7,     7,     7,
       6,     6,     2,     1,     1,     1,     1,     2,     4,     1,
       1,     1,     1,     1,     1,     5,     7,     7,     5,     5,
       7,     5,     2,     7,     7,     5,     3,     5,     7,     0,
       4,     6,     5,     5,     0,     4,     4,     7,     6,     5,
       0,     4,     4,     4,     3,     5,     1,     3,     3,     3,
       3,     2,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     2,     2,     1,     1,     4,     3,     6,     5,
       6,     5,     6,     5,     3,     2,     3,     3,     2,     3,
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
#line 197 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1689 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 207 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1695 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 208 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1701 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 212 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1707 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 214 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1713 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 216 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1721 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 220 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1729 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 224 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1735 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 226 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1741 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 228 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1747 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 230 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1755 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 234 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1763 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 238 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1769 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 240 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1775 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 242 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1781 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 244 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1787 "raku.tab.c"
    break;

  case 18: /* stmt: KW_SAY expr ';'  */
#line 246 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1793 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 248 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1799 "raku.tab.c"
    break;

  case 20: /* stmt: KW_PRINT expr ';'  */
#line 250 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1805 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 252 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1811 "raku.tab.c"
    break;

  case 22: /* stmt: KW_TAKE expr ';'  */
#line 254 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1817 "raku.tab.c"
    break;

  case 23: /* stmt: KW_RETURN expr ';'  */
#line 256 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1823 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN ';'  */
#line 258 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1829 "raku.tab.c"
    break;

  case 25: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 260 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1835 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 262 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1844 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 267 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1851 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 270 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1858 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 273 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1865 "raku.tab.c"
    break;

  case 30: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 276 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 1872 "raku.tab.c"
    break;

  case 31: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 279 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1879 "raku.tab.c"
    break;

  case 32: /* stmt: expr ';'  */
#line 281 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 1885 "raku.tab.c"
    break;

  case 33: /* stmt: if_stmt  */
#line 282 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1891 "raku.tab.c"
    break;

  case 34: /* stmt: while_stmt  */
#line 283 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1897 "raku.tab.c"
    break;

  case 35: /* stmt: for_stmt  */
#line 284 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1903 "raku.tab.c"
    break;

  case 36: /* stmt: given_stmt  */
#line 285 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1909 "raku.tab.c"
    break;

  case 37: /* stmt: KW_TRY block  */
#line 287 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1915 "raku.tab.c"
    break;

  case 38: /* stmt: KW_TRY block KW_CATCH block  */
#line 289 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1921 "raku.tab.c"
    break;

  case 39: /* stmt: unless_stmt  */
#line 290 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1927 "raku.tab.c"
    break;

  case 40: /* stmt: until_stmt  */
#line 291 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1933 "raku.tab.c"
    break;

  case 41: /* stmt: repeat_stmt  */
#line 292 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1939 "raku.tab.c"
    break;

  case 42: /* stmt: sub_decl  */
#line 293 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1945 "raku.tab.c"
    break;

  case 43: /* stmt: class_decl  */
#line 294 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1951 "raku.tab.c"
    break;

  case 44: /* stmt: grammar_decl  */
#line 295 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 1957 "raku.tab.c"
    break;

  case 45: /* if_stmt: KW_IF '(' expr ')' block  */
#line 299 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1963 "raku.tab.c"
    break;

  case 46: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 301 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1969 "raku.tab.c"
    break;

  case 47: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1975 "raku.tab.c"
    break;

  case 48: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 307 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 1981 "raku.tab.c"
    break;

  case 49: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 311 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1987 "raku.tab.c"
    break;

  case 50: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 313 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1993 "raku.tab.c"
    break;

  case 51: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 317 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 1999 "raku.tab.c"
    break;

  case 52: /* repeat_stmt: KW_REPEAT block  */
#line 321 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2005 "raku.tab.c"
    break;

  case 53: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 325 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2015 "raku.tab.c"
    break;

  case 54: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 331 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2025 "raku.tab.c"
    break;

  case 55: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 337 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2034 "raku.tab.c"
    break;

  case 56: /* for_stmt: KW_FOR expr block  */
#line 342 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2041 "raku.tab.c"
    break;

  case 57: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 347 "raku.y"
        { /* PRF-12-given: when_list is now flat [val0, body0, val1, body1, ...] — no pair nodes. */
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2053 "raku.tab.c"
    break;

  case 58: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 355 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2066 "raku.tab.c"
    break;

  case 59: /* when_list: %empty  */
#line 365 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2072 "raku.tab.c"
    break;

  case 60: /* when_list: when_list KW_WHEN expr block  */
#line 367 "raku.y"
        { /* PRF-12-given: push val and body directly — no intermediate TT_SEQ_EXPR pair. */
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2080 "raku.tab.c"
    break;

  case 61: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 373 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2092 "raku.tab.c"
    break;

  case 62: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 381 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2102 "raku.tab.c"
    break;

  case 63: /* class_decl: KW_CLASS IDENT '{' class_body_list '}'  */
#line 389 "raku.y"
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
#line 2119 "raku.tab.c"
    break;

  case 64: /* class_body_list: %empty  */
#line 403 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2125 "raku.tab.c"
    break;

  case 65: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 405 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2132 "raku.tab.c"
    break;

  case 66: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 408 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2139 "raku.tab.c"
    break;

  case 67: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 411 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2153 "raku.tab.c"
    break;

  case 68: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 421 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2165 "raku.tab.c"
    break;

  case 69: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 431 "raku.y"
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
#line 2182 "raku.tab.c"
    break;

  case 70: /* grammar_body_list: %empty  */
#line 445 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2188 "raku.tab.c"
    break;

  case 71: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 447 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2197 "raku.tab.c"
    break;

  case 72: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 452 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2206 "raku.tab.c"
    break;

  case 73: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 457 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2215 "raku.tab.c"
    break;

  case 74: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 464 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2223 "raku.tab.c"
    break;

  case 75: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 468 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2231 "raku.tab.c"
    break;

  case 76: /* param_list: VAR_SCALAR  */
#line 473 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2237 "raku.tab.c"
    break;

  case 77: /* param_list: param_list ',' VAR_SCALAR  */
#line 474 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2243 "raku.tab.c"
    break;

  case 78: /* block: '{' stmt_list '}'  */
#line 477 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2249 "raku.tab.c"
    break;

  case 79: /* closure: '{' expr '}'  */
#line 480 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2255 "raku.tab.c"
    break;

  case 80: /* expr: VAR_SCALAR '=' expr  */
#line 483 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2261 "raku.tab.c"
    break;

  case 81: /* expr: KW_GATHER block  */
#line 484 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2271 "raku.tab.c"
    break;

  case 82: /* expr: cmp_expr  */
#line 489 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2277 "raku.tab.c"
    break;

  case 83: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 492 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2283 "raku.tab.c"
    break;

  case 84: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 493 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2289 "raku.tab.c"
    break;

  case 85: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 494 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2295 "raku.tab.c"
    break;

  case 86: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 495 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2301 "raku.tab.c"
    break;

  case 87: /* cmp_expr: jct_expr '<' jct_expr  */
#line 496 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2307 "raku.tab.c"
    break;

  case 88: /* cmp_expr: jct_expr '>' jct_expr  */
#line 497 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2313 "raku.tab.c"
    break;

  case 89: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 498 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2319 "raku.tab.c"
    break;

  case 90: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 499 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2325 "raku.tab.c"
    break;

  case 91: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 500 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2331 "raku.tab.c"
    break;

  case 92: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 501 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2337 "raku.tab.c"
    break;

  case 93: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 503 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2347 "raku.tab.c"
    break;

  case 94: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 509 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2357 "raku.tab.c"
    break;

  case 95: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 515 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2367 "raku.tab.c"
    break;

  case 96: /* cmp_expr: jct_expr  */
#line 520 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2373 "raku.tab.c"
    break;

  case 97: /* jct_expr: jct_expr '|' range_expr  */
#line 523 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2379 "raku.tab.c"
    break;

  case 98: /* jct_expr: jct_expr '&' range_expr  */
#line 524 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2385 "raku.tab.c"
    break;

  case 99: /* jct_expr: range_expr  */
#line 525 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2391 "raku.tab.c"
    break;

  case 100: /* range_expr: add_expr OP_RANGE add_expr  */
#line 528 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2397 "raku.tab.c"
    break;

  case 101: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 529 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2403 "raku.tab.c"
    break;

  case 102: /* range_expr: add_expr  */
#line 530 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2409 "raku.tab.c"
    break;

  case 103: /* add_expr: add_expr '+' mul_expr  */
#line 533 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2415 "raku.tab.c"
    break;

  case 104: /* add_expr: add_expr '-' mul_expr  */
#line 534 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2421 "raku.tab.c"
    break;

  case 105: /* add_expr: add_expr '~' mul_expr  */
#line 535 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2427 "raku.tab.c"
    break;

  case 106: /* add_expr: mul_expr  */
#line 536 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2433 "raku.tab.c"
    break;

  case 107: /* mul_expr: mul_expr '*' unary_expr  */
#line 539 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2439 "raku.tab.c"
    break;

  case 108: /* mul_expr: mul_expr '/' unary_expr  */
#line 540 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2445 "raku.tab.c"
    break;

  case 109: /* mul_expr: mul_expr '%' unary_expr  */
#line 541 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2451 "raku.tab.c"
    break;

  case 110: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 542 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2457 "raku.tab.c"
    break;

  case 111: /* mul_expr: unary_expr  */
#line 543 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2463 "raku.tab.c"
    break;

  case 112: /* unary_expr: '-' unary_expr  */
#line 546 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2469 "raku.tab.c"
    break;

  case 113: /* unary_expr: '!' unary_expr  */
#line 547 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2475 "raku.tab.c"
    break;

  case 114: /* unary_expr: postfix_expr  */
#line 548 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2481 "raku.tab.c"
    break;

  case 115: /* postfix_expr: call_expr  */
#line 550 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2487 "raku.tab.c"
    break;

  case 116: /* call_expr: IDENT '(' arg_list ')'  */
#line 553 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2496 "raku.tab.c"
    break;

  case 117: /* call_expr: IDENT '(' ')'  */
#line 557 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2502 "raku.tab.c"
    break;

  case 118: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 559 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2512 "raku.tab.c"
    break;

  case 119: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 565 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2520 "raku.tab.c"
    break;

  case 120: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 569 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2531 "raku.tab.c"
    break;

  case 121: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 576 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2540 "raku.tab.c"
    break;

  case 122: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 581 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2551 "raku.tab.c"
    break;

  case 123: /* call_expr: atom '.' IDENT '(' ')'  */
#line 588 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2560 "raku.tab.c"
    break;

  case 124: /* call_expr: atom '.' IDENT  */
#line 593 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 2569 "raku.tab.c"
    break;

  case 125: /* call_expr: KW_DIE expr  */
#line 598 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 2575 "raku.tab.c"
    break;

  case 126: /* call_expr: KW_MAP closure expr  */
#line 600 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2581 "raku.tab.c"
    break;

  case 127: /* call_expr: KW_GREP closure expr  */
#line 602 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2587 "raku.tab.c"
    break;

  case 128: /* call_expr: KW_SORT expr  */
#line 604 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2593 "raku.tab.c"
    break;

  case 129: /* call_expr: KW_SORT closure expr  */
#line 606 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 2599 "raku.tab.c"
    break;

  case 130: /* call_expr: atom  */
#line 607 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 2605 "raku.tab.c"
    break;

  case 131: /* arg_list: expr  */
#line 610 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 2611 "raku.tab.c"
    break;

  case 132: /* arg_list: arg_list ',' expr  */
#line 611 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 2617 "raku.tab.c"
    break;

  case 133: /* atom: LIT_INT  */
#line 614 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 2623 "raku.tab.c"
    break;

  case 134: /* atom: LIT_FLOAT  */
#line 615 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 2629 "raku.tab.c"
    break;

  case 135: /* atom: LIT_STR  */
#line 616 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 2635 "raku.tab.c"
    break;

  case 136: /* atom: LIT_INTERP_STR  */
#line 617 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 2641 "raku.tab.c"
    break;

  case 137: /* atom: VAR_SCALAR  */
#line 618 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2647 "raku.tab.c"
    break;

  case 138: /* atom: VAR_ARRAY  */
#line 619 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2653 "raku.tab.c"
    break;

  case 139: /* atom: VAR_HASH  */
#line 620 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2659 "raku.tab.c"
    break;

  case 140: /* atom: VAR_CAPTURE  */
#line 622 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2667 "raku.tab.c"
    break;

  case 141: /* atom: VAR_FH  */
#line 626 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 2675 "raku.tab.c"
    break;

  case 142: /* atom: VAR_NAMED_CAPTURE  */
#line 630 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 2682 "raku.tab.c"
    break;

  case 143: /* atom: VAR_ARRAY '[' expr ']'  */
#line 633 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2688 "raku.tab.c"
    break;

  case 144: /* atom: VAR_HASH '<' IDENT '>'  */
#line 635 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2694 "raku.tab.c"
    break;

  case 145: /* atom: VAR_HASH '{' expr '}'  */
#line 637 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2700 "raku.tab.c"
    break;

  case 146: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 639 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 2706 "raku.tab.c"
    break;

  case 147: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 641 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2712 "raku.tab.c"
    break;

  case 148: /* atom: IDENT  */
#line 642 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 2718 "raku.tab.c"
    break;

  case 149: /* atom: VAR_TWIGIL  */
#line 644 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 2726 "raku.tab.c"
    break;

  case 150: /* atom: '(' expr ')'  */
#line 647 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 2732 "raku.tab.c"
    break;


#line 2736 "raku.tab.c"

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

#line 649 "raku.y"

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void *raku_yy_scan_string(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void  raku_yy_delete_buffer(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
