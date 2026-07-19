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
  YYSYMBOL_KW_CONSTANT = 35,               /* KW_CONSTANT  */
  YYSYMBOL_KW_GIVEN = 36,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 37,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 38,                /* KW_DEFAULT  */
  YYSYMBOL_KW_EXISTS = 39,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 40,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 41,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 42,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 43,                 /* KW_REPEAT  */
  YYSYMBOL_KW_MAP = 44,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 45,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 46,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 47,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 48,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 49,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 50,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 51,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 52,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 53,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 54,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 55,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 56,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 57,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 58,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 59,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 60,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 61,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 62,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 63,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 64,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 65,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 66,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 67,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 68,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 69,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 70,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 71,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 72,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 73,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 74,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 75,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 76,                    /* OP_SNE  */
  YYSYMBOL_OP_AND = 77,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 78,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 79,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 80,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 81,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 82,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 83,                 /* OP_SMATCH  */
  YYSYMBOL_OP_DIV = 84,                    /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 85,                  /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 86,                 /* OP_REP_XX  */
  YYSYMBOL_87_ = 87,                       /* '='  */
  YYSYMBOL_88_ = 88,                       /* '!'  */
  YYSYMBOL_89_ = 89,                       /* '<'  */
  YYSYMBOL_90_ = 90,                       /* '>'  */
  YYSYMBOL_91_ = 91,                       /* '|'  */
  YYSYMBOL_92_ = 92,                       /* '&'  */
  YYSYMBOL_93_ = 93,                       /* '~'  */
  YYSYMBOL_94_ = 94,                       /* '+'  */
  YYSYMBOL_95_ = 95,                       /* '-'  */
  YYSYMBOL_96_ = 96,                       /* '*'  */
  YYSYMBOL_97_ = 97,                       /* '/'  */
  YYSYMBOL_98_ = 98,                       /* '%'  */
  YYSYMBOL_UMINUS = 99,                    /* UMINUS  */
  YYSYMBOL_100_ = 100,                     /* '.'  */
  YYSYMBOL_101_ = 101,                     /* ';'  */
  YYSYMBOL_102_ = 102,                     /* ','  */
  YYSYMBOL_103_ = 103,                     /* '('  */
  YYSYMBOL_104_ = 104,                     /* ')'  */
  YYSYMBOL_105_ = 105,                     /* '['  */
  YYSYMBOL_106_ = 106,                     /* ']'  */
  YYSYMBOL_107_ = 107,                     /* '{'  */
  YYSYMBOL_108_ = 108,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 109,                 /* $accept  */
  YYSYMBOL_program = 110,                  /* program  */
  YYSYMBOL_stmt_list = 111,                /* stmt_list  */
  YYSYMBOL_stmt = 112,                     /* stmt  */
  YYSYMBOL_if_stmt = 113,                  /* if_stmt  */
  YYSYMBOL_while_stmt = 114,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 115,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 116,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 117,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 118,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 119,               /* given_stmt  */
  YYSYMBOL_when_list = 120,                /* when_list  */
  YYSYMBOL_sub_decl = 121,                 /* sub_decl  */
  YYSYMBOL_sub_body = 122,                 /* sub_body  */
  YYSYMBOL_class_decl = 123,               /* class_decl  */
  YYSYMBOL_role_decl = 124,                /* role_decl  */
  YYSYMBOL_is_clauses = 125,               /* is_clauses  */
  YYSYMBOL_class_body_list = 126,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 127,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 128,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 129,           /* named_arg_list  */
  YYSYMBOL_pair_list = 130,                /* pair_list  */
  YYSYMBOL_param_list = 131,               /* param_list  */
  YYSYMBOL_block = 132,                    /* block  */
  YYSYMBOL_closure = 133,                  /* closure  */
  YYSYMBOL_expr = 134,                     /* expr  */
  YYSYMBOL_tern_expr = 135,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 136,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 137,                 /* jct_expr  */
  YYSYMBOL_range_expr = 138,               /* range_expr  */
  YYSYMBOL_add_expr = 139,                 /* add_expr  */
  YYSYMBOL_repl_expr = 140,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 141,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 142,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 143,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 144,             /* postfix_expr  */
  YYSYMBOL_call_expr = 145,                /* call_expr  */
  YYSYMBOL_arg_list = 146,                 /* arg_list  */
  YYSYMBOL_atom = 147                      /* atom  */
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
#define YYLAST   1960

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  245
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  623

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   342


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
       2,     2,     2,    88,     2,     2,     2,    98,    92,     2,
     103,   104,    96,    94,   102,    95,   100,    97,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   101,
      89,    87,    90,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   105,     2,   106,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   107,    91,   108,    93,     2,     2,     2,
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
      85,    86,    99
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   251,   251,   262,   263,   266,   268,   270,   272,   274,
     276,   280,   283,   287,   289,   291,   293,   295,   297,   301,
     305,   307,   309,   311,   313,   315,   317,   319,   321,   323,
     325,   327,   329,   331,   333,   336,   339,   341,   344,   347,
     349,   351,   353,   355,   360,   364,   368,   373,   377,   384,
     389,   392,   395,   398,   401,   404,   405,   406,   407,   408,
     409,   410,   412,   414,   416,   418,   419,   420,   421,   422,
     423,   424,   427,   429,   431,   435,   439,   441,   445,   449,
     453,   459,   465,   470,   475,   483,   494,   495,   501,   509,
     515,   521,   529,   535,   541,   550,   557,   566,   575,   584,
     585,   590,   606,   621,   622,   634,   648,   649,   652,   655,
     658,   661,   664,   667,   670,   673,   676,   679,   682,   685,
     692,   699,   706,   713,   717,   721,   725,   729,   736,   743,
     750,   757,   767,   775,   784,   791,   801,   811,   826,   827,
     832,   837,   844,   848,   854,   856,   858,   860,   864,   865,
     866,   867,   868,   869,   870,   871,   874,   875,   878,   881,
     882,   887,   890,   892,   895,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   911,   917,   923,   926,   927,
     928,   931,   932,   933,   936,   937,   940,   941,   944,   945,
     946,   949,   950,   951,   952,   953,   956,   957,   958,   960,
     962,   967,   968,   973,   975,   981,   987,   991,   998,  1003,
    1009,  1015,  1022,  1029,  1034,  1039,  1041,  1043,  1045,  1047,
    1049,  1052,  1053,  1056,  1057,  1058,  1059,  1067,  1068,  1069,
    1070,  1071,  1075,  1079,  1082,  1084,  1086,  1088,  1090,  1092,
    1093,  1097,  1101,  1105,  1106,  1107
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
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_CONSTANT", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_EXISTS", "KW_DELETE", "KW_UNLESS",
  "KW_UNTIL", "KW_REPEAT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY",
  "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW",
  "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_AND",
  "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_DIV", "OP_REP_X", "OP_REP_XX", "'='", "'!'", "'<'",
  "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "'.'", "';'", "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "$accept",
  "program", "stmt_list", "stmt", "if_stmt", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "sub_body", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "cmp_expr", "jct_expr", "range_expr", "add_expr", "repl_expr",
  "addsub_expr", "mul_expr", "unary_expr", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-236)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-245)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -236,    42,   822,  -236,  -236,  -236,  -236,  -236,    86,   -57,
     -38,    16,   195,  -236,  -236,  -236,  -236,  -236,   101,   385,
     173,  1405,  1465,    17,    56,  1517,    14,   149,  1517,   490,
      63,  1517,   158,   182,   175,   191,   149,   193,   193,  1577,
     149,   149,  1517,   289,   315,    27,  -236,   320,  1853,  1853,
    -236,  1517,   209,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,   603,   217,  -236,   236,   168,
    -236,     3,   296,   141,   169,  -236,  -236,  -236,   287,   324,
    1517,   388,   887,  1517,   393,  1517,  1517,    51,   939,   313,
     -20,   323,    53,  -236,   149,  -236,  1005,  -236,  -236,   116,
      70,    78,    87,   312,   408,   108,  1517,   145,  1517,   257,
    1517,  1517,   -33,    82,   130,  -236,  -236,   342,  -236,   344,
     359,   361,   345,    65,    73,  1517,  1517,  -236,  1517,  1517,
    1517,  1057,  1517,  -236,   405,  -236,  -236,  -236,   347,   352,
      19,   349,   354,  -236,  -236,   355,   350,   610,  -236,  1853,
    1853,  1853,  1853,  1853,  1853,  1853,  1853,  1853,   318,  1853,
    1853,  1853,  1853,  1853,  1853,  1853,  1853,  1853,  1853,  1853,
    1853,  1853,  1853,   285,   187,   360,    89,  -236,   202,   356,
     370,   357,   362,   363,   450,   364,  -236,   207,  -236,  1517,
    1517,   454,  1517,   371,   355,   228,  -236,  1517,  1517,  -236,
    1629,  -236,  1689,  -236,   119,   125,   129,   161,   387,   389,
     231,  -236,  1517,   237,  -236,  1517,   373,   375,   465,  -236,
    1853,  1853,    -4,  -236,  -236,  -236,  -236,  1517,  1517,  -236,
     467,  1517,   469,  1517,   380,   381,   379,  -236,  -236,  -236,
     149,    -5,  -236,    -3,   386,   394,  -236,  -236,  -236,  -236,
     171,   171,   410,   171,   171,   171,   171,   171,   171,  -236,
    -236,  -236,   171,   171,  -236,  -236,   398,   398,   296,   141,
     169,   169,  -236,  -236,  -236,  -236,   395,   485,  -236,  1117,
    -236,  1517,  1169,  -236,   420,   424,   425,  -236,  1235,  -236,
    1287,  -236,  -236,   407,   427,   406,  -236,   417,  -236,   418,
    1517,   123,   453,   -18,  1741,   275,   422,  1517,  -236,  1801,
    -236,  1517,  -236,     2,  -236,  1517,  1517,  1517,   291,  1517,
     325,   149,   149,   149,   -14,     0,  -236,    34,   419,   238,
     716,   423,   426,   -17,   435,   430,   438,   432,   149,   149,
    -236,  -236,   516,   517,  -236,     9,   419,   241,    21,   201,
     -19,  1853,  1353,  -236,   431,   242,   440,   442,   245,  1517,
    1517,  1517,  -236,   246,    61,  -236,   249,   252,  -236,  -236,
    -236,  -236,  -236,   253,  1517,  -236,  1517,  1517,  1517,   261,
    -236,   164,  -236,   443,  1517,   331,   444,   419,   262,   445,
     446,   265,  -236,   266,  -236,   506,  -236,  -236,   538,   540,
    -236,   541,   542,  -236,   279,   419,  -236,   106,  -236,  -236,
    1517,   149,  -236,  -236,  -236,   452,   456,   527,  -236,  -236,
    -236,    11,    22,   271,   504,  -236,  -236,   419,   419,   269,
     270,   544,   545,   546,  -236,  -236,  -236,   278,   282,  -236,
     461,  -236,  -236,   463,   464,   466,   468,  -236,  1517,   552,
    -236,  -236,  1517,   470,   334,  -236,  -236,   471,   501,   503,
    -236,   304,  -236,  1517,  -236,  -236,   419,  -236,  -236,   472,
     473,    -8,   149,   149,  -236,  -236,  -236,    95,  -236,  -236,
     149,   474,  -236,  -236,   149,  -236,   477,   478,    66,    76,
     239,   475,   476,   561,  -236,  -236,   419,   419,   576,   579,
     580,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,   521,
     308,  -236,  -236,  -236,  1517,  1517,  1517,   336,  -236,  -236,
    -236,  -236,  -236,  -236,  -236,  -236,   584,   585,  -236,  -236,
    -236,    40,    43,   575,   582,   497,  1517,  -236,   586,   587,
     498,  1517,  -236,    94,   144,   502,   505,  -236,  -236,   499,
    -236,  -236,  -236,  -236,  -236,  1517,   507,  -236,  -236,   311,
    -236,  -236,  -236,   149,   316,   149,   317,   508,   509,  -236,
     510,   511,   526,  -236,   536,   590,   591,   537,  1517,  -236,
     604,   633,   547,  1517,  -236,  -236,  -236,    45,  -236,  -236,
     560,  -236,   149,  -236,   149,  -236,  -236,  -236,  -236,  -236,
    -236,   562,   565,  -236,   566,   567,   569,  -236,   583,   149,
     321,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,  -236,
    -236,   149,  -236
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   223,   224,   225,   227,   228,   229,
     230,   240,   239,   241,   242,   231,   232,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
      56,     0,     3,     4,    57,    58,    65,    66,    67,    59,
      60,    68,    69,    70,    71,    64,     0,   161,   163,   177,
     180,   183,   185,   187,   190,   195,   198,   199,   220,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     228,   229,   230,   240,     0,    29,     0,   244,   221,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,     0,   245,   160,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,    79,     0,     0,
       0,     3,     0,   218,    61,    63,   215,   103,     0,     0,
       0,     0,   228,   197,   196,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,   203,     0,     0,
       0,     0,     0,     0,     0,     0,   201,     0,    24,     0,
       0,     0,     0,     0,   221,     0,    32,     0,     0,     6,
       0,     7,     0,     8,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,    36,     0,     0,     0,     0,    83,
       0,     0,     0,     3,    90,    39,    40,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,   216,   217,   219,
       0,     0,   106,     0,     0,     0,   138,   243,   157,   156,
     164,   165,     0,   166,   167,   170,   171,   172,   173,   174,
     175,   176,   168,   169,   178,   179,   181,   182,   184,   186,
     188,   189,   194,   191,   192,   193,   214,     0,    45,     0,
      42,     0,     0,   202,   234,   235,   236,    47,     0,   209,
       0,   200,   159,     0,     0,     0,    31,     0,   222,     0,
       0,     0,   225,   239,     0,     0,     0,     0,    21,     0,
      22,     0,    23,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   182,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,    62,     0,     0,   106,     0,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,   239,   206,     0,     0,   234,   235,
     236,    30,     5,     0,     0,     9,     0,     0,     0,     0,
      14,     0,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    37,    72,    75,    82,     0,     0,
     149,     0,     0,    89,     0,     0,    99,     0,    26,    25,
       0,     0,    84,   237,   238,     0,     0,    76,    78,   104,
     105,     0,     0,     0,     0,   102,    98,     0,     0,     0,
       0,     0,     0,     0,   137,   162,   213,     0,     0,    44,
       0,    46,    49,     0,     0,     0,     0,   207,     0,     0,
     204,   205,     0,     0,     0,   145,   144,     0,     0,     0,
      16,     0,    17,     0,    20,    92,     0,    28,    27,     0,
       0,     0,     0,     0,   150,   151,   152,     0,    88,   100,
       0,     0,    53,    54,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,    97,    95,     0,     0,     0,     0,
       0,   212,   211,    43,    48,    50,    51,    52,   142,     0,
       0,    11,    10,    15,     0,     0,     0,     0,    91,    35,
      38,    74,    73,    80,    81,   153,     0,     0,    87,    85,
      77,     0,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,   107,     0,     0,     0,     0,   108,   109,     0,
      94,    96,   139,   140,   141,     0,     0,   147,   146,     0,
      18,   154,   155,     0,     0,     0,     0,     0,     0,   123,
       0,     0,     0,   124,     0,     0,     0,     0,     0,   114,
       0,     0,     0,     0,   113,   110,   111,     0,   143,    12,
       0,   132,     0,   134,     0,   121,   122,   116,   119,   120,
     115,     0,     0,   125,     0,     0,     0,   126,     0,     0,
       0,    19,   131,   133,   129,   130,   118,   127,   128,   117,
     136,     0,   135
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -236,  -236,     4,  -236,   146,  -236,  -236,  -236,  -236,  -236,
    -236,  -236,  -236,  -205,  -236,  -236,  -236,   339,  -236,  -236,
     310,   400,  -235,    -2,   401,     1,  -146,  -236,   120,   280,
     -24,   520,   523,   283,   -34,  -236,  -236,   -69,  -236
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   147,    53,    54,    55,    56,    57,    58,    59,
      60,   333,    61,   224,    62,    63,   241,   345,    64,   350,
     366,   305,   329,    97,   129,    98,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    99,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      65,   113,   314,    66,     2,   252,   326,   326,   347,   342,
     327,   327,   326,   178,   143,   144,   327,   343,    23,   187,
     410,   411,   107,   109,   115,   116,   112,   195,   114,   117,
     119,   326,   122,   244,   127,   327,   486,   218,   134,   135,
     133,   139,     3,   136,   400,   431,   432,   433,    83,   378,
     326,    84,   145,   326,   327,   326,   398,   327,   140,   327,
     422,   423,   422,   423,   424,   183,   424,   189,   184,    85,
     399,   163,   164,   120,    52,   487,   245,   121,   388,   165,
     533,   175,    87,    82,   179,    88,   181,   182,   534,   434,
     538,   412,   115,   165,   401,   402,   165,   194,   539,    52,
     328,   346,   344,    86,   185,   525,   387,   210,   575,   213,
     219,   216,   217,   429,   430,    89,   576,   425,   208,   485,
     110,    52,   209,   403,   535,   428,   234,   235,   448,   236,
     237,   238,   236,   239,   540,   272,   273,   274,   275,   266,
     267,   426,   191,   318,   563,    65,   320,   565,    66,   609,
     220,   221,   577,   536,   230,   526,   527,   198,   580,   111,
     192,    87,   232,   541,    88,   200,   581,   537,    79,   458,
     123,   199,   231,    80,   202,   165,   281,   542,   459,   201,
     233,   578,   465,   100,   101,   102,    81,   103,   203,    82,
     292,   293,   282,   295,   124,   579,   324,   325,   298,   299,
     478,   301,   582,   306,   104,   435,   307,   148,   105,   374,
     355,   326,   309,   358,   479,   327,   311,   196,   197,   363,
     308,   367,   494,   495,   375,   376,   310,   330,   331,   332,
     312,   583,   335,   222,   337,   167,   168,   223,   341,   152,
     153,   154,   155,   156,   157,   584,   211,   212,   391,   543,
     393,   158,   544,   169,   545,   546,    52,   159,   160,   161,
     162,   518,   161,   162,   313,   170,   171,   172,   223,   250,
     251,   146,   253,   254,   255,   256,   257,   258,   125,   262,
     263,   488,   356,   438,   489,   490,   491,   492,   278,   476,
     279,   550,   551,   477,   126,    87,   564,   566,    88,   276,
     128,   373,   277,   137,   197,   145,   283,   454,   383,   197,
     385,   291,   386,   149,   150,   151,   389,   390,   148,   395,
     396,   397,   204,   205,   206,   259,   260,   261,    65,   138,
     197,   407,   297,   317,   141,   247,   417,   418,   174,   319,
     404,   247,   405,   404,   197,   427,   440,   197,   197,   443,
     447,   449,   610,   450,   197,   452,   451,   247,   214,   215,
     444,   445,   446,   381,   404,   457,   466,   197,   197,   469,
     470,   404,   404,   496,   497,   453,   380,   381,   455,   456,
     449,   166,   501,   510,   197,   461,   502,   173,     4,     5,
       6,     7,   392,   197,   517,    90,    91,    92,    93,    12,
      13,    14,   176,    15,    16,    17,   516,   180,   247,   481,
     197,   480,   556,   197,   188,   590,    94,    27,   404,   404,
     592,   594,   207,   404,    32,   621,   394,   197,   190,    37,
      38,    39,   462,   463,    42,   512,   197,   560,   197,   130,
     132,   264,   265,   225,    46,   226,   227,   559,   228,   508,
     270,   271,   229,   240,   242,   243,   246,    82,   248,   247,
     285,   280,   284,   287,   289,   286,   288,   290,   294,   522,
     523,   524,   296,    48,   315,   323,   316,   321,   528,   322,
      49,   334,   530,   336,   338,   339,    95,   340,    96,   348,
     351,   165,    52,     4,     5,     6,     7,   349,   352,   353,
      90,    91,    92,    93,    12,    13,    14,   359,    15,    16,
      17,   360,   361,   368,   370,   557,   558,   369,   371,   372,
     377,    94,    27,   382,   408,   413,   223,   409,   415,    32,
     419,   420,   439,   471,    37,    38,    39,   570,   414,    42,
     416,   441,   574,   442,   460,   464,   467,   468,   472,    46,
     473,   474,   475,   482,   484,   493,   588,   483,   498,   499,
     500,   591,   503,   593,   504,   505,   509,   506,   514,   507,
     515,   511,   513,   519,   520,   549,   547,   548,    48,   604,
     531,   532,   529,   552,   608,    49,   553,   554,   555,   567,
     612,   118,   613,    51,   561,   562,   568,    52,   569,   573,
     571,   572,   587,   585,   601,   602,   586,   620,   589,   595,
     596,   597,   598,     4,     5,     6,     7,   521,   605,   622,
       8,     9,    10,    11,    12,    13,    14,   599,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   600,   603,    24,
      25,    26,    27,    28,    29,    30,    31,   606,   607,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,   611,   437,   614,    44,    45,   615,   616,   617,    46,
     618,  -244,  -244,    47,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,   421,   619,   268,  -244,  -244,  -244,   269,
       0,     0,  -244,  -244,  -244,  -244,  -244,  -244,    48,  -244,
    -244,  -244,     0,  -244,   379,    49,     0,     0,     0,     0,
       0,    50,     0,    51,     0,     0,     0,    52,   249,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     0,     0,     0,
      44,    45,     0,     0,     0,    46,     0,     0,     0,    47,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,    50,     0,    51,
       0,     0,     0,    52,   406,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
       0,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     0,     0,     0,    44,    45,     0,     0,
       0,    46,     0,     0,     0,    47,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
      48,     0,     0,     0,     0,     0,     0,    49,    94,    27,
       0,     0,     0,    50,     0,    51,    32,     0,     0,    52,
       0,    37,    38,    39,     0,     0,    42,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    46,     0,     0,    90,
      91,    92,    93,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    27,     0,     0,     0,    48,     0,     0,    32,     0,
       0,     0,    49,    37,    38,    39,     0,     0,    42,     0,
      51,   177,     0,     0,    52,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,    48,     0,     0,
       0,     0,     0,     0,    49,     0,    94,    27,     0,     0,
       0,     0,    51,   186,    32,     0,    52,     0,     0,    37,
      38,    39,     0,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    27,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,    51,   193,
       0,     0,    52,     0,     0,     0,    46,     0,     0,   146,
       4,     5,     6,     7,     0,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,    94,    27,
       0,     0,    49,     0,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,    52,     0,    42,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    46,     0,     0,    90,
      91,    92,    93,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    27,     0,     0,     0,    48,     0,     0,    32,     0,
       0,     0,    49,    37,    38,    39,     0,     0,    42,     0,
      51,   354,     0,     0,    52,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,    48,     0,     0,
       0,     0,     0,     0,    49,     0,    94,    27,     0,     0,
       0,     0,    51,   357,    32,     0,    52,     0,     0,    37,
      38,    39,     0,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,    90,    91,    92,
      93,   364,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    27,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,    51,   362,
       0,     0,    52,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    90,    91,    92,    93,   364,    13,    14,
       0,    15,    16,    17,     0,    48,     0,     0,     0,     0,
       0,     0,    49,     0,    94,    27,     0,     0,     0,     0,
      51,   365,    32,     0,    52,     0,     0,    37,    38,    39,
       0,     0,    42,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    46,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    27,     0,     0,
       0,    48,     0,     0,    32,     0,     0,     0,    49,    37,
      38,    39,     0,     0,    42,     0,    51,   436,     0,     0,
      52,     0,     0,     0,    46,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    94,    27,     0,     0,
      49,     0,     0,     0,    32,     0,     0,     0,   106,    37,
      38,    39,    52,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    27,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,   108,     0,
       0,     0,    52,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,    94,    27,
       0,     0,    49,     0,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,    52,     0,    42,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    46,     0,     0,    90,
      91,    92,    93,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    27,     0,     0,     0,    48,     0,     0,    32,     0,
       0,     0,    49,    37,    38,    39,     0,     0,    42,     0,
      51,     0,     0,     0,   131,     0,     0,     0,    46,     0,
       0,     0,     4,     5,   302,     7,     0,     0,     0,    90,
      91,    92,    93,   303,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,    48,     0,     0,
      94,    27,     0,     0,    49,     0,     0,     0,    32,     0,
       0,     0,   300,    37,    38,    39,    52,     0,    42,     0,
       0,     0,     0,     0,     4,     5,   302,     7,    46,     0,
       0,    90,    91,    92,    93,   303,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,   304,     0,     0,     0,    52,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    90,    91,    92,    93,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    94,    27,     0,     0,    49,     0,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,    52,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,   142,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,   384,     0,     0,     0,    52,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
      52
};

