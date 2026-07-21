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
static tree_t *seq1(tree_t *stmt) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmt) expr_add_child(seq, stmt);
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

#line 254 "raku.tab.c"

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
  YYSYMBOL_OP_INC = 84,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 85,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 86,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 87,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 88,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 89,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 90,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 91,                    /* OP_DOR  */
  YYSYMBOL_OP_DIV = 92,                    /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 93,                  /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 94,                 /* OP_REP_XX  */
  YYSYMBOL_95_ = 95,                       /* '='  */
  YYSYMBOL_96_ = 96,                       /* '!'  */
  YYSYMBOL_97_ = 97,                       /* '<'  */
  YYSYMBOL_98_ = 98,                       /* '>'  */
  YYSYMBOL_99_ = 99,                       /* '|'  */
  YYSYMBOL_100_ = 100,                     /* '&'  */
  YYSYMBOL_101_ = 101,                     /* '~'  */
  YYSYMBOL_102_ = 102,                     /* '+'  */
  YYSYMBOL_103_ = 103,                     /* '-'  */
  YYSYMBOL_104_ = 104,                     /* '*'  */
  YYSYMBOL_105_ = 105,                     /* '/'  */
  YYSYMBOL_106_ = 106,                     /* '%'  */
  YYSYMBOL_UMINUS = 107,                   /* UMINUS  */
  YYSYMBOL_108_ = 108,                     /* '.'  */
  YYSYMBOL_109_ = 109,                     /* ';'  */
  YYSYMBOL_110_ = 110,                     /* ','  */
  YYSYMBOL_111_ = 111,                     /* '('  */
  YYSYMBOL_112_ = 112,                     /* ')'  */
  YYSYMBOL_113_ = 113,                     /* '['  */
  YYSYMBOL_114_ = 114,                     /* ']'  */
  YYSYMBOL_115_ = 115,                     /* '{'  */
  YYSYMBOL_116_ = 116,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 117,                 /* $accept  */
  YYSYMBOL_program = 118,                  /* program  */
  YYSYMBOL_stmt_list = 119,                /* stmt_list  */
  YYSYMBOL_stmt = 120,                     /* stmt  */
  YYSYMBOL_if_stmt = 121,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 122,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 123,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 124,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 125,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 126,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 127,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 128,               /* given_stmt  */
  YYSYMBOL_when_list = 129,                /* when_list  */
  YYSYMBOL_sub_decl = 130,                 /* sub_decl  */
  YYSYMBOL_sub_body = 131,                 /* sub_body  */
  YYSYMBOL_method_body = 132,              /* method_body  */
  YYSYMBOL_class_decl = 133,               /* class_decl  */
  YYSYMBOL_role_decl = 134,                /* role_decl  */
  YYSYMBOL_is_clauses = 135,               /* is_clauses  */
  YYSYMBOL_class_body_list = 136,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 137,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 138,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 139,           /* named_arg_list  */
  YYSYMBOL_pair_list = 140,                /* pair_list  */
  YYSYMBOL_param_list = 141,               /* param_list  */
  YYSYMBOL_block = 142,                    /* block  */
  YYSYMBOL_closure = 143,                  /* closure  */
  YYSYMBOL_expr = 144,                     /* expr  */
  YYSYMBOL_tern_expr = 145,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 146,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 147,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 148,                 /* dor_expr  */
  YYSYMBOL_range_expr = 149,               /* range_expr  */
  YYSYMBOL_add_expr = 150,                 /* add_expr  */
  YYSYMBOL_repl_expr = 151,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 152,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 153,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 154,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 155,             /* postfix_expr  */
  YYSYMBOL_call_expr = 156,                /* call_expr  */
  YYSYMBOL_arg_list = 157,                 /* arg_list  */
  YYSYMBOL_atom = 158                      /* atom  */
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
#define YYLAST   2762

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  309
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  786

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   350


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
       2,     2,     2,    96,     2,     2,     2,   106,   100,     2,
     111,   112,   104,   102,   110,   103,   108,   105,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   109,
      97,    95,    98,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   113,     2,   114,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   115,    99,   116,   101,     2,     2,     2,
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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
     107
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   260,   260,   271,   272,   275,   277,   279,   281,   283,
     285,   289,   292,   296,   298,   300,   302,   304,   306,   310,
     314,   316,   318,   320,   322,   324,   326,   328,   330,   332,
     334,   336,   338,   340,   342,   345,   348,   350,   353,   356,
     358,   360,   362,   364,   369,   373,   377,   382,   386,   393,
     398,   401,   404,   407,   410,   413,   415,   417,   419,   421,
     424,   427,   430,   434,   437,   440,   443,   446,   449,   452,
     455,   458,   461,   464,   467,   470,   471,   472,   473,   474,
     475,   476,   478,   480,   482,   484,   485,   486,   487,   488,
     489,   490,   493,   495,   497,   499,   501,   503,   505,   507,
     511,   513,   515,   517,   519,   521,   525,   527,   531,   533,
     535,   537,   541,   543,   547,   551,   557,   563,   568,   574,
     579,   584,   592,   603,   604,   610,   618,   624,   630,   638,
     644,   650,   659,   666,   675,   684,   693,   694,   697,   699,
     703,   704,   705,   708,   710,   714,   730,   745,   746,   758,
     772,   773,   776,   779,   782,   785,   788,   791,   794,   797,
     800,   803,   806,   809,   816,   823,   830,   837,   841,   845,
     849,   853,   860,   867,   874,   881,   891,   899,   907,   916,
     923,   930,   940,   948,   958,   973,   974,   979,   984,   991,
     995,  1001,  1003,  1005,  1007,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1021,  1022,  1023,  1025,  1027,  1029,  1033,
    1036,  1039,  1042,  1045,  1048,  1052,  1055,  1056,  1061,  1064,
    1066,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1079,  1085,  1091,  1097,  1100,  1101,  1102,  1105,  1107,
    1110,  1111,  1112,  1115,  1116,  1119,  1120,  1123,  1124,  1125,
    1128,  1129,  1130,  1131,  1132,  1135,  1136,  1137,  1139,  1141,
    1146,  1147,  1152,  1154,  1160,  1166,  1170,  1174,  1179,  1186,
    1191,  1197,  1203,  1210,  1217,  1222,  1227,  1229,  1231,  1233,
    1235,  1237,  1240,  1241,  1244,  1245,  1246,  1247,  1255,  1256,
    1257,  1258,  1259,  1263,  1267,  1270,  1272,  1274,  1276,  1278,
    1280,  1281,  1285,  1289,  1293,  1294,  1295,  1297,  1300,  1301
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
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX",
  "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "UMINUS", "'.'", "';'", "','", "'('", "')'", "'['", "']'",
  "'{'", "'}'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "for_stmt", "given_stmt", "when_list", "sub_decl", "sub_body",
  "method_body", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "cmp_expr", "jct_expr", "dor_expr", "range_expr", "add_expr",
  "repl_expr", "addsub_expr", "mul_expr", "unary_expr", "postfix_expr",
  "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-549)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-309)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -549,    18,  1086,  -549,  -549,  -549,  -549,  -549,   355,   -63,
     -33,   -15,   125,  -549,  -549,  -549,  -549,  -549,   101,   469,
     319,  1927,  1987,  2047,  2107,  2167,    -8,    21,  2167,   530,
     243,  2167,   147,   174,  2227,  2287,    21,    32,    32,  2347,
      21,    21,  2167,   187,   217,    92,  -549,   276,   292,   318,
    2647,  2647,  -549,  1147,   287,  -549,  -549,  -549,  -549,  -549,
    -549,  -549,  -549,  -549,  -549,  -549,  -549,  2636,   345,  -549,
     331,   549,   289,  -549,   134,   285,   315,   251,  -549,  -549,
    -549,   286,   424,   343,   351,  2167,  2167,  2167,  2167,  2167,
    2167,   435,  1207,  2167,   448,  2167,  2167,    22,  1267,   361,
     118,   283,   -18,  -549,    21,  -549,  1327,  -549,  -549,   385,
     -67,   191,   228,   457,   478,   268,  1147,   322,  1147,    15,
    1147,    21,  1147,    21,   -45,   143,   196,  -549,  -549,   423,
    -549,   429,   452,   468,   462,    -7,     2,  1147,    21,  1147,
      21,  -549,  2167,  2167,  2167,  1387,  2167,  -549,   461,  -549,
    -549,  -549,   467,   456,    17,   519,   464,   498,   520,  -549,
    -549,  -549,   -10,   521,   744,  2167,  2167,  2167,  2167,  2167,
    -549,  2647,  2647,  2647,  2647,  2647,  2647,  2647,  2647,  2647,
     512,  2647,  2647,  2647,  2647,  2647,  2647,  2647,  2647,  2647,
    2647,  2647,  2647,  2647,  2647,  2647,   267,   208,  -549,  -549,
     534,   583,   593,   599,   600,   601,   121,  -549,   111,   550,
     562,   595,   603,   604,   700,   605,  -549,   400,  -549,  2167,
    2167,   704,  2167,   610,   -10,   413,  -549,  2167,  2167,  -549,
    2407,  -549,  2467,  -549,   229,   249,   258,   314,   625,   626,
     414,  2167,  2167,  2167,  2167,  -549,  2167,   417,  2167,  2167,
    -549,  2167,   427,   396,   441,  -549,   713,  2167,  -549,  2647,
    2647,    -3,  -549,  -549,  -549,  -549,  2167,  2167,  -549,   710,
    2167,   711,  2167,   444,   699,   445,  -549,   611,  -549,  -549,
    -549,    21,    -5,  -549,    33,   617,   618,  -549,  -549,  -549,
    1447,  -549,  -549,   588,   619,   112,   635,  1927,  1987,  2167,
    -549,   225,   622,   624,   627,   628,   629,   399,   399,   654,
     399,   399,   399,   399,   399,   399,  -549,  -549,  -549,   399,
     399,  -549,  -549,  -549,   634,   634,   285,   315,   251,   251,
    -549,  -549,  -549,  -549,   632,   725,  -549,  1507,  -549,  -549,
    -549,  -549,  -549,  -549,  2167,  1567,  -549,   645,   649,   650,
    -549,  1627,  -549,  1687,  -549,  -549,   637,   655,   636,  -549,
     652,  -549,   662,  1147,   199,   705,    71,  1747,   394,   672,
    2167,  -549,  2527,  -549,  2167,  -549,    48,  -549,  2167,  2167,
    1447,   673,   686,   687,   688,   407,  1447,   691,   692,   421,
      21,   -14,  2587,  -549,    21,    21,   135,   -55,    52,  -549,
      35,   689,   454,   858,   693,   696,    23,   708,   694,   714,
     695,    21,    21,    21,  -549,  -549,   794,   795,  -549,    75,
     689,   458,    49,   350,    12,  -549,   471,   799,  2167,  2167,
    2167,   230,    40,    98,  -549,  -549,  -549,  -549,  -549,  -549,
    2647,  1807,  -549,   706,   475,   707,   709,   476,  2167,  2167,
    2167,  -549,   480,   195,  -549,   488,   489,  -549,  -549,  -549,
    -549,  -549,   492,  2167,  -549,  2167,  2167,  2167,   493,  -549,
     176,  -549,   712,  1147,   450,   715,   689,   517,   716,   717,
     518,  -549,  -549,  -549,  -549,  -549,   528,  -549,  -549,  -549,
     566,  -549,  -549,  1147,    21,  -549,  -549,   804,  -549,   807,
     809,  -549,   810,   812,  -549,   369,   689,  1927,  1987,  -549,
     250,  -549,  -549,  2167,    21,  -549,  -549,  -549,   718,   721,
     796,  -549,  -549,  -549,  -549,   122,    39,   420,   780,  -549,
    -549,   689,   689,   532,   540,   818,   819,   820,  -549,  -549,
     163,   722,   719,   206,  -549,  -549,  -549,  -549,  -549,   541,
     546,  -549,   728,  -549,  -549,   729,   730,   732,   733,  -549,
    2167,   829,  -549,  -549,  1447,   735,   559,  -549,  -549,   736,
     779,   781,  -549,   547,  -549,  2167,  -549,  -549,   689,  -549,
    -549,   740,   741,   -14,  -549,   551,   667,    21,    21,    21,
    -549,  -549,  -549,   124,  -549,   275,   178,  -549,    21,   738,
    -549,  -549,    21,  -549,   340,   375,    -6,    97,   443,   742,
     743,   842,  -549,  -549,   689,   689,   850,   851,   859,  2167,
    1867,   770,   772,  -549,  -549,  -549,  -549,  -549,  -549,  -549,
    -549,  -549,   808,   555,  -549,  -549,  -549,  2167,  2167,  1447,
     591,  -549,  -549,  -549,  -549,  -549,    21,    21,  -549,  -549,
    -549,  -549,  -549,   875,   876,  -549,  -549,  -549,  -549,  -549,
      57,   833,  -549,    58,  -549,   882,   895,   801,  2167,  -549,
     897,   900,   806,  2167,  -549,    99,   166,   811,   813,  -549,
    -549,   382,  -549,  -549,  -549,  -549,  -549,   253,   279,   569,
    2167,  2167,  2167,   814,  -549,  -549,   570,  -549,   670,  -549,
    -549,  -549,   803,   574,   800,   972,   803,   575,   815,   816,
    -549,   817,   821,   822,  -549,   823,   905,   913,   824,  2167,
    -549,   914,   915,   825,  2167,  -549,  -549,  -549,    59,  -549,
    -549,  -549,   296,   297,   298,  -549,  -549,   826,    21,  -549,
    -549,   803,  -549,  1927,  1987,  -549,   284,  -549,   803,  -549,
    -549,  -549,  -549,  -549,  -549,   827,   828,  -549,   830,   831,
     832,  -549,   835,   803,   581,  -549,  -549,  -549,  -549,  -549,
    -549,   306,   211,  -549,  -549,  -549,  -549,  -549,  -549,  -549,
    -549,  -549,   803,  -549,  -549,  -549
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   284,   285,   286,   288,   289,   290,
     291,   301,   300,   302,   303,   292,   293,   294,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   287,     0,     0,     0,
       0,     0,    76,     0,     3,     4,    77,    78,    85,    86,
      87,    79,    80,    88,    89,    90,    91,    84,     0,   218,
     220,   234,   237,   239,   242,   244,   246,   249,   254,   257,
     258,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,   290,   291,   301,     0,    29,     0,   308,   282,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,   309,   217,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,     0,     0,     0,     3,     0,   279,    81,    83,
     276,   147,     0,     0,     0,     0,     0,     0,   289,   256,
     255,   304,     0,     0,     0,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    71,    72,
       0,     0,     0,     0,     0,   216,     0,   262,     0,     0,
       0,     0,     0,   267,     0,   266,   260,     0,    24,     0,
       0,     0,     0,   304,     0,     0,    32,     0,     0,     6,
       0,     7,     0,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
      36,     0,     0,    95,     0,   107,     0,     0,   120,     0,
       0,     0,     3,   127,    39,    40,     0,     0,   123,     0,
       0,     0,     0,     0,   110,     0,   113,     0,   277,   278,
     280,     0,     0,   150,     0,     0,     0,   185,    73,    74,
       0,   305,   204,   289,   290,   291,   301,     0,     0,     0,
     203,     0,     0,     0,     0,     0,     0,   221,   222,     0,
     223,   224,   227,   228,   229,   230,   231,   232,   233,   225,
     226,   235,   236,   238,   240,   241,   243,   245,   247,   248,
     253,   250,   251,   252,   275,     0,    45,     0,    66,    67,
      68,    69,    70,    42,     0,     0,   261,   295,   296,   297,
      47,     0,   270,     0,   259,   216,     0,     0,     0,    31,
       0,   283,     0,     0,     0,   286,   300,     0,     0,     0,
       0,    21,     0,    22,     0,    23,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     305,     0,     0,    98,   305,     0,     0,   240,   241,   195,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,   305,   215,    82,     0,     0,   150,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,   205,    55,    57,    59,    56,    58,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,   269,     0,   300,   265,     0,     0,   295,   296,   297,
      30,     5,     0,     0,     9,     0,     0,     0,     0,    14,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    63,    62,    61,    34,     0,    64,    65,    37,
      92,    97,    96,     0,     0,   106,   117,     0,   119,     0,
       0,   196,     0,     0,   126,     0,     0,     0,     0,   136,
       0,    26,    25,     0,     0,   121,   298,   299,     0,     0,
     108,   111,   112,   148,   149,     0,     0,     0,     0,   146,
     135,     0,     0,     0,     0,     0,     0,     0,   184,   307,
       0,     0,     0,     0,   206,   207,   214,   219,   274,     0,
       0,    44,     0,    46,    49,     0,     0,     0,     0,   268,
       0,     0,   263,   264,     0,     0,     0,   192,   191,     0,
       0,     0,    16,     0,    17,     0,    20,   129,     0,    28,
      27,   307,   307,     0,    99,     0,   100,     0,     0,     0,
     197,   198,   199,     0,   125,     0,     0,   137,     0,     0,
      53,    54,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   132,     0,     0,     0,     0,     0,     0,
       0,   295,   297,   211,   273,   272,    43,    48,    50,    51,
      52,   189,     0,     0,    11,    10,    15,     0,     0,     0,
       0,   128,    35,    38,    94,    93,   305,     0,   104,   118,
     115,   116,   200,     0,     0,   138,   139,   124,   122,   109,
       0,     3,   177,     0,   180,     0,     0,     0,     0,   156,
       0,     0,     0,     0,   151,     0,     0,     0,     0,   152,
     153,     0,   131,   133,   186,   187,   188,     0,     0,     0,
       0,     0,     0,   307,   194,   193,     0,    18,   101,   102,
     201,   202,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,     0,   168,     0,     0,     0,     0,     0,
     158,     0,     0,     0,     0,   157,   154,   155,     0,   183,
     210,   209,     0,     0,     0,   190,    12,   307,     0,   105,
     176,     0,   141,     0,     0,   140,     0,   179,     0,   165,
     166,   160,   163,   164,   159,     0,     0,   169,     0,     0,
       0,   170,     0,     0,     0,   208,   212,   213,    19,   103,
     175,     0,     0,   142,   178,   173,   174,   162,   171,   172,
     161,   182,     0,   143,   144,   181
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -549,  -549,     1,  -549,  -388,  -488,  -549,  -549,  -549,  -549,
    -549,  -549,  -549,  -549,  -233,  -548,  -549,  -549,  -549,   527,
    -549,  -549,   497,   579,  -271,     8,   666,    -2,  -168,  -549,
     437,  -549,   293,   -11,   759,   760,   516,     4,  -549,  -549,
     -68,  -549
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   164,    55,    56,   393,    57,    58,    59,    60,
      61,    62,   406,    63,   263,   662,    64,    65,   282,   419,
      66,   424,   455,   368,   402,   107,   143,   108,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
     109,    81
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,     2,   584,   491,   377,   309,   126,   399,   665,   416,
      67,   400,    23,   421,   125,   499,   666,   417,     3,   117,
     119,   121,   123,   124,   208,   256,   129,   131,   228,   134,
     217,   285,   138,   140,   127,   128,   213,   147,   225,   214,
     150,   248,   229,   399,   141,   501,   188,   400,   148,   149,
      93,   162,   667,   604,   159,   160,   249,   664,   399,   399,
     513,   514,   400,   400,    94,   257,   248,   399,   399,   399,
      54,   400,   400,   400,   286,   215,   535,   536,   537,   221,
      96,   249,    95,   200,   201,   202,   203,   204,   205,   668,
     269,   209,   605,   211,   212,   502,   503,   222,   648,   271,
     290,    54,   291,   669,   224,   477,   153,    54,   270,   401,
     418,   670,   127,   716,   240,    99,   247,   272,   252,   671,
     254,   717,   500,   154,   250,   251,   526,   527,   538,   253,
     528,   255,   258,   729,   652,   273,    54,   275,   467,   515,
     277,   278,   279,   277,   280,   420,   274,   142,   276,   250,
     251,   533,   534,   188,   740,   672,   545,   718,   747,   135,
     476,   532,   301,   302,   303,   304,   305,   306,   504,   702,
     706,   763,    67,   526,   527,   324,   325,   528,   385,    97,
     721,   570,    98,   389,   653,   654,   136,   530,   722,   396,
     571,   529,   673,   770,   719,   644,   330,   331,   332,   333,
     774,   151,   186,   187,   248,   497,   674,   264,   720,    94,
     739,   259,   260,   219,   546,   781,   344,   355,   356,   249,
     358,   227,   426,   346,   723,   361,   362,   429,   364,    92,
     369,   152,   345,    97,   785,   188,    98,   248,   603,   381,
     382,   383,   384,   577,   188,   227,   387,   388,   397,   398,
      54,   165,   249,   132,   166,   167,   241,   133,   619,   242,
     243,   724,   560,   403,   404,   405,   168,   169,   408,   444,
     410,   244,   547,   594,   620,   725,   165,   447,   238,   166,
     167,   334,   239,   452,   335,   456,   230,   250,   251,   415,
     155,   168,   169,   463,   656,   431,   432,   433,   612,   613,
     231,   241,   156,    97,   242,   243,    98,   261,   464,   465,
     165,   262,   480,   166,   167,   350,   244,   336,   486,   337,
     250,   251,   623,   232,   370,   168,   169,   784,   157,   110,
     111,   112,   241,   113,   170,   242,   243,   233,   371,   245,
     246,   434,   445,   192,   372,   641,   544,   244,   241,   163,
     114,   242,   243,   374,   115,   193,   194,   195,   373,   170,
     399,   462,   553,   244,   400,   162,   597,   375,   472,   730,
     474,   165,   475,   550,   166,   167,   478,   479,   189,   592,
     185,   682,   683,   593,   245,   246,   168,   169,   554,   703,
     494,   655,   707,   170,   196,   731,   220,   566,   490,   492,
     773,   510,   495,   496,   498,   627,   628,   630,   171,   172,
     173,    67,   765,   766,   767,   245,   246,   190,   191,   520,
     521,   522,   783,   391,   392,   376,   541,   542,   543,   262,
     606,   245,   246,   607,   608,   609,   610,    82,   197,    83,
      84,    85,    86,    87,    88,    89,   556,   557,   558,   206,
      90,   660,   198,   675,   170,   661,   676,   764,   677,   678,
     199,   565,   210,    91,   567,   568,    92,   234,   235,   236,
     218,   573,     4,     5,     6,     7,   321,   322,   323,   100,
     101,   102,   103,    12,    13,    14,   663,    15,    16,    17,
     661,   585,   237,   728,   226,   227,   633,   661,   183,   184,
     104,    27,   586,   469,   470,   595,   596,   640,    32,   281,
     227,   598,   354,    37,    38,    39,   485,   227,    42,   316,
     317,   318,   599,   227,   380,   360,   291,   386,    46,   291,
     489,   227,   264,     4,     5,     6,     7,   290,   265,   390,
     100,   101,   102,   103,    12,    13,    14,   266,    15,    16,
      17,   290,   689,   394,   290,   290,   411,   413,   631,   574,
     575,   104,    27,   267,   505,    50,   506,   284,   505,    32,
     531,   696,    51,   288,    37,    38,    39,   268,   105,    42,
     106,   227,   283,   539,    54,   227,   227,   552,   555,    46,
     227,   645,   559,   583,   392,   649,   650,   651,   561,   227,
     562,   563,   564,   470,   291,   569,   657,   289,   307,   308,
     659,   310,   311,   312,   313,   314,   315,   687,   319,   320,
     174,   175,   176,   177,   178,   179,    50,   505,   227,   578,
     581,    92,   180,    51,   287,   694,   695,   292,   227,   130,
     582,    53,   505,   338,   614,    54,   181,   182,   183,   184,
     505,   561,   615,   624,   698,   699,   227,   639,   625,   291,
     348,   290,   705,   646,   347,   227,   711,   693,   635,   227,
      82,   715,    83,    84,    85,    86,    87,    88,    89,   227,
     227,   732,   737,    90,   505,   505,   741,   748,   733,   734,
     735,   505,   339,   782,   647,   392,   427,   738,   392,    92,
     697,   227,   340,   746,   144,   146,   328,   329,   341,   342,
     343,   349,   350,    67,   352,   351,   353,   758,   357,   359,
     378,   379,   762,   395,   407,   409,   412,   414,   422,   423,
     430,   435,   428,   436,   440,   188,   437,   438,   439,   442,
     448,   771,   772,   441,   449,   450,   769,     4,     5,     6,
       7,   457,   459,   458,   293,   294,   295,   296,    12,    13,
      14,   460,    15,    16,    17,    18,    19,    20,   297,   298,
      23,   461,   466,    24,    25,    26,    27,   299,    29,    30,
      31,   471,   481,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,   482,   483,   484,    44,    45,
     487,   488,   511,    46,   262,   512,   516,    47,   523,   524,
     517,   519,   518,   540,   587,   551,   553,   588,   554,   589,
     590,   572,   591,   602,   576,   579,   580,   600,    48,    49,
     601,   611,   616,   617,   618,   622,   621,   626,   627,   628,
      50,   629,   630,   632,   634,   636,   637,    51,   638,   642,
     643,   679,   680,    52,   658,    53,   681,   684,   685,    54,
     300,     4,     5,     6,     7,   690,   686,   691,     8,     9,
      10,    11,    12,    13,    14,   692,    15,    16,    17,    18,
      19,    20,   507,   508,    23,   700,   701,    24,    25,    26,
      27,    28,    29,    30,    31,   704,   708,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,   709,
     710,   712,    44,    45,   713,   714,   742,    46,   661,   755,
     726,    47,   727,   736,   749,   750,   751,   756,   759,   760,
     752,   753,   754,   757,   761,   768,   775,   776,   549,   777,
     778,   779,    48,    49,   780,   525,   468,   326,     0,   327,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,    52,     0,    53,
       0,     0,     0,    54,   509,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,    20,   743,   744,    23,     0,
       0,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     0,     0,     0,    44,    45,     0,     0,
       0,    46,     0,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,    52,     0,    53,     0,     0,     0,    54,   745,     4,
       5,     6,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,    22,    23,     0,     0,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     0,     0,     0,
      44,    45,     0,     0,     0,    46,     0,     0,     0,    47,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
      48,    49,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,    50,     0,     0,     0,    32,     0,     0,    51,
       0,    37,    38,    39,     0,    52,    42,    53,     0,     0,
       0,    54,     0,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   161,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   207,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   216,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   223,
       0,     0,    54,     0,     0,     0,    46,     0,     0,   163,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   425,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   443,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   446,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,   453,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   451,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,   365,     7,     0,     0,     0,   100,   101,   102,
     103,   366,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   454,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,   453,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   161,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   548,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,   688,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   116,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   118,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   120,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   122,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   137,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   139,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,    53,     0,
       0,     0,   145,     0,     0,     0,    46,     0,     0,     0,
       4,     5,   365,     7,     0,     0,     0,   100,   101,   102,
     103,   366,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   363,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   367,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   100,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,    27,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   473,     0,
       0,     0,    54,     0,     0,     0,    46,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   158,   101,   102,
     103,    12,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,    50,     0,     0,    32,     0,     0,     0,
      51,    37,    38,    39,     0,     0,    42,     0,   493,     0,
       0,     0,    54,     0,  -308,  -308,    46,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,     0,     0,     0,  -308,
       0,     0,     0,     0,     0,     0,     0,  -308,  -308,  -308,
       0,     0,     0,  -308,  -308,  -308,  -308,  -308,  -308,     0,
    -308,  -308,  -308,    50,  -308,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,    53,     0,
       0,     0,    54
};

