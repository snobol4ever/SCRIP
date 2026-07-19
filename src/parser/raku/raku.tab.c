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
  YYSYMBOL_class_decl = 122,               /* class_decl  */
  YYSYMBOL_role_decl = 123,                /* role_decl  */
  YYSYMBOL_is_clauses = 124,               /* is_clauses  */
  YYSYMBOL_class_body_list = 125,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 126,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 127,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 128,           /* named_arg_list  */
  YYSYMBOL_pair_list = 129,                /* pair_list  */
  YYSYMBOL_param_list = 130,               /* param_list  */
  YYSYMBOL_block = 131,                    /* block  */
  YYSYMBOL_closure = 132,                  /* closure  */
  YYSYMBOL_expr = 133,                     /* expr  */
  YYSYMBOL_tern_expr = 134,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 135,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 136,                 /* jct_expr  */
  YYSYMBOL_range_expr = 137,               /* range_expr  */
  YYSYMBOL_add_expr = 138,                 /* add_expr  */
  YYSYMBOL_repl_expr = 139,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 140,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 141,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 142,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 143,             /* postfix_expr  */
  YYSYMBOL_call_expr = 144,                /* call_expr  */
  YYSYMBOL_arg_list = 145,                 /* arg_list  */
  YYSYMBOL_atom = 146                      /* atom  */
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
#define YYLAST   1804

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  238
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  608

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
     409,   411,   413,   415,   417,   418,   419,   420,   421,   422,
     423,   426,   428,   430,   434,   438,   440,   444,   448,   452,
     458,   464,   469,   474,   482,   493,   494,   500,   508,   514,
     522,   528,   537,   544,   555,   571,   586,   587,   599,   613,
     614,   617,   620,   623,   626,   629,   632,   635,   638,   641,
     644,   647,   650,   657,   664,   671,   678,   682,   686,   690,
     694,   701,   708,   715,   722,   732,   740,   749,   756,   766,
     776,   791,   792,   797,   802,   809,   813,   819,   821,   823,
     825,   829,   830,   831,   832,   833,   834,   835,   836,   839,
     840,   843,   846,   847,   852,   855,   857,   860,   861,   862,
     863,   864,   865,   866,   867,   868,   869,   870,   876,   882,
     888,   891,   892,   893,   896,   897,   898,   901,   902,   905,
     906,   909,   910,   911,   914,   915,   916,   917,   918,   921,
     922,   923,   925,   927,   932,   933,   938,   940,   946,   952,
     956,   963,   968,   974,   980,   987,   994,   999,  1004,  1006,
    1008,  1010,  1012,  1014,  1017,  1018,  1021,  1022,  1023,  1024,
    1032,  1033,  1034,  1035,  1036,  1040,  1044,  1047,  1049,  1051,
    1053,  1055,  1057,  1058,  1062,  1066,  1070,  1071,  1072
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

