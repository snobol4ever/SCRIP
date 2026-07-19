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
  YYSYMBOL_OP_REP_X = 84,                  /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 85,                 /* OP_REP_XX  */
  YYSYMBOL_86_ = 86,                       /* '='  */
  YYSYMBOL_87_ = 87,                       /* '!'  */
  YYSYMBOL_88_ = 88,                       /* '<'  */
  YYSYMBOL_89_ = 89,                       /* '>'  */
  YYSYMBOL_90_ = 90,                       /* '|'  */
  YYSYMBOL_91_ = 91,                       /* '&'  */
  YYSYMBOL_92_ = 92,                       /* '~'  */
  YYSYMBOL_93_ = 93,                       /* '+'  */
  YYSYMBOL_94_ = 94,                       /* '-'  */
  YYSYMBOL_95_ = 95,                       /* '*'  */
  YYSYMBOL_96_ = 96,                       /* '/'  */
  YYSYMBOL_97_ = 97,                       /* '%'  */
  YYSYMBOL_UMINUS = 98,                    /* UMINUS  */
  YYSYMBOL_99_ = 99,                       /* '.'  */
  YYSYMBOL_100_ = 100,                     /* ';'  */
  YYSYMBOL_101_ = 101,                     /* ','  */
  YYSYMBOL_102_ = 102,                     /* '('  */
  YYSYMBOL_103_ = 103,                     /* ')'  */
  YYSYMBOL_104_ = 104,                     /* '['  */
  YYSYMBOL_105_ = 105,                     /* ']'  */
  YYSYMBOL_106_ = 106,                     /* '{'  */
  YYSYMBOL_107_ = 107,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 108,                 /* $accept  */
  YYSYMBOL_program = 109,                  /* program  */
  YYSYMBOL_stmt_list = 110,                /* stmt_list  */
  YYSYMBOL_stmt = 111,                     /* stmt  */
  YYSYMBOL_if_stmt = 112,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 113,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 114,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 115,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 116,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 117,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 118,               /* given_stmt  */
  YYSYMBOL_when_list = 119,                /* when_list  */
  YYSYMBOL_sub_decl = 120,                 /* sub_decl  */
  YYSYMBOL_class_decl = 121,               /* class_decl  */
  YYSYMBOL_role_decl = 122,                /* role_decl  */
  YYSYMBOL_is_clauses = 123,               /* is_clauses  */
  YYSYMBOL_class_body_list = 124,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 125,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 126,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 127,           /* named_arg_list  */
  YYSYMBOL_pair_list = 128,                /* pair_list  */
  YYSYMBOL_param_list = 129,               /* param_list  */
  YYSYMBOL_block = 130,                    /* block  */
  YYSYMBOL_closure = 131,                  /* closure  */
  YYSYMBOL_expr = 132,                     /* expr  */
  YYSYMBOL_tern_expr = 133,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 134,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 135,                 /* jct_expr  */
  YYSYMBOL_range_expr = 136,               /* range_expr  */
  YYSYMBOL_add_expr = 137,                 /* add_expr  */
  YYSYMBOL_repl_expr = 138,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 139,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 140,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 141,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 142,             /* postfix_expr  */
  YYSYMBOL_call_expr = 143,                /* call_expr  */
  YYSYMBOL_arg_list = 144,                 /* arg_list  */
  YYSYMBOL_atom = 145                      /* atom  */
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
#define YYLAST   1932

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  232
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  582

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   341


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
       2,     2,     2,    87,     2,     2,     2,    97,    91,     2,
     102,   103,    95,    93,   101,    94,    99,    96,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   100,
      88,    86,    89,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   104,     2,   105,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   106,    90,   107,    92,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    98
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   250,   250,   261,   262,   265,   267,   269,   271,   273,
     275,   279,   282,   286,   288,   290,   292,   294,   296,   300,
     304,   306,   308,   310,   312,   314,   316,   318,   320,   322,
     324,   327,   329,   331,   334,   336,   338,   340,   342,   344,
     349,   353,   357,   362,   366,   373,   378,   381,   384,   387,
     390,   393,   394,   395,   396,   397,   398,   400,   402,   404,
     406,   407,   408,   409,   410,   411,   412,   415,   417,   419,
     423,   427,   429,   433,   437,   441,   447,   453,   458,   463,
     471,   482,   483,   489,   497,   503,   512,   519,   530,   546,
     561,   562,   574,   588,   589,   592,   595,   598,   601,   604,
     607,   610,   613,   616,   619,   622,   625,   632,   639,   646,
     653,   657,   661,   665,   669,   676,   683,   690,   697,   707,
     715,   724,   731,   741,   751,   766,   767,   772,   777,   784,
     788,   794,   796,   798,   800,   804,   805,   806,   807,   808,
     809,   810,   811,   814,   815,   818,   821,   822,   827,   830,
     832,   835,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   851,   857,   863,   866,   867,   868,   871,   872,
     873,   876,   877,   880,   881,   884,   885,   886,   889,   890,
     891,   892,   893,   896,   897,   898,   900,   902,   907,   908,
     913,   915,   921,   927,   931,   938,   943,   949,   955,   962,
     969,   974,   979,   981,   983,   985,   987,   989,   992,   993,
     996,   997,   998,   999,  1007,  1008,  1009,  1010,  1011,  1015,
    1019,  1022,  1024,  1026,  1028,  1030,  1032,  1033,  1037,  1041,
    1045,  1046,  1047
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
  "OP_SMATCH", "OP_DIV", "OP_REP_X", "OP_REP_XX", "'='", "'!'", "'<'",
  "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "$accept",
  "program", "stmt_list", "stmt", "if_stmt", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "class_decl", "role_decl", "is_clauses", "class_body_list",
  "grammar_decl", "grammar_body_list", "named_arg_list", "pair_list",
  "param_list", "block", "closure", "expr", "tern_expr", "cmp_expr",
  "jct_expr", "range_expr", "add_expr", "repl_expr", "addsub_expr",
  "mul_expr", "unary_expr", "postfix_expr", "call_expr", "arg_list",
  "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-318)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-232)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -318,    77,   578,  -318,  -318,  -318,  -318,  -318,    54,     3,
     -48,    62,   -42,  -318,  -318,  -318,  -318,  -318,    73,   638,
     282,  1346,  1406,    68,    82,  1466,   -11,   -12,  1466,   698,
    1466,   152,   180,   120,   133,   -12,   131,   131,  1526,   -12,
     -12,  1466,   230,   255,   242,  -318,   292,  1826,  1826,  1466,
     234,  -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,
    -318,  -318,  -318,   175,   161,  -318,   163,   107,  -318,   -33,
     258,   138,   111,  -318,  -318,  -318,   190,   316,  1466,   319,
     758,  1466,   339,  1466,  1466,     2,   824,   204,    11,   208,
     -37,  -318,   -12,  -318,   890,  -318,  -318,   155,    71,    75,
     105,   200,  1466,   269,  1466,   271,  1466,  1466,   -39,    84,
     274,  -318,  -318,   304,  -318,   305,   277,    30,    66,  1466,
    1466,  -318,  1466,  1466,  1466,   956,  1466,  -318,   359,  -318,
    -318,  -318,   303,   -10,   306,   308,  -318,  -318,   310,   307,
     461,  -318,  1826,  1826,  1826,  1826,  1826,  1826,  1826,  1826,
    1826,   290,  1826,  1826,  1826,  1826,  1826,  1826,  1826,  1826,
    1826,  1826,  1826,  1826,  1826,  1826,   213,   -53,   311,    14,
    -318,   202,   312,   326,   309,   318,   317,   406,   320,  -318,
     207,  -318,  1466,  1466,   409,  1466,   324,   310,   210,  -318,
    1466,  1466,  -318,  1586,  -318,  1646,  -318,   114,   116,   123,
     215,  -318,  1466,   216,  -318,  1466,   322,   323,   417,  -318,
    1826,  1826,    -5,  -318,  -318,  -318,   414,  1466,   415,  1466,
     327,   331,   328,  -318,  -318,  -318,   -12,     9,  -318,   334,
     335,  -318,  -318,  -318,  -318,   283,   283,   361,   283,   283,
     283,   283,   283,   283,  -318,  -318,  -318,   283,   283,  -318,
    -318,   346,   346,   258,   138,   111,   111,  -318,  -318,  -318,
    -318,   340,   427,  -318,  1016,  -318,  1466,  1082,  -318,   358,
     360,   362,  -318,  1148,  -318,  1214,  -318,  -318,   342,   356,
     343,  -318,   349,  -318,   351,  1466,   103,   386,   -16,  1706,
     281,   363,  1466,  -318,  1766,  -318,  1466,  -318,  1466,   284,
    1466,   286,   -12,   -12,   -12,   -17,   -14,  -318,    29,   -12,
     219,    -8,   364,   352,   367,   354,   -12,   -12,  -318,  -318,
     444,   446,  -318,    12,     0,   145,   -22,  1826,  1280,  -318,
     368,   220,   369,   370,   223,  1466,  1466,  1466,  -318,   224,
      10,  -318,   228,   233,  -318,  -318,  -318,  -318,  -318,   237,
    1466,  -318,  1466,  1466,  1466,   244,  -318,   109,  -318,   378,
    1466,   288,   388,   394,  -318,   395,  -318,   435,  -318,  -318,
     479,   501,  -318,   502,   503,  -318,   203,   -12,  1466,   -12,
    -318,  -318,  -318,   416,   421,   497,  -318,  -318,  -318,    51,
      18,   205,   475,  -318,   -12,   245,   248,   512,   513,   514,
    -318,  -318,  -318,   249,   253,  -318,   429,  -318,  -318,   430,
     431,   432,   433,  -318,  1466,   520,  -318,  -318,  1466,   436,
     294,  -318,  -318,   437,   469,   473,  -318,   256,  -318,  1466,
    -318,   440,   441,   -13,   -12,   -12,  -318,  -318,  -318,    70,
    -318,   -12,   438,  -318,  -318,   -12,  -318,   442,   445,    22,
      34,   275,   450,   452,   528,  -318,   -12,   -12,   539,   546,
     547,  -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,   490,
     257,  -318,  -318,  -318,  1466,  1466,  1466,   298,  -318,  -318,
    -318,  -318,  -318,  -318,  -318,   548,   549,  -318,  -318,  -318,
       1,     7,   550,   551,   460,  1466,  -318,   555,   556,   462,
    1466,  -318,    39,    60,   471,   472,  -318,  -318,   459,  -318,
    -318,  -318,  -318,  -318,  1466,   474,  -318,  -318,   260,  -318,
    -318,  -318,   -12,   261,   -12,   264,   476,   477,  -318,   486,
     487,   495,  -318,   505,   559,   561,   506,  1466,  -318,   565,
     600,   515,  1466,  -318,  -318,  -318,     8,  -318,  -318,   529,
    -318,   -12,  -318,   -12,  -318,  -318,  -318,  -318,  -318,  -318,
     530,   533,  -318,   534,   535,   537,  -318,   538,   -12,   265,
    -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,  -318,
     -12,  -318
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   210,   211,   212,   214,   215,   216,
     217,   227,   226,   228,   229,   218,   219,   220,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       3,     4,    52,    53,    60,    61,    62,    54,    55,    63,
      64,    65,    66,    59,     0,   148,   150,   164,   167,   170,
     172,   174,   177,   182,   185,   186,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   227,     0,    25,     0,   231,   208,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
       0,   232,   147,     0,    37,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     3,     0,   205,    56,    58,
     202,    90,     0,     0,     0,   215,   184,   183,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,     0,     0,     0,     0,     0,     0,   188,
       0,    24,     0,     0,     0,     0,     0,   208,     0,    28,
       0,     0,     6,     0,     7,     0,     8,     0,     0,     0,
       0,    29,     0,     0,    32,     0,     0,     0,     0,    78,
       0,     0,     0,    35,    36,    81,     0,     0,     0,     0,
       0,     0,     0,   203,   204,   206,     0,     0,    93,     0,
       0,   125,   230,   144,   143,   151,   152,     0,   153,   154,
     157,   158,   159,   160,   161,   162,   163,   155,   156,   165,
     166,   168,   169,   171,   173,   175,   176,   181,   178,   179,
     180,   201,     0,    41,     0,    38,     0,     0,   189,   221,
     222,   223,    43,     0,   196,     0,   187,   146,     0,     0,
       0,    27,     0,   209,     0,     0,     0,   212,   226,     0,
       0,     0,     0,    21,     0,    22,     0,    23,     0,     0,
       0,     0,     0,     0,     0,   168,   169,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,    57,
       0,     0,    93,     0,     0,     0,     0,     0,     0,   197,
       0,     0,     0,     0,     0,     0,     0,     0,   195,     0,
     226,   193,     0,     0,   221,   222,   223,    26,     5,     0,
       0,     9,     0,     0,     0,     0,    14,     0,    13,     0,
       0,     0,     0,     0,    30,     0,    33,    67,    70,    77,
       0,     0,   136,     0,     0,    84,     0,     0,     0,     0,
      79,   224,   225,     0,     0,    71,    73,    91,    92,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
     124,   149,   200,     0,     0,    40,     0,    42,    45,     0,
       0,     0,     0,   194,     0,     0,   191,   192,     0,     0,
       0,   132,   131,     0,     0,     0,    16,     0,    17,     0,
      20,     0,     0,     0,     0,     0,   137,   138,   139,     0,
      83,     0,     0,    49,    50,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,   199,   198,    39,    44,    46,    47,    48,   129,     0,
       0,    11,    10,    15,     0,     0,     0,     0,    31,    34,
      69,    68,    75,    76,   140,     0,     0,    82,    80,    72,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,    94,     0,     0,     0,     0,    95,    96,     0,    85,
      87,   126,   127,   128,     0,     0,   134,   133,     0,    18,
     141,   142,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,   111,     0,     0,     0,     0,     0,   101,     0,
       0,     0,     0,   100,    97,    98,     0,   130,    12,     0,
     119,     0,   121,     0,   108,   109,   103,   106,   107,   102,
       0,     0,   112,     0,     0,     0,   113,     0,     0,     0,
      19,   118,   120,   116,   117,   105,   114,   115,   104,   123,
       0,   122
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -318,  -318,   628,  -318,   206,  -318,  -318,  -318,  -318,  -318,
    -318,  -318,  -318,  -318,  -318,  -318,   325,  -318,  -318,   332,
     357,  -317,    -2,   353,    43,  -142,  -318,   134,   246,   -24,
     504,   496,   247,   -21,  -318,  -318,   -74,  -318
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   140,    51,    52,    53,    54,    55,    56,    57,
      58,   311,    59,    60,    61,   227,   323,    62,   326,   342,
     290,   310,    95,   123,    96,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    97,    76
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      63,   109,   237,   110,   229,   307,   171,   395,   396,   308,
     307,   307,   180,    23,   308,   308,   176,   307,   307,   177,
     188,   308,   308,   320,   111,   112,   136,   137,   378,   379,
     208,   321,   447,   121,   156,   157,   492,   128,   129,   372,
      82,   397,   398,   399,   493,    64,   230,   263,   497,   264,
     354,   184,   370,   534,   178,   371,   498,    85,    83,   158,
      86,   535,   390,   391,   103,   105,   392,    50,   108,   185,
     448,   113,   115,   116,   539,   158,   414,     3,   158,   494,
     484,   127,   540,    85,   130,   400,    86,    87,   373,   374,
     111,   499,   138,    50,    50,    50,   536,   182,   309,   380,
     266,   390,   391,   394,   522,   392,   209,    81,   495,    85,
     524,   568,    86,    80,   424,   322,   267,   541,   216,   393,
     500,   168,   496,   425,   172,   537,   174,   175,   299,   485,
     486,   301,   251,   252,   501,    77,   217,   187,    63,   538,
      78,   257,   258,   259,   260,   200,   542,   203,    84,   206,
     207,   210,   211,    79,   218,   307,    80,   191,   446,   308,
     543,   193,   220,   221,   117,   222,   223,   224,   222,   225,
     106,   192,   219,   523,   525,   194,   158,   145,   146,   147,
     148,   149,   150,    64,   107,   401,   305,   306,   350,   151,
     331,   195,   118,   334,   162,   152,   153,   154,   155,   339,
     292,   343,   294,   351,   352,   196,   163,   164,   165,   296,
     197,   198,   199,   438,   293,   449,   295,   439,   450,   451,
     452,   453,   119,   297,   319,   277,   278,   261,   280,   569,
     262,   160,   161,   283,   284,   120,   286,   122,   291,   142,
     143,   144,  -231,  -231,   131,  -231,  -231,  -231,  -231,  -231,
    -231,  -231,  -231,  -231,   404,   189,   190,  -231,  -231,  -231,
     313,   141,   315,  -231,  -231,  -231,  -231,  -231,  -231,   132,
    -231,  -231,  -231,   133,  -231,  -231,   235,   236,   420,   238,
     239,   240,   241,   242,   243,   502,   247,   248,   503,   166,
     504,   505,    98,    99,   100,   139,   101,   244,   245,   246,
     367,   368,   369,   190,   181,   268,   134,   375,   190,   332,
     276,   190,   183,   282,   385,   386,   298,   300,   232,   232,
     376,   190,   377,   406,   190,   190,   409,   413,   349,   415,
     167,   416,   138,   169,   190,   359,   417,   361,   418,   362,
     232,   363,   159,   365,   470,   357,   376,   423,   456,   376,
     415,   457,   461,   173,   190,   477,   462,   476,   190,   232,
     515,   190,   376,   549,   551,   376,   376,   553,   580,   201,
     202,   204,   205,   154,   155,   440,   212,   442,   410,   411,
     412,   356,   357,   215,   364,   190,   366,   190,   428,   429,
     124,   126,   455,   419,   472,   190,   421,   422,   519,   190,
     249,   250,   518,   427,   213,   214,   226,   255,   256,   228,
      80,   265,   231,   232,   233,   270,   271,   269,   272,   273,
     274,   441,   275,   279,   281,   302,   303,   304,   312,   314,
     316,   481,   482,   483,   317,   318,   324,   325,   158,   487,
     327,   329,   328,   489,   335,   345,   336,   344,   337,   347,
     346,   348,   353,   381,   509,   510,   383,   468,   387,   382,
     388,   384,   433,   358,     4,     5,     6,     7,   405,   407,
     408,     8,     9,    10,    11,    12,    13,    14,   426,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   430,   434,
      24,    25,    26,    27,    28,    29,    30,   431,   432,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,   435,   436,   437,    43,    44,   443,   516,   517,    45,
     550,   444,   552,    46,   445,   454,   458,   459,   460,   463,
     464,   465,   466,   467,   469,   474,   471,   473,   529,   475,
     478,   479,   508,   533,   490,   488,   511,   491,    47,   571,
     506,   572,   507,   512,   513,    48,   514,   547,   520,   521,
     528,   546,   532,    49,   526,   527,   579,    50,   234,   530,
     531,   544,   545,   560,   548,   561,   554,   555,   581,   564,
     563,     4,     5,     6,     7,   567,   556,   557,     8,     9,
      10,    11,    12,    13,    14,   558,    15,    16,    17,    18,
      19,    20,    21,    22,    23,   559,   562,    24,    25,    26,
      27,    28,    29,    30,   565,   566,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,     2,   570,
     573,    43,    44,   574,   575,   576,    45,   577,   578,   480,
      46,     4,     5,     6,     7,     0,   355,   389,    88,    89,
      90,    91,    12,    13,    14,   254,    15,    16,    17,     0,
     403,     0,   253,     0,     0,    47,     0,     0,     0,    92,
      27,     0,    48,     0,     0,     0,    31,     0,     0,     0,
      49,    36,    37,    38,    50,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    88,    89,
      90,    91,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,    92,
      27,     0,    48,     0,     0,     0,    31,     0,    93,     0,
      94,    36,    37,    38,    50,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    88,    89,
      90,    91,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,    47,     0,     0,     0,    92,
      27,     0,    48,     0,     0,     0,    31,     0,   114,     0,
      49,    36,    37,    38,    50,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    88,    89,    90,    91,    12,    13,
      14,     0,    15,    16,    17,    47,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    92,    27,     0,     0,     0,
      49,   170,    31,     0,    50,     0,     0,    36,    37,    38,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      88,    89,    90,    91,    12,    13,    14,     0,    15,    16,
      17,    47,     0,     0,     0,     0,     0,     0,    48,     0,
       0,    92,    27,     0,     0,     0,    49,   179,    31,     0,
      50,     0,     0,    36,    37,    38,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,    47,     0,     0,
       0,     0,     0,     0,    48,     0,     0,    92,    27,     0,
       0,     0,    49,   186,    31,     0,    50,     0,     0,    36,
      37,    38,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,   139,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,    49,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    88,    89,    90,    91,    12,    13,    14,     0,
      15,    16,    17,    47,     0,     0,     0,     0,     0,     0,
      48,     0,     0,    92,    27,     0,     0,     0,    49,   330,
      31,     0,    50,     0,     0,    36,    37,    38,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    88,    89,
      90,    91,    12,    13,    14,     0,    15,    16,    17,    47,
       0,     0,     0,     0,     0,     0,    48,     0,     0,    92,
      27,     0,     0,     0,    49,   333,    31,     0,    50,     0,
       0,    36,    37,    38,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    88,    89,    90,    91,   340,    13,
      14,     0,    15,    16,    17,    47,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    92,    27,     0,     0,     0,
      49,   338,    31,     0,    50,     0,     0,    36,    37,    38,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    45,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      88,    89,    90,    91,   340,    13,    14,     0,    15,    16,
      17,    47,     0,     0,     0,     0,     0,     0,    48,     0,
       0,    92,    27,     0,     0,     0,    49,   341,    31,     0,
      50,     0,     0,    36,    37,    38,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,    47,     0,     0,
       0,     0,     0,     0,    48,     0,     0,    92,    27,     0,
       0,     0,    49,   402,    31,     0,    50,     0,     0,    36,
      37,    38,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,   102,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,   104,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,    49,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,    49,    36,
      37,    38,   125,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,   287,     7,     0,     0,     0,    88,    89,    90,    91,
     288,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,   285,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,   287,     7,     0,     0,     0,    88,    89,    90,    91,
     288,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,   289,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    88,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,    27,     0,
      48,     0,     0,     0,    31,     0,     0,     0,    49,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   135,    89,    90,    91,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,    92,     0,     0,
      48,     0,     0,     0,    31,     0,     0,     0,   360,    36,
      37,    38,    50,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
      48,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,    50
};