static const yytype_int16 yycheck[] =
{
       2,     0,   490,   391,   237,   173,    14,    10,    14,    14,
       2,    14,    26,   284,    25,    70,    22,    22,     0,    21,
      22,    23,    24,    25,    92,    70,    28,    29,    95,    31,
      98,    14,    34,    35,    26,    27,    14,    39,   106,    17,
      42,    26,   109,    10,    36,    10,   101,    14,    40,    41,
     113,    53,    58,    14,    50,    51,    41,   605,    10,    10,
      37,    38,    14,    14,    97,   110,    26,    10,    10,    10,
     115,    14,    14,    14,    57,    53,    64,    65,    66,    97,
      95,    41,   115,    85,    86,    87,    88,    89,    90,    95,
      97,    93,    53,    95,    96,    60,    61,   115,   586,    97,
     110,   115,   112,   109,   106,   376,    14,   115,   115,   112,
     115,    14,   104,    14,   116,    14,   118,   115,   120,    22,
     122,    22,    70,    31,   109,   110,    51,    52,   116,   121,
      55,   123,   124,   681,    10,   137,   115,   139,    67,   116,
     142,   143,   144,   145,   146,   112,   138,   115,   140,   109,
     110,   422,   423,   101,   702,    58,   116,    58,   706,    12,
     112,   112,   164,   165,   166,   167,   168,   169,   401,   112,
     112,   112,   164,    51,    52,   186,   187,    55,   246,   108,
      14,     5,   111,   251,    60,    61,    12,   420,    22,   257,
      14,   116,    95,   741,    95,   583,   192,   193,   194,   195,
     748,    14,    68,    69,    26,    70,   109,   109,   109,    97,
     698,    68,    69,    95,   116,   763,    95,   219,   220,    41,
     222,   110,   290,   112,    58,   227,   228,   115,   230,   111,
     232,    14,   111,   108,   782,   101,   111,    26,   116,   241,
     242,   243,   244,   476,   101,   110,   248,   249,   259,   260,
     115,    26,    41,    10,    29,    30,    26,    14,    95,    29,
      30,    95,    67,   262,   266,   267,    41,    42,   270,   337,
     272,    41,   440,   506,   111,   109,    26,   345,    10,    29,
      30,    14,    14,   351,    17,   353,    95,   109,   110,   281,
      14,    41,    42,    94,   116,   297,   298,   299,   531,   532,
     109,    26,    10,   108,    29,    30,   111,   111,   109,   110,
      26,   115,   380,    29,    30,   109,    41,   109,   386,   111,
     109,   110,   116,    95,    95,    41,    42,   116,    10,    10,
      11,    12,    26,    14,   109,    29,    30,   109,   109,   109,
     110,   116,   344,    92,    95,   578,   116,    41,    26,    62,
      31,    29,    30,    95,    35,   104,   105,   106,   109,   109,
      10,   363,   109,    41,    14,   367,   116,   109,   370,   116,
     372,    26,   374,   441,    29,    30,   378,   379,    93,    10,
      91,   614,   615,    14,   109,   110,    41,    42,   109,   660,
     392,   116,   663,   109,   108,   116,   113,   465,   390,   391,
     116,   403,   394,   395,   396,   109,   109,   109,    77,    78,
      79,   403,   116,   116,   116,   109,   110,   102,   103,   411,
     412,   413,   116,    27,    28,   111,   428,   429,   430,   115,
      10,   109,   110,    13,    14,    15,    16,    82,    14,    84,
      85,    86,    87,    88,    89,    90,   448,   449,   450,    14,
      95,   111,   109,    10,   109,   115,    13,   728,    15,    16,
     109,   463,    14,   108,   466,   467,   111,    10,    11,    12,
     109,   473,     3,     4,     5,     6,   183,   184,   185,    10,
      11,    12,    13,    14,    15,    16,   111,    18,    19,    20,
     115,   493,    14,   111,   109,   110,   564,   115,    99,   100,
      31,    32,   494,   109,   110,   507,   508,   575,    39,    48,
     110,   513,   112,    44,    45,    46,   109,   110,    49,     7,
       8,     9,   514,   110,   110,   112,   112,   110,    59,   112,
     109,   110,   109,     3,     4,     5,     6,   110,   109,   112,
      10,    11,    12,    13,    14,    15,    16,    95,    18,    19,
      20,   110,   620,   112,   110,   110,   112,   112,   560,   109,
     110,    31,    32,    95,   110,    96,   112,   111,   110,    39,
     112,   639,   103,   109,    44,    45,    46,   115,   109,    49,
     111,   110,   115,   112,   115,   110,   110,   112,   112,    59,
     110,   583,   112,    27,    28,   587,   588,   589,   110,   110,
     112,   112,   110,   110,   112,   112,   598,   109,   171,   172,
     602,   174,   175,   176,   177,   178,   179,   619,   181,   182,
      71,    72,    73,    74,    75,    76,    96,   110,   110,   112,
     112,   111,    83,   103,   115,   637,   638,   116,   110,   109,
     112,   111,   110,   109,   112,   115,    97,    98,    99,   100,
     110,   110,   112,   112,   646,   647,   110,   110,   112,   112,
      98,   110,   661,   112,   114,   110,   668,   112,   109,   110,
      82,   673,    84,    85,    86,    87,    88,    89,    90,   110,
     110,   112,   112,    95,   110,   110,   112,   112,   690,   691,
     692,   110,   109,   112,    27,    28,   108,    27,    28,   111,
     109,   110,   109,   705,    38,    39,   190,   191,   109,   109,
     109,   116,   109,   705,    14,   111,   111,   719,    14,   109,
      95,    95,   724,    10,    14,    14,    27,   116,   111,   111,
      95,   109,   113,   109,    80,   101,   109,   109,   109,    14,
      95,   743,   744,   111,    95,    95,   738,     3,     4,     5,
       6,   114,   116,    98,    10,    11,    12,    13,    14,    15,
      16,   109,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   109,    67,    29,    30,    31,    32,    33,    34,    35,
      36,   109,   109,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   109,   109,   109,    54,    55,
     109,   109,   109,    59,   115,   109,    98,    63,    14,    14,
     116,   116,    98,    14,    10,   109,   109,    10,   109,    10,
      10,   109,    10,    27,   109,   109,   109,   109,    84,    85,
     109,    51,    14,    14,    14,   116,   114,   109,   109,   109,
      96,   109,   109,    14,   109,   109,    67,   103,    67,   109,
     109,   109,   109,   109,   116,   111,    14,     7,     7,   115,
     116,     3,     4,     5,     6,    95,     7,    95,    10,    11,
      12,    13,    14,    15,    16,    67,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    10,    10,    29,    30,    31,
      32,    33,    34,    35,    36,    62,    14,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    14,
     109,    14,    54,    55,    14,   109,   116,    59,   115,    14,
     109,    63,   109,   109,   109,   109,   109,    14,    14,    14,
     109,   109,   109,   109,   109,   109,   109,   109,   441,   109,
     109,   109,    84,    85,   109,   418,   367,   188,    -1,   189,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,    -1,   115,   116,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,    -1,   115,   116,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    96,    -1,    -1,    -1,    39,    -1,    -1,   103,
      -1,    44,    45,    46,    -1,   109,    49,   111,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    62,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    59,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    96,    -1,    -1,    39,    -1,    -1,    -1,
     103,    44,    45,    46,    -1,    -1,    49,    -1,   111,    -1,
      -1,    -1,   115,    -1,    68,    69,    59,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,   106,    96,   108,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,   115
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   118,   119,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    29,    30,    31,    32,    33,    34,
      35,    36,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    54,    55,    59,    63,    84,    85,
      96,   103,   109,   111,   115,   120,   121,   123,   124,   125,
     126,   127,   128,   130,   133,   134,   137,   142,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   158,    82,    84,    85,    86,    87,    88,    89,    90,
      95,   108,   111,   113,    97,   115,    95,   108,   111,    14,
      10,    11,    12,    13,    31,   109,   111,   142,   144,   157,
      10,    11,    12,    14,    31,    35,   111,   144,   111,   144,
     111,   144,   111,   144,   144,   150,    14,   142,   142,   144,
     109,   144,    10,    14,   144,    12,    12,   111,   144,   111,
     144,   142,   115,   143,   143,   115,   143,   144,   142,   142,
     144,    14,    14,    14,    31,    14,    10,    10,    10,   154,
     154,   112,   144,    62,   119,    26,    29,    30,    41,    42,
     109,    77,    78,    79,    71,    72,    73,    74,    75,    76,
      83,    97,    98,    99,   100,    91,    68,    69,   101,    93,
     102,   103,    92,   104,   105,   106,   108,    14,   109,   109,
     144,   144,   144,   144,   144,   144,    14,   112,   157,   144,
      14,   144,   144,    14,    17,    53,   112,   157,   109,    95,
     113,    97,   115,   112,   144,   157,   109,   110,    95,   109,
      95,   109,    95,   109,    10,    11,    12,    14,    10,    14,
     144,    26,    29,    30,    41,   109,   110,   144,    26,    41,
     109,   110,   144,   142,   144,   142,    70,   110,   142,    68,
      69,   111,   115,   131,   109,   109,    95,    95,   115,    97,
     115,    97,   115,   144,   142,   144,   142,   144,   144,   144,
     144,    48,   135,   115,   111,    14,    57,   115,   109,   109,
     110,   112,   116,    10,    11,    12,    13,    24,    25,    33,
     116,   144,   144,   144,   144,   144,   144,   147,   147,   145,
     147,   147,   147,   147,   147,   147,     7,     8,     9,   147,
     147,   149,   149,   149,   150,   150,   151,   152,   153,   153,
     154,   154,   154,   154,    14,    17,   109,   111,   109,   109,
     109,   109,   109,   109,    95,   111,   112,   114,    98,   116,
     109,   111,    14,   111,   112,   144,   144,    14,   144,   109,
     112,   144,   144,   111,   144,     5,    14,   111,   140,   144,
      95,   109,    95,   109,    95,   109,   111,   131,    95,    95,
     110,   144,   144,   144,   144,   157,   110,   144,   144,   157,
     112,    27,    28,   122,   112,    10,   157,   150,   150,    10,
      14,   112,   141,   119,   144,   144,   129,    14,   144,    14,
     144,   112,    27,   112,   116,   142,    14,    22,   115,   136,
     112,   141,   111,   111,   138,   112,   157,   108,   113,   115,
      95,   144,   144,   144,   116,   109,   109,   109,   109,   109,
      80,   111,    14,   112,   157,   144,   112,   157,    95,    95,
      95,   112,   157,    14,   112,   139,   157,   114,    98,   116,
     109,   109,   144,    94,   109,   110,    67,    67,   140,   109,
     110,   109,   144,   111,   144,   144,   112,   141,   144,   144,
     157,   109,   109,   109,   109,   109,   157,   109,   109,   109,
     142,   121,   142,   111,   144,   142,   142,    70,   142,    70,
      70,    10,    60,    61,   131,   110,   112,    24,    25,   116,
     144,   109,   109,    37,    38,   116,    98,   116,    98,   116,
     142,   142,   142,    14,    14,   136,    51,    52,    55,   116,
     131,   112,   112,   141,   141,    64,    65,    66,   116,   112,
      14,   144,   144,   144,   116,   116,   116,   145,   112,   139,
     157,   109,   112,   109,   109,   112,   144,   144,   144,   112,
      67,   110,   112,   112,   110,   144,   157,   144,   144,   112,
       5,    14,   109,   144,   109,   110,   109,   131,   112,   109,
     109,   112,   112,    27,   122,   144,   142,    10,    10,    10,
      10,    10,    10,    14,   131,   144,   144,   116,   144,   142,
     109,   109,    27,   116,    14,    53,    10,    13,    14,    15,
      16,    51,   131,   131,   112,   112,    14,    14,    14,    95,
     111,   114,   116,   116,   112,   112,   109,   109,   109,   109,
     109,   144,    14,   157,   109,   109,   109,    67,    67,   110,
     157,   131,   109,   109,   121,   142,   112,    27,   122,   142,
     142,   142,    10,    60,    61,   116,   116,   142,   116,   142,
     111,   115,   132,   111,   132,    14,    22,    58,    95,   109,
      14,    22,    58,    95,   109,    10,    13,    15,    16,   109,
     109,    14,   131,   131,     7,     7,     7,   144,   112,   157,
      95,    95,    67,   112,   144,   144,   157,   109,   142,   142,
      10,    10,   112,   141,    62,   119,   112,   141,    14,    14,
     109,   144,    14,    14,   109,   144,    14,    22,    58,    95,
     109,    14,    22,    58,    95,   109,   109,   109,   111,   132,
     116,   116,   112,   144,   144,   144,   109,   112,    27,   122,
     132,   112,   116,    24,    25,   116,   144,   132,   112,   109,
     109,   109,   109,   109,   109,    14,    14,   109,   144,    14,
      14,   109,   144,   112,   141,   116,   116,   116,   109,   142,
     132,   144,   144,   116,   132,   109,   109,   109,   109,   109,
     109,   132,   112,   116,   116,   132
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   117,   118,   119,   119,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   121,   121,   121,   121,   121,   121,   121,   121,
     122,   122,   122,   122,   122,   122,   123,   123,   124,   124,
     124,   124,   125,   125,   126,   127,   127,   127,   127,   127,
     127,   128,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   131,   131,
     132,   132,   132,   132,   132,   133,   134,   135,   135,   135,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   138,   138,   138,   138,   139,
     139,   140,   140,   140,   140,   141,   141,   141,   141,   141,
     141,   141,   141,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   143,   144,   144,   144,   145,
     145,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   147,   148,   148,
     149,   149,   149,   150,   150,   151,   151,   152,   152,   152,
     153,   153,   153,   153,   153,   154,   154,   154,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   157,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     7,     9,     5,     5,     7,     6,     6,     8,    10,
       6,     4,     4,     4,     3,     5,     5,     6,     6,     2,
       5,     4,     3,     3,     5,     7,     3,     5,     7,     3,
       3,     2,     4,     7,     6,     4,     6,     4,     7,     6,
       7,     7,     7,     6,     6,     4,     4,     4,     4,     4,
       5,     5,     5,     5,     5,     5,     4,     4,     4,     4,
       4,     3,     3,     3,     3,     2,     1,     1,     1,     1,
       1,     2,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     7,     7,     3,     5,     5,     4,     6,
       3,     5,     5,     7,     4,     6,     5,     3,     5,     7,
       3,     5,     5,     3,     2,     7,     7,     5,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     3,     7,     6,
       4,     7,     6,     7,     6,     5,     3,     4,     5,     5,
       3,     3,     4,     5,     5,     6,     5,     0,     3,     3,
       0,     4,     4,     4,     5,     5,     4,     5,     5,     6,
       6,     7,     7,     6,     6,     6,     6,     5,     5,     6,
       6,     7,     7,     7,     7,     7,     6,     4,     7,     6,
       4,     8,     7,     5,     5,     0,     4,     4,     4,     3,
       5,     3,     3,     5,     5,     1,     2,     3,     3,     3,
       4,     5,     5,     3,     3,     4,     5,     5,     9,     8,
       8,     6,     9,     9,     5,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     2,     2,     1,     1,     4,
       3,     4,     3,     6,     6,     5,     3,     3,     6,     5,
       4,     4,     6,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     5,     5,
       1,     1,     1,     1,     2,     3,     4,     5,     1,     2
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
#line 261 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2312 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 271 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2318 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 272 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2324 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 276 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2330 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 278 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2336 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 280 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2342 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 282 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2348 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 284 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2354 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 286 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2362 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 290 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2369 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 293 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2377 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 297 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2383 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 299 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2389 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 301 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2395 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2401 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 305 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2407 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 307 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2415 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 311 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2423 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 315 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2429 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 317 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2435 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 319 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2441 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 321 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2447 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 323 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2453 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 325 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2459 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 327 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2465 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 329 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2471 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 331 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2477 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 333 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2483 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 335 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2489 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 337 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2495 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 339 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2501 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 341 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2507 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 343 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2514 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 346 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2521 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 349 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2527 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 351 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2534 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 354 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2541 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 357 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2547 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 359 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2553 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 361 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2559 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 363 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2565 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 365 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2574 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 370 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2582 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 374 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2590 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 378 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2599 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 383 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2607 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 387 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2618 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 394 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2627 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2634 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2641 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2648 "raku.tab.c"
    break;

  case 53: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 408 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2655 "raku.tab.c"
    break;

  case 54: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 411 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2662 "raku.tab.c"
    break;

  case 55: /* stmt: expr KW_IF expr ';'  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2668 "raku.tab.c"
    break;

  case 56: /* stmt: expr KW_UNLESS expr ';'  */
#line 416 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2674 "raku.tab.c"
    break;

  case 57: /* stmt: expr KW_WHILE expr ';'  */
#line 418 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2680 "raku.tab.c"
    break;

  case 58: /* stmt: expr KW_UNTIL expr ';'  */
#line 420 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2686 "raku.tab.c"
    break;

  case 59: /* stmt: expr KW_FOR expr ';'  */
#line 422 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2693 "raku.tab.c"
    break;

  case 60: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 425 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 2700 "raku.tab.c"
    break;

  case 61: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 428 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 2707 "raku.tab.c"
    break;

  case 62: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 431 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 2715 "raku.tab.c"
    break;

  case 63: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 435 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 2722 "raku.tab.c"
    break;

  case 64: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 438 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 2729 "raku.tab.c"
    break;

  case 65: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 441 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 2736 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 444 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 2743 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 447 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 2750 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 450 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 2757 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 453 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 2764 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 456 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 2771 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 459 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 2778 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 462 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 2785 "raku.tab.c"
    break;

  case 73: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 465 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 2792 "raku.tab.c"
    break;

  case 74: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 468 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 2799 "raku.tab.c"
    break;

  case 75: /* stmt: expr ';'  */
#line 470 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2805 "raku.tab.c"
    break;

  case 76: /* stmt: ';'  */
#line 471 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 2811 "raku.tab.c"
    break;

  case 77: /* stmt: if_stmt  */
#line 472 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2817 "raku.tab.c"
    break;

  case 78: /* stmt: while_stmt  */
#line 473 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2823 "raku.tab.c"
    break;

  case 79: /* stmt: for_stmt  */
#line 474 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2829 "raku.tab.c"
    break;

  case 80: /* stmt: given_stmt  */
#line 475 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2835 "raku.tab.c"
    break;

  case 81: /* stmt: KW_TRY block  */
#line 477 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2841 "raku.tab.c"
    break;

  case 82: /* stmt: KW_TRY block KW_CATCH block  */
#line 479 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2847 "raku.tab.c"
    break;

  case 83: /* stmt: KW_CATCH block  */
#line 481 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2853 "raku.tab.c"
    break;

  case 84: /* stmt: block  */
#line 483 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2859 "raku.tab.c"
    break;

  case 85: /* stmt: unless_stmt  */
#line 484 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2865 "raku.tab.c"
    break;

  case 86: /* stmt: until_stmt  */
#line 485 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2871 "raku.tab.c"
    break;

  case 87: /* stmt: repeat_stmt  */
#line 486 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2877 "raku.tab.c"
    break;

  case 88: /* stmt: sub_decl  */
#line 487 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2883 "raku.tab.c"
    break;

  case 89: /* stmt: class_decl  */
#line 488 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2889 "raku.tab.c"
    break;

  case 90: /* stmt: role_decl  */
#line 489 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2895 "raku.tab.c"
    break;

  case 91: /* stmt: grammar_decl  */
#line 490 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2901 "raku.tab.c"
    break;

  case 92: /* if_stmt: KW_IF '(' expr ')' block  */
#line 494 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2907 "raku.tab.c"
    break;

  case 93: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 496 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2913 "raku.tab.c"
    break;

  case 94: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 498 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2919 "raku.tab.c"
    break;

  case 95: /* if_stmt: KW_IF expr block  */
#line 500 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2925 "raku.tab.c"
    break;

  case 96: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 502 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2931 "raku.tab.c"
    break;

  case 97: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2937 "raku.tab.c"
    break;

  case 98: /* if_stmt: KW_IF expr block elsif_tail  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2943 "raku.tab.c"
    break;

  case 99: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 508 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2949 "raku.tab.c"
    break;

  case 100: /* elsif_tail: KW_ELSIF expr block  */
#line 512 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2955 "raku.tab.c"
    break;

  case 101: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 514 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2961 "raku.tab.c"
    break;

  case 102: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 516 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2967 "raku.tab.c"
    break;

  case 103: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 518 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2973 "raku.tab.c"
    break;

  case 104: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 520 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2979 "raku.tab.c"
    break;

  case 105: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 522 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2985 "raku.tab.c"
    break;

  case 106: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 526 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2991 "raku.tab.c"
    break;

  case 107: /* while_stmt: KW_WHILE expr block  */
#line 528 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 2997 "raku.tab.c"
    break;

  case 108: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 532 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3003 "raku.tab.c"
    break;

  case 109: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 534 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3009 "raku.tab.c"
    break;

  case 110: /* unless_stmt: KW_UNLESS expr block  */
#line 536 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3015 "raku.tab.c"
    break;

  case 111: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 538 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3021 "raku.tab.c"
    break;

  case 112: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 542 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3027 "raku.tab.c"
    break;

  case 113: /* until_stmt: KW_UNTIL expr block  */
#line 544 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3033 "raku.tab.c"
    break;

  case 114: /* repeat_stmt: KW_REPEAT block  */
#line 548 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3039 "raku.tab.c"
    break;

  case 115: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 552 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3049 "raku.tab.c"
    break;

  case 116: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 558 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 3059 "raku.tab.c"
    break;

  case 117: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 564 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3068 "raku.tab.c"
    break;

  case 118: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 569 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3078 "raku.tab.c"
    break;

  case 119: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 575 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3087 "raku.tab.c"
    break;

  case 120: /* for_stmt: KW_FOR expr block  */
#line 580 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3094 "raku.tab.c"
    break;

  case 121: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 585 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3106 "raku.tab.c"
    break;

  case 122: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 593 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3119 "raku.tab.c"
    break;

  case 123: /* when_list: %empty  */
#line 603 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3125 "raku.tab.c"
    break;

  case 124: /* when_list: when_list KW_WHEN expr block  */
#line 605 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3133 "raku.tab.c"
    break;

  case 125: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 611 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3145 "raku.tab.c"
    break;

  case 126: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 619 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3155 "raku.tab.c"
    break;

  case 127: /* sub_decl: KW_SUB IDENT sub_body  */
#line 625 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3165 "raku.tab.c"
    break;

  case 128: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 631 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3177 "raku.tab.c"
    break;

  case 129: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 639 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3187 "raku.tab.c"
    break;

  case 130: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 645 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3197 "raku.tab.c"
    break;

  case 131: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 651 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3210 "raku.tab.c"
    break;

  case 132: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 660 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3221 "raku.tab.c"
    break;

  case 133: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 667 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3234 "raku.tab.c"
    break;

  case 134: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 676 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3247 "raku.tab.c"
    break;

  case 135: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 685 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3258 "raku.tab.c"
    break;

  case 136: /* sub_body: '{' stmt_list '}'  */
#line 693 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3264 "raku.tab.c"
    break;

  case 137: /* sub_body: '{' stmt_list expr '}'  */
#line 695 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3271 "raku.tab.c"
    break;

  case 138: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 698 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3277 "raku.tab.c"
    break;

  case 139: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 700 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3283 "raku.tab.c"
    break;

  case 140: /* method_body: '{' stmt_list '}'  */
#line 703 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3289 "raku.tab.c"
    break;

  case 141: /* method_body: '{' YADA '}'  */
#line 704 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3295 "raku.tab.c"
    break;

  case 142: /* method_body: '{' stmt_list expr '}'  */
#line 706 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3302 "raku.tab.c"
    break;

  case 143: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 709 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3308 "raku.tab.c"
    break;

  case 144: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 711 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3314 "raku.tab.c"
    break;

  case 145: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 715 "raku.y"
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
#line 3332 "raku.tab.c"
    break;

  case 146: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 731 "raku.y"
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
#line 3349 "raku.tab.c"
    break;

  case 147: /* is_clauses: %empty  */
#line 745 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3355 "raku.tab.c"
    break;

  case 148: /* is_clauses: is_clauses IDENT IDENT  */
#line 747 "raku.y"
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
#line 3371 "raku.tab.c"
    break;

  case 149: /* is_clauses: is_clauses TESTOP IDENT  */
#line 759 "raku.y"
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
#line 3387 "raku.tab.c"
    break;

  case 150: /* class_body_list: %empty  */
#line 772 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3393 "raku.tab.c"
    break;

  case 151: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 774 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3400 "raku.tab.c"
    break;

  case 152: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 777 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3407 "raku.tab.c"
    break;

  case 153: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 780 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3414 "raku.tab.c"
    break;

  case 154: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 783 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3421 "raku.tab.c"
    break;

  case 155: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 786 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3428 "raku.tab.c"
    break;

  case 156: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 789 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3435 "raku.tab.c"
    break;

  case 157: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 792 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3442 "raku.tab.c"
    break;

  case 158: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 795 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3449 "raku.tab.c"
    break;

  case 159: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 798 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3456 "raku.tab.c"
    break;

  case 160: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 801 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3463 "raku.tab.c"
    break;

  case 161: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 804 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3470 "raku.tab.c"
    break;

  case 162: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 807 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3477 "raku.tab.c"
    break;

  case 163: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 810 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3488 "raku.tab.c"
    break;

  case 164: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 817 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3499 "raku.tab.c"
    break;

  case 165: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 824 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3510 "raku.tab.c"
    break;

  case 166: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 831 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3521 "raku.tab.c"
    break;

  case 167: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 838 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3529 "raku.tab.c"
    break;

  case 168: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 842 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3537 "raku.tab.c"
    break;

  case 169: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 846 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3545 "raku.tab.c"
    break;

  case 170: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 850 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3553 "raku.tab.c"
    break;

  case 171: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 854 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3564 "raku.tab.c"
    break;

  case 172: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 861 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3575 "raku.tab.c"
    break;

  case 173: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 868 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3586 "raku.tab.c"
    break;

  case 174: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 875 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3597 "raku.tab.c"
    break;

  case 175: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 882 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3611 "raku.tab.c"
    break;

  case 176: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 892 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3623 "raku.tab.c"
    break;

  case 177: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 900 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3635 "raku.tab.c"
    break;

  case 178: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 908 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3648 "raku.tab.c"
    break;

  case 179: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 917 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3659 "raku.tab.c"
    break;

  case 180: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 924 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3670 "raku.tab.c"
    break;

  case 181: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 931 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3684 "raku.tab.c"
    break;

  case 182: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 941 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3696 "raku.tab.c"
    break;

  case 183: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 949 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 3708 "raku.tab.c"
    break;

  case 184: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 959 "raku.y"
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
#line 3725 "raku.tab.c"
    break;

  case 185: /* grammar_body_list: %empty  */
#line 973 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3731 "raku.tab.c"
    break;

  case 186: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 975 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3740 "raku.tab.c"
    break;

  case 187: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 980 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3749 "raku.tab.c"
    break;

  case 188: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 985 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3758 "raku.tab.c"
    break;

  case 189: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 992 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3766 "raku.tab.c"
    break;

  case 190: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 996 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3774 "raku.tab.c"
    break;

  case 191: /* pair_list: IDENT OP_FATARROW expr  */
#line 1002 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3780 "raku.tab.c"
    break;

  case 192: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1004 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3786 "raku.tab.c"
    break;

  case 193: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1006 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3792 "raku.tab.c"
    break;

  case 194: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1008 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3798 "raku.tab.c"
    break;

  case 195: /* param_list: VAR_SCALAR  */
#line 1011 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3804 "raku.tab.c"
    break;

  case 196: /* param_list: IDENT VAR_SCALAR  */
#line 1012 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3810 "raku.tab.c"
    break;

  case 197: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1013 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3816 "raku.tab.c"
    break;

  case 198: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1014 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3822 "raku.tab.c"
    break;

  case 199: /* param_list: param_list ',' VAR_SCALAR  */
#line 1015 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3828 "raku.tab.c"
    break;

  case 200: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1016 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3834 "raku.tab.c"
    break;

  case 201: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1017 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3840 "raku.tab.c"
    break;

  case 202: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1018 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3846 "raku.tab.c"
    break;

  case 203: /* block: '{' stmt_list '}'  */
#line 1021 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3852 "raku.tab.c"
    break;

  case 204: /* block: '{' YADA '}'  */
#line 1022 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3858 "raku.tab.c"
    break;

  case 205: /* block: '{' stmt_list expr '}'  */
#line 1024 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 3864 "raku.tab.c"
    break;

  case 206: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1026 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3870 "raku.tab.c"
    break;

  case 207: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1028 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3876 "raku.tab.c"
    break;

  case 208: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1030 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3884 "raku.tab.c"
    break;

  case 209: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1034 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3891 "raku.tab.c"
    break;

  case 210: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1037 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 3898 "raku.tab.c"
    break;

  case 211: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1040 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 3905 "raku.tab.c"
    break;

  case 212: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1043 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3912 "raku.tab.c"
    break;

  case 213: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1046 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3919 "raku.tab.c"
    break;

  case 214: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1049 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 3925 "raku.tab.c"
    break;

  case 215: /* closure: '{' expr '}'  */
#line 1052 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3931 "raku.tab.c"
    break;

  case 216: /* expr: VAR_SCALAR '=' expr  */
#line 1055 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3937 "raku.tab.c"
    break;

  case 217: /* expr: KW_GATHER block  */
#line 1056 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3947 "raku.tab.c"
    break;

  case 218: /* expr: tern_expr  */
#line 1061 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3953 "raku.tab.c"
    break;

  case 219: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1065 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3959 "raku.tab.c"
    break;

  case 220: /* tern_expr: cmp_expr  */
#line 1066 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3965 "raku.tab.c"
    break;

  case 221: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1069 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3971 "raku.tab.c"
    break;

  case 222: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1070 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3977 "raku.tab.c"
    break;

  case 223: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 1071 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3983 "raku.tab.c"
    break;

  case 224: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 1072 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3989 "raku.tab.c"
    break;

  case 225: /* cmp_expr: jct_expr '<' jct_expr  */
#line 1073 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3995 "raku.tab.c"
    break;

  case 226: /* cmp_expr: jct_expr '>' jct_expr  */
#line 1074 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4001 "raku.tab.c"
    break;

  case 227: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 1075 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4007 "raku.tab.c"
    break;

  case 228: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 1076 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4013 "raku.tab.c"
    break;

  case 229: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1077 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4019 "raku.tab.c"
    break;

  case 230: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1078 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4025 "raku.tab.c"
    break;

  case 231: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1080 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4035 "raku.tab.c"
    break;

  case 232: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1086 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4045 "raku.tab.c"
    break;

  case 233: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1092 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4055 "raku.tab.c"
    break;

  case 234: /* cmp_expr: jct_expr  */
#line 1097 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4061 "raku.tab.c"
    break;

  case 235: /* jct_expr: jct_expr '|' range_expr  */
#line 1100 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4067 "raku.tab.c"
    break;

  case 236: /* jct_expr: jct_expr '&' range_expr  */
#line 1101 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4073 "raku.tab.c"
    break;

  case 237: /* jct_expr: dor_expr  */
#line 1102 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4079 "raku.tab.c"
    break;

  case 238: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1106 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4085 "raku.tab.c"
    break;

  case 239: /* dor_expr: range_expr  */
#line 1107 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4091 "raku.tab.c"
    break;

  case 240: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1110 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4097 "raku.tab.c"
    break;

  case 241: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1111 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4103 "raku.tab.c"
    break;

  case 242: /* range_expr: add_expr  */
#line 1112 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4109 "raku.tab.c"
    break;

  case 243: /* add_expr: add_expr '~' repl_expr  */
#line 1115 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4115 "raku.tab.c"
    break;

  case 244: /* add_expr: repl_expr  */
#line 1116 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4121 "raku.tab.c"
    break;

  case 245: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1119 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4127 "raku.tab.c"
    break;

  case 246: /* repl_expr: addsub_expr  */
#line 1120 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4133 "raku.tab.c"
    break;

  case 247: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1123 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4139 "raku.tab.c"
    break;

  case 248: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1124 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4145 "raku.tab.c"
    break;

  case 249: /* addsub_expr: mul_expr  */
#line 1125 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4151 "raku.tab.c"
    break;

  case 250: /* mul_expr: mul_expr '*' unary_expr  */
#line 1128 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4157 "raku.tab.c"
    break;

  case 251: /* mul_expr: mul_expr '/' unary_expr  */
#line 1129 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4163 "raku.tab.c"
    break;

  case 252: /* mul_expr: mul_expr '%' unary_expr  */
#line 1130 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4169 "raku.tab.c"
    break;

  case 253: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1131 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4175 "raku.tab.c"
    break;

  case 254: /* mul_expr: unary_expr  */
#line 1132 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4181 "raku.tab.c"
    break;

  case 255: /* unary_expr: '-' unary_expr  */
#line 1135 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4187 "raku.tab.c"
    break;

  case 256: /* unary_expr: '!' unary_expr  */
#line 1136 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4193 "raku.tab.c"
    break;

  case 257: /* unary_expr: postfix_expr  */
#line 1137 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4199 "raku.tab.c"
    break;

  case 258: /* postfix_expr: call_expr  */
#line 1139 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4205 "raku.tab.c"
    break;

  case 259: /* call_expr: IDENT '(' arg_list ')'  */
#line 1142 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4214 "raku.tab.c"
    break;

  case 260: /* call_expr: IDENT '(' ')'  */
#line 1146 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4220 "raku.tab.c"
    break;

  case 261: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1148 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4229 "raku.tab.c"
    break;

  case 262: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1153 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4235 "raku.tab.c"
    break;

  case 263: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1155 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4245 "raku.tab.c"
    break;

  case 264: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1161 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4255 "raku.tab.c"
    break;

  case 265: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1167 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4263 "raku.tab.c"
    break;

  case 266: /* call_expr: IDENT '.' KW_NEW  */
#line 1171 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4271 "raku.tab.c"
    break;

  case 267: /* call_expr: IDENT '.' IDENT  */
#line 1175 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4280 "raku.tab.c"
    break;

  case 268: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1180 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4291 "raku.tab.c"
    break;

  case 269: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1187 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4300 "raku.tab.c"
    break;

  case 270: /* call_expr: IDENT '.' CARET IDENT  */
#line 1192 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4310 "raku.tab.c"
    break;

  case 271: /* call_expr: atom '.' CARET IDENT  */
#line 1198 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4320 "raku.tab.c"
    break;

  case 272: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1204 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4331 "raku.tab.c"
    break;

  case 273: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1211 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4342 "raku.tab.c"
    break;

  case 274: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1218 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4351 "raku.tab.c"
    break;

  case 275: /* call_expr: atom '.' IDENT  */
#line 1223 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4360 "raku.tab.c"
    break;

  case 276: /* call_expr: KW_DIE expr  */
#line 1228 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 4366 "raku.tab.c"
    break;

  case 277: /* call_expr: KW_MAP closure expr  */
#line 1230 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4372 "raku.tab.c"
    break;

  case 278: /* call_expr: KW_GREP closure expr  */
#line 1232 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4378 "raku.tab.c"
    break;

  case 279: /* call_expr: KW_SORT expr  */
#line 1234 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4384 "raku.tab.c"
    break;

  case 280: /* call_expr: KW_SORT closure expr  */
#line 1236 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4390 "raku.tab.c"
    break;

  case 281: /* call_expr: atom  */
#line 1237 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4396 "raku.tab.c"
    break;

  case 282: /* arg_list: expr  */
#line 1240 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4402 "raku.tab.c"
    break;

  case 283: /* arg_list: arg_list ',' expr  */
#line 1241 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4408 "raku.tab.c"
    break;

  case 284: /* atom: LIT_INT  */
#line 1244 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4414 "raku.tab.c"
    break;

  case 285: /* atom: LIT_FLOAT  */
#line 1245 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4420 "raku.tab.c"
    break;

  case 286: /* atom: LIT_STR  */
#line 1246 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4426 "raku.tab.c"
    break;

  case 287: /* atom: WORDLIST  */
#line 1248 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4438 "raku.tab.c"
    break;

  case 288: /* atom: LIT_INTERP_STR  */
#line 1255 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4444 "raku.tab.c"
    break;

  case 289: /* atom: VAR_SCALAR  */
#line 1256 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4450 "raku.tab.c"
    break;

  case 290: /* atom: VAR_ARRAY  */
#line 1257 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4456 "raku.tab.c"
    break;

  case 291: /* atom: VAR_HASH  */
#line 1258 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4462 "raku.tab.c"
    break;

  case 292: /* atom: VAR_CAPTURE  */
#line 1260 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4470 "raku.tab.c"
    break;

  case 293: /* atom: VAR_FH  */
#line 1264 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4478 "raku.tab.c"
    break;

  case 294: /* atom: VAR_NAMED_CAPTURE  */
#line 1268 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4485 "raku.tab.c"
    break;

  case 295: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1271 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4491 "raku.tab.c"
    break;

  case 296: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1273 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4497 "raku.tab.c"
    break;

  case 297: /* atom: VAR_HASH '{' expr '}'  */
#line 1275 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4503 "raku.tab.c"
    break;

  case 298: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1277 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4509 "raku.tab.c"
    break;

  case 299: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1279 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4515 "raku.tab.c"
    break;

  case 300: /* atom: IDENT  */
#line 1280 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4521 "raku.tab.c"
    break;

  case 301: /* atom: VAR_TWIGIL  */
#line 1282 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4529 "raku.tab.c"
    break;

  case 302: /* atom: VAR_ARRAY_TWIGIL  */
#line 1286 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4537 "raku.tab.c"
    break;

  case 303: /* atom: VAR_HASH_TWIGIL  */
#line 1290 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4545 "raku.tab.c"
    break;

  case 304: /* atom: '(' ')'  */
#line 1293 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 4551 "raku.tab.c"
    break;

  case 305: /* atom: '(' expr ')'  */
#line 1294 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 4557 "raku.tab.c"
    break;

  case 306: /* atom: '(' expr ',' ')'  */
#line 1296 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 4563 "raku.tab.c"
    break;

  case 307: /* atom: '(' expr ',' arg_list ')'  */
#line 1298 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 4570 "raku.tab.c"
    break;

  case 308: /* atom: block  */
#line 1300 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4576 "raku.tab.c"
    break;

  case 309: /* atom: KW_SUB block  */
#line 1301 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4582 "raku.tab.c"
    break;


#line 4586 "raku.tab.c"

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

#line 1303 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