#define YYPACT_NINF (-308)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-238)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -308,    47,   606,  -308,  -308,  -308,  -308,  -308,   100,   -64,
     -14,     0,    86,  -308,  -308,  -308,  -308,  -308,    84,   278,
     148,  1249,  1309,    48,    58,  1361,   -10,    27,  1361,   671,
     210,  1361,    98,   134,    85,   129,    27,   107,   107,  1421,
      27,    27,  1361,   228,   293,   237,  -308,   304,  1697,  1697,
    1361,   286,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  1686,   279,  -308,   182,   101,  -308,
     -39,    20,   184,   110,  -308,  -308,  -308,   225,   326,  1361,
     350,   731,  1361,   369,  1361,  1361,    32,   783,   296,   -47,
     288,    13,  -308,    27,  -308,   849,  -308,  -308,   294,   111,
     117,   128,   275,   398,   211,  1361,   298,  1361,   300,  1361,
    1361,   -50,    43,   311,  -308,  -308,   325,  -308,   329,   344,
     345,   327,    59,    74,  1361,  1361,  -308,  1361,  1361,  1361,
     901,  1361,  -308,   385,  -308,  -308,  -308,   328,     7,   330,
     333,  -308,  -308,   334,   332,   479,  -308,  1697,  1697,  1697,
    1697,  1697,  1697,  1697,  1697,  1697,   292,  1697,  1697,  1697,
    1697,  1697,  1697,  1697,  1697,  1697,  1697,  1697,  1697,  1697,
    1697,   263,    41,   340,   -36,  -308,    76,   336,   353,   337,
     343,   346,   432,   347,  -308,   170,  -308,  1361,  1361,   433,
    1361,   351,   334,   200,  -308,  1361,  1361,  -308,  1473,  -308,
    1533,  -308,   132,   136,   144,   352,   361,   366,   201,  -308,
    1361,   204,  -308,  1361,   355,   356,   444,  -308,  1697,  1697,
      -4,  -308,  -308,  1361,  1361,  -308,   447,  1361,   448,  1361,
     359,   362,   357,  -308,  -308,  -308,    27,    -3,  -308,   364,
     367,  -308,  -308,  -308,  -308,   220,   220,   389,   220,   220,
     220,   220,   220,   220,  -308,  -308,  -308,   220,   220,  -308,
    -308,   378,   378,    20,   184,   110,   110,  -308,  -308,  -308,
    -308,   370,   458,  -308,   961,  -308,  1361,  1013,  -308,   387,
     390,   391,  -308,  1079,  -308,  1131,  -308,  -308,   375,   386,
     379,  -308,   395,  -308,   405,  1361,   115,   419,   -30,  1585,
     302,   406,  1361,  -308,  1645,  -308,  1361,  -308,     2,  1361,
    1361,  1361,   305,  1361,   309,    27,    27,    27,   -27,   -15,
    -308,    53,    27,   217,   415,   416,    22,   440,   380,   441,
     424,    27,    27,  -308,  -308,   521,   522,  -308,    25,     3,
     212,    91,  1697,  1197,  -308,   436,   224,   442,   443,   229,
    1361,  1361,  1361,  -308,   230,   -19,  -308,   234,   239,  -308,
    -308,  -308,  -308,  -308,   243,  1361,  -308,  1361,  1361,  1361,
     249,  -308,    78,  -308,   446,  1361,   315,   449,    27,   250,
     450,   451,   253,  -308,   254,  -308,   514,  -308,  -308,   535,
     539,  -308,   543,   544,  -308,   252,    27,  -308,  -308,  1361,
      27,  -308,  -308,  -308,   454,   455,   530,  -308,  -308,  -308,
      44,    21,   242,   507,  -308,    27,   257,   258,   545,   546,
     548,  -308,  -308,  -308,   261,   266,  -308,   462,  -308,  -308,
     463,   464,   467,   468,  -308,  1361,   556,  -308,  -308,  1361,
     470,   317,  -308,  -308,   471,   506,   509,  -308,   267,  -308,
    1361,  -308,  -308,    27,  -308,  -308,   477,   478,   -21,    27,
      27,  -308,  -308,  -308,    93,  -308,    27,   473,  -308,  -308,
      27,  -308,   480,   481,    14,    30,   260,   484,   487,   575,
    -308,    27,    27,   583,   584,   586,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,   527,   270,  -308,  -308,  -308,  1361,
    1361,  1361,   319,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,   585,   587,  -308,  -308,  -308,     4,    12,   582,   588,
     497,  1361,  -308,   591,   592,   498,  1361,  -308,    31,    36,
     499,   500,  -308,  -308,   504,  -308,  -308,  -308,  -308,  -308,
    1361,   512,  -308,  -308,   273,  -308,  -308,  -308,    27,   280,
      27,   284,   513,   532,  -308,   533,   542,   557,  -308,   558,
     601,   609,   561,  1361,  -308,   630,   643,   562,  1361,  -308,
    -308,  -308,    17,  -308,  -308,   563,  -308,    27,  -308,    27,
    -308,  -308,  -308,  -308,  -308,  -308,   565,   566,  -308,   567,
     569,   570,  -308,   571,    27,   285,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,    27,  -308
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,   216,   217,   218,   220,   221,   222,
     223,   233,   232,   234,   235,   224,   225,   226,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   219,     0,     0,     0,
       0,     3,     4,    56,    57,    64,    65,    66,    58,    59,
      67,    68,    69,    70,    63,     0,   154,   156,   170,   173,
     176,   178,   180,   183,   188,   191,   192,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
     222,   223,   233,     0,    29,     0,   237,   214,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,   238,   153,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       3,     0,   211,    60,    62,   208,    96,     0,     0,     0,
     221,   190,   189,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,     0,     0,
       0,     0,     0,     0,   194,     0,    24,     0,     0,     0,
       0,     0,   214,     0,    32,     0,     0,     6,     0,     7,
       0,     8,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,    36,     0,     0,     0,     0,    82,     0,     0,
       0,    39,    40,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,   209,   210,   212,     0,     0,    99,     0,
       0,   131,   236,   150,   149,   157,   158,     0,   159,   160,
     163,   164,   165,   166,   167,   168,   169,   161,   162,   171,
     172,   174,   175,   177,   179,   181,   182,   187,   184,   185,
     186,   207,     0,    45,     0,    42,     0,     0,   195,   227,
     228,   229,    47,     0,   202,     0,   193,   152,     0,     0,
       0,    31,     0,   215,     0,     0,     0,   218,   232,     0,
       0,     0,     0,    21,     0,    22,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   174,   175,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   151,    61,     0,     0,    99,     0,     0,
       0,     0,     0,     0,   203,     0,     0,     0,     0,     0,
       0,     0,     0,   201,     0,   232,   199,     0,     0,   227,
     228,   229,    30,     5,     0,     0,     9,     0,     0,     0,
       0,    14,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    37,    71,    74,    81,     0,
       0,   142,     0,     0,    88,     0,     0,    26,    25,     0,
       0,    83,   230,   231,     0,     0,    75,    77,    97,    98,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,   130,   155,   206,     0,     0,    44,     0,    46,    49,
       0,     0,     0,     0,   200,     0,     0,   197,   198,     0,
       0,     0,   138,   137,     0,     0,     0,    16,     0,    17,
       0,    20,    90,     0,    28,    27,     0,     0,     0,     0,
       0,   143,   144,   145,     0,    87,     0,     0,    53,    54,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,   205,   204,    43,    48,
      50,    51,    52,   135,     0,     0,    11,    10,    15,     0,
       0,     0,     0,    89,    35,    38,    73,    72,    79,    80,
     146,     0,     0,    86,    84,    76,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,   100,     0,     0,
       0,     0,   101,   102,     0,    91,    93,   132,   133,   134,
       0,     0,   140,   139,     0,    18,   147,   148,     0,     0,
       0,     0,     0,     0,   116,     0,     0,     0,   117,     0,
       0,     0,     0,     0,   107,     0,     0,     0,     0,   106,
     103,   104,     0,   136,    12,     0,   125,     0,   127,     0,
     114,   115,   109,   112,   113,   108,     0,     0,   118,     0,
       0,     0,   119,     0,     0,     0,    19,   124,   126,   122,
     123,   111,   120,   121,   110,   129,     0,   128
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -308,  -308,   673,  -308,   221,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,   341,  -308,  -308,   349,
     381,  -307,    -2,   384,    40,  -147,  -308,    96,   265,   -22,
     525,   529,   262,   -41,  -308,  -308,   -72,  -308
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   145,    52,    53,    54,    55,    56,    57,    58,
      59,   326,    60,    61,    62,   237,   338,    63,   341,   357,
     300,   323,    96,   128,    97,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    98,    77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,   379,   247,   112,   113,    23,   320,   141,   142,   176,
     321,   335,   320,   320,   320,   185,   321,   321,   321,   336,
     216,   239,   320,   193,   114,   115,   321,   320,   518,   161,
     162,   321,   416,   417,   126,   472,   519,   369,   133,   134,
     187,    82,    65,   389,   523,   560,   181,     3,   435,   182,
     565,   276,   524,   561,   163,   390,    81,    51,   566,   399,
     400,   106,   108,   391,   240,   111,   163,   277,   116,   118,
      86,   121,   520,    87,   473,    83,   411,   412,   163,   132,
     413,    86,   135,   445,    87,   183,    51,    85,   525,   562,
     143,   114,   446,    84,   567,   411,   412,    51,    88,   413,
     322,   521,   189,   510,   337,   164,   378,   415,   548,   217,
     122,   218,   219,   392,   393,   522,   550,   526,   563,   173,
     190,   594,   177,   568,   179,   180,   267,   268,   269,   270,
     401,   527,   564,   414,    51,   192,   163,   569,   312,   261,
     262,   314,   273,    64,   274,   208,   123,   211,   226,   214,
     215,   109,   471,   511,   512,   418,   419,   420,    99,   100,
     101,   110,   102,   228,   230,   231,   227,   232,   233,   234,
     232,   235,   150,   151,   152,   153,   154,   155,   195,   103,
     278,   229,    78,   104,   156,    65,    86,    79,   124,    87,
     157,   158,   159,   160,   167,   422,   318,   319,   196,   421,
      80,   365,   346,    81,   198,   349,   168,   169,   170,   549,
     551,   354,   197,   358,   127,   200,   366,   367,   199,   302,
     119,   206,   320,   304,   120,   207,   321,   287,   288,   201,
     290,   306,   125,   303,   334,   293,   294,   305,   296,   382,
     301,   384,   136,   245,   246,   307,   248,   249,   250,   251,
     252,   253,   474,   257,   258,   475,   476,   477,   478,   147,
     148,   149,   463,   324,   325,   595,   464,   328,   138,   330,
     528,   425,   195,   529,   286,   530,   531,   271,   165,   166,
     272,     4,     5,     6,     7,   202,   203,   204,    89,    90,
      91,    92,    12,    13,    14,   441,    15,    16,    17,   254,
     255,   256,   195,   311,   292,   242,   313,   137,   242,    93,
      27,   159,   160,   386,   387,   388,   347,    32,   139,   395,
     394,   396,    37,    38,    39,   171,   195,    42,   427,   406,
     407,   195,   195,   430,   434,   364,   436,    46,   437,   143,
     172,   195,   374,   438,   376,   439,   377,   242,   144,   380,
     381,   372,   395,   444,   453,   195,   195,   456,   457,   395,
     395,   481,   482,   436,   174,   486,    48,   495,   195,   501,
     487,   242,   195,    49,   541,   195,   452,   575,   502,    94,
     146,    95,   395,   178,   577,    51,   395,   395,   579,   606,
     431,   432,   433,   188,   465,   194,   195,   186,   467,   209,
     210,   212,   213,   371,   372,   440,   383,   195,   442,   443,
     385,   195,   205,   480,   220,   448,   449,   450,   497,   195,
     545,   195,   129,   131,   259,   260,   221,   265,   266,   544,
     222,   223,   224,   236,   225,   238,    81,   241,   242,   466,
     243,   275,   279,   280,   282,   281,   284,   289,   309,   283,
     285,   503,   291,   310,   317,   308,   507,   508,   509,   315,
     316,   327,   329,   331,   513,   333,   332,   339,   515,   342,
     340,   163,   344,   343,   350,   493,   360,   351,   352,   535,
     536,   359,     4,     5,     6,     7,   368,   361,   403,     8,
       9,    10,    11,    12,    13,    14,   362,    15,    16,    17,
      18,    19,    20,    21,    22,    23,   363,   373,    24,    25,
      26,    27,    28,    29,    30,    31,   397,   398,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
     402,   404,   405,    44,    45,   408,   409,   426,    46,   542,
     543,   458,    47,   428,   429,   459,   576,   447,   578,   460,
     451,   454,   455,   461,   462,   468,   469,   470,   479,   483,
     484,   555,   485,   488,   489,   490,   559,    48,   491,   492,
     494,   496,   498,   499,    49,   597,   500,   598,   504,   505,
     573,   514,    50,   516,   517,   532,    51,   244,   533,   534,
     537,   538,   605,   539,   540,   546,   552,   547,   554,   558,
     570,   571,   553,   589,   607,   556,   557,   572,   593,     4,
       5,     6,     7,   574,   580,   586,     8,     9,    10,    11,
      12,    13,    14,   587,    15,    16,    17,    18,    19,    20,
      21,    22,    23,   581,   582,    24,    25,    26,    27,    28,
      29,    30,    31,   583,   590,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,   591,   584,   585,
      44,    45,   588,   592,   596,    46,   599,   600,   601,    47,
     602,   603,   604,     2,     4,     5,     6,     7,   410,   506,
     370,    89,    90,    91,    92,    12,    13,    14,   263,    15,
      16,    17,   424,   264,    48,     0,     0,     0,     0,     0,
       0,    49,    93,    27,     0,     0,     0,     0,     0,    50,
      32,     0,     0,    51,     0,    37,    38,    39,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    89,    90,    91,    92,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    93,    27,     0,     0,    49,     0,     0,     0,
      32,     0,   117,     0,    50,    37,    38,    39,    51,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    89,    90,    91,    92,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,    50,   175,     0,     0,    51,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    89,
      90,    91,    92,    12,    13,    14,     0,    15,    16,    17,
       0,    48,     0,     0,     0,     0,     0,     0,    49,     0,
      93,    27,     0,     0,     0,     0,    50,   184,    32,     0,
      51,     0,     0,    37,    38,    39,     0,     0,    42,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    46,     0,
       0,    89,    90,    91,    92,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,    50,   191,     0,     0,    51,     0,     0,     0,
      46,     0,     0,   144,     4,     5,     6,     7,     0,     0,
       0,    89,    90,    91,    92,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    93,    27,     0,     0,    49,     0,     0,     0,
      32,     0,     0,     0,    50,    37,    38,    39,    51,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    89,    90,    91,    92,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,    50,   345,     0,     0,    51,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    89,
      90,    91,    92,    12,    13,    14,     0,    15,    16,    17,
       0,    48,     0,     0,     0,     0,     0,     0,    49,     0,
      93,    27,     0,     0,     0,     0,    50,   348,    32,     0,
      51,     0,     0,    37,    38,    39,     0,     0,    42,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    46,     0,
       0,    89,    90,    91,    92,   355,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,    50,   353,     0,     0,    51,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    89,    90,    91,
      92,   355,    13,    14,     0,    15,    16,    17,     0,    48,
       0,     0,     0,     0,     0,     0,    49,     0,    93,    27,
       0,     0,     0,     0,    50,   356,    32,     0,    51,     0,
       0,    37,    38,    39,     0,     0,    42,     0,     0,     0,
       0,     0,     4,     5,     6,     7,    46,     0,     0,    89,
      90,    91,    92,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    27,     0,     0,     0,    48,     0,     0,    32,     0,
       0,     0,    49,    37,    38,    39,     0,     0,    42,     0,
      50,   423,     0,     0,    51,     0,     0,     0,    46,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    89,
      90,    91,    92,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,    48,     0,     0,
      93,    27,     0,     0,    49,     0,     0,     0,    32,     0,
       0,     0,   105,    37,    38,    39,    51,     0,    42,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    46,     0,
       0,    89,    90,    91,    92,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,   107,     0,     0,     0,    51,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    89,    90,    91,    92,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    93,    27,     0,     0,    49,     0,     0,     0,
      32,     0,     0,     0,    50,    37,    38,    39,    51,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    89,    90,    91,    92,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,    50,     0,     0,     0,   130,     0,
       0,     0,    46,     0,     0,     0,     4,     5,   297,     7,
       0,     0,     0,    89,    90,    91,    92,   298,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,    93,    27,     0,     0,    49,     0,
       0,     0,    32,     0,     0,     0,   295,    37,    38,    39,
      51,     0,    42,     0,     0,     0,     0,     0,     4,     5,
     297,     7,    46,     0,     0,    89,    90,    91,    92,   298,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    27,     0,     0,
       0,    48,     0,     0,    32,     0,     0,     0,    49,    37,
      38,    39,     0,     0,    42,     0,   299,     0,     0,     0,
      51,     0,     0,     0,    46,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    89,    90,    91,    92,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    93,    27,     0,     0,
      49,     0,     0,     0,    32,     0,     0,     0,    50,    37,
      38,    39,    51,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,   140,    90,    91,
      92,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,   375,     0,
       0,     0,    51,     0,  -237,  -237,    46,  -237,  -237,  -237,
    -237,  -237,  -237,  -237,  -237,  -237,     0,     0,     0,  -237,
    -237,  -237,     0,     0,     0,  -237,  -237,  -237,  -237,  -237,
    -237,     0,  -237,  -237,  -237,    48,  -237,  -237,     0,     0,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,    51
};