static const yytype_int16 yycheck[] =
{
       2,    25,   144,    14,    14,    10,    80,   324,   325,    14,
      10,    10,    86,    26,    14,    14,    14,    10,    10,    17,
      94,    14,    14,    14,    26,    27,    47,    48,    36,    37,
      69,    22,    14,    35,    67,    68,    14,    39,    40,    10,
      88,    63,    64,    65,    22,     2,    56,   100,    14,   102,
      66,    88,    69,    14,    52,    69,    22,    99,   106,    92,
     102,    22,    50,    51,    21,    22,    54,   106,    25,   106,
      52,    28,    29,    30,    14,    92,    66,     0,    92,    57,
      10,    38,    22,    99,    41,   107,   102,    14,    59,    60,
      92,    57,    49,   106,   106,   106,    57,    86,   103,   107,
      86,    50,    51,   103,   103,    54,   108,   104,    86,    99,
     103,   103,   102,   102,     5,   106,   102,    57,    88,   107,
      86,    78,   100,    14,    81,    86,    83,    84,   202,    59,
      60,   205,   156,   157,   100,    81,   106,    94,   140,   100,
      86,   162,   163,   164,   165,   102,    86,   104,    86,   106,
     107,    67,    68,    99,    88,    10,   102,    86,   107,    14,
     100,    86,   119,   120,    12,   122,   123,   124,   125,   126,
     102,   100,   106,   490,   491,   100,    92,    70,    71,    72,
      73,    74,    75,   140,   102,   327,   210,   211,    85,    82,
     264,    86,    12,   267,    83,    88,    89,    90,    91,   273,
      86,   275,    86,   100,   101,   100,    95,    96,    97,    86,
      10,    11,    12,    10,   100,    10,   100,    14,    13,    14,
      15,    16,   102,   100,   226,   182,   183,    14,   185,   546,
      17,    93,    94,   190,   191,   102,   193,   106,   195,    76,
      77,    78,    67,    68,    14,    70,    71,    72,    73,    74,
      75,    76,    77,    78,   328,   100,   101,    82,    83,    84,
     217,   100,   219,    88,    89,    90,    91,    92,    93,    14,
      95,    96,    97,    31,    99,   100,   142,   143,   352,   145,
     146,   147,   148,   149,   150,    10,   152,   153,    13,    99,
      15,    16,    10,    11,    12,    61,    14,     7,     8,     9,
     302,   303,   304,   101,   100,   103,    14,   309,   101,   266,
     103,   101,   104,   103,   316,   317,   101,   101,   103,   103,
     101,   101,   103,   103,   101,   101,   103,   103,   285,   101,
      14,   103,   289,    14,   101,   292,   103,   294,   101,   296,
     103,   298,    84,   300,   418,   101,   101,   103,   103,   101,
     101,   103,   103,    14,   101,   429,   103,   101,   101,   103,
     103,   101,   101,   103,   103,   101,   101,   103,   103,   100,
     101,   100,   101,    90,    91,   377,   102,   379,   335,   336,
     337,   100,   101,   106,   100,   101,   100,   101,   100,   101,
      37,    38,   394,   350,   100,   101,   353,   354,   100,   101,
     154,   155,   476,   360,   100,   100,    47,   160,   161,   106,
     102,   100,   106,   103,   107,    89,   107,   105,   100,   102,
      14,   378,   102,    14,   100,   103,   103,    10,    14,    14,
     103,   433,   434,   435,   103,   107,   102,   102,    92,   441,
      79,    14,   102,   445,    86,    89,    86,   105,    86,   100,
     107,   100,    66,    89,   456,   457,    89,   414,    14,   107,
      14,   107,    27,   100,     3,     4,     5,     6,   100,   100,
     100,    10,    11,    12,    13,    14,    15,    16,   100,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   100,    10,
      29,    30,    31,    32,    33,    34,    35,   103,   103,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    10,    10,    10,    53,    54,   100,   474,   475,    58,
     522,   100,   524,    62,    27,    50,    14,    14,    14,   100,
     100,   100,   100,   100,    14,    66,   100,   100,   495,    66,
     100,   100,    14,   500,   102,   107,     7,   102,    87,   551,
     100,   553,   100,     7,     7,    94,    66,   514,    10,    10,
     100,   102,   100,   102,    14,    14,   568,   106,   107,    14,
      14,   100,   100,    14,   100,    14,   100,   100,   580,    14,
     537,     3,     4,     5,     6,   542,   100,   100,    10,    11,
      12,    13,    14,    15,    16,   100,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   100,   100,    29,    30,    31,
      32,    33,    34,    35,    14,   100,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     0,   100,
     100,    53,    54,   100,   100,   100,    58,   100,   100,   433,
      62,     3,     4,     5,     6,    -1,   289,   322,    10,    11,
      12,    13,    14,    15,    16,   159,    18,    19,    20,    -1,
     328,    -1,   158,    -1,    -1,    87,    -1,    -1,    -1,    31,
      32,    -1,    94,    -1,    -1,    -1,    38,    -1,    -1,    -1,
     102,    43,    44,    45,   106,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    31,
      32,    -1,    94,    -1,    -1,    -1,    38,    -1,   100,    -1,
     102,    43,    44,    45,   106,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    31,
      32,    -1,    94,    -1,    -1,    -1,    38,    -1,   100,    -1,
     102,    43,    44,    45,   106,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    31,    32,    -1,    -1,    -1,
     102,   103,    38,    -1,   106,    -1,    -1,    43,    44,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    87,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    31,    32,    -1,    -1,    -1,   102,   103,    38,    -1,
     106,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    31,    32,    -1,
      -1,    -1,   102,   103,    38,    -1,   106,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    61,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    31,    32,    -1,    -1,    -1,   102,   103,
      38,    -1,   106,    -1,    -1,    43,    44,    45,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    31,
      32,    -1,    -1,    -1,   102,   103,    38,    -1,   106,    -1,
      -1,    43,    44,    45,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    31,    32,    -1,    -1,    -1,
     102,   103,    38,    -1,   106,    -1,    -1,    43,    44,    45,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    87,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    31,    32,    -1,    -1,    -1,   102,   103,    38,    -1,
     106,    -1,    -1,    43,    44,    45,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    31,    32,    -1,
      -1,    -1,   102,   103,    38,    -1,   106,    -1,    -1,    43,
      44,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    32,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    31,    -1,    -1,
      94,    -1,    -1,    -1,    38,    -1,    -1,    -1,   102,    43,
      44,    45,   106,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,   106
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   109,   110,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    53,    54,    58,    62,    87,    94,   102,
     106,   111,   112,   113,   114,   115,   116,   117,   118,   120,
     121,   122,   125,   130,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   145,    81,    86,    99,
     102,   104,    88,   106,    86,    99,   102,    14,    10,    11,
      12,    13,    31,   100,   102,   130,   132,   144,    10,    11,
      12,    14,   102,   132,   102,   132,   102,   102,   132,   137,
      14,   130,   130,   132,   100,   132,   132,    12,    12,   102,
     102,   130,   106,   131,   131,   106,   131,   132,   130,   130,
     132,    14,    14,    31,    14,    10,   141,   141,   132,    61,
     110,   100,    76,    77,    78,    70,    71,    72,    73,    74,
      75,    82,    88,    89,    90,    91,    67,    68,    92,    84,
      93,    94,    83,    95,    96,    97,    99,    14,   132,    14,
     103,   144,   132,    14,   132,   132,    14,    17,    52,   103,
     144,   100,    86,   104,    88,   106,   103,   132,   144,   100,
     101,    86,   100,    86,   100,    86,   100,    10,    11,    12,
     132,   100,   101,   132,   100,   101,   132,   132,    69,   130,
      67,    68,   102,   100,   100,   106,    88,   106,    88,   106,
     132,   132,   132,   132,   132,   132,    47,   123,   106,    14,
      56,   106,   103,   107,   107,   135,   135,   133,   135,   135,
     135,   135,   135,   135,     7,     8,     9,   135,   135,   136,
     136,   137,   137,   138,   139,   140,   140,   141,   141,   141,
     141,    14,    17,   100,   102,   100,    86,   102,   103,   105,
      89,   107,   100,   102,    14,   102,   103,   132,   132,    14,
     132,   100,   103,   132,   132,   102,   132,     5,    14,   102,
     128,   132,    86,   100,    86,   100,    86,   100,   101,   144,
     101,   144,   103,   103,    10,   137,   137,    10,    14,   103,
     129,   119,    14,   132,    14,   132,   103,   103,   107,   130,
      14,    22,   106,   124,   102,   102,   126,    79,   102,    14,
     103,   144,   132,   103,   144,    86,    86,    86,   103,   144,
      14,   103,   127,   144,   105,    89,   107,   100,   100,   132,
      85,   100,   101,    66,    66,   128,   100,   101,   100,   132,
     102,   132,   132,   132,   100,   132,   100,   130,   130,   130,
      69,    69,    10,    59,    60,   130,   101,   103,    36,    37,
     107,    89,   107,    89,   107,   130,   130,    14,    14,   124,
      50,    51,    54,   107,   103,   129,   129,    63,    64,    65,
     107,   133,   103,   127,   144,   100,   103,   100,   100,   103,
     132,   132,   132,   103,    66,   101,   103,   103,   101,   132,
     144,   132,   132,   103,     5,    14,   100,   132,   100,   101,
     100,   103,   103,    27,    10,    10,    10,    10,    10,    14,
     130,   132,   130,   100,   100,    27,   107,    14,    52,    10,
      13,    14,    15,    16,    50,   130,   103,   103,    14,    14,
      14,   103,   103,   100,   100,   100,   100,   100,   132,    14,
     144,   100,   100,   100,    66,    66,   101,   144,   100,   100,
     112,   130,   130,   130,    10,    59,    60,   130,   107,   130,
     102,   102,    14,    22,    57,    86,   100,    14,    22,    57,
      86,   100,    10,    13,    15,    16,   100,   100,    14,   130,
     130,     7,     7,     7,    66,   103,   132,   132,   144,   100,
      10,    10,   103,   129,   103,   129,    14,    14,   100,   132,
      14,    14,   100,   132,    14,    22,    57,    86,   100,    14,
      22,    57,    86,   100,   100,   100,   102,   132,   100,   103,
     130,   103,   130,   103,   100,   100,   100,   100,   100,   100,
      14,    14,   100,   132,    14,    14,   100,   132,   103,   129,
     100,   130,   130,   100,   100,   100,   100,   100,   100,   130,
     103,   130
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   110,   110,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   112,
     113,   114,   114,   115,   116,   117,   117,   117,   117,   118,
     118,   119,   119,   120,   120,   120,   120,   120,   121,   122,
     123,   123,   123,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   125,   126,   126,   126,   126,   127,
     127,   128,   128,   128,   128,   129,   129,   129,   129,   129,
     129,   129,   129,   130,   130,   131,   132,   132,   132,   133,
     133,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   135,   135,   135,   136,   136,
     136,   137,   137,   138,   138,   139,   139,   139,   140,   140,
     140,   140,   140,   141,   141,   141,   142,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   144,   144,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     7,     9,     5,     5,     7,     6,     6,     8,    10,
       6,     4,     4,     4,     3,     2,     5,     4,     3,     3,
       5,     7,     3,     5,     7,     3,     3,     2,     4,     7,
       6,     4,     6,     4,     7,     6,     7,     7,     7,     6,
       6,     2,     1,     1,     1,     1,     2,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     7,     7,
       5,     5,     7,     5,     2,     7,     7,     5,     3,     5,
       7,     0,     4,     6,     5,     7,     6,     7,     6,     5,
       0,     3,     3,     0,     4,     4,     4,     5,     5,     4,
       5,     5,     6,     6,     7,     7,     6,     6,     6,     6,
       5,     5,     6,     6,     7,     7,     7,     7,     7,     6,
       7,     6,     8,     7,     5,     0,     4,     4,     4,     3,
       5,     3,     3,     5,     5,     1,     2,     3,     3,     3,
       4,     5,     5,     3,     3,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     2,     2,     1,     1,     4,     3,     4,
       3,     6,     6,     5,     6,     5,     4,     4,     6,     6,
       5,     3,     2,     3,     3,     2,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     5,     5,     1,     1,     1,     1,
       3,     1,     2
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
#line 251 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2041 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 261 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2047 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 262 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2053 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 266 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2059 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 268 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2065 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 270 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2071 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 272 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2077 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 274 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2083 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 276 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2091 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 280 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2098 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 283 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2106 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 287 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2112 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 289 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2118 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 291 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2124 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 293 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2130 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 295 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2136 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 297 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2144 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 301 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2152 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 305 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2158 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 307 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2164 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 309 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2170 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 311 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2176 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 313 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2182 "raku.tab.c"
    break;

  case 25: /* stmt: TESTOP ';'  */
#line 315 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2188 "raku.tab.c"
    break;

  case 26: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 317 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2194 "raku.tab.c"
    break;

  case 27: /* stmt: TESTOP '(' ')' ';'  */
#line 319 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2200 "raku.tab.c"
    break;

  case 28: /* stmt: TESTOP arg_list ';'  */
#line 321 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2206 "raku.tab.c"
    break;

  case 29: /* stmt: KW_SAY expr ';'  */
#line 323 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2212 "raku.tab.c"
    break;

  case 30: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 325 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2219 "raku.tab.c"
    break;

  case 31: /* stmt: KW_SAY '(' expr ',' expr ')' ';'  */
#line 328 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2225 "raku.tab.c"
    break;

  case 32: /* stmt: KW_PRINT expr ';'  */
#line 330 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2231 "raku.tab.c"
    break;

  case 33: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 332 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2238 "raku.tab.c"
    break;

  case 34: /* stmt: KW_PRINT '(' expr ',' expr ')' ';'  */
#line 335 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT_FH); expr_add_child(c,(yyvsp[-4].node)); expr_add_child(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2244 "raku.tab.c"
    break;

  case 35: /* stmt: KW_TAKE expr ';'  */
#line 337 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2250 "raku.tab.c"
    break;

  case 36: /* stmt: KW_RETURN expr ';'  */
#line 339 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2256 "raku.tab.c"
    break;

  case 37: /* stmt: KW_RETURN ';'  */
#line 341 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2262 "raku.tab.c"
    break;

  case 38: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 343 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2268 "raku.tab.c"
    break;

  case 39: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 345 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2277 "raku.tab.c"
    break;

  case 40: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 350 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2285 "raku.tab.c"
    break;

  case 41: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 354 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2293 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 358 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2302 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 363 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2310 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 367 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2321 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 374 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2330 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 379 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2337 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 382 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2344 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 385 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2351 "raku.tab.c"
    break;

  case 49: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 388 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2358 "raku.tab.c"
    break;

  case 50: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 391 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2365 "raku.tab.c"
    break;

  case 51: /* stmt: expr ';'  */
#line 393 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2371 "raku.tab.c"
    break;

  case 52: /* stmt: if_stmt  */
#line 394 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2377 "raku.tab.c"
    break;

  case 53: /* stmt: while_stmt  */
#line 395 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2383 "raku.tab.c"
    break;

  case 54: /* stmt: for_stmt  */
#line 396 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2389 "raku.tab.c"
    break;

  case 55: /* stmt: given_stmt  */
#line 397 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2395 "raku.tab.c"
    break;

  case 56: /* stmt: KW_TRY block  */
#line 399 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2401 "raku.tab.c"
    break;

  case 57: /* stmt: KW_TRY block KW_CATCH block  */
#line 401 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2407 "raku.tab.c"
    break;

  case 58: /* stmt: KW_CATCH block  */
#line 403 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2413 "raku.tab.c"
    break;

  case 59: /* stmt: block  */
#line 405 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2419 "raku.tab.c"
    break;

  case 60: /* stmt: unless_stmt  */
#line 406 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2425 "raku.tab.c"
    break;

  case 61: /* stmt: until_stmt  */
#line 407 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2431 "raku.tab.c"
    break;

  case 62: /* stmt: repeat_stmt  */
#line 408 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2437 "raku.tab.c"
    break;

  case 63: /* stmt: sub_decl  */
#line 409 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2443 "raku.tab.c"
    break;

  case 64: /* stmt: class_decl  */
#line 410 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2449 "raku.tab.c"
    break;

  case 65: /* stmt: role_decl  */
#line 411 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2455 "raku.tab.c"
    break;

  case 66: /* stmt: grammar_decl  */
#line 412 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2461 "raku.tab.c"
    break;

  case 67: /* if_stmt: KW_IF '(' expr ')' block  */
#line 416 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2467 "raku.tab.c"
    break;

  case 68: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 418 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2473 "raku.tab.c"
    break;

  case 69: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 420 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2479 "raku.tab.c"
    break;

  case 70: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 424 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2485 "raku.tab.c"
    break;

  case 71: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 428 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2491 "raku.tab.c"
    break;

  case 72: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 430 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2497 "raku.tab.c"
    break;

  case 73: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 434 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2503 "raku.tab.c"
    break;

  case 74: /* repeat_stmt: KW_REPEAT block  */
#line 438 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2509 "raku.tab.c"
    break;

  case 75: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 442 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2519 "raku.tab.c"
    break;

  case 76: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 448 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2529 "raku.tab.c"
    break;

  case 77: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 454 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2538 "raku.tab.c"
    break;

  case 78: /* for_stmt: KW_FOR expr block  */
#line 459 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2545 "raku.tab.c"
    break;

  case 79: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 464 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2557 "raku.tab.c"
    break;

  case 80: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 472 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2570 "raku.tab.c"
    break;

  case 81: /* when_list: %empty  */
#line 482 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2576 "raku.tab.c"
    break;

  case 82: /* when_list: when_list KW_WHEN expr block  */
#line 484 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2584 "raku.tab.c"
    break;

  case 83: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 490 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2596 "raku.tab.c"
    break;

  case 84: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 498 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2606 "raku.tab.c"
    break;

  case 85: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 504 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2619 "raku.tab.c"
    break;

  case 86: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 513 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2630 "raku.tab.c"
    break;

  case 87: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 520 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2643 "raku.tab.c"
    break;

  case 88: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 531 "raku.y"
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
#line 2661 "raku.tab.c"
    break;

  case 89: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 547 "raku.y"
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
#line 2678 "raku.tab.c"
    break;

  case 90: /* is_clauses: %empty  */
#line 561 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2684 "raku.tab.c"
    break;

  case 91: /* is_clauses: is_clauses IDENT IDENT  */
#line 563 "raku.y"
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
#line 2700 "raku.tab.c"
    break;

  case 92: /* is_clauses: is_clauses TESTOP IDENT  */
#line 575 "raku.y"
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
#line 2716 "raku.tab.c"
    break;

  case 93: /* class_body_list: %empty  */
#line 588 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2722 "raku.tab.c"
    break;

  case 94: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 590 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2729 "raku.tab.c"
    break;

  case 95: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 593 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2736 "raku.tab.c"
    break;

  case 96: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 596 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2743 "raku.tab.c"
    break;

  case 97: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 599 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2750 "raku.tab.c"
    break;

  case 98: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 602 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2757 "raku.tab.c"
    break;

  case 99: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 605 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2764 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 608 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2771 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 611 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2778 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 614 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2785 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 617 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2792 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 620 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2799 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 623 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2806 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 626 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2817 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 633 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2828 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 640 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2839 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 647 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2850 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 654 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2858 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 658 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2866 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 662 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2874 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 666 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2882 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 670 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2893 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 677 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2904 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 684 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2915 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 691 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2926 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 698 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2940 "raku.tab.c"
    break;

  case 119: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 708 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2952 "raku.tab.c"
    break;

  case 120: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 716 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2965 "raku.tab.c"
    break;

  case 121: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 725 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2976 "raku.tab.c"
    break;

  case 122: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 732 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 2990 "raku.tab.c"
    break;

  case 123: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 742 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3002 "raku.tab.c"
    break;

  case 124: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 752 "raku.y"
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
#line 3019 "raku.tab.c"
    break;

  case 125: /* grammar_body_list: %empty  */
#line 766 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3025 "raku.tab.c"
    break;

  case 126: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 768 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3034 "raku.tab.c"
    break;

  case 127: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 773 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3043 "raku.tab.c"
    break;

  case 128: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 778 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3052 "raku.tab.c"
    break;

  case 129: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 785 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3060 "raku.tab.c"
    break;

  case 130: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 789 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3068 "raku.tab.c"
    break;

  case 131: /* pair_list: IDENT OP_FATARROW expr  */
#line 795 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3074 "raku.tab.c"
    break;

  case 132: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 797 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3080 "raku.tab.c"
    break;

  case 133: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 799 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3086 "raku.tab.c"
    break;

  case 134: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 801 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3092 "raku.tab.c"
    break;

  case 135: /* param_list: VAR_SCALAR  */
#line 804 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3098 "raku.tab.c"
    break;

  case 136: /* param_list: IDENT VAR_SCALAR  */
#line 805 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3104 "raku.tab.c"
    break;

  case 137: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 806 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3110 "raku.tab.c"
    break;

  case 138: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 807 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3116 "raku.tab.c"
    break;

  case 139: /* param_list: param_list ',' VAR_SCALAR  */
#line 808 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3122 "raku.tab.c"
    break;

  case 140: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 809 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3128 "raku.tab.c"
    break;

  case 141: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 810 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3134 "raku.tab.c"
    break;

  case 142: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 811 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3140 "raku.tab.c"
    break;

  case 143: /* block: '{' stmt_list '}'  */
#line 814 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3146 "raku.tab.c"
    break;

  case 144: /* block: '{' YADA '}'  */
#line 815 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3152 "raku.tab.c"
    break;

  case 145: /* closure: '{' expr '}'  */
#line 818 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3158 "raku.tab.c"
    break;

  case 146: /* expr: VAR_SCALAR '=' expr  */
#line 821 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3164 "raku.tab.c"
    break;

  case 147: /* expr: KW_GATHER block  */
#line 822 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3174 "raku.tab.c"
    break;

  case 148: /* expr: tern_expr  */
#line 827 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3180 "raku.tab.c"
    break;

  case 149: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 831 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3186 "raku.tab.c"
    break;

  case 150: /* tern_expr: cmp_expr  */
#line 832 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3192 "raku.tab.c"
    break;

  case 151: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 835 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3198 "raku.tab.c"
    break;

  case 152: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 836 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3204 "raku.tab.c"
    break;

  case 153: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 837 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3210 "raku.tab.c"
    break;

  case 154: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 838 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3216 "raku.tab.c"
    break;

  case 155: /* cmp_expr: jct_expr '<' jct_expr  */
#line 839 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3222 "raku.tab.c"
    break;

  case 156: /* cmp_expr: jct_expr '>' jct_expr  */
#line 840 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3228 "raku.tab.c"
    break;

  case 157: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 841 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3234 "raku.tab.c"
    break;

  case 158: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 842 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3240 "raku.tab.c"
    break;

  case 159: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 843 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3246 "raku.tab.c"
    break;

  case 160: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 844 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3252 "raku.tab.c"
    break;

  case 161: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 846 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3262 "raku.tab.c"
    break;

  case 162: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 852 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3272 "raku.tab.c"
    break;

  case 163: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 858 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3282 "raku.tab.c"
    break;

  case 164: /* cmp_expr: jct_expr  */
#line 863 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3288 "raku.tab.c"
    break;

  case 165: /* jct_expr: jct_expr '|' range_expr  */
#line 866 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3294 "raku.tab.c"
    break;

  case 166: /* jct_expr: jct_expr '&' range_expr  */
#line 867 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3300 "raku.tab.c"
    break;

  case 167: /* jct_expr: range_expr  */
#line 868 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3306 "raku.tab.c"
    break;

  case 168: /* range_expr: add_expr OP_RANGE add_expr  */
#line 871 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3312 "raku.tab.c"
    break;

  case 169: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 872 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3318 "raku.tab.c"
    break;

  case 170: /* range_expr: add_expr  */
#line 873 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3324 "raku.tab.c"
    break;

  case 171: /* add_expr: add_expr '~' repl_expr  */
#line 876 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3330 "raku.tab.c"
    break;

  case 172: /* add_expr: repl_expr  */
#line 877 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 3336 "raku.tab.c"
    break;

  case 173: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 880 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3342 "raku.tab.c"
    break;

  case 174: /* repl_expr: addsub_expr  */
#line 881 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 3348 "raku.tab.c"
    break;

  case 175: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 884 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3354 "raku.tab.c"
    break;

  case 176: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 885 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3360 "raku.tab.c"
    break;

  case 177: /* addsub_expr: mul_expr  */
#line 886 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 3366 "raku.tab.c"
    break;

  case 178: /* mul_expr: mul_expr '*' unary_expr  */
#line 889 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3372 "raku.tab.c"
    break;

  case 179: /* mul_expr: mul_expr '/' unary_expr  */
#line 890 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3378 "raku.tab.c"
    break;

  case 180: /* mul_expr: mul_expr '%' unary_expr  */
#line 891 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3384 "raku.tab.c"
    break;

  case 181: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 892 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3390 "raku.tab.c"
    break;

  case 182: /* mul_expr: unary_expr  */
#line 893 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3396 "raku.tab.c"
    break;

  case 183: /* unary_expr: '-' unary_expr  */
#line 896 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3402 "raku.tab.c"
    break;

  case 184: /* unary_expr: '!' unary_expr  */
#line 897 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3408 "raku.tab.c"
    break;

  case 185: /* unary_expr: postfix_expr  */
#line 898 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3414 "raku.tab.c"
    break;

  case 186: /* postfix_expr: call_expr  */
#line 900 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3420 "raku.tab.c"
    break;

  case 187: /* call_expr: IDENT '(' arg_list ')'  */
#line 903 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3429 "raku.tab.c"
    break;

  case 188: /* call_expr: IDENT '(' ')'  */
#line 907 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3435 "raku.tab.c"
    break;

  case 189: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 909 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 190: /* call_expr: VAR_SCALAR '(' ')'  */
#line 914 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3450 "raku.tab.c"
    break;

  case 191: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 916 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3460 "raku.tab.c"
    break;

  case 192: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 922 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3470 "raku.tab.c"
    break;

  case 193: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 928 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3478 "raku.tab.c"
    break;

  case 194: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 932 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3489 "raku.tab.c"
    break;

  case 195: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 939 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3498 "raku.tab.c"
    break;

  case 196: /* call_expr: IDENT '.' CARET IDENT  */
#line 944 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3508 "raku.tab.c"
    break;

  case 197: /* call_expr: atom '.' CARET IDENT  */
#line 950 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3518 "raku.tab.c"
    break;

  case 198: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 956 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3529 "raku.tab.c"
    break;

  case 199: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 963 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3540 "raku.tab.c"
    break;

  case 200: /* call_expr: atom '.' IDENT '(' ')'  */
#line 970 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3549 "raku.tab.c"
    break;

  case 201: /* call_expr: atom '.' IDENT  */
#line 975 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3558 "raku.tab.c"
    break;

  case 202: /* call_expr: KW_DIE expr  */
#line 980 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3564 "raku.tab.c"
    break;

  case 203: /* call_expr: KW_MAP closure expr  */
#line 982 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3570 "raku.tab.c"
    break;

  case 204: /* call_expr: KW_GREP closure expr  */
#line 984 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3576 "raku.tab.c"
    break;

  case 205: /* call_expr: KW_SORT expr  */
#line 986 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3582 "raku.tab.c"
    break;

  case 206: /* call_expr: KW_SORT closure expr  */
#line 988 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3588 "raku.tab.c"
    break;

  case 207: /* call_expr: atom  */
#line 989 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3594 "raku.tab.c"
    break;

  case 208: /* arg_list: expr  */
#line 992 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3600 "raku.tab.c"
    break;

  case 209: /* arg_list: arg_list ',' expr  */
#line 993 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3606 "raku.tab.c"
    break;

  case 210: /* atom: LIT_INT  */
#line 996 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3612 "raku.tab.c"
    break;

  case 211: /* atom: LIT_FLOAT  */
#line 997 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3618 "raku.tab.c"
    break;

  case 212: /* atom: LIT_STR  */
#line 998 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3624 "raku.tab.c"
    break;

  case 213: /* atom: WORDLIST  */
#line 1000 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3636 "raku.tab.c"
    break;

  case 214: /* atom: LIT_INTERP_STR  */
#line 1007 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3642 "raku.tab.c"
    break;

  case 215: /* atom: VAR_SCALAR  */
#line 1008 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3648 "raku.tab.c"
    break;

  case 216: /* atom: VAR_ARRAY  */
#line 1009 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3654 "raku.tab.c"
    break;

  case 217: /* atom: VAR_HASH  */
#line 1010 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3660 "raku.tab.c"
    break;

  case 218: /* atom: VAR_CAPTURE  */
#line 1012 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3668 "raku.tab.c"
    break;

  case 219: /* atom: VAR_FH  */
#line 1016 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3676 "raku.tab.c"
    break;

  case 220: /* atom: VAR_NAMED_CAPTURE  */
#line 1020 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3683 "raku.tab.c"
    break;

  case 221: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1023 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3689 "raku.tab.c"
    break;

  case 222: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1025 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3695 "raku.tab.c"
    break;

  case 223: /* atom: VAR_HASH '{' expr '}'  */
#line 1027 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3701 "raku.tab.c"
    break;

  case 224: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1029 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3707 "raku.tab.c"
    break;

  case 225: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1031 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3713 "raku.tab.c"
    break;

  case 226: /* atom: IDENT  */
#line 1032 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3719 "raku.tab.c"
    break;

  case 227: /* atom: VAR_TWIGIL  */
#line 1034 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3727 "raku.tab.c"
    break;

  case 228: /* atom: VAR_ARRAY_TWIGIL  */
#line 1038 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3735 "raku.tab.c"
    break;

  case 229: /* atom: VAR_HASH_TWIGIL  */
#line 1042 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3743 "raku.tab.c"
    break;

  case 230: /* atom: '(' expr ')'  */
#line 1045 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3749 "raku.tab.c"
    break;

  case 231: /* atom: block  */
#line 1046 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3755 "raku.tab.c"
    break;

  case 232: /* atom: KW_SUB block  */
#line 1047 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3761 "raku.tab.c"
    break;


#line 3765 "raku.tab.c"

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

#line 1049 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