static const yytype_int16 yycheck[] =
{
       2,    25,   207,     2,     0,   151,    10,    10,   243,    14,
      14,    14,    10,    82,    48,    49,    14,    22,    26,    88,
      37,    38,    21,    22,    26,    27,    25,    96,    14,    28,
      29,    10,    31,    14,    36,    14,    14,    70,    40,    41,
      39,    14,     0,    42,    10,    64,    65,    66,   105,    67,
      10,    89,    51,    10,    14,    10,    70,    14,    31,    14,
      51,    52,    51,    52,    55,    14,    55,    87,    17,   107,
      70,    68,    69,    10,   107,    53,    57,    14,   313,    93,
      14,    80,   100,   103,    83,   103,    85,    86,    22,   108,
      14,   108,    94,    93,    60,    61,    93,    96,    22,   107,
     104,   104,   107,    87,    53,    10,   104,   106,    14,   108,
     112,   110,   111,   348,   349,    14,    22,   108,    10,   108,
     103,   107,    14,   328,    58,   104,   125,   126,    67,   128,
     129,   130,   131,   132,    58,   169,   170,   171,   172,   163,
     164,   346,    89,   212,   104,   147,   215,   104,   147,   104,
      68,    69,    58,    87,    89,    60,    61,    87,    14,   103,
     107,   100,    89,    87,   103,    87,    22,   101,    82,     5,
      12,   101,   107,    87,    87,    93,    87,   101,    14,   101,
     107,    87,   387,    10,    11,    12,   100,    14,   101,   103,
     189,   190,   103,   192,    12,   101,   220,   221,   197,   198,
     405,   200,    58,   202,    31,   351,    87,   101,    35,    86,
     279,    10,    87,   282,   108,    14,    87,   101,   102,   288,
     101,   290,   427,   428,   101,   102,   101,   223,   227,   228,
     101,    87,   231,   103,   233,    94,    95,   107,   240,    71,
      72,    73,    74,    75,    76,   101,   101,   102,   317,    10,
     319,    83,    13,    84,    15,    16,   107,    89,    90,    91,
      92,   466,    91,    92,   103,    96,    97,    98,   107,   149,
     150,    62,   152,   153,   154,   155,   156,   157,   103,   159,
     160,    10,   281,   352,    13,    14,    15,    16,   101,    10,
     103,   496,   497,    14,   103,   100,   531,   532,   103,    14,
     107,   300,    17,    14,   102,   304,   104,   376,   307,   102,
     309,   104,   311,    77,    78,    79,   315,   316,   101,   321,
     322,   323,    10,    11,    12,     7,     8,     9,   330,    14,
     102,   330,   104,   102,    14,   104,   338,   339,    14,   102,
     102,   104,   104,   102,   102,   104,   104,   102,   102,   104,
     104,   102,   587,   104,   102,   102,   104,   104,   101,   102,
     359,   360,   361,   102,   102,   104,   104,   102,   102,   104,
     104,   102,   102,   104,   104,   374,   101,   102,   377,   378,
     102,    85,   104,   452,   102,   384,   104,   100,     3,     4,
       5,     6,   101,   102,   463,    10,    11,    12,    13,    14,
      15,    16,    14,    18,    19,    20,   102,    14,   104,   411,
     102,   410,   104,   102,   101,   104,    31,    32,   102,   102,
     104,   104,    14,   102,    39,   104,   101,   102,   105,    44,
      45,    46,   101,   102,    49,   101,   102,   101,   102,    38,
      39,   161,   162,   101,    59,   101,    87,   516,    87,   448,
     167,   168,   107,    48,   107,   103,   107,   103,   108,   104,
      90,   101,   106,   101,    14,   108,   103,   103,    14,   471,
     472,   473,   101,    88,    87,    10,    87,   104,   480,   104,
      95,    14,   484,    14,   104,   104,   101,   108,   103,   103,
      80,    93,   107,     3,     4,     5,     6,   103,   103,    14,
      10,    11,    12,    13,    14,    15,    16,    87,    18,    19,
      20,    87,    87,   106,   108,   514,   515,    90,   101,   101,
      67,    31,    32,   101,   101,    90,   107,   101,    90,    39,
      14,    14,   101,    27,    44,    45,    46,   536,   108,    49,
     108,   101,   541,   101,   101,   101,   101,   101,    10,    59,
      10,    10,    10,   101,    27,    51,   555,   101,    14,    14,
      14,   563,   101,   565,   101,   101,    14,   101,    67,   101,
      67,   101,   101,   101,   101,    14,   101,   101,    88,   578,
     103,   103,   108,     7,   583,    95,     7,     7,    67,    14,
     592,   101,   594,   103,    10,    10,    14,   107,   101,   101,
      14,    14,   103,   101,    14,    14,   101,   609,   101,   101,
     101,   101,   101,     3,     4,     5,     6,   471,    14,   621,
      10,    11,    12,    13,    14,    15,    16,   101,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   101,   101,    29,
      30,    31,    32,    33,    34,    35,    36,    14,   101,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   101,   352,   101,    54,    55,   101,   101,   101,    59,
     101,    68,    69,    63,    71,    72,    73,    74,    75,    76,
      77,    78,    79,   344,   101,   165,    83,    84,    85,   166,
      -1,    -1,    89,    90,    91,    92,    93,    94,    88,    96,
      97,    98,    -1,   100,   304,    95,    -1,    -1,    -1,    -1,
      -1,   101,    -1,   103,    -1,    -1,    -1,   107,   108,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,   101,    -1,   103,
      -1,    -1,    -1,   107,   108,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    31,    32,
      -1,    -1,    -1,   101,    -1,   103,    39,    -1,    -1,   107,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,
      -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    31,    32,    -1,    -1,
      -1,    -1,   103,   104,    39,    -1,   107,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    59,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,
      95,    44,    45,    46,    -1,    -1,    49,    -1,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,    62,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    31,    32,
      -1,    -1,    95,    -1,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,   107,    -1,    49,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,
      -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    31,    32,    -1,    -1,
      -1,    -1,   103,   104,    39,    -1,   107,    -1,    -1,    44,
      45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    59,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,
      95,    44,    45,    46,    -1,    -1,    49,    -1,   103,   104,
      -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    31,    32,    -1,    -1,    -1,    -1,
     103,   104,    39,    -1,   107,    -1,    -1,    44,    45,    46,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    59,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    88,    -1,    -1,    39,    -1,    -1,    -1,    95,    44,
      45,    46,    -1,    -1,    49,    -1,   103,   104,    -1,    -1,
     107,    -1,    -1,    -1,    59,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    31,    32,    -1,    -1,
      95,    -1,    -1,    -1,    39,    -1,    -1,    -1,   103,    44,
      45,    46,   107,    -1,    49,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    59,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,
      95,    44,    45,    46,    -1,    -1,    49,    -1,   103,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    31,    32,
      -1,    -1,    95,    -1,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,   107,    -1,    49,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,
      -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      31,    32,    -1,    -1,    95,    -1,    -1,    -1,    39,    -1,
      -1,    -1,   103,    44,    45,    46,   107,    -1,    49,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,
      39,    -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,
      49,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    31,    32,    -1,    -1,    95,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,   107,    -1,
      49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      59,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
     107
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   110,   111,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    36,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    54,    55,    59,    63,    88,    95,
     101,   103,   107,   112,   113,   114,   115,   116,   117,   118,
     119,   121,   123,   124,   127,   132,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   147,    82,
      87,   100,   103,   105,    89,   107,    87,   100,   103,    14,
      10,    11,    12,    13,    31,   101,   103,   132,   134,   146,
      10,    11,    12,    14,    31,    35,   103,   134,   103,   134,
     103,   103,   134,   139,    14,   132,   132,   134,   101,   134,
      10,    14,   134,    12,    12,   103,   103,   132,   107,   133,
     133,   107,   133,   134,   132,   132,   134,    14,    14,    14,
      31,    14,    10,   143,   143,   134,    62,   111,   101,    77,
      78,    79,    71,    72,    73,    74,    75,    76,    83,    89,
      90,    91,    92,    68,    69,    93,    85,    94,    95,    84,
      96,    97,    98,   100,    14,   134,    14,   104,   146,   134,
      14,   134,   134,    14,    17,    53,   104,   146,   101,    87,
     105,    89,   107,   104,   134,   146,   101,   102,    87,   101,
      87,   101,    87,   101,    10,    11,    12,    14,    10,    14,
     134,   101,   102,   134,   101,   102,   134,   134,    70,   132,
      68,    69,   103,   107,   122,   101,   101,    87,    87,   107,
      89,   107,    89,   107,   134,   134,   134,   134,   134,   134,
      48,   125,   107,   103,    14,    57,   107,   104,   108,   108,
     137,   137,   135,   137,   137,   137,   137,   137,   137,     7,
       8,     9,   137,   137,   138,   138,   139,   139,   140,   141,
     142,   142,   143,   143,   143,   143,    14,    17,   101,   103,
     101,    87,   103,   104,   106,    90,   108,   101,   103,    14,
     103,   104,   134,   134,    14,   134,   101,   104,   134,   134,
     103,   134,     5,    14,   103,   130,   134,    87,   101,    87,
     101,    87,   101,   103,   122,    87,    87,   102,   146,   102,
     146,   104,   104,    10,   139,   139,    10,    14,   104,   131,
     111,   134,   134,   120,    14,   134,    14,   134,   104,   104,
     108,   132,    14,    22,   107,   126,   104,   131,   103,   103,
     128,    80,   103,    14,   104,   146,   134,   104,   146,    87,
      87,    87,   104,   146,    14,   104,   129,   146,   106,    90,
     108,   101,   101,   134,    86,   101,   102,    67,    67,   130,
     101,   102,   101,   134,   103,   134,   134,   104,   131,   134,
     134,   146,   101,   146,   101,   132,   132,   132,    70,    70,
      10,    60,    61,   122,   102,   104,   108,   134,   101,   101,
      37,    38,   108,    90,   108,    90,   108,   132,   132,    14,
      14,   126,    51,    52,    55,   108,   122,   104,   104,   131,
     131,    64,    65,    66,   108,   135,   104,   129,   146,   101,
     104,   101,   101,   104,   134,   134,   134,   104,    67,   102,
     104,   104,   102,   134,   146,   134,   134,   104,     5,    14,
     101,   134,   101,   102,   101,   122,   104,   101,   101,   104,
     104,    27,    10,    10,    10,    10,    10,    14,   122,   108,
     134,   132,   101,   101,    27,   108,    14,    53,    10,    13,
      14,    15,    16,    51,   122,   122,   104,   104,    14,    14,
      14,   104,   104,   101,   101,   101,   101,   101,   134,    14,
     146,   101,   101,   101,    67,    67,   102,   146,   122,   101,
     101,   113,   132,   132,   132,    10,    60,    61,   132,   108,
     132,   103,   103,    14,    22,    58,    87,   101,    14,    22,
      58,    87,   101,    10,    13,    15,    16,   101,   101,    14,
     122,   122,     7,     7,     7,    67,   104,   134,   134,   146,
     101,    10,    10,   104,   131,   104,   131,    14,    14,   101,
     134,    14,    14,   101,   134,    14,    22,    58,    87,   101,
      14,    22,    58,    87,   101,   101,   101,   103,   134,   101,
     104,   132,   104,   132,   104,   101,   101,   101,   101,   101,
     101,    14,    14,   101,   134,    14,    14,   101,   134,   104,
     131,   101,   132,   132,   101,   101,   101,   101,   101,   101,
     132,   104,   132
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   109,   110,   111,   111,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   113,   113,   113,   114,   115,   115,   116,   117,
     118,   118,   118,   118,   119,   119,   120,   120,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   122,
     122,   123,   124,   125,   125,   125,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   127,   128,   128,
     128,   128,   129,   129,   130,   130,   130,   130,   131,   131,
     131,   131,   131,   131,   131,   131,   132,   132,   133,   134,
     134,   134,   135,   135,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   137,   137,
     137,   138,   138,   138,   139,   139,   140,   140,   141,   141,
     141,   142,   142,   142,   142,   142,   143,   143,   143,   144,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   146,   146,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     7,     9,     5,     5,     7,     6,     6,     8,    10,
       6,     4,     4,     4,     3,     5,     5,     6,     6,     2,
       5,     4,     3,     3,     5,     7,     3,     5,     7,     3,
       3,     2,     4,     7,     6,     4,     6,     4,     7,     6,
       7,     7,     7,     6,     6,     2,     1,     1,     1,     1,
       1,     2,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     7,     7,     5,     5,     7,     5,     2,
       7,     7,     5,     3,     5,     7,     0,     4,     6,     5,
       3,     7,     6,     4,     7,     6,     7,     6,     5,     3,
       4,     6,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       6,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       7,     7,     6,     7,     6,     8,     7,     5,     0,     4,
       4,     4,     3,     5,     3,     3,     5,     5,     1,     2,
       3,     3,     3,     4,     5,     5,     3,     3,     3,     3,
       2,     1,     5,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     2,     2,     1,     1,
       4,     3,     4,     3,     6,     6,     5,     6,     5,     4,
       4,     6,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     5,     5,     1,
       1,     1,     1,     3,     1,     2
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
#line 252 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2064 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 262 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2070 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 263 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2076 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 267 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2082 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 269 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2088 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 271 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2094 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 273 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2100 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 275 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2106 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2114 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 281 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2121 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 284 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2129 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 288 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2135 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 290 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2141 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 292 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2147 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 294 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2153 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 296 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2159 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 298 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2167 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 302 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2175 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 306 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2181 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 308 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2187 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 310 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2193 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 312 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2199 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 314 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2205 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 316 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2211 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 318 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2217 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 320 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2223 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 322 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2229 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 324 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2235 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 326 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2241 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 328 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2247 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 330 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2253 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 332 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2259 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 334 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2266 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 337 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2273 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 340 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2279 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 342 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2286 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 345 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2293 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 348 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2299 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 350 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2305 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 352 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2311 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2317 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 356 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2326 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 361 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2334 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 365 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2342 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 369 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2351 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 374 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2359 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 378 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2370 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 385 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2379 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 390 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2386 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 393 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2393 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 396 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2400 "raku.tab.c"
    break;

  case 53: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2407 "raku.tab.c"
    break;

  case 54: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2414 "raku.tab.c"
    break;

  case 55: /* stmt: expr ';'  */
#line 404 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2420 "raku.tab.c"
    break;

  case 56: /* stmt: ';'  */
#line 405 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 2426 "raku.tab.c"
    break;

  case 57: /* stmt: if_stmt  */
#line 406 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2432 "raku.tab.c"
    break;

  case 58: /* stmt: while_stmt  */
#line 407 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2438 "raku.tab.c"
    break;

  case 59: /* stmt: for_stmt  */
#line 408 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2444 "raku.tab.c"
    break;

  case 60: /* stmt: given_stmt  */
#line 409 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2450 "raku.tab.c"
    break;

  case 61: /* stmt: KW_TRY block  */
#line 411 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2456 "raku.tab.c"
    break;

  case 62: /* stmt: KW_TRY block KW_CATCH block  */
#line 413 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2462 "raku.tab.c"
    break;

  case 63: /* stmt: KW_CATCH block  */
#line 415 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2468 "raku.tab.c"
    break;

  case 64: /* stmt: block  */
#line 417 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2474 "raku.tab.c"
    break;

  case 65: /* stmt: unless_stmt  */
#line 418 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2480 "raku.tab.c"
    break;

  case 66: /* stmt: until_stmt  */
#line 419 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2486 "raku.tab.c"
    break;

  case 67: /* stmt: repeat_stmt  */
#line 420 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2492 "raku.tab.c"
    break;

  case 68: /* stmt: sub_decl  */
#line 421 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2498 "raku.tab.c"
    break;

  case 69: /* stmt: class_decl  */
#line 422 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2504 "raku.tab.c"
    break;

  case 70: /* stmt: role_decl  */
#line 423 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2510 "raku.tab.c"
    break;

  case 71: /* stmt: grammar_decl  */
#line 424 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2516 "raku.tab.c"
    break;

  case 72: /* if_stmt: KW_IF '(' expr ')' block  */
#line 428 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2522 "raku.tab.c"
    break;

  case 73: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 430 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2528 "raku.tab.c"
    break;

  case 74: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 432 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2534 "raku.tab.c"
    break;

  case 75: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 436 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2540 "raku.tab.c"
    break;

  case 76: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 440 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2546 "raku.tab.c"
    break;

  case 77: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 442 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2552 "raku.tab.c"
    break;

  case 78: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 446 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2558 "raku.tab.c"
    break;

  case 79: /* repeat_stmt: KW_REPEAT block  */
#line 450 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2564 "raku.tab.c"
    break;

  case 80: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 454 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2574 "raku.tab.c"
    break;

  case 81: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 460 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2584 "raku.tab.c"
    break;

  case 82: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 466 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2593 "raku.tab.c"
    break;

  case 83: /* for_stmt: KW_FOR expr block  */
#line 471 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2600 "raku.tab.c"
    break;

  case 84: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 476 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2612 "raku.tab.c"
    break;

  case 85: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 484 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2625 "raku.tab.c"
    break;

  case 86: /* when_list: %empty  */
#line 494 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2631 "raku.tab.c"
    break;

  case 87: /* when_list: when_list KW_WHEN expr block  */
#line 496 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2639 "raku.tab.c"
    break;

  case 88: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 502 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2651 "raku.tab.c"
    break;

  case 89: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 510 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2661 "raku.tab.c"
    break;

  case 90: /* sub_decl: KW_SUB IDENT sub_body  */
#line 516 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2671 "raku.tab.c"
    break;

  case 91: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 522 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2683 "raku.tab.c"
    break;

  case 92: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 530 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2693 "raku.tab.c"
    break;

  case 93: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 536 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2703 "raku.tab.c"
    break;

  case 94: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 542 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2716 "raku.tab.c"
    break;

  case 95: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 551 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2727 "raku.tab.c"
    break;

  case 96: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 558 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2740 "raku.tab.c"
    break;

  case 97: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 567 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2753 "raku.tab.c"
    break;

  case 98: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 576 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2764 "raku.tab.c"
    break;

  case 99: /* sub_body: '{' stmt_list '}'  */
#line 584 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2770 "raku.tab.c"
    break;

  case 100: /* sub_body: '{' stmt_list expr '}'  */
#line 586 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 2777 "raku.tab.c"
    break;

  case 101: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 591 "raku.y"
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
#line 2795 "raku.tab.c"
    break;

  case 102: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 607 "raku.y"
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
#line 2812 "raku.tab.c"
    break;

  case 103: /* is_clauses: %empty  */
#line 621 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2818 "raku.tab.c"
    break;

  case 104: /* is_clauses: is_clauses IDENT IDENT  */
#line 623 "raku.y"
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
#line 2834 "raku.tab.c"
    break;

  case 105: /* is_clauses: is_clauses TESTOP IDENT  */
#line 635 "raku.y"
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
#line 2850 "raku.tab.c"
    break;

  case 106: /* class_body_list: %empty  */
#line 648 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2856 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 650 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2863 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 653 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2870 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 656 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2877 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 659 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2884 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 662 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2891 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 665 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2898 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 668 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2905 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 671 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2912 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 674 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2919 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 677 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2926 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 680 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2933 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 683 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2940 "raku.tab.c"
    break;

  case 119: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 686 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2951 "raku.tab.c"
    break;

  case 120: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 693 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2962 "raku.tab.c"
    break;

  case 121: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 700 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2973 "raku.tab.c"
    break;

  case 122: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 707 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2984 "raku.tab.c"
    break;

  case 123: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 714 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2992 "raku.tab.c"
    break;

  case 124: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 718 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3000 "raku.tab.c"
    break;

  case 125: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 722 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3008 "raku.tab.c"
    break;

  case 126: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 726 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3016 "raku.tab.c"
    break;

  case 127: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 730 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3027 "raku.tab.c"
    break;

  case 128: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 737 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3038 "raku.tab.c"
    break;

  case 129: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 744 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3049 "raku.tab.c"
    break;

  case 130: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 751 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3060 "raku.tab.c"
    break;

  case 131: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 758 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3074 "raku.tab.c"
    break;

  case 132: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 768 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3086 "raku.tab.c"
    break;

  case 133: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 776 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3099 "raku.tab.c"
    break;

  case 134: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 785 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3110 "raku.tab.c"
    break;

  case 135: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 792 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3124 "raku.tab.c"
    break;

  case 136: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 802 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3136 "raku.tab.c"
    break;

  case 137: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 812 "raku.y"
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
#line 3153 "raku.tab.c"
    break;

  case 138: /* grammar_body_list: %empty  */
#line 826 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3159 "raku.tab.c"
    break;

  case 139: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 828 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3168 "raku.tab.c"
    break;

  case 140: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 833 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3177 "raku.tab.c"
    break;

  case 141: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 838 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3186 "raku.tab.c"
    break;

  case 142: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 845 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3194 "raku.tab.c"
    break;

  case 143: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 849 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3202 "raku.tab.c"
    break;

  case 144: /* pair_list: IDENT OP_FATARROW expr  */
#line 855 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3208 "raku.tab.c"
    break;

  case 145: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 857 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3214 "raku.tab.c"
    break;

  case 146: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 859 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3220 "raku.tab.c"
    break;

  case 147: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 861 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3226 "raku.tab.c"
    break;

  case 148: /* param_list: VAR_SCALAR  */
#line 864 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3232 "raku.tab.c"
    break;

  case 149: /* param_list: IDENT VAR_SCALAR  */
#line 865 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3238 "raku.tab.c"
    break;

  case 150: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 866 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3244 "raku.tab.c"
    break;

  case 151: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 867 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3250 "raku.tab.c"
    break;

  case 152: /* param_list: param_list ',' VAR_SCALAR  */
#line 868 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3256 "raku.tab.c"
    break;

  case 153: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 869 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3262 "raku.tab.c"
    break;

  case 154: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 870 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3268 "raku.tab.c"
    break;

  case 155: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 871 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3274 "raku.tab.c"
    break;

  case 156: /* block: '{' stmt_list '}'  */
#line 874 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3280 "raku.tab.c"
    break;

  case 157: /* block: '{' YADA '}'  */
#line 875 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3286 "raku.tab.c"
    break;

  case 158: /* closure: '{' expr '}'  */
#line 878 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3292 "raku.tab.c"
    break;

  case 159: /* expr: VAR_SCALAR '=' expr  */
#line 881 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3298 "raku.tab.c"
    break;

  case 160: /* expr: KW_GATHER block  */
#line 882 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3308 "raku.tab.c"
    break;

  case 161: /* expr: tern_expr  */
#line 887 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3314 "raku.tab.c"
    break;

  case 162: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 891 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3320 "raku.tab.c"
    break;

  case 163: /* tern_expr: cmp_expr  */
#line 892 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3326 "raku.tab.c"
    break;

  case 164: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 895 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3332 "raku.tab.c"
    break;

  case 165: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 896 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3338 "raku.tab.c"
    break;

  case 166: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 897 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3344 "raku.tab.c"
    break;

  case 167: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 898 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3350 "raku.tab.c"
    break;

  case 168: /* cmp_expr: jct_expr '<' jct_expr  */
#line 899 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3356 "raku.tab.c"
    break;

  case 169: /* cmp_expr: jct_expr '>' jct_expr  */
#line 900 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3362 "raku.tab.c"
    break;

  case 170: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 901 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3368 "raku.tab.c"
    break;

  case 171: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 902 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3374 "raku.tab.c"
    break;

  case 172: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 903 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3380 "raku.tab.c"
    break;

  case 173: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 904 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3386 "raku.tab.c"
    break;

  case 174: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 906 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3396 "raku.tab.c"
    break;

  case 175: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 912 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3406 "raku.tab.c"
    break;

  case 176: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 918 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3416 "raku.tab.c"
    break;

  case 177: /* cmp_expr: jct_expr  */
#line 923 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3422 "raku.tab.c"
    break;

  case 178: /* jct_expr: jct_expr '|' range_expr  */
#line 926 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3428 "raku.tab.c"
    break;

  case 179: /* jct_expr: jct_expr '&' range_expr  */
#line 927 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3434 "raku.tab.c"
    break;

  case 180: /* jct_expr: range_expr  */
#line 928 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3440 "raku.tab.c"
    break;

  case 181: /* range_expr: add_expr OP_RANGE add_expr  */
#line 931 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3446 "raku.tab.c"
    break;

  case 182: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 932 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3452 "raku.tab.c"
    break;

  case 183: /* range_expr: add_expr  */
#line 933 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3458 "raku.tab.c"
    break;

  case 184: /* add_expr: add_expr '~' repl_expr  */
#line 936 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3464 "raku.tab.c"
    break;

  case 185: /* add_expr: repl_expr  */
#line 937 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 3470 "raku.tab.c"
    break;

  case 186: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 940 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3476 "raku.tab.c"
    break;

  case 187: /* repl_expr: addsub_expr  */
#line 941 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 3482 "raku.tab.c"
    break;

  case 188: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 944 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3488 "raku.tab.c"
    break;

  case 189: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 945 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3494 "raku.tab.c"
    break;

  case 190: /* addsub_expr: mul_expr  */
#line 946 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 3500 "raku.tab.c"
    break;

  case 191: /* mul_expr: mul_expr '*' unary_expr  */
#line 949 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3506 "raku.tab.c"
    break;

  case 192: /* mul_expr: mul_expr '/' unary_expr  */
#line 950 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3512 "raku.tab.c"
    break;

  case 193: /* mul_expr: mul_expr '%' unary_expr  */
#line 951 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3518 "raku.tab.c"
    break;

  case 194: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 952 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3524 "raku.tab.c"
    break;

  case 195: /* mul_expr: unary_expr  */
#line 953 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3530 "raku.tab.c"
    break;

  case 196: /* unary_expr: '-' unary_expr  */
#line 956 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3536 "raku.tab.c"
    break;

  case 197: /* unary_expr: '!' unary_expr  */
#line 957 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3542 "raku.tab.c"
    break;

  case 198: /* unary_expr: postfix_expr  */
#line 958 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3548 "raku.tab.c"
    break;

  case 199: /* postfix_expr: call_expr  */
#line 960 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3554 "raku.tab.c"
    break;

  case 200: /* call_expr: IDENT '(' arg_list ')'  */
#line 963 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3563 "raku.tab.c"
    break;

  case 201: /* call_expr: IDENT '(' ')'  */
#line 967 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3569 "raku.tab.c"
    break;

  case 202: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 969 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3578 "raku.tab.c"
    break;

  case 203: /* call_expr: VAR_SCALAR '(' ')'  */
#line 974 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3584 "raku.tab.c"
    break;

  case 204: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 976 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3594 "raku.tab.c"
    break;

  case 205: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 982 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3604 "raku.tab.c"
    break;

  case 206: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 988 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3612 "raku.tab.c"
    break;

  case 207: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 992 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3623 "raku.tab.c"
    break;

  case 208: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 999 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3632 "raku.tab.c"
    break;

  case 209: /* call_expr: IDENT '.' CARET IDENT  */
#line 1004 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3642 "raku.tab.c"
    break;

  case 210: /* call_expr: atom '.' CARET IDENT  */
#line 1010 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3652 "raku.tab.c"
    break;

  case 211: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1016 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3663 "raku.tab.c"
    break;

  case 212: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1023 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3674 "raku.tab.c"
    break;

  case 213: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1030 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3683 "raku.tab.c"
    break;

  case 214: /* call_expr: atom '.' IDENT  */
#line 1035 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3692 "raku.tab.c"
    break;

  case 215: /* call_expr: KW_DIE expr  */
#line 1040 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3698 "raku.tab.c"
    break;

  case 216: /* call_expr: KW_MAP closure expr  */
#line 1042 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3704 "raku.tab.c"
    break;

  case 217: /* call_expr: KW_GREP closure expr  */
#line 1044 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3710 "raku.tab.c"
    break;

  case 218: /* call_expr: KW_SORT expr  */
#line 1046 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3716 "raku.tab.c"
    break;

  case 219: /* call_expr: KW_SORT closure expr  */
#line 1048 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3722 "raku.tab.c"
    break;

  case 220: /* call_expr: atom  */
#line 1049 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3728 "raku.tab.c"
    break;

  case 221: /* arg_list: expr  */
#line 1052 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3734 "raku.tab.c"
    break;

  case 222: /* arg_list: arg_list ',' expr  */
#line 1053 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3740 "raku.tab.c"
    break;

  case 223: /* atom: LIT_INT  */
#line 1056 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 224: /* atom: LIT_FLOAT  */
#line 1057 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3752 "raku.tab.c"
    break;

  case 225: /* atom: LIT_STR  */
#line 1058 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3758 "raku.tab.c"
    break;

  case 226: /* atom: WORDLIST  */
#line 1060 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3770 "raku.tab.c"
    break;

  case 227: /* atom: LIT_INTERP_STR  */
#line 1067 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3776 "raku.tab.c"
    break;

  case 228: /* atom: VAR_SCALAR  */
#line 1068 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3782 "raku.tab.c"
    break;

  case 229: /* atom: VAR_ARRAY  */
#line 1069 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3788 "raku.tab.c"
    break;

  case 230: /* atom: VAR_HASH  */
#line 1070 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3794 "raku.tab.c"
    break;

  case 231: /* atom: VAR_CAPTURE  */
#line 1072 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3802 "raku.tab.c"
    break;

  case 232: /* atom: VAR_FH  */
#line 1076 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3810 "raku.tab.c"
    break;

  case 233: /* atom: VAR_NAMED_CAPTURE  */
#line 1080 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3817 "raku.tab.c"
    break;

  case 234: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1083 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3823 "raku.tab.c"
    break;

  case 235: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1085 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3829 "raku.tab.c"
    break;

  case 236: /* atom: VAR_HASH '{' expr '}'  */
#line 1087 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3835 "raku.tab.c"
    break;

  case 237: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1089 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3841 "raku.tab.c"
    break;

  case 238: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1091 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3847 "raku.tab.c"
    break;

  case 239: /* atom: IDENT  */
#line 1092 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3853 "raku.tab.c"
    break;

  case 240: /* atom: VAR_TWIGIL  */
#line 1094 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3861 "raku.tab.c"
    break;

  case 241: /* atom: VAR_ARRAY_TWIGIL  */
#line 1098 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3869 "raku.tab.c"
    break;

  case 242: /* atom: VAR_HASH_TWIGIL  */
#line 1102 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3877 "raku.tab.c"
    break;

  case 243: /* atom: '(' expr ')'  */
#line 1105 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3883 "raku.tab.c"
    break;

  case 244: /* atom: block  */
#line 1106 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3889 "raku.tab.c"
    break;

  case 245: /* atom: KW_SUB block  */
#line 1107 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3895 "raku.tab.c"
    break;


#line 3899 "raku.tab.c"

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

#line 1109 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
