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
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}/*--------------------------------------------------------------------------------------------------------------------*/
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

#line 232 "raku.tab.c"

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
  YYSYMBOL_OP_NAME = 54,                   /* OP_NAME  */
  YYSYMBOL_OP_COLON_D = 55,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 56,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 57,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 58,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 59,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 60,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 61,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 62,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 63,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 64,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 65,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 66,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 67,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 68,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 69,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 70,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 71,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 72,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 73,                     /* OP_OR  */
  YYSYMBOL_OP_BIND = 74,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 75,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 76,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 77,                    /* OP_DIV  */
  YYSYMBOL_78_ = 78,                       /* '='  */
  YYSYMBOL_79_ = 79,                       /* '!'  */
  YYSYMBOL_80_ = 80,                       /* '<'  */
  YYSYMBOL_81_ = 81,                       /* '>'  */
  YYSYMBOL_82_ = 82,                       /* '|'  */
  YYSYMBOL_83_ = 83,                       /* '&'  */
  YYSYMBOL_84_ = 84,                       /* '~'  */
  YYSYMBOL_85_ = 85,                       /* '+'  */
  YYSYMBOL_86_ = 86,                       /* '-'  */
  YYSYMBOL_87_ = 87,                       /* '*'  */
  YYSYMBOL_88_ = 88,                       /* '/'  */
  YYSYMBOL_89_ = 89,                       /* '%'  */
  YYSYMBOL_UMINUS = 90,                    /* UMINUS  */
  YYSYMBOL_91_ = 91,                       /* '.'  */
  YYSYMBOL_92_ = 92,                       /* ';'  */
  YYSYMBOL_93_ = 93,                       /* ','  */
  YYSYMBOL_94_ = 94,                       /* '('  */
  YYSYMBOL_95_ = 95,                       /* ')'  */
  YYSYMBOL_96_ = 96,                       /* '['  */
  YYSYMBOL_97_ = 97,                       /* ']'  */
  YYSYMBOL_98_ = 98,                       /* '{'  */
  YYSYMBOL_99_ = 99,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 100,                 /* $accept  */
  YYSYMBOL_program = 101,                  /* program  */
  YYSYMBOL_stmt_list = 102,                /* stmt_list  */
  YYSYMBOL_stmt = 103,                     /* stmt  */
  YYSYMBOL_if_stmt = 104,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 105,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 106,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 107,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 108,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 109,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 110,               /* given_stmt  */
  YYSYMBOL_when_list = 111,                /* when_list  */
  YYSYMBOL_sub_decl = 112,                 /* sub_decl  */
  YYSYMBOL_class_decl = 113,               /* class_decl  */
  YYSYMBOL_role_decl = 114,                /* role_decl  */
  YYSYMBOL_is_clauses = 115,               /* is_clauses  */
  YYSYMBOL_class_body_list = 116,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 117,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 118,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 119,           /* named_arg_list  */
  YYSYMBOL_param_list = 120,               /* param_list  */
  YYSYMBOL_block = 121,                    /* block  */
  YYSYMBOL_closure = 122,                  /* closure  */
  YYSYMBOL_expr = 123,                     /* expr  */
  YYSYMBOL_cmp_expr = 124,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 125,                 /* jct_expr  */
  YYSYMBOL_range_expr = 126,               /* range_expr  */
  YYSYMBOL_add_expr = 127,                 /* add_expr  */
  YYSYMBOL_mul_expr = 128,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 129,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 130,             /* postfix_expr  */
  YYSYMBOL_call_expr = 131,                /* call_expr  */
  YYSYMBOL_arg_list = 132,                 /* arg_list  */
  YYSYMBOL_atom = 133                      /* atom  */
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
#define YYLAST   1238

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  195
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  495

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   333


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
       2,     2,     2,    79,     2,     2,     2,    89,    83,     2,
      94,    95,    87,    85,    93,    86,    91,    88,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    92,
      80,    78,    81,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    96,     2,    97,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    98,    82,    99,    84,     2,     2,     2,
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
      75,    76,    77,    90
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   225,   225,   236,   237,   240,   242,   244,   246,   250,
     254,   256,   258,   260,   264,   268,   270,   272,   274,   276,
     278,   280,   282,   284,   286,   288,   290,   292,   297,   301,
     305,   310,   314,   321,   326,   329,   332,   335,   338,   341,
     342,   343,   344,   345,   346,   348,   350,   351,   352,   353,
     354,   355,   356,   359,   361,   363,   367,   371,   373,   377,
     381,   385,   391,   397,   402,   407,   415,   426,   427,   433,
     441,   447,   456,   463,   474,   490,   505,   506,   520,   521,
     524,   527,   530,   533,   536,   539,   542,   545,   548,   551,
     554,   557,   564,   571,   578,   585,   595,   603,   612,   619,
     629,   639,   654,   655,   660,   665,   672,   676,   682,   683,
     684,   685,   686,   687,   688,   689,   692,   693,   696,   699,
     700,   705,   708,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   724,   730,   736,   739,   740,   741,   744,
     745,   746,   749,   750,   751,   752,   755,   756,   757,   758,
     759,   762,   763,   764,   766,   768,   773,   774,   780,   786,
     790,   797,   802,   808,   814,   821,   828,   833,   838,   840,
     842,   844,   846,   848,   851,   852,   855,   856,   857,   858,
     859,   860,   861,   862,   866,   870,   873,   875,   877,   879,
     881,   883,   884,   888,   892,   896
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
  "KW_HAS", "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND",
  "OP_OR", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_DIV", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "while_stmt",
  "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt", "given_stmt",
  "when_list", "sub_decl", "class_decl", "role_decl", "is_clauses",
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