static const yytype_int16 yycheck[] =
{
       2,   308,   149,    25,    14,    26,    10,    48,    49,    81,
      14,    14,    10,    10,    10,    87,    14,    14,    14,    22,
      70,    14,    10,    95,    26,    27,    14,    10,    14,    68,
      69,    14,   339,   340,    36,    14,    22,    67,    40,    41,
      87,   105,     2,    70,    14,    14,    14,     0,    67,    17,
      14,    87,    22,    22,    93,    70,   103,   107,    22,    37,
      38,    21,    22,    10,    57,    25,    93,   103,    28,    29,
     100,    31,    58,   103,    53,    89,    51,    52,    93,    39,
      55,   100,    42,     5,   103,    53,   107,    87,    58,    58,
      50,    93,    14,   107,    58,    51,    52,   107,    14,    55,
     104,    87,    89,    10,   107,    85,   104,   104,   104,   111,
      12,    68,    69,    60,    61,   101,   104,    87,    87,    79,
     107,   104,    82,    87,    84,    85,   167,   168,   169,   170,
     108,   101,   101,   108,   107,    95,    93,   101,   210,   161,
     162,   213,   101,   145,   103,   105,    12,   107,    89,   109,
     110,   103,   108,    60,    61,    64,    65,    66,    10,    11,
      12,   103,    14,    89,   124,   125,   107,   127,   128,   129,
     130,   131,    71,    72,    73,    74,    75,    76,   102,    31,
     104,   107,    82,    35,    83,   145,   100,    87,   103,   103,
      89,    90,    91,    92,    84,   342,   218,   219,    87,   108,
     100,    86,   274,   103,    87,   277,    96,    97,    98,   516,
     517,   283,   101,   285,   107,    87,   101,   102,   101,    87,
      10,    10,    10,    87,    14,    14,    14,   187,   188,   101,
     190,    87,   103,   101,   236,   195,   196,   101,   198,   311,
     200,   313,    14,   147,   148,   101,   150,   151,   152,   153,
     154,   155,    10,   157,   158,    13,    14,    15,    16,    77,
      78,    79,    10,   223,   224,   572,    14,   227,    31,   229,
      10,   343,   102,    13,   104,    15,    16,    14,    94,    95,
      17,     3,     4,     5,     6,    10,    11,    12,    10,    11,
      12,    13,    14,    15,    16,   367,    18,    19,    20,     7,
       8,     9,   102,   102,   104,   104,   102,    14,   104,    31,
      32,    91,    92,   315,   316,   317,   276,    39,    14,   102,
     322,   104,    44,    45,    46,   100,   102,    49,   104,   331,
     332,   102,   102,   104,   104,   295,   102,    59,   104,   299,
      14,   102,   302,   104,   304,   102,   306,   104,    62,   309,
     310,   102,   102,   104,   104,   102,   102,   104,   104,   102,
     102,   104,   104,   102,    14,   104,    88,   439,   102,   102,
     104,   104,   102,    95,   104,   102,   378,   104,   450,   101,
     101,   103,   102,    14,   104,   107,   102,   102,   104,   104,
     350,   351,   352,   105,   396,   101,   102,   101,   400,   101,
     102,   101,   102,   101,   102,   365,   101,   102,   368,   369,
     101,   102,    14,   415,   103,   375,   101,   102,   101,   102,
     101,   102,    38,    39,   159,   160,   101,   165,   166,   501,
     101,    87,    87,    48,   107,   107,   103,   107,   104,   399,
     108,   101,   106,    90,   101,   108,    14,    14,    87,   103,
     103,   453,   101,    87,    10,   103,   458,   459,   460,   104,
     104,    14,    14,   104,   466,   108,   104,   103,   470,    80,
     103,    93,    14,   103,    87,   435,    90,    87,    87,   481,
     482,   106,     3,     4,     5,     6,    67,   108,   108,    10,
      11,    12,    13,    14,    15,    16,   101,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   101,   101,    29,    30,
      31,    32,    33,    34,    35,    36,   101,   101,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      90,    90,   108,    54,    55,    14,    14,   101,    59,   499,
     500,    27,    63,   101,   101,    10,   548,   101,   550,    10,
     101,   101,   101,    10,    10,   101,   101,    27,    51,    14,
      14,   521,    14,   101,   101,   101,   526,    88,   101,   101,
      14,   101,   101,    67,    95,   577,    67,   579,   101,   101,
     540,   108,   103,   103,   103,   101,   107,   108,   101,    14,
       7,     7,   594,     7,    67,    10,    14,    10,   101,   101,
     101,   101,    14,   563,   606,    14,    14,   103,   568,     3,
       4,     5,     6,   101,   101,    14,    10,    11,    12,    13,
      14,    15,    16,    14,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   101,   101,    29,    30,    31,    32,    33,
      34,    35,    36,   101,    14,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    14,   101,   101,
      54,    55,   101,   101,   101,    59,   101,   101,   101,    63,
     101,   101,   101,     0,     3,     4,     5,     6,   337,   458,
     299,    10,    11,    12,    13,    14,    15,    16,   163,    18,
      19,    20,   343,   164,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    31,    32,    -1,    -1,    -1,    -1,    -1,   103,
      39,    -1,    -1,   107,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    31,    32,    -1,    -1,    95,    -1,    -1,    -1,
      39,    -1,   101,    -1,   103,    44,    45,    46,   107,    -1,
      49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      59,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      31,    32,    -1,    -1,    -1,    -1,   103,   104,    39,    -1,
     107,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,
      39,    -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,
      49,    -1,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
      59,    -1,    -1,    62,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    31,    32,    -1,    -1,    95,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,   107,    -1,
      49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      59,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,   104,    -1,    -1,   107,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      31,    32,    -1,    -1,    -1,    -1,   103,   104,    39,    -1,
     107,    -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,
      39,    -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,
      49,    -1,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    31,    32,
      -1,    -1,    -1,    -1,   103,   104,    39,    -1,   107,    -1,
      -1,    44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,
      -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,
     103,   104,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,
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
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    59,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    31,    32,    -1,    -1,    95,    -1,
      -1,    -1,    39,    -1,    -1,    -1,   103,    44,    45,    46,
     107,    -1,    49,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    59,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    88,    -1,    -1,    39,    -1,    -1,    -1,    95,    44,
      45,    46,    -1,    -1,    49,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    59,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    31,    32,    -1,    -1,
      95,    -1,    -1,    -1,    39,    -1,    -1,    -1,   103,    44,
      45,    46,   107,    -1,    49,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    59,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,
      95,    44,    45,    46,    -1,    -1,    49,    -1,   103,    -1,
      -1,    -1,   107,    -1,    68,    69,    59,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    83,
      84,    85,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    96,    97,    98,    88,   100,   101,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,   107
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
     103,   107,   112,   113,   114,   115,   116,   117,   118,   119,
     121,   122,   123,   126,   131,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   146,    82,    87,
     100,   103,   105,    89,   107,    87,   100,   103,    14,    10,
      11,    12,    13,    31,   101,   103,   131,   133,   145,    10,
      11,    12,    14,    31,    35,   103,   133,   103,   133,   103,
     103,   133,   138,    14,   131,   131,   133,   101,   133,    10,
      14,   133,    12,    12,   103,   103,   131,   107,   132,   132,
     107,   132,   133,   131,   131,   133,    14,    14,    31,    14,
      10,   142,   142,   133,    62,   111,   101,    77,    78,    79,
      71,    72,    73,    74,    75,    76,    83,    89,    90,    91,
      92,    68,    69,    93,    85,    94,    95,    84,    96,    97,
      98,   100,    14,   133,    14,   104,   145,   133,    14,   133,
     133,    14,    17,    53,   104,   145,   101,    87,   105,    89,
     107,   104,   133,   145,   101,   102,    87,   101,    87,   101,
      87,   101,    10,    11,    12,    14,    10,    14,   133,   101,
     102,   133,   101,   102,   133,   133,    70,   131,    68,    69,
     103,   101,   101,    87,    87,   107,    89,   107,    89,   107,
     133,   133,   133,   133,   133,   133,    48,   124,   107,    14,
      57,   107,   104,   108,   108,   136,   136,   134,   136,   136,
     136,   136,   136,   136,     7,     8,     9,   136,   136,   137,
     137,   138,   138,   139,   140,   141,   141,   142,   142,   142,
     142,    14,    17,   101,   103,   101,    87,   103,   104,   106,
      90,   108,   101,   103,    14,   103,   104,   133,   133,    14,
     133,   101,   104,   133,   133,   103,   133,     5,    14,   103,
     129,   133,    87,   101,    87,   101,    87,   101,   103,    87,
      87,   102,   145,   102,   145,   104,   104,    10,   138,   138,
      10,    14,   104,   130,   133,   133,   120,    14,   133,    14,
     133,   104,   104,   108,   131,    14,    22,   107,   125,   103,
     103,   127,    80,   103,    14,   104,   145,   133,   104,   145,
      87,    87,    87,   104,   145,    14,   104,   128,   145,   106,
      90,   108,   101,   101,   133,    86,   101,   102,    67,    67,
     129,   101,   102,   101,   133,   103,   133,   133,   104,   130,
     133,   133,   145,   101,   145,   101,   131,   131,   131,    70,
      70,    10,    60,    61,   131,   102,   104,   101,   101,    37,
      38,   108,    90,   108,    90,   108,   131,   131,    14,    14,
     125,    51,    52,    55,   108,   104,   130,   130,    64,    65,
      66,   108,   134,   104,   128,   145,   101,   104,   101,   101,
     104,   133,   133,   133,   104,    67,   102,   104,   104,   102,
     133,   145,   133,   133,   104,     5,    14,   101,   133,   101,
     102,   101,   131,   104,   101,   101,   104,   104,    27,    10,
      10,    10,    10,    10,    14,   131,   133,   131,   101,   101,
      27,   108,    14,    53,    10,    13,    14,    15,    16,    51,
     131,   104,   104,    14,    14,    14,   104,   104,   101,   101,
     101,   101,   101,   133,    14,   145,   101,   101,   101,    67,
      67,   102,   145,   131,   101,   101,   113,   131,   131,   131,
      10,    60,    61,   131,   108,   131,   103,   103,    14,    22,
      58,    87,   101,    14,    22,    58,    87,   101,    10,    13,
      15,    16,   101,   101,    14,   131,   131,     7,     7,     7,
      67,   104,   133,   133,   145,   101,    10,    10,   104,   130,
     104,   130,    14,    14,   101,   133,    14,    14,   101,   133,
      14,    22,    58,    87,   101,    14,    22,    58,    87,   101,
     101,   101,   103,   133,   101,   104,   131,   104,   131,   104,
     101,   101,   101,   101,   101,   101,    14,    14,   101,   133,
      14,    14,   101,   133,   104,   130,   101,   131,   131,   101,
     101,   101,   101,   101,   101,   131,   104,   131
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
     112,   113,   113,   113,   114,   115,   115,   116,   117,   118,
     118,   118,   118,   119,   119,   120,   120,   121,   121,   121,
     121,   121,   121,   121,   122,   123,   124,   124,   124,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     126,   127,   127,   127,   127,   128,   128,   129,   129,   129,
     129,   130,   130,   130,   130,   130,   130,   130,   130,   131,
     131,   132,   133,   133,   133,   134,   134,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   136,   136,   136,   137,   137,   137,   138,   138,   139,
     139,   140,   140,   140,   141,   141,   141,   141,   141,   142,
     142,   142,   143,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   145,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146
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
       2,     4,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     7,     7,     5,     5,     7,     5,     2,     7,
       7,     5,     3,     5,     7,     0,     4,     6,     5,     7,
       6,     7,     6,     7,     6,     5,     0,     3,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     6,     6,     6,     5,     5,     6,     6,
       7,     7,     7,     7,     7,     6,     7,     6,     8,     7,
       5,     0,     4,     4,     4,     3,     5,     3,     3,     5,
       5,     1,     2,     3,     3,     3,     4,     5,     5,     3,
       3,     3,     3,     2,     1,     5,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
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
#line 252 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2022 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 262 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2028 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 263 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2034 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 267 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2040 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 269 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2046 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 271 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2052 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 273 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2058 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 275 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2064 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2072 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 281 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2079 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 284 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2087 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 288 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2093 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 290 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2099 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 292 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2105 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 294 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2111 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 296 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2117 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 298 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2125 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 302 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2133 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 306 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2139 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 308 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2145 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 310 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2151 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 312 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2157 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 314 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2163 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 316 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2169 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 318 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2175 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 320 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2181 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 322 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2187 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 324 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2193 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 326 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2199 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 328 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2205 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 330 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2211 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 332 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2217 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 334 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2224 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 337 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2231 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 340 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2237 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 342 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2244 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 345 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2251 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 348 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2257 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 350 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2263 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 352 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2269 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2275 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 356 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2284 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 361 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2292 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 365 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2300 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 369 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2309 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 374 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2317 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 378 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2328 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 385 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2337 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 390 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2344 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 393 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2351 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 396 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2358 "raku.tab.c"
    break;

  case 53: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2365 "raku.tab.c"
    break;

  case 54: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2372 "raku.tab.c"
    break;

  case 55: /* stmt: expr ';'  */
#line 404 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2378 "raku.tab.c"
    break;

  case 56: /* stmt: if_stmt  */
#line 405 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2384 "raku.tab.c"
    break;

  case 57: /* stmt: while_stmt  */
#line 406 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2390 "raku.tab.c"
    break;

  case 58: /* stmt: for_stmt  */
#line 407 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2396 "raku.tab.c"
    break;

  case 59: /* stmt: given_stmt  */
#line 408 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2402 "raku.tab.c"
    break;

  case 60: /* stmt: KW_TRY block  */
#line 410 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2408 "raku.tab.c"
    break;

  case 61: /* stmt: KW_TRY block KW_CATCH block  */
#line 412 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2414 "raku.tab.c"
    break;

  case 62: /* stmt: KW_CATCH block  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2420 "raku.tab.c"
    break;

  case 63: /* stmt: block  */
#line 416 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2426 "raku.tab.c"
    break;

  case 64: /* stmt: unless_stmt  */
#line 417 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2432 "raku.tab.c"
    break;

  case 65: /* stmt: until_stmt  */
#line 418 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2438 "raku.tab.c"
    break;

  case 66: /* stmt: repeat_stmt  */
#line 419 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2444 "raku.tab.c"
    break;

  case 67: /* stmt: sub_decl  */
#line 420 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2450 "raku.tab.c"
    break;

  case 68: /* stmt: class_decl  */
#line 421 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2456 "raku.tab.c"
    break;

  case 69: /* stmt: role_decl  */
#line 422 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2462 "raku.tab.c"
    break;

  case 70: /* stmt: grammar_decl  */
#line 423 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2468 "raku.tab.c"
    break;

  case 71: /* if_stmt: KW_IF '(' expr ')' block  */
#line 427 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2474 "raku.tab.c"
    break;

  case 72: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 429 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2480 "raku.tab.c"
    break;

  case 73: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 431 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2486 "raku.tab.c"
    break;

  case 74: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 435 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2492 "raku.tab.c"
    break;

  case 75: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 439 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2498 "raku.tab.c"
    break;

  case 76: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 441 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2504 "raku.tab.c"
    break;

  case 77: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 445 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2510 "raku.tab.c"
    break;

  case 78: /* repeat_stmt: KW_REPEAT block  */
#line 449 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2516 "raku.tab.c"
    break;

  case 79: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 453 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2526 "raku.tab.c"
    break;

  case 80: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 459 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2536 "raku.tab.c"
    break;

  case 81: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 465 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2545 "raku.tab.c"
    break;

  case 82: /* for_stmt: KW_FOR expr block  */
#line 470 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2552 "raku.tab.c"
    break;

  case 83: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 475 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2564 "raku.tab.c"
    break;

  case 84: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 483 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2577 "raku.tab.c"
    break;

  case 85: /* when_list: %empty  */
#line 493 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2583 "raku.tab.c"
    break;

  case 86: /* when_list: when_list KW_WHEN expr block  */
#line 495 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2591 "raku.tab.c"
    break;

  case 87: /* sub_decl: KW_SUB IDENT '(' param_list ')' block  */
#line 501 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2603 "raku.tab.c"
    break;

  case 88: /* sub_decl: KW_SUB IDENT '(' ')' block  */
#line 509 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2613 "raku.tab.c"
    break;

  case 89: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' block  */
#line 515 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2625 "raku.tab.c"
    break;

  case 90: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' block  */
#line 523 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2635 "raku.tab.c"
    break;

  case 91: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' block  */
#line 529 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2648 "raku.tab.c"
    break;

  case 92: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' block  */
#line 538 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2659 "raku.tab.c"
    break;

  case 93: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' block  */
#line 545 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2672 "raku.tab.c"
    break;

  case 94: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 556 "raku.y"
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
#line 2690 "raku.tab.c"
    break;

  case 95: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 572 "raku.y"
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
#line 2707 "raku.tab.c"
    break;

  case 96: /* is_clauses: %empty  */
#line 586 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 2713 "raku.tab.c"
    break;

  case 97: /* is_clauses: is_clauses IDENT IDENT  */
#line 588 "raku.y"
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
#line 2729 "raku.tab.c"
    break;

  case 98: /* is_clauses: is_clauses TESTOP IDENT  */
#line 600 "raku.y"
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
#line 2745 "raku.tab.c"
    break;

  case 99: /* class_body_list: %empty  */
#line 613 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 2751 "raku.tab.c"
    break;

  case 100: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 615 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2758 "raku.tab.c"
    break;

  case 101: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 618 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2765 "raku.tab.c"
    break;

  case 102: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 621 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2772 "raku.tab.c"
    break;

  case 103: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 624 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2779 "raku.tab.c"
    break;

  case 104: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 627 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2786 "raku.tab.c"
    break;

  case 105: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 630 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 2793 "raku.tab.c"
    break;

  case 106: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 633 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2800 "raku.tab.c"
    break;

  case 107: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 636 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2807 "raku.tab.c"
    break;

  case 108: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 639 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2814 "raku.tab.c"
    break;

  case 109: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 642 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2821 "raku.tab.c"
    break;

  case 110: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 645 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2828 "raku.tab.c"
    break;

  case 111: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 648 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2835 "raku.tab.c"
    break;

  case 112: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 651 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2846 "raku.tab.c"
    break;

  case 113: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 658 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2857 "raku.tab.c"
    break;

  case 114: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 665 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2868 "raku.tab.c"
    break;

  case 115: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 672 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2879 "raku.tab.c"
    break;

  case 116: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 679 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2887 "raku.tab.c"
    break;

  case 117: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 683 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 2895 "raku.tab.c"
    break;

  case 118: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 687 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2903 "raku.tab.c"
    break;

  case 119: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 691 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 2911 "raku.tab.c"
    break;

  case 120: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 695 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2922 "raku.tab.c"
    break;

  case 121: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 702 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2933 "raku.tab.c"
    break;

  case 122: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 709 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2944 "raku.tab.c"
    break;

  case 123: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 716 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 2955 "raku.tab.c"
    break;

  case 124: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' block  */
#line 723 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2969 "raku.tab.c"
    break;

  case 125: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' block  */
#line 733 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 2981 "raku.tab.c"
    break;

  case 126: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' block  */
#line 741 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 2994 "raku.tab.c"
    break;

  case 127: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' block  */
#line 750 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3005 "raku.tab.c"
    break;

  case 128: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' block  */
#line 757 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3019 "raku.tab.c"
    break;

  case 129: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' block  */
#line 767 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3031 "raku.tab.c"
    break;

  case 130: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 777 "raku.y"
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
#line 3048 "raku.tab.c"
    break;

  case 131: /* grammar_body_list: %empty  */
#line 791 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3054 "raku.tab.c"
    break;

  case 132: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 793 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3063 "raku.tab.c"
    break;

  case 133: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 798 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3072 "raku.tab.c"
    break;

  case 134: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 803 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3081 "raku.tab.c"
    break;

  case 135: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 810 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3089 "raku.tab.c"
    break;

  case 136: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 814 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3097 "raku.tab.c"
    break;

  case 137: /* pair_list: IDENT OP_FATARROW expr  */
#line 820 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3103 "raku.tab.c"
    break;

  case 138: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 822 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3109 "raku.tab.c"
    break;

  case 139: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 824 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3115 "raku.tab.c"
    break;

  case 140: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 826 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3121 "raku.tab.c"
    break;

  case 141: /* param_list: VAR_SCALAR  */
#line 829 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3127 "raku.tab.c"
    break;

  case 142: /* param_list: IDENT VAR_SCALAR  */
#line 830 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3133 "raku.tab.c"
    break;

  case 143: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 831 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3139 "raku.tab.c"
    break;

  case 144: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 832 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3145 "raku.tab.c"
    break;

  case 145: /* param_list: param_list ',' VAR_SCALAR  */
#line 833 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3151 "raku.tab.c"
    break;

  case 146: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 834 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3157 "raku.tab.c"
    break;

  case 147: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 835 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3163 "raku.tab.c"
    break;

  case 148: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 836 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3169 "raku.tab.c"
    break;

  case 149: /* block: '{' stmt_list '}'  */
#line 839 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3175 "raku.tab.c"
    break;

  case 150: /* block: '{' YADA '}'  */
#line 840 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3181 "raku.tab.c"
    break;

  case 151: /* closure: '{' expr '}'  */
#line 843 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3187 "raku.tab.c"
    break;

  case 152: /* expr: VAR_SCALAR '=' expr  */
#line 846 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3193 "raku.tab.c"
    break;

  case 153: /* expr: KW_GATHER block  */
#line 847 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3203 "raku.tab.c"
    break;

  case 154: /* expr: tern_expr  */
#line 852 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3209 "raku.tab.c"
    break;

  case 155: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 856 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3215 "raku.tab.c"
    break;

  case 156: /* tern_expr: cmp_expr  */
#line 857 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3221 "raku.tab.c"
    break;

  case 157: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 860 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3227 "raku.tab.c"
    break;

  case 158: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 861 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3233 "raku.tab.c"
    break;

  case 159: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 862 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3239 "raku.tab.c"
    break;

  case 160: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 863 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3245 "raku.tab.c"
    break;

  case 161: /* cmp_expr: jct_expr '<' jct_expr  */
#line 864 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3251 "raku.tab.c"
    break;

  case 162: /* cmp_expr: jct_expr '>' jct_expr  */
#line 865 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3257 "raku.tab.c"
    break;

  case 163: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 866 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3263 "raku.tab.c"
    break;

  case 164: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 867 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3269 "raku.tab.c"
    break;

  case 165: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 868 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3275 "raku.tab.c"
    break;

  case 166: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 869 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3281 "raku.tab.c"
    break;

  case 167: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 871 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3291 "raku.tab.c"
    break;

  case 168: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 877 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3301 "raku.tab.c"
    break;

  case 169: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 883 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3311 "raku.tab.c"
    break;

  case 170: /* cmp_expr: jct_expr  */
#line 888 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3317 "raku.tab.c"
    break;

  case 171: /* jct_expr: jct_expr '|' range_expr  */
#line 891 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3323 "raku.tab.c"
    break;

  case 172: /* jct_expr: jct_expr '&' range_expr  */
#line 892 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3329 "raku.tab.c"
    break;

  case 173: /* jct_expr: range_expr  */
#line 893 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3335 "raku.tab.c"
    break;

  case 174: /* range_expr: add_expr OP_RANGE add_expr  */
#line 896 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3341 "raku.tab.c"
    break;

  case 175: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 897 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3347 "raku.tab.c"
    break;

  case 176: /* range_expr: add_expr  */
#line 898 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3353 "raku.tab.c"
    break;

  case 177: /* add_expr: add_expr '~' repl_expr  */
#line 901 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3359 "raku.tab.c"
    break;

  case 178: /* add_expr: repl_expr  */
#line 902 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 3365 "raku.tab.c"
    break;

  case 179: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 905 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3371 "raku.tab.c"
    break;

  case 180: /* repl_expr: addsub_expr  */
#line 906 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 3377 "raku.tab.c"
    break;

  case 181: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 909 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3383 "raku.tab.c"
    break;

  case 182: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 910 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3389 "raku.tab.c"
    break;

  case 183: /* addsub_expr: mul_expr  */
#line 911 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 3395 "raku.tab.c"
    break;

  case 184: /* mul_expr: mul_expr '*' unary_expr  */
#line 914 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3401 "raku.tab.c"
    break;

  case 185: /* mul_expr: mul_expr '/' unary_expr  */
#line 915 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3407 "raku.tab.c"
    break;

  case 186: /* mul_expr: mul_expr '%' unary_expr  */
#line 916 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3413 "raku.tab.c"
    break;

  case 187: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 917 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3419 "raku.tab.c"
    break;

  case 188: /* mul_expr: unary_expr  */
#line 918 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3425 "raku.tab.c"
    break;

  case 189: /* unary_expr: '-' unary_expr  */
#line 921 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3431 "raku.tab.c"
    break;

  case 190: /* unary_expr: '!' unary_expr  */
#line 922 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3437 "raku.tab.c"
    break;

  case 191: /* unary_expr: postfix_expr  */
#line 923 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3443 "raku.tab.c"
    break;

  case 192: /* postfix_expr: call_expr  */
#line 925 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3449 "raku.tab.c"
    break;

  case 193: /* call_expr: IDENT '(' arg_list ')'  */
#line 928 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3458 "raku.tab.c"
    break;

  case 194: /* call_expr: IDENT '(' ')'  */
#line 932 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3464 "raku.tab.c"
    break;

  case 195: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 934 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3473 "raku.tab.c"
    break;

  case 196: /* call_expr: VAR_SCALAR '(' ')'  */
#line 939 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3479 "raku.tab.c"
    break;

  case 197: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 941 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3489 "raku.tab.c"
    break;

  case 198: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 947 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3499 "raku.tab.c"
    break;

  case 199: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 953 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3507 "raku.tab.c"
    break;

  case 200: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 957 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3518 "raku.tab.c"
    break;

  case 201: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 964 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3527 "raku.tab.c"
    break;

  case 202: /* call_expr: IDENT '.' CARET IDENT  */
#line 969 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3537 "raku.tab.c"
    break;

  case 203: /* call_expr: atom '.' CARET IDENT  */
#line 975 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3547 "raku.tab.c"
    break;

  case 204: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 981 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3558 "raku.tab.c"
    break;

  case 205: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 988 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3569 "raku.tab.c"
    break;

  case 206: /* call_expr: atom '.' IDENT '(' ')'  */
#line 995 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3578 "raku.tab.c"
    break;

  case 207: /* call_expr: atom '.' IDENT  */
#line 1000 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[0].sval)); free((yyvsp[0].sval));
          expr_add_child(fe,(yyvsp[-2].node));
          (yyval.node)=fe; }
