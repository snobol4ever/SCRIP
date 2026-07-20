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
  YYSYMBOL_method_body = 123,              /* method_body  */
  YYSYMBOL_class_decl = 124,               /* class_decl  */
  YYSYMBOL_role_decl = 125,                /* role_decl  */
  YYSYMBOL_is_clauses = 126,               /* is_clauses  */
  YYSYMBOL_class_body_list = 127,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 128,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 129,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 130,           /* named_arg_list  */
  YYSYMBOL_pair_list = 131,                /* pair_list  */
  YYSYMBOL_param_list = 132,               /* param_list  */
  YYSYMBOL_block = 133,                    /* block  */
  YYSYMBOL_closure = 134,                  /* closure  */
  YYSYMBOL_expr = 135,                     /* expr  */
  YYSYMBOL_tern_expr = 136,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 137,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 138,                 /* jct_expr  */
  YYSYMBOL_range_expr = 139,               /* range_expr  */
  YYSYMBOL_add_expr = 140,                 /* add_expr  */
  YYSYMBOL_repl_expr = 141,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 142,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 143,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 144,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 145,             /* postfix_expr  */
  YYSYMBOL_call_expr = 146,                /* call_expr  */
  YYSYMBOL_arg_list = 147,                 /* arg_list  */
  YYSYMBOL_atom = 148                      /* atom  */
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
#define YYLAST   2440

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  272
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  677

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
     423,   424,   427,   429,   431,   433,   435,   437,   441,   443,
     447,   449,   451,   453,   457,   459,   463,   467,   473,   479,
     484,   490,   495,   500,   508,   519,   520,   526,   534,   540,
     546,   554,   560,   566,   575,   582,   591,   600,   609,   610,
     613,   615,   619,   620,   621,   624,   626,   630,   646,   661,
     662,   674,   688,   689,   692,   695,   698,   701,   704,   707,
     710,   713,   716,   719,   722,   725,   732,   739,   746,   753,
     757,   761,   765,   769,   776,   783,   790,   797,   807,   815,
     823,   832,   839,   846,   856,   864,   874,   889,   890,   895,
     900,   907,   911,   917,   919,   921,   923,   927,   928,   929,
     930,   931,   932,   933,   934,   937,   938,   939,   941,   943,
     947,   950,   951,   956,   959,   961,   964,   965,   966,   967,
     968,   969,   970,   971,   972,   973,   974,   980,   986,   992,
     995,   996,   997,  1000,  1001,  1002,  1005,  1006,  1009,  1010,
    1013,  1014,  1015,  1018,  1019,  1020,  1021,  1022,  1025,  1026,
    1027,  1029,  1031,  1036,  1037,  1042,  1044,  1050,  1056,  1060,
    1064,  1069,  1076,  1081,  1087,  1093,  1100,  1107,  1112,  1117,
    1119,  1121,  1123,  1125,  1127,  1130,  1131,  1134,  1135,  1136,
    1137,  1145,  1146,  1147,  1148,  1149,  1153,  1157,  1160,  1162,
    1164,  1166,  1168,  1170,  1171,  1175,  1179,  1183,  1184,  1185,
    1187,  1190,  1191
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
  "sub_decl", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "jct_expr", "range_expr", "add_expr",
  "repl_expr", "addsub_expr", "mul_expr", "unary_expr", "postfix_expr",
  "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-488)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-272)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -488,    13,   946,  -488,  -488,  -488,  -488,  -488,   148,   -67,
     -14,   -34,   -75,  -488,  -488,  -488,  -488,  -488,    78,   437,
     231,  1713,  1773,  1825,  1885,  1937,    -5,    53,  1937,   497,
     194,  1937,   124,   158,  1997,  2049,    53,   137,   137,  2109,
      53,    53,  1937,   267,   283,    59,  -488,   312,  2333,  2333,
    -488,  1011,   251,  -488,  -488,  -488,  -488,  -488,  -488,  -488,
    -488,  -488,  -488,  -488,  -488,   621,   272,  -488,   288,   196,
    -488,    97,   305,   301,   177,  -488,  -488,  -488,   282,   385,
    1937,   414,  1063,  1937,   424,  1937,  1937,    60,  1129,   291,
     149,   340,    31,  -488,    53,  -488,  1181,  -488,  -488,   365,
      81,   159,   162,   374,   457,   213,  1011,   378,  1011,   396,
    1011,    53,  1011,    53,     9,   129,   122,  -488,  -488,   392,
    -488,   394,   397,   412,   367,    80,   121,  1011,    53,  1011,
      53,  -488,  1937,  1937,  1937,  1247,  1937,  -488,   458,  -488,
    -488,  -488,   407,   428,    10,   417,   430,  -488,  -488,  -488,
      -4,   427,   628,  -488,  2333,  2333,  2333,  2333,  2333,  2333,
    2333,  2333,  2333,   380,  2333,  2333,  2333,  2333,  2333,  2333,
    2333,  2333,  2333,  2333,  2333,  2333,  2333,  2333,   297,     4,
     436,   151,  -488,    79,   433,   455,   447,   464,   463,   553,
     465,  -488,    87,  -488,  1937,  1937,   555,  1937,   469,    -4,
     254,  -488,  1937,  1937,  -488,  2161,  -488,  2221,  -488,   189,
     190,   191,   252,   485,   486,   273,  -488,  1937,   289,  -488,
    1937,   298,   547,   299,  -488,   566,  1937,  -488,  2333,  2333,
      -3,  -488,  -488,  -488,  -488,  1937,  1937,  -488,   563,  1937,
     564,  1937,   303,   552,   306,  -488,   473,  -488,  -488,  -488,
      53,    30,  -488,    20,   479,   480,  -488,  1307,  -488,  -488,
    1713,  1773,  -488,   139,   413,   413,   504,   413,   413,   413,
     413,   413,   413,  -488,  -488,  -488,   413,   413,  -488,  -488,
     493,   493,   305,   301,   177,   177,  -488,  -488,  -488,  -488,
     484,   574,  -488,  1359,  -488,  1937,  1425,  -488,   502,   503,
     506,  -488,  1477,  -488,  1543,  -488,  -488,   489,   501,   488,
    -488,   496,  -488,   498,  1011,   110,   534,   113,  1595,   421,
     505,  1937,  -488,  2273,  -488,  1937,  -488,    36,  -488,  1937,
    1937,  1307,   425,  1307,   448,    53,   -18,    53,    53,    92,
     -54,   -39,  -488,    26,   495,   307,   734,   507,   508,   -23,
     513,   499,   520,   509,    53,    53,    53,  -488,  -488,   597,
     598,  -488,    14,   495,   310,    47,   270,     6,  -488,   311,
     181,   193,  -488,  2333,  1661,  -488,   512,   314,   514,   515,
     317,  1937,  1937,  1937,  -488,   318,   117,  -488,   321,   322,
    -488,  -488,  -488,  -488,  -488,   325,  1937,  -488,  1937,  1937,
    1937,   330,  -488,    83,  -488,   517,  1011,   450,   519,   495,
     331,   522,   524,   335,  -488,   342,  -488,   587,  -488,  -488,
    -488,  -488,   611,  -488,   616,   617,  -488,   618,   619,  -488,
     327,   495,  1713,  1773,  -488,   207,  -488,  -488,  1937,    53,
    -488,  -488,  -488,   529,   544,   608,  -488,  -488,  -488,  -488,
     106,    29,   333,   604,  -488,  -488,   495,   495,   356,   357,
     642,   651,   652,  -488,  -488,  -488,  -488,  -488,  -488,   360,
     361,  -488,   578,  -488,  -488,   579,   580,   583,   584,  -488,
    1937,   672,  -488,  -488,  1307,   600,   452,  -488,  -488,   601,
     636,   640,  -488,   368,  -488,  1937,  -488,  -488,   495,  -488,
    -488,   607,   623,   -18,    53,    53,    53,  -488,  -488,  -488,
      35,  -488,   216,   228,  -488,    53,   612,  -488,  -488,    53,
    -488,   261,   269,    54,    77,   347,   624,   625,   674,  -488,
    -488,   495,   495,   702,   715,   720,  -488,  -488,  -488,  -488,
    -488,  -488,  -488,  -488,   661,   373,  -488,  -488,  -488,  1937,
    1937,  1307,   456,  -488,  -488,  -488,  -488,  -488,  -488,  -488,
    -488,  -488,   722,   723,  -488,  -488,  -488,  -488,  -488,    48,
     668,  -488,    49,  -488,   727,   728,   633,  1937,  -488,   729,
     737,   660,  1937,  -488,   101,   163,   670,   671,  -488,  -488,
     271,  -488,  -488,  -488,  -488,  -488,  1937,   684,  -488,  -488,
     383,  -488,  -488,  -488,   655,   386,   678,   840,   655,   387,
     686,   689,  -488,   690,   691,   693,  -488,   694,   782,   784,
     698,  1937,  -488,   786,   787,   701,  1937,  -488,  -488,  -488,
      50,  -488,  -488,  -488,   703,  -488,   655,  -488,  1713,  1773,
    -488,   234,  -488,   655,  -488,  -488,  -488,  -488,  -488,  -488,
     704,   705,  -488,   706,   707,   708,  -488,   709,   655,   390,
    -488,  -488,   232,   237,  -488,  -488,  -488,  -488,  -488,  -488,
    -488,  -488,  -488,   655,  -488,  -488,  -488
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   247,   248,   249,   251,   252,   253,
     254,   264,   263,   265,   266,   255,   256,   257,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   250,     0,     0,     0,
      56,     0,     3,     4,    57,    58,    65,    66,    67,    59,
      60,    68,    69,    70,    71,    64,     0,   183,   185,   199,
     202,   205,   207,   209,   212,   217,   220,   221,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   264,     0,    29,     0,   271,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,   272,   182,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     3,     0,   242,    61,    63,
     239,   119,     0,     0,     0,     0,   252,   219,   218,   267,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,   225,     0,     0,     0,     0,     0,   230,     0,
     229,   223,     0,    24,     0,     0,     0,     0,   267,     0,
       0,    32,     0,     0,     6,     0,     7,     0,     8,     0,
       0,     0,     0,     0,     0,     0,    33,     0,     0,    36,
       0,     0,    75,     0,    79,     0,     0,    92,     0,     0,
       0,     3,    99,    39,    40,     0,     0,    95,     0,     0,
       0,     0,     0,    82,     0,    85,     0,   240,   241,   243,
       0,     0,   122,     0,     0,     0,   157,     0,   268,   176,
       0,     0,   175,     0,   186,   187,     0,   188,   189,   192,
     193,   194,   195,   196,   197,   198,   190,   191,   200,   201,
     203,   204,   206,   208,   210,   211,   216,   213,   214,   215,
     238,     0,    45,     0,    42,     0,     0,   224,   258,   259,
     260,    47,     0,   233,     0,   222,   181,     0,     0,     0,
      31,     0,   246,     0,     0,     0,   249,   263,     0,     0,
       0,     0,    21,     0,    22,     0,    23,     0,   102,     0,
       0,     0,     0,     0,     0,   268,     0,   268,     0,     0,
     203,   204,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   268,   180,    62,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   269,     0,
       0,     0,   177,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,   263,   228,     0,     0,
     258,   259,   260,    30,     5,     0,     0,     9,     0,     0,
       0,     0,    14,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    37,    72,    77,    76,
      78,    89,     0,    91,     0,     0,   168,     0,     0,    98,
       0,     0,     0,     0,   108,     0,    26,    25,     0,     0,
      93,   261,   262,     0,     0,    80,    83,    84,   120,   121,
       0,     0,     0,     0,   118,   107,     0,     0,     0,     0,
       0,     0,     0,   156,   270,   178,   179,   184,   237,     0,
       0,    44,     0,    46,    49,     0,     0,     0,     0,   231,
       0,     0,   226,   227,     0,     0,     0,   164,   163,     0,
       0,     0,    16,     0,    17,     0,    20,   101,     0,    28,
      27,   270,   270,     0,     0,     0,     0,   169,   170,   171,
       0,    97,     0,     0,   109,     0,     0,    53,    54,     0,
     117,     0,     0,     0,     0,     0,     0,     0,     0,   106,
     104,     0,     0,     0,     0,     0,   236,   235,    43,    48,
      50,    51,    52,   161,     0,     0,    11,    10,    15,     0,
       0,     0,     0,   100,    35,    38,    74,    73,    90,    87,
      88,   172,     0,     0,   110,   111,    96,    94,    81,     0,
       3,   149,     0,   152,     0,     0,     0,     0,   128,     0,
       0,     0,     0,   123,     0,     0,     0,     0,   124,   125,
       0,   103,   105,   158,   159,   160,     0,   270,   166,   165,
       0,    18,   173,   174,     0,     0,     0,     0,     0,     0,
       0,     0,   139,     0,     0,     0,   140,     0,     0,     0,
       0,     0,   130,     0,     0,     0,     0,   129,   126,   127,
       0,   155,   162,    12,   270,   148,     0,   113,     0,     0,
     112,     0,   151,     0,   137,   138,   132,   135,   136,   131,
       0,     0,   141,     0,     0,     0,   142,     0,     0,     0,
      19,   147,     0,     0,   114,   150,   145,   146,   134,   143,
     144,   133,   154,     0,   115,   116,   153
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -488,  -488,     1,  -488,  -331,  -488,  -488,  -488,  -488,  -488,
    -488,  -488,  -488,  -200,  -487,  -488,  -488,  -488,   442,  -488,
    -488,   438,   500,  -247,    15,   521,    -2,  -154,  -488,   145,
     395,   -22,   641,   643,   391,    -1,  -488,  -488,   -78,  -488
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   152,    53,    54,    55,    56,    57,    58,    59,
      60,   349,    61,   232,   571,    62,    63,   251,   362,    64,
     367,   388,   319,   345,    97,   133,    98,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    99,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      66,     2,   266,   115,   183,   418,   364,   342,    23,   116,
     192,   343,   328,     3,   438,   439,   424,    65,   200,   107,
     109,   111,   113,   114,   254,    87,   119,   121,    88,   124,
     342,   425,   128,   130,   343,   573,   426,   137,    83,   170,
     140,   117,   118,   521,   359,   561,   342,   147,   148,   150,
     343,   131,   360,    86,   170,   138,   139,   342,   342,   342,
     342,   343,   343,   343,   343,   451,   452,   255,   574,   453,
     460,   461,   462,   143,   188,    84,   575,   189,   180,   225,
     410,   184,   522,   186,   187,   440,   427,   428,   490,    52,
     144,   579,    89,    85,   199,   562,   563,   491,   257,   580,
     258,   344,    52,   631,   215,   292,   218,   293,   221,   117,
     223,   226,   576,   190,   463,   618,    52,   635,   458,   459,
     196,   642,   454,   619,   363,   242,   222,   244,   224,   227,
     246,   247,   248,   246,   249,   581,   125,   361,   197,   332,
     409,   577,   334,   243,   429,   245,   280,   281,   339,   661,
     263,   457,   604,   608,   658,   578,   665,   451,   452,   620,
      52,   453,   422,   455,   582,   168,   169,    65,   203,   238,
     126,   672,   556,   286,   287,   288,   289,   623,   583,   369,
     400,   202,   204,   297,   480,   624,   676,   239,   621,   202,
     170,   305,   306,   307,   202,   309,   396,   228,   229,    52,
     312,   313,   622,   315,   122,   320,   340,   341,   123,   497,
     240,   397,   398,    87,   520,   377,    88,    87,   380,   467,
      88,   625,   170,   213,   385,   230,   389,   214,   241,   231,
      79,   511,   346,   347,   348,    80,   194,   351,   295,   353,
     153,   100,   101,   102,   132,   103,   205,   372,    81,   207,
     626,    82,    82,   413,   296,   415,   529,   530,   370,   371,
     206,   174,   104,   208,   627,   358,   105,   157,   158,   159,
     160,   161,   162,   175,   176,   177,   321,   323,   325,   163,
     342,   141,   216,   217,   343,   164,   165,   166,   167,   465,
     322,   324,   326,   378,   219,   220,   470,   142,   553,   264,
     265,   466,   267,   268,   269,   270,   271,   272,   153,   276,
     277,   290,   395,   151,   291,   514,   150,   216,   217,   405,
     486,   407,   605,   408,   564,   609,   145,   411,   412,   219,
     220,   591,   592,   216,   217,   153,   565,   509,   219,   220,
     674,   510,   664,   523,   435,   675,   524,   525,   526,   527,
     417,   419,   420,   421,   423,   327,   202,   584,   311,   231,
     585,    65,   586,   587,   569,   154,   155,   156,   570,   445,
     446,   447,   572,   153,   630,   331,   570,   258,   570,   476,
     477,   478,   178,   659,   209,   210,   211,   273,   274,   275,
     171,   333,   193,   258,   485,   172,   173,   487,   488,   179,
     257,   257,   335,   337,   493,   257,   545,   354,   257,   430,
     356,   431,   430,   202,   456,   464,   202,   552,   472,   202,
     202,   475,   479,   481,   202,   482,   483,   484,   181,   258,
     512,   513,   403,   430,   489,   498,   515,   202,   185,   501,
       4,     5,     6,     7,   202,   195,   502,    90,    91,    92,
      93,    12,    13,    14,   516,    15,    16,    17,   430,   430,
     531,   532,   481,   202,   536,   537,   201,   202,    94,    27,
     551,   212,   258,   600,   237,   202,    32,   597,   543,   216,
     217,    37,    38,    39,   235,   202,    42,   634,   430,   430,
     636,   643,   430,   233,   673,   234,    46,   219,   220,   236,
       4,     5,     6,     7,   166,   167,   250,    90,    91,    92,
      93,    12,    13,    14,   252,    15,    16,    17,   557,   558,
     559,   560,   402,   403,   256,    48,   414,   202,    94,    27,
     566,   253,    49,    82,   568,   259,    32,   294,    95,   298,
      96,    37,    38,    39,    52,   299,    42,   598,   599,   416,
     202,   494,   495,   547,   202,   300,    46,   601,   202,   134,
     136,   278,   279,   284,   285,   301,   302,   303,   304,   308,
     310,   607,   329,   330,   336,   613,   338,   350,   352,   355,
     617,   357,   365,   366,   373,    48,   170,   374,   375,   381,
     382,   391,    49,   383,   632,   390,   392,   393,   120,   394,
      51,   399,   231,   441,    52,   641,   404,   442,   436,   437,
     443,   448,   449,   471,   503,   473,   474,   444,   492,   653,
     496,   504,    65,   499,   657,   500,   505,   506,   507,   508,
     517,     4,     5,     6,     7,   519,   662,   663,     8,     9,
      10,    11,    12,    13,    14,   518,    15,    16,    17,    18,
      19,    20,   260,   261,    23,   528,   533,    24,    25,    26,
      27,    28,    29,    30,    31,   534,   535,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,   538,
     539,   540,    44,    45,   541,   542,   544,    46,   590,  -271,
    -271,    47,  -271,  -271,  -271,  -271,  -271,  -271,  -271,  -271,
    -271,   546,   548,   549,  -271,  -271,  -271,   550,   554,   593,
    -271,  -271,  -271,  -271,  -271,  -271,    48,  -271,  -271,  -271,
     567,  -271,   594,    49,   555,   588,   589,   595,   596,    50,
     606,    51,   602,   603,   612,    52,   262,     4,     5,     6,
       7,   610,   611,   614,     8,     9,    10,    11,    12,    13,
      14,   615,    15,    16,    17,    18,    19,    20,   432,   433,
      23,   616,   570,    24,    25,    26,    27,    28,    29,    30,
      31,   628,   629,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,   633,   637,   644,    44,    45,
     645,   646,   647,    46,   648,   649,   650,    47,   651,   652,
     654,   655,   656,   450,   660,   666,   667,   668,   669,   670,
     671,   282,   469,     0,   283,     0,     0,     0,   401,     0,
       0,     0,    48,     0,     0,     0,     0,     0,     0,    49,
       0,     0,     0,     0,     0,    50,     0,    51,     0,     0,
       0,    52,   434,     4,     5,     6,     7,     0,     0,     0,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,   638,   639,    23,     0,     0,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,     0,     0,     0,    44,    45,     0,     0,     0,    46,
       0,     0,     0,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,    50,     0,    51,     0,     0,     0,    52,   640,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     0,     0,     0,
      44,    45,     0,     0,     0,    46,     0,     0,     0,    47,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    90,    91,    92,    93,    12,    13,    14,     0,    15,
      16,    17,     0,     0,    48,     0,     0,     0,     0,     0,
       0,    49,    94,    27,     0,     0,     0,    50,     0,    51,
      32,     0,     0,    52,     0,    37,    38,    39,     0,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    90,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,    51,   149,     0,     0,    52,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    90,
      91,    92,    93,    12,    13,    14,     0,    15,    16,    17,
       0,    48,     0,     0,     0,     0,     0,     0,    49,     0,
      94,    27,     0,     0,     0,     0,    51,   182,    32,     0,
      52,     0,     0,    37,    38,    39,     0,     0,    42,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    46,     0,
       0,    90,    91,    92,    93,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,    51,   191,     0,     0,    52,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,    48,
       0,     0,     0,     0,     0,     0,    49,     0,    94,    27,
       0,     0,     0,     0,    51,   198,    32,     0,    52,     0,
       0,    37,    38,    39,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,     0,     0,   151,
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
      51,   368,     0,     0,    52,     0,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,    48,     0,     0,
       0,     0,     0,     0,    49,     0,    94,    27,     0,     0,
       0,     0,    51,   376,    32,     0,    52,     0,     0,    37,
      38,    39,     0,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    27,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,    51,   379,
       0,     0,    52,     0,     0,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    90,    91,    92,    93,   386,    13,    14,
       0,    15,    16,    17,     0,    48,     0,     0,     0,     0,
       0,     0,    49,     0,    94,    27,     0,     0,     0,     0,
      51,   384,    32,     0,    52,     0,     0,    37,    38,    39,
       0,     0,    42,     0,     0,     0,     0,     0,     4,     5,
     316,     7,    46,     0,     0,    90,    91,    92,    93,   317,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    27,     0,     0,
       0,    48,     0,     0,    32,     0,     0,     0,    49,    37,
      38,    39,     0,     0,    42,     0,    51,   387,     0,     0,
      52,     0,     0,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,    90,    91,    92,    93,   386,    13,    14,     0,    15,
      16,    17,     0,    48,     0,     0,     0,     0,     0,     0,
      49,     0,    94,    27,     0,     0,     0,     0,    51,   149,
      32,     0,    52,     0,     0,    37,    38,    39,     0,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    90,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,    51,   468,     0,     0,    52,     0,
       0,     0,    46,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,    90,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,    94,    27,     0,     0,    49,     0,
       0,     0,    32,     0,     0,     0,   106,    37,    38,    39,
      52,     0,    42,     0,     0,     0,     0,     0,     4,     5,
       6,     7,    46,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    27,     0,     0,
       0,    48,     0,     0,    32,     0,     0,     0,    49,    37,
      38,    39,     0,     0,    42,     0,   108,     0,     0,     0,
      52,     0,     0,     0,    46,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,    90,    91,    92,    93,    12,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,    94,    27,     0,     0,
      49,     0,     0,     0,    32,     0,     0,     0,   110,    37,
      38,    39,    52,     0,    42,     0,     0,     0,     0,     0,
       4,     5,     6,     7,    46,     0,     0,    90,    91,    92,
      93,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    27,
       0,     0,     0,    48,     0,     0,    32,     0,     0,     0,
      49,    37,    38,    39,     0,     0,    42,     0,   112,     0,
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
     127,     0,     0,     0,    52,     0,     0,     0,    46,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,    90,
      91,    92,    93,    12,    13,    14,     0,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,    48,     0,     0,
      94,    27,     0,     0,    49,     0,     0,     0,    32,     0,
       0,     0,   129,    37,    38,    39,    52,     0,    42,     0,
       0,     0,     0,     0,     4,     5,     6,     7,    46,     0,
       0,    90,    91,    92,    93,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    27,     0,     0,     0,    48,     0,     0,
      32,     0,     0,     0,    49,    37,    38,    39,     0,     0,
      42,     0,    51,     0,     0,     0,   135,     0,     0,     0,
      46,     0,     0,     0,     4,     5,   316,     7,     0,     0,
       0,    90,    91,    92,    93,   317,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    48,
       0,     0,    94,    27,     0,     0,    49,     0,     0,     0,
      32,     0,     0,     0,   314,    37,    38,    39,    52,     0,
      42,     0,     0,     0,     0,     0,     4,     5,     6,     7,
      46,     0,     0,    90,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    27,     0,     0,     0,    48,
       0,     0,    32,     0,     0,     0,    49,    37,    38,    39,
       0,     0,    42,     0,   318,     0,     0,     0,    52,     0,
       0,     0,    46,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   146,    91,    92,    93,    12,    13,    14,
       0,    15,    16,    17,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,    94,     0,     0,     0,    49,     0,
       0,     0,    32,     0,     0,     0,   406,    37,    38,    39,
      52,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
      52
};