#define YYPACT_NINF (-280)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -280,    51,   856,  -280,  -280,  -280,  -280,  -280,   -31,   -64,
     -32,   -21,    93,  -280,  -280,  -280,  -280,  -280,   251,   960,
    1006,   -24,    -8,  1052,   117,    46,  1052,   913,  1052,   147,
     157,    83,    99,    46,    84,    84,   193,    46,  1052,   201,
     227,   209,   246,   296,   296,  1052,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,   186,    23,
     150,  -280,    87,   102,  -280,  -280,  -280,   194,   279,  1052,
     299,  1052,   315,  1052,  1052,    32,   538,    44,   257,   263,
     190,   273,   275,     1,  -280,  1052,   278,  1052,   286,  1052,
    1052,   -43,    90,   285,   298,  -280,   289,  -280,   291,   287,
      47,    50,  1052,  1052,  -280,  1052,  1052,  1052,  1052,  -280,
     341,  -280,  -280,   290,     5,   293,  -280,  -280,  -280,   292,
    -280,   296,   296,   296,   296,   296,   296,   296,   296,   259,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   178,    45,   297,   -40,   295,   312,   300,
     302,   303,   384,   306,  -280,  -280,   129,  1052,  -280,  1098,
    1052,    78,    88,    89,  1052,  1052,   387,  1052,   132,  -280,
     149,  -280,   307,   308,   394,  -280,   296,   296,    -6,   309,
     476,  -280,  -280,  -280,   391,  1052,   392,  1052,   314,   316,
     311,  -280,  -280,  -280,    46,    -4,  -280,   313,   318,  -280,
    -280,   173,   173,   173,   173,   173,   173,   173,   173,  -280,
    -280,  -280,   173,   173,  -280,  -280,   185,   185,   102,   102,
     102,  -280,  -280,  -280,  -280,   319,   400,  -280,   584,  -280,
    1052,   631,   337,   338,   339,  -280,   677,  -280,   724,  1052,
    -280,   326,  1052,   229,   327,  1052,  -280,  1144,  -280,  1052,
    -280,  -280,   323,   340,   324,  1052,  1052,    46,    46,    46,
     -23,    56,  -280,    20,    46,   155,  -280,  -280,    18,   343,
     328,   344,   329,    46,    46,  -280,  -280,   408,  -280,    16,
      -5,   154,   -20,   770,  -280,   342,   187,   345,   346,   188,
    1052,  1052,  1052,  -280,   191,    58,  -280,   199,   202,  -280,
    -280,   203,  -280,  1052,  -280,   347,  1052,   270,   348,  -280,
    -280,  -280,   335,   336,   407,  -280,  -280,   423,   425,  -280,
     426,   431,  -280,   200,    46,  1052,    46,  -280,  -280,  -280,
     350,   352,   420,  -280,  -280,    25,    42,   119,   398,  -280,
      46,   210,   224,   433,   434,   435,  -280,  -280,   225,   232,
    -280,   358,  -280,  -280,   360,   362,   363,   364,  -280,  1052,
     443,  -280,  -280,  1052,   272,  -280,   235,  -280,  1052,  -280,
     367,   368,     4,    46,    46,  -280,  -280,  -280,    70,  -280,
      46,   365,  -280,  -280,    46,  -280,   369,   371,     6,    15,
     236,   370,   374,   453,  -280,    46,    46,   461,   462,   463,
    -280,  -280,  -280,  -280,  -280,  -280,  -280,  -280,   409,   238,
    -280,  1052,   274,  -280,  -280,  -280,  -280,  -280,  -280,  -280,
     465,   466,  -280,  -280,  -280,     2,    13,   459,  1052,  -280,
     464,  1052,  -280,    31,    36,   385,   393,  -280,  -280,   390,
    -280,  -280,  -280,  -280,  -280,  1052,   410,   241,  -280,  -280,
    -280,    46,   261,    46,   265,   418,   419,   429,   432,   487,
    1052,  -280,   511,  1052,  -280,  -280,  -280,    21,  -280,  -280,
     437,  -280,    46,  -280,    46,  -280,  -280,  -280,  -280,   438,
     439,   440,   441,    46,   266,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,    46,  -280
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   176,   177,   178,   179,   180,   181,
     182,   192,   191,   193,   194,   183,   184,   185,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,    40,    41,    46,
      47,    48,    42,    43,    49,    50,    51,    52,     0,   121,
     135,   138,   141,   145,   150,   153,   154,   173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,   192,     0,     0,     0,     0,     0,
       0,     0,   141,     0,     3,   120,     0,    25,     0,     0,
       0,     0,     0,     0,    60,     0,     0,     0,     0,   171,
      44,   168,    76,     0,     0,     0,   180,   152,   151,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,   174,     0,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,    21,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    23,    24,    67,     0,     0,     0,     0,     0,     0,
       0,   169,   170,   172,     0,     0,    78,     0,     0,   102,
     195,   122,   123,   124,   125,   128,   129,   130,   131,   132,
     133,   134,   126,   127,   136,   137,   139,   140,   144,   142,
     143,   149,   146,   147,   148,   167,     0,    29,     0,    26,
       0,     0,   186,   187,   188,    31,     0,   162,     0,     0,
     155,     0,     0,     0,     0,     0,    16,     0,    17,     0,
      18,   119,     0,     0,     0,     0,     0,     0,     0,     0,
     139,   140,   108,     0,     0,     0,   117,   116,     0,     0,
       0,     0,     0,     0,     0,   118,    45,     0,    78,     0,
       0,     0,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,     0,   161,     0,   191,   159,     0,     0,   175,
       5,     0,     7,     0,    10,     0,     0,     0,     0,   186,
     187,   188,     0,     0,    53,    56,    63,     0,     0,   109,
       0,     0,    70,     0,     0,     0,     0,    65,   189,   190,
       0,     0,    57,    59,    77,     0,     0,     0,     0,    75,
       0,     0,     0,     0,     0,     0,   101,   166,     0,     0,
      28,     0,    30,    33,     0,     0,     0,     0,   160,     0,
       0,   157,   158,     0,     0,    11,     0,    12,     0,    15,
       0,     0,     0,     0,     0,   110,   111,   112,     0,    69,
       0,     0,    37,    38,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
     165,   164,    27,    32,    34,    35,    36,   106,     0,     0,
       8,     0,     0,    20,    22,    55,    54,    61,    62,   113,
       0,     0,    68,    66,    58,     0,     0,     0,     0,    84,
       0,     0,    79,     0,     0,     0,     0,    80,    81,     0,
      71,    73,   103,   104,   105,     0,     0,     0,    13,   114,
     115,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,    85,    82,    83,     0,   107,     9,
       0,    96,     0,    98,     0,    92,    88,    91,    87,     0,
       0,     0,     0,     0,     0,    14,    95,    97,    94,    90,
      93,    89,   100,     0,    99
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -280,  -280,   442,  -280,   163,  -280,  -280,  -280,  -280,  -280,
    -280,  -280,  -280,  -280,  -280,  -280,   248,  -280,  -280,   254,
    -279,     0,   333,    -2,  -280,   222,   244,   -16,   139,   -29,
    -280,  -280,  -225,  -280
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    46,    47,    48,    49,    50,    51,    52,
      53,   268,    54,    55,    56,   195,   279,    57,   282,   297,
     265,    95,   106,   155,    59,    60,    61,    62,    63,    64,
      65,    66,   156,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   341,   342,   286,   262,   262,   289,    92,   263,   263,
     277,   294,   262,   298,   117,   118,   263,    86,    88,   197,
     427,    91,   174,   262,    96,    98,    99,   263,    21,   430,
     319,   262,    71,   104,   109,   263,   111,   110,   230,   343,
     344,   345,   317,   119,    68,   459,   151,    69,    72,   152,
     462,     3,   325,   326,   231,    94,   386,    74,   349,   198,
      70,   136,   137,   138,   336,   337,    73,   145,   338,   147,
      89,   149,   150,   336,   337,   320,   321,   338,   364,   346,
     419,   166,   153,   168,   428,   170,    90,   172,   173,   264,
     340,   175,   387,   431,   278,   121,   122,   451,   429,   167,
     188,   189,    94,   190,   191,   192,   193,   432,   453,   460,
     221,   222,   223,   224,   463,   339,   483,   327,   216,   217,
     359,   318,   157,   461,   385,   420,   421,   184,   464,   388,
     186,    93,   389,   390,   391,   392,   158,   227,   409,   228,
     136,   137,   138,   412,    94,   185,   452,   454,   187,    75,
     134,   135,    76,   176,   177,   241,   245,   243,   244,   100,
     260,   261,   251,   252,   262,   254,   247,   249,   263,   101,
     246,   136,   137,   138,   136,   137,   138,   102,    58,   139,
     248,   250,   105,   270,    75,   272,   447,    76,   484,   140,
     141,   142,   225,   103,   276,   226,     4,     5,     6,     7,
     161,   162,   163,    81,    82,    83,    84,    12,    13,    14,
     377,    15,    16,    17,   378,   112,   123,   124,   125,   126,
     127,   128,   239,    25,   240,   255,   129,   200,   287,    29,
     130,   131,   132,   133,    34,    35,    36,   299,   114,    38,
     301,   113,   256,   305,   200,   307,   433,   308,   323,   434,
     324,   435,   436,   312,   313,   132,   133,   314,   315,   316,
     115,    77,    78,    79,   322,    80,   209,   210,   211,   136,
     137,   138,    43,   332,   333,   218,   219,   220,   120,    44,
     239,   239,   351,   354,   239,   143,   358,    45,   355,   356,
     357,   105,   360,   144,   361,   239,   363,   362,   200,     4,
       5,     6,     7,   323,   366,   395,   116,    82,    83,    84,
      12,    13,    14,   146,    15,    16,    17,   323,   360,   396,
     400,   302,   303,   380,   379,   239,   381,   401,   411,   148,
     200,   239,    29,   446,   239,   159,   470,    34,    35,    36,
     394,   160,    38,   201,   202,   203,   204,   205,   206,   207,
     208,   164,   212,   213,   323,   179,   472,   407,   323,   323,
     474,   493,   367,   368,   410,   239,   448,   239,   107,   108,
     169,   165,   416,   417,   418,    43,   214,   215,   171,   178,
     422,   181,    44,   182,   424,   183,   194,   200,   196,   229,
      45,   199,   232,   233,   235,   440,   441,   236,   237,   234,
     238,   253,   257,   258,   259,   269,   271,   280,   266,   273,
     275,   274,   281,   283,   284,   290,   291,   292,   300,   304,
     309,   310,   334,   311,   328,   330,   456,   329,   331,   458,
     370,   371,   372,   373,   350,   374,   375,   352,   353,   365,
     369,   376,   382,   468,   383,   384,   393,   397,   398,   399,
     402,   471,   403,   473,   404,   405,   406,   408,   480,   413,
     414,   482,   437,   425,   423,   426,   438,   439,   442,   443,
     444,   445,   486,   455,   487,   449,   450,   465,   457,     4,
       5,     6,     7,   492,   467,   466,     8,     9,    10,    11,
      12,    13,    14,   494,    15,    16,    17,    18,    19,    20,
      21,   479,   469,    22,    23,    24,    25,    26,    27,    28,
     475,   476,    29,    30,    31,    32,    33,    34,    35,    36,
      37,   477,    38,    39,   478,   481,   335,    40,    41,   485,
     488,   489,   490,   491,    42,   415,   180,   348,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    81,    82,
      83,    84,    12,    13,    14,    43,    15,    16,    17,     0,
       0,     0,    44,     0,     0,     0,     0,     0,    25,     0,
      45,     0,     0,     0,    29,   267,     0,     0,     0,    34,
      35,    36,     0,     0,    38,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    81,    82,    83,    84,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,    43,     0,     0,
      29,     0,     0,     0,    44,    34,    35,    36,     0,     0,
      38,     0,    45,   154,     4,     5,     6,     7,     0,     0,
       0,    81,    82,    83,    84,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,    43,     0,     0,     0,    29,     0,     0,
      44,     0,    34,    35,    36,     0,     0,    38,    45,   285,
       4,     5,     6,     7,     0,     0,     0,    81,    82,    83,
      84,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
      43,     0,     0,    29,     0,     0,     0,    44,    34,    35,
      36,     0,     0,    38,     0,    45,   288,     4,     5,     6,
       7,     0,     0,     0,    81,    82,    83,    84,   295,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,    43,     0,     0,     0,
      29,     0,     0,    44,     0,    34,    35,    36,     0,     0,
      38,    45,   293,     4,     5,     6,     7,     0,     0,     0,
      81,    82,    83,    84,   295,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,    43,     0,     0,    29,     0,     0,     0,
      44,    34,    35,    36,     0,     0,    38,     0,    45,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,    44,     0,     0,     4,
       5,     6,     7,     0,    45,   347,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,     0,     0,    22,    23,    24,    25,    26,    27,    28,
       0,     0,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,    38,    39,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,    42,     0,     4,     5,     6,     7,
       0,     0,     0,    81,    82,    83,    84,    12,    13,    14,
       0,    15,    16,    17,     0,    43,     0,     0,     0,     0,
       0,     0,    44,    25,     0,     0,     0,     0,     0,    29,
      45,     0,     0,     0,    34,    35,    36,     0,     0,    38,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      81,    82,    83,    84,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,    43,     0,     0,     0,    29,     0,     0,    44,
       0,    34,    35,    36,     0,    97,    38,    45,     0,     4,
       5,     6,     7,     0,     0,     0,    81,    82,    83,    84,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,    43,
       0,     0,    29,     0,     0,     0,    44,    34,    35,    36,
       0,     0,    38,     0,    85,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    43,     0,     0,    29,     0,
       0,     0,    44,    34,    35,    36,     0,     0,    38,     0,
      87,     4,     5,     6,     7,     0,     0,     0,    81,    82,
      83,    84,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    43,     0,     0,    29,     0,     0,     0,    44,    34,
      35,    36,     0,     0,    38,     0,    45,     4,     5,     6,
       7,     0,     0,     0,    81,    82,    83,    84,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,    43,     0,     0,
      29,     0,     0,     0,    44,    34,    35,    36,     0,     0,
      38,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,     0,   306
};