#line 3587 "raku.tab.c"
    break;

  case 208: /* call_expr: KW_DIE expr  */
#line 1005 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3593 "raku.tab.c"
    break;

  case 209: /* call_expr: KW_MAP closure expr  */
#line 1007 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3599 "raku.tab.c"
    break;

  case 210: /* call_expr: KW_GREP closure expr  */
#line 1009 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3605 "raku.tab.c"
    break;

  case 211: /* call_expr: KW_SORT expr  */
#line 1011 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3611 "raku.tab.c"
    break;

  case 212: /* call_expr: KW_SORT closure expr  */
#line 1013 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3617 "raku.tab.c"
    break;

  case 213: /* call_expr: atom  */
#line 1014 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 3623 "raku.tab.c"
    break;

  case 214: /* arg_list: expr  */
#line 1017 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 3629 "raku.tab.c"
    break;

  case 215: /* arg_list: arg_list ',' expr  */
#line 1018 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 3635 "raku.tab.c"
    break;

  case 216: /* atom: LIT_INT  */
#line 1021 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 3641 "raku.tab.c"
    break;

  case 217: /* atom: LIT_FLOAT  */
#line 1022 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 3647 "raku.tab.c"
    break;

  case 218: /* atom: LIT_STR  */
#line 1023 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 3653 "raku.tab.c"
    break;

  case 219: /* atom: WORDLIST  */