static const yytype_int16 yycheck[] =
{
       2,     0,   156,    25,    82,   336,   253,    10,    26,    14,
      88,    14,   212,     0,    37,    38,    70,     2,    96,    21,
      22,    23,    24,    25,    14,   100,    28,    29,   103,    31,
      10,    70,    34,    35,    14,   522,    10,    39,   105,    93,
      42,    26,    27,    14,    14,    10,    10,    48,    49,    51,
      14,    36,    22,    87,    93,    40,    41,    10,    10,    10,
      10,    14,    14,    14,    14,    51,    52,    57,    14,    55,
      64,    65,    66,    14,    14,    89,    22,    17,    80,    70,
     327,    83,    53,    85,    86,   108,    60,    61,     5,   107,
      31,    14,    14,   107,    96,    60,    61,    14,   102,    22,
     104,   104,   107,   590,   106,   101,   108,   103,   110,    94,
     112,   102,    58,    53,   108,    14,   107,   604,   365,   366,
      89,   608,   108,    22,   104,   127,   111,   129,   113,   114,
     132,   133,   134,   135,   136,    58,    12,   107,   107,   217,
     104,    87,   220,   128,   344,   130,   168,   169,   226,   636,
     152,   104,   104,   104,   104,   101,   643,    51,    52,    58,
     107,    55,    70,   363,    87,    68,    69,   152,    87,    89,
      12,   658,   503,   174,   175,   176,   177,    14,   101,   257,
      67,   102,   101,   104,    67,    22,   673,   107,    87,   102,
      93,   104,   194,   195,   102,   197,    86,    68,    69,   107,
     202,   203,   101,   205,    10,   207,   228,   229,    14,   409,
      89,   101,   102,   100,   108,   293,   103,   100,   296,   373,
     103,    58,    93,    10,   302,   103,   304,    14,   107,   107,
      82,   431,   231,   235,   236,    87,    87,   239,    87,   241,
     101,    10,    11,    12,   107,    14,    87,   108,   100,    87,
      87,   103,   103,   331,   103,   333,   456,   457,   260,   261,
     101,    84,    31,   101,   101,   250,    35,    71,    72,    73,
      74,    75,    76,    96,    97,    98,    87,    87,    87,    83,
      10,    14,   101,   102,    14,    89,    90,    91,    92,   108,
     101,   101,   101,   295,   101,   102,   374,    14,   498,   154,
     155,   108,   157,   158,   159,   160,   161,   162,   101,   164,
     165,    14,   314,    62,    17,   108,   318,   101,   102,   321,
     398,   323,   569,   325,   108,   572,    14,   329,   330,   101,
     102,   531,   532,   101,   102,   101,   108,    10,   101,   102,
     108,    14,   108,    10,   346,   108,    13,    14,    15,    16,
     335,   336,   337,   338,   339,   103,   102,    10,   104,   107,
      13,   346,    15,    16,   103,    77,    78,    79,   107,   354,
     355,   356,   103,   101,   103,   102,   107,   104,   107,   381,
     382,   383,   100,   630,    10,    11,    12,     7,     8,     9,
      85,   102,   101,   104,   396,    94,    95,   399,   400,    14,
     102,   102,   104,   104,   406,   102,   484,   104,   102,   102,
     104,   104,   102,   102,   104,   104,   102,   495,   104,   102,
     102,   104,   104,   102,   102,   104,   104,   102,    14,   104,
     432,   433,   102,   102,   104,   104,   438,   102,    14,   104,
       3,     4,     5,     6,   102,   105,   104,    10,    11,    12,
      13,    14,    15,    16,   439,    18,    19,    20,   102,   102,
     104,   104,   102,   102,   104,   104,   101,   102,    31,    32,
     102,    14,   104,   551,   107,   102,    39,   104,   480,   101,
     102,    44,    45,    46,    87,   102,    49,   104,   102,   102,
     104,   104,   102,   101,   104,   101,    59,   101,   102,    87,
       3,     4,     5,     6,    91,    92,    48,    10,    11,    12,
      13,    14,    15,    16,   107,    18,    19,    20,   503,   504,
     505,   506,   101,   102,   107,    88,   101,   102,    31,    32,
     515,   103,    95,   103,   519,   108,    39,   101,   101,   106,
     103,    44,    45,    46,   107,    90,    49,   549,   550,   101,
     102,   101,   102,   101,   102,   108,    59,   101,   102,    38,
      39,   166,   167,   172,   173,   101,   103,    14,   103,    14,
     101,   570,    87,    87,    27,   577,    10,    14,    14,    27,
     582,   108,   103,   103,    80,    88,    93,   103,    14,    87,
      87,    90,    95,    87,   596,   106,   108,   101,   101,   101,
     103,    67,   107,    90,   107,   607,   101,   108,   101,   101,
      90,    14,    14,   101,    27,   101,   101,   108,   101,   621,
     101,    10,   607,   101,   626,   101,    10,    10,    10,    10,
     101,     3,     4,     5,     6,    27,   638,   639,    10,    11,
      12,    13,    14,    15,    16,   101,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    51,    14,    29,    30,    31,
      32,    33,    34,    35,    36,    14,    14,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   101,
     101,   101,    54,    55,   101,   101,    14,    59,    14,    68,
      69,    63,    71,    72,    73,    74,    75,    76,    77,    78,
      79,   101,   101,    67,    83,    84,    85,    67,   101,     7,
      89,    90,    91,    92,    93,    94,    88,    96,    97,    98,
     108,   100,     7,    95,   101,   101,   101,     7,    67,   101,
      62,   103,    10,    10,   101,   107,   108,     3,     4,     5,
       6,    14,    14,    14,    10,    11,    12,    13,    14,    15,
      16,    14,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   101,   107,    29,    30,    31,    32,    33,    34,    35,
      36,   101,   101,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   101,   108,   101,    54,    55,
     101,   101,   101,    59,   101,   101,    14,    63,    14,   101,
      14,    14,   101,   361,   101,   101,   101,   101,   101,   101,
     101,   170,   374,    -1,   171,    -1,    -1,    -1,   318,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,   101,    -1,   103,    -1,    -1,
      -1,   107,   108,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   101,    -1,   103,    -1,    -1,    -1,   107,   108,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    31,    32,    -1,    -1,    -1,   101,    -1,   103,
      39,    -1,    -1,   107,    -1,    44,    45,    46,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    62,
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
     107,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    31,    32,    -1,    -1,    -1,    -1,   103,   104,
      39,    -1,   107,    -1,    -1,    44,    45,    46,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      59,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,   104,    -1,    -1,   107,    -1,
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
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,
      -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,
      -1,    -1,    49,    -1,   103,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    59,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    31,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    39,    -1,    -1,    -1,   103,    44,    45,    46,
     107,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     119,   121,   124,   125,   128,   133,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   148,    82,
      87,   100,   103,   105,    89,   107,    87,   100,   103,    14,
      10,    11,    12,    13,    31,   101,   103,   133,   135,   147,
      10,    11,    12,    14,    31,    35,   103,   135,   103,   135,
     103,   135,   103,   135,   135,   140,    14,   133,   133,   135,
     101,   135,    10,    14,   135,    12,    12,   103,   135,   103,
     135,   133,   107,   134,   134,   107,   134,   135,   133,   133,
     135,    14,    14,    14,    31,    14,    10,   144,   144,   104,
     135,    62,   111,   101,    77,    78,    79,    71,    72,    73,
      74,    75,    76,    83,    89,    90,    91,    92,    68,    69,
      93,    85,    94,    95,    84,    96,    97,    98,   100,    14,
     135,    14,   104,   147,   135,    14,   135,   135,    14,    17,
      53,   104,   147,   101,    87,   105,    89,   107,   104,   135,
     147,   101,   102,    87,   101,    87,   101,    87,   101,    10,
      11,    12,    14,    10,    14,   135,   101,   102,   135,   101,
     102,   135,   133,   135,   133,    70,   102,   133,    68,    69,
     103,   107,   122,   101,   101,    87,    87,   107,    89,   107,
      89,   107,   135,   133,   135,   133,   135,   135,   135,   135,
      48,   126,   107,   103,    14,    57,   107,   102,   104,   108,
      24,    25,   108,   135,   138,   138,   136,   138,   138,   138,
     138,   138,   138,     7,     8,     9,   138,   138,   139,   139,
     140,   140,   141,   142,   143,   143,   144,   144,   144,   144,
      14,    17,   101,   103,   101,    87,   103,   104,   106,    90,
     108,   101,   103,    14,   103,   104,   135,   135,    14,   135,
     101,   104,   135,   135,   103,   135,     5,    14,   103,   131,
     135,    87,   101,    87,   101,    87,   101,   103,   122,    87,
      87,   102,   147,   102,   147,   104,    27,   104,    10,   147,
     140,   140,    10,    14,   104,   132,   111,   135,   135,   120,
      14,   135,    14,   135,   104,    27,   104,   108,   133,    14,
      22,   107,   127,   104,   132,   103,   103,   129,   104,   147,
     135,   135,   108,    80,   103,    14,   104,   147,   135,   104,
     147,    87,    87,    87,   104,   147,    14,   104,   130,   147,
     106,    90,   108,   101,   101,   135,    86,   101,   102,    67,
      67,   131,   101,   102,   101,   135,   103,   135,   135,   104,
     132,   135,   135,   147,   101,   147,   101,   133,   113,   133,
     133,   133,    70,   133,    70,    70,    10,    60,    61,   122,
     102,   104,    24,    25,   108,   135,   101,   101,    37,    38,
     108,    90,   108,    90,   108,   133,   133,   133,    14,    14,
     127,    51,    52,    55,   108,   122,   104,   104,   132,   132,
      64,    65,    66,   108,   104,   108,   108,   136,   104,   130,
     147,   101,   104,   101,   101,   104,   135,   135,   135,   104,
      67,   102,   104,   104,   102,   135,   147,   135,   135,   104,
       5,    14,   101,   135,   101,   102,   101,   122,   104,   101,
     101,   104,   104,    27,    10,    10,    10,    10,    10,    10,
      14,   122,   135,   135,   108,   135,   133,   101,   101,    27,
     108,    14,    53,    10,    13,    14,    15,    16,    51,   122,
     122,   104,   104,    14,    14,    14,   104,   104,   101,   101,
     101,   101,   101,   135,    14,   147,   101,   101,   101,    67,
      67,   102,   147,   122,   101,   101,   113,   133,   133,   133,
     133,    10,    60,    61,   108,   108,   133,   108,   133,   103,
     107,   123,   103,   123,    14,    22,    58,    87,   101,    14,
      22,    58,    87,   101,    10,    13,    15,    16,   101,   101,
      14,   122,   122,     7,     7,     7,    67,   104,   135,   135,
     147,   101,    10,    10,   104,   132,    62,   111,   104,   132,
      14,    14,   101,   135,    14,    14,   101,   135,    14,    22,
      58,    87,   101,    14,    22,    58,    87,   101,   101,   101,
     103,   123,   135,   101,   104,   123,   104,   108,    24,    25,
     108,   135,   123,   104,   101,   101,   101,   101,   101,   101,
      14,    14,   101,   135,    14,    14,   101,   135,   104,   132,
     101,   123,   135,   135,   108,   123,   101,   101,   101,   101,
     101,   101,   123,   104,   108,   108,   123
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
     112,   112,   113,   113,   113,   113,   113,   113,   114,   114,
     115,   115,   115,   115,   116,   116,   117,   118,   118,   118,
     118,   118,   118,   119,   119,   120,   120,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   122,   123,   123,   123,   123,   123,   124,   125,   126,
     126,   126,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   128,   129,   129,   129,
     129,   130,   130,   131,   131,   131,   131,   132,   132,   132,
     132,   132,   132,   132,   132,   133,   133,   133,   133,   133,
     134,   135,   135,   135,   136,   136,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     138,   138,   138,   139,   139,   139,   140,   140,   141,   141,
     142,   142,   142,   143,   143,   143,   143,   143,   144,   144,
     144,   145,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148
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
       1,     1,     5,     7,     7,     3,     5,     5,     5,     3,
       5,     7,     3,     5,     5,     3,     2,     7,     7,     5,
       7,     5,     3,     5,     7,     0,     4,     6,     5,     3,
       7,     6,     4,     7,     6,     7,     6,     5,     3,     4,
       5,     5,     3,     3,     4,     5,     5,     6,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     4,
       7,     6,     4,     8,     7,     5,     5,     0,     4,     4,
       4,     3,     5,     3,     3,     5,     5,     1,     2,     3,
       3,     3,     4,     5,     5,     3,     3,     4,     5,     5,
       3,     3,     2,     1,     5,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     2,     2,
       1,     1,     4,     3,     4,     3,     6,     6,     5,     3,
       3,     6,     5,     4,     4,     6,     6,     5,     3,     2,
       3,     3,     2,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     5,     5,     1,     1,     1,     1,     2,     3,     4,
       5,     1,     2
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
#line 2185 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 262 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2191 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 263 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2197 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 267 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2203 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 269 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2209 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 271 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2215 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 273 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2221 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 275 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2227 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2235 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 281 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2242 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 284 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2250 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 288 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2256 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 290 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2262 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 292 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2268 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 294 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2274 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 296 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2280 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 298 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2288 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 302 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2296 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 306 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2302 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 308 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2308 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 310 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2314 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 312 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2320 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 314 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2326 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 316 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2332 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 318 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2338 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 320 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2344 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 322 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2350 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 324 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2356 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 326 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2362 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 328 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2368 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 330 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2374 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 332 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2380 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 334 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2387 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 337 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2394 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 340 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2400 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 342 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2407 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 345 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2414 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 348 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2420 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 350 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2426 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 352 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2432 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2438 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 356 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2447 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 361 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2455 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 365 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2463 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 369 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2472 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 374 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2480 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 378 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2491 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 385 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2500 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 390 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2507 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 393 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2514 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 396 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2521 "raku.tab.c"
    break;

  case 53: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2528 "raku.tab.c"
    break;

  case 54: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2535 "raku.tab.c"
    break;

  case 55: /* stmt: expr ';'  */
#line 404 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2541 "raku.tab.c"
    break;

  case 56: /* stmt: ';'  */
#line 405 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 2547 "raku.tab.c"
    break;

  case 57: /* stmt: if_stmt  */
#line 406 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2553 "raku.tab.c"
    break;

  case 58: /* stmt: while_stmt  */
#line 407 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2559 "raku.tab.c"
    break;

  case 59: /* stmt: for_stmt  */
#line 408 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2565 "raku.tab.c"
    break;

  case 60: /* stmt: given_stmt  */
#line 409 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2571 "raku.tab.c"
    break;

  case 61: /* stmt: KW_TRY block  */
#line 411 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2577 "raku.tab.c"
    break;

  case 62: /* stmt: KW_TRY block KW_CATCH block  */
#line 413 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2583 "raku.tab.c"
    break;

  case 63: /* stmt: KW_CATCH block  */
#line 415 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2589 "raku.tab.c"
    break;

  case 64: /* stmt: block  */
#line 417 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2595 "raku.tab.c"
    break;

  case 65: /* stmt: unless_stmt  */
#line 418 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2601 "raku.tab.c"
    break;

  case 66: /* stmt: until_stmt  */
#line 419 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2607 "raku.tab.c"
    break;

  case 67: /* stmt: repeat_stmt  */
#line 420 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2613 "raku.tab.c"
    break;

  case 68: /* stmt: sub_decl  */
#line 421 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2619 "raku.tab.c"
    break;

  case 69: /* stmt: class_decl  */
#line 422 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2625 "raku.tab.c"
    break;

  case 70: /* stmt: role_decl  */
#line 423 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2631 "raku.tab.c"
    break;

  case 71: /* stmt: grammar_decl  */
#line 424 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2637 "raku.tab.c"
    break;

  case 72: /* if_stmt: KW_IF '(' expr ')' block  */
#line 428 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2643 "raku.tab.c"
    break;

  case 73: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 430 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2649 "raku.tab.c"
    break;

  case 74: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 432 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2655 "raku.tab.c"
    break;

  case 75: /* if_stmt: KW_IF expr block  */
#line 434 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2661 "raku.tab.c"
    break;

  case 76: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 436 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2667 "raku.tab.c"
    break;

  case 77: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 438 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2673 "raku.tab.c"
    break;

  case 78: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 442 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2679 "raku.tab.c"
    break;

  case 79: /* while_stmt: KW_WHILE expr block  */
#line 444 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 2685 "raku.tab.c"
    break;

  case 80: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 448 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2691 "raku.tab.c"
    break;

  case 81: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 450 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2697 "raku.tab.c"
    break;

  case 82: /* unless_stmt: KW_UNLESS expr block  */
#line 452 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2703 "raku.tab.c"
    break;

  case 83: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 454 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2709 "raku.tab.c"
    break;

  case 84: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 458 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2715 "raku.tab.c"
    break;

  case 85: /* until_stmt: KW_UNTIL expr block  */
#line 460 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2721 "raku.tab.c"
    break;

  case 86: /* repeat_stmt: KW_REPEAT block  */
#line 464 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2727 "raku.tab.c"
    break;

  case 87: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 468 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2737 "raku.tab.c"
    break;

  case 88: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 474 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2747 "raku.tab.c"
    break;

  case 89: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 480 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2756 "raku.tab.c"
    break;

  case 90: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 485 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2766 "raku.tab.c"
    break;

  case 91: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 491 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2775 "raku.tab.c"
    break;

  case 92: /* for_stmt: KW_FOR expr block  */
#line 496 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2782 "raku.tab.c"
    break;

  case 93: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 501 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2794 "raku.tab.c"
    break;

  case 94: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 509 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 2807 "raku.tab.c"
    break;

  case 95: /* when_list: %empty  */
#line 519 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 2813 "raku.tab.c"
    break;

  case 96: /* when_list: when_list KW_WHEN expr block  */
#line 521 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 2821 "raku.tab.c"
    break;

  case 97: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 527 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2833 "raku.tab.c"
    break;

  case 98: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 535 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2843 "raku.tab.c"
    break;

  case 99: /* sub_decl: KW_SUB IDENT sub_body  */
#line 541 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2853 "raku.tab.c"
    break;

  case 100: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 547 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2865 "raku.tab.c"
    break;

  case 101: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 555 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2875 "raku.tab.c"
    break;

  case 102: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 561 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 2885 "raku.tab.c"
    break;

  case 103: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 567 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2898 "raku.tab.c"
    break;

  case 104: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 576 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2909 "raku.tab.c"
    break;

  case 105: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 583 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2922 "raku.tab.c"
    break;

  case 106: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 592 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 2935 "raku.tab.c"
    break;

  case 107: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 601 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 2946 "raku.tab.c"
    break;

  case 108: /* sub_body: '{' stmt_list '}'  */
#line 609 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2952 "raku.tab.c"
    break;

  case 109: /* sub_body: '{' stmt_list expr '}'  */
#line 611 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 2959 "raku.tab.c"
    break;

  case 110: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 614 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 2965 "raku.tab.c"
    break;

  case 111: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 616 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 2971 "raku.tab.c"
    break;

  case 112: /* method_body: '{' stmt_list '}'  */
#line 619 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 2977 "raku.tab.c"
    break;

  case 113: /* method_body: '{' YADA '}'  */
#line 620 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 2983 "raku.tab.c"
    break;

  case 114: /* method_body: '{' stmt_list expr '}'  */
#line 622 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 2990 "raku.tab.c"
    break;

  case 115: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 625 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 2996 "raku.tab.c"
    break;

  case 116: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 627 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3002 "raku.tab.c"
    break;

  case 117: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 631 "raku.y"
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
#line 3020 "raku.tab.c"
    break;

  case 118: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 647 "raku.y"
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
#line 3037 "raku.tab.c"
    break;

  case 119: /* is_clauses: %empty  */
#line 661 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3043 "raku.tab.c"
    break;

  case 120: /* is_clauses: is_clauses IDENT IDENT  */
#line 663 "raku.y"
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
#line 3059 "raku.tab.c"
    break;

  case 121: /* is_clauses: is_clauses TESTOP IDENT  */
#line 675 "raku.y"
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
#line 3075 "raku.tab.c"
    break;

  case 122: /* class_body_list: %empty  */
#line 688 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3081 "raku.tab.c"
    break;

  case 123: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 690 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3088 "raku.tab.c"
    break;

  case 124: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 693 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3095 "raku.tab.c"
    break;

  case 125: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 696 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3102 "raku.tab.c"
    break;

  case 126: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 699 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3109 "raku.tab.c"
    break;

  case 127: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 702 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3116 "raku.tab.c"
    break;

  case 128: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 705 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3123 "raku.tab.c"
    break;

  case 129: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 708 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3130 "raku.tab.c"
    break;

  case 130: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 711 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3137 "raku.tab.c"
    break;

  case 131: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 714 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3144 "raku.tab.c"
    break;

  case 132: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 717 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3151 "raku.tab.c"
    break;

  case 133: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 720 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3158 "raku.tab.c"
    break;

  case 134: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 723 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3165 "raku.tab.c"
    break;

  case 135: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 726 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3176 "raku.tab.c"
    break;

  case 136: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 733 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3187 "raku.tab.c"
    break;

  case 137: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 740 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3198 "raku.tab.c"
    break;

  case 138: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 747 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3209 "raku.tab.c"
    break;

  case 139: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 754 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3217 "raku.tab.c"
    break;

  case 140: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 758 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3225 "raku.tab.c"
    break;

  case 141: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 762 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3233 "raku.tab.c"
    break;

  case 142: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 766 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3241 "raku.tab.c"
    break;

  case 143: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 770 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3252 "raku.tab.c"
    break;

  case 144: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 777 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3263 "raku.tab.c"
    break;

  case 145: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 784 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3274 "raku.tab.c"
    break;

  case 146: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 791 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3285 "raku.tab.c"
    break;

  case 147: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 798 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3299 "raku.tab.c"
    break;

  case 148: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 808 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3311 "raku.tab.c"
    break;

  case 149: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 816 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3323 "raku.tab.c"
    break;

  case 150: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 824 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3336 "raku.tab.c"
    break;

  case 151: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 833 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3347 "raku.tab.c"
    break;

  case 152: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 840 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3358 "raku.tab.c"
    break;

  case 153: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 847 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3372 "raku.tab.c"
    break;

  case 154: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 857 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3384 "raku.tab.c"
    break;

  case 155: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 865 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 3396 "raku.tab.c"
    break;

  case 156: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 875 "raku.y"
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
#line 3413 "raku.tab.c"
    break;

  case 157: /* grammar_body_list: %empty  */
#line 889 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3419 "raku.tab.c"
    break;

  case 158: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 891 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3428 "raku.tab.c"
    break;

  case 159: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 896 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3437 "raku.tab.c"
    break;

  case 160: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 901 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3446 "raku.tab.c"
    break;

  case 161: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 908 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3454 "raku.tab.c"
    break;

  case 162: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 912 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3462 "raku.tab.c"
    break;

  case 163: /* pair_list: IDENT OP_FATARROW expr  */
#line 918 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3468 "raku.tab.c"
    break;

  case 164: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 920 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3474 "raku.tab.c"
    break;

  case 165: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 922 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3480 "raku.tab.c"
    break;

  case 166: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 924 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3486 "raku.tab.c"
    break;

  case 167: /* param_list: VAR_SCALAR  */
#line 927 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3492 "raku.tab.c"
    break;

  case 168: /* param_list: IDENT VAR_SCALAR  */
#line 928 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3498 "raku.tab.c"
    break;

  case 169: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 929 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3504 "raku.tab.c"
    break;

  case 170: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 930 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3510 "raku.tab.c"
    break;

  case 171: /* param_list: param_list ',' VAR_SCALAR  */
#line 931 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3516 "raku.tab.c"
    break;

  case 172: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 932 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3522 "raku.tab.c"
    break;

  case 173: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 933 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3528 "raku.tab.c"
    break;

  case 174: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 934 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3534 "raku.tab.c"
    break;

  case 175: /* block: '{' stmt_list '}'  */
#line 937 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3540 "raku.tab.c"
    break;

  case 176: /* block: '{' YADA '}'  */
#line 938 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3546 "raku.tab.c"
    break;

  case 177: /* block: '{' stmt_list expr '}'  */
#line 940 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 3552 "raku.tab.c"
    break;

  case 178: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 942 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3558 "raku.tab.c"
    break;

  case 179: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 944 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3564 "raku.tab.c"
    break;

  case 180: /* closure: '{' expr '}'  */
#line 947 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3570 "raku.tab.c"
    break;

  case 181: /* expr: VAR_SCALAR '=' expr  */
#line 950 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3576 "raku.tab.c"
    break;

  case 182: /* expr: KW_GATHER block  */
#line 951 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3586 "raku.tab.c"
    break;

  case 183: /* expr: tern_expr  */
#line 956 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3592 "raku.tab.c"
    break;

  case 184: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 960 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3598 "raku.tab.c"
    break;

  case 185: /* tern_expr: cmp_expr  */
#line 961 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3604 "raku.tab.c"
    break;

  case 186: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 964 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3610 "raku.tab.c"
    break;

  case 187: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 965 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3616 "raku.tab.c"
    break;

  case 188: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 966 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3622 "raku.tab.c"
    break;

  case 189: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 967 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3628 "raku.tab.c"
    break;

  case 190: /* cmp_expr: jct_expr '<' jct_expr  */
#line 968 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3634 "raku.tab.c"
    break;

  case 191: /* cmp_expr: jct_expr '>' jct_expr  */
#line 969 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3640 "raku.tab.c"
    break;

  case 192: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 970 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3646 "raku.tab.c"
    break;

  case 193: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 971 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3652 "raku.tab.c"
    break;

  case 194: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 972 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3658 "raku.tab.c"
    break;

  case 195: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 973 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3664 "raku.tab.c"
    break;

  case 196: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 975 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3674 "raku.tab.c"
    break;

  case 197: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 981 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3684 "raku.tab.c"
    break;

  case 198: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 987 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3694 "raku.tab.c"
    break;

  case 199: /* cmp_expr: jct_expr  */
#line 992 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3700 "raku.tab.c"
    break;

  case 200: /* jct_expr: jct_expr '|' range_expr  */
#line 995 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3706 "raku.tab.c"
    break;

  case 201: /* jct_expr: jct_expr '&' range_expr  */
#line 996 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3712 "raku.tab.c"
    break;

  case 202: /* jct_expr: range_expr  */
#line 997 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3718 "raku.tab.c"
    break;

  case 203: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1000 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3724 "raku.tab.c"
    break;

  case 204: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1001 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3730 "raku.tab.c"
    break;

  case 205: /* range_expr: add_expr  */
#line 1002 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3736 "raku.tab.c"
    break;

  case 206: /* add_expr: add_expr '~' repl_expr  */
#line 1005 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3742 "raku.tab.c"
    break;

  case 207: /* add_expr: repl_expr  */
#line 1006 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 3748 "raku.tab.c"
    break;

  case 208: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1009 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3754 "raku.tab.c"
    break;

  case 209: /* repl_expr: addsub_expr  */
#line 1010 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 3760 "raku.tab.c"
    break;

  case 210: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1013 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3766 "raku.tab.c"
    break;

  case 211: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1014 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3772 "raku.tab.c"
    break;

  case 212: /* addsub_expr: mul_expr  */
#line 1015 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 3778 "raku.tab.c"
    break;

  case 213: /* mul_expr: mul_expr '*' unary_expr  */
#line 1018 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3784 "raku.tab.c"
    break;

  case 214: /* mul_expr: mul_expr '/' unary_expr  */
#line 1019 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3790 "raku.tab.c"
    break;

  case 215: /* mul_expr: mul_expr '%' unary_expr  */
#line 1020 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3796 "raku.tab.c"
    break;

  case 216: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1021 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3802 "raku.tab.c"
    break;

  case 217: /* mul_expr: unary_expr  */
#line 1022 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 3808 "raku.tab.c"
    break;

  case 218: /* unary_expr: '-' unary_expr  */
#line 1025 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 3814 "raku.tab.c"
    break;

  case 219: /* unary_expr: '!' unary_expr  */
#line 1026 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 3820 "raku.tab.c"
    break;

  case 220: /* unary_expr: postfix_expr  */
#line 1027 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 3826 "raku.tab.c"
    break;

  case 221: /* postfix_expr: call_expr  */
#line 1029 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 3832 "raku.tab.c"
    break;

  case 222: /* call_expr: IDENT '(' arg_list ')'  */
#line 1032 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3841 "raku.tab.c"
    break;

  case 223: /* call_expr: IDENT '(' ')'  */
#line 1036 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 3847 "raku.tab.c"
    break;

  case 224: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1038 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 3856 "raku.tab.c"
    break;

  case 225: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1043 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 3862 "raku.tab.c"
    break;

  case 226: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1045 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3872 "raku.tab.c"
    break;

  case 227: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1051 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3882 "raku.tab.c"
    break;

  case 228: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1057 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 3890 "raku.tab.c"
    break;

  case 229: /* call_expr: IDENT '.' KW_NEW  */
#line 1061 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3898 "raku.tab.c"
    break;

  case 230: /* call_expr: IDENT '.' IDENT  */
#line 1065 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3907 "raku.tab.c"
    break;

  case 231: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1070 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3918 "raku.tab.c"
    break;

  case 232: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1077 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3927 "raku.tab.c"
    break;

  case 233: /* call_expr: IDENT '.' CARET IDENT  */
#line 1082 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3937 "raku.tab.c"
    break;

  case 234: /* call_expr: atom '.' CARET IDENT  */
#line 1088 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3947 "raku.tab.c"
    break;

  case 235: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1094 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3958 "raku.tab.c"
    break;

  case 236: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1101 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 3969 "raku.tab.c"
    break;

  case 237: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1108 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3978 "raku.tab.c"
    break;

  case 238: /* call_expr: atom '.' IDENT  */
#line 1113 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 3987 "raku.tab.c"
    break;

  case 239: /* call_expr: KW_DIE expr  */
#line 1118 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 3993 "raku.tab.c"
    break;

  case 240: /* call_expr: KW_MAP closure expr  */
#line 1120 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3999 "raku.tab.c"
    break;

  case 241: /* call_expr: KW_GREP closure expr  */
#line 1122 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4005 "raku.tab.c"
    break;

  case 242: /* call_expr: KW_SORT expr  */
#line 1124 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4011 "raku.tab.c"
    break;

  case 243: /* call_expr: KW_SORT closure expr  */
#line 1126 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4017 "raku.tab.c"
    break;

  case 244: /* call_expr: atom  */
#line 1127 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4023 "raku.tab.c"
    break;

  case 245: /* arg_list: expr  */
#line 1130 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4029 "raku.tab.c"
    break;

  case 246: /* arg_list: arg_list ',' expr  */
#line 1131 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4035 "raku.tab.c"
    break;

  case 247: /* atom: LIT_INT  */
#line 1134 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4041 "raku.tab.c"
    break;

  case 248: /* atom: LIT_FLOAT  */
#line 1135 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4047 "raku.tab.c"
    break;

  case 249: /* atom: LIT_STR  */
#line 1136 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4053 "raku.tab.c"
    break;

  case 250: /* atom: WORDLIST  */
#line 1138 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4065 "raku.tab.c"
    break;

  case 251: /* atom: LIT_INTERP_STR  */
#line 1145 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4071 "raku.tab.c"
    break;

  case 252: /* atom: VAR_SCALAR  */
#line 1146 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4077 "raku.tab.c"
    break;

  case 253: /* atom: VAR_ARRAY  */
#line 1147 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4083 "raku.tab.c"
    break;

  case 254: /* atom: VAR_HASH  */
#line 1148 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4089 "raku.tab.c"
    break;

  case 255: /* atom: VAR_CAPTURE  */
#line 1150 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4097 "raku.tab.c"
    break;

  case 256: /* atom: VAR_FH  */
#line 1154 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4105 "raku.tab.c"
    break;

  case 257: /* atom: VAR_NAMED_CAPTURE  */
#line 1158 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4112 "raku.tab.c"
    break;

  case 258: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1161 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4118 "raku.tab.c"
    break;

  case 259: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1163 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4124 "raku.tab.c"
    break;

  case 260: /* atom: VAR_HASH '{' expr '}'  */
#line 1165 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4130 "raku.tab.c"
    break;

  case 261: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1167 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4136 "raku.tab.c"
    break;

  case 262: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1169 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4142 "raku.tab.c"
    break;

  case 263: /* atom: IDENT  */
#line 1170 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4148 "raku.tab.c"
    break;

  case 264: /* atom: VAR_TWIGIL  */
#line 1172 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4156 "raku.tab.c"
    break;

  case 265: /* atom: VAR_ARRAY_TWIGIL  */
#line 1176 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4164 "raku.tab.c"
    break;

  case 266: /* atom: VAR_HASH_TWIGIL  */
#line 1180 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4172 "raku.tab.c"
    break;

  case 267: /* atom: '(' ')'  */
#line 1183 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 4178 "raku.tab.c"
    break;

  case 268: /* atom: '(' expr ')'  */
#line 1184 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 4184 "raku.tab.c"
    break;

  case 269: /* atom: '(' expr ',' ')'  */
#line 1186 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 4190 "raku.tab.c"
    break;

  case 270: /* atom: '(' expr ',' arg_list ')'  */
#line 1188 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 4197 "raku.tab.c"
    break;

  case 271: /* atom: block  */
#line 1190 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4203 "raku.tab.c"
    break;

  case 272: /* atom: KW_SUB block  */
#line 1191 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4209 "raku.tab.c"
    break;


#line 4213 "raku.tab.c"

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

#line 1193 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
