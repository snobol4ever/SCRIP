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

#line 233 "raku.tab.c"

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
  YYSYMBOL_OP_DOTEQ = 74,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 75,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 76,                    /* OP_DIV  */
  YYSYMBOL_77_ = 77,                       /* '='  */
  YYSYMBOL_78_ = 78,                       /* '!'  */
  YYSYMBOL_79_ = 79,                       /* '<'  */
  YYSYMBOL_80_ = 80,                       /* '>'  */
  YYSYMBOL_81_ = 81,                       /* '|'  */
  YYSYMBOL_82_ = 82,                       /* '&'  */
  YYSYMBOL_83_ = 83,                       /* '~'  */
  YYSYMBOL_84_ = 84,                       /* '+'  */
  YYSYMBOL_85_ = 85,                       /* '-'  */
  YYSYMBOL_86_ = 86,                       /* '*'  */
  YYSYMBOL_87_ = 87,                       /* '/'  */
  YYSYMBOL_88_ = 88,                       /* '%'  */
  YYSYMBOL_UMINUS = 89,                    /* UMINUS  */
  YYSYMBOL_90_ = 90,                       /* '.'  */
  YYSYMBOL_91_ = 91,                       /* ';'  */
  YYSYMBOL_92_ = 92,                       /* ','  */
  YYSYMBOL_93_ = 93,                       /* '('  */
  YYSYMBOL_94_ = 94,                       /* ')'  */
  YYSYMBOL_95_ = 95,                       /* '['  */
  YYSYMBOL_96_ = 96,                       /* ']'  */
  YYSYMBOL_97_ = 97,                       /* '{'  */
  YYSYMBOL_98_ = 98,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 99,                  /* $accept  */
  YYSYMBOL_program = 100,                  /* program  */
  YYSYMBOL_stmt_list = 101,                /* stmt_list  */
  YYSYMBOL_stmt = 102,                     /* stmt  */
  YYSYMBOL_if_stmt = 103,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 104,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 105,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 106,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 107,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 108,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 109,               /* given_stmt  */
  YYSYMBOL_when_list = 110,                /* when_list  */
  YYSYMBOL_sub_decl = 111,                 /* sub_decl  */
  YYSYMBOL_class_decl = 112,               /* class_decl  */
  YYSYMBOL_role_decl = 113,                /* role_decl  */
  YYSYMBOL_is_clauses = 114,               /* is_clauses  */
  YYSYMBOL_class_body_list = 115,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 116,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 117,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 118,           /* named_arg_list  */
  YYSYMBOL_param_list = 119,               /* param_list  */
  YYSYMBOL_block = 120,                    /* block  */
  YYSYMBOL_closure = 121,                  /* closure  */
  YYSYMBOL_expr = 122,                     /* expr  */
  YYSYMBOL_cmp_expr = 123,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 124,                 /* jct_expr  */
  YYSYMBOL_range_expr = 125,               /* range_expr  */
  YYSYMBOL_add_expr = 126,                 /* add_expr  */
  YYSYMBOL_mul_expr = 127,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 128,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 129,             /* postfix_expr  */
  YYSYMBOL_call_expr = 130,                /* call_expr  */
  YYSYMBOL_arg_list = 131,                 /* arg_list  */
  YYSYMBOL_atom = 132                      /* atom  */
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
#define YYLAST   1237

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  194
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  490

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   332


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
       2,     2,     2,    78,     2,     2,     2,    88,    82,     2,
      93,    94,    86,    84,    92,    85,    90,    87,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    91,
      79,    77,    80,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    95,     2,    96,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    97,    81,    98,    83,     2,     2,     2,
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
      75,    76,    89
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
     441,   447,   456,   465,   481,   496,   497,   511,   512,   515,
     518,   521,   524,   527,   530,   533,   536,   539,   542,   545,
     548,   555,   562,   569,   576,   586,   594,   603,   610,   620,
     630,   645,   646,   651,   656,   663,   667,   673,   674,   675,
     676,   677,   678,   679,   680,   683,   684,   687,   690,   691,
     696,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   715,   721,   727,   730,   731,   732,   735,   736,
     737,   740,   741,   742,   743,   746,   747,   748,   749,   750,
     753,   754,   755,   757,   759,   764,   765,   771,   777,   781,
     788,   793,   799,   805,   812,   819,   824,   829,   831,   833,
     835,   837,   839,   842,   843,   846,   847,   848,   849,   850,
     851,   852,   853,   857,   861,   864,   866,   868,   870,   872,
     874,   875,   879,   883,   887
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
  "OP_DOTEQ", "OP_SMATCH", "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'",
  "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'",
  "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "$accept", "program",
  "stmt_list", "stmt", "if_stmt", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "class_decl", "role_decl", "is_clauses", "class_body_list",
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