static const yytype_int16 yycheck[] =
{
       2,   280,   281,   228,    10,    10,   231,    23,    14,    14,
      14,   236,    10,   238,    43,    44,    14,    19,    20,    14,
      14,    23,    65,    10,    26,    27,    28,    14,    24,    14,
      10,    10,    96,    33,    36,    14,    38,    37,    78,    59,
      60,    61,    65,    45,    75,    14,    14,    78,    80,    17,
      14,     0,    34,    35,    94,    98,    14,    78,   283,    54,
      91,    84,    85,    86,    48,    49,    98,    69,    52,    71,
      94,    73,    74,    48,    49,    55,    56,    52,   303,    99,
      10,    80,    50,    85,    78,    87,    94,    89,    90,    95,
      95,    91,    50,    78,    98,    72,    73,    95,    92,    98,
     102,   103,    98,   105,   106,   107,   108,    92,    95,    78,
     139,   140,   141,   142,    78,    99,    95,    99,   134,   135,
      62,    65,    78,    92,    99,    55,    56,    80,    92,    10,
      80,    14,    13,    14,    15,    16,    92,    92,   363,    94,
      84,    85,    86,   368,    98,    98,   425,   426,    98,    91,
      63,    64,    94,    63,    64,   157,    78,   159,   160,    12,
     176,   177,   164,   165,    10,   167,    78,    78,    14,    12,
      92,    84,    85,    86,    84,    85,    86,    94,   180,    77,
      92,    92,    98,   185,    91,   187,   411,    94,   467,    87,
      88,    89,    14,    94,   194,    17,     3,     4,     5,     6,
      10,    11,    12,    10,    11,    12,    13,    14,    15,    16,
      10,    18,    19,    20,    14,    14,    66,    67,    68,    69,
      70,    71,    93,    30,    95,    93,    76,    95,   230,    36,
      80,    81,    82,    83,    41,    42,    43,   239,    29,    46,
     242,    14,    93,   245,    95,   247,    10,   249,    93,    13,
      95,    15,    16,   255,   256,    82,    83,   257,   258,   259,
      14,    10,    11,    12,   264,    14,     7,     8,     9,    84,
      85,    86,    79,   273,   274,   136,   137,   138,    92,    86,
      93,    93,    95,    95,    93,    91,    95,    94,   290,   291,
     292,    98,    93,    14,    95,    93,    93,    95,    95,     3,
       4,     5,     6,    93,   306,    95,    10,    11,    12,    13,
      14,    15,    16,    14,    18,    19,    20,    93,    93,    95,
      95,    92,    93,   325,   324,    93,   326,    95,    93,    14,
      95,    93,    36,    95,    93,    78,    95,    41,    42,    43,
     340,    78,    46,   121,   122,   123,   124,   125,   126,   127,
     128,    78,   130,   131,    93,    57,    95,   359,    93,    93,
      95,    95,    92,    93,    92,    93,    92,    93,    35,    36,
      92,    96,   372,   373,   374,    79,   132,   133,    92,    94,
     380,    92,    86,    92,   384,    98,    45,    95,    98,    92,
      94,    98,    97,    81,    92,   395,   396,    94,    14,    99,
      94,    14,    95,    95,    10,    14,    14,    94,    99,    95,
      99,    95,    94,    94,    14,    78,    78,    78,    92,    92,
      97,    81,    14,    99,    81,    81,   428,    99,    99,   431,
      95,    95,    25,    10,    92,    10,    10,    92,    92,    92,
      92,    10,    92,   445,    92,    25,    48,    14,    14,    14,
      92,   451,    92,   453,    92,    92,    92,    14,   460,    92,
      92,   463,    92,    94,    99,    94,    92,    14,     7,     7,
       7,    62,   472,    14,   474,    10,    10,    92,    14,     3,
       4,     5,     6,   483,    94,    92,    10,    11,    12,    13,
      14,    15,    16,   493,    18,    19,    20,    21,    22,    23,
      24,    14,    92,    27,    28,    29,    30,    31,    32,    33,
      92,    92,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    92,    46,    47,    92,    14,   278,    51,    52,    92,
      92,    92,    92,    92,    58,   372,    94,   283,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    79,    18,    19,    20,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      94,    -1,    -1,    -1,    36,    99,    -1,    -1,    -1,    41,
      42,    43,    -1,    -1,    46,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    79,    -1,    -1,
      36,    -1,    -1,    -1,    86,    41,    42,    43,    -1,    -1,
      46,    -1,    94,    95,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    79,    -1,    -1,    -1,    36,    -1,    -1,
      86,    -1,    41,    42,    43,    -1,    -1,    46,    94,    95,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      79,    -1,    -1,    36,    -1,    -1,    -1,    86,    41,    42,
      43,    -1,    -1,    46,    -1,    94,    95,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    79,    -1,    -1,    -1,
      36,    -1,    -1,    86,    -1,    41,    42,    43,    -1,    -1,
      46,    94,    95,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    79,    -1,    -1,    36,    -1,    -1,    -1,
      86,    41,    42,    43,    -1,    -1,    46,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,     3,
       4,     5,     6,    -1,    94,    95,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    47,    -1,    -1,    -1,    51,    52,    -1,
      -1,    -1,    -1,    -1,    58,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    30,    -1,    -1,    -1,    -1,    -1,    36,
      94,    -1,    -1,    -1,    41,    42,    43,    -1,    -1,    46,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    79,    -1,    -1,    -1,    36,    -1,    -1,    86,
      -1,    41,    42,    43,    -1,    92,    46,    94,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    79,
      -1,    -1,    36,    -1,    -1,    -1,    86,    41,    42,    43,
      -1,    -1,    46,    -1,    94,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    79,    -1,    -1,    36,    -1,
      -1,    -1,    86,    41,    42,    43,    -1,    -1,    46,    -1,
      94,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    79,    -1,    -1,    36,    -1,    -1,    -1,    86,    41,
      42,    43,    -1,    -1,    46,    -1,    94,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    79,    -1,    -1,
      36,    -1,    -1,    -1,    86,    41,    42,    43,    -1,    -1,
      46,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   101,   102,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    27,    28,    29,    30,    31,    32,    33,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      51,    52,    58,    79,    86,    94,   103,   104,   105,   106,
     107,   108,   109,   110,   112,   113,   114,   117,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   133,    75,    78,
      91,    96,    80,    98,    78,    91,    94,    10,    11,    12,
      14,    10,    11,    12,    13,    94,   123,    94,   123,    94,
      94,   123,   127,    14,    98,   121,   123,    92,   123,   123,
      12,    12,    94,    94,   121,    98,   122,   122,   122,   123,
     121,   123,    14,    14,    29,    14,    10,   129,   129,   123,
      92,    72,    73,    66,    67,    68,    69,    70,    71,    76,
      80,    81,    82,    83,    63,    64,    84,    85,    86,    77,
      87,    88,    89,    91,    14,   123,    14,   123,    14,   123,
     123,    14,    17,    50,    95,   123,   132,    78,    92,    78,
      78,    10,    11,    12,    78,    96,    80,    98,   123,    92,
     123,    92,   123,   123,    65,   121,    63,    64,    94,    57,
     102,    92,    92,    98,    80,    98,    80,    98,   123,   123,
     123,   123,   123,   123,    45,   115,    98,    14,    54,    98,
      95,   125,   125,   125,   125,   125,   125,   125,   125,     7,
       8,     9,   125,   125,   126,   126,   127,   127,   128,   128,
     128,   129,   129,   129,   129,    14,    17,    92,    94,    92,
      78,    94,    97,    81,    99,    92,    94,    14,    94,    93,
      95,   123,    94,   123,   123,    78,    92,    78,    92,    78,
      92,   123,   123,    14,   123,    93,    93,    95,    95,    10,
     127,   127,    10,    14,    95,   120,    99,    99,   111,    14,
     123,    14,   123,    95,    95,    99,   121,    14,    98,   116,
      94,    94,   118,    94,    14,    95,   132,   123,    95,   132,
      78,    78,    78,    95,   132,    14,    95,   119,   132,   123,
      92,   123,    92,    93,    92,   123,    94,   123,   123,    97,
      81,    99,   123,   123,   121,   121,   121,    65,    65,    10,
      55,    56,   121,    93,    95,    34,    35,    99,    81,    99,
      81,    99,   121,   121,    14,   116,    48,    49,    52,    99,
      95,   120,   120,    59,    60,    61,    99,    95,   119,   132,
      92,    95,    92,    92,    95,   123,   123,   123,    95,    62,
      93,    95,    95,    93,   132,    92,   123,    92,    93,    92,
      95,    95,    25,    10,    10,    10,    10,    10,    14,   121,
     123,   121,    92,    92,    25,    99,    14,    50,    10,    13,
      14,    15,    16,    48,   121,    95,    95,    14,    14,    14,
      95,    95,    92,    92,    92,    92,    92,   123,    14,   132,
      92,    93,   132,    92,    92,   104,   121,   121,   121,    10,
      55,    56,   121,    99,   121,    94,    94,    14,    78,    92,
      14,    78,    92,    10,    13,    15,    16,    92,    92,    14,
     121,   121,     7,     7,     7,    62,    95,   132,    92,    10,
      10,    95,   120,    95,   120,    14,   123,    14,   123,    14,
      78,    92,    14,    78,    92,    92,    92,    94,   123,    92,
      95,   121,    95,   121,    95,    92,    92,    92,    92,    14,
     123,    14,   123,    95,   120,    92,   121,   121,    92,    92,
      92,    92,   121,    95,   121
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   102,   102,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   104,   104,   104,   105,   106,   106,   107,
     108,   109,   109,   109,   109,   110,   110,   111,   111,   112,
     112,   112,   112,   112,   113,   114,   115,   115,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   117,   118,   118,   118,   118,   119,   119,   120,   120,
     120,   120,   120,   120,   120,   120,   121,   121,   122,   123,
     123,   123,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   125,   125,   125,   126,
     126,   126,   127,   127,   127,   127,   128,   128,   128,   128,
     128,   129,   129,   129,   130,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   132,   132,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     7,     9,
       5,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       7,     3,     7,     3,     3,     2,     4,     7,     6,     4,
       6,     4,     7,     6,     7,     7,     7,     6,     6,     2,
       1,     1,     1,     1,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     5,     7,     7,     5,     5,     7,     5,
       2,     7,     7,     5,     3,     5,     7,     0,     4,     6,
       5,     7,     6,     7,     6,     5,     0,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     7,     7,     7,     6,     7,     6,     8,
       7,     5,     0,     4,     4,     4,     3,     5,     1,     2,
       3,     3,     3,     4,     5,     5,     3,     3,     3,     3,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     2,     2,     1,     1,     4,     3,     6,     6,     5,
       6,     5,     4,     4,     6,     6,     5,     3,     2,     3,
       3,     2,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     5,
       5,     1,     1,     1,     1,     3
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
#line 226 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1830 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 236 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1836 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 237 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1842 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 241 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1848 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 243 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1854 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 245 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1860 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 247 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1868 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 251 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1876 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 255 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1882 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 257 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1888 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 259 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1894 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 261 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1902 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 265 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1910 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 269 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1916 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 271 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1922 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 273 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1928 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 275 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1934 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY expr ';'  */
#line 277 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1940 "raku.tab.c"
    break;

  case 20: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 279 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1946 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT expr ';'  */
#line 281 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1952 "raku.tab.c"
    break;

  case 22: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 283 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1958 "raku.tab.c"
    break;

  case 23: /* stmt: KW_TAKE expr ';'  */
#line 285 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1964 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN expr ';'  */
#line 287 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1970 "raku.tab.c"
    break;

  case 25: /* stmt: KW_RETURN ';'  */
#line 289 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1976 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 291 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1982 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 293 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 1991 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 298 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 1999 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 302 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2007 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 306 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2016 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 311 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2024 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 315 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2035 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 322 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2044 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 327 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2051 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 330 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2058 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 333 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2065 "raku.tab.c"
    break;

  case 37: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 336 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2072 "raku.tab.c"
    break;

  case 38: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 339 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2079 "raku.tab.c"
    break;

  case 39: /* stmt: expr ';'  */
#line 341 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2085 "raku.tab.c"
    break;

  case 40: /* stmt: if_stmt  */
#line 342 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2091 "raku.tab.c"
    break;

  case 41: /* stmt: while_stmt  */
#line 343 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2097 "raku.tab.c"
    break;

  case 42: /* stmt: for_stmt  */
#line 344 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2103 "raku.tab.c"
    break;

  case 43: /* stmt: given_stmt  */
#line 345 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2109 "raku.tab.c"
    break;

  case 44: /* stmt: KW_TRY block  */
#line 347 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2115 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TRY block KW_CATCH block  */
#line 349 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2121 "raku.tab.c"
    break;

  case 46: /* stmt: unless_stmt  */
#line 350 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2127 "raku.tab.c"
    break;

  case 47: /* stmt: until_stmt  */
#line 351 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2133 "raku.tab.c"
    break;

  case 48: /* stmt: repeat_stmt  */
#line 352 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2139 "raku.tab.c"
    break;

  case 49: /* stmt: sub_decl  */
#line 353 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2145 "raku.tab.c"
    break;

  case 50: /* stmt: class_decl  */
#line 354 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2151 "raku.tab.c"
    break;

  case 51: /* stmt: role_decl  */
#line 355 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2157 "raku.tab.c"
    break;

  case 52: /* stmt: grammar_decl  */
#line 356 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2163 "raku.tab.c"
    break;

  case 53: /* if_stmt: KW_IF '(' expr ')' block  */
#line 360 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2169 "raku.tab.c"
    break;

  case 54: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 362 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2175 "raku.tab.c"
    break;

  case 55: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 364 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2181 "raku.tab.c"
    break;

  case 56: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 368 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2187 "raku.tab.c"
    break;

  case 57: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 372 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2193 "raku.tab.c"
    break;

  case 58: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 374 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2199 "raku.tab.c"
    break;

  case 59: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2205 "raku.tab.c"
    break;

  case 60: /* repeat_stmt: KW_REPEAT block  */
#line 382 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2211 "raku.tab.c"
    break;

  case 61: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 386 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2221 "raku.tab.c"
    break;

  case 62: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 392 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2231 "raku.tab.c"
    break;

  case 63: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 398 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2240 "raku.tab.c"
    break;

  case 64: /* for_stmt: KW_FOR expr block  */
#line 403 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2247 "raku.tab.c"
    break;

  case 65: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 408 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2259 "raku.tab.c"
    break;

  case 66: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 416 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2272 "raku.tab.c"
    break;

  case 67: /* when_list: %empty  */
#line 426 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2278 "raku.tab.c"
    break;

  case 68: /* when_list: when_list KW_WHEN expr block  */
#line 428 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2286 "raku.tab.c"
    break;

  case 69: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 434 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2298 "raku.tab.c"
    break;

  case 70: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 442 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2308 "raku.tab.c"
    break;

  case 71: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 448 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2321 "raku.tab.c"
    break;

  case 72: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 457 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2332 "raku.tab.c"
    break;

  case 73: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 464 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2345 "raku.tab.c"
    break;

  case 74: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 475 "raku.y"
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
#line 2363 "raku.tab.c"
    break;

  case 75: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 491 "raku.y"
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
#line 2380 "raku.tab.c"
    break;

  case 76: /* is_clauses: %empty  */
#line 505 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2386 "raku.tab.c"
    break;

  case 77: /* is_clauses: is_clauses IDENT IDENT  */
#line 507 "raku.y"
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
#line 2402 "raku.tab.c"
    break;

  case 78: /* class_body_list: %empty  */
#line 520 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2408 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 522 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2415 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 525 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2422 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 528 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2429 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 531 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2436 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 534 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2443 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 537 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2450 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 540 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2457 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 543 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2464 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 546 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2471 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 549 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2478 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 552 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2485 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 555 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2492 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 558 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2503 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 565 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2514 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 572 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2525 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 579 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2536 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 586 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2550 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 596 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2562 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 604 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2575 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 613 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2586 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 620 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2600 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 630 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2612 "raku.tab.c"
    break;

  case 101: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 640 "raku.y"
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
#line 2629 "raku.tab.c"
    break;

  case 102: /* grammar_body_list: %empty  */
#line 654 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2635 "raku.tab.c"
    break;

  case 103: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 656 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2644 "raku.tab.c"
    break;

  case 104: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 661 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2653 "raku.tab.c"
    break;

  case 105: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 666 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2662 "raku.tab.c"
    break;

  case 106: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 673 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2670 "raku.tab.c"
    break;

  case 107: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 677 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2678 "raku.tab.c"
    break;

  case 108: /* param_list: VAR_SCALAR  */
#line 682 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2684 "raku.tab.c"
    break;

  case 109: /* param_list: IDENT VAR_SCALAR  */
#line 683 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2690 "raku.tab.c"
    break;

  case 110: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 684 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2696 "raku.tab.c"
    break;

  case 111: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 685 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2702 "raku.tab.c"
    break;

  case 112: /* param_list: param_list ',' VAR_SCALAR  */
#line 686 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2708 "raku.tab.c"
    break;

  case 113: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 687 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2714 "raku.tab.c"
    break;

  case 114: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 688 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2720 "raku.tab.c"
    break;

  case 115: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 689 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2726 "raku.tab.c"
    break;

  case 116: /* block: '{' stmt_list '}'  */
#line 692 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2732 "raku.tab.c"
    break;

  case 117: /* block: '{' YADA '}'  */
#line 693 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2738 "raku.tab.c"
    break;

  case 118: /* closure: '{' expr '}'  */
#line 696 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2744 "raku.tab.c"
    break;

  case 119: /* expr: VAR_SCALAR '=' expr  */
#line 699 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2750 "raku.tab.c"
    break;

  case 120: /* expr: KW_GATHER block  */
#line 700 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2760 "raku.tab.c"
    break;

  case 121: /* expr: cmp_expr  */
#line 705 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2766 "raku.tab.c"
    break;

  case 122: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 708 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2772 "raku.tab.c"
    break;

  case 123: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 709 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2778 "raku.tab.c"
    break;

  case 124: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 710 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2784 "raku.tab.c"
    break;

  case 125: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 711 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2790 "raku.tab.c"
    break;

  case 126: /* cmp_expr: jct_expr '<' jct_expr  */
#line 712 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2796 "raku.tab.c"
    break;

  case 127: /* cmp_expr: jct_expr '>' jct_expr  */
#line 713 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2802 "raku.tab.c"
    break;

  case 128: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 714 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2808 "raku.tab.c"
    break;

  case 129: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 715 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2814 "raku.tab.c"
    break;

  case 130: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 716 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2820 "raku.tab.c"
    break;

  case 131: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 717 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2826 "raku.tab.c"
    break;

  case 132: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 719 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2836 "raku.tab.c"
    break;

  case 133: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 725 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2846 "raku.tab.c"
    break;

  case 134: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 731 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2856 "raku.tab.c"
    break;

  case 135: /* cmp_expr: jct_expr  */
#line 736 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2862 "raku.tab.c"
    break;

  case 136: /* jct_expr: jct_expr '|' range_expr  */
#line 739 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2868 "raku.tab.c"
    break;

  case 137: /* jct_expr: jct_expr '&' range_expr  */
#line 740 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2874 "raku.tab.c"
    break;

  case 138: /* jct_expr: range_expr  */
#line 741 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2880 "raku.tab.c"
    break;

  case 139: /* range_expr: add_expr OP_RANGE add_expr  */
#line 744 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2886 "raku.tab.c"
    break;

  case 140: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 745 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2892 "raku.tab.c"
    break;

  case 141: /* range_expr: add_expr  */
#line 746 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2898 "raku.tab.c"
    break;

  case 142: /* add_expr: add_expr '+' mul_expr  */
#line 749 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2904 "raku.tab.c"
    break;

  case 143: /* add_expr: add_expr '-' mul_expr  */
#line 750 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2910 "raku.tab.c"
    break;

  case 144: /* add_expr: add_expr '~' mul_expr  */
#line 751 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2916 "raku.tab.c"
    break;

  case 145: /* add_expr: mul_expr  */
#line 752 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2922 "raku.tab.c"
    break;

  case 146: /* mul_expr: mul_expr '*' unary_expr  */
#line 755 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2928 "raku.tab.c"
    break;

  case 147: /* mul_expr: mul_expr '/' unary_expr  */
#line 756 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2934 "raku.tab.c"
    break;

  case 148: /* mul_expr: mul_expr '%' unary_expr  */
#line 757 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2940 "raku.tab.c"
    break;

  case 149: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 758 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2946 "raku.tab.c"
    break;

  case 150: /* mul_expr: unary_expr  */
#line 759 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2952 "raku.tab.c"
    break;

  case 151: /* unary_expr: '-' unary_expr  */
#line 762 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2958 "raku.tab.c"
    break;

  case 152: /* unary_expr: '!' unary_expr  */
#line 763 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2964 "raku.tab.c"
    break;

  case 153: /* unary_expr: postfix_expr  */
#line 764 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2970 "raku.tab.c"
    break;

  case 154: /* postfix_expr: call_expr  */
#line 766 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2976 "raku.tab.c"
    break;

  case 155: /* call_expr: IDENT '(' arg_list ')'  */
#line 769 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2985 "raku.tab.c"
    break;

  case 156: /* call_expr: IDENT '(' ')'  */
#line 773 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2991 "raku.tab.c"
    break;

  case 157: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 775 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3001 "raku.tab.c"
    break;

  case 158: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 781 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3011 "raku.tab.c"
    break;

  case 159: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 787 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3019 "raku.tab.c"
    break;

  case 160: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 791 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3030 "raku.tab.c"
    break;

  case 161: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 798 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3039 "raku.tab.c"
    break;

  case 162: /* call_expr: IDENT '.' CARET IDENT  */
#line 803 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3049 "raku.tab.c"
    break;

  case 163: /* call_expr: atom '.' CARET IDENT  */
#line 809 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3059 "raku.tab.c"
    break;

  case 164: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 815 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3070 "raku.tab.c"
    break;

  case 165: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 822 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3081 "raku.tab.c"
    break;

  case 166: /* call_expr: atom '.' IDENT '(' ')'  */
#line 829 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3090 "raku.tab.c"
    break;

  case 167: /* call_expr: atom '.' IDENT  */
#line 834 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3099 "raku.tab.c"
    break;

  case 168: /* call_expr: KW_DIE expr  */
#line 839 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3105 "raku.tab.c"
    break;

  case 169: /* call_expr: KW_MAP closure expr  */
#line 841 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3111 "raku.tab.c"
    break;

  case 170: /* call_expr: KW_GREP closure expr  */
#line 843 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3117 "raku.tab.c"
    break;

  case 171: /* call_expr: KW_SORT expr  */
#line 845 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3123 "raku.tab.c"
    break;

  case 172: /* call_expr: KW_SORT closure expr  */
#line 847 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3129 "raku.tab.c"
    break;

  case 173: /* call_expr: atom  */
#line 848 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3135 "raku.tab.c"
    break;

  case 174: /* arg_list: expr  */
#line 851 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3141 "raku.tab.c"
    break;

  case 175: /* arg_list: arg_list ',' expr  */
#line 852 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3147 "raku.tab.c"
    break;

  case 176: /* atom: LIT_INT  */
#line 855 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3153 "raku.tab.c"
    break;

  case 177: /* atom: LIT_FLOAT  */
#line 856 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3159 "raku.tab.c"
    break;

  case 178: /* atom: LIT_STR  */
#line 857 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3165 "raku.tab.c"
    break;

  case 179: /* atom: LIT_INTERP_STR  */
#line 858 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3171 "raku.tab.c"
    break;

  case 180: /* atom: VAR_SCALAR  */
#line 859 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3177 "raku.tab.c"
    break;

  case 181: /* atom: VAR_ARRAY  */
#line 860 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3183 "raku.tab.c"
    break;

  case 182: /* atom: VAR_HASH  */
#line 861 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3189 "raku.tab.c"
    break;

  case 183: /* atom: VAR_CAPTURE  */
#line 863 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3197 "raku.tab.c"
    break;

  case 184: /* atom: VAR_FH  */
#line 867 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3205 "raku.tab.c"
    break;

  case 185: /* atom: VAR_NAMED_CAPTURE  */
#line 871 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3212 "raku.tab.c"
    break;

  case 186: /* atom: VAR_ARRAY '[' expr ']'  */
#line 874 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3218 "raku.tab.c"
    break;

  case 187: /* atom: VAR_HASH '<' IDENT '>'  */
#line 876 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3224 "raku.tab.c"
    break;

  case 188: /* atom: VAR_HASH '{' expr '}'  */
#line 878 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3230 "raku.tab.c"
    break;

  case 189: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 880 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3236 "raku.tab.c"
    break;

  case 190: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 882 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3242 "raku.tab.c"
    break;

  case 191: /* atom: IDENT  */
#line 883 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3248 "raku.tab.c"
    break;

  case 192: /* atom: VAR_TWIGIL  */
#line 885 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3256 "raku.tab.c"
    break;

  case 193: /* atom: VAR_ARRAY_TWIGIL  */
#line 889 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3264 "raku.tab.c"
    break;

  case 194: /* atom: VAR_HASH_TWIGIL  */
#line 893 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3272 "raku.tab.c"
    break;

  case 195: /* atom: '(' expr ')'  */
#line 896 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3278 "raku.tab.c"
    break;


#line 3282 "raku.tab.c"

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

#line 898 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
