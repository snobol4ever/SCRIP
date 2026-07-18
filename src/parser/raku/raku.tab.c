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
static const char *testop_rt(const char *s) {
    if (!s) return "__rk_test_ok";
    if (!strcmp(s, "plan")) return "__rk_test_plan";
    if (!strcmp(s, "ok")) return "__rk_test_ok";
    if (!strcmp(s, "nok")) return "__rk_test_nok";
    if (!strcmp(s, "is")) return "__rk_test_is";
    if (!strcmp(s, "isnt")) return "__rk_test_isnt";
    if (!strcmp(s, "done-testing")) return "__rk_test_done";
    if (!strcmp(s, "skip-rest")) return "__rk_test_skip_rest";
    if (!strcmp(s, "skip")) return "__rk_test_skip";
    if (!strcmp(s, "todo")) return "__rk_test_todo";
    if (!strcmp(s, "diag")) return "__rk_test_diag";
    if (!strcmp(s, "pass")) return "__rk_test_pass";
    if (!strcmp(s, "flunk")) return "__rk_test_flunk";
    return "__rk_test_ok";
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

#line 248 "raku.tab.c"

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
  YYSYMBOL_KW_USE = 21,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 22,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 23,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 24,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 25,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 26,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 27,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 28,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 29,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 30,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 31,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 32,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 33,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 34,                 /* KW_RETURN  */
  YYSYMBOL_KW_GIVEN = 35,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 36,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 37,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 38,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 39,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 40,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 41,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 42,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 43,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 44,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 45,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 46,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 47,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 48,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 49,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 50,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 51,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 52,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 53,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 54,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 55,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 56,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 57,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 58,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 59,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 60,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 61,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 62,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 63,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 64,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 65,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 66,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 67,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 68,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 69,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 70,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 71,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 72,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 73,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 74,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 75,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 76,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 77,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 78,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 79,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 80,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 81,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 82,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 83,                    /* OP_DIV  */
  YYSYMBOL_84_ = 84,                       /* '='  */
  YYSYMBOL_85_ = 85,                       /* '!'  */
  YYSYMBOL_86_ = 86,                       /* '<'  */
  YYSYMBOL_87_ = 87,                       /* '>'  */
  YYSYMBOL_88_ = 88,                       /* '|'  */
  YYSYMBOL_89_ = 89,                       /* '&'  */
  YYSYMBOL_90_ = 90,                       /* '~'  */
  YYSYMBOL_91_ = 91,                       /* '+'  */
  YYSYMBOL_92_ = 92,                       /* '-'  */
  YYSYMBOL_93_ = 93,                       /* '*'  */
  YYSYMBOL_94_ = 94,                       /* '/'  */
  YYSYMBOL_95_ = 95,                       /* '%'  */
  YYSYMBOL_UMINUS = 96,                    /* UMINUS  */
  YYSYMBOL_97_ = 97,                       /* '.'  */
  YYSYMBOL_98_ = 98,                       /* ';'  */
  YYSYMBOL_99_ = 99,                       /* ','  */
  YYSYMBOL_100_ = 100,                     /* '('  */
  YYSYMBOL_101_ = 101,                     /* ')'  */
  YYSYMBOL_102_ = 102,                     /* '['  */
  YYSYMBOL_103_ = 103,                     /* ']'  */
  YYSYMBOL_104_ = 104,                     /* '{'  */
  YYSYMBOL_105_ = 105,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 106,                 /* $accept  */
  YYSYMBOL_program = 107,                  /* program  */
  YYSYMBOL_stmt_list = 108,                /* stmt_list  */
  YYSYMBOL_stmt = 109,                     /* stmt  */
  YYSYMBOL_if_stmt = 110,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 111,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 112,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 113,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 114,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 115,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 116,               /* given_stmt  */
  YYSYMBOL_when_list = 117,                /* when_list  */
  YYSYMBOL_sub_decl = 118,                 /* sub_decl  */
  YYSYMBOL_class_decl = 119,               /* class_decl  */
  YYSYMBOL_role_decl = 120,                /* role_decl  */
  YYSYMBOL_is_clauses = 121,               /* is_clauses  */
  YYSYMBOL_class_body_list = 122,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 123,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 124,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 125,           /* named_arg_list  */
  YYSYMBOL_pair_list = 126,                /* pair_list  */
  YYSYMBOL_param_list = 127,               /* param_list  */
  YYSYMBOL_block = 128,                    /* block  */
  YYSYMBOL_closure = 129,                  /* closure  */
  YYSYMBOL_expr = 130,                     /* expr  */
  YYSYMBOL_tern_expr = 131,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 132,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 133,                 /* jct_expr  */
  YYSYMBOL_range_expr = 134,               /* range_expr  */
  YYSYMBOL_add_expr = 135,                 /* add_expr  */
  YYSYMBOL_mul_expr = 136,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 137,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 138,             /* postfix_expr  */
  YYSYMBOL_call_expr = 139,                /* call_expr  */
  YYSYMBOL_arg_list = 140,                 /* arg_list  */
  YYSYMBOL_atom = 141                      /* atom  */
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
#define YYLAST   1715

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  106
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  228
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  575

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   339


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
       2,     2,     2,    85,     2,     2,     2,    95,    89,     2,
     100,   101,    93,    91,    99,    92,    97,    94,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    98,
      86,    84,    87,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   102,     2,   103,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   104,    88,   105,    90,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    96
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   247,   247,   258,   259,   262,   264,   266,   268,   270,
     272,   276,   280,   282,   284,   286,   288,   290,   294,   298,
     300,   302,   304,   306,   308,   310,   312,   314,   316,   318,
     321,   323,   325,   328,   330,   332,   334,   336,   338,   343,
     347,   351,   356,   360,   367,   372,   375,   378,   381,   384,
     387,   388,   389,   390,   391,   392,   394,   396,   398,   400,
     401,   402,   403,   404,   405,   406,   409,   411,   413,   417,
     421,   423,   427,   431,   435,   441,   447,   452,   457,   465,
     476,   477,   483,   491,   497,   506,   513,   524,   540,   555,
     556,   568,   582,   583,   586,   589,   592,   595,   598,   601,
     604,   607,   610,   613,   616,   619,   626,   633,   640,   647,
     651,   655,   659,   663,   670,   677,   684,   691,   701,   709,
     718,   725,   735,   745,   760,   761,   766,   771,   778,   782,
     788,   790,   792,   794,   798,   799,   800,   801,   802,   803,
     804,   805,   808,   809,   812,   815,   816,   821,   824,   826,
     829,   830,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   845,   851,   857,   860,   861,   862,   865,   866,   867,
     870,   871,   872,   873,   876,   877,   878,   879,   880,   883,
     884,   885,   887,   889,   894,   895,   900,   902,   908,   914,
     918,   925,   930,   936,   942,   949,   956,   961,   966,   968,
     970,   972,   974,   976,   979,   980,   983,   984,   985,   986,
     994,   995,   996,   997,   998,  1002,  1006,  1009,  1011,  1013,
    1015,  1017,  1019,  1020,  1024,  1028,  1032,  1033,  1034
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
  "VAR_FH", "VAR_NAMED_CAPTURE", "KW_USE", "TESTOP", "KW_MY", "KW_SAY",
  "KW_PRINT", "KW_IF", "KW_ELSE", "KW_ELSIF", "KW_WHILE", "KW_FOR",
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_GIVEN", "KW_WHEN",
  "KW_DEFAULT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL",
  "KW_REPEAT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_ROLE",
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND",
  "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_DIV", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('",
  "')'", "'['", "']'", "'{'", "'}'", "$accept", "program", "stmt_list",
  "stmt", "if_stmt", "while_stmt", "unless_stmt", "until_stmt",
  "repeat_stmt", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "class_decl", "role_decl", "is_clauses", "class_body_list",
  "grammar_decl", "grammar_body_list", "named_arg_list", "pair_list",
  "param_list", "block", "closure", "expr", "tern_expr", "cmp_expr",
  "jct_expr", "range_expr", "add_expr", "mul_expr", "unary_expr",
  "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-314)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-228)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -314,    87,   454,  -314,  -314,  -314,  -314,  -314,   108,   -60,
     -56,    14,    91,  -314,  -314,  -314,  -314,  -314,    97,   234,
     250,  1179,  1237,    28,    46,  1287,     8,    55,  1287,   634,
    1287,   166,   174,    69,   114,    55,   115,   115,  1345,    55,
      55,  1287,   220,   249,   236,  -314,   275,  1611,  1611,  1287,
     269,  -314,  -314,  -314,  -314,  -314,  -314,  -314,  -314,  -314,
    -314,  -314,  -314,  1600,   223,  -314,   165,   129,  -314,    82,
     175,  -314,  -314,  -314,   239,   335,  1287,   354,   685,  1287,
     368,  1287,  1287,    17,   736,   287,    54,   288,   -51,  -314,
      55,  -314,   787,  -314,  -314,   -37,   -35,    53,    84,   283,
    1287,   157,  1287,   177,  1287,  1287,   -28,    85,   297,  -314,
    -314,   315,  -314,   319,   291,   -29,    76,  1287,  1287,  -314,
    1287,  1287,  1287,   838,  1287,  -314,   371,  -314,  -314,  -314,
     316,    18,   317,   322,  -314,  -314,   318,   323,   571,  -314,
    1611,  1611,  1611,  1611,  1611,  1611,  1611,  1611,  1611,   292,
    1611,  1611,  1611,  1611,  1611,  1611,  1611,  1611,  1611,  1611,
    1611,  1611,  1611,   180,   173,   325,    58,  -314,   128,   321,
     342,   326,   332,   334,   418,   336,  -314,   203,  -314,  1287,
    1287,   421,  1287,   340,   318,   221,  -314,  1287,  1287,  -314,
    1395,  -314,  1453,  -314,    95,   111,   122,   224,  -314,  1287,
     228,  -314,  1287,   338,   339,   431,  -314,  1611,  1611,    -4,
    -314,  -314,  -314,   428,  1287,   429,  1287,   343,   344,   341,
    -314,  -314,  -314,    55,     6,  -314,   347,   350,  -314,  -314,
    -314,  -314,   300,   300,   372,   300,   300,   300,   300,   300,
     300,  -314,  -314,  -314,   300,   300,  -314,  -314,   216,   216,
     175,   175,   175,  -314,  -314,  -314,  -314,   353,   440,  -314,
     901,  -314,  1287,   951,  -314,   377,   378,   379,  -314,  1015,
    -314,  1065,  -314,  -314,   352,   369,   366,  -314,   383,  -314,
     384,  1287,   295,   424,   -20,  1503,   301,   393,  1287,  -314,
    1561,  -314,  1287,  -314,  1287,   303,  1287,   305,    55,    55,
      55,    49,    65,  -314,    42,    55,   242,   -10,   417,   400,
     419,   404,    55,    55,  -314,  -314,   500,   503,  -314,    16,
      -1,   214,    -9,  1611,  1129,  -314,   420,   243,   422,   423,
     246,  1287,  1287,  1287,  -314,   247,    -6,  -314,   252,   253,
    -314,  -314,  -314,  -314,  -314,   256,  -314,  1287,  1287,  1287,
     257,  -314,    45,  -314,   425,  1287,   307,   426,   427,  -314,
     432,  -314,   492,  -314,  -314,   512,   515,  -314,   516,   517,
    -314,   222,    55,  1287,    55,  -314,  -314,  -314,   434,   436,
     502,  -314,  -314,  -314,    39,    26,   299,   480,  -314,    55,
     260,   261,   521,   523,   524,  -314,  -314,  -314,   264,   265,
    -314,   442,  -314,  -314,   443,   445,   447,   449,  -314,  1287,
     534,  -314,  -314,  1287,   309,  -314,  -314,   451,   485,   486,
    -314,   268,  -314,  1287,  -314,   455,   457,     3,    55,    55,
    -314,  -314,  -314,    73,  -314,    55,   452,  -314,  -314,    55,
    -314,   456,   460,     1,    22,   197,   463,   464,   549,  -314,
      55,    55,   557,   558,   559,  -314,  -314,  -314,  -314,  -314,
    -314,  -314,  -314,   501,   272,  -314,  -314,  1287,  1287,  1287,
     311,  -314,  -314,  -314,  -314,  -314,  -314,  -314,   560,   562,
    -314,  -314,  -314,     2,     4,   554,   555,   481,  1287,  -314,
     566,   574,   509,  1287,  -314,    25,    29,   510,   525,  -314,
    -314,   498,  -314,  -314,  -314,  -314,  -314,  1287,   528,  -314,
    -314,   278,  -314,  -314,  -314,    55,   279,    55,   282,   529,
     530,  -314,   532,   533,   536,  -314,   537,   585,   607,   538,
    1287,  -314,   608,   618,   543,  1287,  -314,  -314,  -314,     7,
    -314,  -314,   544,  -314,    55,  -314,    55,  -314,  -314,  -314,
    -314,  -314,  -314,   545,   553,  -314,   561,   563,   564,  -314,
     569,    55,   285,  -314,  -314,  -314,  -314,  -314,  -314,  -314,
    -314,  -314,  -314,    55,  -314
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   206,   207,   208,   210,   211,   212,
     213,   223,   222,   224,   225,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,     0,     0,     0,     0,
       3,     4,    51,    52,    59,    60,    61,    53,    54,    62,
      63,    64,    65,    58,     0,   147,   149,   163,   166,   169,
     173,   178,   181,   182,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   211,   212,   213,   223,
       0,    24,     0,   227,   204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,   228,
     146,     0,    36,     0,     0,     0,     0,     0,     0,    73,
       0,     0,     0,     3,     0,   201,    55,    57,   198,    89,
       0,     0,     0,   211,   180,   179,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,    23,     0,
       0,     0,     0,     0,   204,     0,    27,     0,     0,     6,
       0,     7,     0,     8,     0,     0,     0,     0,    28,     0,
       0,    31,     0,     0,     0,     0,    77,     0,     0,     0,
      34,    35,    80,     0,     0,     0,     0,     0,     0,     0,
     199,   200,   202,     0,     0,    92,     0,     0,   124,   226,
     143,   142,   150,   151,     0,   152,   153,   156,   157,   158,
     159,   160,   161,   162,   154,   155,   164,   165,   167,   168,
     172,   170,   171,   177,   174,   175,   176,   197,     0,    40,
       0,    37,     0,     0,   185,   217,   218,   219,    42,     0,
     192,     0,   183,   145,     0,     0,     0,    26,     0,   205,
       0,     0,     0,   208,   222,     0,     0,     0,     0,    20,
       0,    21,     0,    22,     0,     0,     0,     0,     0,     0,
       0,   167,   168,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   144,    56,     0,     0,    92,     0,
       0,     0,     0,     0,     0,   193,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,   222,   189,     0,     0,
     217,   218,   219,    25,     5,     0,     9,     0,     0,     0,
       0,    13,     0,    12,     0,     0,     0,     0,     0,    29,
       0,    32,    66,    69,    76,     0,     0,   135,     0,     0,
      83,     0,     0,     0,     0,    78,   220,   221,     0,     0,
      70,    72,    90,    91,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,   123,   148,   196,     0,     0,
      39,     0,    41,    44,     0,     0,     0,     0,   190,     0,
       0,   187,   188,     0,     0,   131,   130,     0,     0,     0,
      15,     0,    16,     0,    19,     0,     0,     0,     0,     0,
     136,   137,   138,     0,    82,     0,     0,    48,    49,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,     0,   195,   194,    38,    43,    45,
      46,    47,   128,     0,     0,    10,    14,     0,     0,     0,
       0,    30,    33,    68,    67,    74,    75,   139,     0,     0,
      81,    79,    71,     0,     0,     0,     0,     0,     0,    98,
       0,     0,     0,     0,    93,     0,     0,     0,     0,    94,
      95,     0,    84,    86,   125,   126,   127,     0,     0,   133,
     132,     0,    17,   140,   141,     0,     0,     0,     0,     0,
       0,   109,     0,     0,     0,   110,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,    99,    96,    97,     0,
     129,    11,     0,   118,     0,   120,     0,   107,   108,   102,
     105,   106,   101,     0,     0,   111,     0,     0,     0,   112,
       0,     0,     0,    18,   117,   119,   115,   116,   104,   113,
     114,   103,   122,     0,   121
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -314,  -314,   655,  -314,   230,  -314,  -314,  -314,  -314,  -314,
    -314,  -314,  -314,  -314,  -314,  -314,   346,  -314,  -314,   345,
     373,  -313,    -2,   374,    43,  -138,  -314,   140,   262,   -24,
     160,   -45,  -314,  -314,   -73,  -314
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   138,    51,    52,    53,    54,    55,    56,    57,
      58,   307,    59,    60,    61,   224,   319,    62,   322,   338,
     286,   306,    93,   121,    94,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    95,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,   107,   134,   135,   234,   168,   303,   390,   391,   303,
     304,   177,   303,   304,   303,   485,   304,   303,   304,   185,
     316,   304,   108,   486,   109,   110,   373,   374,   317,    23,
      80,   173,   226,   119,   174,   181,   490,   126,   127,   527,
     441,   205,    79,   532,   491,    64,   349,   528,    81,   188,
     418,   533,   367,   182,   392,   393,   394,   213,   487,   419,
     409,   186,   187,   189,   101,   103,   385,   386,   106,   175,
     387,   111,   113,   114,   227,   214,    50,    83,   442,   492,
      84,   125,   529,   477,   128,   488,   534,     3,   109,   385,
     386,    83,   136,   387,    84,   375,   395,   305,    82,   489,
     389,   368,   369,   515,   206,   517,   493,    50,   561,   530,
     318,    85,    50,   535,   253,   254,   255,   256,   365,   165,
     494,   388,   169,   531,   171,   172,   295,   536,   104,   297,
     248,   249,   478,   479,   366,   184,    63,   190,   179,   156,
     157,   158,   262,   197,   440,   200,   105,   203,   204,   154,
     155,   191,   207,   208,    78,   156,   157,   158,   263,    50,
     217,   218,   215,   219,   220,   221,   219,   222,   192,   117,
     516,   518,   156,   157,   158,   156,   157,   158,   115,   288,
     216,    64,   193,   301,   302,   396,   116,   327,    83,    75,
     330,    84,    76,   289,   257,   290,   335,   258,   339,   143,
     144,   145,   146,   147,   148,    77,   292,   495,    78,   291,
     496,   149,   497,   498,   118,   150,   151,   152,   153,   120,
     293,   315,   273,   274,   303,   276,   562,   187,   304,   264,
     279,   280,   432,   282,   129,   287,   433,     4,     5,     6,
       7,   140,   141,   142,    86,    87,    88,    89,    12,    13,
      14,   399,    15,    16,    17,   198,   199,   309,   159,   311,
      96,    97,    98,   130,    99,    90,    27,   131,   160,   161,
     162,   259,    31,   260,   414,   201,   202,    36,    37,    38,
     232,   233,    41,   235,   236,   237,   238,   239,   240,   132,
     244,   245,    45,   194,   195,   196,   362,   363,   364,   241,
     242,   243,   187,   370,   272,   328,   156,   157,   158,   443,
     380,   381,   444,   445,   446,   447,   250,   251,   252,    47,
     187,   139,   278,   294,   345,   229,    48,   296,   136,   229,
     137,   354,    91,   356,    92,   357,   163,   358,    50,   360,
     464,   371,   187,   372,   401,   187,   187,   404,   408,   164,
     470,   410,   187,   411,   412,   413,   352,   229,   417,   371,
     371,   450,   451,   410,   187,   455,   456,   469,   166,   229,
     434,   187,   436,   508,   405,   406,   407,   187,   371,   542,
     544,   371,   170,   546,   371,   178,   573,   449,   152,   153,
     180,   415,   416,   346,   347,   212,   511,   209,   421,   351,
     352,   359,   187,   361,   187,   422,   423,   465,   187,   512,
     187,   122,   124,   210,   246,   247,   435,   211,   223,   229,
     225,   228,    78,   261,   265,   474,   475,   476,   230,   266,
     268,   267,   270,   480,   269,   275,   271,   482,   277,   298,
     299,   300,   308,   310,   312,   313,   314,   320,   502,   503,
     321,   323,   462,   324,   325,   340,   341,     4,     5,     6,
       7,   331,   332,   333,     8,     9,    10,    11,    12,    13,
      14,   342,    15,    16,    17,    18,    19,    20,    21,    22,
      23,   343,   344,    24,    25,    26,    27,    28,    29,    30,
     348,   353,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,   376,   377,   378,    43,    44,   379,
     509,   510,    45,   543,   382,   545,    46,   383,   400,   427,
     402,   403,   428,   420,   424,   429,   430,   431,   425,   439,
     448,   522,   437,   426,   438,   452,   526,   453,   454,    47,
     457,   458,   564,   459,   565,   460,    48,   461,   463,   466,
     540,   467,   468,   471,    49,   472,   483,   481,    50,   572,
     484,   499,   500,   501,   504,   505,   506,   507,   519,   520,
     513,   574,   514,   556,     4,     5,     6,     7,   560,   521,
     523,     8,     9,    10,    11,    12,    13,    14,   524,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   539,   553,
      24,    25,    26,    27,    28,    29,    30,   525,   537,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,   554,   557,   538,    43,    44,   541,   547,   548,    45,
     549,   550,   558,    46,   551,   552,   555,     4,     5,     6,
       7,   559,   563,   566,    86,    87,    88,    89,    12,    13,
      14,   567,    15,    16,    17,     2,    47,   473,   350,   568,
       0,   569,   570,    48,   384,    90,    27,   571,     0,   398,
       0,    49,    31,     0,     0,    50,   231,    36,    37,    38,
       0,     0,    41,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    45,     0,     0,    86,    87,    88,    89,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    27,     0,    47,
       0,     0,     0,    31,     0,     0,    48,     0,    36,    37,
      38,     0,   112,    41,    49,     0,     0,     0,    50,     4,
       5,     6,     7,    45,     0,     0,    86,    87,    88,    89,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    27,     0,
      47,     0,     0,     0,    31,     0,     0,    48,     0,    36,
      37,    38,     0,     0,    41,    49,   167,     0,     0,    50,
       4,     5,     6,     7,    45,     0,     0,    86,    87,    88,
      89,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    27,
       0,    47,     0,     0,     0,    31,     0,     0,    48,     0,
      36,    37,    38,     0,     0,    41,    49,   176,     0,     0,
      50,     4,     5,     6,     7,    45,     0,     0,    86,    87,
      88,    89,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      27,     0,    47,     0,     0,     0,    31,     0,     0,    48,
       0,    36,    37,    38,     0,     0,    41,    49,   183,     0,
       0,    50,     0,     0,     0,     0,    45,     0,     0,   137,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    86,    87,    88,    89,    12,    13,    14,     0,    15,
      16,    17,     0,    47,     0,     0,     0,     0,     0,     0,
      48,     0,    90,    27,     0,     0,     0,     0,    49,    31,
       0,     0,    50,     0,    36,    37,    38,     0,     0,    41,
       0,     0,     0,     0,     4,     5,     6,     7,     0,    45,
       0,    86,    87,    88,    89,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    27,     0,     0,    47,     0,     0,    31,
       0,     0,     0,    48,    36,    37,    38,     0,     0,    41,
       0,    49,   326,     0,     0,    50,     0,     0,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    86,    87,    88,    89,    12,
      13,    14,     0,    15,    16,    17,    47,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    90,    27,     0,     0,
       0,    49,   329,    31,     0,    50,     0,     0,    36,    37,
      38,     0,     0,    41,     0,     0,     0,     0,     4,     5,
       6,     7,     0,    45,     0,    86,    87,    88,    89,   336,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    27,     0,     0,
      47,     0,     0,    31,     0,     0,     0,    48,    36,    37,
      38,     0,     0,    41,     0,    49,   334,     0,     0,    50,
       0,     0,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    86,
      87,    88,    89,   336,    13,    14,     0,    15,    16,    17,
      47,     0,     0,     0,     0,     0,     0,    48,     0,     0,
      90,    27,     0,     0,     0,    49,   337,    31,     0,    50,
       0,     0,    36,    37,    38,     0,     0,    41,     0,     0,
       0,     0,     4,     5,     6,     7,     0,    45,     0,    86,
      87,    88,    89,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    27,     0,     0,    47,     0,     0,    31,     0,     0,
       0,    48,    36,    37,    38,     0,     0,    41,     0,    49,
     397,     0,     0,    50,     0,     0,     0,    45,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    86,    87,    88,
      89,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,    47,     0,     0,     0,    90,    27,
       0,    48,     0,     0,     0,    31,     0,     0,     0,   100,
      36,    37,    38,    50,     0,    41,     0,     0,     0,     0,
       4,     5,     6,     7,     0,    45,     0,    86,    87,    88,
      89,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    27,
       0,     0,    47,     0,     0,    31,     0,     0,     0,    48,
      36,    37,    38,     0,     0,    41,     0,   102,     0,     0,
       0,    50,     0,     0,     0,    45,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    86,    87,    88,    89,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,    47,     0,     0,     0,    90,    27,     0,    48,
       0,     0,     0,    31,     0,     0,     0,    49,    36,    37,
      38,    50,     0,    41,     0,     0,     0,     0,     4,     5,
       6,     7,     0,    45,     0,    86,    87,    88,    89,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    27,     0,     0,
      47,     0,     0,    31,     0,     0,     0,    48,    36,    37,
      38,     0,     0,    41,     0,    49,     0,     0,     0,   123,
       0,     0,     0,    45,     0,     0,     4,     5,   283,     7,
       0,     0,     0,    86,    87,    88,    89,   284,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
      47,     0,     0,     0,    90,    27,     0,    48,     0,     0,
       0,    31,     0,     0,     0,   281,    36,    37,    38,    50,
       0,    41,     0,     0,     0,     0,     4,     5,   283,     7,
       0,    45,     0,    86,    87,    88,    89,   284,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    27,     0,     0,    47,     0,
       0,    31,     0,     0,     0,    48,    36,    37,    38,     0,
       0,    41,     0,   285,     0,     0,     0,    50,     0,     0,
       0,    45,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    86,    87,    88,    89,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,    47,     0,
       0,     0,    90,    27,     0,    48,     0,     0,     0,    31,
       0,     0,     0,    49,    36,    37,    38,    50,     0,    41,
       0,     0,     0,     0,     4,     5,     6,     7,     0,    45,
       0,   133,    87,    88,    89,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,     0,     0,     0,    47,     0,     0,    31,
       0,     0,     0,    48,    36,    37,    38,     0,     0,    41,
       0,   355,     0,     0,     0,    50,     0,  -227,  -227,    45,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,     0,
       0,     0,  -227,  -227,     0,     0,  -227,  -227,  -227,  -227,
    -227,  -227,     0,  -227,  -227,  -227,    47,  -227,  -227,     0,
       0,     0,     0,    48,     0,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,    50
};

static const yytype_int16 yycheck[] =
{
       2,    25,    47,    48,   142,    78,    10,   320,   321,    10,
      14,    84,    10,    14,    10,    14,    14,    10,    14,    92,
      14,    14,    14,    22,    26,    27,    36,    37,    22,    26,
      86,    14,    14,    35,    17,    86,    14,    39,    40,    14,
      14,    69,   102,    14,    22,     2,    66,    22,   104,    84,
       5,    22,    10,   104,    63,    64,    65,    86,    57,    14,
      66,    98,    99,    98,    21,    22,    50,    51,    25,    52,
      54,    28,    29,    30,    56,   104,   104,    97,    52,    57,
     100,    38,    57,    10,    41,    84,    57,     0,    90,    50,
      51,    97,    49,    54,   100,   105,   105,   101,    84,    98,
     101,    59,    60,   101,   106,   101,    84,   104,   101,    84,
     104,    14,   104,    84,   159,   160,   161,   162,    69,    76,
      98,   105,    79,    98,    81,    82,   199,    98,   100,   202,
     154,   155,    59,    60,    69,    92,   138,    84,    84,    90,
      91,    92,    84,   100,   105,   102,   100,   104,   105,    67,
      68,    98,    67,    68,   100,    90,    91,    92,   100,   104,
     117,   118,    86,   120,   121,   122,   123,   124,    84,   100,
     483,   484,    90,    91,    92,    90,    91,    92,    12,    84,
     104,   138,    98,   207,   208,   323,    12,   260,    97,    81,
     263,   100,    84,    98,    14,    84,   269,    17,   271,    70,
      71,    72,    73,    74,    75,    97,    84,    10,   100,    98,
      13,    82,    15,    16,   100,    86,    87,    88,    89,   104,
      98,   223,   179,   180,    10,   182,   539,    99,    14,   101,
     187,   188,    10,   190,    14,   192,    14,     3,     4,     5,
       6,    76,    77,    78,    10,    11,    12,    13,    14,    15,
      16,   324,    18,    19,    20,    98,    99,   214,    83,   216,
      10,    11,    12,    14,    14,    31,    32,    31,    93,    94,
      95,    98,    38,   100,   347,    98,    99,    43,    44,    45,
     140,   141,    48,   143,   144,   145,   146,   147,   148,    14,
     150,   151,    58,    10,    11,    12,   298,   299,   300,     7,
       8,     9,    99,   305,   101,   262,    90,    91,    92,    10,
     312,   313,    13,    14,    15,    16,   156,   157,   158,    85,
      99,    98,   101,    99,   281,   101,    92,    99,   285,   101,
      61,   288,    98,   290,   100,   292,    97,   294,   104,   296,
     413,    99,    99,   101,   101,    99,    99,   101,   101,    14,
     423,    99,    99,   101,   101,    99,    99,   101,   101,    99,
      99,   101,   101,    99,    99,   101,   101,    99,    14,   101,
     372,    99,   374,   101,   331,   332,   333,    99,    99,   101,
     101,    99,    14,   101,    99,    98,   101,   389,    88,    89,
     102,   348,   349,    98,    99,   104,   469,   100,   355,    98,
      99,    98,    99,    98,    99,    98,    99,    98,    99,    98,
      99,    37,    38,    98,   152,   153,   373,    98,    47,   101,
     104,   104,   100,    98,   103,   427,   428,   429,   105,    87,
      98,   105,    14,   435,   100,    14,   100,   439,    98,   101,
     101,    10,    14,    14,   101,   101,   105,   100,   450,   451,
     100,    79,   409,   100,    14,   103,    87,     3,     4,     5,
       6,    84,    84,    84,    10,    11,    12,    13,    14,    15,
      16,   105,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    98,    98,    29,    30,    31,    32,    33,    34,    35,
      66,    98,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    87,   105,    87,    53,    54,   105,
     467,   468,    58,   515,    14,   517,    62,    14,    98,    27,
      98,    98,    10,    98,    98,    10,    10,    10,   101,    27,
      50,   488,    98,   101,    98,    14,   493,    14,    14,    85,
      98,    98,   544,    98,   546,    98,    92,    98,    14,    98,
     507,    66,    66,    98,   100,    98,   100,   105,   104,   561,
     100,    98,    98,    14,     7,     7,     7,    66,    14,    14,
      10,   573,    10,   530,     3,     4,     5,     6,   535,    98,
      14,    10,    11,    12,    13,    14,    15,    16,    14,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   100,    14,
      29,    30,    31,    32,    33,    34,    35,    98,    98,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    14,    14,    98,    53,    54,    98,    98,    98,    58,
      98,    98,    14,    62,    98,    98,    98,     3,     4,     5,
       6,    98,    98,    98,    10,    11,    12,    13,    14,    15,
      16,    98,    18,    19,    20,     0,    85,   427,   285,    98,
      -1,    98,    98,    92,   318,    31,    32,    98,    -1,   324,
      -1,   100,    38,    -1,    -1,   104,   105,    43,    44,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    58,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    85,
      -1,    -1,    -1,    38,    -1,    -1,    92,    -1,    43,    44,
      45,    -1,    98,    48,   100,    -1,    -1,    -1,   104,     3,
       4,     5,     6,    58,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      85,    -1,    -1,    -1,    38,    -1,    -1,    92,    -1,    43,
      44,    45,    -1,    -1,    48,   100,   101,    -1,    -1,   104,
       3,     4,     5,     6,    58,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    85,    -1,    -1,    -1,    38,    -1,    -1,    92,    -1,
      43,    44,    45,    -1,    -1,    48,   100,   101,    -1,    -1,
     104,     3,     4,     5,     6,    58,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    85,    -1,    -1,    -1,    38,    -1,    -1,    92,
      -1,    43,    44,    45,    -1,    -1,    48,   100,   101,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    58,    -1,    -1,    61,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    31,    32,    -1,    -1,    -1,    -1,   100,    38,
      -1,    -1,   104,    -1,    43,    44,    45,    -1,    -1,    48,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    58,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    85,    -1,    -1,    38,
      -1,    -1,    -1,    92,    43,    44,    45,    -1,    -1,    48,
      -1,   100,   101,    -1,    -1,   104,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    31,    32,    -1,    -1,
      -1,   100,   101,    38,    -1,   104,    -1,    -1,    43,    44,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    58,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      85,    -1,    -1,    38,    -1,    -1,    -1,    92,    43,    44,
      45,    -1,    -1,    48,    -1,   100,   101,    -1,    -1,   104,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      31,    32,    -1,    -1,    -1,   100,   101,    38,    -1,   104,
      -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    58,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    85,    -1,    -1,    38,    -1,    -1,
      -1,    92,    43,    44,    45,    -1,    -1,    48,    -1,   100,
     101,    -1,    -1,   104,    -1,    -1,    -1,    58,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    31,    32,
      -1,    92,    -1,    -1,    -1,    38,    -1,    -1,    -1,   100,
      43,    44,    45,   104,    -1,    48,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    58,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    85,    -1,    -1,    38,    -1,    -1,    -1,    92,
      43,    44,    45,    -1,    -1,    48,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    58,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    31,    32,    -1,    92,
      -1,    -1,    -1,    38,    -1,    -1,    -1,   100,    43,    44,
      45,   104,    -1,    48,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    58,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      85,    -1,    -1,    38,    -1,    -1,    -1,    92,    43,    44,
      45,    -1,    -1,    48,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    58,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    31,    32,    -1,    92,    -1,    -1,
      -1,    38,    -1,    -1,    -1,   100,    43,    44,    45,   104,
      -1,    48,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    58,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    85,    -1,
      -1,    38,    -1,    -1,    -1,    92,    43,    44,    45,    -1,
      -1,    48,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    58,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    31,    32,    -1,    92,    -1,    -1,    -1,    38,
      -1,    -1,    -1,   100,    43,    44,    45,   104,    -1,    48,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    58,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    85,    -1,    -1,    38,
      -1,    -1,    -1,    92,    43,    44,    45,    -1,    -1,    48,
      -1,   100,    -1,    -1,    -1,   104,    -1,    67,    68,    58,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    -1,
      -1,    -1,    82,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    85,    97,    98,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,   104
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   107,   108,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    53,    54,    58,    62,    85,    92,   100,
     104,   109,   110,   111,   112,   113,   114,   115,   116,   118,
     119,   120,   123,   128,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   141,    81,    84,    97,   100,   102,
      86,   104,    84,    97,   100,    14,    10,    11,    12,    13,
      31,    98,   100,   128,   130,   140,    10,    11,    12,    14,
     100,   130,   100,   130,   100,   100,   130,   135,    14,   128,
     128,   130,    98,   130,   130,    12,    12,   100,   100,   128,
     104,   129,   129,   104,   129,   130,   128,   128,   130,    14,
      14,    31,    14,    10,   137,   137,   130,    61,   108,    98,
      76,    77,    78,    70,    71,    72,    73,    74,    75,    82,
      86,    87,    88,    89,    67,    68,    90,    91,    92,    83,
      93,    94,    95,    97,    14,   130,    14,   101,   140,   130,
      14,   130,   130,    14,    17,    52,   101,   140,    98,    84,
     102,    86,   104,   101,   130,   140,    98,    99,    84,    98,
      84,    98,    84,    98,    10,    11,    12,   130,    98,    99,
     130,    98,    99,   130,   130,    69,   128,    67,    68,   100,
      98,    98,   104,    86,   104,    86,   104,   130,   130,   130,
     130,   130,   130,    47,   121,   104,    14,    56,   104,   101,
     105,   105,   133,   133,   131,   133,   133,   133,   133,   133,
     133,     7,     8,     9,   133,   133,   134,   134,   135,   135,
     136,   136,   136,   137,   137,   137,   137,    14,    17,    98,
     100,    98,    84,   100,   101,   103,    87,   105,    98,   100,
      14,   100,   101,   130,   130,    14,   130,    98,   101,   130,
     130,   100,   130,     5,    14,   100,   126,   130,    84,    98,
      84,    98,    84,    98,    99,   140,    99,   140,   101,   101,
      10,   135,   135,    10,    14,   101,   127,   117,    14,   130,
      14,   130,   101,   101,   105,   128,    14,    22,   104,   122,
     100,   100,   124,    79,   100,    14,   101,   140,   130,   101,
     140,    84,    84,    84,   101,   140,    14,   101,   125,   140,
     103,    87,   105,    98,    98,   130,    98,    99,    66,    66,
     126,    98,    99,    98,   130,   100,   130,   130,   130,    98,
     130,    98,   128,   128,   128,    69,    69,    10,    59,    60,
     128,    99,   101,    36,    37,   105,    87,   105,    87,   105,
     128,   128,    14,    14,   122,    50,    51,    54,   105,   101,
     127,   127,    63,    64,    65,   105,   131,   101,   125,   140,
      98,   101,    98,    98,   101,   130,   130,   130,   101,    66,
      99,   101,   101,    99,   140,   130,   130,   101,     5,    14,
      98,   130,    98,    99,    98,   101,   101,    27,    10,    10,
      10,    10,    10,    14,   128,   130,   128,    98,    98,    27,
     105,    14,    52,    10,    13,    14,    15,    16,    50,   128,
     101,   101,    14,    14,    14,   101,   101,    98,    98,    98,
      98,    98,   130,    14,   140,    98,    98,    66,    66,    99,
     140,    98,    98,   110,   128,   128,   128,    10,    59,    60,
     128,   105,   128,   100,   100,    14,    22,    57,    84,    98,
      14,    22,    57,    84,    98,    10,    13,    15,    16,    98,
      98,    14,   128,   128,     7,     7,     7,    66,   101,   130,
     130,   140,    98,    10,    10,   101,   127,   101,   127,    14,
      14,    98,   130,    14,    14,    98,   130,    14,    22,    57,
      84,    98,    14,    22,    57,    84,    98,    98,    98,   100,
     130,    98,   101,   128,   101,   128,   101,    98,    98,    98,
      98,    98,    98,    14,    14,    98,   130,    14,    14,    98,
     130,   101,   127,    98,   128,   128,    98,    98,    98,    98,
      98,    98,   128,   101,   128
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   106,   107,   108,   108,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   110,   110,   110,   111,
     112,   112,   113,   114,   115,   115,   115,   115,   116,   116,
     117,   117,   118,   118,   118,   118,   118,   119,   120,   121,
     121,   121,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   123,   124,   124,   124,   124,   125,   125,
     126,   126,   126,   126,   127,   127,   127,   127,   127,   127,
     127,   127,   128,   128,   129,   130,   130,   130,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   133,   133,   133,   134,   134,   134,
     135,   135,   135,   135,   136,   136,   136,   136,   136,   137,
     137,   137,   138,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   140,   140,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     9,     5,     5,     7,     6,     6,     8,    10,     6,
       4,     4,     4,     3,     2,     5,     4,     3,     3,     5,
       7,     3,     5,     7,     3,     3,     2,     4,     7,     6,
       4,     6,     4,     7,     6,     7,     7,     7,     6,     6,
       2,     1,     1,     1,     1,     2,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     7,     7,     5,
       5,     7,     5,     2,     7,     7,     5,     3,     5,     7,
       0,     4,     6,     5,     7,     6,     7,     6,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     7,
       6,     8,     7,     5,     0,     4,     4,     4,     3,     5,
       3,     3,     5,     5,     1,     2,     3,     3,     3,     4,
       5,     5,     3,     3,     3,     3,     2,     1,     5,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     1,     1,     4,     3,     4,     3,     6,     6,     5,
       6,     5,     4,     4,     6,     6,     5,     3,     2,     3,
       3,     2,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       5,     5,     1,     1,     1,     1,     3,     1,     2
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
#line 248 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 1985 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 258 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 1991 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 259 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 1997 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 263 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2003 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 265 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2009 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 267 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2015 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 269 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2021 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 271 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2027 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 273 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2035 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2043 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 281 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2049 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 283 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2055 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 285 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2061 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 287 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2067 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 289 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2073 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 291 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2081 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 295 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2089 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 299 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2095 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 301 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2101 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2107 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 305 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2113 "raku.tab.c"
    break;

  case 23: /* stmt: KW_USE IDENT ';'  */
#line 307 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2119 "raku.tab.c"
    break;

  case 24: /* stmt: TESTOP ';'  */
#line 309 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2125 "raku.tab.c"
    break;

  case 25: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 311 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2131 "raku.tab.c"
    break;

  case 26: /* stmt: TESTOP '(' ')' ';'  */
#line 313 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2137 "raku.tab.c"
    break;

  case 27: /* stmt: TESTOP arg_list ';'  */
#line 315 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2143 "raku.tab.c"
    break;

  case 28: /* stmt: KW_SAY expr ';'  */
#line 317 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2149 "raku.tab.c"
    break;

  case 29: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 319 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2156 "raku.tab.c"
    break;

  case 30: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 322 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2162 "raku.tab.c"
    break;

  case 31: /* stmt: KW_PRINT expr ';'  */
#line 324 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2168 "raku.tab.c"
    break;

  case 32: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 326 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2175 "raku.tab.c"
    break;

  case 33: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 329 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2181 "raku.tab.c"
    break;

  case 34: /* stmt: KW_TAKE expr ';'  */
#line 331 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2187 "raku.tab.c"
    break;

  case 35: /* stmt: KW_RETURN expr ';'  */
#line 333 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2193 "raku.tab.c"
    break;

  case 36: /* stmt: KW_RETURN ';'  */
#line 335 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2199 "raku.tab.c"
    break;

  case 37: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 337 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2205 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 339 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2214 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 344 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2222 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 348 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2230 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 352 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2239 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 357 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2247 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 361 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2258 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 368 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2267 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 373 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2274 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 376 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2281 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 379 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2288 "raku.tab.c"
    break;

  case 48: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 382 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2295 "raku.tab.c"
    break;

  case 49: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 385 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2302 "raku.tab.c"
    break;

  case 50: /* stmt: expr ';'  */
#line 387 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2308 "raku.tab.c"
    break;

  case 51: /* stmt: if_stmt  */
#line 388 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2314 "raku.tab.c"
    break;

  case 52: /* stmt: while_stmt  */
#line 389 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2320 "raku.tab.c"
    break;

  case 53: /* stmt: for_stmt  */
#line 390 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2326 "raku.tab.c"
    break;

  case 54: /* stmt: given_stmt  */
#line 391 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2332 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TRY block  */
#line 393 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2338 "raku.tab.c"
    break;

  case 56: /* stmt: KW_TRY block KW_CATCH block  */
#line 395 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2344 "raku.tab.c"
    break;

  case 57: /* stmt: KW_CATCH block  */
#line 397 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2350 "raku.tab.c"
    break;

  case 58: /* stmt: block  */
#line 399 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2356 "raku.tab.c"
    break;

  case 59: /* stmt: unless_stmt  */
#line 400 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2362 "raku.tab.c"
    break;

  case 60: /* stmt: until_stmt  */
#line 401 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2368 "raku.tab.c"
    break;

  case 61: /* stmt: repeat_stmt  */
#line 402 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2374 "raku.tab.c"
    break;

  case 62: /* stmt: sub_decl  */
#line 403 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2380 "raku.tab.c"
    break;

  case 63: /* stmt: class_decl  */
#line 404 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2386 "raku.tab.c"
    break;

  case 64: /* stmt: role_decl  */
#line 405 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2392 "raku.tab.c"
    break;

  case 65: /* stmt: grammar_decl  */
#line 406 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2398 "raku.tab.c"
    break;

  case 66: /* if_stmt: KW_IF '(' expr ')' block  */
#line 410 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2404 "raku.tab.c"
    break;

  case 67: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 412 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2410 "raku.tab.c"
    break;

  case 68: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2416 "raku.tab.c"
    break;

  case 69: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 418 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2422 "raku.tab.c"
    break;

  case 70: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 422 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2428 "raku.tab.c"
    break;

  case 71: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 424 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2434 "raku.tab.c"
    break;

  case 72: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 428 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2440 "raku.tab.c"
    break;

  case 73: /* repeat_stmt: KW_REPEAT block  */
#line 432 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2446 "raku.tab.c"
    break;

  case 74: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 436 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2456 "raku.tab.c"
    break;

  case 75: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 442 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2466 "raku.tab.c"
    break;

  case 76: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 448 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2475 "raku.tab.c"
    break;

  case 77: /* for_stmt: KW_FOR expr block  */
#line 453 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2482 "raku.tab.c"
    break;

  case 78: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 458 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2494 "raku.tab.c"
    break;

  case 79: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 466 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2507 "raku.tab.c"
    break;

  case 80: /* when_list: %empty  */
#line 476 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2513 "raku.tab.c"
    break;

  case 81: /* when_list: when_list KW_WHEN expr block  */
#line 478 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2521 "raku.tab.c"
    break;

  case 82: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 484 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2533 "raku.tab.c"
    break;

  case 83: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 492 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2543 "raku.tab.c"
    break;

  case 84: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 498 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2556 "raku.tab.c"
    break;

  case 85: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 507 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2567 "raku.tab.c"
    break;

  case 86: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 514 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2580 "raku.tab.c"
    break;

  case 87: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 525 "raku.y"
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
#line 2598 "raku.tab.c"
    break;

  case 88: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 541 "raku.y"
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
#line 2615 "raku.tab.c"
    break;

  case 89: /* is_clauses: %empty  */
#line 555 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2621 "raku.tab.c"
    break;

  case 90: /* is_clauses: is_clauses IDENT IDENT  */
#line 557 "raku.y"
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
#line 2637 "raku.tab.c"
    break;

  case 91: /* is_clauses: is_clauses TESTOP IDENT  */
#line 569 "raku.y"
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
#line 2653 "raku.tab.c"
    break;

  case 92: /* class_body_list: %empty  */
#line 582 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2659 "raku.tab.c"
    break;

  case 93: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 584 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2666 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 587 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2673 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 590 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2680 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 593 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2687 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 596 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2694 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 599 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2701 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 602 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2708 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 605 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2715 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 608 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2722 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 611 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2729 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 614 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2736 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 617 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2743 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 620 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2754 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 627 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2765 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 634 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2776 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 641 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2787 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 648 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2795 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 652 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2803 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 656 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2811 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 660 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2819 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 664 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2830 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 671 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2841 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 678 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2852 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 685 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2863 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 692 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2877 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 702 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2889 "raku.tab.c"
    break;

  case 119: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 710 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2902 "raku.tab.c"
    break;

  case 120: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 719 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2913 "raku.tab.c"
    break;

  case 121: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 726 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2927 "raku.tab.c"
    break;

  case 122: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 736 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2939 "raku.tab.c"
    break;

  case 123: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 746 "raku.y"
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
#line 2956 "raku.tab.c"
    break;

  case 124: /* grammar_body_list: %empty  */
#line 760 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2962 "raku.tab.c"
    break;

  case 125: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 762 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2971 "raku.tab.c"
    break;

  case 126: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 767 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2980 "raku.tab.c"
    break;

  case 127: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 772 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 2989 "raku.tab.c"
    break;

  case 128: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 779 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 2997 "raku.tab.c"
    break;

  case 129: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 783 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3005 "raku.tab.c"
    break;

  case 130: /* pair_list: IDENT OP_FATARROW expr  */
#line 789 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3011 "raku.tab.c"
    break;

  case 131: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 791 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3017 "raku.tab.c"
    break;

  case 132: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 793 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3023 "raku.tab.c"
    break;

  case 133: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 795 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3029 "raku.tab.c"
    break;

  case 134: /* param_list: VAR_SCALAR  */
#line 798 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3035 "raku.tab.c"
    break;

  case 135: /* param_list: IDENT VAR_SCALAR  */
#line 799 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3041 "raku.tab.c"
    break;

  case 136: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 800 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3047 "raku.tab.c"
    break;

  case 137: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 801 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3053 "raku.tab.c"
    break;

  case 138: /* param_list: param_list ',' VAR_SCALAR  */
#line 802 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3059 "raku.tab.c"
    break;

  case 139: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 803 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3065 "raku.tab.c"
    break;

  case 140: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 804 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3071 "raku.tab.c"
    break;

  case 141: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 805 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3077 "raku.tab.c"
    break;

  case 142: /* block: '{' stmt_list '}'  */
#line 808 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3083 "raku.tab.c"
    break;

  case 143: /* block: '{' YADA '}'  */
#line 809 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3089 "raku.tab.c"
    break;

  case 144: /* closure: '{' expr '}'  */
#line 812 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3095 "raku.tab.c"
    break;

  case 145: /* expr: VAR_SCALAR '=' expr  */
#line 815 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3101 "raku.tab.c"
    break;

  case 146: /* expr: KW_GATHER block  */
#line 816 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3111 "raku.tab.c"
    break;

  case 147: /* expr: tern_expr  */
#line 821 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3117 "raku.tab.c"
    break;

  case 148: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 825 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3123 "raku.tab.c"
    break;

  case 149: /* tern_expr: cmp_expr  */
#line 826 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3129 "raku.tab.c"
    break;

  case 150: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 829 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3135 "raku.tab.c"
    break;

  case 151: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 830 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3141 "raku.tab.c"
    break;

  case 152: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 831 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3147 "raku.tab.c"
    break;

  case 153: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 832 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3153 "raku.tab.c"
    break;

  case 154: /* cmp_expr: jct_expr '<' jct_expr  */
#line 833 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3159 "raku.tab.c"
    break;

  case 155: /* cmp_expr: jct_expr '>' jct_expr  */
#line 834 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3165 "raku.tab.c"
    break;

  case 156: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 835 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3171 "raku.tab.c"
    break;

  case 157: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 836 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3177 "raku.tab.c"
    break;

  case 158: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 837 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3183 "raku.tab.c"
    break;

  case 159: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 838 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3189 "raku.tab.c"
    break;

  case 160: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 840 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3199 "raku.tab.c"
    break;

  case 161: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 846 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3209 "raku.tab.c"
    break;

  case 162: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 852 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3219 "raku.tab.c"
    break;

  case 163: /* cmp_expr: jct_expr  */
#line 857 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3225 "raku.tab.c"
    break;

  case 164: /* jct_expr: jct_expr '|' range_expr  */
#line 860 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3231 "raku.tab.c"
    break;

  case 165: /* jct_expr: jct_expr '&' range_expr  */
#line 861 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3237 "raku.tab.c"
    break;

  case 166: /* jct_expr: range_expr  */
#line 862 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3243 "raku.tab.c"
    break;

  case 167: /* range_expr: add_expr OP_RANGE add_expr  */
#line 865 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3249 "raku.tab.c"
    break;

  case 168: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 866 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3255 "raku.tab.c"
    break;

  case 169: /* range_expr: add_expr  */
#line 867 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3261 "raku.tab.c"
    break;

  case 170: /* add_expr: add_expr '+' mul_expr  */
#line 870 "raku.y"
                             { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3267 "raku.tab.c"
    break;

  case 171: /* add_expr: add_expr '-' mul_expr  */
#line 871 "raku.y"
                             { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3273 "raku.tab.c"
    break;

  case 172: /* add_expr: add_expr '~' mul_expr  */
#line 872 "raku.y"
                             { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3279 "raku.tab.c"
    break;

  case 173: /* add_expr: mul_expr  */
#line 873 "raku.y"
                             { (yyval.node)=(yyvsp[0].node); }
#line 3285 "raku.tab.c"
    break;

  case 174: /* mul_expr: mul_expr '*' unary_expr  */
#line 876 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3291 "raku.tab.c"
    break;

  case 175: /* mul_expr: mul_expr '/' unary_expr  */
#line 877 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3297 "raku.tab.c"
    break;

  case 176: /* mul_expr: mul_expr '%' unary_expr  */
#line 878 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3303 "raku.tab.c"
    break;

  case 177: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 879 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3309 "raku.tab.c"
    break;

  case 178: /* mul_expr: unary_expr  */
#line 880 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3315 "raku.tab.c"
    break;

  case 179: /* unary_expr: '-' unary_expr  */
#line 883 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3321 "raku.tab.c"
    break;

  case 180: /* unary_expr: '!' unary_expr  */
#line 884 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3327 "raku.tab.c"
    break;

  case 181: /* unary_expr: postfix_expr  */
#line 885 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3333 "raku.tab.c"
    break;

  case 182: /* postfix_expr: call_expr  */
#line 887 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3339 "raku.tab.c"
    break;

  case 183: /* call_expr: IDENT '(' arg_list ')'  */
#line 890 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3348 "raku.tab.c"
    break;

  case 184: /* call_expr: IDENT '(' ')'  */
#line 894 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3354 "raku.tab.c"
    break;

  case 185: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 896 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3363 "raku.tab.c"
    break;

  case 186: /* call_expr: VAR_SCALAR '(' ')'  */
#line 901 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3369 "raku.tab.c"
    break;

  case 187: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 903 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3379 "raku.tab.c"
    break;

  case 188: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 909 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3389 "raku.tab.c"
    break;

  case 189: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 915 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3397 "raku.tab.c"
    break;

  case 190: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 919 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3408 "raku.tab.c"
    break;

  case 191: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 926 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3417 "raku.tab.c"
    break;

  case 192: /* call_expr: IDENT '.' CARET IDENT  */
#line 931 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3427 "raku.tab.c"
    break;

  case 193: /* call_expr: atom '.' CARET IDENT  */
#line 937 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3437 "raku.tab.c"
    break;

  case 194: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 943 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3448 "raku.tab.c"
    break;

  case 195: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 950 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3459 "raku.tab.c"
    break;

  case 196: /* call_expr: atom '.' IDENT '(' ')'  */
#line 957 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3468 "raku.tab.c"
    break;

  case 197: /* call_expr: atom '.' IDENT  */
#line 962 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3477 "raku.tab.c"
    break;

  case 198: /* call_expr: KW_DIE expr  */
#line 967 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3483 "raku.tab.c"
    break;

  case 199: /* call_expr: KW_MAP closure expr  */
#line 969 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3489 "raku.tab.c"
    break;

  case 200: /* call_expr: KW_GREP closure expr  */
#line 971 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3495 "raku.tab.c"
    break;

  case 201: /* call_expr: KW_SORT expr  */
#line 973 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3501 "raku.tab.c"
    break;

  case 202: /* call_expr: KW_SORT closure expr  */
#line 975 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3507 "raku.tab.c"
    break;

  case 203: /* call_expr: atom  */
#line 976 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3513 "raku.tab.c"
    break;

  case 204: /* arg_list: expr  */
#line 979 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3519 "raku.tab.c"
    break;

  case 205: /* arg_list: arg_list ',' expr  */
#line 980 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3525 "raku.tab.c"
    break;

  case 206: /* atom: LIT_INT  */
#line 983 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3531 "raku.tab.c"
    break;

  case 207: /* atom: LIT_FLOAT  */
#line 984 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3537 "raku.tab.c"
    break;

  case 208: /* atom: LIT_STR  */
#line 985 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3543 "raku.tab.c"
    break;

  case 209: /* atom: WORDLIST  */
#line 987 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3555 "raku.tab.c"
    break;

  case 210: /* atom: LIT_INTERP_STR  */
#line 994 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3561 "raku.tab.c"
    break;

  case 211: /* atom: VAR_SCALAR  */
#line 995 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3567 "raku.tab.c"
    break;

  case 212: /* atom: VAR_ARRAY  */
#line 996 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3573 "raku.tab.c"
    break;

  case 213: /* atom: VAR_HASH  */
#line 997 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3579 "raku.tab.c"
    break;

  case 214: /* atom: VAR_CAPTURE  */
#line 999 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3587 "raku.tab.c"
    break;

  case 215: /* atom: VAR_FH  */
#line 1003 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3595 "raku.tab.c"
    break;

  case 216: /* atom: VAR_NAMED_CAPTURE  */
#line 1007 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3602 "raku.tab.c"
    break;

  case 217: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1010 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3608 "raku.tab.c"
    break;

  case 218: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1012 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3614 "raku.tab.c"
    break;

  case 219: /* atom: VAR_HASH '{' expr '}'  */
#line 1014 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3620 "raku.tab.c"
    break;

  case 220: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1016 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3626 "raku.tab.c"
    break;

  case 221: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1018 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3632 "raku.tab.c"
    break;

  case 222: /* atom: IDENT  */
#line 1019 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3638 "raku.tab.c"
    break;

  case 223: /* atom: VAR_TWIGIL  */
#line 1021 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3646 "raku.tab.c"
    break;

  case 224: /* atom: VAR_ARRAY_TWIGIL  */
#line 1025 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3654 "raku.tab.c"
    break;

  case 225: /* atom: VAR_HASH_TWIGIL  */
#line 1029 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3662 "raku.tab.c"
    break;

  case 226: /* atom: '(' expr ')'  */
#line 1032 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3668 "raku.tab.c"
    break;

  case 227: /* atom: block  */
#line 1033 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3674 "raku.tab.c"
    break;

  case 228: /* atom: KW_SUB block  */
#line 1034 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3680 "raku.tab.c"
    break;


#line 3684 "raku.tab.c"

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

#line 1036 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
