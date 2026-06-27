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
static const char *rk_multi_mangle(const char *base, ExprList *params) {
    static char buf[512]; int np = params ? params->count : 0;
    int pos = snprintf(buf, sizeof buf, "%s$%d", base, np);
    for (int i = 0; i < np; i++) { tree_t *p = params->items[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        char safe[64]; int j = 0;
        for (const char *c = ty; *c && j < 63; c++, j++) safe[j] = (*c == ':') ? '_' : *c; safe[j] = 0;
        pos += snprintf(buf + pos, sizeof buf - pos, "$%s", safe); }
    return intern(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_param(const char *type, const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_def_param(const char *type, const char *def, const char *name) {
    char buf[160]; snprintf(buf, sizeof buf, "%s%s", type, def);
    return rk_typed_param(intern(buf), name);
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

#line 229 "raku.tab.c"

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
  YYSYMBOL_KW_ROLE = 51,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 52,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 53,                  /* KW_PROTO  */
  YYSYMBOL_OP_COLON_D = 54,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 55,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 56,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 57,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 58,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 59,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 60,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 61,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 62,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 63,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 64,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 65,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 66,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 67,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 68,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 69,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 70,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 71,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 72,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 73,                   /* OP_BIND  */
  YYSYMBOL_OP_SMATCH = 74,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 75,                    /* OP_DIV  */
  YYSYMBOL_76_ = 76,                       /* '='  */
  YYSYMBOL_77_ = 77,                       /* '!'  */
  YYSYMBOL_78_ = 78,                       /* '<'  */
  YYSYMBOL_79_ = 79,                       /* '>'  */
  YYSYMBOL_80_ = 80,                       /* '|'  */
  YYSYMBOL_81_ = 81,                       /* '&'  */
  YYSYMBOL_82_ = 82,                       /* '~'  */
  YYSYMBOL_83_ = 83,                       /* '+'  */
  YYSYMBOL_84_ = 84,                       /* '-'  */
  YYSYMBOL_85_ = 85,                       /* '*'  */
  YYSYMBOL_86_ = 86,                       /* '/'  */
  YYSYMBOL_87_ = 87,                       /* '%'  */
  YYSYMBOL_UMINUS = 88,                    /* UMINUS  */
  YYSYMBOL_89_ = 89,                       /* '.'  */
  YYSYMBOL_90_ = 90,                       /* ';'  */
  YYSYMBOL_91_ = 91,                       /* ','  */
  YYSYMBOL_92_ = 92,                       /* '('  */
  YYSYMBOL_93_ = 93,                       /* ')'  */
  YYSYMBOL_94_ = 94,                       /* '['  */
  YYSYMBOL_95_ = 95,                       /* ']'  */
  YYSYMBOL_96_ = 96,                       /* '{'  */
  YYSYMBOL_97_ = 97,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 98,                  /* $accept  */
  YYSYMBOL_program = 99,                   /* program  */
  YYSYMBOL_stmt_list = 100,                /* stmt_list  */
  YYSYMBOL_stmt = 101,                     /* stmt  */
  YYSYMBOL_if_stmt = 102,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 103,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 104,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 105,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 106,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 107,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 108,               /* given_stmt  */
  YYSYMBOL_when_list = 109,                /* when_list  */
  YYSYMBOL_sub_decl = 110,                 /* sub_decl  */
  YYSYMBOL_class_decl = 111,               /* class_decl  */
  YYSYMBOL_role_decl = 112,                /* role_decl  */
  YYSYMBOL_is_clauses = 113,               /* is_clauses  */
  YYSYMBOL_class_body_list = 114,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 115,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 116,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 117,           /* named_arg_list  */
  YYSYMBOL_param_list = 118,               /* param_list  */
  YYSYMBOL_block = 119,                    /* block  */
  YYSYMBOL_closure = 120,                  /* closure  */
  YYSYMBOL_expr = 121,                     /* expr  */
  YYSYMBOL_cmp_expr = 122,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 123,                 /* jct_expr  */
  YYSYMBOL_range_expr = 124,               /* range_expr  */
  YYSYMBOL_add_expr = 125,                 /* add_expr  */
  YYSYMBOL_mul_expr = 126,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 127,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 128,             /* postfix_expr  */
  YYSYMBOL_call_expr = 129,                /* call_expr  */
  YYSYMBOL_arg_list = 130,                 /* arg_list  */
  YYSYMBOL_atom = 131                      /* atom  */
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
#define YYLAST   1149

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  189
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  472

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


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
       2,     2,     2,    77,     2,     2,     2,    87,    81,     2,
      92,    93,    85,    83,    91,    84,    89,    86,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    90,
      78,    76,    79,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    94,     2,    95,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    96,    80,    97,    82,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    88
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   220,   220,   231,   232,   235,   237,   239,   241,   245,
     249,   251,   253,   255,   259,   263,   265,   267,   269,   271,
     273,   275,   277,   279,   281,   283,   285,   287,   292,   296,
     303,   308,   311,   314,   317,   320,   323,   324,   325,   326,
     327,   328,   330,   332,   333,   334,   335,   336,   337,   338,
     341,   343,   345,   349,   353,   355,   359,   363,   367,   373,
     379,   384,   389,   397,   408,   409,   415,   423,   429,   438,
     447,   463,   478,   479,   493,   494,   497,   500,   503,   506,
     509,   512,   515,   518,   521,   524,   527,   530,   537,   544,
     551,   558,   568,   576,   585,   594,   609,   610,   615,   620,
     627,   631,   637,   638,   639,   640,   641,   642,   643,   644,
     647,   648,   651,   654,   655,   660,   663,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   679,   685,   691,
     694,   695,   696,   699,   700,   701,   704,   705,   706,   707,
     710,   711,   712,   713,   714,   717,   718,   719,   721,   723,
     728,   729,   735,   741,   745,   752,   757,   763,   769,   776,
     783,   788,   793,   795,   797,   799,   801,   803,   806,   807,
     810,   811,   812,   813,   814,   815,   816,   817,   821,   825,
     828,   830,   832,   834,   836,   838,   839,   843,   847,   851
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
  "KW_HAS", "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_COLON_D",
  "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND", "OP_OR", "OP_BIND",
  "OP_SMATCH", "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('",
  "')'", "'['", "']'", "'{'", "'}'", "$accept", "program", "stmt_list",
  "stmt", "if_stmt", "while_stmt", "unless_stmt", "until_stmt",
  "repeat_stmt", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "class_decl", "role_decl", "is_clauses", "class_body_list",
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

#define YYPACT_NINF (-275)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -275,    23,   732,  -275,  -275,  -275,  -275,  -275,   -30,   -13,
     -58,    16,    -9,  -275,  -275,  -275,  -275,  -275,   157,   832,
     877,    22,    33,   922,   156,    76,   922,   787,   922,   163,
     173,   112,   135,    76,   104,   104,   183,    76,   922,   216,
     227,   207,   241,  1057,  1057,   922,  -275,  -275,  -275,  -275,
    -275,  -275,  -275,  -275,  -275,  -275,  -275,  -275,   167,   119,
     269,  -275,    57,    79,  -275,  -275,  -275,   188,   922,   260,
     922,   274,   922,   922,    35,   287,    45,   249,   251,   140,
     256,   210,   -48,  -275,   922,   219,   922,   252,   922,   922,
     -29,    60,   254,   288,  -275,   265,  -275,   266,   261,   -31,
     -21,   922,   922,  -275,   922,   922,   922,   922,  -275,   313,
    -275,  -275,   263,   349,   270,  -275,  -275,  -275,   272,  -275,
    1057,  1057,  1057,  1057,  1057,  1057,  1057,  1057,   209,  1057,
    1057,  1057,  1057,  1057,  1057,  1057,  1057,  1057,  1057,  1057,
    1057,  1057,   145,   277,   -23,   275,   290,   276,   284,   283,
     362,   285,  -275,  -275,    91,   922,  -275,   967,   922,    48,
      55,    56,   922,   922,   364,   922,   146,  -275,   165,  -275,
     291,   292,   371,  -275,  1057,  1057,    -5,   286,   447,  -275,
    -275,  -275,   372,   922,   373,   922,   295,   296,   293,  -275,
    -275,  -275,    76,    -7,  -275,   299,  -275,  -275,   166,   166,
     166,   166,   166,   166,   166,   166,  -275,  -275,  -275,   166,
     166,  -275,  -275,   138,   138,    79,    79,    79,  -275,  -275,
    -275,  -275,   300,   379,  -275,   922,   507,   318,   319,   320,
    -275,   551,  -275,   604,   922,  -275,   307,   922,   160,   308,
     922,  -275,  1012,  -275,   922,  -275,  -275,   304,   321,   305,
     922,   922,    76,    76,    76,    13,    65,  -275,     6,    76,
     170,  -275,  -275,    21,   322,   306,   325,   309,    76,    76,
    -275,  -275,   391,  -275,   -35,     5,   -18,   648,  -275,   317,
     323,   171,   922,   922,   922,  -275,   179,     2,  -275,   180,
     185,  -275,  -275,   192,  -275,   922,  -275,   326,   922,   175,
     327,  -275,  -275,  -275,   315,   328,   384,  -275,  -275,   400,
     402,  -275,   405,   408,  -275,   120,    76,   922,    76,  -275,
    -275,  -275,   329,   330,   397,  -275,  -275,    -4,     8,   164,
    -275,    76,   193,   409,   410,   411,  -275,  -275,   196,   217,
    -275,  -275,   337,   338,   339,   340,  -275,   922,   417,  -275,
    -275,   922,   204,  -275,   220,  -275,   922,  -275,   342,   343,
     -20,    76,    76,  -275,  -275,  -275,    19,  -275,    76,   341,
    -275,  -275,    76,  -275,   345,   347,   -12,    25,   218,   350,
     352,  -275,    76,   428,   436,   438,  -275,  -275,  -275,  -275,
    -275,  -275,  -275,   385,   221,  -275,   922,   250,  -275,  -275,
    -275,  -275,  -275,  -275,  -275,   437,   439,  -275,  -275,  -275,
      17,    18,   434,   922,  -275,   441,   922,  -275,    36,    37,
     374,   382,  -275,  -275,  -275,  -275,  -275,  -275,   922,   383,
     228,  -275,  -275,  -275,    76,   229,    76,   233,   392,   406,
     407,   412,   467,   922,  -275,   478,   922,  -275,  -275,  -275,
    -275,  -275,   413,  -275,    76,  -275,    76,  -275,  -275,  -275,
    -275,   415,   416,   418,   419,  -275,  -275,  -275,  -275,  -275,
    -275,  -275
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   170,   171,   172,   173,   174,   175,
     176,   186,   185,   187,   188,   177,   178,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,    37,    38,    43,
      44,    45,    39,    40,    46,    47,    48,    49,     0,   115,
     129,   132,   135,   139,   144,   147,   148,   167,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     174,   175,   176,   186,     0,     0,     0,     0,     0,     0,
       0,   135,     0,     3,   114,     0,    25,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,   165,    41,
     162,    72,     0,     0,     0,   174,   146,   145,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   150,   168,     0,     0,     6,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,     0,    21,
       0,     0,     0,    61,     0,     0,     0,     0,     0,    23,
      24,    64,     0,     0,     0,     0,     0,     0,     0,   163,
     164,   166,     0,     0,    74,     0,    96,   189,   116,   117,
     118,   119,   122,   123,   124,   125,   126,   127,   128,   120,
     121,   130,   131,   133,   134,   138,   136,   137,   143,   140,
     141,   142,   161,     0,    26,     0,     0,   180,   181,   182,
      28,     0,   156,     0,     0,   149,     0,     0,     0,     0,
       0,    16,     0,    17,     0,    18,   113,     0,     0,     0,
       0,     0,     0,     0,     0,   133,   134,   102,     0,     0,
       0,   111,   110,     0,     0,     0,     0,     0,     0,     0,
     112,    42,     0,    74,     0,     0,     0,     0,   157,     0,
       0,     0,     0,     0,     0,   155,     0,   185,   153,     0,
       0,   169,     5,     0,     7,     0,    10,     0,     0,     0,
       0,   180,   181,   182,     0,     0,    50,    53,    60,     0,
       0,   103,     0,     0,    67,     0,     0,     0,     0,    62,
     183,   184,     0,     0,    54,    56,    73,     0,     0,     0,
      71,     0,     0,     0,     0,     0,    95,   160,     0,     0,
      27,    30,     0,     0,     0,     0,   154,     0,     0,   151,
     152,     0,     0,    11,     0,    12,     0,    15,     0,     0,
       0,     0,     0,   104,   105,   106,     0,    66,     0,     0,
      34,    35,     0,    70,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,   159,   158,    29,    31,
      32,    33,   100,     0,     0,     8,     0,     0,    20,    22,
      52,    51,    58,    59,   107,     0,     0,    65,    63,    55,
       0,     0,     0,     0,    80,     0,     0,    75,     0,     0,
       0,     0,    76,    77,    68,    97,    98,    99,     0,     0,
       0,    13,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,    81,    78,    79,
     101,     9,     0,    92,     0,    94,     0,    88,    84,    87,
      83,     0,     0,     0,     0,    14,    91,    93,    90,    86,
      89,    85
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -275,  -275,   414,  -275,   141,  -275,  -275,  -275,  -275,  -275,
    -275,  -275,  -275,  -275,  -275,  -275,   242,  -275,  -275,   223,
    -274,     0,   316,    -2,  -275,    85,   222,   -17,   108,   -32,
    -275,  -275,  -223,  -275
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    46,    47,    48,    49,    50,    51,    52,
      53,   263,    54,    55,    56,   193,   274,    57,   276,   289,
     260,    94,   105,   153,    59,    60,    61,    62,    63,    64,
      65,    66,   154,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   332,   412,   281,    21,   257,    91,   272,   286,   258,
     290,   116,   117,   328,   329,   257,   311,    85,    87,   258,
      71,    90,   374,     3,    95,    97,    98,   257,   257,   404,
     164,   258,   258,   103,   108,   172,   110,   109,    72,   415,
     333,   334,   335,   118,   328,   329,    68,   182,   165,   149,
     442,   445,   150,   225,   339,   317,   318,   184,   375,    69,
     312,   313,   330,   347,   413,   183,   143,    93,   145,   226,
     147,   148,   352,   405,   406,   185,    93,   309,   414,   336,
      74,    70,   166,    75,   168,   151,   170,   171,   259,   273,
     173,    74,    73,   373,    75,   135,   136,   137,   331,   186,
     187,   416,   188,   189,   190,   191,   218,   219,   220,   221,
     434,   436,   443,   446,    88,   417,   213,   214,   319,   133,
     134,   155,   174,   175,   240,    89,   444,   447,   394,   310,
     365,   242,   244,   397,   366,   156,   435,   437,   241,   135,
     136,   137,   135,   136,   137,   243,   245,   135,   136,   137,
     159,   160,   161,   236,   138,   238,   239,   255,   256,   222,
     246,   247,   223,   249,   139,   140,   141,    76,    77,    78,
      92,    79,    93,   430,   376,    99,    58,   377,   378,   379,
     380,   265,   234,   267,   235,   100,     4,     5,     6,     7,
     120,   121,   271,    80,    81,    82,    83,    12,    13,    14,
     104,    15,    16,    17,   101,   198,   199,   200,   201,   202,
     203,   204,   205,    25,   209,   210,   206,   207,   208,    29,
     135,   136,   137,   279,    34,    35,    36,   102,   418,    38,
     111,   419,   291,   420,   421,   293,   113,   250,   297,   197,
     299,   112,   300,   215,   216,   217,   131,   132,   304,   305,
     294,   295,   306,   307,   308,   114,   251,   119,   197,   314,
      43,   315,   234,   316,   342,   355,   356,    44,   324,   325,
     234,   348,   346,   349,   144,    45,   234,   142,   350,   104,
     343,   344,   345,   351,   315,   197,   382,   348,   146,   386,
       4,     5,     6,     7,   395,   234,   354,    80,    81,    82,
      83,    12,    13,    14,   163,    15,    16,    17,   234,   167,
     387,   396,   234,   197,   429,   368,   367,    25,   369,   234,
     315,   452,   454,    29,   315,   157,   456,   158,    34,    35,
      36,   381,   162,    38,   122,   123,   124,   125,   126,   127,
     431,   234,   169,   128,   177,   392,   176,   129,   130,   131,
     132,   106,   107,   211,   212,   179,   180,   181,   192,   194,
     401,   402,   403,   195,    43,   197,   196,   224,   407,   228,
     227,    44,   409,   229,   230,   231,   232,   233,   248,    45,
     152,   254,   424,   261,   252,   253,   264,   266,   268,   269,
     270,   275,   277,   278,   282,   283,   284,   292,   296,   301,
     302,   320,   303,   321,   322,   326,   323,   340,   358,   360,
     361,   439,   362,   341,   441,   363,   353,   357,   364,   370,
     371,   359,   372,   383,   384,   385,   450,   388,   389,   390,
     391,   393,   398,   399,   453,   425,   455,   410,   408,   411,
     422,   462,   423,   426,   464,   427,   428,   432,   438,   433,
       4,     5,     6,     7,   466,   440,   467,     8,     9,    10,
      11,    12,    13,    14,   448,    15,    16,    17,    18,    19,
      20,    21,   449,   451,    22,    23,    24,    25,    26,    27,
      28,   461,   457,    29,    30,    31,    32,    33,    34,    35,
      36,    37,   463,    38,    39,     0,   458,   459,    40,    41,
     338,   400,   460,   465,    42,   468,   469,   178,   470,   471,
       4,     5,     6,     7,     0,   327,     0,    80,    81,    82,
      83,    12,    13,    14,    43,    15,    16,    17,     0,     0,
       0,    44,     0,     0,     0,     0,     0,    25,     0,    45,
       0,     0,     0,    29,   262,     0,     0,     0,    34,    35,
      36,     0,     0,    38,     4,     5,     6,     7,     0,     0,
       0,    80,    81,    82,    83,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,    43,     0,     0,    29,     0,     0,
       0,    44,    34,    35,    36,     0,     0,    38,     0,    45,
     280,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    80,    81,    82,    83,   287,    13,
      14,     0,    15,    16,    17,     0,     0,     0,    43,     0,
       0,     0,     0,     0,    25,    44,     0,     0,     0,     0,
      29,     0,     0,    45,   285,    34,    35,    36,     0,     0,
      38,     4,     5,     6,     7,     0,     0,     0,    80,    81,
      82,    83,   287,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    43,     0,     0,    29,     0,     0,     0,    44,    34,
      35,    36,     0,     0,    38,     0,    45,   288,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,    44,     0,     0,     4,     5,     6,     7,     0,
      45,   337,     8,     9,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,    20,    21,     0,     0,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,    38,    39,
       0,     0,     0,    40,    41,     0,     0,     0,     0,    42,
       4,     5,     6,     7,     0,     0,     0,    80,    81,    82,
      83,    12,    13,    14,     0,    15,    16,    17,     0,    43,
       0,     0,     0,     0,     0,     0,    44,    25,     0,     0,
       0,     0,     0,    29,    45,     0,     0,     0,    34,    35,
      36,     0,     0,    38,     0,     4,     5,     6,     7,     0,
       0,     0,    80,    81,    82,    83,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,    43,     0,     0,     0,    29,     0,
       0,    44,     0,    34,    35,    36,     0,    96,    38,    45,
       4,     5,     6,     7,     0,     0,     0,    80,    81,    82,
      83,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,    43,
       0,     0,     0,    29,     0,     0,    44,     0,    34,    35,
      36,     0,     0,    38,    84,     4,     5,     6,     7,     0,
       0,     0,    80,    81,    82,    83,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,    43,     0,     0,     0,    29,     0,
       0,    44,     0,    34,    35,    36,     0,     0,    38,    86,
       4,     5,     6,     7,     0,     0,     0,    80,    81,    82,
      83,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,    43,
       0,     0,     0,    29,     0,     0,    44,     0,    34,    35,
      36,     0,     0,    38,    45,     4,     5,     6,     7,     0,
       0,     0,    80,    81,    82,    83,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,    43,     0,     0,     0,    29,     0,
       0,    44,     0,    34,    35,    36,     0,     0,    38,   237,
       4,     5,     6,     7,     0,     0,     0,   115,    81,    82,
      83,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,    29,     0,     0,    44,     0,    34,    35,
      36,     0,     0,    38,   298,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,     0,     0,     0,
       0,    44,     0,     0,     0,     0,     0,     0,     0,    45
};

static const yytype_int16 yycheck[] =
{
       2,   275,    14,   226,    24,    10,    23,    14,   231,    14,
     233,    43,    44,    48,    49,    10,    10,    19,    20,    14,
      78,    23,    14,     0,    26,    27,    28,    10,    10,    10,
      78,    14,    14,    33,    36,    64,    38,    37,    96,    14,
      58,    59,    60,    45,    48,    49,    76,    78,    96,    14,
      14,    14,    17,    76,   277,    34,    35,    78,    50,    89,
      54,    55,    97,    61,    76,    96,    68,    96,    70,    92,
      72,    73,   295,    54,    55,    96,    96,    64,    90,    97,
      89,    94,    84,    92,    86,    50,    88,    89,    93,    96,
      90,    89,    76,    97,    92,    82,    83,    84,    93,   101,
     102,    76,   104,   105,   106,   107,   138,   139,   140,   141,
      93,    93,    76,    76,    92,    90,   133,   134,    97,    62,
      63,    76,    62,    63,    76,    92,    90,    90,   351,    64,
      10,    76,    76,   356,    14,    90,   410,   411,    90,    82,
      83,    84,    82,    83,    84,    90,    90,    82,    83,    84,
      10,    11,    12,   155,    75,   157,   158,   174,   175,    14,
     162,   163,    17,   165,    85,    86,    87,    10,    11,    12,
      14,    14,    96,   396,    10,    12,   178,    13,    14,    15,
      16,   183,    91,   185,    93,    12,     3,     4,     5,     6,
      71,    72,   192,    10,    11,    12,    13,    14,    15,    16,
      96,    18,    19,    20,    92,   120,   121,   122,   123,   124,
     125,   126,   127,    30,   129,   130,     7,     8,     9,    36,
      82,    83,    84,   225,    41,    42,    43,    92,    10,    46,
      14,    13,   234,    15,    16,   237,    29,    91,   240,    93,
     242,    14,   244,   135,   136,   137,    80,    81,   250,   251,
      90,    91,   252,   253,   254,    14,    91,    90,    93,   259,
      77,    91,    91,    93,    93,    90,    91,    84,   268,   269,
      91,    91,    93,    93,    14,    92,    91,    89,    93,    96,
     282,   283,   284,    91,    91,    93,    93,    91,    14,    93,
       3,     4,     5,     6,    90,    91,   298,    10,    11,    12,
      13,    14,    15,    16,    94,    18,    19,    20,    91,    90,
      93,    91,    91,    93,    93,   317,   316,    30,   318,    91,
      91,    93,    93,    36,    91,    76,    93,    76,    41,    42,
      43,   331,    76,    46,    65,    66,    67,    68,    69,    70,
      90,    91,    90,    74,    56,   347,    92,    78,    79,    80,
      81,    35,    36,   131,   132,    90,    90,    96,    45,    96,
     360,   361,   362,    14,    77,    93,    96,    90,   368,    79,
      95,    84,   372,    97,    90,    92,    14,    92,    14,    92,
      93,    10,   382,    97,    93,    93,    14,    14,    93,    93,
      97,    92,    92,    14,    76,    76,    76,    90,    90,    95,
      79,    79,    97,    97,    79,    14,    97,    90,    93,    25,
      10,   413,    10,    90,   416,    10,    90,    90,    10,    90,
      90,    93,    25,    14,    14,    14,   428,    90,    90,    90,
      90,    14,    90,    90,   434,     7,   436,    92,    97,    92,
      90,   443,    90,     7,   446,     7,    61,    10,    14,    10,
       3,     4,     5,     6,   454,    14,   456,    10,    11,    12,
      13,    14,    15,    16,    90,    18,    19,    20,    21,    22,
      23,    24,    90,    90,    27,    28,    29,    30,    31,    32,
      33,    14,    90,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    14,    46,    47,    -1,    90,    90,    51,    52,
     277,   360,    90,    90,    57,    90,    90,    93,    90,    90,
       3,     4,     5,     6,    -1,   273,    -1,    10,    11,    12,
      13,    14,    15,    16,    77,    18,    19,    20,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    30,    -1,    92,
      -1,    -1,    -1,    36,    97,    -1,    -1,    -1,    41,    42,
      43,    -1,    -1,    46,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    77,    -1,    -1,    36,    -1,    -1,
      -1,    84,    41,    42,    43,    -1,    -1,    46,    -1,    92,
      93,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    30,    84,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    92,    93,    41,    42,    43,    -1,    -1,
      46,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    77,    -1,    -1,    36,    -1,    -1,    -1,    84,    41,
      42,    43,    -1,    -1,    46,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,     3,     4,     5,     6,    -1,
      92,    93,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
      -1,    -1,    -1,    51,    52,    -1,    -1,    -1,    -1,    57,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    30,    -1,    -1,
      -1,    -1,    -1,    36,    92,    -1,    -1,    -1,    41,    42,
      43,    -1,    -1,    46,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    77,    -1,    -1,    -1,    36,    -1,
      -1,    84,    -1,    41,    42,    43,    -1,    90,    46,    92,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    77,
      -1,    -1,    -1,    36,    -1,    -1,    84,    -1,    41,    42,
      43,    -1,    -1,    46,    92,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    77,    -1,    -1,    -1,    36,    -1,
      -1,    84,    -1,    41,    42,    43,    -1,    -1,    46,    92,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    77,
      -1,    -1,    -1,    36,    -1,    -1,    84,    -1,    41,    42,
      43,    -1,    -1,    46,    92,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    77,    -1,    -1,    -1,    36,    -1,
      -1,    84,    -1,    41,    42,    43,    -1,    -1,    46,    92,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    36,    -1,    -1,    84,    -1,    41,    42,
      43,    -1,    -1,    46,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    99,   100,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    27,    28,    29,    30,    31,    32,    33,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      51,    52,    57,    77,    84,    92,   101,   102,   103,   104,
     105,   106,   107,   108,   110,   111,   112,   115,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   131,    76,    89,
      94,    78,    96,    76,    89,    92,    10,    11,    12,    14,
      10,    11,    12,    13,    92,   121,    92,   121,    92,    92,
     121,   125,    14,    96,   119,   121,    90,   121,   121,    12,
      12,    92,    92,   119,    96,   120,   120,   120,   121,   119,
     121,    14,    14,    29,    14,    10,   127,   127,   121,    90,
      71,    72,    65,    66,    67,    68,    69,    70,    74,    78,
      79,    80,    81,    62,    63,    82,    83,    84,    75,    85,
      86,    87,    89,   121,    14,   121,    14,   121,   121,    14,
      17,    50,    93,   121,   130,    76,    90,    76,    76,    10,
      11,    12,    76,    94,    78,    96,   121,    90,   121,    90,
     121,   121,    64,   119,    62,    63,    92,    56,   100,    90,
      90,    96,    78,    96,    78,    96,   121,   121,   121,   121,
     121,   121,    45,   113,    96,    14,    96,    93,   123,   123,
     123,   123,   123,   123,   123,   123,     7,     8,     9,   123,
     123,   124,   124,   125,   125,   126,   126,   126,   127,   127,
     127,   127,    14,    17,    90,    76,    92,    95,    79,    97,
      90,    92,    14,    92,    91,    93,   121,    92,   121,   121,
      76,    90,    76,    90,    76,    90,   121,   121,    14,   121,
      91,    91,    93,    93,    10,   125,   125,    10,    14,    93,
     118,    97,    97,   109,    14,   121,    14,   121,    93,    93,
      97,   119,    14,    96,   114,    92,   116,    92,    14,   121,
      93,   130,    76,    76,    76,    93,   130,    14,    93,   117,
     130,   121,    90,   121,    90,    91,    90,   121,    92,   121,
     121,    95,    79,    97,   121,   121,   119,   119,   119,    64,
      64,    10,    54,    55,   119,    91,    93,    34,    35,    97,
      79,    97,    79,    97,   119,   119,    14,   114,    48,    49,
      97,    93,   118,    58,    59,    60,    97,    93,   117,   130,
      90,    90,    93,   121,   121,   121,    93,    61,    91,    93,
      93,    91,   130,    90,   121,    90,    91,    90,    93,    93,
      25,    10,    10,    10,    10,    10,    14,   119,   121,   119,
      90,    90,    25,    97,    14,    50,    10,    13,    14,    15,
      16,   119,    93,    14,    14,    14,    93,    93,    90,    90,
      90,    90,   121,    14,   130,    90,    91,   130,    90,    90,
     102,   119,   119,   119,    10,    54,    55,   119,    97,   119,
      92,    92,    14,    76,    90,    14,    76,    90,    10,    13,
      15,    16,    90,    90,   119,     7,     7,     7,    61,    93,
     130,    90,    10,    10,    93,   118,    93,   118,    14,   121,
      14,   121,    14,    76,    90,    14,    76,    90,    90,    90,
     121,    90,    93,   119,    93,   119,    93,    90,    90,    90,
      90,    14,   121,    14,   121,    90,   119,   119,    90,    90,
      90,    90
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,   100,   100,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     102,   102,   102,   103,   104,   104,   105,   106,   107,   107,
     107,   107,   108,   108,   109,   109,   110,   110,   110,   110,
     111,   112,   113,   113,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   116,   116,   116,   116,
     117,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     119,   119,   120,   121,   121,   121,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     123,   123,   123,   124,   124,   124,   125,   125,   125,   125,
     126,   126,   126,   126,   126,   127,   127,   127,   128,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   130,   130,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     7,     9,
       5,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       7,     3,     7,     3,     3,     2,     4,     6,     4,     7,
       6,     7,     7,     7,     6,     6,     2,     1,     1,     1,
       1,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       5,     7,     7,     5,     5,     7,     5,     2,     7,     7,
       5,     3,     5,     7,     0,     4,     6,     5,     7,     6,
       6,     5,     0,     3,     0,     4,     4,     4,     5,     5,
       4,     5,     5,     6,     6,     7,     7,     6,     6,     7,
       7,     7,     6,     7,     6,     5,     0,     4,     4,     4,
       3,     5,     1,     2,     3,     3,     3,     4,     5,     5,
       3,     3,     3,     3,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     3,     3,     1,     2,     2,     1,     1,     4,
       3,     6,     6,     5,     6,     5,     4,     4,     6,     6,
       5,     3,     2,     3,     3,     2,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     5,     5,     1,     1,     1,     1,     3
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
#line 221 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1798 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 231 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1804 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 232 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1810 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 236 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1816 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 238 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1822 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 240 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1828 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 242 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1836 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 246 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1844 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 250 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1850 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 252 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1856 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 254 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1862 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 256 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1870 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 260 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1878 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 264 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1884 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 266 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1890 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 268 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1896 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 270 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1902 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY expr ';'  */
#line 272 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1908 "raku.tab.c"
    break;

  case 20: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 274 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1914 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT expr ';'  */
#line 276 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1920 "raku.tab.c"
    break;

  case 22: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 278 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1926 "raku.tab.c"
    break;

  case 23: /* stmt: KW_TAKE expr ';'  */
#line 280 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1932 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN expr ';'  */
#line 282 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1938 "raku.tab.c"
    break;

  case 25: /* stmt: KW_RETURN ';'  */
#line 284 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1944 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 286 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1950 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 288 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1959 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 293 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 1967 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 297 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 1978 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 304 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 1987 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 309 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1994 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 312 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2001 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 315 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2008 "raku.tab.c"
    break;

  case 34: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 318 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2015 "raku.tab.c"
    break;

  case 35: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 321 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2022 "raku.tab.c"
    break;

  case 36: /* stmt: expr ';'  */
#line 323 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2028 "raku.tab.c"
    break;

  case 37: /* stmt: if_stmt  */
#line 324 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2034 "raku.tab.c"
    break;

  case 38: /* stmt: while_stmt  */
#line 325 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2040 "raku.tab.c"
    break;

  case 39: /* stmt: for_stmt  */
#line 326 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2046 "raku.tab.c"
    break;

  case 40: /* stmt: given_stmt  */
#line 327 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2052 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TRY block  */
#line 329 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2058 "raku.tab.c"
    break;

  case 42: /* stmt: KW_TRY block KW_CATCH block  */
#line 331 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2064 "raku.tab.c"
    break;

  case 43: /* stmt: unless_stmt  */
#line 332 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2070 "raku.tab.c"
    break;

  case 44: /* stmt: until_stmt  */
#line 333 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2076 "raku.tab.c"
    break;

  case 45: /* stmt: repeat_stmt  */
#line 334 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2082 "raku.tab.c"
    break;

  case 46: /* stmt: sub_decl  */
#line 335 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2088 "raku.tab.c"
    break;

  case 47: /* stmt: class_decl  */
#line 336 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2094 "raku.tab.c"
    break;

  case 48: /* stmt: role_decl  */
#line 337 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2100 "raku.tab.c"
    break;

  case 49: /* stmt: grammar_decl  */
#line 338 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2106 "raku.tab.c"
    break;

  case 50: /* if_stmt: KW_IF '(' expr ')' block  */
#line 342 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2112 "raku.tab.c"
    break;

  case 51: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 344 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2118 "raku.tab.c"
    break;

  case 52: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 346 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2124 "raku.tab.c"
    break;

  case 53: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 350 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2130 "raku.tab.c"
    break;

  case 54: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 354 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2136 "raku.tab.c"
    break;

  case 55: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 356 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2142 "raku.tab.c"
    break;

  case 56: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 360 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2148 "raku.tab.c"
    break;

  case 57: /* repeat_stmt: KW_REPEAT block  */
#line 364 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2154 "raku.tab.c"
    break;

  case 58: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 368 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2164 "raku.tab.c"
    break;

  case 59: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 374 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2174 "raku.tab.c"
    break;

  case 60: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 380 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2183 "raku.tab.c"
    break;

  case 61: /* for_stmt: KW_FOR expr block  */
#line 385 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2190 "raku.tab.c"
    break;

  case 62: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 390 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2202 "raku.tab.c"
    break;

  case 63: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 398 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2215 "raku.tab.c"
    break;

  case 64: /* when_list: %empty  */
#line 408 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2221 "raku.tab.c"
    break;

  case 65: /* when_list: when_list KW_WHEN expr block  */
#line 410 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2229 "raku.tab.c"
    break;

  case 66: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 416 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2241 "raku.tab.c"
    break;

  case 67: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 424 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2251 "raku.tab.c"
    break;

  case 68: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 430 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2264 "raku.tab.c"
    break;

  case 69: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 439 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2275 "raku.tab.c"
    break;

  case 70: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 448 "raku.y"
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
#line 2293 "raku.tab.c"
    break;

  case 71: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 464 "raku.y"
        {
            const char *rname = intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *rd = ast_node_new(TT_ROLE_DECL);
            ast_push(rd, leaf_sval(TT_VAR, rname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(rd, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = rd;
        }
#line 2310 "raku.tab.c"
    break;

  case 72: /* is_clauses: %empty  */
#line 478 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2316 "raku.tab.c"
    break;

  case 73: /* is_clauses: is_clauses IDENT IDENT  */
#line 480 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)malloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)malloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); free((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 2332 "raku.tab.c"
    break;

  case 74: /* class_body_list: %empty  */
#line 493 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2338 "raku.tab.c"
    break;

  case 75: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 495 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2345 "raku.tab.c"
    break;

  case 76: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 498 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2352 "raku.tab.c"
    break;

  case 77: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 501 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2359 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 504 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2366 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 507 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2373 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 510 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2380 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 513 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2387 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 516 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2394 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 519 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2401 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 522 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2408 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 525 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2415 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 528 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2422 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 531 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2433 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 538 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2444 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 545 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2455 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 552 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2466 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 559 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2480 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 569 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2492 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 577 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2505 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 586 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2516 "raku.tab.c"
    break;

  case 95: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 595 "raku.y"
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
#line 2533 "raku.tab.c"
    break;

  case 96: /* grammar_body_list: %empty  */
#line 609 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2539 "raku.tab.c"
    break;

  case 97: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 611 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2548 "raku.tab.c"
    break;

  case 98: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 616 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2557 "raku.tab.c"
    break;

  case 99: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 621 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2566 "raku.tab.c"
    break;

  case 100: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 628 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2574 "raku.tab.c"
    break;

  case 101: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 632 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2582 "raku.tab.c"
    break;

  case 102: /* param_list: VAR_SCALAR  */
#line 637 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2588 "raku.tab.c"
    break;

  case 103: /* param_list: IDENT VAR_SCALAR  */
#line 638 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2594 "raku.tab.c"
    break;

  case 104: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 639 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2600 "raku.tab.c"
    break;

  case 105: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 640 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2606 "raku.tab.c"
    break;

  case 106: /* param_list: param_list ',' VAR_SCALAR  */
#line 641 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2612 "raku.tab.c"
    break;

  case 107: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 642 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2618 "raku.tab.c"
    break;

  case 108: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 643 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2624 "raku.tab.c"
    break;

  case 109: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 644 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2630 "raku.tab.c"
    break;

  case 110: /* block: '{' stmt_list '}'  */
#line 647 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2636 "raku.tab.c"
    break;

  case 111: /* block: '{' YADA '}'  */
#line 648 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2642 "raku.tab.c"
    break;

  case 112: /* closure: '{' expr '}'  */
#line 651 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2648 "raku.tab.c"
    break;

  case 113: /* expr: VAR_SCALAR '=' expr  */
#line 654 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2654 "raku.tab.c"
    break;

  case 114: /* expr: KW_GATHER block  */
#line 655 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2664 "raku.tab.c"
    break;

  case 115: /* expr: cmp_expr  */
#line 660 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2670 "raku.tab.c"
    break;

  case 116: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 663 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2676 "raku.tab.c"
    break;

  case 117: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 664 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2682 "raku.tab.c"
    break;

  case 118: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 665 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2688 "raku.tab.c"
    break;

  case 119: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 666 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2694 "raku.tab.c"
    break;

  case 120: /* cmp_expr: jct_expr '<' jct_expr  */
#line 667 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2700 "raku.tab.c"
    break;

  case 121: /* cmp_expr: jct_expr '>' jct_expr  */
#line 668 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2706 "raku.tab.c"
    break;

  case 122: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 669 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2712 "raku.tab.c"
    break;

  case 123: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 670 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2718 "raku.tab.c"
    break;

  case 124: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 671 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2724 "raku.tab.c"
    break;

  case 125: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 672 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2730 "raku.tab.c"
    break;

  case 126: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 674 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2740 "raku.tab.c"
    break;

  case 127: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 680 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2750 "raku.tab.c"
    break;

  case 128: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 686 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2760 "raku.tab.c"
    break;

  case 129: /* cmp_expr: jct_expr  */
#line 691 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2766 "raku.tab.c"
    break;

  case 130: /* jct_expr: jct_expr '|' range_expr  */
#line 694 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2772 "raku.tab.c"
    break;

  case 131: /* jct_expr: jct_expr '&' range_expr  */
#line 695 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2778 "raku.tab.c"
    break;

  case 132: /* jct_expr: range_expr  */
#line 696 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2784 "raku.tab.c"
    break;

  case 133: /* range_expr: add_expr OP_RANGE add_expr  */
#line 699 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2790 "raku.tab.c"
    break;

  case 134: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 700 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2796 "raku.tab.c"
    break;

  case 135: /* range_expr: add_expr  */
#line 701 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2802 "raku.tab.c"
    break;

  case 136: /* add_expr: add_expr '+' mul_expr  */
#line 704 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2808 "raku.tab.c"
    break;

  case 137: /* add_expr: add_expr '-' mul_expr  */
#line 705 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2814 "raku.tab.c"
    break;

  case 138: /* add_expr: add_expr '~' mul_expr  */
#line 706 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2820 "raku.tab.c"
    break;

  case 139: /* add_expr: mul_expr  */
#line 707 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2826 "raku.tab.c"
    break;

  case 140: /* mul_expr: mul_expr '*' unary_expr  */
#line 710 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2832 "raku.tab.c"
    break;

  case 141: /* mul_expr: mul_expr '/' unary_expr  */
#line 711 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2838 "raku.tab.c"
    break;

  case 142: /* mul_expr: mul_expr '%' unary_expr  */
#line 712 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2844 "raku.tab.c"
    break;

  case 143: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 713 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2850 "raku.tab.c"
    break;

  case 144: /* mul_expr: unary_expr  */
#line 714 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2856 "raku.tab.c"
    break;

  case 145: /* unary_expr: '-' unary_expr  */
#line 717 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2862 "raku.tab.c"
    break;

  case 146: /* unary_expr: '!' unary_expr  */
#line 718 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2868 "raku.tab.c"
    break;

  case 147: /* unary_expr: postfix_expr  */
#line 719 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2874 "raku.tab.c"
    break;

  case 148: /* postfix_expr: call_expr  */
#line 721 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2880 "raku.tab.c"
    break;

  case 149: /* call_expr: IDENT '(' arg_list ')'  */
#line 724 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2889 "raku.tab.c"
    break;

  case 150: /* call_expr: IDENT '(' ')'  */
#line 728 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2895 "raku.tab.c"
    break;

  case 151: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 730 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2905 "raku.tab.c"
    break;

  case 152: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 736 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2915 "raku.tab.c"
    break;

  case 153: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 742 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 2923 "raku.tab.c"
    break;

  case 154: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 746 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2934 "raku.tab.c"
    break;

  case 155: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 753 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2943 "raku.tab.c"
    break;

  case 156: /* call_expr: IDENT '.' CARET IDENT  */
#line 758 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2953 "raku.tab.c"
    break;

  case 157: /* call_expr: atom '.' CARET IDENT  */
#line 764 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 2963 "raku.tab.c"
    break;

  case 158: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 770 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2974 "raku.tab.c"
    break;

  case 159: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 777 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2985 "raku.tab.c"
    break;

  case 160: /* call_expr: atom '.' IDENT '(' ')'  */
#line 784 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 2994 "raku.tab.c"
    break;

  case 161: /* call_expr: atom '.' IDENT  */
#line 789 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3003 "raku.tab.c"
    break;

  case 162: /* call_expr: KW_DIE expr  */
#line 794 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3009 "raku.tab.c"
    break;

  case 163: /* call_expr: KW_MAP closure expr  */
#line 796 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3015 "raku.tab.c"
    break;

  case 164: /* call_expr: KW_GREP closure expr  */
#line 798 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3021 "raku.tab.c"
    break;

  case 165: /* call_expr: KW_SORT expr  */
#line 800 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3027 "raku.tab.c"
    break;

  case 166: /* call_expr: KW_SORT closure expr  */
#line 802 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3033 "raku.tab.c"
    break;

  case 167: /* call_expr: atom  */
#line 803 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3039 "raku.tab.c"
    break;

  case 168: /* arg_list: expr  */
#line 806 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3045 "raku.tab.c"
    break;

  case 169: /* arg_list: arg_list ',' expr  */
#line 807 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3051 "raku.tab.c"
    break;

  case 170: /* atom: LIT_INT  */
#line 810 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3057 "raku.tab.c"
    break;

  case 171: /* atom: LIT_FLOAT  */
#line 811 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3063 "raku.tab.c"
    break;

  case 172: /* atom: LIT_STR  */
#line 812 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3069 "raku.tab.c"
    break;

  case 173: /* atom: LIT_INTERP_STR  */
#line 813 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3075 "raku.tab.c"
    break;

  case 174: /* atom: VAR_SCALAR  */
#line 814 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3081 "raku.tab.c"
    break;

  case 175: /* atom: VAR_ARRAY  */
#line 815 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3087 "raku.tab.c"
    break;

  case 176: /* atom: VAR_HASH  */
#line 816 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3093 "raku.tab.c"
    break;

  case 177: /* atom: VAR_CAPTURE  */
#line 818 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3101 "raku.tab.c"
    break;

  case 178: /* atom: VAR_FH  */
#line 822 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3109 "raku.tab.c"
    break;

  case 179: /* atom: VAR_NAMED_CAPTURE  */
#line 826 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3116 "raku.tab.c"
    break;

  case 180: /* atom: VAR_ARRAY '[' expr ']'  */
#line 829 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3122 "raku.tab.c"
    break;

  case 181: /* atom: VAR_HASH '<' IDENT '>'  */
#line 831 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3128 "raku.tab.c"
    break;

  case 182: /* atom: VAR_HASH '{' expr '}'  */
#line 833 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3134 "raku.tab.c"
    break;

  case 183: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 835 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3140 "raku.tab.c"
    break;

  case 184: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 837 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3146 "raku.tab.c"
    break;

  case 185: /* atom: IDENT  */
#line 838 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3152 "raku.tab.c"
    break;

  case 186: /* atom: VAR_TWIGIL  */
#line 840 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3160 "raku.tab.c"
    break;

  case 187: /* atom: VAR_ARRAY_TWIGIL  */
#line 844 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3168 "raku.tab.c"
    break;

  case 188: /* atom: VAR_HASH_TWIGIL  */
#line 848 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3176 "raku.tab.c"
    break;

  case 189: /* atom: '(' expr ')'  */
#line 851 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3182 "raku.tab.c"
    break;


#line 3186 "raku.tab.c"

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

#line 853 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