#line 1025 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 3665 "raku.tab.c"
    break;

  case 220: /* atom: LIT_INTERP_STR  */
#line 1032 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 3671 "raku.tab.c"
    break;

  case 221: /* atom: VAR_SCALAR  */
#line 1033 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3677 "raku.tab.c"
    break;

  case 222: /* atom: VAR_ARRAY  */
#line 1034 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3683 "raku.tab.c"
    break;

  case 223: /* atom: VAR_HASH  */
#line 1035 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3689 "raku.tab.c"
    break;

  case 224: /* atom: VAR_CAPTURE  */
#line 1037 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3697 "raku.tab.c"
    break;

  case 225: /* atom: VAR_FH  */
#line 1041 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 3705 "raku.tab.c"
    break;

  case 226: /* atom: VAR_NAMED_CAPTURE  */
#line 1045 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 3712 "raku.tab.c"
    break;

  case 227: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1048 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3718 "raku.tab.c"
    break;

  case 228: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1050 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3724 "raku.tab.c"
    break;

  case 229: /* atom: VAR_HASH '{' expr '}'  */
#line 1052 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3730 "raku.tab.c"
    break;

  case 230: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1054 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 3736 "raku.tab.c"
    break;

  case 231: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1056 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3742 "raku.tab.c"
    break;

  case 232: /* atom: IDENT  */
#line 1057 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 3748 "raku.tab.c"
    break;

  case 233: /* atom: VAR_TWIGIL  */
#line 1059 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3756 "raku.tab.c"
    break;

  case 234: /* atom: VAR_ARRAY_TWIGIL  */
#line 1063 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3764 "raku.tab.c"
    break;

  case 235: /* atom: VAR_HASH_TWIGIL  */
#line 1067 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 3772 "raku.tab.c"
    break;

  case 236: /* atom: '(' expr ')'  */
#line 1070 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 3778 "raku.tab.c"
    break;

  case 237: /* atom: block  */
#line 1071 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3784 "raku.tab.c"
    break;

  case 238: /* atom: KW_SUB block  */
#line 1072 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 3790 "raku.tab.c"
    break;


#line 3794 "raku.tab.c"

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

#line 1074 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