#define YYPACT_NINF (-279)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -279,     9,   815,  -279,  -279,  -279,  -279,  -279,    46,   -73,
     -33,   -16,    52,  -279,  -279,  -279,  -279,  -279,   257,   917,
     962,   -40,   -36,  1008,   100,    24,  1008,   871,  1008,    86,
     120,    44,    56,    24,    64,    64,   192,    24,  1008,   140,
     195,   135,   223,  1144,  1144,  1008,  -279,  -279,  -279,  -279,
    -279,  -279,  -279,  -279,  -279,  -279,  -279,  -279,   156,    99,
     150,  -279,    67,   103,  -279,  -279,  -279,   208,   226,  1008,
     252,  1008,   287,  1008,  1008,    49,   300,     3,   250,   275,
     176,   282,   213,   -24,  -279,  1008,   258,  1008,   274,  1008,
    1008,   -56,    84,   224,   284,  -279,   283,  -279,   285,   247,
     -19,    34,  1008,  1008,  -279,  1008,  1008,  1008,  1008,  -279,
     330,  -279,  -279,   286,   365,   289,  -279,  -279,  -279,   288,
    -279,  1144,  1144,  1144,  1144,  1144,  1144,  1144,  1144,   267,
    1144,  1144,  1144,  1144,  1144,  1144,  1144,  1144,  1144,  1144,
    1144,  1144,  1144,   124,   130,   293,   -12,   291,   308,   292,
     298,   302,   366,   303,  -279,  -279,   132,  1008,  -279,  1053,
    1008,    42,    48,    57,  1008,  1008,   377,  1008,   149,  -279,
     189,  -279,   304,   305,   387,  -279,  1144,  1144,     1,   306,
     471,  -279,  -279,  -279,   386,  1008,   388,  1008,   307,   309,
     310,  -279,  -279,  -279,    24,   -11,  -279,   312,  -279,  -279,
     111,   111,   111,   111,   111,   111,   111,   111,  -279,  -279,
    -279,   111,   111,  -279,  -279,   116,   116,   103,   103,   103,
    -279,  -279,  -279,  -279,   313,   393,  -279,   532,  -279,  1008,
     577,   332,   333,   334,  -279,   631,  -279,   676,  1008,  -279,
     321,  1008,   122,   322,  1008,  -279,  1099,  -279,  1008,  -279,
    -279,   318,   335,   319,  1008,  1008,    24,    24,    24,    89,
      92,  -279,    20,    24,   190,  -279,  -279,    -6,   336,   320,
     339,   323,    24,    24,  -279,  -279,   406,  -279,    -1,    13,
      68,   730,  -279,   337,   215,   338,   340,   231,  1008,  1008,
    1008,  -279,   234,   -41,  -279,   237,   240,  -279,  -279,   241,
    -279,  1008,  -279,   341,  1008,   163,   342,  -279,  -279,  -279,
     329,   343,   399,  -279,  -279,   416,   417,  -279,   420,   424,
    -279,    40,    24,  1008,    24,  -279,  -279,  -279,   344,   345,
     414,  -279,  -279,    10,    18,   235,   392,  -279,    24,   245,
     427,   428,   429,  -279,  -279,   253,   256,  -279,   353,  -279,
    -279,   354,   356,   358,   359,  -279,  1008,   437,  -279,  -279,
    1008,   276,  -279,   259,  -279,  1008,  -279,   361,   363,   -20,
      24,    24,  -279,  -279,  -279,    35,  -279,    24,   357,  -279,
    -279,    24,  -279,   364,   367,    -7,     2,   249,   368,   370,
     444,  -279,    24,   455,   456,   457,  -279,  -279,  -279,  -279,
    -279,  -279,  -279,  -279,   404,   263,  -279,  1008,   281,  -279,
    -279,  -279,  -279,  -279,  -279,  -279,   458,   460,  -279,  -279,
    -279,    21,    28,   452,  1008,  -279,   459,  1008,  -279,     5,
      25,   380,   381,  -279,  -279,   403,  -279,  -279,  -279,  -279,
    1008,   389,   264,  -279,  -279,  -279,    24,   268,    24,   269,
     415,   425,   430,   433,   465,  1008,  -279,   483,  1008,  -279,
    -279,  -279,    30,  -279,  -279,   434,  -279,    24,  -279,    24,
    -279,  -279,  -279,  -279,   435,   436,   438,   439,    24,   272,
    -279,  -279,  -279,  -279,  -279,  -279,  -279,  -279,    24,  -279
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   175,   176,   177,   178,   179,   180,
     181,   191,   190,   192,   193,   182,   183,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,    40,    41,    46,
      47,    48,    42,    43,    49,    50,    51,    52,     0,   120,
     134,   137,   140,   144,   149,   152,   153,   172,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   179,   180,   181,   191,     0,     0,     0,     0,     0,
       0,     0,   140,     0,     3,   119,     0,    25,     0,     0,
       0,     0,     0,     0,    60,     0,     0,     0,     0,   170,
      44,   167,    75,     0,     0,     0,   179,   151,   150,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,   173,     0,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,    21,     0,     0,     0,    64,     0,     0,     0,     0,
       0,    23,    24,    67,     0,     0,     0,     0,     0,     0,
       0,   168,   169,   171,     0,     0,    77,     0,   101,   194,
     121,   122,   123,   124,   127,   128,   129,   130,   131,   132,
     133,   125,   126,   135,   136,   138,   139,   143,   141,   142,
     148,   145,   146,   147,   166,     0,    29,     0,    26,     0,
       0,   185,   186,   187,    31,     0,   161,     0,     0,   154,
       0,     0,     0,     0,     0,    16,     0,    17,     0,    18,
     118,     0,     0,     0,     0,     0,     0,     0,     0,   138,
     139,   107,     0,     0,     0,   116,   115,     0,     0,     0,
       0,     0,     0,     0,   117,    45,     0,    77,     0,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,   190,   158,     0,     0,   174,     5,     0,
       7,     0,    10,     0,     0,     0,     0,   185,   186,   187,
       0,     0,    53,    56,    63,     0,     0,   108,     0,     0,
      70,     0,     0,     0,     0,    65,   188,   189,     0,     0,
      57,    59,    76,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,   100,   165,     0,     0,    28,     0,    30,
      33,     0,     0,     0,     0,   159,     0,     0,   156,   157,
       0,     0,    11,     0,    12,     0,    15,     0,     0,     0,
       0,     0,   109,   110,   111,     0,    69,     0,     0,    37,
      38,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,    72,     0,     0,     0,     0,   164,   163,    27,    32,
      34,    35,    36,   105,     0,     0,     8,     0,     0,    20,
      22,    55,    54,    61,    62,   112,     0,     0,    68,    66,
      58,     0,     0,     0,     0,    83,     0,     0,    78,     0,
       0,     0,     0,    79,    80,     0,    71,   102,   103,   104,
       0,     0,     0,    13,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,     0,     0,    84,
      81,    82,     0,   106,     9,     0,    95,     0,    97,     0,
      91,    87,    90,    86,     0,     0,     0,     0,     0,     0,
      14,    94,    96,    93,    89,    92,    88,    99,     0,    98
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -279,  -279,   411,  -279,   151,  -279,  -279,  -279,  -279,  -279,
    -279,  -279,  -279,  -279,  -279,  -279,   242,  -279,  -279,   251,
    -278,     0,   145,    -2,  -279,   169,   128,   -17,   142,   -30,
    -279,  -279,  -225,  -279
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    46,    47,    48,    49,    50,    51,    52,
      53,   267,    54,    55,    56,   195,   278,    57,   280,   295,
     264,    95,   106,   155,    59,    60,    61,    62,    63,    64,
      65,    66,   156,    67
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   339,   284,   276,    21,   287,    92,   423,   174,     3,
     292,   261,   296,   117,   118,   262,   426,    86,    88,   454,
     356,    91,    71,   261,    96,    98,    99,   262,   323,   324,
     317,   261,   383,   104,   109,   262,   111,   110,   261,   457,
     261,    94,   262,   119,   262,   415,    72,   334,   335,    75,
     374,   336,    76,    89,   375,   166,   346,    90,   334,   335,
     184,    74,   336,   151,    73,   229,   152,   145,   384,   147,
     424,   149,   150,   167,   318,   319,   361,    94,   185,   427,
     157,   230,   455,   168,   425,   170,   277,   172,   173,   416,
     417,   175,   325,   428,   158,   263,   456,   337,   100,   153,
     188,   189,   458,   190,   191,   192,   193,   338,   382,   220,
     221,   222,   223,   186,    93,   446,   459,   215,   216,   244,
      68,    94,   448,    69,   478,   246,   340,   341,   342,   134,
     135,   187,   101,   245,   248,   405,    70,   102,   224,   247,
     408,   225,    75,   447,   449,    76,   176,   177,   249,   103,
     136,   137,   138,   315,   112,   240,   316,   242,   243,   259,
     260,   105,   250,   251,   114,   253,   343,   136,   137,   138,
     121,   122,   136,   137,   138,   136,   137,   138,    58,   139,
     107,   108,   442,   269,   479,   271,   161,   162,   163,   140,
     141,   142,   132,   133,   275,     4,     5,     6,     7,   136,
     137,   138,    81,    82,    83,    84,    12,    13,    14,   113,
      15,    16,    17,   300,   301,   123,   124,   125,   126,   127,
     128,   226,    25,   227,   238,   129,   239,   285,    29,   130,
     131,   132,   133,    34,    35,    36,   297,   115,    38,   299,
     144,   254,   303,   199,   305,   385,   306,   120,   386,   387,
     388,   389,   310,   311,   364,   365,   312,   313,   314,   429,
     213,   214,   430,   320,   431,   432,   146,    77,    78,    79,
      43,    80,   330,   331,   208,   209,   210,    44,   217,   218,
     219,   255,   321,   199,   322,    45,   352,   353,   354,   105,
     200,   201,   202,   203,   204,   205,   206,   207,   143,   211,
     212,   148,   363,     4,     5,     6,     7,   238,   165,   348,
      81,    82,    83,    84,    12,    13,    14,   178,    15,    16,
      17,   377,   376,   238,   378,   351,   238,   159,   355,   357,
      25,   358,   238,   360,   359,   199,    29,   321,   391,   392,
     179,    34,    35,    36,   183,   357,    38,   396,   238,   169,
     397,   407,   160,   199,   403,   238,   238,   441,   465,   164,
     321,   321,   467,   469,   321,   171,   488,   406,   238,   412,
     413,   414,   443,   238,   181,   194,   182,   418,    43,   197,
     236,   420,   199,   196,   228,    44,   198,   231,   232,   234,
     233,   252,   436,    45,   154,   235,   237,   258,   256,   257,
     268,   272,   270,   273,   265,   279,   281,   282,   274,   288,
     289,   290,   298,   302,   307,   308,   326,   309,   327,   328,
     332,   329,   451,   367,   369,   453,   370,   371,   347,   349,
     372,   350,   362,   366,   373,   379,   380,   368,   463,   381,
     390,   393,   394,   395,   398,   399,   466,   400,   468,   401,
     402,   404,   409,   475,   410,   419,   477,   421,   435,   433,
     422,   434,   437,   438,   439,   440,   450,   481,   444,   482,
     445,   460,   461,   452,     4,     5,     6,     7,   487,   474,
     464,     8,     9,    10,    11,    12,    13,    14,   489,    15,
      16,    17,    18,    19,    20,    21,   462,   476,    22,    23,
      24,    25,    26,    27,    28,   180,   470,    29,    30,    31,
      32,    33,    34,    35,    36,    37,   471,    38,    39,   333,
     411,   472,    40,    41,   473,   480,   483,   484,    42,   485,
     486,     0,   345,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,    43,
      15,    16,    17,     0,     0,     0,    44,     0,     0,     0,
       0,     0,    25,     0,    45,     0,     0,     0,    29,   266,
       0,     0,     0,    34,    35,    36,     0,     0,    38,     0,
       4,     5,     6,     7,     0,     0,     0,    81,    82,    83,
      84,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
      43,     0,     0,    29,     0,     0,     0,    44,    34,    35,
      36,     0,     0,    38,     0,    45,   283,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    81,    82,    83,    84,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,    43,     0,     0,     0,     0,
       0,    25,    44,     0,     0,     0,     0,    29,     0,     0,
      45,   286,    34,    35,    36,     0,     0,    38,     0,     4,
       5,     6,     7,     0,     0,     0,    81,    82,    83,    84,
     293,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,    43,
       0,     0,    29,     0,     0,     0,    44,    34,    35,    36,
       0,     0,    38,     0,    45,   291,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      81,    82,    83,    84,   293,    13,    14,     0,    15,    16,
      17,     0,     0,     0,    43,     0,     0,     0,     0,     0,
      25,    44,     0,     0,     0,     0,    29,     0,     0,    45,
     294,    34,    35,    36,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,    44,     0,     0,     4,     5,
       6,     7,     0,    45,   344,     8,     9,    10,    11,    12,
      13,    14,     0,    15,    16,    17,    18,    19,    20,    21,
       0,     0,    22,    23,    24,    25,    26,    27,    28,     0,
       0,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       0,    38,    39,     0,     0,     0,    40,    41,     0,     0,
       0,     0,    42,     0,     4,     5,     6,     7,     0,     0,
       0,    81,    82,    83,    84,    12,    13,    14,     0,    15,
      16,    17,     0,    43,     0,     0,     0,     0,     0,     0,
      44,    25,     0,     0,     0,     0,     0,    29,    45,     0,
       0,     0,    34,    35,    36,     0,     0,    38,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    81,    82,    83,
      84,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,    43,
       0,     0,     0,    29,     0,     0,    44,     0,    34,    35,
      36,     0,    97,    38,    45,     4,     5,     6,     7,     0,
       0,     0,    81,    82,    83,    84,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    43,     0,     0,    29,     0,
       0,     0,    44,    34,    35,    36,     0,     0,    38,     0,
      85,     4,     5,     6,     7,     0,     0,     0,    81,    82,
      83,    84,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
      43,     0,     0,     0,    29,     0,     0,    44,     0,    34,
      35,    36,     0,     0,    38,    87,     4,     5,     6,     7,
       0,     0,     0,    81,    82,    83,    84,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,    43,     0,     0,    29,
       0,     0,     0,    44,    34,    35,    36,     0,     0,    38,
       0,    45,     4,     5,     6,     7,     0,     0,     0,    81,
      82,    83,    84,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,    43,     0,     0,     0,    29,     0,     0,    44,     0,
      34,    35,    36,     0,     0,    38,   241,     4,     5,     6,
       7,     0,     0,     0,   116,    82,    83,    84,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
      29,     0,     0,     0,    44,    34,    35,    36,     0,     0,
      38,     0,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,     0,     0,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     0,    45
};

static const yytype_int16 yycheck[] =
{
       2,   279,   227,    14,    24,   230,    23,    14,    64,     0,
     235,    10,   237,    43,    44,    14,    14,    19,    20,    14,
      61,    23,    95,    10,    26,    27,    28,    14,    34,    35,
      10,    10,    14,    33,    36,    14,    38,    37,    10,    14,
      10,    97,    14,    45,    14,    10,    79,    48,    49,    90,
      10,    52,    93,    93,    14,    79,   281,    93,    48,    49,
      79,    77,    52,    14,    97,    77,    17,    69,    50,    71,
      77,    73,    74,    97,    54,    55,   301,    97,    97,    77,
      77,    93,    77,    85,    91,    87,    97,    89,    90,    54,
      55,    91,    98,    91,    91,    94,    91,    98,    12,    50,
     102,   103,    77,   105,   106,   107,   108,    94,    98,   139,
     140,   141,   142,    79,    14,    94,    91,   134,   135,    77,
      74,    97,    94,    77,    94,    77,    58,    59,    60,    62,
      63,    97,    12,    91,    77,   360,    90,    93,    14,    91,
     365,    17,    90,   421,   422,    93,    62,    63,    91,    93,
      83,    84,    85,    64,    14,   157,    64,   159,   160,   176,
     177,    97,   164,   165,    29,   167,    98,    83,    84,    85,
      71,    72,    83,    84,    85,    83,    84,    85,   180,    76,
      35,    36,   407,   185,   462,   187,    10,    11,    12,    86,
      87,    88,    81,    82,   194,     3,     4,     5,     6,    83,
      84,    85,    10,    11,    12,    13,    14,    15,    16,    14,
      18,    19,    20,    91,    92,    65,    66,    67,    68,    69,
      70,    91,    30,    93,    92,    75,    94,   229,    36,    79,
      80,    81,    82,    41,    42,    43,   238,    14,    46,   241,
      14,    92,   244,    94,   246,    10,   248,    91,    13,    14,
      15,    16,   254,   255,    91,    92,   256,   257,   258,    10,
     132,   133,    13,   263,    15,    16,    14,    10,    11,    12,
      78,    14,   272,   273,     7,     8,     9,    85,   136,   137,
     138,    92,    92,    94,    94,    93,   288,   289,   290,    97,
     121,   122,   123,   124,   125,   126,   127,   128,    90,   130,
     131,    14,   304,     3,     4,     5,     6,    92,    95,    94,
      10,    11,    12,    13,    14,    15,    16,    93,    18,    19,
      20,   323,   322,    92,   324,    94,    92,    77,    94,    92,
      30,    94,    92,    92,    94,    94,    36,    92,   338,    94,
      56,    41,    42,    43,    97,    92,    46,    94,    92,    91,
      94,    92,    77,    94,   356,    92,    92,    94,    94,    77,
      92,    92,    94,    94,    92,    91,    94,    91,    92,   369,
     370,   371,    91,    92,    91,    45,    91,   377,    78,    14,
      14,   381,    94,    97,    91,    85,    97,    96,    80,    91,
      98,    14,   392,    93,    94,    93,    93,    10,    94,    94,
      14,    94,    14,    94,    98,    93,    93,    14,    98,    77,
      77,    77,    91,    91,    96,    80,    80,    98,    98,    80,
      14,    98,   424,    94,    25,   427,    10,    10,    91,    91,
      10,    91,    91,    91,    10,    91,    91,    94,   440,    25,
      48,    14,    14,    14,    91,    91,   446,    91,   448,    91,
      91,    14,    91,   455,    91,    98,   458,    93,    14,    91,
      93,    91,     7,     7,     7,    61,    14,   467,    10,   469,
      10,    91,    91,    14,     3,     4,     5,     6,   478,    14,
      91,    10,    11,    12,    13,    14,    15,    16,   488,    18,
      19,    20,    21,    22,    23,    24,    93,    14,    27,    28,
      29,    30,    31,    32,    33,    94,    91,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    91,    46,    47,   277,
     369,    91,    51,    52,    91,    91,    91,    91,    57,    91,
      91,    -1,   281,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    78,
      18,    19,    20,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    93,    -1,    -1,    -1,    36,    98,
      -1,    -1,    -1,    41,    42,    43,    -1,    -1,    46,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      78,    -1,    -1,    36,    -1,    -1,    -1,    85,    41,    42,
      43,    -1,    -1,    46,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    30,    85,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      93,    94,    41,    42,    43,    -1,    -1,    46,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    78,
      -1,    -1,    36,    -1,    -1,    -1,    85,    41,    42,    43,
      -1,    -1,    46,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      30,    85,    -1,    -1,    -1,    -1,    36,    -1,    -1,    93,
      94,    41,    42,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,     3,     4,
       5,     6,    -1,    93,    94,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    -1,    -1,    -1,    51,    52,    -1,    -1,
      -1,    -1,    57,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    30,    -1,    -1,    -1,    -1,    -1,    36,    93,    -1,
      -1,    -1,    41,    42,    43,    -1,    -1,    46,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    78,
      -1,    -1,    -1,    36,    -1,    -1,    85,    -1,    41,    42,
      43,    -1,    91,    46,    93,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    78,    -1,    -1,    36,    -1,
      -1,    -1,    85,    41,    42,    43,    -1,    -1,    46,    -1,
      93,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      78,    -1,    -1,    -1,    36,    -1,    -1,    85,    -1,    41,
      42,    43,    -1,    -1,    46,    93,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    78,    -1,    -1,    36,
      -1,    -1,    -1,    85,    41,    42,    43,    -1,    -1,    46,
      -1,    93,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    78,    -1,    -1,    -1,    36,    -1,    -1,    85,    -1,
      41,    42,    43,    -1,    -1,    46,    93,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      36,    -1,    -1,    -1,    85,    41,    42,    43,    -1,    -1,
      46,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   100,   101,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    27,    28,    29,    30,    31,    32,    33,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    46,    47,
      51,    52,    57,    78,    85,    93,   102,   103,   104,   105,
     106,   107,   108,   109,   111,   112,   113,   116,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   132,    74,    77,
      90,    95,    79,    97,    77,    90,    93,    10,    11,    12,
      14,    10,    11,    12,    13,    93,   122,    93,   122,    93,
      93,   122,   126,    14,    97,   120,   122,    91,   122,   122,
      12,    12,    93,    93,   120,    97,   121,   121,   121,   122,
     120,   122,    14,    14,    29,    14,    10,   128,   128,   122,
      91,    71,    72,    65,    66,    67,    68,    69,    70,    75,
      79,    80,    81,    82,    62,    63,    83,    84,    85,    76,
      86,    87,    88,    90,    14,   122,    14,   122,    14,   122,
     122,    14,    17,    50,    94,   122,   131,    77,    91,    77,
      77,    10,    11,    12,    77,    95,    79,    97,   122,    91,
     122,    91,   122,   122,    64,   120,    62,    63,    93,    56,
     101,    91,    91,    97,    79,    97,    79,    97,   122,   122,
     122,   122,   122,   122,    45,   114,    97,    14,    97,    94,
     124,   124,   124,   124,   124,   124,   124,   124,     7,     8,
       9,   124,   124,   125,   125,   126,   126,   127,   127,   127,
     128,   128,   128,   128,    14,    17,    91,    93,    91,    77,
      93,    96,    80,    98,    91,    93,    14,    93,    92,    94,
     122,    93,   122,   122,    77,    91,    77,    91,    77,    91,
     122,   122,    14,   122,    92,    92,    94,    94,    10,   126,
     126,    10,    14,    94,   119,    98,    98,   110,    14,   122,
      14,   122,    94,    94,    98,   120,    14,    97,   115,    93,
     117,    93,    14,    94,   131,   122,    94,   131,    77,    77,
      77,    94,   131,    14,    94,   118,   131,   122,    91,   122,
      91,    92,    91,   122,    93,   122,   122,    96,    80,    98,
     122,   122,   120,   120,   120,    64,    64,    10,    54,    55,
     120,    92,    94,    34,    35,    98,    80,    98,    80,    98,
     120,   120,    14,   115,    48,    49,    52,    98,    94,   119,
      58,    59,    60,    98,    94,   118,   131,    91,    94,    91,
      91,    94,   122,   122,   122,    94,    61,    92,    94,    94,
      92,   131,    91,   122,    91,    92,    91,    94,    94,    25,
      10,    10,    10,    10,    10,    14,   120,   122,   120,    91,
      91,    25,    98,    14,    50,    10,    13,    14,    15,    16,
      48,   120,    94,    14,    14,    14,    94,    94,    91,    91,
      91,    91,    91,   122,    14,   131,    91,    92,   131,    91,
      91,   103,   120,   120,   120,    10,    54,    55,   120,    98,
     120,    93,    93,    14,    77,    91,    14,    77,    91,    10,
      13,    15,    16,    91,    91,    14,   120,     7,     7,     7,
      61,    94,   131,    91,    10,    10,    94,   119,    94,   119,
      14,   122,    14,   122,    14,    77,    91,    14,    77,    91,
      91,    91,    93,   122,    91,    94,   120,    94,   120,    94,
      91,    91,    91,    91,    14,   122,    14,   122,    94,   119,
      91,   120,   120,    91,    91,    91,    91,   120,    94,   120
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    99,   100,   101,   101,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   103,   103,   103,   104,   105,   105,   106,
     107,   108,   108,   108,   108,   109,   109,   110,   110,   111,
     111,   111,   111,   112,   113,   114,   114,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   117,   117,   117,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   120,   121,   122,   122,
     122,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   124,   124,   124,   125,   125,
     125,   126,   126,   126,   126,   127,   127,   127,   127,   127,
     128,   128,   128,   129,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132
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
       5,     7,     6,     6,     5,     0,     3,     0,     4,     4,
       4,     5,     5,     4,     5,     5,     6,     6,     7,     7,
       6,     6,     7,     7,     7,     6,     7,     6,     8,     7,
       5,     0,     4,     4,     4,     3,     5,     1,     2,     3,
       3,     3,     4,     5,     5,     3,     3,     3,     3,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     3,     3,     3,     3,     1,
       2,     2,     1,     1,     4,     3,     6,     6,     5,     6,
       5,     4,     4,     6,     6,     5,     3,     2,     3,     3,
       2,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     5,     5,
       1,     1,     1,     1,     3
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
#line 1827 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 236 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1833 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 237 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1839 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 241 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1845 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 243 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 1851 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 245 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1857 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 247 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 1865 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 251 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 1873 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 255 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 1879 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 257 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1885 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 259 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1891 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 261 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1899 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 265 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 1907 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 269 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 1913 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 271 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1919 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 273 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1925 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 275 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 1931 "raku.tab.c"
    break;

  case 19: /* stmt: KW_SAY expr ';'  */
#line 277 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1937 "raku.tab.c"
    break;

  case 20: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 279 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1943 "raku.tab.c"
    break;

  case 21: /* stmt: KW_PRINT expr ';'  */
#line 281 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 1949 "raku.tab.c"
    break;

  case 22: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 283 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 1955 "raku.tab.c"
    break;

  case 23: /* stmt: KW_TAKE expr ';'  */
#line 285 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 1961 "raku.tab.c"
    break;

  case 24: /* stmt: KW_RETURN expr ';'  */
#line 287 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 1967 "raku.tab.c"
    break;

  case 25: /* stmt: KW_RETURN ';'  */
#line 289 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 1973 "raku.tab.c"
    break;

  case 26: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 291 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 1979 "raku.tab.c"
    break;

  case 27: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 293 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 1988 "raku.tab.c"
    break;

  case 28: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 298 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 1996 "raku.tab.c"
    break;

  case 29: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 302 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2004 "raku.tab.c"
    break;

  case 30: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 306 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2013 "raku.tab.c"
    break;

  case 31: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 311 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2021 "raku.tab.c"
    break;

  case 32: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 315 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2032 "raku.tab.c"
    break;

  case 33: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 322 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2041 "raku.tab.c"
    break;

  case 34: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 327 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2048 "raku.tab.c"
    break;

  case 35: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 330 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2055 "raku.tab.c"
    break;

  case 36: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 333 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2062 "raku.tab.c"
    break;

  case 37: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 336 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2069 "raku.tab.c"
    break;

  case 38: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 339 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2076 "raku.tab.c"
    break;

  case 39: /* stmt: expr ';'  */
#line 341 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2082 "raku.tab.c"
    break;

  case 40: /* stmt: if_stmt  */
#line 342 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2088 "raku.tab.c"
    break;

  case 41: /* stmt: while_stmt  */
#line 343 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2094 "raku.tab.c"
    break;

  case 42: /* stmt: for_stmt  */
#line 344 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2100 "raku.tab.c"
    break;

  case 43: /* stmt: given_stmt  */
#line 345 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2106 "raku.tab.c"
    break;

  case 44: /* stmt: KW_TRY block  */
#line 347 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2112 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TRY block KW_CATCH block  */
#line 349 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2118 "raku.tab.c"
    break;

  case 46: /* stmt: unless_stmt  */
#line 350 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2124 "raku.tab.c"
    break;

  case 47: /* stmt: until_stmt  */
#line 351 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2130 "raku.tab.c"
    break;

  case 48: /* stmt: repeat_stmt  */
#line 352 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2136 "raku.tab.c"
    break;

  case 49: /* stmt: sub_decl  */
#line 353 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2142 "raku.tab.c"
    break;

  case 50: /* stmt: class_decl  */
#line 354 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2148 "raku.tab.c"
    break;

  case 51: /* stmt: role_decl  */
#line 355 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2154 "raku.tab.c"
    break;

  case 52: /* stmt: grammar_decl  */
#line 356 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2160 "raku.tab.c"
    break;

  case 53: /* if_stmt: KW_IF '(' expr ')' block  */
#line 360 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2166 "raku.tab.c"
    break;

  case 54: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 362 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2172 "raku.tab.c"
    break;

  case 55: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 364 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2178 "raku.tab.c"
    break;

  case 56: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 368 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2184 "raku.tab.c"
    break;

  case 57: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 372 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2190 "raku.tab.c"
    break;

  case 58: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 374 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2196 "raku.tab.c"
    break;

  case 59: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 378 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2202 "raku.tab.c"
    break;

  case 60: /* repeat_stmt: KW_REPEAT block  */
#line 382 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2208 "raku.tab.c"
    break;

  case 61: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 386 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2218 "raku.tab.c"
    break;

  case 62: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 392 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2228 "raku.tab.c"
    break;

  case 63: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 398 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2237 "raku.tab.c"
    break;

  case 64: /* for_stmt: KW_FOR expr block  */
#line 403 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2244 "raku.tab.c"
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
#line 2256 "raku.tab.c"
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
#line 2269 "raku.tab.c"
    break;

  case 67: /* when_list: %empty  */
#line 426 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2275 "raku.tab.c"
    break;

  case 68: /* when_list: when_list KW_WHEN expr block  */
#line 428 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2283 "raku.tab.c"
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
#line 2295 "raku.tab.c"
    break;

  case 70: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 442 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2305 "raku.tab.c"
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
#line 2318 "raku.tab.c"
    break;

  case 72: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 457 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2329 "raku.tab.c"
    break;

  case 73: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 466 "raku.y"
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
#line 2347 "raku.tab.c"
    break;

  case 74: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 482 "raku.y"
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
#line 2364 "raku.tab.c"
    break;

  case 75: /* is_clauses: %empty  */
#line 496 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2370 "raku.tab.c"
    break;

  case 76: /* is_clauses: is_clauses IDENT IDENT  */
#line 498 "raku.y"
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
#line 2386 "raku.tab.c"
    break;

  case 77: /* class_body_list: %empty  */
#line 511 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2392 "raku.tab.c"
    break;

  case 78: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 513 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2399 "raku.tab.c"
    break;

  case 79: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 516 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2406 "raku.tab.c"
    break;

  case 80: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 519 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2413 "raku.tab.c"
    break;

  case 81: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 522 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2420 "raku.tab.c"
    break;

  case 82: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 525 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2427 "raku.tab.c"
    break;

  case 83: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 528 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2434 "raku.tab.c"
    break;

  case 84: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 531 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2441 "raku.tab.c"
    break;

  case 85: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 534 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2448 "raku.tab.c"
    break;

  case 86: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 537 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2455 "raku.tab.c"
    break;

  case 87: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 540 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2462 "raku.tab.c"
    break;

  case 88: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 543 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2469 "raku.tab.c"
    break;

  case 89: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 546 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2476 "raku.tab.c"
    break;

  case 90: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 549 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2487 "raku.tab.c"
    break;

  case 91: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 556 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2498 "raku.tab.c"
    break;

  case 92: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 563 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2509 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 570 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2520 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 577 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2534 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 587 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2546 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 595 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2559 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 604 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2570 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 611 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2584 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 621 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2596 "raku.tab.c"
    break;

  case 100: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 631 "raku.y"
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
#line 2613 "raku.tab.c"
    break;

  case 101: /* grammar_body_list: %empty  */
#line 645 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2619 "raku.tab.c"
    break;

  case 102: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 647 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2628 "raku.tab.c"
    break;

  case 103: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 652 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2637 "raku.tab.c"
    break;

  case 104: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 657 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2646 "raku.tab.c"
    break;

  case 105: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 664 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2654 "raku.tab.c"
    break;

  case 106: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 668 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 2662 "raku.tab.c"
    break;

  case 107: /* param_list: VAR_SCALAR  */
#line 673 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 2668 "raku.tab.c"
    break;

  case 108: /* param_list: IDENT VAR_SCALAR  */
#line 674 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2674 "raku.tab.c"
    break;

  case 109: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 675 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2680 "raku.tab.c"
    break;

  case 110: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 676 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2686 "raku.tab.c"
    break;

  case 111: /* param_list: param_list ',' VAR_SCALAR  */
#line 677 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 2692 "raku.tab.c"
    break;

  case 112: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 678 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 2698 "raku.tab.c"
    break;

  case 113: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 679 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2704 "raku.tab.c"
    break;

  case 114: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 680 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 2710 "raku.tab.c"
    break;

  case 115: /* block: '{' stmt_list '}'  */
#line 683 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2716 "raku.tab.c"
    break;

  case 116: /* block: '{' YADA '}'  */
#line 684 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2722 "raku.tab.c"
    break;

  case 117: /* closure: '{' expr '}'  */
#line 687 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 2728 "raku.tab.c"
    break;

  case 118: /* expr: VAR_SCALAR '=' expr  */
#line 690 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 2734 "raku.tab.c"
    break;

  case 119: /* expr: KW_GATHER block  */
#line 691 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 2744 "raku.tab.c"
    break;

  case 120: /* expr: cmp_expr  */
#line 696 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 2750 "raku.tab.c"
    break;

  case 121: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 699 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2756 "raku.tab.c"
    break;

  case 122: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 700 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2762 "raku.tab.c"
    break;

  case 123: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 701 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2768 "raku.tab.c"
    break;

  case 124: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 702 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2774 "raku.tab.c"
    break;

  case 125: /* cmp_expr: jct_expr '<' jct_expr  */
#line 703 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2780 "raku.tab.c"
    break;

  case 126: /* cmp_expr: jct_expr '>' jct_expr  */
#line 704 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2786 "raku.tab.c"
    break;

  case 127: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 705 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2792 "raku.tab.c"
    break;

  case 128: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 706 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2798 "raku.tab.c"
    break;

  case 129: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 707 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2804 "raku.tab.c"
    break;

  case 130: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 708 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2810 "raku.tab.c"
    break;

  case 131: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 710 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 2820 "raku.tab.c"
    break;

  case 132: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 716 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 2830 "raku.tab.c"
    break;

  case 133: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 722 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 2840 "raku.tab.c"
    break;

  case 134: /* cmp_expr: jct_expr  */
#line 727 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2846 "raku.tab.c"
    break;

  case 135: /* jct_expr: jct_expr '|' range_expr  */
#line 730 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2852 "raku.tab.c"
    break;

  case 136: /* jct_expr: jct_expr '&' range_expr  */
#line 731 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2858 "raku.tab.c"
    break;

  case 137: /* jct_expr: range_expr  */
#line 732 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 2864 "raku.tab.c"
    break;

  case 138: /* range_expr: add_expr OP_RANGE add_expr  */
#line 735 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2870 "raku.tab.c"
    break;

  case 139: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 736 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2876 "raku.tab.c"
    break;

  case 140: /* range_expr: add_expr  */
#line 737 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 2882 "raku.tab.c"
    break;

  case 141: /* add_expr: add_expr '+' mul_expr  */
#line 740 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2888 "raku.tab.c"
    break;

  case 142: /* add_expr: add_expr '-' mul_expr  */
#line 741 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2894 "raku.tab.c"
    break;

  case 143: /* add_expr: add_expr '~' mul_expr  */
#line 742 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2900 "raku.tab.c"
    break;

  case 144: /* add_expr: mul_expr  */
#line 743 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 2906 "raku.tab.c"
    break;

  case 145: /* mul_expr: mul_expr '*' unary_expr  */
#line 746 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2912 "raku.tab.c"
    break;

  case 146: /* mul_expr: mul_expr '/' unary_expr  */
#line 747 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2918 "raku.tab.c"
    break;

  case 147: /* mul_expr: mul_expr '%' unary_expr  */
#line 748 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2924 "raku.tab.c"
    break;

  case 148: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 749 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2930 "raku.tab.c"
    break;

  case 149: /* mul_expr: unary_expr  */
#line 750 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 2936 "raku.tab.c"
    break;

  case 150: /* unary_expr: '-' unary_expr  */
#line 753 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 2942 "raku.tab.c"
    break;

  case 151: /* unary_expr: '!' unary_expr  */
#line 754 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 2948 "raku.tab.c"
    break;

  case 152: /* unary_expr: postfix_expr  */
#line 755 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 2954 "raku.tab.c"
    break;

  case 153: /* postfix_expr: call_expr  */
#line 757 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 2960 "raku.tab.c"
    break;

  case 154: /* call_expr: IDENT '(' arg_list ')'  */
#line 760 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 2969 "raku.tab.c"
    break;

  case 155: /* call_expr: IDENT '(' ')'  */
#line 764 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 2975 "raku.tab.c"
    break;

  case 156: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 766 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 2985 "raku.tab.c"
    break;

  case 157: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 772 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2995 "raku.tab.c"
    break;

  case 158: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 778 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3003 "raku.tab.c"
    break;

  case 159: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 782 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3014 "raku.tab.c"
    break;

  case 160: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 789 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3023 "raku.tab.c"
    break;

  case 161: /* call_expr: IDENT '.' CARET IDENT  */
#line 794 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3033 "raku.tab.c"
    break;

  case 162: /* call_expr: atom '.' CARET IDENT  */
#line 800 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3043 "raku.tab.c"
    break;

  case 163: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 806 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3054 "raku.tab.c"
    break;

  case 164: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 813 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3065 "raku.tab.c"
    break;

  case 165: /* call_expr: atom '.' IDENT '(' ')'  */
#line 820 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3074 "raku.tab.c"
    break;

  case 166: /* call_expr: atom '.' IDENT  */
#line 825 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3083 "raku.tab.c"
    break;

  case 167: /* call_expr: KW_DIE expr  */
#line 830 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3089 "raku.tab.c"
    break;

  case 168: /* call_expr: KW_MAP closure expr  */
#line 832 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3095 "raku.tab.c"
    break;

  case 169: /* call_expr: KW_GREP closure expr  */
#line 834 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3101 "raku.tab.c"
    break;

  case 170: /* call_expr: KW_SORT expr  */
#line 836 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3107 "raku.tab.c"
    break;

  case 171: /* call_expr: KW_SORT closure expr  */
#line 838 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3113 "raku.tab.c"
    break;

  case 172: /* call_expr: atom  */
#line 839 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3119 "raku.tab.c"
    break;

  case 173: /* arg_list: expr  */
#line 842 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3125 "raku.tab.c"
    break;

  case 174: /* arg_list: arg_list ',' expr  */
#line 843 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3131 "raku.tab.c"
    break;

  case 175: /* atom: LIT_INT  */
#line 846 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3137 "raku.tab.c"
    break;

  case 176: /* atom: LIT_FLOAT  */
#line 847 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3143 "raku.tab.c"
    break;

  case 177: /* atom: LIT_STR  */
#line 848 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3149 "raku.tab.c"
    break;

  case 178: /* atom: LIT_INTERP_STR  */
#line 849 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3155 "raku.tab.c"
    break;

  case 179: /* atom: VAR_SCALAR  */
#line 850 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3161 "raku.tab.c"
    break;

  case 180: /* atom: VAR_ARRAY  */
#line 851 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3167 "raku.tab.c"
    break;

  case 181: /* atom: VAR_HASH  */
#line 852 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3173 "raku.tab.c"
    break;

  case 182: /* atom: VAR_CAPTURE  */
#line 854 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3181 "raku.tab.c"
    break;

  case 183: /* atom: VAR_FH  */
#line 858 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3189 "raku.tab.c"
    break;

  case 184: /* atom: VAR_NAMED_CAPTURE  */
#line 862 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3196 "raku.tab.c"
    break;

  case 185: /* atom: VAR_ARRAY '[' expr ']'  */
#line 865 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3202 "raku.tab.c"
    break;

  case 186: /* atom: VAR_HASH '<' IDENT '>'  */
#line 867 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3208 "raku.tab.c"
    break;

  case 187: /* atom: VAR_HASH '{' expr '}'  */
#line 869 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3214 "raku.tab.c"
    break;

  case 188: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 871 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3220 "raku.tab.c"
    break;

  case 189: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 873 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3226 "raku.tab.c"
    break;

  case 190: /* atom: IDENT  */
#line 874 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3232 "raku.tab.c"
    break;

  case 191: /* atom: VAR_TWIGIL  */
#line 876 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3240 "raku.tab.c"
    break;

  case 192: /* atom: VAR_ARRAY_TWIGIL  */
#line 880 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3248 "raku.tab.c"
    break;

  case 193: /* atom: VAR_HASH_TWIGIL  */
#line 884 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3256 "raku.tab.c"
    break;

  case 194: /* atom: '(' expr ')'  */
#line 887 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3262 "raku.tab.c"
    break;


#line 3266 "raku.tab.c"

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

#line 889 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
